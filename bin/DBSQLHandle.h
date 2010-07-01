#define _DBSQLHANDLE_H_
#include <common.h>
#ifndef _URIOBJ_H_
#include <URIObj.h>
#endif
#ifndef _OPTIONS_H_
#include <Options.h>
#endif


/**
 * The database handle object is abstracted in a structure.
 * This is done so that locking on the handle can be
 * performed in multi-threaded applications without the 
 * need to create seperate database handle object.
 *
 */


#define DBH_SQL_AZ_FOOTPRINT "DBH_SQL_AZ_FOOTPRINT"


/*
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
 */
#define INT4OID      23
#define TEXTOID      25
#define VARCHAROID 1043


struct DBObj_s {
	PGconn *dbconn;
} typedef DBObj_t;

DBObj_t *DBSQLHandleInit( const Opts_t *opts );
void     DBSQLHandleCleanUp( DBObj_t *db );
int      DBSQLPrepareAzFootprint( DBObj_t *db );
int      DBSQLExecFootprint( DBObj_t *db, URIObj_t *uri, int reclevel );

