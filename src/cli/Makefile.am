# This file is processed by GNU automake to generate Makefile.in

AM_CPPFLAGS = \
    $(WARNINGCFLAGS) \
    -I$(top_srcdir)/src/common \
    -I$(top_builddir)/src/common

# Option --header-file: produce header file scanner.h.
# Using abs_builddir as a workaround for automake-1.11
# broken ylwrap script.
AM_LFLAGS = --header-file=$(abs_builddir)/scanner.h

# Option -d: produce header file parser.h
AM_YFLAGS = -d

LDADD = \
    -lpthread \
    $(LIBREADLINE) \
    $(top_builddir)/src/common/libemu8051.a

if INTERACTIVE
LDADD += -lutil
endif

if DEVICE
LDADD += $(EMU8051_DEVICE_LIBS)
endif

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
