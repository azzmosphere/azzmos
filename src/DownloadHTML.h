#define _DOWNLOADHTML_H_

#ifndef _OPTIONS_H_
#include <Options.h>
#endif

#include <common.h>

struct DownloadHTML_s {
	CURLcode dl_result;   /* result returned by curl function */
	FILE    *dl_fh;       /* file handle for HTML */
	CURL    *dl_ch;       /* CURL handle to use */
	char   **dl_fname;    /* temp file name */
} typedef DownloadHTML_t;

DownloadHTML_t * InitDownloadHTML( const Opts_t *opt );
void CleanUpDownloadHTML( DownloadHTML_t *dl_t );

CURLcode PerformDownloadHTML( DownloadHTML_t *dl_t, const char *url );

FILE *DownloadHTMLGetFh( DownloadHTML_t *dl_t);

#define DownloadHTMLGetFname( dl )     (*dl->dl_fname)
#define DownloadHTMLSetFH( dl, fh )    ( dl->dl_fh = fh)

