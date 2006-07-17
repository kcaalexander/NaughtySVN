## Process this file with automake to produce Makefile.in

## FIXME: All subversion related paths are hard coded, we need to change
##        this to variables

#SVN_CFLAGS=`../svn-config --cflags`
#SVN_LIBS=`../svn-config --libs`
APR_CFLAGS =`$(APRCONFIGTOOL) --includes --cflags`
APR_LIBS =`$(APRCONFIGTOOL) --libs`
INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
        $(PACKAGE_CFLAGS) \
        -I$(top_srcdir) \
        -I$(top_builddir) \
        -g -g -Wall -Wmissing-prototypes -Wstrict-prototypes \
        -Wmissing-declarations -pthread -DNEON_ZLIB -DNEON_SSL \
        -DSVN_DEBUG -DAP_DEBUG -Wpointer-arith -Wwrite-strings -Wshadow \
        $(SVN_CFLAGS) \
        $(APR_CFLAGS)

naughtysvn_includedir=$(includedir)

naughtysvn_include_HEADERS=naughtysvn.h

naughtysvndir = $(libdir)

naughtysvn_LTLIBRARIES = libnaughtysvn.la

libnaughtysvn_la_SOURCES = \
        svn-nsvn-types.h \
        svn-nsvn-admin.c \
        svn-nsvn-init.c \
        svn-nsvn-wc.c \
        svn-nsvn-repos.c \
        svn-nsvn-common.c

libnaughtysvn_la_LDFLAGS = -no-undefined -avoid-version  $(APR_LIBS)
libnaughtysvn_la_LIBADD = -lexpat \
                          -lsvn_client-1 -lsvn_repos-1 \
                          -lsvn_subr-1 -lsvn_fs-1

#pkgconfigdir=$(libdir)/pkgconfig
#pkgconfig_DATA=libnaughtysvn.pc

#EXTRA_DIST = \
#        libnaughtysvn.pc.in

