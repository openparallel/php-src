dnl $Id$
dnl config.m4 for extension pdo_sqlite

PHP_ARG_WITH(pdo-sqlite, for sqlite 3 driver for PDO,
[  --with-pdo-sqlite             Include PDO sqlite 3 support])

if test "$PHP_PDO_SQLITE" != "no"; then
  SEARCH_PATH="$PHP_PDO_SQLITE /usr/local /usr"     # you might want to change this
  SEARCH_FOR="/include/sqlite3.h"  # you most likely want to change this
  if test -r $PHP_PDO_SQLITE/$SEARCH_FOR; then # path given as parameter
    PDO_SQLITE_DIR=$PHP_PDO_SQLITE
  else # search default path list
    AC_MSG_CHECKING([for sqlite3 files in default path])
    for i in $SEARCH_PATH ; do
      if test -r $i/$SEARCH_FOR; then
        PDO_SQLITE_DIR=$i
        AC_MSG_RESULT(found in $i)
      fi
    done
  fi
  if test -z "$PDO_SQLITE_DIR"; then
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Please reinstall the sqlite3 distribution])
  fi

  PHP_ADD_INCLUDE($PDO_SQLITE_DIR/include)

  LIBNAME=sqlite3
  LIBSYMBOL=sqlite3_open

  PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  [
    PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $PDO_SQLITE_DIR/lib, PDO_SQLITE_SHARED_LIBADD)
    AC_DEFINE(HAVE_PDO_SQLITELIB,1,[ ])
  ],[
    AC_MSG_ERROR([wrong sqlite lib version or lib not found])
  ],[
    -L$PDO_SQLITE_DIR/lib -lm
  ])
  PHP_SUBST(PDO_SQLITE_SHARED_LIBADD)

  AC_MSG_CHECKING([for PDO includes])
  if test -f $prefix/include/php/ext/pdo/php_pdo_driver.h; then
  	pdo_inc_path=$prefix/include/php/ext
  elif test -f $abs_srcdir/include/php/ext/pdo/php_pdo_driver.h; then
  	pdo_inc_path=$abs_srcdir/ext
  elif test -f ext/pdo/php_pdo_driver.h; then
  	pdo_inc_path=ext
  else
	AC_MSG_ERROR([Cannot find php_pdo_driver.h.])
  fi
  AC_MSG_RESULT($pdo_inc_path)

  PHP_NEW_EXTENSION(pdo_sqlite, pdo_sqlite.c sqlite_driver.c sqlite_statement.c, $ext_shared,,-I$pdo_inc_path)
fi
