# This file is processed by GNU automake to generate Makefile.in

AM_CPPFLAGS = \
	-Wall \
	-I$(top_srcdir)

noinst_LIBRARIES = libemu8051.a

libemu8051_a_SOURCES = \
	instructions_8051.c \
	options.c options.h \
	log.c log.h \
	hexfile.c hexfile.h \
	cpu8051.c cpu8051.h \
	memory.c memory.h \
	psw.c psw.h \
	sfr.c sfr.h \
	timers.c timers.h \
	common.h \
	reg8051.h

# These files are generated automatically by a perl script.
instructions_8051.c instructions_8051.h disasm.h : opcode2c.pl opcodes.lst
	./opcode2c.pl

CLEANFILES = *~

MAINTAINERCLEANFILES = \
	Makefile.in \
	instructions_8051.c \
	instructions_8051.h \
	disasm.h

EXTRA_DIST = \
	opcode2c.pl \
	opcodes.lst \
	instructions_8051.h \
	disasm.h
