AC_PREREQ(2.53)
AC_INIT(screenshot, 0.1, http://www.openedhand.com/)
AM_INIT_AUTOMAKE()
AC_CONFIG_SRCDIR(screenshot-utils.c)
AM_CONFIG_HEADER(config.h)

AM_DISABLE_STATIC
AC_ISC_POSIX
AC_PROG_CC
AC_STDC_HEADERS
AC_PROG_LIBTOOL

PKG_CHECK_MODULES(GTK, gtk+-2.0 matchbox-panel)

dnl appletdir=$(pkg-config --variable=appletdir matchbox-panel)

APPLETDIR=$()
AC_OUTPUT([
Makefile
])
