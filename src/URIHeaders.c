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


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  URIHeaderCleanUp
 *  Description:  Deconstruct the URIHeader object
 * =====================================================================================
 */
void
URIHeaderCleanUp ( URIHeader_t *uh )
{
       URIHeader_t *tmp;
       struct list_head *pos, *q;

       list_for_each_safe( pos, q, &uh->uh_list ) {
	       tmp = list_entry(pos, URIHeader_t, uh_list);
	       list_del( &uh->uh_list);
	       
	       if ( tmp->uh_key ) {
		       *(tmp->uh_key) = NULL;
		       free( *(tmp->uh_key));
	       }

	       if( tmp->uh_val ) {
		       *(tmp->uh_val) = NULL;
		       free( *(tmp->uh_val));
	       }
	       free(tmp);
	       tmp = NULL;
       }
}		/* -----  end of function URIHeaderCleanUp  ----- */

