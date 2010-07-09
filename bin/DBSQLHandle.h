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

#define DBH_SQL_AZ_FOOTPRINT "DBH_SQL_AZ_FOOTPRINT"


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

struct DBObj_s {
	PGconn *dbconn;
} typedef DBObj_t;


/* #####   EXPORTED FUNCTION DECLARATIONS   ######################################### */

DBObj_t *DBSQLHandleInit( const Opts_t *opts );
void     DBSQLHandleCleanUp( DBObj_t *db );
int      DBSQLPrepareStatement( DBObj_t *db,  const char *stmtName, const char *sql, const Oid *paramTypes, const char *errmsg, int nParams);


// Soon to be obsolete
int      DBSQLPrepareAzFootprint( DBObj_t *db );
int      DBSQLExecFootprint( DBObj_t *db, URIObj_t *uri, int reclevel );

