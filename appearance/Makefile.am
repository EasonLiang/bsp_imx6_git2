bin_PROGRAMS = matchbox-appearance

matchbox_appearance_SOURCES = mb-appearance.c
matchbox_appearance_CPPFLAGS = \
	$(GCONF_CFLAGS) $(GTK_CFLAGS) \
	-DDATADIR="\"$(datadir)\"" -DPKGDATADIR="\"$(pkgdatadir)\""
matchbox_appearance_LDADD = $(GCONF_LIBS) $(GTK_LIBS)

applicationdir = $(datadir)/applications
dist_application_DATA = mb-appearance.desktop
