AC_INIT(matchbox-config-gtk, 0.1, http://www.o-hand.com/)
AM_INIT_AUTOMAKE([1.6])
AM_CONFIG_HEADER(config.h)
AC_CONFIG_SRCDIR(appearance/mb-appearance.c)

AC_ISC_POSIX
AC_PROG_CC
AC_STDC_HEADERS

PKG_CHECK_MODULES(GTK, gtk+-2.0)
PKG_CHECK_MODULES(GCONF, gconf-2.0)

CFLAGS="$CFLAGS -Wall"

AC_OUTPUT([
Makefile
appearance/Makefile
])
