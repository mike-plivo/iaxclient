bin_PROGRAMS=testcall
testcall_SOURCES=testcall.c

AM_CPPFLAGS=-I$(top_srcdir)/lib
LDADD=$(top_builddir)/lib/libiaxclient.la

if ! WIN32
AM_CPPFLAGS += -DPOSIXSLEEP
endif

EXTRA_DIST = testcall-jb.c

