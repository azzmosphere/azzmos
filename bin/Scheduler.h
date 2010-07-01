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


/* #####   EXPORTED DATA TYPES   #################################################### */

struct Scheduler_s {
	const Opts_t  *sc_opts;
	int            sc_status;
} typedef Scheduler_t;


/* #####   EXPORTED FUNCTION DECLARATIONS   ######################################### */

void SchedulerProcess( Scheduler_t *sc );
Scheduler_t * SchedulerInit( );
void SchedulerCleanUp( Scheduler_t *sc );


/* #####   EXPORTED MACROS   ######################################################## */

#define SchedulerSetCurlStatus(  sc, code)   (sc->sc_curlstatus = code)  
#define SchedulerSetOpts( sc, opts )         (sc->sc_opts = opts )
#define SchedulerSetStatus( sc, status )     (sc->sc_status = status )

#define SchedulerGetCurlStatus( sc )         sc->sc_curlstatus
#define SchedulerGetOpts( sc )               sc->sc_opts
#define SchedulerGetStatus( sc )             sc->sc_status

