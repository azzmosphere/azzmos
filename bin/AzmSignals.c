/*
 * =====================================================================================
 *
 *       Filename:  AzmSignals.c
 *
 *    Description:  Handle signals delivered to the program,  the signals handles here 
 *                  are, SIGHUP, SIGINT, SIGKILL, SIGQUIT
 *
 *        Version:  1.0
 *        Created:  07/08/2010 17:21:00
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */

#include <AzmSignals.h>

static void sig_exit( Scheduler_t *sc );

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  sig_hup
 *  Description:  Handler for the sig_hup signal
 * =====================================================================================
 */
extern void
sig_hup()
{
	/* ignore this signal all together */
	DEBUG("SIGHUP was called");
	
	/* reset the signal */
	signal(SIGHUP, sig_hup);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  sig_sc
 *  Description:  Sets the sc object so that it can be used with common signals.
 * =====================================================================================
 */
extern Scheduler_t *
sig_sc( Scheduler_t *sc )
{
	static Scheduler_t *sc_l;

	if( sc ) {
		DEBUG("setting the SC object to handle signals");
		sc_l = sc;
	}
	DEBUG("returning the sc object");
	return sc_l;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  sig_exit
 *  Description:  Set the SC_EXIT flag allowing SC program to exit gracefully
 * =====================================================================================
 */
static void
sig_exit( Scheduler_t *sc)
{
	DEBUG("setting SC_EXIT flag");
	scLock( sc );
	sc->sc_flags |= SC_EXIT;
	scUnlock( sc );
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  sig_int
 *  Description:  Gracefully exit if the SIGINT signal is given
 * =====================================================================================
 */
extern void
sig_int()
{
	Scheduler_t *sc = sig_sc(NULL);
	sig_exit( sc );
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  sig_kill
 *  Description:  Gracefully exit if the SIGINT signal is given
 * =====================================================================================
 */
extern void
sig_kill()
{
	Scheduler_t *sc = sig_sc(NULL);
	sig_exit( sc );
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  sig_kill
 *  Description:  Gracefully exit if the SIGINT signal is given
 * =====================================================================================
 */
extern void
sig_quit()
{
	Scheduler_t *sc = sig_sc(NULL);
	sig_exit( sc );
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  sig_init
 *  Description:  Initilize the signal handlers.
 * =====================================================================================
 */
extern void
sig_init( Scheduler_t *sc )
{
	sig_sc( sc );
	signal( SIGHUP, sig_hup);
	signal( SIGINT, sig_int);
	signal( SIGKILL, sig_kill);
	signal( SIGQUIT, sig_quit);
}

