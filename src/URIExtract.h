#define _URIEXTRACT_H_
#include <common.h>
#ifndef _UTILS_H_
#include <Utils.h>
#endif
#ifndef _URIOBJ_H_
#include <URIObj.h>
#endif

#define O_REHREF  0x01 /* Check for HREF  */
#define O_REFQP   0x02 /* check to see if seed is FQP */
#define O_REISP   0x04 /* Is path only */
#define O_RENPTH  0x08 /* Address does not have path defined */
#define O_RENOP   0x10 /* No path defined */
#define O_OVECCOUNT 30;

enum {
	REGEX_T_HREF,
	REGEX_T_FQP,
	REGEX_T_ISP,
	REGEX_T_NOP,
	REGEX_T_LAST
} typedef regex_types;

struct URIRegex_s {
	unsigned long  ur_opts;      /* Bitwise options for if regex's have 
	                                been compiled*/
	pcre         **ur_regex;    /* Detect if string has a href */
	int            ur_rv;        /* last return code */
	char         **ur_errptr;    /* error pointer */
	int            ur_erroffset; 
} typedef URIRegex_t;

URIRegex_t * URIRegexInit();
void URIRegexCleanUp( URIRegex_t * reuri );
char * URIRegexecNextHREF( URIRegex_t *reuri, const char *subject, int *offset, URIObj_t *uri );
bool IsNoPath(URIRegex_t *reuri, const char *subject, URIObj_t *uri);
bool IsPathOnly( URIRegex_t *reuri, const char *subject, URIObj_t *uri);
bool IsFQP( URIRegex_t *reuri, const char *subject, URIObj_t *uri);
