/*
 * =====================================================================================
 *
 *       Filename:  DownloadURI.c
 *
 *    Description:  Implements the DownloadURI algoritm which:
 *
 *    1) ** Qualifies a URI object into a Fully Qualified Path (FQP)
 *    2) ** Checks that the FQP meets the standards of the politeness policy
 *    3) Uploads the content to the database.
 *    4) Extracts Hrefs within the content.
 *    5) Recursively calls itself on each extracted HREF.  
 *
 *        Version:  1.0
 *        Created:  24/05/2010 22:19:57
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <DownloadURI.h>



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  DownloadURIPerform
 *  Description: Performs the download of the URI and the upload to the database
 * =====================================================================================
 */
DownloadURI_t *
DownloadURIPerform( 
		    DownloadHTML_t *dl, 
		    const char *seed, 
		    URIRegex_t *urirel,  
		    URIObj_t *uri, 
		    int recurse, 
		    DBObj_t *db
)
{
	DownloadURI_t *duri = NULL;
	int urirv = 0, uriid = 0;
	CURLcode curlstatus;
	char *href;

	if( (curlstatus = PerformDownloadHTML( dl, URIObjGetFQP(uri)) ) != CURLE_OK)
		syslog(LOG_ERR, "%s - %s -%s", __FILE__, "CURL", curl_easy_strerror( curlstatus));

	else if( (urirv = URIObjSetContent(uri, dl)) != 0)
		SYSLOG_ERR("DownloadURI","URI" ,urirv);

	/* upload URI to database */
	else if( (uriid = DBSQLExecFootprint( db, uri, recurse)) == 0 ) {
		syslog(LOG_CRIT, "can not upload to database!!!!");
		URIObjCleanUp(uri);
		urirv = 1;
	}
	/* Extract Href List from content */
	else if( (duri = GetHrefList(urirel, uri)) == NULL ) {
		syslog(LOG_ERR, "could not get HREF list");
		urirv = 1;
	} 
	else if ( urirv == 0 ) {
		/* set the URIID */
		URIObjSetId( uri, uriid);
		URIObjFreeContent( uri );	
	}
	
	if( curlstatus != CURLE_OK || urirv != 0 ) 
		return NULL;
	
	return duri;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  DownloadURI
 *  Description:  This is the main interface, it should be used as the access point
 *                to access the algoritm unless performing unit tests, note this is 
 *                a recursive algoritm.
 * =====================================================================================
 */
int 
DownloadURI(DownloadHTML_t *dl, 
	    URIQualify_t *uq,
	    const char *seed, 
	    URIRegex_t *urirel, 
	    int recurse, 
	    URIObj_t *uri, 
	    DBObj_t *db, 
	    const Opts_t *opts
)
{
	int       urirv = 0;
	DownloadURI_t *duri, *tmp;
	struct list_head *pos;
	char *href;
	URIObj_t *nexturi;

	syslog(LOG_DEBUG, "%s:%d - DownloadURI started", __FILE__, __LINE__);
	URIObjSetFQP( uri, seed );

	if( (duri = DownloadURIPerform(dl, seed, urirel, uri, recurse, db)) == NULL )
		SYSLOG_ERR( "DownloadURI", "could not download URI", errno);
	else if( (urirv = URIQualifyDlURI(uq, urirel, duri, uri, dl)) == 0 ){


		/* Start reading each link and transversing through them */
		list_for_each( pos, &duri->du_list ) {
			tmp  = list_entry(pos,  DownloadURI_t, du_list);
			href = DownloadURIGetHref(tmp);
			
			nexturi = URIObjClone(uri);
			URIObjFreeContent( nexturi );

			syslog( LOG_DEBUG, "href is = '%s'",  href);

			DownloadURI( dl, uq, href, urirel, (recurse + 1), nexturi, db, opts);
			free( href );
			URIObjCleanUp(nexturi);

		}	
	} 
	else {
		syslog( LOG_ERR, 
			"could not allocate URI list when reading seed - %s - %s", 
			seed,
			strerror( urirv )
		);
	}

	syslog( LOG_DEBUG, "recursion level = '%d'", recurse);
	return urirv;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  GetHrefList
 *  Description:  Gets a list of HREF's from the URI content.
 * =====================================================================================
 */
DownloadURI_t *
GetHrefList( URIRegex_t *reuri,  URIObj_t *uri )
{
	char *href;
	int   offset = 0, err;
	DownloadURI_t *duri = (DownloadURI_t *) malloc( sizeof(DownloadURI_t)), *tmp;

	if( duri == NULL ) {
		syslog( LOG_ERR, "can not allocate memory for URI list - %s", strerror(errno));
		return duri;
	}

	INIT_LIST_HEAD( &duri->du_list );
	while ( (href = URIRegexecNextHREF( reuri, URIObjGetContent(uri), &offset, uri )) != NULL) {
		tmp = (DownloadURI_t *) malloc( sizeof(DownloadURI_t));
		if( tmp == NULL ) {
			syslog( LOG_WARNING, "could not allocate memory for URI - skipping");
			continue;
		}
		if( err = DownloadURISetListItem( tmp, duri, href) ) {
			syslog( LOG_WARNING, "could not add list item to URI list - skipping %s", strerror(errno));
			continue;
		}
	}
	return duri;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  DownloadURISetListItem
 *  Description:  Append this href to the existing list.
 * =====================================================================================
 */
int 
DownloadURISetListItem(DownloadURI_t *tmp, DownloadURI_t *duri,  char *href)
{
	int rv = 0;

	tmp->du_href = ( char **) malloc( sizeof( char *));
	if( tmp->du_href == NULL) 
		return errno;

	*(tmp->du_href) = strdup(href);
	list_add(&(tmp->du_list), &(duri->du_list));
	return rv;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  DownloadURICleanUp
 *  Description:  
 * =====================================================================================
 */
void
DownloadURICleanUp ( DownloadURI_t *du )
{
 /* REMAINS TO BE IMPLEMENTED */
}		/* -----  end of function DownloadURICleanUp  ----- */

