/*
 * =====================================================================================
 *
 *       Filename:  URIExtract.c
 *
 *    Description: Implements PERL like regular expressions,  the primary use of this
 *                 library is for URIQualify interface,  but it can be used any time
 *                 a PERL regular expression needs to be used.
 *
 *        Version:  1.0
 *        Created:  25/05/2010 22:12:44
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */

#include <URIExtract.h>

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  URIRegexInit
 *  Description:  Initilize the Regex object
 * =====================================================================================
 */
URIRegex_t *
URIRegexInit( )
{
	int i = 0;
        URIRegex_t *reuri = (URIRegex_t *) malloc(sizeof(URIRegex_t));
        reuri->ur_opts = reuri->ur_rv   = 0;
	reuri->ur_errptr = (char **) malloc(sizeof(char *));

	/* allocate memory for RE pointers */
	reuri->ur_regex = ( pcre **) malloc( (sizeof(pcre *) * REGEX_T_LAST));
	for( i = 0; i < REGEX_T_LAST; i ++ ) 
		reuri->ur_regex[i] = NULL;

        return reuri;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  URIRegexCleanUp
 *  Description:  Deconstruct the URIHeader object
 * =====================================================================================
 */
void
URIRegexCleanUp( URIRegex_t * reuri )
{
	int i;
        if( reuri != NULL ) {
		for( i = 0; i < REGEX_T_LAST; i ++ ) {
			if( reuri->ur_regex[i] != NULL)
				pcre_free( reuri->ur_regex[i] );
		}
		reuri->ur_regex = NULL;
		free( reuri->ur_regex );
        }
	
	*reuri->ur_errptr = NULL;
	free( *reuri->ur_errptr);
	reuri->ur_errptr = NULL;
	free(reuri->ur_errptr);

        reuri = NULL;
        free( reuri );
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  URIRegexCompile
 *  Description:  Compile the regular expression, if an error occurs handle it and 
 *                return the offset.
 * =====================================================================================
 */
static pcre *
URIRegexCompile( URIRegex_t *reuri, const char *pattern, long opts, regex_types regex )
{

	pcre *re = pcre_compile(
		pattern,
		PCRE_CASELESS,
		(const char **)reuri->ur_errptr,
		&reuri->ur_erroffset,
		NULL
	);

	if( ! re ) {
		syslog(LOG_ERR, "%s - %s:%d (%d) could not compile re - %s",
				__FILE__,
				"URIRegexCompile",
				__LINE__,
				reuri->ur_erroffset,
				*reuri->ur_errptr
		);
		syslog(LOG_DEBUG, "using pattern - '%s'", pattern);
	} 
	else {
		reuri->ur_opts |= opts;
		reuri->ur_regex[regex] = re;
	}
	return re;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  URIRegexExec
 *  Description:  Execute the regular expression.   return the fist match as string
 *                from  offset ur_suboffset and reset ur_suboffset to the end of first 
 *                match.  On any return NULL and print the error.
 * =====================================================================================
 */
static char *
URIRegexExec( URIRegex_t *ruri, 
	      pcre *code, 
	      const char *subject, 
	      long opts, 
	      int *offset, 
	      int ovecsize,
	      int  *ovector )
{
	char *rstr = NULL;
	int rv = 0;
	
	if( ( rv = pcre_exec( code, 
		  	NULL, 
			subject, 
			strnlen(subject, BUFSIZ), 
			*offset,
			opts,
			ovector,
			ovecsize
	)) < 0) {
		switch( rv ) {
			case PCRE_ERROR_NOMATCH:
				WARN("no match found");
				break;
			default:
				ERROR("unknown reguler expr error");
				break;

		}
		return rstr;
	}
	rstr = (char *) USplice( subject, (ovector[0] + *offset ), (ovector[1] + *offset -1));
	*offset += ovector[1];
	return rstr;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ProcRegEx
 *  Description:  Process the regular expression,  that is compile it if the regular 
 *                was not previously compiled and execute.  Return the resultent of 
 *                URIRegexExec, or NULL on failure.
 * =====================================================================================
 */
inline static char *
ProcRegEx( URIRegex_t *reuri, 
	  const char *subject, 
	  URIObj_t *uri, 
	  const char * pattern, 
	  const char *func,
	  int bopt,
	  regex_types rtype,
	  int *roffset,
	  int ovecsize,
	  int *ovector
)
{
	pcre *re;
	char * rv = NULL;
	
	if( reuri->ur_opts & bopt )
		re = reuri->ur_regex[rtype];
	else if( (re = URIRegexCompile( reuri, pattern, bopt, rtype)) == NULL) {
		syslog( LOG_ERR, 
			"ERROR: %s - %d -%s", 
			func,
			uri->uri_content_offset,
			*reuri->ur_errptr
		);
		return NULL;
	}
	rv = URIRegexExec( reuri, re, subject,  0, roffset, ovecsize, ovector);
	return rv;

}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  URIRegexecNextHREF
 *  Description:  Return the next HREF in subject startoing at position offset.
 * =====================================================================================
 */
char * 
URIRegexecNextHREF( URIRegex_t *reuri, const char *subject, int *offset, URIObj_t *uri )
{
	const char *pattern = "href\\s*=\\s*\"([\\w\\.\\/\\:\\?\\=\\%\\;\\&]+)\"";
	int ovecsize = 30, ovector[ovecsize];
	char *href = ProcRegEx( reuri, 
			  subject, 
			  uri, 
			  pattern, 
			  "URIRegexNextHREF", 
			  O_REHREF, 
			  REGEX_T_HREF, 
			  &uri->uri_content_offset,
			  ovecsize,
			  ovector); 

	if( href != NULL ) {
		free( href );
		href =  (char *) USplice(subject, ovector[2], (ovector[3] -1));
		*offset  = ovector[3];
	}
	return href;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  IsFQP
 *  Description:  allocate the Fully Qualified Path (FQP) from a given seed, if the seed 
 *                is not a FQP then return false, otherwise return true. 
 *
 *                A Fully Qualified Path is one that has all three sections, they are:
 *                the scheme, authority and path. 
 * =====================================================================================
 */
bool 
IsFQP( URIRegex_t *reuri, const char *subject, URIObj_t *uri)
{
	const char *pattern = "^([\\w\\d\\+\\-\\.]+)://([\\w\\d\\.]+:?\\d*)([/\\?#][\\w/\\.\\d\\?\\=\\%\\;]*)$";
	int offset = 0, ovecsize = 30, ovector[ovecsize];
	bool rc = false;
	char *rv =  ProcRegEx(
		reuri,
		subject,
		uri,
		pattern,
		"(private function) IsFQP",
		O_REFQP,
		REGEX_T_FQP,
		&offset,
		ovecsize,
		ovector
	);
	if( rv != NULL ) {
		rc = true;
		uri->uri_scheme_so = ovector[2];
		uri->uri_scheme_eo = (ovector[3] -1);
		uri->uri_auth_so = ovector[4];
		uri->uri_auth_eo = (ovector[5] -1);
		uri->uri_path_so = ovector[6];
		uri->uri_path_eo = (ovector[7] -1);
	}

	return rc;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  IsPathNq
 *  Description:  For valid paths that do not have a '/' at start match them here.
 * =====================================================================================
 */
bool
IsPathNq ( URIRegex_t *reuri, const char *subject, URIObj_t *uri, char **out )
{
	const char *pattern = "^\\s*([\\w\\d-\\._\\~\\?\\#]+)\\s*$"; 
	bool rc = false;
	int offset = 0, ovector[O_OVECCOUNT];
	char *rv =  ProcRegEx(
		reuri,
		subject,
		uri,
		pattern,
		"(private function) IsPathNq",
		O_REUPTH,
		REGEX_T_UPTH,
		&offset,
		O_OVECCOUNT,
		ovector
	);

	if( rv != NULL ) {
		rc = true;
		*(out) = USplice( subject, ovector[2], (ovector[3] -1));
	}

	return rc;
}		/* -----  end of function IsPathNq  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  IsPathOnly
 *  Description:  
 *  
 *   Allthough RFC3986 states that path-abempty is a path that "begins with "/" or is empty",
 *  this project looks at path path abempty as a path that does that begins with "/" but
 *  does not have any other characters. The following test therefore check to see if,
 *  the the seed begins with a "/" and not "//",  if it does and luri is not null then
 *  it constructs the fully qualified path as:
 *  URIObjGetScheme( luri) + URIObjGetAuth( luri ) + seed,
 *
 * =====================================================================================
 */
bool
IsPathOnly( URIRegex_t *reuri, const char *subject, URIObj_t *uri)
{
	char *pattern = strdup("^[/\\?#][^/][\\w]*");
	int offset = 0, ovecsize = 30, ovector[ovecsize];
	bool rc = false;
	char *rv = ProcRegEx(
		reuri,
		subject,
		uri,
		pattern,
		"(private) IsPathOnly",
		O_REISP,
		REGEX_T_ISP,
		&offset,
		ovecsize,
		ovector
	);
	if( rv != NULL )
		rc = true;
	return rc;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  IsNoPath
 *  Description:  Return true if the string is defined as no_path
 *             
 * =====================================================================================
 */
bool
IsNoPath(URIRegex_t *reuri, const char *subject, URIObj_t *uri)
{
	const char *pattern = "^([\\w\\d\\+\\-\\.]+:)(//[\\w\\d\\.]+:?\\d*)$";
	int offset = 0, ovecsize = 30, ovector[ovecsize], ssize = strlen(subject);
	bool rc = false;
	char *rv = ProcRegEx(
		reuri,
		subject,
		uri,
		pattern,
		"(private) IsPathOnly",
		O_RENOP,
		REGEX_T_NOP,
		&offset,
		ovecsize,
		ovector
	);
	if( (rv != NULL) )
		rc = true;

	return rc;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  URIRegexSplitURIHeader
 *  Description:  When subject is URI header then split it into two parts based upon 
 *                the ':', on success return '0' otherwise return the appropriate
 *                error message and key and val remain unset.
 * =====================================================================================
 */
int
URIRegexSplitURIHeader ( URIRegex_t *reuri, const char *subject, char **key, char **val)
{
	int rv = 0, offset = 0, ovector[O_OVECCOUNT]; 
	const char *pattern = "^\\s*([^:]+[^\\s]*)\\s*:\\s*([^\\r]+)\\s*";
	const char *re = ProcRegEx(
		reuri,
		subject,
		NULL,
		pattern,
		"(public) URIRegexURIHeader",
		O_REHDR,
		REGEX_T_HDR,
		&offset,
		O_OVECCOUNT,
		ovector
	);

	if( re == NULL ) {
		rv = errno;
	}
	else {
		*(key) = USplice( subject, ovector[2], (ovector[3] - 1));
		*(val) = USplice( subject, ovector[4], (ovector[5] - 1));
	} 

	return rv;
}		/* -----  end of function URIRegexSplitURIHeader  ----- */



