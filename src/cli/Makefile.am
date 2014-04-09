# This file is processed by GNU automake to generate Makefile.in

AM_CPPFLAGS = \
    $(WARNINGCFLAGS) \
    -I$(top_srcdir)/src/common \
    -I$(top_builddir)/src/common

# Option -d: produce header file scanner.h
AM_LFLAGS = --header-file=scanner.h

# Option -d: produce header file parser.h
AM_YFLAGS = -d

LDADD = \
    $(LIBREADLINE) \
    $(top_builddir)/src/common/libemu8051.a

bin_PROGRAMS = emu8051-cli

emu8051_cli_SOURCES = \
    parser.y scanner.l \
    main.c \
    menu.c menu.h \
    keyboard.c keyboard.h

# We want these in the dist tarball
EXTRA_DIST = scanner.h

CLEANFILES = *~

MAINTAINERCLEANFILES = \
    Makefile.in \
    scanner.h scanner.c \
    parser.h parser.c
