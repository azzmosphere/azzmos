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
 *         Author:  Aaron Spiteri
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
	if( (fqp = NormalizeFQP( uri )) == NULL ) {
		syslog( LOG_ERR, "could not normalize FQP");
	}

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
	int   err = 0;
	char *path;
	
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
	else if( IsPathNq(urire,seed, uri, &path) ) {
	  	asprintf(&fqp, 
			 "%s://%s/%s", 
			 URIObjGetScheme( uri ),
			 URIObjGetAuth( uri ), 
			 path
		) ;
	}

	return fqp;
}		/* -----  end of function URIQualifyGetFQP  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  URIQualifyInit
 *  Description:  Initilize a URIQualify_t object, on success this function will return
 *  the newly created object.  On failure it will return NULL. The objects are used
 *  to exclude any FQP from being downloaded.
 * =====================================================================================
 */
URIQualify_t *
URIQualifyInit ( )
{
	URIQualify_t *uq = ( URIQualify_t * ) malloc( sizeof( URIQualify_t ));
	
	if( uq != NULL ) {
		uq->uq_fqp = (char **) malloc( sizeof(char *));
	}
	return uq;
}		/* -----  end of function URIQualifyInit  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  URIQualifyAppend
 *  Description:  Appends a new element to the URIQualify_t list. Because this object
 *                is shared among all downloaders it is uses mutex_lock to ensure that 
 *                it is thread safe.
 *
 *  On success this routine *  will return 0, otherwise it will return the 
 *  value of errno.
 * =====================================================================================
 */
int
URIQualifyAppend ( URIQualify_t *uqin, const char *seed )
{
	int rv = 0;
	URIQualify_t *uq = URIQualifyInit();

	if( (rv = pthread_mutex_lock( URIQualifyGetLock( uqin ))) == 0 ) {

		if( uq == NULL )
			rv = errno;
		else {
			URIQualifySetFqp( uq, seed );
			URIQualifySetLock( uq,  URIQualifyGetLock( uqin ));
			list_add( &(uq->uq_list), &(uqin->uq_list));
		}
	}

	/*  unlock the object */
	if( rv == 0 ) {
		rv = pthread_mutex_unlock( URIQualifyGetLock( uqin ));
		if ( rv == EPERM ) {
			syslog( LOG_WARNING, "URIQualifyAppend - %s - %s", seed, strerror( rv));
			rv = 0;
		}

	}

	return rv;
}		/* -----  end of function URIQualifyAppend  ----- */



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  URIQualifyChkHeaders
 *  Description:  Download and test the URIHeaders with previously downloaded headers.
 *                On any rejected reason return a non 0 value otherwise return 0. 
 * =====================================================================================
 */
inline static int 
URIQualifyChkHeaders ( URIHeader_t *uh, const char *href, DownloadHTML_t *dl, URIRegex_t *re )
{
	int   res = 0, i = 0;
	char *valid_ctype[UQ_VALID_CTYPE_COUNT] = UQ_VALID_CTYPES, *ct;

	if( (res = URIHeaderAllocate( uh, dl, re, href)) == 0) {

		if( !URIHeaderHasValue( uh, AHN_CONTENT_TYPE)) {
			syslog( LOG_ERR, "%s header is not defined", AHN_CONTENT_TYPE);
			res = 1;
		}
		else 
			ct = URIHeaderGetValue( uh, AHN_CONTENT_TYPE);
	}
	
	syslog( LOG_ERR,"%s:%s - %d res to '%d'", __FILE__, "URIQualifyChkHeaders", __LINE__, res);
	
	if( res == 0) {
		res = 1;
		for( i = 0; i < UQ_VALID_CTYPE_COUNT; i ++ ) {
			if( strncmp( ct, valid_ctype[i], BUFSIZ) == 0)	{
				res = 0;
				break;
			}
		}
	}
	return res;
}		/* -----  end of function URIQualifyChkHeaders  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  URIQualifyDlURI
 *  Description:  This function is called by the DownloadURI algoritm to qualify a list
 *                of href's known has a DownloadURI_t object.  
 *
 *                If a FQP allready exists on the list it will not be appended.
 *                
 *                If the object fails to be qualified by the URIQualify routine it
 *                will be removed from the DownloadURI_t list.  
 *
 *                The final part of this function is to check with the database when
 *                the object was last downloaded and remove any objects from the list
 *                that have been downloaded to recently.
 *
 *                On success this routine will return 0, otherwise the appropriate 
 *                error will be returned.
 *
 * =====================================================================================
 */
