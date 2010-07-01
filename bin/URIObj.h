/*
 * =====================================================================================
 *
 *       Filename:  URIObj.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  24/05/2010 22:40:20
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */

/* #####   HEADER FILE INCLUDES   ################################################### */

#define _URIOBJ_H_

#ifndef __COMMON_H__
#include <common.h>
#endif

#ifndef _DOWNLOADHTML_H_
#include <DownloadHTML.h>
#endif

#ifndef _URIHEADERSTYPES_H_
#include <URIHeadersTypes.h>
#endif


/* #####   EXPORTED DATA TYPES   #################################################### */

struct URIObj_s {
	char ** uri_content;  /* The HTML content of the URI */
	char ** uri_fqp;      /* The Fully Qualified Address */

	int uri_scheme_so;    /* offset values for scheme */
	int uri_scheme_eo; 

	int uri_auth_so;      /* offset values for auth */
	int uri_auth_eo;

	int uri_path_so;
	int uri_path_eo;

	int uri_content_offset; /* offset value for getting URI content */

	URIHeader_t *uri_uh;    /* linked list of URI headers */

	int uri_id;
} typedef URIObj_t;


/* #####   EXPORTED FUNCTION DECLARATIONS   ######################################### */

URIObj_t *URIObjInit();
void      URIObjCleanUp(URIObj_t * uri);

int  URIObjSetContent( URIObj_t *uri, DownloadHTML_t *dl);
int  URIObjSetFQP( URIObj_t *uri, const char *fqp);
void URIObjFreeElement( char **uriel );
URIObj_t * URIObjClone( URIObj_t *uriin );

char * NormalizeFQP( URIObj_t *uri );



/* #####   EXPORTED MACROS   ######################################################## */

#define URIObjGetScheme( uri )    USplice( *uri->uri_fqp, uri->uri_scheme_so, uri->uri_scheme_eo)
#define URIObjGetAuth( uri )      USplice( *uri->uri_fqp, uri->uri_auth_so, uri->uri_auth_eo)
#define URIObjGetPath( uri )      USplice( *uri->uri_fqp, uri->uri_path_so, uri->uri_path_eo)

#define URIObjGetFQP( uri )     *uri->uri_fqp
#define URIObjGetContent( uri ) *uri->uri_content
#define URIObjGetId( uri )       uri->uri_id

#define URIObjSetId( uri, id )  (uri->uri_id = id )
#define URIObjSetUH( uri, uh )  (uri->uri_uh = uh )

#define URIObjFreeContent( uri ) URIObjFreeElement( uri->uri_content )
#define URIObjFreeFQP( uri )     URIObjFreeElement( uri->uri_fqp )


