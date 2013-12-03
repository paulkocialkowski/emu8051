# Regression tests for emu8051

AS504 = as504
AS51 = asem

SUFFIXES = .hex .asm .sh

# Do not remove intermediate .hex files
.SECONDARY:

ASM_SRC = \
    mul1.asm mul2.asm \
    div.asm \
    orl.asm anl.asm \
    mov.asm \
    timer0.asm timer1.asm timer2.asm

if RUN_TESTS

TESTS = \
    mul1.sh mul2.sh \
    div.sh \
    orl.sh anl.sh \
    mov.sh \
    timer0.sh timer1.sh timer2.sh

# Tell make how to generate a .sh file after a .hex file is generated:
.hex.sh:
	@ln -sf $(srcdir)/opcodes.sh $@

# Tell make how to generate a .hex file from a .asm source file.
# Warning: make distcheck will fail because asem nor as504 have option to
# specify output filename (and path), except patched version of as504:
.asm.hex:
if USE_AS504HV
	$(AS504) -Fhex -O$@ $<
else
if USE_AS504
	$(AS504) -Fhex $<
else
if USE_AS51
	$(AS51) $<
else
	echo error
endif
endif
endif

else
TESTS =
endif

EXTRA_DIST = opcodes.sh $(ASM_SRC)

CLEANFILES = *~ *.lst *.hex $(TESTS)

MAINTAINERCLEANFILES = Makefile.in
