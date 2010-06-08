/*
 * =====================================================================================
 *
 *       Filename:  URIHeaders.c
 *
 *    Description:  Functions to allocate, read and manipulate the URIHEader_t object
 *
 *        Version:  1.0
 *        Created:  02/06/2010 22:15:28
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri <azzmosphere@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */

#include <URIHeaders.h>


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  URIHeaderInit
 *  Description:  Initilize the and construct the URIHeader_t object.  The function
 *                will return a newly malloced URIHeader with the list inilized or
 *                will return NULL 
 * =====================================================================================
 */
URIHeader_t *
URIHeaderInit ( )
{
	URIHeader_t *uh;

	uh = (URIHeader_t *) malloc( sizeof(URIHeader_t));

	if( uh )
		INIT_LIST_HEAD( &uh->uh_list );
	else 
		syslog( LOG_ERR, "could not allocate URIheaders %s", strerror( errno ));

	return uh;
}		/* -----  end of function URIHeaderInit  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  URIHeaderCleanUp
 *  Description:  Deconstruct the URIHeader object
 * =====================================================================================
 */
void
URIHeaderCleanUp ( URIHeader_t *uh )
{
       URIHeader_t *tmp;
       struct list_head *pos, *q;

       list_for_each_safe( pos, q, &uh->uh_list ) {
	       tmp = list_entry(pos, URIHeader_t, uh_list);
	       list_del( &tmp->uh_list);
	       
	       if ( tmp->uh_key ) {
		       *(tmp->uh_key) = NULL;
		       free( *(tmp->uh_key));
		       free( tmp->uh_key );
	       }

	       if( tmp->uh_val ) {
		       *(tmp->uh_val) = NULL;
		       free( *(tmp->uh_val));
		       free( tmp->uh_val );
	       }
	       free(tmp);
       }
}		/* -----  end of function URIHeaderCleanUp  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  URIHeaderAllocateElement
 *  Description:  Allocate the next element of the URIHeader object.
 * =====================================================================================
 */
static inline int
URIHeaderSetListItem( URIHeader_t *uh, URIHeader_t *tmp, const char *key, const char *val )
{
	int rv = 0;
	if( uh == NULL ) {
		syslog( LOG_ERR, 
			"could not allocate memory for URIHeader - %s", 
			strerror(errno));
		return errno;
	}
	if( (tmp->uh_key = (char **) malloc( sizeof(char *))) == NULL ) {
		syslog( LOG_ERR, "could not allocate memory for version header - %s", strerror(errno));
		return errno;
	}

	if( (tmp->uh_val = (char **) malloc( sizeof(char *))) == NULL ) {
		syslog( LOG_ERR, "could not allocate memory for version header - %s", strerror(errno));
		return errno;
	}

	*(tmp->uh_key) = strdup(key);
	*(tmp->uh_val) = strdup(val);

	list_add(&tmp->uh_list, &(uh->uh_list));

	return rv;
}		/* -----  end of static function URIHeaderAllocateElement  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  URIHeaderAllocateFromFile
 *  Description:  Allocate URI headers from a file using the URIRegex object
 *                to determine the headers.
 *
 *                On sucess return 0 on any failure return the appropriate error
 *                number
 * =====================================================================================
 */
