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
DBSth_t *DBSQLSthInit( DBObj_t *db, 
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
 *         Name:  DBSQLExecSthExec
 *  Description:  Executes the statment handle.  On success 0 is returned otherwise the 
 *                assosiated error value is returned.  Any error message will be printed
 *                to the system log.
 * =====================================================================================
 */
int
DBSQLExecSthExec ( DBSth_t *sth, const char **paramValues )
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
	if( PQresultStatus( sth->db_rc ) != PGRES_TUPLES_OK ) {
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



// EVERYTHING BELOW THIS LINE WILL BE REMOVED, 
// the SQL routines will need to be refactored to cope with the 
// new back end.
/**
 * Prepares the insert_footprint command so
 * that it can be executed multiple times without
 * the database having to re-plan the statement.
 *
 */
int
DBSQLPrepareAzFootprint(DBObj_t *db )
{
	PGresult *rc;
	PGconn *conn         = db->dbconn;
	const char *stmtName = DBH_SQL_AZ_FOOTPRINT;
        const char *query    = "SELECT insert_az_footprint($1,$2,$3,$4,$5);";
	int nParams          = 5, rv = 0;
	const Oid paramTypes[5] = {
		VARCHAROID,
		TEXTOID,
		TEXTOID,
		TEXTOID,
		INT4OID
	};

	rc = PQprepare( conn, stmtName, query, nParams, paramTypes);
	if( PQresultStatus( rc ) != PGRES_COMMAND_OK ) {
		syslog(LOG_ERR, "could not prepare db footprint STH - %s ", PQresultErrorMessage(rc));
		rv = 1;
	}
	
	return rv;
}


/**
 * replicate the SQL statement in the event of error
 */
inline static void 
_DebugSQL( PGconn *conn, char *s, char *a, char *p, char *c, char *r)
{
	/* check that it is not a connection error */
	if(PQstatus(conn) == CONNECTION_BAD )
		return;

	FILE *f = fopen("az_sqldump.sql", "w");
	int error;
	char *se = (char *) malloc(strnlen(s, BUFSIZ) ), 
	     *ae = (char *) malloc(strnlen(a, BUFSIZ) ), 
	     *pe = (char *) malloc(strnlen(p, BUFSIZ) ), 
	     *re = (char *) malloc(strnlen(r, BUFSIZ) ), 
	     *ce = (char *) malloc(strnlen(c, BUFSIZ) ),
	     *sql; 
	PQescapeStringConn (conn, se, s, strlen(s), &error);
	PQescapeStringConn (conn, ae, a, strlen(a), &error);
	PQescapeStringConn (conn, pe, p, strlen(p), &error);
	PQescapeStringConn (conn, re, r, strlen(r), &error);
	PQescapeStringConn (conn, ce, c, strlen(c), &error);
	asprintf(&sql, 
		"SELECT insert_az_footprint('%s','%s','%s','%s','%s');", 
		se, ae, pe, ce, re
	);
	syslog( LOG_DEBUG, "SQL:  %s", sql);

	fputs(sql, f);	
	fclose(f);
}

/**
 * Upload the URI to the database.
 *
 * Return the URIID
 */
int
DBSQLExecFootprint( DBObj_t *db, URIObj_t *uri, int reclevel )
{
	PGresult *rc;
	PGconn *conn         = db->dbconn;
	const char *stmtName = DBH_SQL_AZ_FOOTPRINT;
	char *s = (char *) URIObjGetScheme( uri ),
	     *a = (char *) URIObjGetAuth( uri ),
	     *p = (char *) URIObjGetPath( uri ),
	     *c = (char *) URIObjGetContent( uri ),
	     *r = (char *) itoa( reclevel );
	int nParams = 5;
	const char *paramValues[5] = { s, a, p, c, r };
	const int *paramLengths = NULL;
	const int *paramFormats = NULL;  /* parameters assumed as text */
	int resultFormat = 0;
	int rv = 0;

	rc = PQexecPrepared( conn, stmtName, nParams, paramValues, paramLengths, paramFormats, resultFormat);
	if( PQresultStatus( rc ) != PGRES_TUPLES_OK ) {
		syslog(LOG_ERR, "could not execute db footprint STH - %s ", PQresultErrorMessage(rc));
		syslog(LOG_DEBUG, 
			"scheme = '%s': auth = '%s': path = '%s': reclevel = '%s'",
			s, a, p, r
		);
		_DebugSQL(conn, s, a, p, c, r);
		rv = 0;
	} else if( PQgetisnull(rc, 0, 0) ) {
		syslog( LOG_ERR, "a null value was returned when getting URI id");
		rv = 0;
	} else {

		rv = atoi( PQgetvalue( rc, 0, 0 ));
		syslog( LOG_DEBUG, "Upload successful URIID set to %d for '%s'", rv , URIObjGetFQP(uri) );	
	}
	PQclear( rc );
	return rv;
}
