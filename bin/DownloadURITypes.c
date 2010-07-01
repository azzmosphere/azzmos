/*
 * =====================================================================================
 *
 *       Filename:  DownloadURITypes.c
 *
 *    Description:  Common functions that are used by
 *                  DownloadURI and URIExtract`
 *
 *        Version:  1.0
 *        Created:  29/05/2010 23:20:48
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */

#include <DownloadURITypes.h>


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  DownloadURISetHref
 *  Description:  Set the href value of a given URI,  on sucess return
 *                0 otherwise return the appropriate error message.
 * =====================================================================================
 */
int
DownloadURISetHref ( DownloadURI_t *duri, const char *href )
{
	int rv = 0;

	*duri->du_href = NULL;
	free( *duri->du_href );
	*duri->du_href = strdup( href );

	return rv;
}		/* -----  end of function DownloadURISetHref  ----- */

