## Process this file with automake to produce Makefile.in

SUBDIRS = svn nautilus nsvn po

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = \
	autogen.sh

install-data-local:
	@$(NORMAL_INSTALL)
	if test -d $(srcdir)/pixmaps; then \
	  $(mkinstalldirs) $(DESTDIR)$(datadir)/pixmaps/$(PACKAGE); \
	  for pixmap in $(srcdir)/pixmaps/*; do \
	    if test -f $$pixmap; then \
	      $(INSTALL_DATA) $$pixmap $(DESTDIR)$(datadir)/pixmaps/$(PACKAGE); \
	    fi \
	  done \
	fi

dist-hook:
	if test -d pixmaps; then \
	  mkdir $(distdir)/pixmaps; \
	  for pixmap in pixmaps/*; do \
	    if test -f $$pixmap; then \
	      cp -p $$pixmap $(distdir)/pixmaps; \
	    fi \
	  done \
	fi

#spec = naughtysvn.spec

#EXTRA_DIST = $(spec) $(spec).in autogen.sh naughtysvn.pc.in

#all-local: $(spec)

#pkgconfigdir = $(libdir)/pkgconfig
#pkgconfig_DATA = naughtysvn.pc

#rpm: $(distdir).tar.gz
#        rpm -ta $(distdir).tar.gz

