/*
 * =====================================================================================
 *
 *       Filename:  DBSQLHandle.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/07/2010 23:31:57
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */

/* #####   HEADER FILE INCLUDES   ################################################### */

#define _DBSQLHANDLE_H_

#ifndef __COMMON_H__
#include <common.h>
#endif

#ifndef _URIOBJ_H_
#include <URIObj.h>
#endif

#ifndef _OPTIONS_H_
#include <Options.h>
#endif


/* #####   EXPORTED TYPE DEFINITIONS   ############################################## */


/*=====================================================================================
 * To find Oid these you need to look into 
 * server/catalog/pg_type.h,  which is
 * a very messy place. 
 *
 * However most (all) OID are the type name
 * in capitals plus OID. So a guess should 
 * be good enough.
 *
 * Do not include in the code,  bad things
 * happen when you try.
 *
 *=====================================================================================
 */
#define INT4OID      23
#define TEXTOID      25
#define VARCHAROID 1043


/* #####   EXPORTED DATA TYPES   #################################################### */

/*===================================================================================== 
 * DBObj_t
 * =======
 *
 * The database object is used for two purposes,  the primary purpose is to keep a 
 * active handle to the database. The secondary purpose is to provide a unique index
 * to the statement handles.
 *=====================================================================================*/
struct DBObj_s {
	PGconn *dbconn;
	int     dbidx;
} typedef DBObj_t;

 
/*===================================================================================== 
 * DBSth_t
 * =======
 *
 * The statement handle is used to provide a SQL statement handle that can re-executed
 * many times.  The handle once initilized keeps the compiled executatble SQL statement
 * on the database until the handle is destroyed.  After each execution of the handle it
 * will need to cleared before it is reused.  However it will keep the results on the 
 * database and they can be retrieved in sequence useing the fetch statement.
 *=====================================================================================*/
struct DBSth_s {
	DBObj_t    *db_conn; 
	const char *db_stmtName; 
	int         db_nParams; 
	const int  *db_paramLengths; 
	const int  *db_paramFormats;
	const Oid  *db_paramTypes;
       	int         db_resultFormat;
	PGresult   *db_rc;
	const char *db_errmsg;
} typedef DBSth_t;

/* #####   EXPORTED FUNCTION DECLARATIONS   ######################################### */

DBObj_t *DBSQLHandleInit( const Opts_t *opts );
void     DBSQLHandleCleanUp( DBObj_t *db );
DBSth_t *DBSQLSthInit( DBObj_t *, const char *, const Oid  *, const char *, int);
void     DBSQLSthFinit ( DBSth_t *sth );
int      DBSQLPrepareStatement( DBObj_t *db,  const char *stmtName, const char *sql, const Oid *paramTypes, const char *errmsg, int nParams, PGresult *);
int      DBSQLExecSthQuery ( DBSth_t *sth, const char **paramValues );
int      DBSQLExecSthCmd ( DBSth_t *sth, const char **paramValues );
int      DBSQLExecSth ( DBSth_t *sth, const char **paramValues, int pgcheck );




/* #####   EXPORTED MACROS   ######################################################## */
#define DB_ERROR(m1,m2)  ERROR_B(m1, PQerrorMessage(m2))

/*===================================================================================== 
 * The following macros are special in that they are to be used as if they where 
 * functions hence the lack of upper casing on them.
 *=====================================================================================*/
#define DBSQLGetIdx( db ) ( db->dbidx )
#define DBSQLGetResult( sth, row, col ) ( PQgetvalue( sth->db_rc, row, col) )
#define DBSQLClearResult( sth )  ( PQclear(sth->db_rc ))

