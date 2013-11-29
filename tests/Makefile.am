# Regression tests for emu8051

AS51 = asem

SUFFIXES = .hex .asm .sh

# Should find a way to automatically generate each symlink here:
TESTS = \
    mul1.sh mul2.sh \
    div.sh \
    orl.sh anl.sh \
    mov.sh \
    timer0.sh timer1.sh timer2.sh

mul1.sh: mul1.hex
mul2.sh: mul2.hex
div.sh: div.hex
orl.sh: orl.hex
anl.sh: anl.hex
mov.sh: mov.hex
timer0.sh: timer0.hex
timer1.sh: timer1.hex
timer2.sh: timer2.hex

# Tell make how to generate a .sh file after a .hex file is generated:
.hex.sh:
	ln -sf opcodes.sh $@

# Tell make how to generate a .hex file from a .asm source file:
.asm.hex:
	$(AS51) $<

EXTRA_DIST = $(TESTS) *.asm *.hex

CLEANFILES = *~ *.lst *.hex $(TESTS)

MAINTAINERCLEANFILES = Makefile.in
