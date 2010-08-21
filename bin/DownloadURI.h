/*
 * =====================================================================================
 *
 *       Filename:  DownloadURI.h
 *
 *    Description:  DownlaodURI main header file.
 *
 *        Version:  1.0
 *        Created:  28/05/2010 23:02:58
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */

/* #####   HEADER FILE INCLUDES   ################################################### */

#define _DOWNLOADURI_H_

#ifndef __DOWNLOADURITYPES_H__
#include <DownloadURITypes.h>
#endif

#ifndef _DOWNLOADHTML_H_
#include <DownloadHTML.h>
#endif

#ifndef _URIOBJ_H_
#include <URIObj.h>
#endif

#ifndef _URIEXTRACT_H_
#include <URIExtract.h>
#endif

#ifndef  __DBSQLHANDLESTH_H__
#include <DBSQLHandleSTH.h>
#endif

#ifndef _OPTIONS_H_
#include <Options.h>
#endif

#ifndef __URIQUALIFIER_H__
#include <URIQualifier.h>
#endif


/* #####   EXPORTED FUNCTION DECLARATIONS   ######################################### */

extern int      dl_uri(DownloadHTML_t *dl, URIQualify_t *uq, const char *seed, URIRegex_t *urire, int maxrecurse, URIObj_t *luri, const Opts_t *opts, DBSQLHandleSth_t *dbsth);
int             DownloadURISetListItem(  DownloadURI_t *tmp, DownloadURI_t *duri,  char *href);
extern DownloadURI_t * getHrefList( URIRegex_t *reuri,  URIObj_t *uri );
DownloadURI_t * downloadURIPerform( DownloadHTML_t *dl, const char *seed, URIRegex_t *urirel,  URIObj_t *uri, int recurse, DBSQLHandleSth_t *dbsth);
void            DownloadURICleanUp ( DownloadURI_t *du );



