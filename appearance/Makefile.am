bin_PROGRAMS = matchbox-appearance

matchbox_appearance_SOURCES = mb-appearance.c
matchbox_appearance_CPPFLAGS = \
	$(GCONF_CFLAGS) $(GTK_CFLAGS) $(GLADE_CFLAGS) \
	-DDATADIR="\"$(datadir)\"" -DPKGDATADIR="\"$(pkgdatadir)\""
matchbox_appearance_LDADD = $(GCONF_LIBS) $(GTK_LIBS) $(GLADE_LIBS)

applicationdir = $(datadir)/applications
application_DATA = mb-appearance.desktop

gladedir = $(pkgdatadir)
glade_DATA = appearance.glade

EXTRA_DIST = $(application_DATA) $(pixmap_DATA) $(glade_DATA)
