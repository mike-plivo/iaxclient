dnl Copyright (C) 2004-2005  Mikael Magnusson <mikma@users.sourceforge.net>

AC_PREREQ(2.59)

dnl Package version
m4_define(IAXC_VERSION, [2.1beta3])

AC_INIT(iaxclient, IAXC_VERSION, [iaxclient-devel@lists.sourceforge.net])

AC_CONFIG_SRCDIR([lib/iaxclient.h])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign -Wall])
AM_MAINTAINER_MODE

dnl Libtool library version
IAXC_LT_CURRENT=1
IAXC_LT_REVISION=2
IAXC_LT_AGE=0

AC_SUBST(IAXC_LT_CURRENT)
AC_SUBST(IAXC_LT_REVISION)
AC_SUBST(IAXC_LT_AGE)

AC_CANONICAL_HOST

AC_GNU_SOURCE

AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_GCC_TRADITIONAL
AC_PROG_CXX
AC_PROG_INSTALL
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL

AC_HEADER_STDC
AC_HEADER_STDBOOL
AC_HEADER_TIME
AC_CHECK_HEADERS([sys/time.h varargs.h sys/socket.h])

AC_C_CONST
AC_C_INLINE
AC_C_VOLATILE
AC_TYPE_PID_T
AC_TYPE_SIZE_T

AC_CHECK_FUNCS([vsnprintf _vsnprintf])
AC_CHECK_FUNCS([srandomdev srandom srand48 random lrand48])
AC_CHECK_FUNCS(gettimeofday)

ACX_PTHREAD

AC_CHECK_PROG(have_pkg_config, pkg-config, yes, no)

if test x"$have_pkg_config" = "xno"; then
   AC_MSG_ERROR(pkg-config is required to install this program)
fi

PKG_PROG_PKG_CONFIG

AC_PATH_PROG([WISH],[wish])


dnl Default CFLAGS and CXXFLAGS if unset
CFLAGS=${CFLAGS-"-O2 -g -Wall --std=gnu99"}
CXXFLAGS=${CXXFLAGS-"-O2 -g -Wall"}

AC_ARG_ENABLE(local-gsm,
      [AS_HELP_STRING([--enable-local-gsm],
         [Use local gsm library [default=auto]])],,
      [enable_local_gsm="auto"])

AC_ARG_ENABLE(local-iax,
      [AS_HELP_STRING([--disable-local-iax],
         [Don't use local iax library])],,
      [enable_local_iax="yes"])

AC_ARG_ENABLE(debug-iax,
      [AS_HELP_STRING([--enable-debug-iax],
         [Enable debug support in local iax library])],
      enable_debug_iax2=$enableval,
      enable_debug_iax2="no")

AC_ARG_ENABLE(video,
      [AS_HELP_STRING([--enable-video],
         [Enable video support [default=yes]])],,
      [enable_video="yes"])

AC_ARG_WITH(echo-can,
      [AS_HELP_STRING([--with-echo-can],
         [use echo can (span or speex) [default=speex]])],
      use_echo_can=$withval,
      use_echo_can="speex")

AC_ARG_ENABLE(clients,
      [AS_HELP_STRING([--enable-clients],
         [Select clients (all iaxcomm iaxphone stresstest testcall tkphone vtestcall WinIAX wx) [default=auto]])],,
      enable_clients="auto")

AC_ARG_WITH(ilbc,
      [AS_HELP_STRING([--with-ilbc],
         [Enable iLBC support (You need to place the iLBC reference sources in lib/iLBC) [default=auto]])],,
      with_ilbc="auto")

AC_ARG_WITH(wish,
      [AS_HELP_STRING([--with-wish],
         [Path to Tcl command language interpreter])],
      WISH="${with_wish}",
      with_wish="auto")

AC_ARG_WITH(ogg,
      [AS_HELP_STRING([--without-ogg],
         [OGG support])],,
      with_ogg="auto")

AC_ARG_WITH(theora,
      [AS_HELP_STRING([--without-theora],
         [Theora support])],,
      with_theora="auto")

AC_ARG_WITH(vidcap,
      [AS_HELP_STRING([--without-vidcap],
         [libvidcap support])],,
      with_vidcap="auto")

AC_ARG_WITH(ffmpeg,
      [AS_HELP_STRING([--with-ffmpeg],
         [FFmpeg support])],
      FFMPEG="${with_ffmpeg}",
      with_ffmpeg="no")

case "$with_ilbc" in
   yes)
      enable_local_ilbc=yes ;;
   no)
      enable_local_ilbc=no ;;
   auto)
      AC_MSG_CHECKING([for iLBC reference sources in lib/iLBC])
      if test -r lib/iLBC/iLBC_encode.c; then
         enable_local_ilbc=yes
      else
         enable_local_ilbc=no
      fi
      AC_MSG_RESULT([$enable_local_ilbc]) ;;

   *)
      AC_MSG_ERROR([bad value ${with_ilbc} for --with-ilbc]) ;;
