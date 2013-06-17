# General compiler flags
AM_CFLAGS = -Wall -g $(DEPS_CFLAGS) $(MBAPP_CFLAGS)

# A convenience static library that contains the common GTK+ and X code.
noinst_LTLIBRARIES = libshot.la
libshot_la_SOURCES = \
	screenshot-utils.c screenshot-utils.h \
	screenshot-ui.c screenshot-ui.h

if WITH_MB_APPLET
# A matchbox-panel-2 applet
appletdir = $(libdir)/matchbox-panel
applet_LTLIBRARIES = libscreenshot.la
libscreenshot_la_SOURCES = applet.c
libscreenshot_la_LDFLAGS = -avoid-version
libscreenshot_la_LIBADD = $(DEPS_LIBS) $(MBAPP_LIBS) libshot.la

# The icon for the panel applet.
icondir = $(datadir)/pixmaps
icon_DATA = applet-screenshot.png
endif

# A standalone tool for running from a terminal and scripts
bin_PROGRAMS = screenshot
screenshot_SOURCES = main.c
screenshot_LDADD = $(DEPS_LIBS) libshot.la
