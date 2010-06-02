#AM_LDFLAGS   = @LIBCURL@  @PCRE_LIBS@
AM_LDFLAGS   = @LIBCURL@ @POSTGRESQL_LDFLAGS@
AM_CFLAGS = @POSTGRESQL_CFLAGS@
bin_PROGRAMS = azzmos
azzmos_SOURCES = azzmos.c azzmos.h \
		 DownloadHTML.h DownloadHTML.c \
		 common.h \
		 URIObj.c URIObj.h \
		 DownloadURI.h DownloadURI.c \
		 Utils.h Utils.c \
		 URIExtract.c URIExtract.h \
		 Options.h Options.c \
		 DBSQLHandle.c DBSQLHandle.h \
		 Scheduler.h Scheduler.c \
		 URIQualifier.c URIQualifier.h \
		 list.h \
		 DownloadURITypes.c DownloadURITypes.h \
		 URIHeaders.c  URIHeaders.h
