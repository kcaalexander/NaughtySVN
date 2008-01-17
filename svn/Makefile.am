## Process this file with automake to produce Makefile.in

## FIXME: All subversion related paths are hard coded, we need to change
##        this to variables

#SVN_CFLAGS=`../svn-config --cflags`
#SVN_LIBS=`../svn-config --libs`

gladedir = $(datadir)/$(PACKAGE_NAME)/glade

lib_LTLIBRARIES = libnaughtysvn.la

APR_CFLAGS =`$(APRCONFIGTOOL) --includes --cppflags --cflags`
APR_LIBS =`$(APRCONFIGTOOL) --libs`

AM_CFLAGS = \
		-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
		-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
		-DGLADEDIR=\""$(gladedir)"\" \
		$(glib_CFLAGS) \
		$(gtk_CFLAGS) \
		$(gnome_CFLAGS) \
		$(libglade_CFLAGS) \
		-I$(top_srcdir) \
		-I$(top_builddir) \
		$(SVN_CFLAGS) \
		$(APR_CFLAGS)

libnaughtysvn_includedir=$(includedir)

libnaughtysvn_include_HEADERS=naughtysvn.h

libnaughtysvn_libdir = $(libdir)


libnaughtysvn_la_SOURCES = \
		svn-nsvn-types.h \
		svn-nsvn-admin.c \
		svn-nsvn-init.c \
		svn-nsvn-auth.c \
		dlg_auth.c dlg_auth.h \
		svn-nsvn-wc.c \
		svn-nsvn-repos.c \
		svn-nsvn-common.c

libnaughtysvn_la_LDFLAGS = -no-undefined -avoid-version $(APR_LIBS) $(SVN_LIBS)
libnaughtysvn_la_LIBADD = -lexpat \
		-lsvn_client-1 -lsvn_repos-1 \
		-lsvn_subr-1 -lsvn_fs-1

DISTCLEANFILES = \
		libnaughtysvn.pc

EXTRA_DIST = \
		libnaughtysvn.pc.in

pkgconfigdir=$(libdir)/pkgconfig
pkgconfig_DATA=libnaughtysvn.pc

