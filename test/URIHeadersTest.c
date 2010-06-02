/*
 * =====================================================================================
 *
 *       Filename:  URIHeadersTest.c
 *
 *    Description:  Unit tests for URIHeaders_t object functions
 *
 *        Version:  1.0
 *        Created:  02/06/2010 22:36:18
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri <azzmosphere@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */

#include <CuTest.h>
#include <URIHeaders.h>


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  GetSuite
 *  Description:  Create the test suites.
 * =====================================================================================
 */
CuSuite *
GetSuite ( )
{
	CuSuite *suite = CuSuiteNew();

	return suite;
}		/* -----  end of function GetSuite  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  Run the tests
 * =====================================================================================
 */
int
main ( )
{
	CuSuite  *suite  = CuSuiteNew();
	CuString *output = CuStringNew();
	CuSuiteAddSuite( suite, GetSuite());
	CuSuiteRun(suite);
	CuSuiteSummary( suite, output);
	fprintf( stdout, "%s", output->buffer);
	exit( suite->failCount);
}		/* -----  end of function main  ----- */

