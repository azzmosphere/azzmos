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


/* #####   PROTOTYPES  -  LOCAL TO THIS SOURCE FILE   ############################### */
static inline  int   initDownloaderThread( DownloadEngine_t *, pthread_t *, pthread_attr_t *, Scheduler_t *);
static inline  DBSQLHandleSth_t * initSchedDb( Scheduler_t *sc );
static inline void   finitSchedDb( Scheduler_t *sc );
static inline void   getNextSeed( Scheduler_t *sc );
static inline bool   allocatedSeed( Scheduler_t *sc );
static bool          scLock( Scheduler_t *sc );
static inline int    getDcountFromOpts( Scheduler_t *sc );

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  void scheduleProcess( void *argsin)
 *  Description:  Initilize each downloader process then allocate a seed to that process
 *                as the processes are available and the seeds are available.  
 *
 *                Any downloader can pick up a unallocated seed at any time.  Once the 
 *                seed has been allocated the downloader thread will set the sc object 
 *                to SC_ALLOCATED, this will cause the Scheduler to get the next seed
 *                from the opts object, and if that is not available then from the 
 *                database.
 *
 *                This function is designed to be a threaded process.
 * =====================================================================================
 */
extern void
schedulerProcess( void *argsin )
{
	Scheduler_t *sc = (Scheduler_t *) argsin;
	int   rv = 0, 
	      dcount = getDcountFromOpts(sc), 
	      maxtries = 0,
	      i = 0;
	pthread_t thread[dcount];
	pthread_attr_t attr;
	DownloadEngine_t *dlc[ dcount ];

	/* Start downloader */
	for( i = 0; i < dcount; i ++ ) {
			
			if( rv = initDownloaderThread(dlc[i], &thread[i], &attr, sc) ) {
				syslog( LOG_ERR, "error createing downloader thread - %s", strerror(rv));
			 	i --;
		       		maxtries ++;
		 		continue;
			}

			/* see if we have exceeded max tries if so this is fatal */
			if( maxtries >= DL_INIT_MAX_TRIES) {
				syslog( LOG_CRIT, "exceeded maximum attempts to create downloader thread");
				schedulerSetStatus( sc, EXIT_FAILURE);
				return;
			}
	}

	do {
		/* get next seed */
		if( allocatedSeed( sc ) ) {
			getNextSeed(sc);
		}
	} while( ! chkExitStatus(sc));
	
	/* clean up each downloader thread. */
	for( i = 0; i < dcount; i ++ ) {
		deEngCleanUp( dlc[i], thread[i]);
	}

	syslog(LOG_DEBUG, "downloader engine finished");
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  initDownloaderThread
 *  Description:  Create a new downloader thread.
 * =====================================================================================
 */
static int
initDownloaderThread ( DownloadEngine_t *dlc, 
		       pthread_t *thread, 
		       pthread_attr_t *attr, 
		       Scheduler_t *sc )
{
	int rv = 0;
	dlc = initDownloadEngine( sc );
	if( dlc == NULL ) {
		rv = errno;
	}
	else { 
		rv = pthread_create( thread, attr, downloaderEngine, (void *) dlc); 
	}

	return rv;
}		/* -----  end of static function initDownloaderThread  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  destroySc
 *  Description:  destroy the SC, something bad has happened during creation.
 * =====================================================================================
 */
static inline void
destroySc( Scheduler_t *sc )
{
	if( sc->sc_sth ) {
		finitSchedDb( sc );
	}

	pthread_mutex_destroy( &sc->sc_lock );
	sc = NULL;
	free( sc );
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  schedulerInit
 *  Description:  Construct a new scheduler object.
 * =====================================================================================
 */
extern Scheduler_t *
schedulerInit( const Opts_t *opts )
{
	int pt_errno = 0;
	URIQualify_t    *uq;
	pthread_mutex_t *uq_lock;

	Scheduler_t *sc = (Scheduler_t *) malloc( sizeof(Scheduler_t) );
	if( ! sc ) {
		syslog(LOG_ERR, "could not initilize scheduler object");
		return NULL;
	}
	schedulerSetOpts( sc, opts );

	if( initSchedDb( sc ) == NULL ) {
		free(sc);
		return NULL;
	}

	/* create the locking variable. */
	if( pt_errno = pthread_mutex_init( &sc->sc_lock, NULL) ) {
		syslog( LOG_CRIT, "could not create mutex lock for scheduler - %s", strerror( pt_errno ));
		destroySc(sc);
		return NULL;
	}

	/* create the UQ object  */
	uq_lock = (pthread_mutex_t *) malloc( sizeof( pthread_mutex_t ));
	if( !uq_lock || (pt_errno = pthread_mutex_init( uq_lock, NULL)) ) {
		syslog( LOG_CRIT, "could not create mutex lock for URI Qualifier - %s", strerror( pt_errno ));
		destroySc(sc);
		return NULL;
	}

	if( !(uq = URIQualifyInit()) ) {
		syslog( LOG_CRIT, "could not create the URI Qualifier object");
		destroySc(sc);
		return NULL;

	}
	URIQualifySetLock( uq, uq_lock );
	INIT_LIST_HEAD( &uq->uq_list );

	/* set default flags */
	sc->sc_flags = SC_DEFAULT;
	sc->sc_seed  = (char **) malloc(sizeof(char *));

	return sc;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  schedulerCleanUp
 *  Description:  Deconstruct the scheduler object
 * =====================================================================================
 */
extern void
schedulerCleanUp( Scheduler_t *sc )
{
	/* stop the data base */
	finitSchedDb( sc );

	/* get rid of SC lock */
	pthread_mutex_destroy( &sc->sc_lock );

	/*  get rid of the UQ */
	pthread_mutex_destroy(sc->sc_uq->uq_lock);
	CleanUpURIQualify( sc->sc_uq );

	sc = NULL;
	free(sc);
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  initSchedDb
 *  Description:  Initilize a database statement handle for use with the scheduler. 
 * =====================================================================================
 */
static inline DBSQLHandleSth_t *
initSchedDb ( Scheduler_t *sc )
{
	const Opts_t *opts = schedulerGetOpts(sc);
	DBObj_t      *db = DBSQLHandleInit( opts );
	DBSQLHandleSth_t *sth;

	if( db == NULL ) {
		syslog( LOG_CRIT, "could not initilize database handle for scheduler");
		schedulerSetStatus( sc, EXIT_FAILURE);
		return NULL;
	}

	if( (sth = dbInitSth( db )) == NULL ) {
		syslog( LOG_CRIT, "could not initilize statement handle for scheduler");
		 DBSQLHandleCleanUp( db );
		 schedulerSetStatus( sc, EXIT_FAILURE);
		 return NULL;
	}

	sc->sc_db  = db;
	sc->sc_sth = sth;

	return sth;
}		/* -----  end of function initSchedDb  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  void finitSchedDb
 *  Description:  Destroy database and statement handle
 * =====================================================================================
 */
inline static void
finitSchedDb ( Scheduler_t *sc )
{
	dbFinitSth( sc->sc_sth );
	DBSQLHandleCleanUp( sc->sc_db );
}		/* -----  end of static function finitSchedDb  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getNextSeed
 *  Description:  Read the database handle and fetch the next seed.
 *
 *  TO BE IMPLEMENTED:  For now we just set the seed to the opts seed.
 * =====================================================================================
 */
static inline void 
getNextSeed( Scheduler_t *sc )
{
	if( ! scLock(sc) ) {
		return;
	}

	/*  allicate seed from options if SEED_OPT is true */
	if( sc->sc_flags &= SC_SEEDOPT ) {
		*(sc->sc_seed) = strdup( sc->sc_opts->o_seed );	
		sc->sc_flags |= ~SC_SEEDOPT;
	}

	/* reset allocated seed back to false */
	sc->sc_flags |= ~SC_ALLOCATED;
	scUnlock(sc);
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  scSeedLock
 *  Description:  Try and lock process,  return true if process is locked otherwise
 *                return false.
 * =====================================================================================
 */
static bool
scLock( Scheduler_t *sc ) 
{
	int err;

	/*  try and get a lock */
	if( err = pthread_mutex_lock( &sc->sc_lock ) ){
		syslog( LOG_CRIT, "could not create lock on scheduler - ", strerror(err));
		sc->sc_flags |= SC_EXIT;
		return false;
	}
	return true;

}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  allocatedSeed
 *  Description:  returns true if the seed has been allocated, otherwise return false.
 * =====================================================================================
 */
static inline bool 
allocatedSeed( Scheduler_t *sc )
{
	bool rv = false;
	if( ! scLock(sc) ) {
		return false;
	}

	if( sc->sc_flags &= SC_ALLOCATED ) {
		rv = true;
	}
	scUnlock(sc);
	return rv;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ckExitStatus
 *  Description:  Get the value of the exit status and return it.
 * =====================================================================================
 */
extern bool
chkExitStatus( Scheduler_t *sc )
{
	bool rv = false;

	rv = scLock(sc);

	if( rv && (sc->sc_flags &= SC_EXIT) ) {
		rv = true;
	}

	scUnlock(sc);
	return rv;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getDcountFromOpts
 *  Description:  Check the options list for the DCOUNT variable and return it.
 * =====================================================================================
 */
static inline int 
getDcountFromOpts( Scheduler_t *sc )
{
	int dcount = 0;
	if(! scLock( sc ) ) {
		return dcount;
	}

	const Opts_t *opts = schedulerGetOpts(sc);
	dcount = opts->o_dcount;
	scUnlock( sc );

	return dcount;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  schedulerSetFlag
 *  Description:  Sets a SC flag.
 * =====================================================================================
 */
extern void
schedulerSetFlag ( Scheduler_t *sc, SchedulerFlags_t flag  )
{
	scLock( sc );
	sc->sc_flags |= flag;
	scUnlock(sc );
}		/* -----  end of function schedulerSetFlag  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  schedulerSetFlag
 *  Description:  Sets a SC flag.
 * =====================================================================================
 */
extern void
schedulerSetStatus ( Scheduler_t *sc, int status  )
{
	scLock( sc );
	sc->sc_status = status;
	scUnlock(sc );
}	



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  schdulerGetSeed
 *  Description:  Do not lock in this getter.  This will be done in the check to see
 *                if a seed has been allocated.
 * =====================================================================================
 */
extern char *
schedulerGetSeed ( Scheduler_t *sc )
{
	char *seed;


	if( sc->sc_seed ) {
		seed = strdup( *(sc->sc_seed) );
	}

	sc->sc_seed = NULL;
	free( sc->sc_seed );
	sc->sc_flags |= SC_ALLOCATED;


	return seed;
}		/* -----  end of function schdulerGetSeed  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  seedAvailable
 *  Description:  Checks to see if a seed is available,  if it is then keep lock and 
 *                return true.  Otherwise return false.
 * =====================================================================================
 */
extern bool
seedAvailable ( Scheduler_t *sc )
{
	bool rv = false;
	scLock( sc );

	if( sc->sc_flags =~  SC_ALLOCATED ) {
		rv = true;
	}
	else {
		scUnlock( sc );
	}

	return rv;
}		/* -----  end of function isAllocated  ----- */
