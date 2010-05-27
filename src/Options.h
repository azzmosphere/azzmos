#define _OPTIONS_H_
#include <common.h>

struct Opts_s {
	char *o_dbconf;
	char *o_seed;
	char *o_tdir;
	char *o_clientenc;
} typedef Opts_t;

Opts_t * GetOptions(int *argc, char **argv);


#define DEFAULT_CLIENC  "iso-8859-1" /* the default encoding of the database. */

