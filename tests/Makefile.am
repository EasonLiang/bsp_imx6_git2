TESTS = dot-desktop pixbuf

check_PROGRAMS=dot-desktop pixbuf

AM_CPPFLAGS = @CHECK_CFLAGS@ -I../libmb @GCC_WARNINGS@ @XLIBS_CFLAGS@ @PANGO_CFLAGS@ @PNG_CFLAGS@
AM_LDFLAGS = -L../libmb -lmb

dot_desktop_SOURCES=dot-desktop.c
dot_desktop_LDADD= @CHECK_LIBS@

pixbuf_SOURCES=pixbuf.c oh.h
pixbuf_LDADD= @CHECK_LIBS@

noinst_PROGRAMS = dump-image
dump_image_SOURCES=dump-image.c

EXTRA_DIST = oh-overlayed.png oh.png oh-scaled.png overlay.png
	     oh.jpg oh.xpm