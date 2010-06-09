/*
 * =====================================================================================
 *
 *       Filename:  URIExtractTest
 *
 *    Description:  Unit tests for URIExtract (PCRE)
 *
 *        Version:  1.0
 *        Created:  22/05/2010 19:16:19
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */


#include <CuTest.h>
#include <URIExtract.h>


URIObj_t *uri, *luri;
URIRegex_t *urire; 
const char *seed = "http://www.ato.gov.au/something.html";


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  TestURIQualify
 *  Description:  Test that a fully qualified address is seen as a FQP
 * =====================================================================================
 */
void
TestURIQualify( CuTest *tc )
{
        CuAssertStrEquals( tc, seed, URIQualify(urire, seed, luri, NULL  ));
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  TestURIQualify2
 *  Description:  Test that a fully qualified address is seen as a FQP
 * =====================================================================================
 */
void
TestURIQualify2( CuTest *tc )
{
	char *lseed = "http://www.ato.gov.au/";
        CuAssertStrEquals( tc, lseed, URIQualify(urire, lseed, uri, NULL  ));
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  TestURIObjGetScheme
 *  Description:  Test the scheme section of the URI
 * =====================================================================================
 */
void
TestURIObjGetScheme( CuTest *tc )
{
	CuAssertStrEquals( tc, "http", URIObjGetScheme( luri ));
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  TestURIObjGetAuth
 *  Description:  Test the Auth function
 * =====================================================================================
 */
void
TestURIObjGetAuth( CuTest *tc )
{
	CuAssertStrEquals( tc, "www.ato.gov.au", URIObjGetAuth( luri ));
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: TestURIObjGetPath 
 *  Description: Test the Path section
 * =====================================================================================
 */
void
TestURIObjGetPath( CuTest *tc )
{
	CuAssertStrEquals( tc, "/something.html", URIObjGetPath( luri ));
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: TestURIObjGetPath 
 *  Description: Test a Path Only URI
 * =====================================================================================
 */
void
TestPathOnly( CuTest *tc )
{
	char *lseed = strdup( "/a/location/to/a/path.html"),
	     *gseed = strdup("http://www.ato.gov.au/");
	URIQualify( urire, gseed, uri, luri);

	CuAssertStrEquals( tc, 
			"http://www.ato.gov.au/a/location/to/a/path.html",
			URIQualify( urire, lseed, uri, luri)
	);
				
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name: TestURINoPath
 *  Description: Test a URI with no path defined
 * =====================================================================================
 */
void
TestURINoPath( CuTest *tc )
{
	char *lseed = strdup("http://www.ato.gov.au");
	CuAssertStrEquals( tc,
			"http://www.ato.gov.au/",
			URIQualify( urire, lseed, uri, luri)
	);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: TestURINoPath
 *  Description: Test a URI with various characters 
 * =====================================================================================
 */
void
TestURIQualify3( CuTest *tc )
{
	char *lseed = "http://www.perlmonks.org/?node_id=840289";
        CuAssertStrEquals( tc, lseed, URIQualify(urire, lseed, uri, NULL  ));
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  TestPathOnly2
 *  Description:  Test paths that do not start with '/'`
 * =====================================================================================
 */
void
TestPathOnly2( CuTest *tc )
{
	char *lseed = strdup( "ml.asp?45937"),
	     *gseed = strdup("http://www.ato.gov.au/");
	URIQualify( urire, gseed, uri, luri);

	CuAssertStrEquals( tc, 
			"http://www.ato.gov.au/ml.asp?45937",
			URIQualify( urire, lseed, uri, luri)
	);
				
}


 /* REMAINS TO BE IMPLEMENTED */
void
TestURINormalization( CuTest *tc )
{
	URIQualify( urire, "htTP://WWW.ato.GOV.aU/SomePath/", uri, luri );
	CuAssertStrEquals( tc,
			"http://www.ato.gov.au/SomePath/",
			URIObjGetFQP( uri )
	);
			
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name: GetSuite
 *  Description: Create test suites
 * =====================================================================================
 */
CuSuite *GetSuite()
{
        CuSuite *suite = CuSuiteNew();

        SUITE_ADD_TEST( suite, TestURIQualify);
        SUITE_ADD_TEST( suite, TestURIObjGetScheme);
	SUITE_ADD_TEST( suite, TestURIObjGetAuth);
	SUITE_ADD_TEST( suite, TestURIObjGetPath);
	SUITE_ADD_TEST( suite, TestPathOnly );
	SUITE_ADD_TEST( suite, TestURINoPath );
	SUITE_ADD_TEST( suite, TestURIQualify3 );
	SUITE_ADD_TEST( suite, TestPathOnly2 );
	SUITE_ADD_TEST( suite, TestURINormalization );
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: main function
 *  Description: Runs all tests
 * =====================================================================================
 */
int
main(void)
{
	uri   = URIObjInit();
	luri  = URIObjInit();
	urire = URIRegexInit(); 

        CuSuite  *suite  = CuSuiteNew();
        CuString *output = CuStringNew();

        CuSuiteAddSuite( suite, GetSuite());
        CuSuiteRun(suite);
        CuSuiteSummary( suite, output);

	fprintf( stdout, "%s\n", output->buffer);
        exit( suite->failCount);
}

