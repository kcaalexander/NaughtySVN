## Process this file with automake to produce Makefile.in

SUBDIRS = svn nautilus nsvn po

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = \
	autogen.sh
        
DISTCLEANFILES = \
		intltool-extract \
		intltool-merge \
		intltool-update

dist-hook:
	if test -d $(srcdir)/pixmaps; then \
	  mkdir $(datadir)/$(PACKAGE_NAME)/pixmaps; \
	  for pixmap in pixmaps/*; do \
	    if test -f $$pixmap; then \
	      cp -p $$pixmap $(datadir)/$(PACKAGE_NAME)/pixmaps; \
	    fi \
	  done \
	fi

uninstall-local:
	if test -d $(DESTDIR)$(datadir)/$(PACKAGE); then \
	  rm -rf "$(DESTDIR)$(datadir)/$(PACKAGE)"; \
	fi
	if test -d $(datadir)/$(PACKAGE_NAME); then \
	  rm -rf "$(datadir)/$(PACKAGE_NAME)"; \
	fi
	pkill -9 nautilus

install-data-local:
	if test -d $(srcdir)/pixmaps; then \
	  mkdir $(datadir)/$(PACKAGE_NAME)/pixmaps; \
	  for pixmap in pixmaps/*; do \
	    if test -f $$pixmap; then \
	      cp -p $$pixmap $(datadir)/$(PACKAGE_NAME)/pixmaps; \
	    fi \
	  done \
	fi

install-exec-local:
	pkill -9 nautilus
#spec = naughtysvn.spec

#EXTRA_DIST = $(spec) $(spec).in autogen.sh naughtysvn.pc.in

#all-local: $(spec)

#pkgconfigdir = $(libdir)/pkgconfig
#pkgconfig_DATA = naughtysvn.pc

#rpm: $(distdir).tar.gz
#        rpm -ta $(distdir).tar.gz

