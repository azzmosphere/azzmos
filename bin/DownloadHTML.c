/*
 * =====================================================================================
 *
 *       Filename:  DownloadHTML.c
 *
 *    Description:  Downloads a HTML page or URI headers using cURL.  
 *
 *        Version:  1.0
 *        Created:  01/06/2010 22:09:36
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <DownloadHTML.h>


/* #####   FUNCTION DEFINITIONS  -  LOCAL TO THIS SOURCE FILE   ##################### */

static size_t downloadHTMLWriteData(void *ptr, size_t size, size_t nmemb, void *stream);


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  InitDownloadHTML
 *  Description:  Initilize the DownloadHTML object 
 * =====================================================================================
 */
DownloadHTML_t * 
InitDownloadHTML( const Opts_t *opts )
{
	DownloadHTML_t *dl_t;
	CURLcode res;

	if( (dl_t = (DownloadHTML_t *) malloc( sizeof(DownloadHTML_t))) == NULL ) {
		ERROR("could not allocate memory");

	}
	else if( (dl_t->dl_fname = (char **) malloc(sizeof( char *))) == NULL) {
		ERROR("could not allocate memory for tmp file pointer");
		free(dl_t);
		dl_t = NULL;
	}
	else if( (*dl_t->dl_fname  = strdup(tempnam(opts->o_tdir, NULL))) == NULL ) {
		ERROR("could not get temporary file");
		free(dl_t->dl_fname);
	       	free(dl_t);
       		dl_t = NULL;
	}
	else if( (dl_t->dl_fh = fopen(*dl_t->dl_fname, "w+")) ==  NULL) {
		ERROR("could not create temp file");
		free(dl_t);
		dl_t = NULL;
	}	
	else if( (dl_t->dl_ch = curl_easy_init()) == NULL ) {
		ERROR("could not initilise CURL handle");
		fclose(dl_t->dl_fh);
		free(dl_t);
		dl_t = NULL;
	}
	else if( (res = curl_easy_setopt( dl_t->dl_ch, CURLOPT_WRITEFUNCTION,  downloadHTMLWriteData)) != CURLE_OK) {
		ERROR_C("setting write function", res);
		cleanUpDownloadHTML( dl_t );
	}
	else if( (res = curl_easy_setopt( dl_t->dl_ch, CURLOPT_WRITEDATA, dl_t->dl_fh)) != CURLE_OK) {
		ERROR_C("setting write pointer", res);
		cleanUpDownloadHTML( dl_t );
	}
	else {
		dl_t->dl_result = CURLE_OK;
	}
	return dl_t;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  PerformDownloadHTML
 *  Description: Downloads a HTML page 
 * =====================================================================================
 */
CURLcode 
performDownloadHTML( DownloadHTML_t *dl_t, const char *url )
{
	CURLcode res;
	reset_file( dl_t->dl_fh);
	res = curl_easy_setopt(dl_t->dl_ch, CURLOPT_URL, url);
	if( res != CURLE_OK){
		ERROR_C_URL("could not set URL",url,res);
	}
	else if( (res = curl_easy_perform(dl_t->dl_ch)) != CURLE_OK) {
		ERROR_C_URL("could not download url", url, res);
	}
	return res;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  cleanUpDownloadHTML
 *  Description:  Destroy the DownloadHTML object
 * =====================================================================================
 */
void
cleanUpDownloadHTML( DownloadHTML_t *dl_t )
{
	if( dl_t != NULL ) {
		curl_easy_cleanup( dl_t->dl_ch );
		fclose( dl_t->dl_fh);
		unlink( *dl_t->dl_fname);
		free( *dl_t->dl_fname);
		free( dl_t );
		dl_t = NULL;
	}
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  downloadHTMLWriteData
 *  Description:  Write data to file handle (callback function for cURL)
 * =====================================================================================
 */
static size_t 
downloadHTMLWriteData(void *ptr, size_t size, size_t nmemb, void *stream)
{
        int written = fwrite(ptr, size, nmemb, (FILE *)stream);
        return written;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  downloadHTMLGetHeaders
 *  Description:  Download the headers for a specific URI, the request method is set to
 *                HEAD here but that is done through the CURL_NOBODY option instead of 
 *                directly for consistancy.
 * =====================================================================================
 */
CURLcode
downloadHTMLGetHeaders ( DownloadHTML_t *dl, const char *uri )
{
	CURLcode res = CURLE_OK;
	CURL    *cl  = DownloadHTMLGetCH(dl);
	FILE    *fh  = DownloadHTMLGetFh(dl);

	reset_file( fh );
	if(  (res = curl_easy_setopt( cl, CURLOPT_NOBODY, true )) != CURLE_OK ) {
		ERROR_C("could not get headers", res);
	}
	else if ( (res = curl_easy_setopt( cl, CURLOPT_URL, uri )) != CURLE_OK ){
		ERROR_C_URL("could not set URL", uri, res);
	}
	else if ( (res = curl_easy_setopt( cl, CURLOPT_WRITEHEADER, fh)) != CURLE_OK) {
		ERROR_C_URL("could not set file handle for headers", uri, res);
	}
	else{	
		res = curl_easy_perform( cl );
	}
	if( res == CURLE_OK) {
		res = curl_easy_setopt( cl, CURLOPT_NOBODY, false );
		res = curl_easy_setopt( cl, CURLOPT_HEADER, false);
	}
	curl_easy_setopt( cl, CURLOPT_WRITEDATA, fh);
	return res;
}		/* -----  end of function downloadHTMLGetHeaders  ----- */

