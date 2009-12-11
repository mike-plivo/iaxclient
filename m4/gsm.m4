dnl 20041024 Downloaded from http://digium-cvs.netmonks.ca/viewcvs.cgi/*checkout*/gnophone/m4/gsm.m4?rev=1.2 
dnl This is a little s script written by Rob Flynn of Linux Support Services, Inc.
dnl Hopefully, it will allow better detection of the GSM libraries.  Who knows ;-)
dnl If you have any patches, send them to:
dnl
dnl rob@linux-support.net
dnl
dnl Thanks! :-)
dnl
dnl AM_PATH_GSM([ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
AC_DEFUN([AM_PATH_GSM],[
AC_ARG_WITH(gsm,
	[AS_HELP_STRING([--with-gsm=PFX],
	[Prefix where libgsm is installed (optional)])],
	gsm_prefix="$withval",
	gsm_prefix="")
AC_ARG_WITH(gsm-libraries,
	[AS_HELP_STRING([--with-gsm-libraries=DIR],
	[Directory where libgsm library is installed (optional)])],
	gsm_libraries="$withval",
	gsm_libraries="")
AC_ARG_WITH(gsm-includes,
	[AS_HELP_STRING([--with-gsm-includes=DIR],
	[Directory where libgsm header files are installed (optional)])],
	gsm_includes="$withval",
	gsm_includes="")
AC_ARG_ENABLE(gsmtest,
	[AS_HELP_STRING([--disable-gsmtest],
	[Do not try to compile and run a test gsm program])],
	,
	enable_gsmtest=yes)

  if test "x$gsm_libraries" != "x" ; then
    GSM_LIBS="-L$gsm_libraries"
  elif test "x$gsm_prefix" != "x" && test "x$gsm_prefix" != "xno" ; then
    GSM_LIBS="-L$gsm_prefix/lib"
  elif test "x$prefix" != "xNONE" ; then
    GSM_LIBS="-L$prefix/lib"
  fi

  GSM_LIBS="$GSM_LIBS -lgsm"

  if test "x$gsm_includes" != "x" ; then
    GSM_CFLAGS="-I$gsm_includes"
  elif test "x$gsm_prefix" != "x" && test "x$gsm_prefix" != "xno" ; then
    GSM_CFLAGS="-I$gsm_prefix/include"
  elif test "x$prefix" != "xNONE"; then
    GSM_CFLAGS="-I$prefix/include"
  fi

  no_gsm=""

  if test "x$enable_gsmtest" = "xyes" ; then
    ac_save_CFLAGS="$CFLAGS"
    ac_save_LIBS="$LIBS"
    CFLAGS="$CFLAGS $GSM_CFLAGS"
    LIBS="$LIBS $GSM_LIBS"

    AC_MSG_CHECKING([for gsm])
    AC_TRY_RUN([
#include <gsm.h>
int main ()
{
	gsm g = gsm_create();
	gsm_destroy(g);
	return 0;
}
    ],,no_gsm=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])
    CFLAGS="$ac_save_CFLAGS"
    LIBS="$ac_save_LIBS"
  fi

  if test "x$no_gsm" = "x" ; then
    AC_MSG_RESULT([yes])
    ifelse([$1], , :, [$1])
  else
    AC_MSG_RESULT([no])
    echo "*** Could not run gsm test program, checking why..."
    CFLAGS="$CFLAGS $GSM_CFLAGS"
    LIBS="$LIBS $GSM_LIBS"
    AC_TRY_LINK([
#include <gsm.h>
],  [ return 0; ],
    [ echo "*** The test program compiled, but did not run. This usually means"
      echo "*** that the run-time linker is not finding gsm or finding the wrong"
      echo "*** version of gsm. If it is not finding gsm, you'll need to set your"
      echo "*** LD_LIBRARY_PATH environment variable, or edit /etc/ld.so.conf to point"
      echo "*** to the installed location  Also, make sure you have run ldconfig if that"
      echo "*** is required on your system"
      echo "***"
      echo "*** If you have an old version installed, it is best to remove it, although"
      echo "*** you may also be able to get things to work by modifying LD_LIBRARY_PATH"],
    [ echo "*** The test program failed to compile or link. See the file config.log for the"
      echo "*** exact error that occured. This usually means gsm was incorrectly installed"
      echo "*** or that you have moved gsm since it was installed." ])
    CFLAGS="$ac_save_CFLAGS"
    LIBS="$ac_save_LIBS"
    GSM_CFLAGS=""
    GSM_LIBS=""
    ifelse([$2], , :, [$2])
  fi
  AC_SUBST(GSM_CFLAGS)
  AC_SUBST(GSM_LIBS)
])

