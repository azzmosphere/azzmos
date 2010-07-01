SOURCES = CuTest.c CuTest.h \
	  $(top_srcdir)/bin/Utils.c \
	  $(top_srcdir)/bin/URIExtract.c \
	  $(top_srcdir)/bin/URIObj.c \
	  $(top_srcdir)/bin/DownloadHTML.c \
	  $(top_srcdir)/bin/URIQualifier.c \
	  $(top_srcdir)/bin/DownloadURI.c  \
	  $(top_srcdir)/bin/DBSQLHandle.c \
	  $(top_srcdir)/bin/URIHeaders.c  \
	  $(top_srcdir)/bin/DownloadURITypes.c 

AM_CFLAGS = -I$(top_srcdir)/bin @POSTGRESQL_CFLAGS@ -D__DATA__=\"$(top_srcdir)/data\"
AM_LDFLAGS = @POSTGRESQL_LDFLAGS@ @LIBCURL@ 
test_SUtilsTest_SOURCES =  SUtilsTest.c $(SOURCES) 
test_URIExtractTest_SOURCES  = URIExtractTest.c $(SOURCES)
test_DownloadURITest_SOURCES = DownloadURITest.c $(SOURCES)
test_URIHeadersTest_SOURCES =  URIHeadersTest.c $(SOURCES)

check_PROGRAMS = test_SUtilsTest test_URIExtractTest test_DownloadURITest test_URIHeadersTest

TESTS = test_SUtilsTest test_URIExtractTest test_DownloadURITest test_URIHeadersTest
