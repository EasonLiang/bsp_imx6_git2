#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.53)
AC_INIT([settings-daemon], 0.0.1, [mallum@o-hand.com])
AC_CONFIG_SRCDIR([settings-daemon.c])

AM_INIT_AUTOMAKE()
AM_CONFIG_HEADER(config.h)

# Checks for programs.
AC_GNU_SOURCE
AC_PROG_CC


dnl TODO: make gconf optional
PKG_CHECK_MODULES(APP, [gconf-2.0 gdk-x11-2.0])


AC_SUBST(APP_CFLAGS)
AC_SUBST(APP_LIBS)


# Find how and where to put the GConf schemas

AC_PATH_PROG(GCONFTOOL, gconftool-2, no)
if test x"$GCONFTOOL" = xno; then
	AC_MSG_WARN([gconftool-2 executable not found in your path])
fi
AM_GCONF_SOURCE_2

AC_MSG_CHECKING([whether to use xrdb to set Xft settings])
AC_ARG_WITH([xrdb],
        [AS_HELP_STRING([--with-xrdb],
        [use xrdb [default=no]])],
        [],[with_xrdb=no])
AS_IF([test "$with_xrdb" = "yes"], [AC_DEFINE(WITH_XRDB, 1, [Use xrdb])])
AC_MSG_RESULT([$with_xrdb])

# Checks for header files.

AC_HEADER_STDC

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_PID_T


 
AC_OUTPUT([Makefile])
