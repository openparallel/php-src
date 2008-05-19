# -*- makefile -*-

peardir=$(PEAR_INSTALLDIR)

# Skip all php.ini files altogether
PEAR_INSTALL_FLAGS = -n -dshort_open_tag=0 -dsafe_mode=0 -dopen_basedir= -derror_reporting=1803 -dmemory_limit=-1 -ddetect_unicode=0

install-pear-installer: $(SAPI_CLI_PATH)
	@$(top_builddir)/sapi/cli/php $(PEAR_INSTALL_FLAGS) $(builddir)/install-pear-nozlib.phar -d "$(peardir)" -b "$(bindir)"

install-pear:
	@echo "Installing PEAR environment:      $(INSTALL_ROOT)$(peardir)/"
	@if test ! -f $(builddir)/install-pear-nozlib.phar; then \
		if test -f $(srcdir)/install-pear-nozlib.phar; then \
			cp $(srcdir)/install-pear-nozlib.phar $(builddir)/install-pear-nozlib.phar; \
		else \
			$(top_builddir)/sapi/cli/php -n $(srcdir)/fetch.php http://pear.php.net/install-pear-nozlib.phar $(builddir)/install-pear-nozlib.phar; \
		fi \
	fi
	@if test -f $(builddir)/install-pear-nozlib.phar && $(mkinstalldirs) $(INSTALL_ROOT)$(peardir); then \
		$(MAKE) -s install-pear-installer; \
	else \
		cat $(srcdir)/install-pear.txt; \
	fi

