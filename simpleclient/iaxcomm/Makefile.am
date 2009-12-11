PROG=iaxcomm
IAXCOMM_VERSION=1.1.0final

iaxcommdatadir = ${datadir}/iaxcomm

if MACOSX
noinst_PROGRAMS = iaxcomm
else
bin_PROGRAMS = iaxcomm
endif

XRCS = rc/accounts.xrc rc/devices.xrc rc/directory.xrc rc/frame.xrc	\
rc/menubar.xrc rc/panel.xrc rc/prefs.xrc

BITMAPS = rc/bitmaps/KP0.bmp rc/bitmaps/KP1.bmp rc/bitmaps/KP2.bmp	\
rc/bitmaps/KP3.bmp rc/bitmaps/KP4.bmp rc/bitmaps/KP5.bmp		\
rc/bitmaps/KP6.bmp rc/bitmaps/KP7.bmp rc/bitmaps/KP8.bmp		\
rc/bitmaps/KP9.bmp rc/bitmaps/KPPOUND.bmp rc/bitmaps/KPSTAR.bmp

nobase_dist_iaxcommdata_DATA = $(XRCS) $(BITMAPS) rc/application.ico	\
rc/logo.xpm iaxcomm.htb

iaxcomm_SOURCES = main.cc prefs.cc calls.cc directory.cc dial.cc	\
frame.cc devices.cc accounts.cc ringer.cc accounts.h app.h calls.h	\
devices.h dial.h directory.h frame.h main.h prefs.h ringer.h		\
rc/logo.xpm $(XRCS)

# BUILT_SOURCES = xrc_res.cc
# nodist_iaxcomm_SOURCES = $(BUILT_SOURCES)

CLEANFILES = $(BUILT_SOURCES)

AM_CPPFLAGS = -I$(top_srcdir)/lib $(WX_CPPFLAGS) -DIAXCOMM_VERSION=\"$(IAXCOMM_VERSION)\" -DXRCDIRECTORY=\"$(iaxcommdatadir)/rc\" -DDATADIR=\"$(iaxcommdatadir)\"
AM_CXXFLAGS = $(GDK2_CFLAGS) $(GTK_CFLAGS) $(WX_CXXFLAGS_ONLY)

LDADD = $(top_builddir)/lib/libiaxclient.la $(GTK_LIBS) $(WX_LIBS)	\
$(WX_XRC_LIBS)

if WIN32
AM_CPPFLAGS += -D__WIN95__
AM_CXXFLAGS += -fno-rtti -fno-exceptions -fno-pcc-struct-return -fstrict-aliasing
iaxcomm_SOURCES += iaxcomm.rc rc/application.ico iaxcomm.exe.manifest
AM_RCFLAGS = -D__WIN32__ -D__WIN95__ -D__GNUWIN32__
endif

RCCOMPILE = $(RC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
	$(CPPFLAGS) $(AM_RCFLAGS) $(RCFLAGS)

.rc.o:
	$(RCCOMPILE) -o $@ -i $<

xrc_res.cc: $(XRCS)
	$(WXRC) -c -o $@ $^

if MACOSX
TARGET=$(PROG).app
iaxcomm_SOURCES += Info.plist

all-local: $(TARGET)

installdirs-local:
	test -z "$(DESTDIR)$(bindir)" || $(mkdir_p) "$(DESTDIR)$(bindir)"

install-exec-local:
	test -z "$(DESTDIR)$(bindir)" || $(mkdir_p) "$(DESTDIR)$(bindir)"
	cp -ar $(PROG).app "$(DESTDIR)$(bindir)/"
	$(LIBTOOL) --mode=clean $(RM) "$(DESTDIR)$(bindir)/$(PROG).app/Contents/MacOS/$(PROG)"
	$(LIBTOOL) --mode=install $(INSTALL_PROGRAM) $(PROG) "$(DESTDIR)$(bindir)/$(PROG).app/Contents/MacOS/$(PROG)"

uninstall-local:
	$(RM) -r "$(DESTDIR)$(bindir)/$(PROG).app"

clean-local:
	$(RM) -r $(PROG).app

$(PROG).app: $(PROG)
	-$(mkdir_p) $@/Contents/MacOS
	-$(mkdir_p) $@/Contents/Resources
	cp `$(WX_CONFIG_PATH) --prefix`/lib/libwx_mac-2.4*.rsrc $@/Contents/Resources/$(PROG).rsrc
	echo -n "APPL????" > $@/Contents/PkgInfo
	sed "s/@VERSION@/$(VERSION)/g" < $(srcdir)/Info.plist > $@/Contents/Info.plist
	cp iaxcomm.icns  $@/Contents/Resources
	cp -r $(srcdir)/rc $@/Contents/Resources
	$(INSTALL_PROGRAM) $(PROG) "`pwd`/$@/Contents/MacOS/$(PROG)"

endif MACOSX

EXTRA_DIST = QUICKSTART ring.raw iaxcomm.icns iaxcomm.sln iaxcomm.vcproj