esac

case $host_os in
   *mingw32*|*cygwin* )
      OSTYPE=WIN32
      AC_DEFINE(WIN32,,[mingw or cygwin]);;

   *linux* )
      OSTYPE=LINUX
      AC_DEFINE(LINUX,,[linux]);;

   *darwin* )
      OSTYPE=MACOSX
      AC_DEFINE(MACOSX,,[darwin]);;

   * )
      AC_MSG_WARN([OSTYPE cannot be detected, assuming Linux ($host_os)])
      OSTYPE=LINUX
      AC_DEFINE(LINUX,,[linux]);;
esac

AC_SUBST(OSTYPE)

PKG_CHECK_MODULES(PORTAUDIO, [portaudio-2.0 >= 19],,AC_MSG_ERROR([
   portaudio is required to build this package!
   please see http://www.portaudio.com/ for how to
   obtain a copy.
]))

PKG_CHECK_MODULES(SPEEX, [speex >= 1.2],,AC_MSG_ERROR([
   speex is required to build this package!
   please see http://www.xiph.org/ for how to
   obtain a copy.
]))

PKG_CHECK_MODULES(SPEEXDSP, [speexdsp >= 1.2],,AC_MSG_ERROR([
   speexdsp is required to build this package!
   please see http://www.xiph.org/ for how to
   obtain a copy.
]))

PKG_CHECK_MODULES(SDL, [sdl >= 1.2], has_sdl=yes, has_sdl=no)
PKG_CHECK_MODULES(GTK, [gtk+-2.0 >= 2.0], has_gtk2=yes, has_gtk2=no)
PKG_CHECK_MODULES(GDK2, [gdk-2.0 >= 2.0.0], has_gdk2=yes, has_gdk2=no)
PKG_CHECK_MODULES(ALSA, [alsa >= 1.0], has_alsa=yes, has_alsa=no)
PKG_CHECK_MODULES(OGG, [ogg >= 1.1.3], has_ogg=yes, has_ogg=no)
PKG_CHECK_MODULES(OGGZ, [oggz >= 0.9.5], has_oggz=yes, has_oggz=no)
PKG_CHECK_MODULES(THEORA, [theora >= 1.0alpha7], has_theora=yes, has_theora=no)
PKG_CHECK_MODULES(VIDCAP, [vidcap >= 0.1], has_vidcap=yes, has_vidcap=no)
PKG_CHECK_MODULES(FFMPEG, [libavcodec >= 51.40.3], has_ffmpeg=yes, has_ffmpeg=no)

has_gsm=no
if test x$enable_local_gsm = xyes; then
   has_gsm=yes
   GSM_CFLAGS='-I$(top_srcdir)/lib/gsm/inc'
else
   AM_PATH_GSM(has_gsm=yes, has_gsm=no)
fi

if test x$enable_video = xyes; then

   if test x$with_ogg != xno; then
      if test x$has_ogg = xyes; then
         AC_DEFINE(USE_OGG, 1, [OGG])
         PKG_REQUIRES="$PKG_REQUIRES ogg"
      elif test x$with_ogg != xauto ; then
         AC_MSG_ERROR([
            libogg is required to build this package!
            please see http://www.xiph.org/ for how to
            obtain a copy.
         ])
      fi
   fi
     
   if test x$with_theora != xno; then
      if test x$has_theora = xyes; then
         AC_DEFINE(USE_THEORA, 1, [THEORA])
         PKG_REQUIRES="$PKG_REQUIRES theora"
      elif test x$with_theora != xauto ; then
         AC_MSG_ERROR([
            libtheora is required to build this package!
            please see http://www.xiph.org/ for how to
            obtain a copy.
         ])
      fi
   fi
   
   if test x$has_vidcap = xyes; then
      AC_DEFINE(USE_VIDCAP, 1, [VIDCAP])
      PKG_REQUIRES="$PKG_REQUIRES vidcap"
   else
      AC_MSG_ERROR([
         libvidcap is required to build this package!
         please see http://libvidcap.sourceforge.net/ for how to
         obtain a copy.
      ])
   fi
   
   if test x$with_ffmpeg != xno; then
      if test x$has_ffmpeg = xyes; then
         AC_DEFINE(USE_FFMPEG, 1, [FFMPEG])
         PKG_REQUIRES="$PKG_REQUIRES ffmpeg"
      elif test x$with_ffmpeg != xauto ; then
         AC_MSG_ERROR([
            FFmpeg is required to build this package!
            please see http://ffmpeg.mplayerhq.hu/ for how to
            obtain a copy.
         ])
      fi
   fi
