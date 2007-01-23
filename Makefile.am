# General compiler flags
AM_CFLAGS = -Wall -g $(GTK_CFLAGS)

# A convenience static library that contains the common GTK+ and X code.
noinst_LTLIBRARIES = libshot.la
libshot_la_SOURCES = \
	screenshot-utils.c screenshot-utils.h \
	screenshot-ui.c screenshot-ui.h

# A matchbox-panel-2 applet
appletdir = $(libdir)/matchbox-panel
applet_LTLIBRARIES = libscreenshot.la
libscreenshot_la_SOURCES = applet.c
libscreenshot_la_LDFLAGS = -no-undefined
libscreenshot_la_LIBADD = $(GTK_LIBS) libshot.la

# A standalone tool for running from a terminal and scripts
bin_PROGRAMS = screenshot
screenshot_SOURCES = main.c
screenshot_LDADD = $(GTK_LIBS) libshot.la

icondir = $(datadir)/pixmaps
icon_DATA = applet-screenshot.png
