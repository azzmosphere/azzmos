/*
 * =====================================================================================
 *
 *       Filename:  DBSQLHandleSTHTypes.h
 *
 *    Description:  Types used by the database abstraction layer.
 *
 *        Version:  1.0
 *        Created:  16/07/2010 23:59:28
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */


/* #####   HEADER FILE INCLUDES   ################################################### */

#define __DBSQLHANDLESTHTYPES_H__

#ifndef _DBSQLHANDLE_H_
#include <DBSQLHandle.h>
#endif


/* #####   EXPORTED TYPE DEFINITIONS   ############################################## */

struct DBSQLHandleSth_s {
	DBSth_t   *usth_edge;  /* statement handle for edges */
} typedef DBSQLHandleSth_t;



