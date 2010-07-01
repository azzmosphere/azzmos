/*
 * =====================================================================================
 *
 *       Filename:  Utils.h
 *
 *    Description:  Functions that are commonly used and fit into a miscellaneois
 *                  category are placed in here.
 *
 *        Version:  1.0
 *        Created:  05/06/2010 22:58:34
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri
 *        Company:  
 *
 * =====================================================================================
 */

/* #####   HEADER FILE INCLUDES   ################################################### */
#define _UTILS_H_

#ifndef __COMMON_H__
#include <common.h>
#endif


/* #####   EXPORTED FUNCTION DECLARATIONS   ######################################### */

char * USplice( const char *in, unsigned int start, unsigned int end);
char * _MacItoA_( int num );

