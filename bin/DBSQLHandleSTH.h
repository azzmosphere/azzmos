/*
 * =====================================================================================
 *
 *       Filename:  DBSQLHandleSTH.h
 *
 *    Description:  The functions and definitions for the Database abstraction layer.
 *                  This file should be included instead of DBSQLHandle.h as it 
 *                  contains the abstraction to the database and as such 
 *
 *        Version:  1.0
 *        Created:  16/07/2010 23:26:12
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */

/* #####   HEADER FILE INCLUDES   ################################################### */
#define __DBSQLHANDLESTH_H__

#ifndef _DBSQLHANDLE_H_
#include <DBSQLHandle.h>
#endif

#ifndef __URIQUALIFIERDB_H__
#include <URIQualifierDB.h>
#endif

/* #####   EXPORTED FUNCTION DECLARATIONS   ######################################### */
extern DBSQLHandleSth_t * dbInitSth ( DBObj_t * );
extern void               dbFinitSth( DBSQLHandleSth_t *usth );



