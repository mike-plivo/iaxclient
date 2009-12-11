# Configure paths for libtheora
# Karl Heyes 02-Feb-2004

dnl XIPH_PATH_THEORA([ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
dnl Test for libtheora, and define THEORA_CFLAGS THEORA_LIBS
dnl
AC_DEFUN([XIPH_PATH_THEORA],
[
dnl Get the cflags and libraries for theora
dnl
AC_ARG_WITH(theora,
    [AC_HELP_STRING([--with-theora=PFX],
    [Prefix where libtheora is installed (optional)])],
    theora_prefix="$withval",
    theora_prefix="")
AC_ARG_WITH(theora-libraries,
    [AC_HELP_STRING([--with-theora-libraries=DIR],
        [Directory where libtheora library is installed (optional)])],
    theora_libraries="$withval",
    theora_libraries="")
AC_ARG_WITH(theora-includes,
    [AC_HELP_STRING([--with-theora-includes=DIR],
        [Directory where libtheora header files are installed (optional)])],
    theora_includes="$withval",
    theora_includes="")
AC_ARG_ENABLE(theoratest,
    [AC_HELP_STRING([--disable-theoratest],
    [Do not try to compile and run a theora test program])],
    ,
    enable_theoratest=yes)

  if test "x$theora_libraries" != "x"; then
    THEORA_LIBS="-L$theora_libraries"
  elif test "x$theora_prefix" != "x"; then
    THEORA_LIBS="-L$theora_prefix/lib"
  elif test "x$prefix" != "xNONE"; then
    THEORA_LIBS="-L$prefix/lib"
  fi

  THEORA_LIBS="$THEORA_LIBS -ltheora"

  if test "x$theora_includes" != "x"; then
    THEORA_CFLAGS="-I$theora_includes"
  elif test "x$theora_prefix" != "x"; then
    THEORA_CFLAGS="-I$theora_prefix/include"
  elif test "x$prefix" != "xNONE"; then
    THEORA_CFLAGS="-I$prefix/include"
  fi

  THEORA_CFLAGS="$THEORA_CFLAGS $OGG_CFLAGS"

  no_theora=""

  if test "x$enable_theoratest" = "xyes"; then
    ac_save_CFLAGS="$CFLAGS"
    ac_save_LIBS="$LIBS"
    CFLAGS="$CFLAGS $THEORA_CFLAGS"
    LIBS="$LIBS $THEORA_LIBS"

    AC_MSG_CHECKING([for libtheora])
    rm -f conf.theoratest
    AC_TRY_RUN([
#include <stdio.h>
#include <stdlib.h>
#include <theora/theora.h>
int main()
{
  system("touch conf.theoratest");
  return 0;
}

],,no_theora=yes,[echo $ac_n "cross_compiling; assumed OK... $ac_c"])
    CFLAGS="$ac_save_CFLAGS"
    LIBS="$ac_save_LIBS"
  fi

  if test "x$no_theora" = "x"; then
    AC_MSG_RESULT([yes])
    ifelse([$1], , :, [$1])     
  else
    AC_MSG_RESULT([no])
    if test -f conf.theoratest; then
      :
    else
       echo "*** Could not run theora test program, checking why..."
       CFLAGS="$CFLAGS $THEORA_CFLAGS"
       LIBS="$LIBS $THEORA_LIBS"
       AC_TRY_LINK([
#include <stdio.h>
#include <theora/theora.h>
],     [ return 0; ],
       [ echo "*** The test program compiled, but did not run. This usually means"
       echo "*** that the run-time linker is not finding theora or finding the wrong"
       echo "*** version of theora. If it is not finding theora, you'll need to set your"
       echo "*** LD_LIBRARY_PATH environment variable, or edit /etc/ld.so.conf to point"
       echo "*** to the installed location  Also, make sure you have run ldconfig if that"
       echo "*** is required on your system"
       echo "***"
       echo "*** If you have an old version installed, it is best to remove it, although"
       echo "*** you may also be able to get things to work by modifying LD_LIBRARY_PATH"],
       [ echo "*** The test program failed to compile or link. See the file config.log for the"
       echo "*** exact error that occured. This usually means theora was incorrectly installed"
       echo "*** or that you have moved theora since it was installed." ])
       CFLAGS="$ac_save_CFLAGS"
       LIBS="$ac_save_LIBS"
     fi
     THEORA_CFLAGS=""
     THEORA_LIBS=""
     ifelse([$2], , :, [$2])
  fi
  AC_SUBST(THEORA_CFLAGS)
  AC_SUBST(THEORA_LIBS)
  rm -f conf.theoratest
])

