INCLUDES = -DPKGDATADIR=\"$(pkgdatadir)\" @APP_CFLAGS@
bin_PROGRAMS = settings-daemon

settings_daemon_PROGRAMS = settings-daemon.c                      \
                           xsettings-common.c xsettings-manager.c \
		           xsettings-common.c xsettings-manager.c

settings_daemon_LDADD = @APP_LIBS@ 
