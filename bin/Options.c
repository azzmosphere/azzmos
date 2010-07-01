/**
 * Get various options.
 */

#include <Options.h>

static void 
Usage()
{
	const char *fmt =  "  %s, %-25s %s\n";
	fprintf( stdout, "Usage: %s --dbconf [dbconf] --seed [seed]\n", PACKAGE);
	fprintf( stdout, fmt, "-c", "--client-encoding=CLIENTE", "set the text encoding option for the database");
	fprintf( stdout, fmt, "-d", "--dbconf=DBCONF", "database connection file");
	fprintf( stdout, fmt, "-s", "--seed=SEED", "initial URI to use as seed");
	fprintf( stdout, fmt, "-t", "--temp=TEMP", "set temp directory");
	fprintf( stdout, fmt, "-h", "--help", "this help sscreen");
}

Opts_t *
GetOptions(int *argc, char **argv)
{
	bool help = false;
	char ch;

	struct option longopts[] = {
		{ "dbconf",   required_argument,  NULL, 'd' },
		{ "help",     no_argument,        NULL, 'h' },
		{ "seed",     required_argument,  NULL, 's' },
		{ "temp",     required_argument,  NULL, 't' },
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
			case 'h':
			default:
				help = true;
		}
	}

	if( help ) {
		Usage();
		return NULL;
	}	

	if( opts->o_seed == NULL  || opts->o_dbconf == NULL ) {
		fprintf( stdout, "missing required argument\n");
		Usage();
		return NULL;
	}

	if( opts->o_tdir == NULL)
		opts->o_tdir = strdup( getenv("TMPDIR"));
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
