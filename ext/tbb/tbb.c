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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_tbb.h"

#include "Zend/zend_execute.h"

// #include <tbb/tbb.h>

/* If you declare any globals in php_tbb.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(tbb)
*/

/* True global resources - no need for thread safety here */
static int le_tbb;

/* {{{ tbb_functions[]
 *
 * Every user visible function must have an entry in tbb_functions[].
 */
const zend_function_entry tbb_functions[] = {
    PHP_FE(parallel_map, NULL)
	/* Coming Soon - PHP_FE(parallel_reduce, NULL) */
	{NULL, NULL, NULL}	/* Must be the last line in tbb_functions[] */
};
/* }}} */

/* {{{ tbb_module_entry
 */
zend_module_entry tbb_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"tbb",
	tbb_functions,
	PHP_MINIT(tbb),
	PHP_MSHUTDOWN(tbb),
	PHP_RINIT(tbb),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(tbb),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(tbb),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_TBB
ZEND_GET_MODULE(tbb)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("tbb.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_tbb_globals, tbb_globals)
    STD_PHP_INI_ENTRY("tbb.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_tbb_globals, tbb_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_tbb_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_tbb_init_globals(zend_tbb_globals *tbb_globals)
{
	tbb_globals->global_value = 0;
	tbb_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(tbb)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(tbb)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(tbb)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(tbb)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(tbb)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "tbb support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* {{{ proto array parallel_map(mixed callback, array arg [, int grainsize = 0])
   Loop over an array, running the user supplied callback for each key.
   Returns the modified array. */
PHP_FUNCTION(parallel_map)
{
    zval *array = NULL;
    zval ***params = NULL;
    zval *result = NULL;
	HashPosition array_pos;

    zend_fcall_info fci = empty_fcall_info;
    zend_fcall_info_cache fci_cache = empty_fcall_info_cache;

    long grainsize = 0;
    unsigned int items = 0;
	int k;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "fa|l", &fci, &fci_cache, &array, &grainsize) == FAILURE) {
        return;
    }

	items = zend_hash_num_elements(Z_ARRVAL(*array));
	array_init_size(return_value, items);
	zend_hash_internal_pointer_reset_ex(Z_ARRVAL(*array), &array_pos);
	params = (zval ***)safe_emalloc(1, sizeof(zval **), 0);

	for (k = 0; k < items; k++) {
		uint str_key_len;
		ulong num_key;
		char *str_key;
		int key_type = 0;

		MAKE_STD_ZVAL(result);
		fci.retval_ptr_ptr = &result;
		fci.param_count = 1;
		fci.params = params;
		fci.no_separation = 0;

		zend_hash_get_current_data_ex( NULL, (void**)&params[0], &array_pos );
		zend_hash_move_forward_ex( NULL, &array_pos );

		if (zend_call_function(&fci, &fci_cache TSRMLS_CC) != SUCCESS || !result) {
				php_error_docref(NULL TSRMLS_CC, E_WARNING, "An error occurred while invoking the parallel_map callback");
				zval_dtor(return_value);
				RETVAL_NULL();
				goto error_out;
		}

		add_next_index_zval(return_value, result);
	}

error_out:
	efree(params);
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
