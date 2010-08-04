/*
 * =====================================================================================
 *
 *       Filename:  Options.h
 *
 *    Description:  Define command and file driven options that can be used with this
 *                  program.
 *
 *        Version:  1.0
 *        Created:  30/07/2010 23:28:08
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */


/* #####   HEADER FILE INCLUDES   ################################################### */
#define _OPTIONS_H_

#ifndef __COMMON_H__
#include <common.h>
#endif


/* #####   EXPORTED TYPE DEFINITIONS   ############################################## */
struct Opts_s {
	char           *o_dbconf;     /* databsae configuration file */
	char           *o_seed;       /* seed that has been argued in CLI options */
	char           *o_tdir;       /* temporary directory */
	char           *o_clientenc;  /* character encoding */
	int             o_dcount;     /* number of simultanious downloader threads */
	pthread_mutex_t o_lock;       /* lock the options */
} typedef Opts_t;


/* #####   EXPORTED FUNCTION DECLARATIONS   ######################################### */
Opts_t * getOptions(int *argc, char **argv);


/* #####   EXPORTED VARIABLES   ##################################################### */
#define DEFAULT_CLIENC  "iso-8859-1" /* the default encoding of the database. */
#define DEFAULT_DCOUNT  4            /* default number of threads */

