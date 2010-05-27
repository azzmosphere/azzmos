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
#ifndef __COMMON_H__
#include <common.h>
#endif
#ifndef _URIEXTRACT_H_
#include <URIExtract.h>
#endif
#ifndef _URIOBJ_H_
#include <URIObj.h>
#endif
#ifndef _DOWNLOADURI_H_
#include <DownloadURI.h>
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
int URIQualifyAppend ( URIQualify_t *uqin, const char *seed );


/* #####   EXPORTED MACROS   ######################################################## */

#define URIQualifySetFqp(uq, fqp)   ( *uq->uq_fqp = strdup(fqp))
#define URIQualifySetLock( uq, l )  (  uq->uq_lock = l )
#define URIQualifyGetThisFqp( uq)   *uq->uq_fqp
#define URIQualifyGetLock( uq )      uq->uq_lock 