int
URIQualifyDlURI ( URIQualify_t *uq, 
		  URIRegex_t *urire, 
		  DownloadURI_t *duri, 
		  URIObj_t *uri, 
		  DownloadHTML_t *dl, 
		  DBObj_t *db )
{
	int rv = 0;
	DownloadURI_t *duri_tmp;
	struct list_head *pos;
	struct list_head *uq_pos, *q;
	char * href;
	URIObj_t *nexturi;
	URIQualify_t *uq_tmp;
	URIHeader_t  *uh;
	bool found = false;

	list_for_each_safe( pos, q, &duri->du_list ) {
		duri_tmp  = list_entry(pos,  DownloadURI_t, du_list);
		href = DownloadURIGetHref(duri_tmp);

		nexturi = URIObjClone(uri);
		URIObjFreeContent( nexturi );

		if( (href = URIQualify( urire, href , nexturi, NULL)) != NULL ) { 
			found = false;

			/* upload the edge here this will also give a URI id that can be
			 * used later on. */
			/* upload reposnse and headers */
			/* REMAINS TO BE IMPLEMENTED */
			syslog( LOG_DEBUG, "%s:%d - dbupload uri     = '%s'", __FILE__, __LINE__,URIObjGetFQP(uri));
			syslog( LOG_DEBUG, "%s:%d - dbupload nexturi = '%s'", __FILE__, __LINE__,URIObjGetFQP(nexturi));


			/* locking is not taken to seriosly at this point as we are not
			 * writing to the list */

			/*  FROM THIS SECTION BEYOND WE WILL REPLACE WITH 
			 *  THE MD5 ALGORITM */
			pthread_mutex_lock( URIQualifyGetLock( uq ) );

			list_for_each( uq_pos, &uq->uq_list ) {
				uq_tmp = list_entry(uq_pos, URIQualify_t, uq_list);
				
				if( strncmp( href, URIQualifyGetThisFqp(uq_tmp), BUFSIZ) == 0 ) {
					found = true;
					break;
				}
			}
			/*  ABOVE WILL BE REPLACED WITH MD5 */

			pthread_mutex_unlock( URIQualifyGetLock( uq ));
			if( ! found ) {
				uh = URIHeaderInit();
				if( URIQualifyChkHeaders( uh, href, dl, urire) != 0) {
					found = true;
					URIHeaderCleanUp(uh);
				}
				else 
					duri_tmp->du_uh = uh;
			}

			if( found ) {
				list_del(&duri_tmp->du_list );
				DownloadURICleanUp( duri_tmp );
				free( duri_tmp );
			}
			else {
				URIQualifyAppend( uq, href );
				DownloadURISetHref( duri_tmp, href );


				/*  upload response and headers */
				syslog( LOG_DEBUG, "%s:%d - response = '%s'", __FILE__, __LINE__, URIHeaderGetValue(uh, AHN_HTTP_RESPONSE));
			}

		}
		else {
			list_del( &duri_tmp->du_list );
			DownloadURICleanUp( duri_tmp );
		}
		URIObjCleanUp(nexturi);
	}

	return rv;
}		/* -----  end of function URIQualifyDlURI  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  CleanUpURIQualifyElement
 *  Description:  
 * =====================================================================================
 */
static inline void 
CleanUpURIQualifyElement( URIQualify_t *uq )
{
	if( *(uq->uq_fqp) != NULL ) {
		free( *uq->uq_fqp );
	}
	free( uq->uq_fqp );
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  CleanUpURIQualify
 *  Description:  
 * =====================================================================================
 */
void 
CleanUpURIQualify( URIQualify_t *uq )
{
	struct list_head *pos;
	struct list_head *uq_pos, *q;
	URIQualify_t *uq_tmp;

	list_for_each_safe( uq_pos, q, &uq->uq_list ) {
		uq_tmp = list_entry(uq_pos, URIQualify_t, uq_list);
		list_del( &uq_tmp->uq_list );
		CleanUpURIQualifyElement( uq_tmp);
		free( uq_tmp );
	}

}


