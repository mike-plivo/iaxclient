# 2006-07-29 Mikael Magnusson updated to V19
# 2004-10-25 Mikael Magnusson stolen from speex.m4
# Configure paths for libspeex
# Jean-Marc Valin <jean-marc.valin@usherbrooke.ca>
# Shamelessly stolen from:
# Jack Moffitt <jack@icecast.org> 10-21-2000
# Shamelessly stolen from Owen Taylor and Manish Singh

dnl AM_PATH_PORTAUDIO([ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
dnl Test for libportaudio, and define PORTAUDIO_CFLAGS and PORTAUDIO_LIBS
dnl
AC_DEFUN([AM_PATH_PORTAUDIO],
[dnl 
dnl Get the cflags and libraries
dnl
AC_ARG_WITH(portaudio,[AS_HELP_STRING([--with-portaudio=PFX],[Prefix where libportaudio is installed (optional)])], portaudio_prefix="$withval", portaudio_prefix="")
AC_ARG_WITH(portaudio-libraries,[AS_HELP_STRING([--with-portaudio-libraries=DIR],[Directory where libportaudio library is installed (optional)])], portaudio_libraries="$withval", portaudio_libraries="")
AC_ARG_WITH(portaudio-includes,[AS_HELP_STRING([--with-portaudio-includes=DIR],[Directory where libportaudio header files are installed (optional)])], portaudio_includes="$withval", portaudio_includes="")
AC_ARG_ENABLE(portaudiotest, [AS_HELP_STRING([--disable-portaudiotest],[Do not try to compile and run a test Portaudio program])],, enable_portaudiotest=yes)

  if test "x$portaudio_libraries" != "x" ; then
    PORTAUDIO_LIBS="-L$portaudio_libraries"
  elif test "x$portaudio_prefix" != "x" ; then
    PORTAUDIO_LIBS="-L$portaudio_prefix/lib"
  elif test "x$prefix" != "xNONE" ; then
    PORTAUDIO_LIBS="-L$prefix/lib"
  fi

  PORTAUDIO_LIBS="$PORTAUDIO_LIBS -lportaudio"

  if test "x$portaudio_includes" != "x" ; then
    PORTAUDIO_CFLAGS="-I$portaudio_includes"
  elif test "x$portaudio_prefix" != "x" ; then
    PORTAUDIO_CFLAGS="-I$portaudio_prefix/include"
  elif test "x$prefix" != "xNONE"; then
    PORTAUDIO_CFLAGS="-I$prefix/include"
  fi

  AC_MSG_NOTICE(Checking for Portaudio)
  no_portaudio=""

dnl  AC_CHECK_HEADER(portaudio.h,,no_portaudio="yes")

dnl  AC_CHECK_LIB(portaudio, Pa_Initialize,,no_portaudio="yes")

  if test "x$enable_portaudiotest" = "xyes" ; then
    ac_save_CFLAGS="$CFLAGS"
    ac_save_LIBS="$LIBS"
    CFLAGS="$CFLAGS $PORTAUDIO_CFLAGS"
    LIBS="$LIBS $PORTAUDIO_LIBS"

    AC_MSG_CHECKING(for Portaudio)

dnl
dnl Now check if the installed Portaudio is sufficiently new.
dnl
      AC_LINK_IFELSE([
#include <stdio.h>
#include <portaudio.h>

int main ()
{
  PaDeviceIndex count;

  if ( Pa_Initialize() < 0 )
    return 1;

  if ( (count = Pa_GetDeviceCount()) < 0 )
    return 1;

  if ( Pa_Terminate() < 0 )
    return 1;

  return 0;
}

],,no_portaudio=yes)
       CFLAGS="$ac_save_CFLAGS"
       LIBS="$ac_save_LIBS"
  fi


  if test "x$no_portaudio" = "x" ; then
     AC_MSG_RESULT([yes])
     ifelse([$1], , :, [$1])
  else
     AC_MSG_RESULT([no (version $portaudio_version)])
       echo "*** Could not run Portaudio test program, checking why..."
       CFLAGS="$CFLAGS $PORTAUDIO_CFLAGS"
       LIBS="$LIBS $PORTAUDIO_LIBS"
       AC_TRY_LINK([
#include <stdio.h>
#include <portaudio.h>

int main ()
{
  PaDeviceIndex count;

  if ( Pa_Initialize() < 0 )
    return 1;

  if ( (count = Pa_GetDeviceCount()) < 0 )
    return 1;

  if ( Pa_Terminate() < 0 )
    return 1;

  return 0;
}
],     [ return 0; ],
       [ echo "*** The test program compiled, but did not run. This usually means"
       echo "*** that the run-time linker is not finding Portaudio or finding the wrong"
       echo "*** version of Portaudio. If it is not finding Portaudio, you'll need to set your"
       echo "*** LD_LIBRARY_PATH environment variable, or edit /etc/ld.so.conf to point"
       echo "*** to the installed location  Also, make sure you have run ldconfig if that"
       echo "*** is required on your system"
       echo "***"
       echo "*** If you have an old version installed, it is best to remove it, although"
       echo "*** you may also be able to get things to work by modifying LD_LIBRARY_PATH"],
       [ echo "*** The test program failed to compile or link. See the file config.log for the"
       echo "*** exact error that occured. This usually means Portaudio was incorrectly installed"
       echo "*** or that you have moved Portaudio since it was installed." ])
       CFLAGS="$ac_save_CFLAGS"
       LIBS="$ac_save_LIBS"

     PORTAUDIO_CFLAGS=""
     PORTAUDIO_LIBS=""
     ifelse([$2], , :, [$2])
  fi
  AC_SUBST(PORTAUDIO_CFLAGS)
  AC_SUBST(PORTAUDIO_LIBS)
])
