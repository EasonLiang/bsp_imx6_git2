INCLUDES = -DPKGDATADIR=\"$(pkgdatadir)\" @APP_CFLAGS@
bin_PROGRAMS = settings-daemon

settings_daemon_SOURCES =  settings-daemon.c                      \
                           xsettings-common.c xsettings-manager.c \
		           xsettings-common.h xsettings-manager.h

settings_daemon_LDADD = @APP_LIBS@ 
