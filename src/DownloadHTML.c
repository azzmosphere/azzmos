/**
 * DownloadHTML.c
 *
 * Download a HTML page from the internet and return the file
 * handle, and any other attributes to the calling function.
 *
 * To do this use a CURLHandle object.  This object can be 
 * reused as required.
 */

#include <DownloadHTML.h>

static size_t DownloadHTMLWriteData(void *ptr, size_t size, size_t nmemb, void *stream);

/**
 *  Initilize a file handle and create usable default values.
 *  On error return NULL.
 *
 * This function needs to be modified so that file is opened in
 * text mode fopen( f, "wt+");
 */
DownloadHTML_t * 
InitDownloadHTML( const Opts_t *opts )
{
	DownloadHTML_t *dl_t;
	CURLcode res;


	if( (dl_t = (DownloadHTML_t *) malloc( sizeof(DownloadHTML_t))) == NULL )
	{
		SYSLOG_ERR("InitDownloadHTML()", "could not allocate memory", errno);

	}
	else if( (dl_t->dl_fname = (char **) malloc(sizeof( char *))) == NULL)
	{
		syslog( LOG_ERR, "could not allocate memory for tmp file pointer");
		free(dl_t);
		dl_t = NULL;
	}
	else if( (*dl_t->dl_fname  = strdup(tempnam(opts->o_tdir, NULL))) == NULL )
	{
		syslog( LOG_ERR, "could not get temporary file");
		free(dl_t->dl_fname);
	       	free(dl_t);
       		dl_t = NULL;
	}
	else if( (dl_t->dl_fh = fopen(*dl_t->dl_fname, "w+")) ==  NULL)
	{
		SYSLOG_ERR("InitDownloadHTML()", "could not create temp file", errno);
		free(dl_t);
		dl_t = NULL;
	}	
	else if( (dl_t->dl_ch = curl_easy_init()) == NULL )
	{
		syslog( LOG_ERR, "%s could not initilise CURL handle", __FILE__);
		fclose(dl_t->dl_fh);
		free(dl_t);
		dl_t = NULL;
	}
	else if( (res = curl_easy_setopt( dl_t->dl_ch, CURLOPT_WRITEFUNCTION,  DownloadHTMLWriteData)) != CURLE_OK)
	{
		syslog( LOG_ERR, "%s - setting write function - %s", __FILE__, curl_easy_strerror(res));
		CleanUpDownloadHTML( dl_t );
	}
	else if( (res = curl_easy_setopt( dl_t->dl_ch, CURLOPT_WRITEDATA, dl_t->dl_fh)) != CURLE_OK)
	{
		syslog( LOG_ERR, "%s - setting write pointer - %s", __FILE__, curl_easy_strerror(res));
		CleanUpDownloadHTML( dl_t );
	}
	else
	{
		dl_t->dl_result = CURLE_OK;
	}
	return dl_t;
}

/**
 * PerformDownloadHTML( DownloadHTML_t *dl_t )
 *
 * Download HTML page from the web.
 */
CURLcode 
PerformDownloadHTML( DownloadHTML_t *dl_t, const char *url )
{
	CURLcode res;
	rewind( dl_t->dl_fh);

	if( ((res = curl_easy_setopt(dl_t->dl_ch, CURLOPT_URL, url)) != CURLE_OK) || (res = curl_easy_perform(dl_t->dl_ch)))
	{
		syslog( LOG_ERR, "%s - could not get URL (%s) - %s", __FILE__, url, curl_easy_strerror(res));
	}
	return res;
}

/***
 * Destroy the DownloadHTML object
 */
void
CleanUpDownloadHTML( DownloadHTML_t *dl_t )
{
	if( dl_t != NULL )
	{
		curl_easy_cleanup( dl_t->dl_ch );
		fclose( dl_t->dl_fh);
		unlink( *dl_t->dl_fname);
		free( *dl_t->dl_fname);
		free( dl_t );
		dl_t = NULL;
	}
}

static size_t 
DownloadHTMLWriteData(void *ptr, size_t size, size_t nmemb, void *stream)
{
        int written = fwrite(ptr, size, nmemb, (FILE *)stream);
        return written;
}

FILE *
DownloadHTMLGetFh( DownloadHTML_t *dl)
{
	return dl->dl_fh;
}
