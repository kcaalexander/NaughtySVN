## Process this file with automake to produce Makefile.in

gladedir = $(datadir)/$(PACKAGE_NAME)/glade
glade_DATA = naughtysvn.glade

schemadir = @GCONF_SCHEMA_FILE_DIR@
schema_DATA = naughtysvn.schemas

APR_CFLAGS =`$(APRCONFIGTOOL) --includes --cppflags --cflags`
APR_LIBS =`$(APRCONFIGTOOL) --libs`

AM_CFLAGS = \
		-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
		-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
		-DGLADEDIR=\""$(gladedir)"\" \
		-DPIXDIR=\""$(datadir)/$(PACKAGE_NAME)/pixmaps"\" \
		$(glib_CFLAGS) \
		$(gtk_CFLAGS) \
		$(gnome_CFLAGS) \
		$(libglade_CFLAGS) \
		$(gtkspell_CFLAGS) \
		-Wall -g \
		-I$(top_builddir) \
		$(SVN_CFLAGS) \
		$(APR_CFLAGS)
#		-DGLADEDIR=\""."\"

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
		dlg_add.c dlg_add.h \
		dlg_update.c dlg_update.h \
		dlg_log.c dlg_log.h \
		dlg_result.c dlg_result.h

naughtysvn_LDADD = \
		$(glib_LIBS) \
		$(gtk_LIBS) \
		$(gnome_LIBS) \
		$(libglade_LIBS) \
		$(gtkspell_LIBS) \
		$(INTLLIBS) \
		-L$(top_builddir)/svn \
		$(SVN_LIBS) \
		-lnaughtysvn


install-data-local:
	GCONF_CONFIG_SOURCE=$(GCONF_SCHEMA_CONFIG_SOURCE) $(GCONFTOOL) --makefile-install-rule $(srcdir)/$(schema_DATA)
#	$(PKILLTOOL) -9 gconfd
uninstall-local:
	GCONF_CONFIG_SOURCE=$(GCONF_SCHEMA_CONFIG_SOURCE) $(GCONFTOOL) --makefile-uninstall-rule $(srcdir)/$(schema_DATA)
#	$(PKILLTOOL) -9 gconfd
	rm -rf $(gladedir)

EXTRA_DIST = \
		$(glade_DATA) \
		$(schema_DATA)
