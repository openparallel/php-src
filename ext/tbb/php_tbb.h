/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2010 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_TBB_H
#define PHP_TBB_H

extern zend_module_entry tbb_module_entry;
#define phpext_tbb_ptr &tbb_module_entry

#ifdef PHP_WIN32
#	define PHP_TBB_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_TBB_API __attribute__ ((visibility("default")))
#else
#	define PHP_TBB_API
#endif

#ifndef ZTS
#error PHP must be built with ZTS for the tbb extension to work.
#endif
#include "TSRM.h"

PHP_MINIT_FUNCTION(tbb);
PHP_MSHUTDOWN_FUNCTION(tbb);
PHP_RINIT_FUNCTION(tbb);
PHP_RSHUTDOWN_FUNCTION(tbb);
PHP_MINFO_FUNCTION(tbb);

PHP_FUNCTION(confirm_tbb_compiled);	/* For testing, remove later. */
PHP_FUNCTION(tbb_new_interp); /* Benchmarking interpreter creation */
PHP_FUNCTION(tbb_array_filter_ctx_test); /* Testing new interpreter contexts with code */

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(tbb)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(tbb)
*/

/* In every utility function you add that needs to use variables 
   in php_tbb_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as TBB_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define TBB_G(v) TSRMG(tbb_globals_id, zend_tbb_globals *, v)
#else
#define TBB_G(v) (tbb_globals.v)
#endif

#endif	/* PHP_TBB_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
