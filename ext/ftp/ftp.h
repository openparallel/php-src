/* $Id$ */

#ifndef	_INCLUDED_FTP_H
#define	_INCLUDED_FTP_H

#if COMPILE_DL
#undef HAVE_FTP
#define HAVE_FTP 1
#endif

#if HAVE_FTP

extern php3_module_entry php3_ftp_module_entry;
#define php3_ftp_module_ptr &php3_ftp_module_entry

extern PHP_MINIT_FUNCTION(ftp);

PHP_FUNCTION(ftp_connect);
PHP_FUNCTION(ftp_login);
PHP_FUNCTION(ftp_chdir);
PHP_FUNCTION(ftp_nlist);
PHP_FUNCTION(ftp_get);
PHP_FUNCTION(ftp_put);
PHP_FUNCTION(ftp_quit);

#define phpext_ftp_ptr php3_ftp_module_ptr

#else
#define php3_ftp_module_ptr NULL
#endif	/* HAVE_FTP */

#endif
