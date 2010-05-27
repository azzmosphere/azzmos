/**
 * Common Utilities.
 */

#include <Utils.h>

/**
 * splice a character strting from offset start to 
 * offset end.  If end is NULL then go to end of
 * string. '\0'
 *
 * On any error return NUL.
 */
char *
USplice( const char *in, unsigned int start, unsigned int end)
{
	char *out;
	int   l, i = 0;

	if( (char *)end == NULL )
		end = strlen(in);

	if( (l = end - start) < 0 )
		return NULL;

	if( end > strlen(in))
		return NULL;
	if( (out = (char *) malloc((l + 1))) == NULL )
		return NULL;
	
	for( l = start; l < (end + 1); l ++ )
		out[i++] = in[l];

	if( out[i] != '\0')
		out[i] = '\0';
	return out;
}

/**
 * Mac does not provide a itoa function,  
 * so this function is used, Only if the
 * program is getting compiled on OSX
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
