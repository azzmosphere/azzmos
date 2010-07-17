/*
 * =====================================================================================
 *
 *       Filename:  DBSQLHandle.c
 *
 *    Description:  This is the primary database interface for postgresql,  all database
 *                  functions belong in here.
 *
 *        Version:  1.0
 *        Created:  17/06/2010 22:17:49
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */

#include <DBSQLHandle.h>

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  GetConnStr
 *  Description:  Read database configuration file and return a the string to connect
 *                to database.
 * =====================================================================================
 */
static const char *
GetConnStr( const char *dbconf )
{
	char *connstr = strdup("");
	FILE *f = fopen( dbconf, "r");
	char *buf = (char *) malloc( (sizeof(char) * BUFSIZ));
	int len = 0;

	if( f == NULL ) {
		SYSLOG_ERR( "GetConnStr", "could not open dbconf", errno);
		return NULL;
	}

	flockfile(f);
	while( !feof(f) ) {
		buf = fgets( buf, BUFSIZ, f);	
		if( buf != NULL) {
			len = strlen(buf) + strlen(connstr);
			connstr = (char *) realloc(connstr, len);
			strncat( connstr, buf, len );
		}
		free(buf);
	}

	buf = NULL;
	free(buf);
	funlockfile(f);	
	fclose(f);

	return connstr;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  DBSQLHandleInit
 *  Description:  Connect to database, and return handle containing the connection.
 * =====================================================================================
 */
DBObj_t *
DBSQLHandleInit(const Opts_t *opts )
{
	char *connstr;
	char *clientenc;
	char *dbconf = opts->o_dbconf;
	PGresult *result;

	DBObj_t *db = ( DBObj_t *) malloc( sizeof(DBObj_t));
	if( ! db ) {
		SYSLOG_ERR( "DBSQLHandleInit", "could not allocate memory for db connection", errno);
		return NULL;
	}

	/* Get connstr */
	if( (connstr = (char *) GetConnStr(dbconf)) == NULL) {
		syslog( LOG_ERR, "could not get the db connstr");
		db = NULL;
		free(db);
		return NULL;
	}


	/* Create connection */
	db->dbconn = PQconnectdb( connstr );

	/* Check status of connection status */
	if( PQstatus( db->dbconn ) == CONNECTION_BAD ) {
		syslog( LOG_ERR, 
			"something has gone wrong with the database connection - %s", 
			PQerrorMessage( db->dbconn)
		);
		return NULL;
	} 
	else {
		/* Check the client_encoding, we want it to be iso-8559-1 if
		 * it isn't thne try and set it otherwise it is fatal. */
		clientenc = (char *) PQparameterStatus(db->dbconn, "client_encoding");
		syslog( LOG_DEBUG, "client_encoding = '%s'", clientenc);

		if( strncmp( clientenc, opts->o_clientenc, BUFSIZ ) != 0 ) {
			const Oid paramTypes[1] = {TEXTOID};
			const char *const paramValues[1] = {opts->o_clientenc};
			syslog(LOG_DEBUG, "attempting to change client-encoding to %s", opts->o_clientenc);

			result = PQexecParams(db->dbconn, "SET client_encoding = %1;", 
					      1, 
					      paramTypes, 
					      paramValues, 
					      NULL, 
					      NULL, 
					      0 );
			if( result == NULL || PQresultStatus( result ) != PGRES_COMMAND_OK) {
				syslog( LOG_CRIT, "could not change client-encoding - %s", PQresultErrorMessage( result ));
				PQfinish( db->dbconn );
				free(db);
				db = NULL;
			}
		}
	}

	/* Set initial index value */
	db->dbidx = 0;

	return db;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  DBSQLHandleCleanUp
 *  Description:  Destroy the datbase object.
 * =====================================================================================
 */
void     
DBSQLHandleCleanUp( DBObj_t *db )
{
	if( db != NULL ) {
		PQfinish( db->dbconn );
		free(db);
	}
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  DBObj_t *DBSQLInitSth( DBObj_t *db, 
 *                                            const char *sql,
 *                                            const Oid  *paramTypes
 *                                            const char *errmsg,
 *                                            int   nParams
 *                )

 *
 *  Description: Prepare a statement handle that can be reused throughout the program.
 *               the handle should have a unique name (stmtName) than can be called
 *               at a later time and reused with the value parameters.
 *
 *               On success this routine will return the new handle on failure 
 *               the it will return NULL.
 * =====================================================================================
 */
DBSth_t *
DBSQLSthInit( DBObj_t *db, 
                       const char *sql,
                       const Oid  *paramTypes,
                       const char *errmsg,
                       int   nParams
)
{
	Oid *lparamTypes;
	int i;
	DBSth_t *sth = (DBSth_t *) malloc( sizeof(DBSth_t));

	if( ! sth ) {
		syslog( LOG_ERR, "%s - could not initlize the statment handle", errmsg);
		return NULL;
	}

	/*  clone parameter types */
	lparamTypes = ( Oid *) malloc( (sizeof( Oid ) * nParams) );
	for( i = 0; i < nParams; i ++ ) {
		lparamTypes[i] = paramTypes[i];
	}

	/* Get the index value and then increment it by 1  */
	sth->db_stmtName = itoa( db->dbidx );
	db->dbidx ++;

	/*  Set the values argued to us in the handle */
	sth->db_conn       = db;
	sth->db_nParams    = nParams;
	sth->db_paramTypes = lparamTypes;
	sth->db_errmsg     = strdup( errmsg );

	/*  Set parameters that will are a little hard coded */
	sth->db_paramLengths = NULL;
	sth->db_paramFormats = NULL;  /* parameters assumed as text */
	sth->db_resultFormat = 0;
	sth->db_rc           = NULL;  /* set initial value to NULL */

	/* Finally prepare our statement */
	i = DBSQLPrepareStatement( sth->db_conn, 
			           sth->db_stmtName, 
				   sql, 
				   sth->db_paramTypes , 
				   sth->db_errmsg, 
				   sth->db_nParams,
				   sth->db_rc);

	if( i ) {
		syslog( LOG_ERR, "%s - could not prepare statement", sth->db_errmsg);
		return NULL;
	}

	return sth;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  DBSQLSthFinit
 *  Description:  Destroy a statement handle.  Note this does not stop the name from 
 *                being part of the namespace but you can override that.
 * =====================================================================================
 */
void
DBSQLSthFinit ( DBSth_t *sth )
{
	int i = 0;

	if( ! sth ) {
		sth = NULL;
		free( sth );
		return;
	}


	sth->db_stmtName   = NULL;
	sth->db_paramLengths = sth->db_paramFormats = NULL;
   	sth->db_paramTypes = NULL;
   	sth->db_errmsg     = NULL;

	if( sth->db_rc )
		PQclear( sth->db_rc);

	free((void *) sth->db_stmtName);
	free((void *) sth->db_paramLengths);
	free((void *) sth->db_paramFormats);
	free((void *) sth->db_paramTypes);
	free((void *) sth->db_errmsg);

	sth = NULL;
	free((void *)  sth );
}		/* -----  end of function DBSQLSthFinit  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  DBSQLExecSthExec
 *  Description:  Executes the statment handle.  On success 0 is returned otherwise the 
 *                assosiated error value is returned.  Any error message will be printed
 *                to the system log.
 * =====================================================================================
 */
int
DBSQLExecSthQuery ( DBSth_t *sth, const char **paramValues )
{
	return DBSQLExecSth( sth, paramValues,  PGRES_TUPLES_OK );
}		/* -----  end of function DBSQLExecSthExec  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  DBSQLExecSthCmd
 *  Description:  Execute a command such as:
 *                INSERT, UPDATE, EXEC 
 * =====================================================================================
 */
int
DBSQLExecSthCmd ( DBSth_t *sth, const char **paramValues )
{
	return DBSQLExecSth( sth, paramValues,  PGRES_COMMAND_OK );
}		/* -----  end of function DBSQLExecSthCmd  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  DBSQLExecSth
 *  Description:  Executes the statment handle.  On success 0 is returned otherwise the 
 *                assosiated error value is returned.  Any error message will be printed
 *                to the system log.
 * =====================================================================================
 */
int
DBSQLExecSth ( DBSth_t *sth, const char **paramValues, int pgcheck )
{
	int rv = 0;
	sth->db_rc = PQexecPrepared( sth->db_conn->dbconn, 
			sth->db_stmtName, 
			sth->db_nParams, 
			paramValues, 
			sth->db_paramLengths, 
			sth->db_paramFormats, 
			sth->db_resultFormat
	);
	if( PQresultStatus( sth->db_rc ) != pgcheck ) {
		syslog(LOG_ERR, "%s - %s ", sth->db_errmsg ,PQresultErrorMessage(sth->db_rc));
		rv =  PQresultStatus( sth->db_rc );
	}

	return rv;
}		/* -----  end of function DBSQLExecSthExec  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  int  DBSQLPrepareStatement( DBObj_t *db, 
 *                                            const char *stmtName,
 *                                            const char *sql,
 *                                            const Oid  *paramTypes
 *                                            const char *errmsg,
 *                                            int   nParams
 *                )
 *
 *  Description: Prepare a statement handle that can be reused throughout the program.
 *               the handle should have a unique name (stmtName) than can be called
 *               at a later time and reused with the value parameters.
 *
 *               On success this routine will return '0' on failure '1' is returned.
 * =====================================================================================
 */
int
DBSQLPrepareStatement ( DBObj_t *db,  
		        const char *stmtName,
			const char *query,
			const Oid  *paramTypes,
			const char *errmsg,
			int         nParams,
			PGresult   *rc
)
{
	PGconn *  conn = db->dbconn;
	int       rv   = 0;

	rc = PQprepare( conn, stmtName, query, nParams, paramTypes);
	if( PQresultStatus( rc ) != PGRES_COMMAND_OK ) {
		syslog(LOG_ERR, "%s(%s) %s - %s ",
		      __FILE__, 
		      "DBSQLPrepareStatement",
		      errmsg,
		      PQresultErrorMessage(rc));
		rv = 1;
	}
	
	return rv;
}		/* -----  end of function DBSQLPrepareStatement  ----- */

