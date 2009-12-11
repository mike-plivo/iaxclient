PROG=iaxphone

if MACOSX
noinst_PROGRAMS = iaxphone
else
bin_PROGRAMS = iaxphone
endif

iaxphone_SOURCES = iaxphone.cc iaxphone.h

LDADD = $(top_builddir)/lib/libiaxclient.la $(GTK_LIBS) $(WX_LIBS)
AM_CPPFLAGS = -I$(top_srcdir)/lib $(WX_CPPFLAGS)
AM_CXXFLAGS = $(GTK_CFLAGS) $(WX_CXXFLAGS_ONLY) 

if WIN32
AM_CPPFLAGS += -D__WIN95__
AM_CXXFLAGS += -fno-rtti -fno-exceptions -fno-pcc-struct-return -fstrict-aliasing
else
AM_CPPFLAGS += -DPOSIXSLEEP
endif

if MACOSX
TARGET=$(PROG).app
iaxphone_SOURCES += Info.plist

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
	cp `$(WX_CONFIG_PATH) --prefix`/lib/libwx_mac*.rsrc $@/Contents/Resources/$(PROG).rsrc
	echo -n "APPL????" > $@/Contents/PkgInfo
	cp $(srcdir)/Info.plist $@/Contents/Info.plist 
	$(INSTALL_PROGRAM) $(PROG) "`pwd`/$@/Contents/MacOS/$(PROG)"

endif MACOSX
