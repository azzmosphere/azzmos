/*
 * =====================================================================================
 *
 *       Filename:  DBSQLHandleSTH.c
 *
 *    Description: The abstraction layer contains the database definitions and the 
 *                 statement handles for running the database queries and commands. 
 *                 The abstraction layer is the interface between the underlying 
 *                 database struction and the application.  No function should 
 *                 directly communicate with the DBSQLHandle,  It should go 
 *                 through the abstraction layer. This allows for multiple database
 *                 engines to be used in a convient manner.
 *
 *        Version:  1.0
 *        Created:  16/07/2010 23:37:03
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */


/* #####   HEADER FILE INCLUDES   ################################################### */
#include <DBSQLHandleSTH.h>

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  URIQDbInitEdgeSth
 *  Description:  Initilize statement handles that will be used with URI qualifier
 * =====================================================================================
 */
extern DBSQLHandleSth_t *
dbInitSth ( DBObj_t *db )
{
	DBSQLHandleSth_t *usth = (DBSQLHandleSth_t *) malloc( sizeof(DBSQLHandleSth_t));
	if ( ! usth ) {
		syslog( LOG_ERR, "could not allocate memory for DBA sth %s", strerror( errno ));
		return NULL;
	}

	usth = initEdgeSth( db, usth );
	return usth;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  URIDbFinitSth
 *  Description:  Destroy the Statement handle holder.
 * =====================================================================================
 */
extern void
dbFinitSth ( DBSQLHandleSth_t *usth )
{
	if( ! usth ) return;
	if( usth->usth_edge )
		DBSQLSthFinit( usth->usth_edge );
	free( usth );
}		/* -----  end of function URIDbFinitSth  ----- */

