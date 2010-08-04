/*
 * =====================================================================================
 *
 *       Filename:  Options.c
 *
 *    Description:  Define Command Line Interface (CLI) options that can be used
 *                  with azzmos and azzmosctl. Also define any configuration file
 *                  options.
 *
 *        Version:  1.0
 *        Created:  30/07/2010 23:30:46
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */

#include <Options.h>


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  usage
 *  Description:  Describe the usage of the program.
 * =====================================================================================
 */
static void 
usage()
{
	const char *fmt =  "  %s, %-25s %s\n";
	fprintf( stdout, "usage: %s --dbconf [dbconf] --seed [seed]\n", PACKAGE);
	fprintf( stdout, fmt, "-c", "--client-encoding=CLIENTE", "set the text encoding option for the database");
	fprintf( stdout, fmt, "-d", "--dbconf=DBCONF", "database connection file");
	fprintf( stdout, fmt, "-m", "--maxthread=MAXTHREAD","maximum number of downloader threads");
	fprintf( stdout, fmt, "-s", "--seed=SEED", "initial URI to use as seed");
	fprintf( stdout, fmt, "-t", "--temp=TEMP", "set temp directory");
	fprintf( stdout, fmt, "-h", "--help", "this help sscreen");
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getOptions
 *  Description:  Get CLI options and place them into the options object
 * =====================================================================================
 */
Opts_t *
getOptions(int *argc, char **argv)
{
	bool help = false;
	char ch;

	struct option longopts[] = {
		{ "dbconf",   required_argument,  NULL, 'd' },
		{ "help",     no_argument,        NULL, 'h' },
		{ "seed",     required_argument,  NULL, 's' },
		{ "temp",     required_argument,  NULL, 't' },
		{ "maxthread",required_argument,  NULL, 'm' },
		{ "client-encoding", required_argument, NULL, 'c'},
		{ NULL,       0,                  NULL,  0  }
	};
	Opts_t *opts = malloc( sizeof( Opts_t ) );
	if( ! opts ) {
		syslog( LOG_ERR, "could not get enough memory for assigning options");
		return NULL;
	}

	opts->o_tdir = NULL;
	opts->o_seed = NULL;
	opts->o_clientenc = NULL;
	opts->o_dcount = DEFAULT_DCOUNT;

	while( (ch = getopt_long(*argc, argv, "hd:s:t:c:", longopts, NULL)) != -1) {
		switch (ch ) {
			case 'd':
				opts->o_dbconf = strdup(optarg);
				break;
			case 't':
				opts->o_tdir = strdup(optarg);
				break;

			case 's':
				opts->o_seed = strdup(optarg);
				break;
			case 'c':
				opts->o_clientenc = strdup(optarg);
				break;
			case 'm':
				opts->o_dcount = atoi(optarg);
			case 'h':
			default:
				help = true;
		}
	}

	if( help ) {
		usage();
		return NULL;
	}	

	if( opts->o_seed == NULL  || opts->o_dbconf == NULL ) {
		fprintf( stdout, "missing required argument\n");
		usage();
		return NULL;
	}

	if( opts->o_tdir == NULL) {
		opts->o_tdir = strdup( getenv("TMPDIR"));
	}

	if( ! opts->o_tdir ) {
		syslog(LOG_ERR, "temporary directory was not argued and TMPDIR is not set");
		return NULL;
	}

	if( opts->o_clientenc == NULL)
		opts->o_clientenc = strdup(DEFAULT_CLIENC);

	*argc -= optind;
	argv += optind;
	return opts;
}
