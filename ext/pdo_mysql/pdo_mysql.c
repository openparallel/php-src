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
  | Author: George Schlossnagle <george@omniti.com>                      |
  |         Johannes Schlueter <johannes@mysql.com>                      |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "pdo/php_pdo.h"
#include "pdo/php_pdo_driver.h"
#include "php_pdo_mysql.h"
#include "php_pdo_mysql_int.h"

#ifdef COMPILE_DL_PDO_MYSQL
ZEND_GET_MODULE(pdo_mysql)
#endif

ZEND_DECLARE_MODULE_GLOBALS(pdo_mysql);

#ifndef PHP_WIN32
# ifndef PDO_MYSQL_UNIX_ADDR
#  ifdef PHP_MYSQL_UNIX_SOCK_ADDR
#   define PDO_MYSQL_UNIX_ADDR PHP_MYSQL_UNIX_SOCK_ADDR
#  else
#   define PDO_MYSQL_UNIX_ADDR "/tmp/mysql.sock"
#  endif
# endif
#endif

/* {{{ PHP_INI_BEGIN
*/
PHP_INI_BEGIN()
#ifndef PHP_WIN32
	STD_PHP_INI_ENTRY("pdo_mysql.default_socket", PDO_MYSQL_UNIX_ADDR, PHP_INI_SYSTEM, OnUpdateStringUnempty, default_socket, zend_pdo_mysql_globals, pdo_mysql_globals)
#endif
#if PDO_DBG_ENABLED
	STD_PHP_INI_ENTRY("pdo_mysql.debug",	NULL, PHP_INI_SYSTEM, OnUpdateString, debug, zend_pdo_mysql_globals, pdo_mysql_globals)
#endif
PHP_INI_END()
/* }}} */

/* true global environment */		
		
/* {{{ PHP_MINIT_FUNCTION
 */
static PHP_MINIT_FUNCTION(pdo_mysql)
{
	REGISTER_INI_ENTRIES();

	REGISTER_PDO_CLASS_CONST_LONG("MYSQL_ATTR_USE_BUFFERED_QUERY", (long)PDO_MYSQL_ATTR_USE_BUFFERED_QUERY);
	REGISTER_PDO_CLASS_CONST_LONG("MYSQL_ATTR_LOCAL_INFILE", (long)PDO_MYSQL_ATTR_LOCAL_INFILE);	
	REGISTER_PDO_CLASS_CONST_LONG("MYSQL_ATTR_INIT_COMMAND", (long)PDO_MYSQL_ATTR_INIT_COMMAND);
#ifndef PDO_USE_MYSQLND
	REGISTER_PDO_CLASS_CONST_LONG("MYSQL_ATTR_MAX_BUFFER_SIZE", (long)PDO_MYSQL_ATTR_MAX_BUFFER_SIZE);
	REGISTER_PDO_CLASS_CONST_LONG("MYSQL_ATTR_READ_DEFAULT_FILE", (long)PDO_MYSQL_ATTR_READ_DEFAULT_FILE);
	REGISTER_PDO_CLASS_CONST_LONG("MYSQL_ATTR_READ_DEFAULT_GROUP", (long)PDO_MYSQL_ATTR_READ_DEFAULT_GROUP);
	REGISTER_PDO_CLASS_CONST_LONG("MYSQL_ATTR_COMPRESS", (long)PDO_MYSQL_ATTR_COMPRESS);
#endif
	REGISTER_PDO_CLASS_CONST_LONG("MYSQL_ATTR_DIRECT_QUERY", (long)PDO_MYSQL_ATTR_DIRECT_QUERY);
	REGISTER_PDO_CLASS_CONST_LONG("MYSQL_ATTR_FOUND_ROWS", (long)PDO_MYSQL_ATTR_FOUND_ROWS);
	REGISTER_PDO_CLASS_CONST_LONG("MYSQL_ATTR_IGNORE_SPACE", (long)PDO_MYSQL_ATTR_IGNORE_SPACE);

	return php_pdo_register_driver(&pdo_mysql_driver);
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
static PHP_MSHUTDOWN_FUNCTION(pdo_mysql)
{
	php_pdo_unregister_driver(&pdo_mysql_driver);
#if PDO_USE_MYSQLND
	UNREGISTER_INI_ENTRIES();
#endif

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
static PHP_MINFO_FUNCTION(pdo_mysql)
{
	php_info_print_table_start();

	php_info_print_table_header(2, "PDO Driver for MySQL", "enabled");
	php_info_print_table_row(2, "Client API version", mysql_get_client_info());

	php_info_print_table_end();

#ifdef PDO_USE_MYSQLND
	DISPLAY_INI_ENTRIES();
#endif
}
/* }}} */


#if PDO_USE_MYSQLND && PDO_DBG_ENABLED
/* {{{ PHP_RINIT_FUNCTION
 */
static PHP_RINIT_FUNCTION(pdo_mysql)
{	
	if (PDO_MYSQL_G(debug)) {
		MYSQLND_DEBUG *dbg = mysqlnd_debug_init(mysqlnd_debug_std_no_trace_funcs TSRMLS_CC);
		if (!dbg) {
			return FAILURE;
		}
		dbg->m->set_mode(dbg, PDO_MYSQL_G(debug));
		PDO_MYSQL_G(dbg) = dbg;
	}
	
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
static PHP_RSHUTDOWN_FUNCTION(pdo_mysql)
{
	MYSQLND_DEBUG *dbg = PDO_MYSQL_G(dbg);
	PDO_DBG_ENTER("RSHUTDOWN");
	if (dbg) {
		dbg->m->close(dbg);
		dbg->m->free_handle(dbg);
		PDO_MYSQL_G(dbg) = NULL;
	}

	return SUCCESS;
}
/* }}} */
#endif

/* {{{ PHP_GINIT_FUNCTION
 */
static PHP_GINIT_FUNCTION(pdo_mysql)
{
#ifndef PHP_WIN32
	pdo_mysql_globals->default_socket = NULL;
#endif
#if PDO_DBG_ENABLED
	pdo_mysql_globals->debug = NULL;	/* The actual string */
	pdo_mysql_globals->dbg = NULL;	/* The DBG object*/
#endif
}
/* }}} */

/* {{{ pdo_mysql_functions[] */
const zend_function_entry pdo_mysql_functions[] = {
	{NULL, NULL, NULL}
};
/* }}} */

/* {{{ pdo_mysql_deps[] */
#if ZEND_MODULE_API_NO >= 20050922
static const zend_module_dep pdo_mysql_deps[] = {
	ZEND_MOD_REQUIRED("pdo")
#ifdef PDO_USE_MYSQLND
	ZEND_MOD_REQUIRED("mysqlnd")
#endif
	{NULL, NULL, NULL}
};
#endif
/* }}} */

/* {{{ pdo_mysql_module_entry */
zend_module_entry pdo_mysql_module_entry = {
	STANDARD_MODULE_HEADER_EX, NULL,
	pdo_mysql_deps,
	"pdo_mysql",
	pdo_mysql_functions,
	PHP_MINIT(pdo_mysql),
	PHP_MSHUTDOWN(pdo_mysql),
#if PDO_USE_MYSQLND && PDO_DBG_ENABLED
	PHP_RINIT(pdo_mysql),
	PHP_RSHUTDOWN(pdo_mysql),
#else
	NULL,
	NULL,
#endif
	PHP_MINFO(pdo_mysql),
	"1.0.2",
	PHP_MODULE_GLOBALS(pdo_mysql),
	PHP_GINIT(pdo_mysql),
	NULL,
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
};
/* }}} */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
