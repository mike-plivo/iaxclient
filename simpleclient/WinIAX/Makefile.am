bin_PROGRAMS = WinIAX

WinIAX_SOURCES = StdAfx.cpp StdAfx.h WinIAX.cpp winiax.h resource.rc	\
	default1.bin resource.h icon1.ico

AM_CPPFLAGS = -I$(top_srcdir)/lib -D_WIN32_IE=0x0300
LDADD = $(top_builddir)/lib/libiaxclient.la -lcomdlg32 -lgdi32 -lcomctl32 -lmingwthrd

RCCOMPILE = $(RC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
	$(AM_CPPFLAGS) $(CPPFLAGS)

.rc.o:
	$(RCCOMPILE) -o $@ -i $<

EXTRA_DIST = ReadMe.txt WinIAX.dsp WinIAX.dsw