fi

has_iax2=no
if test x$enable_local_iax != xyes; then
   AM_PATH_IAX2(0.2.3,has_iax2=yes,has_iax2=no)
fi

if test x$has_iax2 = xno && test x$enable_local_iax != xno; then
   has_iax2=yes
   enable_local_iax2=yes
   IAX2_CFLAGS='-I$(top_srcdir)/lib/libiax2/src -DLIBIAX'
   IAX2_LIBS=""
   IAX2_CONFIG=""
fi

AM_OPTIONS_WXCONFIG

has_wx="no"
AM_PATH_WXCONFIG(2.6.0, [has_wx="2.6"],
      [AM_PATH_WXCONFIG(2.4.0, [has_wx="2.4"])])
AM_PATH_WXRC()

dnl Begin wx xrc check
save_CPPFLAGS="$CPPFLAGS"
save_CXXFLAGS="$CXXFLAGS"
save_LIBS="$LIBS"

CPPFLAGS="$WX_CPPFLAGS"
CXXFLAGS="$WX_CXXFLAGS_ONLY"

AC_LANG_PUSH(C++)
if test x$has_wx != xno; then
   AC_CHECK_HEADER(wx/xrc/xmlres.h,has_wx_xrc=yes,
         [AC_MSG_WARN([Can't find wx/xrc/xml.h])
         has_wx_xrc=no])
fi

if test x$has_wx_xrc = xyes; then

dnl Check for debug libraries
   wx_lib_prefix=

   case $WX_CXXFLAGS in
      *__WXDEBUG__*) wx_lib_prefix=d
   esac

   has_wx_xrc=xno
   AC_MSG_CHECKING([for wx xrc library])
   for wx_xrc_lib in " " "-lwx_gtk${wx_lib_prefix}_xrc-2.4" "-lwx_msw${wx_lib_prefix}_xrc-2.4" "-lwxxrc"; do
      LIBS="$wx_xrc_lib $WX_LIBS $save_LIBS"
      AC_LINK_IFELSE([
#include <wx/xrc/xmlres.h>
int main()
{
	wxXmlResource *res;
	res->Get();
	return 0;
}],[has_wx_xrc=yes
   WX_XRC_LIBS="$wx_xrc_lib"
   break])
   done

   AC_MSG_RESULT([$has_wx_xrc ($WX_XRC_LIBS)])
fi
AC_SUBST(WX_XRC_LIBS)

AC_LANG_POP(C++)

LIBS="$save_LIBS"
CPPFLAGS="$save_CPPFLAGS"
CXXFLAGS="$save_CXXFLAGS"
dnl End wx xrc check

if test x$enable_video = xyes; then
   AC_DEFINE(USE_VIDEO, 1, [Define to 1 to enable video support])
fi

if test x$use_echo_can = xspan; then
   AC_DEFINE(SPAN_EC, 1, [Define to 1 to enable span echo cancellation])
fi

if test x$use_echo_can = xspeex; then
   AC_DEFINE(SPEEX_EC, 1, [Define to 1 to enable speex echo cancellation])
fi

if test x$enable_local_ilbc = xyes; then
   AC_DEFINE(CODEC_ILBC,,[Define to 1 to enable ILBC support])
fi

if test x$has_gsm = xyes && test x$with_gsm != xno; then
   AC_DEFINE(CODEC_GSM,,[Define to 1 to enable GSM support])
fi

AM_CONDITIONAL(VIDEO, test x$enable_video = xyes)
AM_CONDITIONAL(OGG, test x$has_ogg = xyes)
AM_CONDITIONAL(THEORA, test x$has_theora = xyes)
AM_CONDITIONAL(VIDCAP, test x$has_vidcap = xyes)
AM_CONDITIONAL(FFMPEG, test x$has_ffmpeg = xyes)
AM_CONDITIONAL(SPAN_EC, test x$use_echo_can = xspan)
AM_CONDITIONAL(USE_CODEC_GSM, test x$has_gsm = xyes && test x$with_gsm != xno)
AM_CONDITIONAL(USE_LOCAL_GSM, test x$enable_local_gsm = xyes)
AM_CONDITIONAL(USE_LOCAL_IAX2, test x$enable_local_iax2 = xyes)
AM_CONDITIONAL(USE_DEBUG_IAX2, test x$enable_debug_iax2 = xyes)
AM_CONDITIONAL(USE_LOCAL_ILBC, test x$enable_local_ilbc = xyes)
AM_CONDITIONAL(LINUX, test x$OSTYPE = xLINUX)
AM_CONDITIONAL(WIN32, test x$OSTYPE = xWIN32)
AM_CONDITIONAL(MACOSX, test x$OSTYPE = xMACOSX)

# Autodetect clients
if test "x$enable_clients" != "xauto"; then
   for client in ${enable_clients}; do
      case "$client" in
         iaxcomm | iaxphone | stresstest | testcall | tkphone | vtestcall | WinIAX | wx)
            clients="$clients $client" ;;
         all | yes)
            clients="iaxcomm iaxphone stresstest testcall tkphone vtestcall WinIAX wx"
            break ;;
         none | no)
            clients=""
            break ;;
         *)
            AC_MSG_ERROR(bad value ${client} for --enable-clients) ;;
      esac
   done
