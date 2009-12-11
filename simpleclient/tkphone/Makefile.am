# Should support LINUX MACOSX WIN32 SOLARIS FREEBSD
bin_SCRIPTS = tkphone

iaxcli_PROGRAMS=iaxcli
iaxcli_SOURCES = iaxcli.c tones.c tones.h

iaxclidir = ${libdir}/iaxclient/tkphone

iaxcli_SCRIPTS = tkiaxphone
dist_iaxcli_DATA = monitor.ui monitor.ui.tcl phone.ui phone.ui.tcl pref.ui pref.ui.tcl

LDADD = $(top_builddir)/lib/libiaxclient.la $(GTK_LIBS) $(GDK_LIBS)
AM_CPPFLAGS = -I$(top_srcdir)/lib -DUSE_HOTKEY=0
AM_CFLAGS = $(GTK_CFLAGS) $(GDK2_CFLAGS)

if ! WIN32
  AM_CPPFLAGS += -DPOSIXSLEEP
endif

SUBST = sed \
	-e 's,[@]iaxclidir[@],$(iaxclidir),g' \
	-e 's,[@]WISH[@],$(WISH),g'

.in.sh:
	$(SUBST) $< > $@

CLEANFILES =

EXTRA_DIST = License tkiaxphone
