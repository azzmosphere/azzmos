SOURCES = CuTest.c CuTest.h \
	  $(top_srcdir)/src/Utils.c \
	  $(top_srcdir)/src/URIExtract.c \
	  $(top_srcdir)/src/URIObj.c \
	  $(top_srcdir)/src/DownloadHTML.c \
	  $(top_srcdir)/src/URIQualifier.c \
	  $(top_srcdir)/src/DownloadURI.c  \
	  $(top_srcdir)/src/DBSQLHandle.c

AM_CFLAGS = -I$(top_srcdir)/src @POSTGRESQL_CFLAGS@ -D__DATA__=\"$(top_srcdir)/data\"
AM_LDFLAGS = @POSTGRESQL_LDFLAGS@ @LIBCURL@ 
test_SUtilsTest_SOURCES =  SUtilsTest.c $(SOURCES) 
test_URIExtractTest_SOURCES  = URIExtractTest.c $(SOURCES)
test_DownloadURITest_SOURCES = DownloadURITest.c $(SOURCES)

check_PROGRAMS = test_SUtilsTest test_URIExtractTest test_DownloadURITest

TESTS = test_SUtilsTest test_URIExtractTest test_DownloadURITest
