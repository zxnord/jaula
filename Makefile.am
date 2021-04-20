
# # svn info
# # $Author: morongo $
# # $Date: 2009-01-11 16:17:03 +0000 (Sun, 11 Jan 2009) $
# # $HeadURL: file:///svn/p/jaula/code/trunk/Makefile.am $
# # $Id: Makefile.am 45 2009-01-11 16:17:03Z morongo $
# # $Revision: 45 $

# # First level directories
SUBDIRS = @SUBDIRS@

# # base data directory
datarootdir = @datarootdir@

# # Suggested by libtool
ACLOCAL_AMFLAGS=-I m4

EXTRA_DIST = TODO.lst

# # Automatic recompilation of dependencies
LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck

# # Target to generate project documentation
docs:
	make -C doc

# # EOF $Id: Makefile.am 45 2009-01-11 16:17:03Z morongo $

