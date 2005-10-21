## Process this file with automake to produce Makefile.in

glade_DATA = naughtysvn.glade
gladedir = $(datadir)/$(PACKAGE_NAME)/glade

INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
        -DGLADEDIR=\""$(gladedir)"\" \
	@PACKAGE_CFLAGS@ \
       -Wall -g \
       -I$(top_builddir)
#        -DGLADEDIR=\""."\"

bin_PROGRAMS = naughtysvn

naughtysvn_SOURCES = \
	main.c \
        window_list.h \
        global.c global.h \
	dlg_reposcreate.c dlg_reposcreate.h \
        dlg_aboutnsvn.c dlg_aboutnsvn.h \
        dlg_preferences.c dlg_preferences.h

naughtysvn_LDADD = @PACKAGE_LIBS@ \
                   $(INTLLIBS) \
                   -L$(top_builddir)/svn \
                   -lsvn-naughtysvn

kill_existing_nautilus=`/usr/bin/pkill -9 nautilus`

EXTRA_DIST =    \
        $(glade_DATA)

