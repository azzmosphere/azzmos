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
#define _URIHEADERS_H_

#ifndef _URIHEADERSTYPES_H_
#include <URIHeadersTypes.h>
#endif

#ifndef _URIEXTRACT_H_
#include <URIExtract.h>
#endif


/* #####   EXPORTED FUNCTION DECLARATIONS   ######################################### */
int          URIHeaderAllocate( URIHeader_t *uh, DownloadHTML_t *dl, URIRegex_t *re, const char *seed); 
int          URIHeaderAllocateFromFile( URIHeader_t *uh, URIRegex_t *ue, FILE *in);


