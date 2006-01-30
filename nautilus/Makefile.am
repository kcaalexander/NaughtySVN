## Process this file with automake to produce Makefile.in

INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
        $(NAUTILUS_CFLAGS) \
        $(PACKAGE_CFLAGS) \
        -I$(top_srcdir) \
        -I$(top_builddir) \
        -Wall -g

nautilus_extension_libdir=`$(PKG_CONFIG) libnautilus-extension --variable=libdir`
nautilus_extensiondir=$(nautilus_extension_libdir)/nautilus/extensions-1.0

nautilus_extension_LTLIBRARIES=libnautilus-naughtysvn.la

libnautilus_naughtysvn_la_SOURCES =     \
        nautilus-naughtysvn.c   \
        nautilus-naughtysvn.h   \
        naughtysvn-module.c

libnautilus_naughtysvn_la_LDFLAGS = -module -avoid-version
libnautilus_naughtysvn_la_LIBADD  = $(NAUTILUS_LIBS) \
                                    -L$(top_builddir)/svn \
                                    -lsvn-naughtysvn