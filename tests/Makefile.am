# Regression tests for emu8051

AS51 = asem

SUFFIXES = .hex .asm

TESTS = \
    opcodes

check_PROGRAMS = mul1.hex mul2.hex

mul1.hex: mul1.asm

mul2.hex: mul2.asm

.asm.hex:
	$(AS51) $<

EXTRA_DIST = $(TESTS)

CLEANFILES = *~ *.lst *.log

MAINTAINERCLEANFILES = Makefile.in
