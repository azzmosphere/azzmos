/*
 * =====================================================================================
 *
 *       Filename:  Azzmos.c
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


#include <azzmos.h>

int 
main( int argc, char *argv[] )
{
	int status = EXIT_SUCCESS;
	Scheduler_t *sc;
	atexit(closelog);
	openlog(PACKAGE_NAME, LOG_CONS|LOG_PERROR, LOG_USER);
	INFO("program started");

	/* make sure that the log is started before calling this
	 * function but that the CURL handle has not yet been initilized */
	DEBUG( "getting CLI options");
	Opts_t *opts = getOptions( &argc, argv );
	if( opts == NULL ){ 
		exit( EXIT_FAILURE );
	}

	/* From this point the Downloader has started */
	DEBUG("Downloader engine started" );
	CURLcode curlstatus = curl_global_init(CURL_GLOBAL_ALL);
	if(curlstatus == CURLE_OK){ 
		atexit(curl_global_cleanup);
	}
	if( curlstatus != CURLE_OK) {
		ERROR_C("CURL global init failed", curlstatus);
		status = EXIT_FAILURE;
	} 
	else {

		/* set up scheduler first, this way if a seed 
		 * is defined it can be downloaded */
		if((sc = schedulerInit(opts)) == NULL) {
			ERROR( "could not set scheduler");	
			status = EXIT_FAILURE;
		} 
		else { 
			sig_init( sc );
			schedulerProcess( (void *)sc);	
		}
	}
	schedulerCleanUp(sc);
	INFO("program finished");
	exit(status);
}

