TESTS = dot-desktop pixbuf
check_PROGRAMS=dot-desktop pixbuf

AM_CPPFLAGS = @CHECK_CFLAGS@ -I../libmb @XLIBS_CFLAGS@
AM_LDFLAGS = -L../libmb -lmb

dot_desktop_SOURCES=dot-desktop.c
dot_desktop_LDADD= @CHECK_LIBS@

pixbuf_SOURCES=pixbuf.c
pixbuf_LDADD= @CHECK_LIBS@

noinst_PROGRAMS = dump-image
dump_image_SOURCES=dump-image.c
