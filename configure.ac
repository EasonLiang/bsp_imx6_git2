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

# Checks for header files.

AC_HEADER_STDC

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_PID_T


# Checks for library functions.
 
AC_OUTPUT([Makefile])
