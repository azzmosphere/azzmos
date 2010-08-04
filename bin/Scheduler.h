/*
 * =====================================================================================
 *
 *       Filename:  Scheduler.h
 *
 *    Description:  
 *
 *        Version:  1.2
 *        Created:  14/05/2010 22:50:33
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */



/* #####   HEADER FILE INCLUDES   ################################################### */

#define _SCEDULER_H_
#ifndef _COMMON_H_
#include <common.h>
#endif

#ifndef _SCHEDULERTYPES_H_
#include <SchedulerTypes.h>
#endif

#ifndef _URIEXTRACT_H_
#include <URIExtract.h>
#endif

#ifndef _DOWNLOADHTML_H_
#include <DownloadHTML.h>
#endif

#ifndef _OPTIONS_H_
#include <Options.h>
#endif

#ifndef _DBSQLHANDLE_H_
#include <DBSQLHandle.h>
#endif

#ifndef __URIQUALIFIER_H__
#include <URIQualifier.h>
#endif

#ifndef _DOWNLOADURI_H_
#include <DownloadURI.h>
#endif

#ifndef __DOWNLOADER_H__
#include <Downloader.h>
#endif


/* #####   EXPORTED DATA TYPES   #################################################### */




/* #####   EXPORTED FUNCTION DECLARATIONS   ######################################### */

extern void          schedulerProcess( void * argsin );
extern Scheduler_t * schedulerInit( const Opts_t *opts );
extern void          schedulerCleanUp( Scheduler_t *sc );

/* #####   EXPORTED MACROS   ######################################################## */

#define schedulerSetCurlStatus(  sc, code)   (sc->sc_curlstatus = code)  
#define schedulerSetOpts( sc, opts )         (sc->sc_opts = opts )
#define schedulerGetCurlStatus( sc )         sc->sc_curlstatus
#define schedulerGetOpts( sc )               sc->sc_opts
#define schedulerGetStatus( sc )             sc->sc_status

