/*
 * =====================================================================================
 *
 *       Filename:  URIQualifierDB.h
 *
 *    Description: Create an abstraction between the database and the URIQualfier, this
 *                 is done so that different databases can be compiled into the source
 *                 in future releases. 
 *
 *                 The file will need to be included as part of the URIQualifier.c 
 *                 imports and should replace URIQualifier.h as it will all ready
 *                 include the libraries in it and a cyclical reference could 
 *                 occur.
 *
 *        Version:  1.0
 *        Created:  11/07/2010 20:33:17
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */

/* #####   HEADER FILE INCLUDES   ################################################### */
#define __URIQUALIFIERDB_H__

#include <DBSQLHandleSTHTypes.h>

#ifndef __URIQUALIFIERDB_H__
#include <URIQualiferDb.h>
#endif

/* #####   EXPORTED MACROS   ######################################################## */

/* The following MACROS are used to create the statement handle to the edges table */
#define UQ_DB_EDGE_SQL        "SELECT upload_edge( $1, $2, $3, $4, $5, $6);"
#define UQ_DB_EDGE_PARAMTYPE  {TEXTOID, TEXTOID, TEXTOID, TEXTOID, TEXTOID, TEXTOID}
#define UQ_DB_EDGE_ERRMSG     "URIQualifierDB - Edge"
#define UQ_DB_EDGE_NPARAMS    6
#define UQ_DB_EDGE_INIT( db, param ) DBSQLSthInit( db, \
		                            UQ_DB_EDGE_SQL, \
		                            param,\
                                            UQ_DB_EDGE_ERRMSG, \
		                            UQ_DB_EDGE_NPARAMS)
#define UQ_DB_EDGE_ROW  0
#define UQ_DB_EDGE_COL  0
/* #####   EXPORTED TYPE DEFINITIONS   ############################################## */

/* #####   EXPORTED DATA TYPES   #################################################### */

/* #####   EXPORTED VARIABLES   ##################################################### */

/* #####   EXPORTED FUNCTION DECLARATIONS   ######################################### */
extern DBSQLHandleSth_t * initEdgeSth( DBObj_t *db, DBSQLHandleSth_t * usth );

