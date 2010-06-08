/*
 * =====================================================================================
 *
 *       Filename:  DownloadURITypes.h
 *
 *    Description:  Generic types and macros, used by URIQualfier and DownloadURI
 *
 *        Version:  1.0
 *        Created:  28/05/2010 23:26:50
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */



/* #####   HEADER FILE INCLUDES   ################################################### */
#define __DOWNLOADURITYPES_H__

#ifndef __COMMON__
#include <common.h>
#endif

#ifndef _URIHEADERS_H_
#include <URIHeaders.h>
#endif


/* #####   EXPORTED TYPE DEFINITIONS   ############################################## */

struct DownloadURI_s {
	char              **du_href;   /* string reference to the FQP */
	URIHeader_t        *du_uh;     /* The URIHeaders for the HREF */
	struct list_head    du_list;   /* linked list object */
} typedef DownloadURI_t;


/* #####   EXPORTED FUNCTION DECLARATIONS   ######################################### */

int DownloadURISetHref ( DownloadURI_t *duri, const char *href );

/* #####   EXPORTED MACROS   ######################################################## */

#define DownloadURIGetHref( duri )  strdup(*duri->du_href)



