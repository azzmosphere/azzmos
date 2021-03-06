/*
 * =====================================================================================
 *
 *       Filename:  Downloader.c
 *
 *    Description: The downloader thread is used to donwload URIs.  It retrieves one
 *                 URL from a list provided to it by the scheduler and then continues
 *                 to donwload as many sites as it can transversing down each reference
 *                 offered to it by the initlial seed provided. 
 *
 *        Version:  1.0
 *        Created:  18/07/2010 21:46:41
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */

#include <Downloader.h>

static void              dl_error( Scheduler_t *sc, char *msg );
static bool              de_get_seed( char **seed, Scheduler_t *sc );
/*  static */ DownloadEngine_t *de_sth_init( DownloadEngine_t *de, Scheduler_t *sc );
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  downloaderEngine
 *  Description:  The main function
 * =====================================================================================
 */
extern void*
downloaderEngine(void *argsin)
{
	DownloadEngine_t *de = (DownloadEngine_t *) argsin;
	Scheduler_t *sc = de->de_sc;
	URIObj_t *uri;
	char     *seed;
	int       dlerr = 0;

	de = de_sth_init( de, sc);
	if( ! de ) {
		ERROR("could not initilize database");
		schedulerSetStatus( sc, EXIT_FAILURE);
		return;
	}

	/* downloader engine starts here */
	DEBUG("downloader started");

	/*  loop will continue until SC_EXIT */
	while( de_get_seed(&seed, sc) ){
		DEBUG("got seed from scheduler");

		/*  create initial URI and qualify it */
		uri =  URIObjInit();
		DEBUG_STR( "seed", seed);

		/* For each iteration of the loop get the next seed*/	
		dlerr = dl_uri( de->de_dl, sc->sc_uq, seed, de->de_urire, 0, uri, sc->sc_opts, de->de_dbsth);
		if( dlerr ) {
			ERROR_E("error downloading site", dlerr);
			schedulerSetStatus( sc, EXIT_FAILURE);
		}
		URIObjCleanUp(uri);
	}
	URIObjCleanUp(uri);
	dbFinitSth( de->de_dbsth );
	DBSQLHandleCleanUp(de->de_db);
	DEBUG("downloader finished");
	/* Downloader has finished */
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  de_sth_init
 *  Description:  initilise the database and statement handle for the downloader engine,
 *                this must be done inside the downloader thread
 * =====================================================================================
 */
/*  static */DownloadEngine_t *
de_sth_init( DownloadEngine_t *de, Scheduler_t *sc )
{
	de->de_db = DBSQLHandleInit(  sc->sc_opts );
	/* create connection to the database */
	if( de->de_db == NULL) {
		dl_error(sc,"download engine no db");
		return NULL;
	}	

	/*  Initlize the statement handle. */
	de->de_dbsth = dbInitSth( de->de_db );
	if( de->de_dbsth == NULL ) {
		dl_error(sc, "could not create STH");
		return NULL;
	} 
	return de;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  download_eng_init
 *  Description:  Initilize the downloader engine.
 * =====================================================================================
 */
extern DownloadEngine_t *
download_eng_init(  Scheduler_t * sc)
{
	const Opts_t *opts = sc->sc_opts;

	DownloadEngine_t *de = (DownloadEngine_t *) malloc( sizeof(DownloadEngine_t) );
	if( ! de ) {
		dl_error(sc, "could not inilize downloader engine");
		return NULL;
	}


	/* create regular expression pattern holder */
	if( (de->de_urire = URIRegexInit()) == NULL) {
		dl_error(sc, "regular expresion error for downloader engine");
		free( de );
		return NULL;
	}

	/* create internet socket */
	if( (de->de_dl = InitDownloadHTML( opts )) == NULL ) {
		dl_error(sc, "could not initilize DownloadHTML");
		URIRegexCleanUp( de->de_urire );
		free( de );
		return NULL;
	} 

	/* set de_sc if all has gone well */
	de->de_sc = sc;

	return de;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  dl_error
 *  Description:  Print the error message and set the scheduler to EXIT_FAILURE.  also
 *                set the SC flag  SC_EXIT to true.
 * =====================================================================================
 */
static void 
dl_error( Scheduler_t *sc, char *msg )
{
	ERROR(msg);
	schedulerSetFlag( sc, SC_EXIT );
	schedulerSetStatus( sc, EXIT_FAILURE);	
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  dlEngCleanUp
 *  Description:  Destroy the downloader engine object.
 * =====================================================================================
 */
extern void
deEngCleanUp(DownloadEngine_t *de, pthread_t thread )
{
	cleanUpDownloadHTML( de->de_dl );
	URIRegexCleanUp( de->de_urire );
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  de_get_seed
 *  Description:  Check the exit status,  if the exit status is set to true then return
 *                false.  Otherwise wait until a seed becomes available and then grab 
 *                it.
 * =====================================================================================
 */
static bool 
de_get_seed( char **seed, Scheduler_t *sc )
{
	bool rv    = true;
	bool found = false;

	DEBUG("waiting for available seed");
	do {
		DEBUG("checking to see if exit has been called");
		if (  chkExitStatus( sc ) ) {
			rv = false;
			DEBUG("exit routine called");
			break;
		}
		DEBUG("checking if seed available");
		scLock( sc );
		if( seedAvailable(sc) ){
			DEBUG("seed available");
			*(seed) = schedulerGetSeed( sc );
			DEBUG_STR("seed", *(seed));
			found = true;
		}
		scUnlock( sc );
	} while( !found && rv );
	if( found) {
		DEBUG("got seed leaving loop");
	}

	return found;
}
