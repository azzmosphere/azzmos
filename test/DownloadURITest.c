/*
 * =====================================================================================
 *
 *       Filename:  DownloadURITest.c
 *
 *    Description:  Unit tests for DownloadURI
 *
 *        Version:  1.0
 *        Created:  22/05/2010 19:16:19
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Aaron Spiteri <azzmosphere@gmail.com>
 *        Company:  
 *
 * =====================================================================================
 */

#include <CuTest.h>
#include <DownloadURI.h>

URIObj_t      *uri, *luri;
URIRegex_t    *urire; 
DownloadURI_t *dlu;
char *htmlfile;
char *content;

void
TestGetHrefList( CuTest *tc )
{
	/* set content directly for the purposes of a test */
	*uri->uri_content = strdup( content );
	URIObjSetFQP( uri, "http://www.perlmonks.org/");
	dlu = getHrefList( urire, uri );

	CuAssertPtrNotNull( tc, dlu );
	
}

void
TestHref1( CuTest *tc )
{
	bool hit = false;
	DownloadURI_t *tmp;
	struct list_head *pos;

	list_for_each( pos, &dlu->du_list ) {	
		tmp = list_entry(pos,  DownloadURI_t, du_list);
		if( strcmp( DownloadURIGetHref( tmp ), "test_files/common.css") )
			hit = true;
	}
	CuAssertIntEquals( tc, hit, true );
	
}

void
TestHref2( CuTest *tc )
{
	bool hit = false;
	DownloadURI_t *tmp;
	struct list_head *pos;

	list_for_each( pos, &dlu->du_list ) {	
		tmp = list_entry(pos,  DownloadURI_t, du_list);
		if( strcmp( DownloadURIGetHref( tmp ), "http://www.perlmonks.org/?node_id=840289") )
			hit = true;
	}
	CuAssertIntEquals( tc, hit, true );
	
}


CuSuite *GetSuite()
{
        CuSuite *suite = CuSuiteNew();

        SUITE_ADD_TEST( suite, TestGetHrefList );
        SUITE_ADD_TEST( suite, TestHref1 );
        SUITE_ADD_TEST( suite, TestHref2 );

}

int
InitContent( const char *fin )
{
	FILE *f = fopen( fin, "r");
	char buf[BUFSIZ];

	if( f == NULL )
		return errno;

	content = NULL;
	while( ! feof(f)) {
		fgets( buf, BUFSIZ, f);
		if( buf != NULL ) {
			if( content == NULL ) {
				content = (char *) malloc((strlen(buf) + 1));
				strcpy(content, buf);

			} else {
				content = (char *) realloc(content, (strlen(content) + strlen(buf) + 1));
				strcat( content, buf);
			}
		}
		
	}

	fclose(f);
	return 0;
}

int
main(void)
{
	uri   = URIObjInit();
	luri  = URIObjInit();
	urire = URIRegexInit(); 
	dlu   = NULL;

	asprintf(&htmlfile, "%s/%s",  __DATA__, "test.html");
	if( InitContent( htmlfile ) ) {
		fprintf(stderr, "could not read data file %s\n", strerror(errno));
		fprintf(stderr, "htmlfile = '%s'\n", htmlfile);
		exit( 1);
	}

        CuSuite  *suite  = CuSuiteNew();
        CuString *output = CuStringNew();

        CuSuiteAddSuite( suite, GetSuite());
        CuSuiteRun(suite);
        CuSuiteSummary( suite, output);

	fprintf( stdout, "%s", output->buffer);
	free( content );
        exit( suite->failCount);
}



