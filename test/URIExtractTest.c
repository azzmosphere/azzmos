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
 *         Name:  testUriQualify
 *  Description:  Test that a fully qualified address is seen as a FQP
 * =====================================================================================
 */
void
testUriQualify( CuTest *tc )
{
        CuAssertStrEquals( tc, seed, uriQualify(urire, seed, luri, NULL  ));
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  testUriQualify2
 *  Description:  Test that a fully qualified address is seen as a FQP
 * =====================================================================================
 */
void
testUriQualify2( CuTest *tc )
{
	char *lseed = "http://www.ato.gov.au/";
        CuAssertStrEquals( tc, lseed, uriQualify(urire, lseed, uri, NULL  ));
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  testUriObjGetScheme
 *  Description:  Test the scheme section of the URI
 * =====================================================================================
 */
void
testUriObjGetScheme( CuTest *tc )
{
	CuAssertStrEquals( tc, "http", URIObjGetScheme( luri ));
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  testUriObjGetAuth
 *  Description:  Test the Auth function
 * =====================================================================================
 */
void
testUriObjGetAuth( CuTest *tc )
{
	CuAssertStrEquals( tc, "www.ato.gov.au", URIObjGetAuth( luri ));
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: testUriObjGetPath 
 *  Description: Test the Path section
 * =====================================================================================
 */
void
testUriObjGetPath( CuTest *tc )
{
	CuAssertStrEquals( tc, "/something.html", URIObjGetPath( luri ));
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: testUriObjGetPath 
 *  Description: Test a Path Only URI
 * =====================================================================================
 */
void
testPathOnly( CuTest *tc )
{
	char *lseed = strdup( "/a/location/to/a/path.html"),
	     *gseed = strdup("http://www.ato.gov.au/");
	uriQualify( urire, gseed, uri, luri);

	CuAssertStrEquals( tc, 
			"http://www.ato.gov.au/a/location/to/a/path.html",
			uriQualify( urire, lseed, uri, luri)
	);
				
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name: testUriNoPath
 *  Description: Test a URI with no path defined
 * =====================================================================================
 */
void
testUriNoPath( CuTest *tc )
{
	char *lseed = strdup("http://www.ato.gov.au");
	CuAssertStrEquals( tc,
			"http://www.ato.gov.au/",
			uriQualify( urire, lseed, uri, luri)
	);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name: testUriNoPath
 *  Description: Test a URI with various characters 
 * =====================================================================================
 */
void
testUriQualify3( CuTest *tc )
{
	char *lseed = "http://www.perlmonks.org/?node_id=840289";
        CuAssertStrEquals( tc, lseed, uriQualify(urire, lseed, uri, NULL  ));
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  testPathOnly2
 *  Description:  Test paths that do not start with '/'`
 * =====================================================================================
 */
void
testPathOnly2( CuTest *tc )
{
	char *lseed = strdup( "ml.asp?45937"),
	     *gseed = strdup("http://www.ato.gov.au/");
	uriQualify( urire, gseed, uri, luri);

	CuAssertStrEquals( tc, 
			"http://www.ato.gov.au/ml.asp?45937",
			uriQualify( urire, lseed, uri, luri)
	);
				
}


 /* REMAINS TO BE IMPLEMENTED */
void
testUriNormalization( CuTest *tc )
{
	uriQualify( urire, "htTP://WWW.ato.GOV.aU/SomePath/", uri, luri );
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

        SUITE_ADD_TEST( suite, testUriQualify);
        SUITE_ADD_TEST( suite, testUriObjGetScheme);
	SUITE_ADD_TEST( suite, testUriObjGetAuth);
	SUITE_ADD_TEST( suite, testUriObjGetPath);
	SUITE_ADD_TEST( suite, testPathOnly );
	SUITE_ADD_TEST( suite, testUriNoPath );
	SUITE_ADD_TEST( suite, testUriQualify3 );
	SUITE_ADD_TEST( suite, testPathOnly2 );
	SUITE_ADD_TEST( suite, testUriNormalization );
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

