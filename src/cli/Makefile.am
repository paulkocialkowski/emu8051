# This file is processed by GNU automake to generate Makefile.in

AM_CPPFLAGS = \
    $(WARNINGCFLAGS) \
    -I@top_srcdir@ \
    -I@top_srcdir@/src/common

LDADD = \
    $(top_builddir)/src/common/libemu8051.a

bin_PROGRAMS = emu8051-cli

emu8051_cli_SOURCES = \
    emuconsole.c \
    keyboard.c keyboard.h

# we want these in the dist tarball
EXTRA_DIST =

CLEANFILES = *~

MAINTAINERCLEANFILES = Makefile.in
