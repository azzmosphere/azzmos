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
downloadURIPerform( 
		    DownloadHTML_t *dl, 
		    const char *seed, 
		    URIRegex_t *urirel,  
		    URIObj_t *uri, 
		    int recurse, 
		    DBSQLHandleSth_t *dbsth
)
{
	DownloadURI_t *duri = NULL;
	int urirv = 0, uriid = 0;
	CURLcode curlstatus;
	char *href;
	char *url =  URIObjGetFQP(uri);

	if( (curlstatus = performDownloadHTML( dl,url) ) != CURLE_OK){
		ERROR_C_URL("performing download",url,curlstatus);
	}
	else if( (urirv = URIObjSetContent(uri, dl)) != 0){
		ERROR_E("could not set content for URI", urirv);
	}

	/* upload URL to database */
        /* REMAINS TO BE IMPLEMENTED */
	/*  else if( (uriid = DBSQLExecFootprint( db, uri, recurse)) == 0 ) {
		ERROR("can not upload to database!!!!");
		URIObjCleanUp(uri);
		urirv = 1;
	}*/

	/* Extract Href List from content */
	else if( (duri = getHrefList(urirel, uri)) == NULL ) {
		ERROR("could not get HREF list");
		urirv = 1;
	} 
	else if ( urirv == 0 ) {
		/* set URL id */
		/* set the URIID */
		//URIObjSetId( uri, uriid);
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
extern int 
dl_uri(DownloadHTML_t *dl, 
	    URIQualify_t *uq,
	    const char *seed, 
	    URIRegex_t *urirel, 
	    int recurse, 
	    URIObj_t *uri, 
	    const Opts_t *opts,
	    DBSQLHandleSth_t *dbsth
)
{
	int       urirv = 0;
	DownloadURI_t *duri, *tmp;
	struct list_head *pos;
	char *href;
	URIObj_t *nexturi;

	DEBUG("DownloadURI started");
	DEBUG_STR("seed", seed);
	URIObjSetFQP( uri, seed );
	uriQualify(urirel, seed, uri, NULL);
	duri = downloadURIPerform(dl, seed, urirel, uri, recurse, dbsth);
	if( duri == NULL ){
		ERROR( "could not download URI");
	}
	else if( (urirv = uriQualifyDlURI(uq, urirel, duri, uri, dl, dbsth)) == 0 ){

		/* Start reading each link and transversing through them */
		list_for_each( pos, &duri->du_list ) {
			tmp  = list_entry(pos,  DownloadURI_t, du_list);
			href = DownloadURIGetHref(tmp);
			nexturi = URIObjClone(uri);
			URIObjFreeContent( nexturi );
			dl_uri( dl, uq, href, urirel, (recurse + 1), nexturi, opts, dbsth);
			free( href );
			URIObjCleanUp(nexturi);
		}	
	} 
	else {
		ERROR_E("could not allocate URI list when reading seed", urirv);
	}
	DEBUG_INT("recursion level", recurse);
	return urirv;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getHrefList
 *  Description:  Gets a list of HREF's from the URI content.
 * =====================================================================================
 */
extern DownloadURI_t *
getHrefList( URIRegex_t *reuri,  URIObj_t *uri )
{
	char *href;
	int   offset = 0, err;
	DownloadURI_t *duri = (DownloadURI_t *) malloc( sizeof(DownloadURI_t)), *tmp;

	if( duri == NULL ) {
		ERROR("can not allocate memory for URI list");
		return duri;
	}
	INIT_LIST_HEAD( &duri->du_list );
	while ( (href = URIRegexecNextHREF( reuri, URIObjGetContent(uri), &offset, uri )) != NULL) {
		tmp = (DownloadURI_t *) malloc( sizeof(DownloadURI_t));
		if( tmp == NULL ) {
			WARN("could not allocate memory for URI - skipping");
			continue;
		}
		if( err = DownloadURISetListItem( tmp, duri, href) ) {
			WARN_E( "could not add list item to URI list - skipping ", errno);
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