else
   clients="$clients testcall"

   if test x$enable_video = xyes && test x$has_oggz = xyes && test x$has_theora = xyes; then
      clients="$clients stresstest"
   fi

   if test x$has_wx != xno; then
      clients="$clients iaxphone"
      if test "x$has_gdk2" = xyes; then
         clients="$clients wx"
      fi

      if test x$has_wx_xrc = xyes; then
         clients="$clients iaxcomm"
      fi
   fi

   if test x$has_sdl = xyes && test x$enable_video = xyes; then
      clients="$clients vtestcall"
   fi

   if test x$OSTYPE = xWIN32; then
      clients="$clients WinIAX"
   fi

   if test x$WISH != x && test "x$has_gdk2" = "xyes"; then
      clients="$clients tkphone"
   fi
fi

for client in $clients
do
   case "$client" in
      testcall)
         CLIENTS="$CLIENTS $client";;

      stresstest)
         CLIENTS="$CLIENTS $client";;

      vtestcall)
         if test x$has_sdl != xyes || test x$enable_video != xyes ; then
            AC_MSG_ERROR([vtestcall requires SDL and video])
         fi
         CLIENTS="$CLIENTS $client";;

      iaxcomm)
         CLIENTS="$CLIENTS $client";;

      iaxphone)
         CLIENTS="$CLIENTS $client";;

      tkphone)
         if ! test "x$has_gtk2" = "xyes"; then
            AC_MSG_ERROR([tkphone requires GTK2])
         fi
         if ! test "x$has_gdk2" = "xyes"; then
            AC_MSG_ERROR([tkphone requires GDK2])
         fi
         CLIENTS="$CLIENTS $client";;

      WinIAX)
         CLIENTS="$CLIENTS $client";;

      wx)
         if test "x$has_wx" = "xno"; then
            AC_MSG_ERROR([wx client requires wxWidgets])
         fi
         if ! test "x$has_gtk2" = "xyes"; then
            AC_MSG_ERROR([wx client requires GTK2])
         fi
         if ! test "x$has_gdk2" = "xyes"; then
            AC_MSG_ERROR([wx client requires GDK2])
         fi
         CLIENTS="$CLIENTS $client";;

      *) ;;
   esac
done

AC_SUBST(CLIENTS)
AC_SUBST(PKG_REQUIRES)

AC_CONFIG_FILES([
      Makefile
      lib/Makefile
      simpleclient/Makefile
      iaxclient.pc
      simpleclient/testcall/Makefile
      simpleclient/stresstest/Makefile
      simpleclient/vtestcall/Makefile
      simpleclient/iaxcomm/Makefile
      simpleclient/iaxphone/Makefile
      simpleclient/tkphone/Makefile
      simpleclient/WinIAX/Makefile
      simpleclient/wx/Makefile
])

AC_OUTPUT

AC_MSG_NOTICE([Configured clients: $clients])