int
URIHeaderAllocateFromFile ( URIHeader_t *uh, URIRegex_t *ue, FILE *in )
{
	int   rv = 0;
	char  buf[BUFSIZ];
	char *key, *val;
	URIHeader_t *tmp;

	rewind( in );

	if ( in == NULL ) {
		syslog( LOG_ERR, "couldn't URI header file; %s",
			strerror(errno) );
		return errno;
	}

	/* first line of headers is allways the HTTP version */
	if( fgets( buf, BUFSIZ, in ) == NULL ) {
		syslog( LOG_ERR, "could not find any headers - %s", strerror(errno));
		return errno;
	}

	if( (uh->uh_key = (char **) malloc( sizeof(char *))) == NULL ) {
		syslog( LOG_ERR, "could not allocate memory for version header - %s", strerror(errno));
		return errno;
	}

	if( (uh->uh_val = (char **) malloc( sizeof(char *))) == NULL ) {
		syslog( LOG_ERR, "could not allocate memory for version header - %s", strerror(errno));
		return errno;
	}

	*(uh->uh_key) = strdup(AHN_HTTP_OK);
	*(uh->uh_val) = USplice( buf, 0, (strnlen(buf,BUFSIZ) - 3)); 

	/* get uri headers from the file */
	while( feof(in) == false ) {
		if( (tmp = (URIHeader_t *) malloc(sizeof(URIHeader_t))) == NULL ) {
			syslog( LOG_ERR, 
				"could allocate memory for URIHeader object %s",
				strerror(errno)
			);
			break;

		}
		else if( fgets( buf, BUFSIZ, in) != NULL) {
			if( (rv = URIRegexSplitURIHeader(ue, buf, &key, &val )) != 0) {
				syslog(LOG_ERR, "could not allocate header - %s", strerror(rv));
				break;
			}
			else if( (rv = URIHeaderSetListItem( uh, tmp, key, val)) != 0) {
				syslog(LOG_ERR, "could not set list item for - %s - %s", key, strerror(rv));
				break;
			}
		}
	}

	return rv;
}		/* -----  end of function URIHeaderAllocateFromFile  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  URIHeaderHasValue
 *  Description:  Check the URIHeaders object for a list item.  If it exists then
 *                return true,  otherwise return false.  It should also mean that 
 *                list is set to item when a consquent call is made to
 *                URIHeaderGetValue
 * =====================================================================================
 */
bool
URIHeaderHasValue ( URIHeader_t *uh, const char *key )
{
	bool rv = false;
	URIHeader_t *tmp;
	struct list_head *pos;

	list_for_each(pos, &uh->uh_list) {
		tmp = list_entry( pos, URIHeader_t, uh_list);

		if( strncmp(key, *(tmp->uh_key), BUFSIZ) == 0) {
			rv = true;
			break;
		}
	}

	return rv;
}		/* -----  end of function URIHeaderHasValue  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  URIHeaderGetValue
 *  Description:  Loop through URIHeaders object and return the value of the header
 *                that has the key of key. On failure routine will return NULL.  However
 *                this should not happen because it should be qualified by the HasValue
 *                function first.
 * =====================================================================================
 */
char *
URIHeaderGetValue ( URIHeader_t *uh, const char *key )
{
	char *ret = NULL;
	URIHeader_t *tmp;
	struct list_head *pos;

	list_for_each(pos, &uh->uh_list) {
		tmp = list_entry( pos, URIHeader_t, uh_list);

		if( strncmp(key, *(tmp->uh_key), BUFSIZ) == 0) {
			ret = strdup( *(tmp->uh_val));
			break;
		}
	}

	return ret;
}		/* -----  end of function URIHeaderGetValue  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  URIHeaderAllocate
 *  Description:  Download the headers from the seed argument and then
 *                allocate them. On error return the corresponding error message
 *                otherwise return 0.
 * =====================================================================================
 */
int
URIHeaderAllocate ( URIHeader_t *uh, DownloadHTML_t *dl, URIRegex_t *re, const char *seed )
{
	int ret = 0;
	CURLcode res = CURLE_OK;

	if( (res = DownloadHTMLGetHeaders(dl, seed)) != CURLE_OK) {
		syslog( LOG_ERR, "could not download headers - %s", curl_easy_strerror( res ));
		ret = 1;
	}
	else 
		ret = URIHeaderAllocateFromFile( uh, re, DownloadHTMLGetFh(dl)); 
	
	return ret;
}		/* -----  end of function URIHeaderAllocate  ----- */
