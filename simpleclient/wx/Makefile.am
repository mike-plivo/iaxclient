# Should support LINUX MACOSX WIN32
bin_PROGRAMS = wxiax
wxiax_SOURCES = wx.cc

AM_CPPFLAGS =					\
	-DDEFAULT_DESTSERV=\"guest@ast1\"	\
	-DDEFAULT_DESTEXT=\"1001\"		\
	-DDEFAULT_NUM_CALLS=1			\
	-I$(top_srcdir)/lib			\
	$(WX_CPPFLAGS)

AM_CXXFLAGS =					\
	$(GTK_CFLAGS)				\
	$(WX_CXXFLAGS_ONLY)			\
	$(GDK2_CFLAGS)

LDADD =						\
	$(top_builddir)/lib/libiaxclient.la	\
	$(GTK_LIBS)				\
	$(WX_LIBS)				\
	$(GDK2_LIBS)

if ! WIN32
AM_CPPFLAGS += -DPOSIXSLEEP
endif

if WIN32
AM_CPPFLAGS += -D__WIN95__
# -D__WXDEBUG__
AM_CXXFLAGS += -fno-rtti -fno-exceptions -fno-pcc-struct-return -fstrict-aliasing -Wall
endif

# if MACOSX
# AM_CXXFLAGS += -DPOSIXSLEEP `wx-config --cflags`
# LDADD += -framework CoreAudio -framework AudioToolbox `wx-config --libs`
# #CXX=`wx-config --cxx`
# REZ=`wx-config --rezflags`
# else
# REZ=echo
# endif


# $(PROG).app: $(OBJS) $(LIBS)
# 	mkdir -p $(PROG).app/Contents/MacOS $(PROG).app/Contents/Resources
# 	cp `wx-config --prefix`/lib/libwx_mac*.rsrc $(PROG).app/Contents/Resources/$(PROG).rsrc
# 	echo -n "APPL????" > $(PROG).app/Contents/PkgInfo
# 	cp Info.plist $(PROG).app/Contents/Info.plist 
# 	$(CXX) $(OBJS) $(LIBS) $(SYSLIBS) -o $(PROG).app/Contents/MacOS/$(PROG)

EXTRA_DIST = Info.plist wxVC.dsp

