bin_PROGRAMS = mb-appearance

mb_appearance_SOURCES = mb-appearance.c
mb_appearance_CPPFLAGS = \
	$(GCONF_CFLAGS) $(GTK_CFLAGS) $(GLADE_CFLAGS) \
	-DDATADIR="\"$(datadir)\"" -DPKGDATADIR="\"$(pkgdatadir)\""
mb_appearance_LDADD = $(GCONF_LIBS) $(GTK_LIBS) $(GLADE_LIBS)

pixmapdir = $(datadir)/pixmaps
pixmap_DATA = mb-appearance.png

gladedir = $(pkgdatadir)
glade_DATA = appearance.glade

EXTRA_DIST = $(pixmap_DATA) $(glade_DATA)
