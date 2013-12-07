# This file is processed by GNU automake to generate Makefile.in

AM_CPPFLAGS = \
    -Wall \
    -I@top_srcdir@ \
    -I@top_srcdir@/src/common \
    -I$(top_srcdir)/pixmaps \
    @GTK_CFLAGS@ \
    @GLIB_CFLAGS@ \
    -DDATADIR=\"$(datadir)\" \
    -DGDK_PIXBUF_DISABLE_DEPRECATED \
    -DGDK_DISABLE_DEPRECATED \
    -DGTK_DISABLE_DEPRECATED

LDADD = \
    $(top_builddir)/src/common/libemu8051.a \
    @GTK_LIBS@ \
    @GLIB_LIBS@

bin_PROGRAMS = emu8051-gtk

emu8051_gtk_SOURCES = \
    emugtk.c emugtk.h \
    app-config.c app-config.h \
    memwin.c memwin.h \
    pgmwin.c pgmwin.h \
    regwin.c regwin.h \
    pswwin.c pswwin.h \
    filemenu.c filemenu.h \
    viewmenu.c viewmenu.h \
    helpmenu.c helpmenu.h \
    messagebox.c messagebox.h

EXTRA_DIST =

CLEANFILES = *~

MAINTAINERCLEANFILES = Makefile.in
