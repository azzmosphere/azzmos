/*
 * =====================================================================================
 *
 *       Filename:  SchedulerTypes.h
 *
 *    Description:  Define type that is needed for the scheduler process.
 *
 *        Version:  1.0
 *        Created:  22/07/2010 22:22:53
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */


/* #####   HEADER FILE INCLUDES   ################################################### */
#define _SCHEDULERTYPES_H_

#ifndef _COMMON_H_ 
#include <common.h>
#endif

#ifndef _OPTIONS_H_
#include <Options.h>
#endif

#ifndef __DBSQLHANDLESTH_H__
#include <DBSQLHandleSTH.h>
#endif

#ifndef __URIQUALIFIER_H__
#include <URIQualifier.h>
#endif


/* #####   EXPORTED TYPE DEFINITIONS   ############################################## */
enum SchedulerFlags {
	SC_DEFAULT   = 0x00,
	SC_ALLOCATED = 0x01,
	SC_EXIT      = 0x02,
	SC_SEEDOPT   = 0x04
} typedef SchedulerFlags_t;

struct Scheduler_s {
	const Opts_t     *sc_opts;   /*  options structure */
	int               sc_status; /*  status of the scheduler process */
	DBObj_t          *sc_db;     /*  database object  */
	DBSQLHandleSth_t *sc_sth;    /*  statement handle object */
	pthread_mutex_t   sc_lock;   /*  lock for scheduler */
	URIQualify_t     *sc_uq;     /*  URI Qualification object */
	char            **sc_seed;   /*  Current seed to be allocated */
	long              sc_flags;  /*  Flags that are used with Scheduler */
} typedef Scheduler_t;


/* #####   EXPORTED FUNCTION DECLARATIONS   ######################################### */
extern bool chkExitStatus( Scheduler_t *sc );
extern void schedulerSetStatus (Scheduler_t *sc, int status );
extern char * schedulerGetSeed ( Scheduler_t *sc );
extern bool seedAvailable ( Scheduler_t *sc );
extern bool scLock( Scheduler_t *sc );


/* #####   EXPORTED MACROS   ######################################################## */
#define scUnlock( sc )    pthread_mutex_unlock( &sc->sc_lock)

