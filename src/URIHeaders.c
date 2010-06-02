/*
 * =====================================================================================
 *
 *       Filename:  URIHeaders.c
 *
 *    Description:  Functions to allocate, read and manipulate the URIHEader_t object
 *
 *        Version:  1.0
 *        Created:  02/06/2010 22:15:28
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri <azzmosphere@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */

#include <URIHeaders.h>


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  URIHeaderInit
 *  Description:  Initilize the and construct the URIHeader_t object.  The function
 *                will return a newly malloced URIHeader with the list inilized or
 *                will return NULL 
 * =====================================================================================
 */
URIHeader_t *
URIHeaderInit ( )
{
	URIHeader_t *uh;

	uh = (URIHeader_t *) malloc( sizeof(URIHeader_t));

	if( uh )
		INIT_LIST_HEAD( &uh->uh_list );
	else 
		syslog( LOG_ERR, "could not allocate URIheaders %s", strerror( errno ));

	return uh;
}		/* -----  end of function URIHeaderInit  ----- */

