bin_PROGRAMS = settings-daemon xsettings-dump

settings_daemon_SOURCES = \
	xsettings-common.c xsettings-common.h \
	xsettings-manager.c xsettings-manager.h \
	settings-daemon.c

settings_daemon_CPPFLAGS = @APP_CFLAGS@ -DHAVE_XFT2
settings_daemon_CFLAGS = -Wall
settings_daemon_LDADD = @APP_LIBS@

xsettings_dump_SOURCES = \
	xsettings-common.c xsettings-common.h \
	xsettings-client.c xsettings-client.h \
	xsettings-dump.c

xsettings_dump_CPPFLAGS = @APP_CFLAGS@
xsettings_dump_CFLAGS = -Wall
xsettings_dump_LDADD = @APP_LIBS@

schemadir = $(GCONF_SCHEMA_FILE_DIR)
schema_DATA = settings-daemon.schemas

install-data-local:
	GCONF_CONFIG_SOURCE=$(GCONF_SCHEMA_CONFIG_SOURCE) \
		$(GCONFTOOL) --makefile-install-rule $(schema_DATA) || \
		(echo ;\
		echo "*****************************************************"; \
		echo "Installation of schemas failed, install them manually"; \
		echo "*****************************************************";)
		@true

EXTRA_DIST = $(schema_DATA)
