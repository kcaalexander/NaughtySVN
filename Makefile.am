## Process this file with automake to produce Makefile.in

SUBDIRS = svn nautilus pixmaps nsvn po

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = \
		autogen.sh
        
DISTCLEANFILES = \
		intltool-extract \
		intltool-merge \
		intltool-update

installdev: install
	pkill -9 nautilus

uninstalldev: uninstall
	pkill -9 nautilus

nsvndatadir = $(datadir)/$(PACKAGE_NAME)

uninstall-local:
	rm -rf $(nsvndatadir)

#spec = naughtysvn.spec

#EXTRA_DIST = $(spec) $(spec).in autogen.sh naughtysvn.pc.in

#all-local: $(spec)

#pkgconfigdir = $(libdir)/pkgconfig
#pkgconfig_DATA = naughtysvn.pc

#rpm: $(distdir).tar.gz
#        rpm -ta $(distdir).tar.gz

