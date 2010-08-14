/*
 * =====================================================================================
 *
 *       Filename:  URIObj.c
 *
 *    Description: The URI object is used to store and conain parts of a URI including
 *    the Fully Qualified Path (FQP), content and the internal uriid` 
 *
 *        Version:  1.0
 *        Created:  24/05/2010 22:43:23
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <URIObj.h>

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  URIObjFreeElement
 *  Description:  Internal function that free's a element of the URI object
 * =====================================================================================
 */
void 
URIObjFreeElement( char **uriel )
{
	if( uriel != NULL ) {
		*uriel = NULL;
		free( *uriel );
	}
	uriel = NULL;
	free( uriel );
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  URIObjInit
 *  Description:  Construct the URI object
 * =====================================================================================
 */
URIObj_t *
URIObjInit()
{
	URIObj_t *uri = (URIObj_t *) malloc(sizeof( URIObj_t));
	if( uri == NULL ) {
		ERROR("could not allocate memory for URIObj");
	} 
	else if ( ( uri->uri_content = (char **) malloc(sizeof( char *))) == NULL) {
		ERROR( "could not allocate memory for content");
		free( uri );
		uri = NULL;
	} 
	else if( (uri->uri_fqp = (char **) malloc( sizeof( char *))) == NULL) {
		ERROR( "could not allocate memory for FQP");
		free( uri->uri_content);
		free(uri);
		uri = NULL;
	}
	if ( uri == NULL ){
		return uri;
	}
	uri->uri_uh = NULL;
	uri->uri_scheme_so = uri->uri_scheme_eo 
		           = uri->uri_auth_so 
			   = uri->uri_auth_eo 
			   = uri->uri_path_so 
			   = uri->uri_path_eo 
			   = uri->uri_content_offset
			   = uri->uri_id = 0;
	return uri;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  NormalizeFQP
 *  Description:  Convert the authority section of the FQP to lower case and return
 *                it as character array
 * =====================================================================================
 */
char *
NormalizeFQP( URIObj_t *uri )
{
	char *fqp = strdup(*uri->uri_fqp);
	int i = 0;
	for( ; i < uri->uri_path_so; i ++ )
		if( isupper(fqp[i]) )
			fqp[i] = tolower( fqp[i]);

	if( URIObjSetFQP( uri, fqp) != 0)
		return NULL;	
	return fqp;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  URIObjCleanUp
 *  Description:  Deconstruct the URI object
 * =====================================================================================
 */
void
URIObjCleanUp( URIObj_t *uri )
{
	if ( uri != NULL ) {
		if( uri->uri_content != NULL ) {
			*(uri->uri_content) = NULL;
			free( *(uri->uri_content) );
			uri->uri_content = NULL;
			free(uri->uri_content);
		}

		if( uri->uri_fqp != NULL ) {
			*(uri->uri_fqp) = NULL;
			free( *(uri->uri_fqp) );
			uri->uri_fqp = NULL;
			free( uri->uri_fqp);
		}

		/* note that the headers are not initilized the downloader will
		 * allocate these for us */
		if( uri->uri_uh != NULL ) {
			URIHeaderCleanUp( uri->uri_uh );
		}
	} 
	uri = NULL;
	free( uri );
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  URIObjSetFQP
 *  Description:  Sets the Fully Qualified Path (FQP) of a specific URI and returns
 *                0 on success or EFAULT on error
 * =====================================================================================
 */
int 
URIObjSetFQP( URIObj_t *uri, const char *fqp)
{
	int rv = 0;

	if( fqp == NULL ) {
		ERROR_B( "NULL FPQ", strerror(EFAULT));
		rv = EFAULT;
	}
	/* Value should be one less then BUFSIZ to allow for '\0' character in string */
	else if( strlen( fqp ) >= BUFSIZ) {
		ERROR_B( "FPQ exceeds BUFSIZ in length", strerror(EFAULT));
		rv = EFAULT;
	} 
	else {
		*(uri->uri_fqp) = strdup( fqp );
		if( *uri->uri_fqp == NULL ) {
			ERROR("could not add FQP to URI");
			rv = errno;
		}
	}

	return rv;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  URIObjSetContent
 *  Description:  Read file handle and allocate contents dynamically to the content 
 *                string.
 * =====================================================================================
 */
int
URIObjSetContent( URIObj_t *uri, DownloadHTML_t *dl_t)
{
	int     rv = 0;
	char    buf[BUFSIZ];
	size_t  blen = 0;
	FILE   *io_read = DownloadHTMLGetFh( dl_t );

	if( uri == NULL ||  io_read == NULL) {
		if( uri == NULL){
			ERROR("invalid URI object");
		}
		else{
			ERROR_F(DownloadHTMLGetFname( dl_t ));
		}
		return  EFAULT;
	}
	rewind( io_read );
	if( *uri->uri_content != NULL)
		*uri->uri_content = NULL;
	free( *uri->uri_content);

	clearerr(io_read);

	while( (feof(io_read) == false)  && (ferror(io_read) == 0)  && (rv == 0)) {
		if( fgets( buf, BUFSIZ, io_read) == NULL) {
			/* Don't turn this into a error condition, it may be legit */
			WARN("DL file returned NULL");
		}
		else {
			blen = strnlen(buf, BUFSIZ) + 1;
			/* Initial read  */
			if ( *uri->uri_content == NULL && (*uri->uri_content = (char *) malloc( blen ))) {
				strncpy(*uri->uri_content, buf, blen);
			}
			/* failed initlization */
			else if( *uri->uri_content == NULL ) {
				ERROR("could not create content"); 
			}

			/* Append data */
			else if( (*uri->uri_content = (char *) realloc( *uri->uri_content, (strlen(*uri->uri_content) + blen))) == NULL) {
				ERROR("could not reallocate memory");
				rv = errno;
			}
			else{
				strncat( *uri->uri_content, buf, blen);
			}
		}

	}

	return rv;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  URIObjClone
 *  Description:  Clones a URI
 * =====================================================================================
 */
URIObj_t * 
URIObjClone( URIObj_t *uriin )
{
	URIObj_t *uriout = URIObjInit();
	URIObjSetFQP( uriout, URIObjGetFQP( uriin) );
	if( *uriin->uri_content != NULL ){
		*uriout->uri_content = strdup( *uriin->uri_content);
	}
	uriout->uri_scheme_so = uriin->uri_scheme_so;
	uriout->uri_scheme_eo = uriin->uri_scheme_eo;
	uriout->uri_auth_so = uriin->uri_auth_so;
	uriout->uri_auth_eo = uriin->uri_auth_eo;
	uriout->uri_path_so = uriin->uri_path_so;
	uriout->uri_path_eo = uriin->uri_path_eo;
	return uriout;
}

