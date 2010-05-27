/*
 * =====================================================================================
 *
 *       Filename:  URIQualifier.c
 *
 *    Description:  Qualifiers a URI before it parsed to the DownloadURI function.
 *
 *        Version:  1.0
 *        Created:  19/05/2010 20:28:20
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <URIQualifier.h>

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  URIQualify
 *  Description:   The URIQualify function is an interface to the URI FQP Qualification.
 *  on success the it returns the FQP path and sets the URI fqp on failure it retuns
 *  NULL and leaves the URI object untouched.
 * =====================================================================================
 */
char *
URIQualify(  URIRegex_t *urire, const char *seed,  URIObj_t *uri,  URIObj_t *luri) 
{
	char *fqp = NULL;
	int err = 0;
	
	
	if( (fqp = URIQualifyGetFQP(urire, seed, uri)) == NULL ) 
		syslog( LOG_ERR, "could not set Fully Qualified Path");	
	else
		if( err = URIObjSetFQP( uri, fqp) ) 
			syslog( LOG_ERR, "could not allocate FQP - %s", strerror( err ));


	/* Normalize string */

	/* test sanity of authority using DNS */

	syslog( LOG_DEBUG, "URIQualifier set FQP to '%s'", fqp);
	return fqp;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  URIQualifyGetFQP
 *  Description:  Attempts to create a FQP path from the seed and URI arguments.
 *                On success it will return FQP, otherwise NULL is returned.
 * =====================================================================================
 */
char *
URIQualifyGetFQP ( URIRegex_t *urire, const char *seed,  URIObj_t *uri )
{
	char *fqp = NULL;
	int err = 0;
	
	if( IsFQP( urire, seed, uri ) )
		fqp = strdup( seed );
	
	/* test if this a PATH_ONLY seed, is so try and 
	 * get the scheme and auht from the uri */
	else if( IsPathOnly( urire, seed, uri) ) {
		if( URIObjGetScheme( uri ) && URIObjGetAuth( uri ) )
		       asprintf(&fqp, 
				"%s://%s%s",  
				 URIObjGetScheme( uri ),  
				 URIObjGetAuth( uri ), seed);	
	} 
	else if( IsNoPath( urire, seed, uri ) )
		asprintf(&fqp, "%s/", seed);

	/* some sites will use a non standard path such as 'ml.asp?45937'  */
	else if( strnlen( seed, BUFSIZ) < BUFSIZ ) {
	  	if( asprintf(&fqp, 
			 "%s://%s/%s", 
			 URIObjGetScheme( uri ),
			 URIObjGetAuth( uri ), 
			 seed
		) == -1 ) {
			fqp = NULL;
			syslog( LOG_ERR, 
				"when trying to get FQP - the following happened - %s", 
				strerror( errno)
			);
		}

	}

	return fqp;
}		/* -----  end of function URIQualifyGetFQP  ----- */
