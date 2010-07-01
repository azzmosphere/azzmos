/*
 * =====================================================================================
 *
 *       Filename:  URIQualifier.h
 *
 *    Description:  Qualify the FQP of a URI
 *
 *        Version:  1.0
 *        Created:  19/05/2010 20:28:42
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */


/* #####   HEADER FILE INCLUDES   ################################################### */

#define __URIQUALIFIER_H__

#ifndef __DOWNLOADURITYPES_H__
#include <DownloadURITypes.h>
#endif

#ifndef _URIEXTRACT_H_
#include <URIExtract.h>
#endif

#ifndef _URIOBJ_H_
#include <URIObj.h>
#endif

#ifndef _URIHEADERS_H_
#include <URIHeaders.h>
#endif

#ifndef _DOWNLOADHTML_H_
#include <DownloadHTML.h>
#endif

#ifndef _DBSQLHANDLE_H_
#include <DBSQLHandle.h>
#endif

/* #####   EXPORTED DATA TYPES   #################################################### */

struct URIQualify_s {
	char           **uq_fqp;  /*  The FQP */
	pthread_mutex_t *uq_lock; /*  Locking variable for multithreading */
	struct list_head uq_list; /*  linked list */

} typedef URIQualify_t;
/* ----------  end of struct URIQualify_s  ---------- */


/* #####   EXPORTED FUNCTION DECLARATIONS   ######################################### */

char * URIQualify(  URIRegex_t *urirel, const char *seed,  URIObj_t *uri,  URIObj_t *luri);
char * URIQualifyGetFQP ( URIRegex_t *urire, const char *seed,  URIObj_t *uri );

URIQualify_t * URIQualifyInit();
int            URIQualifyAppend ( URIQualify_t *uqin, const char *seed );
void           CleanUpURIQualify( URIQualify_t *uq );
int            URIQualifyDlURI ( URIQualify_t *uq, URIRegex_t *urire, DownloadURI_t *duri, URIObj_t *uri, DownloadHTML_t *dl, DBObj_t *db );


/* #####   EXPORTED MACROS   ######################################################## */

#define URIQualifySetFqp(uq, fqp)   ( *(uq->uq_fqp) = strdup(fqp))
#define URIQualifySetLock( uq, l )  (  uq->uq_lock = l )
#define URIQualifyGetThisFqp( uq)   *uq->uq_fqp
#define URIQualifyGetLock( uq )      uq->uq_lock 

#define UQ_VALID_CTYPES       {"text/html"}  /* Each content type that azzmos supports 
                                               should be appended to this list */
#define UQ_VALID_CTYPE_COUNT  1              /* The list count goes here */




