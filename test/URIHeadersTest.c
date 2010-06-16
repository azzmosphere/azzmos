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

URIHeader_t *uh, *uh2;
URIRegex_t  *re;


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  TestURIHeaderInit
 *  Description:  Test the contruction of the URI header Object. 
 * =====================================================================================
 */
void
TestURIHeaderInit ( CuTest *tc )
{
	uh = URIHeaderInit();
	CuAssertPtrNotNull( tc, uh );
}		/* -----  end of function TestURIHeaderInit  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  TestURIHeaderAllocateFromFile
 *  Description:  Test the allocation of headers once they have been written to a file
 * =====================================================================================
 */
void
TestURIHeaderAllocateFromFile ( CuTest *tc )
{

	FILE	*in;	       /* input-file pointer */
	char	*in_file_name; /* input-file name    */

	asprintf( &in_file_name, "%s/headers.txt", __DATA__);

	in	= fopen( in_file_name, "r" );
	if ( in == NULL ) {
		fprintf ( stderr, "couldn't open file '%s'; %s\n",
				in_file_name, strerror(errno) );
		exit (EXIT_FAILURE);
	}

	CuAssertIntEquals( tc, 0, URIHeaderAllocateFromFile( uh,re, in ));

	if( fclose(in) == EOF ) {			/* close input file   */
		fprintf ( stderr, "couldn't close file '%s'; %s\n",
				in_file_name, strerror(errno) );
	}


}		/* -----  end of function TestURIHeaderAllocateFromFile  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  TestURIHeaderHasValue1
 *  Description:  Test that the correct value is returned.
 * =====================================================================================
 */
void
TestURIHeaderHasValue1 ( CuTest *tc )
{
	CuAssertIntEquals( tc, true, URIHeaderHasValue( uh, AHN_CONTENT_TYPE));
}		/* -----  end of function TestURIHeaderHasValue  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  TestURIHeaderHasValue2
 *  Description:  Test that the correct value is returned.
 * =====================================================================================
 */
void
TestURIHeaderHasValue2 ( CuTest *tc )
{
	CuAssertIntEquals( tc, false, URIHeaderHasValue( uh, AHN_REFERER));
}		/* -----  end of function TestURIHeaderHasValue  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  TestURIHeaderGetValue
 *  Description:  Test the correct value is returned
 * =====================================================================================
 */
void
TestURIHeaderGetValue ( CuTest *tc )
{
	CuAssertStrEquals( tc, "text/html", URIHeaderGetValue( uh, AHN_CONTENT_TYPE));
}		/* -----  end of function TestURIHeaderGetValue  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  TestURIHeadersAllocate
 *  Description:  Test allocations of headers from a foreign web site
 * =====================================================================================
 */
void
TestURIHeadersAllocate ( CuTest *tc )
{
	Opts_t          opts;
	DownloadHTML_t *dl    = InitDownloadHTML(&opts);
	URIRegex_t     *re   = URIRegexInit();
	const char     *seed = "http://www.ato.gov.au/";
	char           *fname;
	CURL           *ch;

	uh2   = URIHeaderInit();
	asprintf(&fname, "%s/headers.tmp.txt", __DATA__);	

	FILE *f = fopen( fname, "w+");
	curl_global_init(CURL_GLOBAL_NOTHING);
	ch = curl_easy_init();
	DownloadHTMLSetFH(dl, f);

	CuAssertIntEquals(tc, CURLE_OK, URIHeaderAllocate( uh2, dl, re, seed));
	
	curl_easy_cleanup( ch );
	curl_global_cleanup();

	fclose(f);
	unlink(f);
}		/* -----  end of function TestURIHeadersAllocate  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  TestURIHeaderHasValue3
 *  Description:  Test that the correct value is returned.
 * =====================================================================================
 */
void
TestURIHeaderHasValue3 ( CuTest *tc )
{
	CuAssertIntEquals( tc, true, URIHeaderHasValue( uh, AHN_CONTENT_TYPE));
}	


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  TestURIHeaderCleanUp
 *  Description:  Test that object can be decontructed
 * =====================================================================================
 */
void
TestURIHeaderCleanUp ( CuTest *tc )
{
	URIHeaderCleanUp( uh );
	//URIHeaderCleanUp( uh2 );
}		/* -----  end of function TestURIHeaderCleanUp  ----- */

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
	SUITE_ADD_TEST( suite, TestURIHeaderInit );
	SUITE_ADD_TEST( suite, TestURIHeaderAllocateFromFile  );
	SUITE_ADD_TEST( suite, TestURIHeaderHasValue1  );
	SUITE_ADD_TEST( suite, TestURIHeaderHasValue2  );
	SUITE_ADD_TEST( suite, TestURIHeaderGetValue  );
	SUITE_ADD_TEST( suite, TestURIHeaderCleanUp  );
	SUITE_ADD_TEST( suite, TestURIHeadersAllocate );
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
	re = URIRegexInit();
	CuSuite  *suite  = CuSuiteNew();
	CuString *output = CuStringNew();
	CuSuiteAddSuite( suite, GetSuite());
	CuSuiteRun(suite);
	CuSuiteSummary( suite, output);
	URIRegexCleanUp( re );
	fprintf( stdout, "%s", output->buffer);
	exit( suite->failCount);
}		/* -----  end of function main  ----- */

