## Process this file with automake to produce Makefile.in
pixmapsdir = $(datadir)/$(PACKAGE_NAME)/pixmaps
pixmaps_DATA = \
		add.png \
		checkout.png \
		commit.png \
		create_repos.png \
		preferences-general.png \
		update.png \
		user_icon.png

uninstall-local:
	rm -rf $(pixmapsdir)

EXTRA_DIST = \
		$(pixmaps_DATA)
