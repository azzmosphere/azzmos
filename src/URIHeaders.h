/*
 * =====================================================================================
 *
 *       Filename:  URIHeaders.h
 *
 *    Description:  Contains the object and function definitions for the URIHeader_t 
 *                  object.  A URIHeader_t contains the headers that are returned by
 *                  a URL after the DownloadHTMLGetHeaders method is called.
 *
 *        Version:  1.0
 *        Created:  02/06/2010 21:43:06
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri <azzmosphere@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */

/* #####   HEADER FILE INCLUDES   ################################################### */
#ifndef __COMMON_H__
#include <common.h>
#endif

#ifndef _DOWNLOADHTML_H_
#include <DownloadHTML.h>
#endif

#ifndef _URIEXTRACT_H_
#include <URIExtract.h>
#endif

#ifndef _UTILS_H_
#include <Utils.h>
#endif

/* #####   EXPORTED TYPE DEFINITIONS   ############################################## */
struct URIHeader_s {
	char **          uh_key;   /* The key value that should be used */
	char **          uh_val;   /* The value returned by the server */
	struct list_head uh_list;  /* Linked list of headers */
} typedef URIHeader_t;

/* #####   EXPORTED DATA TYPES   #################################################### */
#define AHN_ACCEPT               "Accept"           /* -  This is more a client header */
#define AHN_ACCEPT_CHARSET       "Accept-Charset" 
#define AHN_ACCEPT_ENCODING      "Accept-Encoding"
#define AHN_ACCEPT_LANGUAGE      "Accept-Language"
#define AHN_ACCEPT_RANGES        "Accept-Ranges"    /* a value for this might be bytes */
#define AHN_AGE                  "Age"
#define AHN_ALLOW                "Allow"
#define AHN_AUTHORIZATION        "Authorization"
#define AHN_CACHE_CONTROL        "Cache-Control"
#define AHN_CONNECTION           "Connection"
#define AHN_CONTENT_ENCODING     "Content-Encoding"
#define AHN_CONTENT_LANGUAGE     "Content-Language"
#define AHN_CONTENT_LENGTH       "Content-Length"
#define AHN_CONTENT_LOCATION     "Content-Location:" /* - not this can be a relative or absolute path */
#define AHN_CONTENT_MD5          "Content-MD5"      /* - refer to RFC 1864 */
#define AHN_CONTENT_RANGE        "Content-Range"
#define AHN_CONTENT_TYPE         "Content-Type"
#define AHN_DATE                 "Date"
#define AHN_ETAG                 "ETag"
#define AHN_EXPECT               "Expect"
#define AHN_EXPIRES              "Expires"
#define AHN_FROM                 "From"              /* - as in email address (From: webmaster@w3.org) */
#define AHN_HOST                 "Host"
#define AHN_IF_MATCH             "If-Match"
#define AHN_IF_MODIFIED_SINCE    "If-Modified-Since" /* - example If-Modified-Since: Sat, 29 Oct 1994 19:43:31 GMT */
#define AHN_IF_NONE_MATCH        "If-None-Match"
#define AHN_IF_RANGE             "If-Range"
#define AHN_LAST_MODIFIED        "Last-Modified"
#define AHN_LOCATION             "Location"          /* - used to redirect (must be an absolute path and should be followed) */
#define AHN_MAX_FORWARDS         "Max-Forwards"
#define AHN_PRAGMA               "Pragma"
#define AHN_PROXY_AUTHENTICATE   "Proxy-Authenticate:"
#define AHN_PROXY_AUTHORIZATION  "Proxy-Authorization"
#define AHN_RANGE                "Range"
#define AHN_REFERER              "Referer"
#define AHN_RETRY_AFTER          "Retry-After"
#define AHN_SERVER               "Server"
#define AHN_SET_COOKIE           "Set-Cookie"
#define AHN_TE                   "Te"
#define AHN_TRAILER              "Trailer"
#define AHN_TRANSFER_ENCODING    "Transfer-Encoding"
#define AHN_UPGRADE              "Upgrade"
#define AHN_USER_AGENT           "User-Agent"
#define AHN_VARY                 "Vary"
#define AHN_VIA                  "Via"

/* special response allways called by Azzmos first that states that
 * the foreign host that states what level (version) of HTTP is supported
 */
#define AHN_HTTP_OK             "Http-Supported"

/* #####   EXPORTED FUNCTION DECLARATIONS   ######################################### */
URIHeader_t *URIHeaderInit();
void         URIHeaderCleanUp( URIHeader_t *uh );
int          URIHeaderAllocate( URIHeader_t *uh, DownloadHTML_t *dl, URIRegex_t *re, const char *seed); 
char *       URIHeaderGetValue( URIHeader_t *uh, const char *key);
bool         URIHeaderHasValue( URIHeader_t *uh, const char *key);
int          URIHeaderAllocateFromFile( URIHeader_t *uh, URIRegex_t *ue, FILE *in);


