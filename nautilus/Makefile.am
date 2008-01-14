## Process this file with automake to produce Makefile.in
SUBDIRS = 48x48

gtk_update_icon_cache = gtk-update-icon-cache -f $(themedir)

install-data-hook: update-icon-cache
uninstall-hook: update-icon-cache

update-icon-cache:
	@-if test -z "$(DESTDIR)"; then \
		echo "Updating Gtk icon cache."; \
		$(gtk_update_icon_cache); \
	else \
		echo "*** Icon cache not updated.  After (un)install, run this:"; \
		echo "***   $(gtk_update_icon_cache)"; \
	fi

#glade_DATA = ../nsvn/naughtysvn.glade
gladedir = $(datadir)/$(PACKAGE_NAME)/glade

# This is needed, since the calls we do against have svn/svn-nsvn-wc.c depends on include files from svn and apr
APR_CFLAGS =`$(APRCONFIGTOOL) --includes --cppflags --cflags`
#APR_LIBS =`$(APRCONFIGTOOL) --libs`

AM_CFLAGS = \
		-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
		-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
		-DGLADEDIR=\""$(gladedir)"\" \
		-DPIXDIR=\""$(datadir)/$(PACKAGE_NAME)/pixmaps"\" \
		$(NAUTILUS_CFLAGS) \
		$(glib_CFLAGS) \
		$(gtk_CFLAGS) \
		$(gnome_CFLAGS) \
		$(libglade_CFLAGS) \
		-I$(top_srcdir) \
		-I$(top_builddir) \
		$(SVN_CFLAGS) \
		$(APR_CFLAGS) \
		-Wall -g

nautilus_extension_libdir=`$(PKG_CONFIG) libnautilus-extension --variable=libdir`
nautilus_extensiondir=$(nautilus_extension_libdir)/nautilus/extensions-1.0

nautilus_extension_LTLIBRARIES=libnautilus-naughtysvn.la

libnautilus_naughtysvn_la_SOURCES = \
		nautilus-naughtysvn.c \
		nautilus-naughtysvn.h \
		naughtysvn-module.c

libnautilus_naughtysvn_la_LDFLAGS = -module -avoid-version -no-undefined \
			--rpath '$(libdir)' \
			$(NAUTILUS_LIBS) \
			-L$(top_builddir)/svn \
			-lnaughtysvn

#EXTRA_DIST = \
#		$(glade_DATA)
