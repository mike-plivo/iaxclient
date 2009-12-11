# 20041024 Mikael Magnusson
# 20041024 Downloaded from http://digium-cvs.netmonks.ca/viewcvs.cgi/*checkout*/gnophone/m4/iax.m4?rev=1.2
# Configure paths for IAX
# Stolen and modifed by Rob Flynn
# Manish Singh    98-9-30
# stolen back from Frank Belew
# stolen from Manish Singh
# Shamelessly stolen from Owen Taylor

dnl AM_PATH_IAX2([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
dnl Test for IAX2, and define IAX2_CFLAGS and IAX2_LIBS
dnl
AC_DEFUN([AM_PATH_IAX2],
[

AC_ARG_WITH(iax2-prefix,[AS_HELP_STRING([--with-iax2-prefix=PFX],[Prefix where IAX is installed (optional)])],
            iax2_prefix="$withval", iax2_prefix="")
AC_ARG_WITH(iax2-exec-prefix,[AS_HELP_STRING([--with-iax2-exec-prefix=PFX],[Exec prefix where IAX is installed (optional)])],
            iax2_exec_prefix="$withval", iax2_exec_prefix="")
AC_ARG_ENABLE(iax2test, [AS_HELP_STRING([--disable-iax2test],[Do not try to compile and run a test IAX program])],
		    , enable_iax2test=yes)

  if test x$iax2_exec_prefix != x ; then
     iax2_args="$iax2_args --exec-prefix=$iax2_exec_prefix"
     if test x${IAX2_CONFIG+set} != xset ; then
        IAX2_CONFIG=$iax2_exec_prefix/bin/iax-config
     fi
  fi
  if test x$iax2_prefix != x ; then
     iax2_args="$iax2_args --prefix=$iax2_prefix"
     if test x${IAX2_CONFIG+set} != xset ; then
        IAX2_CONFIG=$iax2_prefix/bin/iax-config
     fi
  fi

  AC_PATH_PROG(IAX2_CONFIG, iax-config, no)
  min_iax2_version=ifelse([$1], ,0.2.2,$1)
  AC_MSG_CHECKING(for IAX - version >= $min_iax2_version)
  no_iax=""
  if test "$IAX2_CONFIG" = "no" ; then
    no_iax=yes
  else
    IAX2_CFLAGS=`$IAX2_CONFIG $iaxconf_args --cflags`
    IAX2_LIBS=`$IAX2_CONFIG $iaxconf_args --libs`

    iax2_major_version=`$IAX2_CONFIG $iax2_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    iax2_minor_version=`$IAX2_CONFIG $iax2_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    iax2_micro_version=`$IAX2_CONFIG $iax2_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
    if test "x$enable_iax2test" = "xyes" ; then
      ac_save_CFLAGS="$CFLAGS"
      ac_save_LIBS="$LIBS"
      CFLAGS="$CFLAGS $IAX2_CFLAGS"
      LIBS="$LIBS $IAX2_LIBS"
dnl
dnl Now check if the installed IAX is sufficiently new. (Also sanity
dnl checks the results of iax-config to some extent
dnl
      rm -f conf.iaxtest
      AC_TRY_RUN([
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iax/iax2.h>

char*
my_strdup (char *str)
{
  char *new_str;
  
  if (str)
    {
      new_str = malloc ((strlen (str) + 1) * sizeof(char));
      strcpy (new_str, str);
    }
  else
    new_str = NULL;
  
  return new_str;
}

int main ()
{
  int major, minor, micro;
  char *tmp_version;

  system ("touch conf.iaxtest");

  /* HP/UX 9 (%@#!) writes to sscanf strings */
  tmp_version = my_strdup("$min_iax2_version");
  if (sscanf(tmp_version, "%d.%d.%d", &major, &minor, &micro) != 3) {
     printf("%s, bad version string\n", "$min_iax2_version");
     exit(1);
   }

   if (($iax2_major_version > major) ||
      (($iax2_major_version == major) && ($iax2_minor_version > minor)) ||
      (($iax2_major_version == major) && ($iax2_minor_version == minor) && ($iax2_micro_version >= micro)))
    {
      return 0;
    }
  else
    {
      printf("\n*** 'iax-config --version' returned %d.%d.%d, but the minimum version\n", $iax2_major_version, $iax2_minor_version, $iax2_micro_version);
      printf("*** of IAX required is %d.%d.%d. If iax-config is correct, then it is\n", major, minor, micro);
      printf("*** best to upgrade to the required version.\n");
      printf("*** If iax-config was wrong, set the environment variable IAX_CONFIG\n");
      printf("*** to point to the correct copy of iax-config, and remove the file\n");
      printf("*** config.cache before re-running configure\n");
      return 1;
    }
}

],, no_iax=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])
       CFLAGS="$ac_save_CFLAGS"
       LIBS="$ac_save_LIBS"
     fi
  fi
  if test "x$no_iax" = x ; then
     AC_MSG_RESULT(yes)
     ifelse([$2], , :, [$2])     
  else
     AC_MSG_RESULT(no)
     if test "$IAX2_CONFIG" = "no" ; then
       echo "*** The iax-config script installed by IAX could not be found"
       echo "*** If IAX was installed in PREFIX, make sure PREFIX/bin is in"
       echo "*** your path, or set the IAX_CONFIG environment variable to the"
       echo "*** full path to iax-config."
     else
       if test -f conf.iaxtest ; then
        :
       else
          echo "*** Could not run IAX test program, checking why..."
          CFLAGS="$CFLAGS $IAX2_CFLAGS"
          LIBS="$LIBS $IAX2_LIBS"
          AC_TRY_LINK([
#include <stdio.h>
#include <iax/iax2.h>
],      [ return 0; ],
        [ echo "*** The test program compiled, but did not run. This usually means"
          echo "*** that the run-time linker is not finding IAX or finding the wrong"
          echo "*** version of IAX. If it is not finding IAX, you'll need to set your"
          echo "*** LD_LIBRARY_PATH environment variable, or edit /etc/ld.so.conf to point"
          echo "*** to the installed location  Also, make sure you have run ldconfig if that"
          echo "*** is required on your system"
	  echo "***"
          echo "*** If you have an old version installed, it is best to remove it, although"
          echo "*** you may also be able to get things to work by modifying LD_LIBRARY_PATH"],
        [ echo "*** The test program failed to compile or link. See the file config.log for the"
          echo "*** exact error that occured. This usually means IAX was incorrectly installed"
          echo "*** or that you have moved IAX since it was installed. In the latter case, you"
          echo "*** may want to edit the iax-config script: $IAX2_CONFIG" ])
          CFLAGS="$ac_save_CFLAGS"
          LIBS="$ac_save_LIBS"
       fi
     fi
     IAX2_CFLAGS=""
     IAX2_LIBS=""
     ifelse([$3], , :, [$3])
  fi
  AC_SUBST(IAX2_CFLAGS)
  AC_SUBST(IAX2_LIBS)
  rm -f conf.iaxtest
])
