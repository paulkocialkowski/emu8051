# This file is processed by GNU automake to generate Makefile.in

AM_CPPFLAGS = \
    $(WARNINGCFLAGS) \
    -I$(builddir)

if DEVICE
AM_CPPFLAGS += $(EMU8051_DEVICE_CFLAGS)
endif

VPATH = $(srcdir) $(builddir)

noinst_LIBRARIES = libemu8051.a

BUILT_SOURCES = \
    instructions_8051.c instructions_8051.h \
    opcodes.h opcodes.c

PERL_SRC = \
    opcodes2c.pl \
    opcodes.lst

libemu8051_a_SOURCES = \
    options.c options.h \
    log.c log.h \
    serial.c serial.h \
    iotrace.c iotrace.h \
    exectrace.c exectrace.h \
    hardware.c hardware.h \
    hexfile.c hexfile.h \
    cpu8051.c cpu8051.h \
    memory.c memory.h \
    interrupt.c interrupt.h \
    psw.c psw.h \
    sfr.c sfr.h \
    operations.c operations.h \
    gp_timers.c gp_timers.h \
    timers.c timers.h \
    common.h \
    reg8051.h

if DEVICE
libemu8051_a_SOURCES += device.c device.h
endif

if KB9012
libemu8051_a_SOURCES += kb9012.c kb9012.h
endif

nodist_libemu8051_a_SOURCES = \
    $(BUILT_SOURCES)

# These files are generated automatically by a perl script.
$(BUILT_SOURCES) : $(PERL_SRC)
	@echo "  PERL     opcodes2c.pl"
	@$(srcdir)/opcodes2c.pl $(srcdir)/opcodes.lst $(builddir)

CLEANFILES = \
    *~ \
    $(BUILT_SOURCES)

MAINTAINERCLEANFILES = \
    Makefile.in

EXTRA_DIST = \
    $(PERL_SRC)
