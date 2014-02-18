# Regression tests for emu8051

AS504 = as504
AS51 = asem

SUFFIXES = .hex .asm .sh

# Do not remove intermediate .hex files
.SECONDARY:

ASM_SRC = \
    add.asm \
    mul1.asm mul2.asm \
    div.asm div32u.asm \
    orl.asm anl.asm \
    mov.asm \
    sqroot.asm \
    timer0.asm timer1.asm timer2.asm

if RUN_TESTS

TESTS_ASM = \
    add.sh \
    mul1.sh mul2.sh \
    div.sh div32u.sh \
    orl.sh anl.sh \
    mov.sh \
    sqroot.sh \
    timer0.sh timer1.sh timer2.sh

TESTS_CMD = \
    cmd_dr.sh \
    cmd_de.sh \
    cmd_di.sh \
    cmd_dp.sh \
    cmd_we.sh \
    cmd_wi.sh \
    cmd_wp.sh \
    cmd_wr.sh

TESTS = \
    $(TESTS_CMD) \
    $(TESTS_ASM)

# If you do not want an implicit rule to be used for a target that has no
# commands, you can give that target empty commands by writing a semicolon.
cli.sh: ;

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

EXTRA_DIST = $(TESTS_CMD) cmd.sh opcodes.sh $(ASM_SRC)

CLEANFILES = *~ *.lst *.hex $(TESTS_ASM)

MAINTAINERCLEANFILES = Makefile.in
