## Process this file with automake to produce Makefile.in

glade_DATA = naughtysvn.glade
gladedir = $(datadir)/$(PACKAGE_NAME)/glade


INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
        -DGLADEDIR=\""$(gladedir)"\" \
        -DPIXDIR=\""$(datadir)/$(PACKAGE_NAME)/pixmaps"\" \
	@PACKAGE_CFLAGS@ \
       -Wall -g \
       -I$(top_builddir)
#        -DGLADEDIR=\""."\"

bin_PROGRAMS = naughtysvn

naughtysvn_SOURCES = \
        main.c \
        window_list.h \
        global.c global.h \
        gconf.c gconf.h \
        dlg_reposcreate.c dlg_reposcreate.h \
        dlg_aboutnsvn.c dlg_aboutnsvn.h \
        dlg_preferences.c dlg_preferences.h \
        dlg_checkout.c dlg_checkout.h \
        dlg_commit.c dlg_commit.h \
        dlg_add.c dlg_add.h

naughtysvn_LDADD = @PACKAGE_LIBS@ \
                   $(INTLLIBS) \
                   -L$(top_builddir)/svn \
                   -lnaughtysvn

schemadir = @GCONF_SCHEMA_FILE_DIR@
schema_DATA = naughtysvn.schemas

install-data-local:
	GCONF_CONFIG_SOURCE=$(GCONF_SCHEMA_CONFIG_SOURCE) $(GCONFTOOL) --makefile-install-rule $(srcdir)/$(schema_DATA)
	pkill -9 gconfd
uninstall-local:
	GCONF_CONFIG_SOURCE=$(GCONF_SCHEMA_CONFIG_SOURCE) $(GCONFTOOL) --makefile-uninstall-rule $(srcdir)/$(schema_DATA)
	pkill -9 gconfd

EXTRA_DIST =    \
        $(glade_DATA) \
        $(schema_DATA)
