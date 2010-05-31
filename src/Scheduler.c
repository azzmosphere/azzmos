/*
 * =====================================================================================
 *
 *       Filename:  Scheduler.c
 *
 *    Description: Schedule each downloader,  each downloader will
 *                 run in its own thread. 
 *
 *        Version:  1.2
 *        Created:  14/05/2010 22:47:47
 *       Revision:  $Revision:$
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */


#include <Scheduler.h>


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ScheduleProcess
 *  Description:  Schedules each Downloader.
 * =====================================================================================
 */
void
SchedulerProcess( Scheduler_t *sc )
{
	const Opts_t *opts = SchedulerGetOpts(sc);
	int rv = 0;

	/* This section here is the section that will represent each downloader */
	syslog( LOG_DEBUG, "downloader started");
	/* create regular expression pattern holder */
	URIRegex_t *urire = URIRegexInit( );

	/* create internet socket and start downloading stuff */
	DownloadHTML_t *dl;

	/* Create URIQualifier list */
 	/* REMAINS TO BE IMPLEMENTED */
	URIQualify_t *uq;
	pthread_mutex_t uq_lock;
	if( rv = pthread_mutex_init( &uq_lock, NULL) ) {
		syslog( LOG_CRIT, "could not create mutex lock for URI Qualifier - %s", strerror( rv ));
		SchedulerSetStatus( sc, EXIT_FAILURE);
	}
	else {
		uq = URIQualifyInit();
		URIQualifySetLock( uq, &uq_lock );
		INIT_LIST_HEAD( &uq->uq_list );
	}


	if( (dl = InitDownloadHTML( opts )) == NULL ) {
		syslog( LOG_ERR, "could not initilize DownloadHTML");
		SchedulerSetStatus( sc, EXIT_FAILURE);
	} 
	else if ( rv == 0 ){
		/*  create initial URI and qualify it */
		URIObj_t *uri =  URIObjInit();

		/* get initial URI seed */
		char *seed = URIQualify( urire, opts->o_seed, uri, NULL );

		/* create connection to the database */
		DBObj_t *db;
		if( (db = DBSQLHandleInit( opts)) == NULL)
			SchedulerSetStatus( sc, EXIT_FAILURE);
		
		else if( DBSQLPrepareAzFootprint( db ) ) {
			DBSQLHandleCleanUp( db );
			CleanUpDownloadHTML( dl );
			syslog(LOG_CRIT, "could not create STH");
			SchedulerSetStatus(sc, EXIT_FAILURE);
		} 
		else {
			

			/* For each iteration of the loop get the next seed*/	
			if( DownloadURI( dl, uq, seed, urire, 0, uri, db, opts) ) {
				syslog(LOG_ERR, "error downloading site");
				SchedulerSetStatus( sc, EXIT_FAILURE);
			}
		}

		URIObjCleanUp(uri);
		DBSQLHandleCleanUp(db);
	}

	pthread_mutex_destroy(&uq_lock);
	CleanUpURIQualify( uq );
	CleanUpDownloadHTML( dl );
	URIRegexCleanUp( urire );
	syslog(LOG_DEBUG, "downloader finished");
	/* Downloader has finished */

	syslog(LOG_DEBUG, "downloader engine finished");
	/* Downloader engine finished */
}

/**
 *  Initilize the sceduler object.
 */
Scheduler_t *
SchedulerInit( )
{
	Scheduler_t *sc = (Scheduler_t *) malloc( sizeof(Scheduler_t) );
	if( ! sc ) {
		syslog(LOG_ERR, "could not initilize scheduler object");
		return NULL;
	}
	return sc;
}

/**
 * Deinitilize scheduler object.  Do not free the opts. 
 * This should be done outside of this program. 
 */
void
SchedulerCleanUp( Scheduler_t *sc )
{
	sc = NULL;
	free(sc);
}
