# This file is processed by GNU automake to generate Makefile.in

AM_CPPFLAGS = \
    $(WARNINGCFLAGS) \
    -I$(top_srcdir)/pixmaps \
    -I$(top_srcdir)/src/common \
    -I$(top_builddir)/src/common \
    @GTK_CFLAGS@ \
    @GLIB_CFLAGS@ \
    -DDATADIR=\"$(datadir)\" \
    -DGDK_PIXBUF_DISABLE_DEPRECATED \
    -DGDK_DISABLE_DEPRECATED \
    -DGTK_DISABLE_DEPRECATED

LDADD = \
    -lpthread \
    $(top_builddir)/src/common/libemu8051.a \
    @GTK_LIBS@ \
    @ZLIB_LIBS@ \
    @GLIB_LIBS@

if INTERACTIVE
LDADD += -lutil
endif

if DEVICE
LDADD += $(EMU8051_DEVICE_LIBS)
endif

bin_PROGRAMS = emu8051-gtk

emu8051_gtk_SOURCES = \
    main.c main.h \
    app-config.c app-config.h \
    memwin.c memwin.h \
    pgmwin.c pgmwin.h \
    regwin.c regwin.h \
    pswwin.c pswwin.h \
    timerwin.c timerwin.h \
    filemenu.c filemenu.h \
    viewmenu.c viewmenu.h \
    helpmenu.c helpmenu.h \
    messagebox.c messagebox.h

CLEANFILES = *~

MAINTAINERCLEANFILES = Makefile.in
