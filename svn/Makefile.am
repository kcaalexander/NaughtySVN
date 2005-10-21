## Process this file with automake to produce Makefile.in

## FIXME: All subversion related paths are hard coded, we need to change
##        this to variables

# SVN_CFLAGS="`../svn-config --cflags`"
# SVN_LIBS="`../svn-config --libs`"

INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
        $(PACKAGE_CFLAGS) \
        -I$(top_srcdir) \
        -I$(top_builddir) \
        -g -g -Wall -Wmissing-prototypes -Wstrict-prototypes \
        -Wmissing-declarations -pthread -DNEON_ZLIB -DNEON_SSL \
        -DSVN_DEBUG -DAP_DEBUG -Wpointer-arith -Wwrite-strings -Wshadow \
        -I/usr/local/include/subversion-1 \
        -I/usr/include/neon \
        -I/usr/local/apache2/include \
        -I/usr/local/BerkeleyDB.4.3/include

svn_naughtysvn_includedir=$(includedir)

svn_naughtysvn_include_HEADERS=svn_naughtysvn.h

svn_naughtysvndir = $(libdir)

svn_naughtysvn_LTLIBRARIES = libsvn-naughtysvn.la

libsvn_naughtysvn_la_SOURCES = \
        svn-nsvn-private.h \
        svn-nsvn-admin.c \
        svn-nsvn-init.c \
        svn-nsvn-wc.c \
        svn-nsvn-repos.c

libsvn_naughtysvn_la_LDFLAGS = -module -avoid-version
libsvn_naughtysvn_la_LIBADD =  /usr/lib/libneon.la \
                               -L/usr/local/lib \
                               -L/usr/local/apache2/lib \
                               -laprutil-0 -lgdbm -ldb-4.3 \
                               -lexpat -lapr-0 -lrt -lm \
                               -lcrypt -lnsl  -lpthread \
                               -ldl -lsvn_client-1 -lsvn_repos-1 \
                               -lsvn_subr-1 -lsvn_fs-1

#pkgconfigdir=$(libdir)/pkgconfig
#pkgconfig_DATA=libsvn_naughtysvn.pc

#EXTRA_DIST = \
#        libsvn_naughtysvn.pc.in

