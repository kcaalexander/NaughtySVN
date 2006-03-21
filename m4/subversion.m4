dnl Copyright here
dnl From: Michele Mattioni <michele.mattioni@poste.it>
dnl Tweaked by: Alexander Thomas <alexander@collab.net>

AC_DEFUN([AM_CHECK_SVN_HEADER],
[
  SVN_INCLUDES="/usr/local/include /usr/include"

  dnl Checking for user supplied subversion include path.
  AC_ARG_WITH(svn-include,
  [[  --with-svn-include=DIR   Use the given path to the subversion headers.]],
  [
    if test "$withval" != "yes" -a "$withval" != ""; then
      SVN_INCLUDES=$withval
    fi
  ])
  
  dnl Checking for the presence of subversion header file svn_types.h.
  AC_MSG_CHECKING([for Subversion headers])
  SVN_INCLUDE=""
  for VALUE in $SVN_INCLUDES ; do
    if test -f $VALUE/subversion-1/svn_types.h ; then
      SVN_INCLUDE=$VALUE
      break
    fi
  done
  if test $SVN_INCLUDE ; then
    AC_MSG_RESULT([found])
  else
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Subversion headers are required. Try --with-svn-include.])
  fi

  dnl Exportingites of AC_LIB_LINKFLAGS_BODY.
  SVN_CFLAGS="-I $SVN_INCLUDE/subversion-1"
  AC_SUBST(SVN_CFLAGS)
])


AC_DEFUN([AM_CHECK_SVN_LIB],
[
  SVN_LIBS="/usr/local/lib /usr/lib"

  dnl Checking for user supplied subversion library path.
  AC_ARG_WITH(svn-lib,
  [[  --with-svn-lib=DIR  Use the given path to the subversion libraries.]],
  [
    if test "$withval" != "yes" -a "$withval" != ""; then
        SVN_LIBS=$withval
    fi
  ])

  dnl Checking for presence for subversion librarie libsvn_client.
  AC_MSG_CHECKING([for Subversion libraries])
  SVN_LIB=""
  for VALUE in $SVN_LIBS ; do
    if ls $VALUE/libsvn_client-1.* >/dev/null 2>&1; then
      SVN_LIB=$VALUE
      break
    fi
  done
  if test $SVN_LIB ; then
    AC_MSG_RESULT([found])
  else
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Subversion libraries are required. Try --with-svn-lib.])
  fi

  dnl Exporting Subversion library path and libraries.
  SVN_LIBS="-L$SVN_LIB -lsvn_client-1 -lsvn_wc-1 -lsvn_ra-1 -lsvn_delta-1 -lsvn_subr-1"
  AC_SUBST(SVN_LIBS)
])
