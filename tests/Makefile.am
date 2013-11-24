# tests for emu8051

AS51 = asem

SUFFIXES = .hex .asm

bin_PROGRAMS = mul1.hex mul2.hex

mul1.hex: mul1.asm

mul2.hex: mul2.asm

.asm.hex:
	$(AS51) $<

CLEANFILES = *~ *.lst

MAINTAINERCLEANFILES = Makefile.in
