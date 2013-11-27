# Regression tests for emu8051

AS51 = asem

SUFFIXES = .hex .asm

TESTS = \
    opcodes

check_PROGRAMS = mul1.hex mul2.hex div.hex orl.hex anl.hex mov.hex timer0.hex timer1.hex

mul1.hex: mul1.asm
mul2.hex: mul2.asm
div.hex: div.asm
orl.hex: orl.asm
anl.hex: anl.asm
mov.hex: mov.asm
timer0.hex: timer0.asm
timer1.hex: timer1.asm

.asm.hex:
	$(AS51) $<

EXTRA_DIST = $(TESTS)

CLEANFILES = *~ *.lst *.log

MAINTAINERCLEANFILES = Makefile.in
