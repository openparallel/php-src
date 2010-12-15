dnl $Id$
dnl config.m4 for extension tbb

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(tbb, for tbb support,
dnl Make sure that the comment is aligned:
dnl [  --with-tbb             Include tbb support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(tbb, whether to enable tbb support,
dnl Make sure that the comment is aligned:
dnl [  --enable-tbb           Enable tbb support])

if test "$PHP_TBB" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-tbb -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/tbb.h"  # you most likely want to change this
  dnl if test -r $PHP_TBB/$SEARCH_FOR; then # path given as parameter
  dnl   TBB_DIR=$PHP_TBB
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for tbb files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       TBB_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$TBB_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the tbb distribution])
  dnl fi

  dnl # --with-tbb -> add include path
  dnl PHP_ADD_INCLUDE($TBB_DIR/include)

  dnl # --with-tbb -> check for lib and symbol presence
  dnl LIBNAME=tbb # you may want to change this
  dnl LIBSYMBOL=tbb # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $TBB_DIR/lib, TBB_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_TBBLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong tbb lib version or lib not found])
  dnl ],[
  dnl   -L$TBB_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(TBB_SHARED_LIBADD)

  PHP_NEW_EXTENSION(tbb, tbb.c, $ext_shared)
fi
