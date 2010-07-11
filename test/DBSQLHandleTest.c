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

#define STATUS_1 "0"
#define STATUS_2 "1"

/* #####   VARIABLES  -  LOCAL TO THIS SOURCE FILE   ############################### */
DBObj_t *db;
DBSth_t *sth1, *sth2, *sth3;

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
 *         Name:  TestIndexIdx
 *  Description:  Test that the index is set to be 1 before any processing starts.
 * =====================================================================================
 */
void
TestIndexIdx( CuTest *tc )
{
	CuAssertIntEquals( tc, DBSQLGetIdx( db ), 1);
}	

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
 *         Name:  TestDBSQLSthInit2
 *  Description:  Create a SQL statement handle.  Do some really basic SQL for this
 *                and check that we have a value.  For this we simply add a value
 *                to the system_log table to state what we are doing.  IE 'UNIT TEST'
 *                then we check that has returned a value and that we can get the 
 *                value.  Pretty basic stuff.
 * =====================================================================================
 */
void
TestDBSQLSthInit2 ( CuTest *tc )
{
	Oid paramTypes[2]  = {INT4OID, TEXTOID};
	Oid paramTypes2[1] = {TEXTOID};

	sth2 = DBSQLSthInit(db,SQL2,paramTypes, "TestDBSQLSthInit2",2 );
	CuAssertPtrNotNull( tc, sth2 );
	sth3 = DBSQLSthInit(db,SQL3,paramTypes, "TestDBSQLSthInit2",1 );
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  TestSQLSthExec1
 *  Description:  Exexute the update statement.  
 * =====================================================================================
 */
void
TestSQLSthExec1 ( CuTest *tc )
{
	char **paramValues;
	paramValues = (char **) malloc( sizeof( char *));
	paramValues[0] = strdup( STATUS_1 );
	paramValues[1] = strdup( MSG_1 );
	CuAssertIntEquals( tc, 0, DBSQLExecSthCmd( sth1, paramValues ));
}		/* -----  end of function TestSQLSthExec1  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  TestSQLSthExec2
 *  Description:  Exexute the update statement.  
 * =====================================================================================
 */
void
TestSQLSthExec2 ( CuTest *tc )
{
	char **paramValues;
	paramValues = (char **) malloc( sizeof( char *));
	paramValues[0] = strdup( STATUS_2 );
	paramValues[1] = strdup( MSG_1 );
	CuAssertIntEquals( tc, 0, DBSQLExecSthCmd( sth2, paramValues ));
}	

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  TestSQLSthExec3
 *  Description:  Exexute the update statement.  
 * =====================================================================================
 */
void
TestSQLSthExec3 ( CuTest *tc )
{
	char **paramValues;
	paramValues = (char **) malloc( sizeof( char *));
	paramValues[0] = strdup( STATUS_2 );
	CuAssertIntEquals( tc, 0, DBSQLExecSthQuery( sth3, paramValues ));
}	

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  TestSQLSthChkResults
 *  Description:  Exexute the update statement.  
 * =====================================================================================
 */
void
TestSQLSthChkResults ( CuTest *tc )
{
	CuAssertStrEquals( tc, MSG_1, DBSQLGetResult( sth3, 0, 0 ));
}	


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
	SUITE_ADD_TEST( suite, TestIndexIdx );
	SUITE_ADD_TEST( suite, TestDBSQLSthInit2 );
	SUITE_ADD_TEST( suite, TestSQLSthExec1 );
	SUITE_ADD_TEST( suite, TestSQLSthExec2 );
	SUITE_ADD_TEST( suite, TestSQLSthExec3 );
	SUITE_ADD_TEST( suite, TestSQLSthChkResults);
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

	DBSQLSthFinit( sth1 );
	DBSQLSthFinit( sth2 );
	DBSQLSthFinit( sth3 );
	DBSQLHandleCleanUp( db );
	exit( suite->failCount);
}		/* -----  end of function main  ----- */


