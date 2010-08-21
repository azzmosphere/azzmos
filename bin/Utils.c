/*
 * =====================================================================================
 *
 *       Filename:  Utils.c
 *
 *    Description:  Common tools that are used throughout all programs.
 *
 *        Version:  1.0
 *        Created:  08/08/2010 14:24:14
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */


#include <Utils.h>

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  USplice
 *  Description:  splice a character strting from offset start to offset end.  If end is 
 *                NULL then go to end of string. '\0'
 * =====================================================================================
 */
char *
USplice( const char *in, unsigned int start, unsigned int end)
{
	char *out;
	int   l, i = 0;

	if( (char *)end == NULL ){
		end = strlen(in);
	}
	if( (l = end - start) < 0 ){
		return NULL;
	}
	if( end > strlen(in)){
		return NULL;
	}
	if( (out = (char *) malloc((l + 1))) == NULL ){
		return NULL;
	}	
	for( l = start; l < (end + 1); l ++ ){
		out[i++] = in[l];
	}
	if( out[i] != '\0'){
		out[i] = '\0';
	}
	return out;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  _MacItoA_
 *  Description:  Mac does not provide a itoa function, so this function is used, Only if 
 *                the  program is getting compiled on OSX
 * =====================================================================================
 */
char *
_MacItoA_( int num )
{
	char *text;
	int result = asprintf( &text, "%d", num );
	if( result < 0 )
	{
		syslog( LOG_ERR, "error converting number to string");
		return NULL;
	}
	return text;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  _syslog_print_error
 *  Description:  Print a error
 * =====================================================================================
 */
extern void 
_syslog_print_error( unsigned int tid, char *fname, int lineno, char *m1, char *m2, int pri )
{
	syslog( pri, "%u:%s:%d - ERROR - %s - %s", tid, fname, lineno, m1, m2);
	perror( m2 );
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  _syslog_print_error_url
 *  Description:  Print a error that is specifically related to a URL
 * =====================================================================================
 */
extern void
_syslog_print_error_url(  unsigned int tid, char *fname, int lineno, char *m1, char *url, char *m2, int pri )
{
	char *mm2;
	asprintf( &mm2, "%s (%s)", m1, url);
	_syslog_print_error( tid, fname, lineno, mm2, m2, pri);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  reset_file
 *  Description:  Reset file back to the beginning, file is reopened in "w+" mode.
 * =====================================================================================
 */
inline void
reset_file ( FILE *fh )
{
	rewind(fh);
	fprintf(fh,"%d",EOF);
	freopen(NULL,"w+",fh);
}		/* -----  end of function resetFile  ----- */

