bin_PROGRAMS = settings-daemon

settings_daemon_SOURCES = \
	xsettings-common.c xsettings-common.h \
	xsettings-manager.c xsettings-manager.h \
	settings-daemon.c

settings_daemon_CPPFLAGS = @APP_CFLAGS@ -DHAVE_XFT2
settings_daemon_CFLAGS = -Wall
settings_daemon_LDADD = @APP_LIBS@ 
