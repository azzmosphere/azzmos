/*
 * =====================================================================================
 *
 *       Filename:  DBSQLHandleTest.c
 *
 *    Description:  Unit testing for DB SQL handles.
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
#include <DBSQLHandle.h>
#include <CuTest.h>


/* #####   CONSTANTS  -  LOCAL TO THIS SOURCE FILE   ############################### */
#define SQL1  "UPDATE footprint SET status = $1 WHERE msg = $2;"
#define SQL2  "INSERT INTO footprint( status, msg) VALUES ( $1, $2 );"
#define SQL3  "SELECT msg FROM footprint WHERE status = $1;"
#define MSG_1 "testing DBSQLHandle"

#define STATUS_1 0
#define STATUS_2 1

/* #####   VARIABLES  -  LOCAL TO THIS SOURCE FILE   ############################### */
DBObj_t *db;
DBSth_t *sth1, *sth2;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  TestDBSQLHandleInit
 *  Description:  Inilize the Db handle and test that we can get a connection to the 
 *                database.  If the database is not connected then this test will not 
 *                work.
 * =====================================================================================
 */
void
TestDBSQLHandleInit( CuTest *tc )
{
	char *dbpath;
	Opts_t opts;

	asprintf( &dbpath, "%s/%s", __DATA__, "db.conf");

	opts.o_dbconf    = dbpath;
	opts.o_clientenc = DEFAULT_CLIENC;

	db = DBSQLHandleInit( &opts );
	CuAssertPtrNotNull( tc, db );
}		/* -----  end of function TestURIHeaderInit  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  TestIndexInit
 *  Description:  Test that the index is set to be 0 before any processing starts.
 * =====================================================================================
 */
void
TestIndexInit ( CuTest *tc )
{
	CuAssertIntEquals( tc, DBSQLGetIdx( db ), 0);
}		/* -----  end of function TestIndexInit  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  TestDBSQLSthInit
 *  Description:  Create a SQL statement handle.  Do some really basic SQL for this
 *                and check that we have a value.  For this we simply add a value
 *                to the system_log table to state what we are doing.  IE 'UNIT TEST'
 *                then we check that has returned a value and that we can get the 
 *                value.  Pretty basic stuff.
 * =====================================================================================
 */
void
TestDBSQLSthInit1 ( CuTest *tc )
{
	Oid paramTypes[2] = {INT4OID, TEXTOID};

	sth1 = DBSQLSthInit(db,SQL1,paramTypes, "TestDBSQLSthInit1",2 );
	CuAssertPtrNotNull( tc, sth1 );
}		/* -----  end of function TestDBSQLSthInit  ----- */

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
	SUITE_ADD_TEST( suite, TestDBSQLHandleInit );
	SUITE_ADD_TEST( suite, TestIndexInit );
	SUITE_ADD_TEST( suite, TestDBSQLSthInit1 );
	return suite;
}		/* -----  end of function GetSuite  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  Run the tests
 * =====================================================================================
 */
int
main ()
{
	CuSuite  *suite  = CuSuiteNew();
	CuString *output = CuStringNew();
	CuSuiteAddSuite( suite, GetSuite());
	CuSuiteRun(suite);
	CuSuiteSummary( suite, output);
	fprintf( stdout, "%s", output->buffer);

	DBSQLHandleCleanUp( db );
	exit( suite->failCount);
}		/* -----  end of function main  ----- */


