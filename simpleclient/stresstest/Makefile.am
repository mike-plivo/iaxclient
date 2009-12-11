bin_PROGRAMS=stresstest
stresstest_SOURCES=stresstest.c file.c file.h

AM_CPPFLAGS=-I$(top_srcdir)/lib $(SDL_CFLAGS) $(OGGZ_CFLAGS) $(THEORA_CFLAGS)
stresstest_LDADD=$(top_builddir)/lib/libiaxclient.la $(OGGZ_LIBS) $(THEORA_LIBS)

if MACOSX
stresstest_LDFLAGS =			\
		-framework AudioToolbox \
		-framework AudioUnit	\
		-framework Carbon	\
		-framework CoreAudio	\
		-framework QuartzCore	\
		-framework QuickTime
endif

