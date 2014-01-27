# This file is processed by GNU automake to generate Makefile.in

AM_CPPFLAGS = \
    $(WARNINGCFLAGS) \
    -I@top_srcdir@ \
    -I@top_srcdir@/src/common

# Option -d: produce header file parser.h
AM_YFLAGS = -d

LDADD = \
    $(top_builddir)/src/common/libemu8051.a

bin_PROGRAMS = emu8051-cli

emu8051_cli_SOURCES = \
    parser.y scanner.l \
    emuconsole.c \
    menu.c menu.h \
    keyboard.c keyboard.h

# We want these in the dist tarball
EXTRA_DIST = scanner.h

CLEANFILES = *~

MAINTAINERCLEANFILES = \
    Makefile.in \
    scanner.h scanner.c \
    parser.h parser.c
