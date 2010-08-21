/*
 * =====================================================================================
 *
 *       Filename:  URIQualifierDB.c
 *
 *    Description:  Abstraction layer between the database and the URIQualifier.  This
 *                  file helps collect information in the native C formats and convert
 *                  into formats that the PostgresQL library understands.  This file
 *                  should be able to be replaced for different databse releases in 
 *                  future versions.
 *
 *        Version:  1.0
 *        Created:  11/07/2010 20:37:55
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */

/* #####   HEADER FILE INCLUDES   ################################################### */
#include <URIQualifierDB.h>

/* #####   FUNCTION DEFINITIONS  -  EXPORTED FUNCTIONS   ############################ */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  init_edge_sth
 *  Description:  Initilize the edges statement handle. (local function)
 * =====================================================================================
 */
extern DBSQLHandleSth_t * 
initEdgeSth( DBObj_t *db, DBSQLHandleSth_t * usth )
{
	Oid params[UQ_DB_EDGE_NPARAMS] = UQ_DB_EDGE_PARAMTYPE;

	usth->usth_edge = UQ_DB_EDGE_INIT( db, params );
	if( ! usth->usth_edge ) {
		ERROR( "could not initilize the the edge statement");
		dbFinitSth( usth );
		usth = NULL;
	}
	return usth;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  uriUploadEdge
 *  Description:  Create the edge on the database.
 * =====================================================================================
 */
extern int 
uriUploadEdge( DBSQLHandleSth_t *usth, URIObj_t *xuri, URIObj_t *yuri )
{
	int rv = 0, pcount = 0;
	char *paramValues[UQ_DB_EDGE_NPARAMS], *idstr;

	DEBUG("uploading edge");
	paramValues[pcount ++] = URIObjGetScheme( xuri );
	paramValues[pcount ++] = URIObjGetAuth( xuri );
	paramValues[pcount ++] = URIObjGetPath(xuri);
	paramValues[pcount ++] = URIObjGetScheme( yuri );
	paramValues[pcount ++] = URIObjGetAuth( yuri );
	paramValues[pcount ++] = URIObjGetPath(yuri);
	rv = DBSQLExecSthQuery( usth->usth_edge, (const char **)paramValues );

	/* check result and assign id */
	if( rv ) {
		ERROR_E("could not upload edge", rv);
	}
	else {
		idstr = DBSQLGetResult(usth->usth_edge, UQ_DB_EDGE_ROW, UQ_DB_EDGE_COL );
		yuri->uri_id = atoi( idstr );
	}
	return rv;
}
