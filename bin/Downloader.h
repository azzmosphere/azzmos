/*
 * =====================================================================================
 *
 *       Filename:  Downloader.h
 *
 *    Description:  Common arguments and functions for the Downloader thread.
 *
 *        Version:  1.0
 *        Created:  14/07/2010 22:30:33
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */


/* #####   HEADER FILE INCLUDES   ################################################### */
#define __DOWNLOADER_H__

#ifndef _COMMON_H_
#include <common.h>
#endif

#ifndef _OPTIONS_H_
#include <Options.h>
#endif

#ifndef _SCHEDULERTYPES_H_
#include <SchedulerTypes.h>
#endif


/* #####   EXPORTED TYPE DEFINITIONS   ############################################## */

struct DownloadEngine_s {
	Scheduler_t      *de_sc;    /* scheduler object used for syncrnization */
	DownloadHTML_t   *de_dl;    /* link to the network */
	DBObj_t          *de_db;    /* database handle */
	DBSQLHandleSth_t *de_dbsth; /* database statement handle */
	URIRegex_t       *de_urire; /* URI regular expression object */
} typedef DownloadEngine_t;


/* #####   EXPORTED FUNCTION DECLARATIONS   ######################################### */
extern DownloadEngine_t *download_eng_init(  Scheduler_t *);
extern void             *downloaderEngine( void *argsin);


/* #####   EXPORTED VARIABLES   ##################################################### */
#ifndef DL_INIT_MAX_TRIES
#define DL_INIT_MAX_TRIES 100  /* The maximum amount of tries that is aloud for a thread to fail */
#endif

