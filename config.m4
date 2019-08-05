dnl $Id$
dnl config.m4 for extension ecdh

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(ecdh, for ecdh support,
dnl Make sure that the comment is aligned:
dnl [  --with-ecdh             Include ecdh support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(ecdh, whether to enable ecdh support,
Make sure that the comment is aligned:
[  --enable-ecdh           Enable ecdh support])

if test "$PHP_ECDH" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-ecdh -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/ecdh.h"  # you most likely want to change this
  dnl if test -r $PHP_ECDH/$SEARCH_FOR; then # path given as parameter
  dnl   ECDH_DIR=$PHP_ECDH
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for ecdh files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       ECDH_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$ECDH_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the ecdh distribution])
  dnl fi

  dnl # --with-ecdh -> add include path
  dnl PHP_ADD_INCLUDE($ECDH_DIR/include)

  dnl # --with-ecdh -> check for lib and symbol presence
  dnl LIBNAME=ecdh # you may want to change this
  dnl LIBSYMBOL=ecdh # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $ECDH_DIR/$PHP_LIBDIR, ECDH_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_ECDHLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong ecdh lib version or lib not found])
  dnl ],[
  dnl   -L$ECDH_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(ECDH_SHARED_LIBADD)

  PHP_NEW_EXTENSION(ecdh, ecdh.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
