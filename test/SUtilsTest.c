#include <Utils.h>
#include <CuTest.h>
//                                     1         2
//                           012345678901234567890123
static const char *splstr = "this is a string to test";

/* Get full string. */
void 
USplice1( CuTest *tc )
{
	CuAssertStrEquals( tc, splstr, USplice(splstr, 0, NULL));
}


/*
 * Get last 4 charcters  (offset is 20)
 */
void
USplice2( CuTest *tc )
{
	CuAssertStrEquals( tc, "test", USplice(splstr, 20, NULL));
}

/**
 * Get first 10 charcters
 */
USplice3( CuTest *tc )
{
	CuAssertStrEquals( tc, "this is a ", USplice(splstr, 0, 9));
}


/**
 * Six charcters from within string
 */
USplice4( CuTest *tc )
{
	CuAssertStrEquals( tc, "string", USplice(splstr, 10, 15));
}

CuSuite *GetSuite()
{
	CuSuite *suite = CuSuiteNew();
	SUITE_ADD_TEST( suite, USplice1);
	SUITE_ADD_TEST( suite, USplice2);
	SUITE_ADD_TEST( suite, USplice3);
	SUITE_ADD_TEST( suite, USplice4);
}

int
main(void)
{
	CuSuite  *suite  = CuSuiteNew();
	CuString *output = CuStringNew();

	CuSuiteAddSuite( suite, GetSuite());
	CuSuiteRun(suite);
	CuSuiteSummary( suite, output);
	exit( suite->failCount);
}
