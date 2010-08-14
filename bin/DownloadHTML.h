/*
 * =====================================================================================
 *
 *       Filename:  DownloadHTML.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/06/2010 22:20:33
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */


/* #####   HEADER FILE INCLUDES   ################################################### */

#define _DOWNLOADHTML_H_

#ifndef _OPTIONS_H_
#include <Options.h>
#endif

#ifndef _COMMON_H_
#include <common.h>
#endif

#ifndef _UTILS_H_
#include <Utils.h>
#endif


/* #####   EXPORTED TYPE DEFINITIONS   ############################################## */

struct DownloadHTML_s {
	CURLcode dl_result;   /* result returned by curl function */
	FILE    *dl_fh;       /* file handle for HTML */
	CURL    *dl_ch;       /* CURL handle to use */
	char   **dl_fname;    /* temp file name */
} typedef DownloadHTML_t;


/* #####   EXPORTED FUNCTION DECLARATIONS   ######################################### */

DownloadHTML_t * InitDownloadHTML( const Opts_t *opt );
void cleanUpDownloadHTML( DownloadHTML_t *dl_t );
CURLcode performDownloadHTML( DownloadHTML_t *dl_t, const char *url );
CURLcode DownloadHTMLGetHeaders ( DownloadHTML_t *dl, const char *uri );




/* #####   EXPORTED MACROS   ######################################################## */

#define DownloadHTMLGetFname( dl )     (*(dl->dl_fname))
#define DownloadHTMLSetFH( dl, fh )    ( dl->dl_fh = fh)
#define DownloadHTMLGetFh( dl )        dl->dl_fh 
#define DownloadHTMLGetCH( dl )        dl->dl_ch


