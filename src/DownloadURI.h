#define _DOWNLOADURI_H_
#include <common.h>
#ifndef _DOWNLOADHTML_H_
#include <DownloadHTML.h>
#endif
#ifndef _URIOBJ_H_
#include <URIObj.h>
#endif
#ifndef _URIEXTRACT_H_
#include <URIExtract.h>
#endif
#ifndef  _DBSQLHANDLE_H_
#include <DBSQLHandle.h>
#endif
#ifndef _OPTIONS_H_
#include <Options.h>
#endif

int DownloadURI(DownloadHTML_t *dl, const char *seed, URIRegex_t *urire, int maxrecurse, URIObj_t *luri, DBObj_t *db, Opts_t *opts);

struct DownloadURI_s {
	char              **du_href;
	struct list_head    du_list;
} typedef DownloadURI_t;

int DownloadURISetListItem(  DownloadURI_t *tmp, DownloadURI_t *duri,  char *href);
DownloadURI_t * GetHrefList( URIRegex_t *reuri,  URIObj_t *uri );
DownloadURI_t * DownloadURIPerform( DownloadHTML_t *dl, const char *seed, URIRegex_t *urirel,  URIObj_t *uri, int recurse, DBObj_t *db);
void DownloadURICleanUp ( DownloadURI_t *du );



#define DownloadURIGetHref( duri )  *duri->du_href
