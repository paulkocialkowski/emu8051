#!/usr/bin/perl
#
# Copyright (C) 1999 Jonathan St-André
# Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>
#
# This file is released under the GPLv2

# Arg1: opcodes list filename
# Arg2: output directory

if (($#ARGV + 1) < 2) {
    die "Missing arguments.\n";
}

my $opcodes_lst_filename = $ARGV[0];
my $builddir = $ARGV[1];

open OPCODELST, $opcodes_lst_filename or die "Error opening " .
    $opcodes_lst_filename . " : $!\n";

my $file;

$file = $builddir . "/instructions_8051.h";
open INST_DEF, ">" . $file or die "Error creating <" . $file . "> : $!\n";

$file = $builddir . "/instructions_8051.c";
open INST_IMP, ">" . $file or die "Error creating <" . $file . "> : $!\n";

$file = $builddir . "/opcodes.h";
open OPCODES_DEF, ">" . $file or die "Error creating <" . $file . "> : $!\n";

$file = $builddir . "/opcodes.c";
open OPCODES_IMP, ">" . $file or die "Error creating <" . $file . "> : $!\n";

# Write GPL license
# Argument 0 is file descriptor
sub write_header
{
    my $fd = $_[0];

    print $fd " *\n";
    print $fd " * Do not modify this file directly, as it was created by opcodes2c.pl\n";
    print $fd " * Any modifications made directly to this file will be lost.\n";
    print $fd " *\n";
    print $fd " * Copyright (C) 1999 Jonathan St-André\n";
    print $fd " * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>\n";
    print $fd " *\n";
    print $fd " * This file is released under the GPLv2\n";
    print $fd " */\n\n";
}

# Write C function source code line to INST_IMP file.
# Prefix each line with a tab (indentation) and add newline character at the end.
sub cfw
{
    print INST_IMP "\t",$_[0],"\n";
}

# Header for instructions_8051.c
print INST_IMP "/*\n";
print INST_IMP " * instructions_8051.c\n";
write_header(INST_IMP);
print INST_IMP "/* Define only here, for not having extern scope on local variables. */\n";
print INST_IMP "#define INSTRUCTIONS_8051_M 1\n\n\n";
print INST_IMP "#include <stdbool.h>\n\n";
print INST_IMP "#include \"reg8051.h\"\n";
print INST_IMP "#include \"cpu8051.h\"\n";
print INST_IMP "#include \"interrupt.h\"\n";
print INST_IMP "#include \"memory.h\"\n";
print INST_IMP "#include \"psw.h\"\n";
print INST_IMP "#include \"operations.h\"\n";
print INST_IMP "#include \"instructions_8051.h\"\n\n\n";

# Header for opcodes.h
print OPCODES_DEF "/*\n";
print OPCODES_DEF " * opcodes.h\n";
write_header(OPCODES_DEF);
print OPCODES_DEF "#ifndef OPCODES_H\n";
print OPCODES_DEF "#define OPCODES_H 1\n\n";

print OPCODES_DEF "int\n";
print OPCODES_DEF "opcodes_get_instr_size(uint8_t opcode);\n";
print OPCODES_DEF "char *\n";
print OPCODES_DEF "opcodes_get_instr_type_str(uint8_t opcode);\n";
print OPCODES_DEF "int\n";
print OPCODES_DEF "opcodes_get_instr_arg_type_id(unsigned int offset);\n";
print OPCODES_DEF "char *\n";
print OPCODES_DEF "opcodes_get_instr_arg_type_str(unsigned int offset);\n";

# opcodes.c
print OPCODES_IMP "/*\n";
print OPCODES_IMP " * opcodes.c\n";
write_header(OPCODES_IMP);
print OPCODES_IMP "#include <stdint.h>\n\n";

# Column indexes in opcodes.lst table
use constant COL_OPCODE => 0;
use constant COL_INSTR  => 1;
use constant COL_ARGS   => 2;

use constant COL_COUNT_NO_ARGS => 4;

$nbinst=0;
$nbaddr=0;
$nbargs=0;
$instnumb=0;

# Read file describing each instruction/opcode

# Discard first two lines, which are comments
$ligne = <OPCODELST>;
$ligne = <OPCODELST>;

while($ligne=<OPCODELST>) {
    chop $ligne;

    if (length $ligne < 2) {
        next;
    }

    @wordlist = split ' ',$ligne;
    $nbword = @wordlist;
    $instruction = $wordlist[COL_INSTR];

    for ($i = (COL_INSTR + 1); $i < ($nbword - 2); $i++) {
        $instruction = "$instruction $wordlist[$i]";
    }

    $a_instruction[$instnumb] = $instruction;
    $a_bytes[$instnumb] = $wordlist[$nbword - 2];
    $a_cycles[$instnumb] = $wordlist[$nbword - 1];
    $a_opcodehex[$instnumb] = $wordlist[COL_OPCODE];

    $instfunction[$instnumb] = "CPU8051::OP_$wordlist[COL_OPCODE]";

    $instargs[$instnumb << 2] = $instargs[($instnumb << 2) + 1] =
        $instargs[($instnumb << 2) + 2] = $instargs[($instnumb << 2) + 3] = 0;

    if ($nbword > COL_COUNT_NO_ARGS) {
	@argslist = split /\,/,$wordlist[COL_ARGS];
	$argslistsize = @argslist;
	$instargs[$instnumb << 2] = $argslistsize;
	for ($i = 0; $i < $argslistsize; $i++) {
	    if (not exists $argstypes{$argslist[$i]}) {
		$argstypes[$nbargs] = $argslist[$i];
		$argstypes{$argslist[$i]} = $nbargs++;
	    }
	    $instargs[($instnumb << 2) + $i + 1] = $argstypes{$argslist[$i]};
	}
    }

    if (not exists $insttext{$wordlist[COL_INSTR]}) {
	$insttext[$nbinst] = $wordlist[COL_INSTR];
	$insttext{$wordlist[COL_INSTR]} = $nbinst++;
    }

    $insttype[$instnumb] = $insttext{$wordlist[COL_INSTR]};

    if (not exists $addrmode{$wordlist[COL_ARGS]}) {
	$addrmode[$nbaddr] = $wordlist[COL_ARGS];
	$addrmode{$wordlist[COL_ARGS]} = $nbaddr++;
    }

    $nbbytes[$instnumb] = $wordlist[$nbword - 2];

    $instaddr[$instnumb] = $addrmode{$wordlist[COL_ARGS]};

    $instnumb++;
}

# ------------------------------------------------------------------------------
print OPCODES_IMP "/* Size(in bytes) of each instruction (offset in table is instruction opcode) */\n";

$nbelement = @nbbytes;

print OPCODES_IMP "static int instr_size[$nbelement] = {";

for ($i = 0; $i < $nbelement; $i++) {
    if ($i % 16 == 0) {
        print OPCODES_IMP "\n\t";
    } else {
        print OPCODES_IMP " ";
    }
    print OPCODES_IMP "$nbbytes[$i],";
}
print OPCODES_IMP "\n";
print OPCODES_IMP "};\n";
print OPCODES_IMP "\n";

# ------------------------------------------------------------------------------
print OPCODES_IMP "/*\n";
print OPCODES_IMP " * For all 256 opcodes, the value in this table gives the instruction type\n";
print OPCODES_IMP " * ex.: MOV, INC, CLR, CPL,...\n";
print OPCODES_IMP " * To know what is the instruction type, use\n";
print OPCODES_IMP " * the number as an offset in instr_type_str[]\n";
print OPCODES_IMP " */\n";

$nbelement = @insttype;

print OPCODES_IMP "static int instr_type_id[$nbelement] = {";

for ($i = 0; $i < $nbelement; $i++) {
    if ($i % 16 == 0) {
        print OPCODES_IMP "\n\t";
    } else {
        print OPCODES_IMP " ";
    }
    print OPCODES_IMP "$insttype[$i],";
}
print OPCODES_IMP "\n";
print OPCODES_IMP "};\n";
print OPCODES_IMP "\n";

# ------------------------------------------------------------------------------
print OPCODES_IMP "/* List of instructions types referenced by instr_type_id[] */\n";
$nbelement = @insttext;
$elementnb = 0;
print OPCODES_IMP "static char *instr_type_str[$nbelement] = {\n";

foreach $instruc (@insttext) {
    print OPCODES_IMP "\t\"$instruc\"";
    if ($elementnb++ < ($nbelement - 1)) {
        print OPCODES_IMP ",";
    }
    print OPCODES_IMP "\n";
}

print OPCODES_IMP "};\n";
print OPCODES_IMP "\n";

# ------------------------------------------------------------------------------
print OPCODES_IMP "/*\n";
print OPCODES_IMP " * Table describing all arguments types of an instruction\n";
print OPCODES_IMP " * The table is indexed instr_arg_type_id[ opcode * 4]\n";
print OPCODES_IMP " * instr_arg_type_id[opcode*4 + 1] gives number of instruction arguments\n";
print OPCODES_IMP " * instr_arg_type_id[opcode*4 + i] for i=1,2 and 3 gives type of each argument\n";
print OPCODES_IMP " * for most instructions, the 3rd argument isn't used\n";
print OPCODES_IMP " * the argument type is referenced to instr_arg_type_str[]\n";
print OPCODES_IMP " */\n";

$nbelement = @instargs;

print OPCODES_IMP "static int instr_arg_type_id[$nbelement] = {";

for ($i = 0; $i < $nbelement; $i++) {
    if ($i % 16 == 0) {
        print OPCODES_IMP "\n\t";
    } else {
        print OPCODES_IMP " ";
    }

    print OPCODES_IMP "$instargs[$i],";
}
print OPCODES_IMP "\n";
print OPCODES_IMP "};\n";
print OPCODES_IMP "\n";

# ------------------------------------------------------------------------------
print OPCODES_IMP "/*\n";
print OPCODES_IMP " * List all types of arguments available to instructions\n";
print OPCODES_IMP " * Referenced by instr_arg_type_id[]\n";
print OPCODES_IMP " */\n";

$nbelement = @argstypes;
$elementnb = 0;

print OPCODES_IMP "static char *instr_arg_type_str[$nbelement] = {\n";

foreach $args (@argstypes) {
    print OPCODES_IMP "\t\"$args\"";
    if ($elementnb++ < $nbelement-1) {
        print OPCODES_IMP ",";
    }
    print OPCODES_IMP "\n";
}
print OPCODES_IMP "};\n";
print OPCODES_IMP "\n";

print OPCODES_IMP "int\n";
print OPCODES_IMP "opcodes_get_instr_size(uint8_t opcode)\n";
print OPCODES_IMP "{\n";
print OPCODES_IMP "\treturn instr_size[opcode];\n";
print OPCODES_IMP "}\n";
print OPCODES_IMP "\n";

print OPCODES_IMP "char *\n";
print OPCODES_IMP "opcodes_get_instr_type_str(uint8_t opcode)\n";
print OPCODES_IMP "{\n";
print OPCODES_IMP "\treturn instr_type_str[instr_type_id[opcode]];\n";
print OPCODES_IMP "}\n";
print OPCODES_IMP "\n";

print OPCODES_IMP "int\n";
print OPCODES_IMP "opcodes_get_instr_arg_type_id(unsigned int offset)\n";
print OPCODES_IMP "{\n";
print OPCODES_IMP "\treturn instr_arg_type_id[offset];\n";
print OPCODES_IMP "}\n";
print OPCODES_IMP "\n";

print OPCODES_IMP "char *\n";
print OPCODES_IMP "opcodes_get_instr_arg_type_str(unsigned int offset)\n";
print OPCODES_IMP "{\n";
print OPCODES_IMP "\treturn instr_arg_type_str[instr_arg_type_id[offset]];\n";
print OPCODES_IMP "}\n";
print OPCODES_IMP "\n";


# ------------------------------------------------------------------------------
for ($i = 0 ; $i < 256; $i++) {
    print INST_IMP "/*","*"x76,"\n";
    print INST_IMP " * Instruction \"$a_instruction[$i]\" takes $a_cycles[$i] cycle(s) and $a_bytes[$i] byte(s).\n";
    print INST_IMP " ","*"x76,"/\n";
    print INST_IMP "int\n";
    print INST_IMP "cpu8051_OP_$a_opcodehex[$i](void)\n";
    print INST_IMP "{\n";

    if( $i == 0x85 ) {
	# Cas particulier pour MOV direct,direct -> src et dest sont inverses dans la memoire
        cfw("unsigned char srcaddr = mem_read8( PGM_MEM_ID, cpu8051.pc++, true );");
	cfw("unsigned char source = mem_read_direct( srcaddr, true );");
	cfw("unsigned char destaddr = mem_read8( PGM_MEM_ID, cpu8051.pc++, true );");
	cfw("unsigned char destination = mem_read_direct( destaddr, true );");
	cfw("destination = source;");
	cfw("mem_write_direct( destaddr, destination, false );");
    }
    else {
	if ($instargs[$i*4] > 0) {
	    $op_destination=$instargs[$i*4+1];
	    $destination_unused=($insttype[$i] == 26 || $insttype[$i] == 28 || $insttype[$i] == 36 || $insttype[$i] == 38 || $insttype[$i] == 39 || $insttype[$i] == 40 || $insttype[$i] == 44);
	    if ($op_destination == 0) { # addr11
		cfw("unsigned int addr11 = ( ( mem_read8( PGM_MEM_ID, cpu8051.pc - 1, true ) << 3 ) & 0xF00 ) + mem_read8( PGM_MEM_ID, cpu8051.pc, true );");
		cfw("cpu8051.pc++;");
	    }
	    if ($op_destination == 1) { # addr16
		cfw("uint16_t addr16 = pgm_read_addr16(cpu8051.pc, true);");
		cfw("cpu8051.pc += 2;");
	    }
	    if ($op_destination == 2) { # A
		cfw("unsigned char destination = mem_read_direct( _ACC_, true );");
	    }
	    if ($op_destination == 3) { # direct
		cfw("unsigned char destaddr = mem_read8( PGM_MEM_ID, cpu8051.pc++, true );");
		if ($destination_unused) {
		    cfw("unsigned char destination = mem_read_direct( destaddr, true );");
		} else {
		    cfw("unsigned char destination = mem_read_direct( destaddr, false );");
		}
	    }
	    if ($op_destination == 4) { # @R0
		if ($insttype[$i] == 44) { # MOVX
		    if ($destination_unused) {
			cfw("unsigned char destination = mem_read_external ( mem_read_direct( BANKPSW + _R0_, true ) | ( mem_read_direct( _P2_, true ) << 8 ), true );");
		    } else {
			cfw("unsigned char destination = mem_read_external ( mem_read_direct( BANKPSW + _R0_, true ) | ( mem_read_direct( _P2_, true ) << 8 ), false );");
		    }
		} else {
		    if ($destination_unused) {
			cfw("unsigned char destination = mem_read_indirect ( mem_read_direct( BANKPSW + _R0_, true ), true );");
		    } else {
			cfw("unsigned char destination = mem_read_indirect ( mem_read_direct( BANKPSW + _R0_, true ), false );");
		    }
		}
	    }
	    if ($op_destination == 5) { # @R1
		if ($insttype[$i] == 44) { # MOVX
		    if ($destination_unused) {
			cfw("unsigned char destination = mem_read_external ( mem_read_direct( BANKPSW + _R1_, true ) | ( mem_read_direct( _P2_, true ) << 8 ), true );");
		    } else {
			cfw("unsigned char destination = mem_read_external ( mem_read_direct( BANKPSW + _R1_, true ) | ( mem_read_direct( _P2_, true ) << 8 ), false );");
		    }
		} else {
		    if ($destination_unused) {
			cfw("unsigned char destination = mem_read_indirect ( mem_read_direct( BANKPSW + _R1_, true ), true );");
		    } else {
			cfw("unsigned char destination = mem_read_indirect ( mem_read_direct( BANKPSW + _R1_, true ), false );");
		    }
		}
	    }
	    if ($op_destination == 6) { # R0
		cfw("unsigned char destination = mem_read_direct( BANKPSW + _R0_, true );");
	    }
	    if ($op_destination == 7) { # R1
		cfw("unsigned char destination = mem_read_direct( BANKPSW + _R1_, true );");
	    }
	    if ($op_destination == 8) { # R2
		cfw("unsigned char destination = mem_read_direct( BANKPSW + _R2_, true );");
	    }
	    if ($op_destination == 9) { # R3
		cfw("unsigned char destination = mem_read_direct( BANKPSW + _R3_, true );");
	    }
	    if ($op_destination == 10) { # R4
		cfw("unsigned char destination = mem_read_direct( BANKPSW + _R4_, true );");
	    }
	    if ($op_destination == 11) { # R5
		cfw("unsigned char destination = mem_read_direct( BANKPSW + _R5_, true );");
	    }
	    if ($op_destination == 12) { # R6
		cfw("unsigned char destination = mem_read_direct( BANKPSW + _R6_, true );");
	    }
	    if ($op_destination == 13) { # R7
		cfw("unsigned char destination = mem_read_direct( BANKPSW + _R7_, true );");
	    }
	    if ($op_destination == 14) { # bitaddr
		cfw("unsigned char destination, dstbitaddr = mem_read8( PGM_MEM_ID, cpu8051.pc++, true );");
		if ($destination_unused) {
		    cfw("destination = mem_read_bit( dstbitaddr, true );");
		} else {
		    cfw("destination = mem_read_bit( dstbitaddr, false );");
		}
	    }
	    if ($op_destination == 15) { # reladdr
		cfw("cpu8051.pc++;");
		cfw("unsigned int destination = ((signed char) mem_read8(PGM_MEM_ID, cpu8051.pc - 1, true)) + cpu8051.pc;");
	    }
	    if ($op_destination == 16) { # #data
		cfw("unsigned char destination = mem_read8( PGM_MEM_ID, cpu8051.pc++, true );");
	    }
	    if ($op_destination == 17) { # C
		cfw("unsigned char destination = psw_read_cy();");
	    }
	    if ($op_destination == 18) { # @A+DPTR
		cfw("unsigned int destination = mem_read_direct( _ACC_, true ) + mem_sfr_read_dptr();");
	    }
            if ($op_destination == 20) { # AB
                cfw("unsigned char destination = mem_read_direct( _ACC_, true );");
                cfw("unsigned char source = mem_read_direct( _B_, true );");
            }
	    if ($op_destination == 21) { # DPTR
		cfw("unsigned int destination = mem_sfr_read_dptr();");
	    }
	    if ($op_destination == 23) { # /bitaddr
		cfw("unsigned char destination, dstbitaddr = mem_read8( PGM_MEM_ID, (cpu8051.pc)++, true );");
		if ($destination_unused) {
		    cfw("destination = ( mem_read_bit( dstbitaddr ) ^ 1, true );");
		} else {
		    cfw("destination = ( mem_read_bit( dstbitaddr ) ^ 1, false );");
		}
	    }
	    if ($op_destination == 24) { # @DPTR
		if ($destination_unused) {
		    cfw("unsigned char destination = mem_read_external(mem_sfr_read_dptr(), true);");
		} else {
		    cfw("unsigned char destination = mem_read_external(mem_sfr_read_dptr(), false);");
		}
	    }
	}

	if ($instargs[$i*4] > 1) {
	    $op_source=$instargs[$i*4+2];
	    if ($op_source == 0) { # addr11
		cfw("unsigned int addr11 = ( ( mem_read8( PGM_MEM_ID, cpu8051.pc - 1, true ) << 3 ) & 0xF00 ) + mem_read8( PGM_MEM_ID, (cpu8051.pc)++, true );");
	    }
	    if ($op_source == 1) { # addr16
		cfw("uint16_t addr16 = pgm_read_addr16(cpu8051.pc, true);");
		cfw("cpu8051.pc += 2;");
	    }
	    if ($op_source == 2) { # A
		cfw("unsigned char source = mem_read_direct( _ACC_, true );");
	    }
	    if ($op_source == 3) { # direct
		cfw("unsigned char srcaddr = mem_read8( PGM_MEM_ID, (cpu8051.pc)++, true );");
		cfw("unsigned char source = mem_read_direct( srcaddr, false );");
	    }
	    if ($op_source == 4) { # @R0
		if ($insttype[$i] == 44) { # MOVX
		    cfw("unsigned char source = mem_read_external ( mem_read_direct( BANKPSW + _R0_, true ) | ( mem_read_direct( _P2_, true ) << 8 ), false );");
		} else {
		    cfw("unsigned char source = mem_read_indirect ( mem_read_direct( BANKPSW + _R0_, true ), false );");
		}
	    }
	    if ($op_source == 5) { # @R1
		if ($insttype[$i] == 44) { # MOVX
		    cfw("unsigned char source = mem_read_external ( mem_read_direct( BANKPSW + _R1_, true ) | ( mem_read_direct( _P2_, true ) << 8 ), false );");
		} else {
		    cfw("unsigned char source = mem_read_indirect ( mem_read_direct( BANKPSW + _R1_, true ), false );");
		}
	    }
	    if ($op_source == 6) { # R0
		cfw("unsigned char source = mem_read_direct( BANKPSW + _R0_, true );");
	    }
	    if ($op_source == 7) { # R1
		cfw("unsigned char source = mem_read_direct( BANKPSW + _R1_, true );");
	    }
	    if ($op_source == 8) { # R2
		cfw("unsigned char source = mem_read_direct( BANKPSW + _R2_, true );");
	    }
	    if ($op_source == 9) { # R3
		cfw("unsigned char source = mem_read_direct( BANKPSW + _R3_, true );");
	    }
	    if ($op_source == 10) { # R4
		cfw("unsigned char source = mem_read_direct( BANKPSW + _R4_, true );");
	    }
	    if ($op_source == 11) { # R5
		cfw("unsigned char source = mem_read_direct( BANKPSW + _R5_, true );");
	    }
	    if ($op_source == 12) { # R6
		cfw("unsigned char source = mem_read_direct( BANKPSW + _R6_, true );");
	    }
	    if ($op_source == 13) { # R7
		cfw("unsigned char source = mem_read_direct( BANKPSW + _R7_, true );");
	    }

	    if ($op_source == 14) { # bitaddr
		cfw("unsigned char source, srcbitaddr = mem_read8( PGM_MEM_ID, (cpu8051.pc)++, true );");
		cfw("source = mem_read_bit( srcbitaddr, false );");
	    }
	    if ($op_source == 15) { # reladdr
		cfw("(cpu8051.pc)++;");
		cfw("unsigned int source = ((signed char) mem_read8(PGM_MEM_ID, cpu8051.pc - 1, true)) + cpu8051.pc;");
	    }
	    if ($op_source == 16) { # #data
		cfw("unsigned char source = mem_read8( PGM_MEM_ID, (cpu8051.pc)++, true );");
	    }
	    if ($op_source == 17) { # C
		cfw("unsigned char source = psw_read_cy();");
	    }
	    if ($op_source == 18) { # @A+DPTR
		cfw("unsigned char source = mem_read8( PGM_MEM_ID, mem_read_direct( _ACC_, true ) + mem_sfr_read_dptr(), false );");
	    }
	    if ($op_source == 19) { # @A+PC
		cfw("unsigned char source = mem_read8( PGM_MEM_ID, mem_read_direct( _ACC_, true) + ( ++cpu8051.pc ), false );");
	    }
	    if ($op_source == 21) { # DPTR
		cfw("unsigned int source = mem_sfr_read_dptr();");
	    }
	    if ($op_source == 22) { # #data16
		cfw("uint16_t source = pgm_read_addr16(cpu8051.pc, true);");
		cfw("cpu8051.pc += 2;");
	    }
	    if ($op_source == 23) { # /bitaddr
		cfw("unsigned char source, srcbitaddr = mem_read8( PGM_MEM_ID, (cpu8051.pc)++, true );");
		cfw("source = ( mem_read_bit( srcbitaddr, false ) ^ 1 );");
	    }
	    if ($op_source == 24) { # @DPTR
		cfw("unsigned char source = mem_read_external(mem_sfr_read_dptr(), false);");
	    }
	}

##############################################################################
	$modifysrc=0;

	# RR
	if ($insttype[$i] == 3) {
	    cfw("destination = ( destination >> 1 ) | ( destination << 7 );");
	}

	# INC
	if ($insttype[$i] == 4) {
	    cfw("destination++;");
	}

	# JBC
	if ($insttype[$i] == 5) {
	    cfw("if ( destination == 1 ) { cpu8051.pc = source; destination = 0; }");
	}

	# ACALL
	if ($insttype[$i] == 6) {
	    cfw("stack_push16(cpu8051.pc);");
	}

	# LCALL
	if ($insttype[$i] == 7) {
	    cfw("stack_push16(cpu8051.pc);");
	}

	# RRC
	if ($insttype[$i] == 8) {
	    cfw("unsigned char new_cy = destination & 0x01;");
	    cfw("destination = ( destination >> 1 ) | (psw_read_cy() << 7);");
	    cfw("psw_write_cy(new_cy);");
	}

	# DEC
	if ($insttype[$i] == 9) {
	    cfw("destination--;");
	}

	# JB
	if ($insttype[$i] == 10) {
	    cfw("if ( destination == 1 ) { cpu8051.pc = source; }");
	}

	# RET
	if ($insttype[$i] == 11) {
            cfw("cpu8051.pc = stack_pop16();");
	}

	# RL
	if ($insttype[$i] == 12) {
	    cfw("destination = ( destination << 1 ) | ( destination >> 7 );");
	}

	# ADD
	if ($insttype[$i] == 13) {
	    cfw("destination = addition(destination, source, 0);");
	}

	# JNB
	if ($insttype[$i] == 14) {
	    cfw("if ( destination == 0 ) { cpu8051.pc = source; }");
	}

	# RETI
	if ($insttype[$i] == 15) {
	    cfw("cpu8051.interrupt_priority = INTERRUPT_PRIORITY_NONE;");
	    cfw("cpu8051.interrupt_latency = INTERRUPT_LATENCY;");
            cfw("cpu8051.pc = stack_pop16();");
	}

	# RLC
	if ($insttype[$i] == 16) {
	    cfw("unsigned char new_cy = destination & 0x80;");
	    cfw("destination = ( destination << 1 ) | psw_read_cy();");
	    cfw("psw_write_cy(new_cy);");
	}

	# ADDC
        # ADD and ADDC function identically except that ADDC adds the value of
        # operand as well as the value of the Carry flag whereas ADD does not
        # add the Carry flag to the result.
	if ($insttype[$i] == 17) {
	    cfw("unsigned char carryflag = psw_read_cy();");
	    cfw("destination = addition(destination, source, carryflag);");
	}

	# JC
	if ($insttype[$i] == 18) {
	    cfw("if (psw_read_cy()) { cpu8051.pc = destination; }");
	}

	# ORL
	if ($insttype[$i] == 19) {
            cfw("destination |= source;");
	}

	# JNC
	if ($insttype[$i] == 20) {
	    cfw("if (psw_read_cy() == 0) { cpu8051.pc = destination; }");
	}

	# ANL
	if ($insttype[$i] == 21) {
            cfw("destination &= source;");
	}

	# JZ
	if ($insttype[$i] == 22) {
	    cfw("if ( mem_read_direct( _ACC_, true ) == 0 ) { cpu8051.pc = destination; }");
	}

	# XRL
	if ($insttype[$i] == 23) {
	    cfw("destination ^= source;");
	}

	# JNZ
	if ($insttype[$i] == 24) {
	    cfw("if ( mem_read_direct( _ACC_, true ) != 0 ) { cpu8051.pc = destination; }");
	}

	# JMP
	if ($insttype[$i] == 25) {
	    cfw("cpu8051.pc = destination;");
	}

	# MOV
	if ($insttype[$i] == 26) {
	    cfw("destination = source;");
	}

	# SJMP
	if ($insttype[$i] == 27) {
	    cfw("cpu8051.pc = destination;");
	}

	# MOVC
	if ($insttype[$i] == 28) {
	    cfw("destination = source;");
	}

	# DIV
	if ($insttype[$i] == 29) {
            # A = destination
            # B = source
	    cfw("psw_clr_cy();");
            # If B is zero, the OV flag will be set indicating a
            # division-by-zero error
	    cfw("if (source != 0) {");
	    cfw("    mem_write_direct(_ACC_, destination/source, true);");
            cfw("    mem_write_direct( _B_, destination%source, true);");
	    cfw("    psw_clr_ov();");
	    cfw("} else {");
	    cfw("    psw_set_ov();");
	    cfw("}");
	}

	# SUBB
	if ($insttype[$i] == 30) {
	    cfw("unsigned char carryflag = psw_read_cy();");
	    cfw("psw_clr_cy();");
	    cfw("psw_clr_ac();");
	    cfw("psw_clr_ov();");
	    cfw("if ( destination < ( source + carryflag ) ) {");
	    cfw("  psw_set_cy();");
	    cfw("  if ((destination & 0x7F) > ((source + carryflag) & 0x7F))  psw_set_ov();");
	    cfw("} else if ((destination & 0x7F) < ((source + carryflag) & 0x7F))   psw_set_ov();");
	    cfw("if ((destination & 0x0F) < ((source + carryflag) & 0x0F))   psw_set_ac();");
	    cfw("destination -= source + carryflag;");
	}

	# MUL
	if ($insttype[$i] == 31) {
            # A = destination
            # B = source
	    cfw("psw_clr_cy();");
	    cfw("mem_write_direct(_ACC_, destination * source, true);");
            cfw("mem_write_direct(_B_, (destination * source) / 0x100, true);");
	    cfw("if (mem_read_direct(_B_, true) > 0)");
	    cfw("    psw_set_ov();");
	    cfw("else");
	    cfw("    psw_clr_ov();");
	}

	# CPL
	if ($insttype[$i] == 33) {
	    if ($instargs[$i*4+1] == 2) { cfw("destination ^= 0xFF;"); }
	    else { cfw("destination ^= 0x01;"); }
	}

	# CJNE
	if ($insttype[$i] == 34) {
	    cfw("unsigned int reladdr = ((signed char) mem_read8(PGM_MEM_ID, cpu8051.pc, true)) + (cpu8051.pc + 1);");
	    cfw("psw_clr_cy();");
	    cfw("if ( destination < source ) psw_set_cy();");
	    cfw("if ( destination != source ) cpu8051.pc = reladdr; else cpu8051.pc++;");
	}

	# PUSH
	if ($insttype[$i] == 35) {
	    cfw("stack_push8(destination);");
	}

	# CLR
	if ($insttype[$i] == 36) {
	    cfw("destination = 0;");
	}

	# SWAP
	if ($insttype[$i] == 37) {
	    cfw("destination = ( destination << 4 ) + ( destination >> 4 );");
	}

	# XCH
	if ($insttype[$i] == 38) {
	    cfw("unsigned char tmpval = destination;");
	    cfw("destination = source; source = tmpval;");
	    $modifysrc=1;
	}

	# POP
	if ($insttype[$i] == 39) {
            cfw("destination = stack_pop8();");
	}

	# SETB
	if ($insttype[$i] == 40) {
	    cfw("destination = 1;");
	}

	# DA
	if ($insttype[$i] == 41) {
	    cfw("if (((destination & 0x0F) > 9) || psw_read_ac()) {");
	    cfw("   if ( ( destination + 6 ) > 0xFF)  psw_set_cy();");
	    cfw("   destination += 6;");
	    cfw("}");
	    cfw("if ( psw_read_cy() || ( ( destination & 0xF0 ) > 0x90 ) ) {");
	    cfw("   if ( ( destination + 0x60 ) > 0xFF ) psw_set_cy();");
	    cfw("   destination += 0x60;");
	    cfw("}");
	}

	# DJNZ
	if ($insttype[$i] == 42) {
	    cfw("destination--;");
	    cfw("if ( destination != 0 ) cpu8051.pc = source;");
	}

	# XCHD
	if ($insttype[$i] == 43) {
	    cfw("unsigned char tmpval = ( destination & 0x0F );");
	    cfw("destination = ( destination & 0xF0 ) + ( source & 0x0F );");
	    cfw("source = ( source & 0xF0 ) + tmpval;");
	    $modifysrc=1;
	}

	# MOVX
	if ($insttype[$i] == 44) {
	    cfw("destination = source;");
	}



##############################################################################


	if ($instargs[$i*4] > 0) {
	    $op_destination=$instargs[$i*4+1];
	    $destination_unused=($insttype[$i] == 5 || $insttype[$i] == 10 || $insttype[$i] == 14 || $insttype[$i] == 34 || $insttype[$i] == 35);
	    if ($op_destination == 0) { # addr11
		cfw("cpu8051.pc = ( cpu8051.pc & 0xF800 ) | addr11;");
	    }
	    if ($op_destination == 1) { # addr16
		cfw("cpu8051.pc = addr16;");
	    }
	    if ($op_destination == 2) { # A
		cfw("mem_write_direct( _ACC_, destination, true );");
	    }
	    if ($op_destination == 3) { # direct
		if ($destination_unused) {
		    cfw("mem_write_direct( destaddr, destination, true );");
		} else {
		    cfw("mem_write_direct( destaddr, destination, false );");
		}
	    }
	    if ($op_destination == 4) { # @R0
		if ($insttype[$i] == 44) { # MOVX
		    if ($destination_unused) {
			cfw("mem_write_external( mem_read_direct( BANKPSW + _R0_, true ) | ( mem_read_direct( _P2_, true ) << 8 ), destination, true );");
		    } else {
			cfw("mem_write_external( mem_read_direct( BANKPSW + _R0_, true ) | ( mem_read_direct( _P2_, true ) << 8 ), destination, false );");
		    }
		} else {
		    if ($destination_unused) {
			cfw("mem_write_indirect( mem_read_direct( BANKPSW + _R0_, true ), destination, true );");
		    } else {
			cfw("mem_write_indirect( mem_read_direct( BANKPSW + _R0_, true ), destination, false );");
		    }
		}
	    }
	    if ($op_destination == 5) { # @R1
		if ($insttype[$i] == 44) { # MOVX
		    if ($destination_unused) {
			cfw("mem_write_external( mem_read_direct( BANKPSW + _R1_, true ) | ( mem_read_direct( _P2_, true ) << 8 ), destination, true );");
		    } else {
			cfw("mem_write_external( mem_read_direct( BANKPSW + _R1_, true ) | ( mem_read_direct( _P2_, true ) << 8 ), destination, false );");
		    }
		} else {
		    if ($destination_unused) {
			cfw("mem_write_indirect( mem_read_direct( BANKPSW + _R1_, true ), destination, true );");
		    } else {
			cfw("mem_write_indirect( mem_read_direct( BANKPSW + _R1_, true ), destination, false );");
		    }
		}
	    }
	    if ($op_destination == 6) { # R0
		cfw("mem_write_direct( BANKPSW + _R0_, destination, true );");
	    }
	    if ($op_destination == 7) { # R1
		cfw("mem_write_direct( BANKPSW + _R1_, destination, true );");
	    }
	    if ($op_destination == 8) { # R2
		cfw("mem_write_direct( BANKPSW + _R2_, destination, true );");
	    }
	    if ($op_destination == 9) { # R3
		cfw("mem_write_direct( BANKPSW + _R3_, destination, true );");
	    }
	    if ($op_destination == 10) { # R4
		cfw("mem_write_direct( BANKPSW + _R4_, destination, true );");
	    }
	    if ($op_destination == 11) { # R5
		cfw("mem_write_direct( BANKPSW + _R5_, destination, true );");
	    }
	    if ($op_destination == 12) { # R6
		cfw("mem_write_direct( BANKPSW + _R6_, destination, true );");
	    }
	    if ($op_destination == 13) { # R7
		cfw("mem_write_direct( BANKPSW + _R7_, destination, true );");
	    }

	    if ($op_destination == 14) { # bitaddr
		if ($destination_unused) {
		    cfw("mem_write_bit( dstbitaddr, destination, true );");
		} else {
		    cfw("mem_write_bit( dstbitaddr, destination, false );");
		}
	    }
	    if ($op_destination == 17) { # C
		cfw("psw_write_cy(destination);");
	    }
	    if ($op_destination == 21) { # DPTR
		cfw("mem_sfr_write_dptr(destination, true);");
	    }
	    if ($op_destination == 23) { # /bitaddr
		if ($destination_unused) {
		    cfw("mem_write_bit( dstbitaddr, destination, true );");
		} else {
		    cfw("mem_write_bit( dstbitaddr, destination, false );");
		}
	    }
	    if ($op_destination == 24) { # @DPTR
		if ($destination_unused) {
		    cfw("mem_write_external(mem_sfr_read_dptr(), destination, true);");
		} else {
		    cfw("mem_write_external(mem_sfr_read_dptr(), destination, false);");
		}
	    }
	}

	if ($modifysrc == 1) {
	    if ($instargs[$i*4] > 1) {
		$op_source=$instargs[$i*4+2];
		if ($op_source == 0) { # addr11
		    cfw("cpu8051.pc = ( cpu8051.pc & 0xF800 ) | addr11;");
		}
		if ($op_source == 1) { # addr16
		    cfw("cpu8051.pc = addr16;");
		}
		if ($op_source == 2) { # A
		    cfw("mem_write_direct( _ACC_, source, true );");
		}
		if ($op_source == 3) { # direct
		    cfw("mem_write_direct( srcaddr, source, false );");
		}
		if ($op_source == 4) { # @R0
		    if ($insttype[$i] == 44) { # MOVX
			cfw("mem_write_external( mem_read_direct( BANKPSW + _R0_, true ) | ( mem_read_direct( _P2_, true ) << 8 ), source, false );");
		    } else {
			cfw("mem_write_indirect( mem_read_direct( BANKPSW + _R0_, true ), source, false );");
		    }
		}
		if ($op_source == 5) { # @R1
		    if ($insttype[$i] == 44) { # MOVX
			cfw("mem_write_external( mem_read_direct( BANKPSW + _R1_, true ) | ( mem_read_direct( _P2_, true ) << 8 ), source, false );");
		    } else {
			cfw("mem_write_indirect( mem_read_direct( BANKPSW + _R1_, true ), source, false );");
		    }
		}
		if ($op_source == 6) { # R0
		    cfw("mem_write_direct( BANKPSW + _R0_, source, true );");
		}
		if ($op_source == 7) { # R1
		    cfw("mem_write_direct( BANKPSW + _R1_, source, true );");
		}
		if ($op_source == 8) { # R2
		    cfw("mem_write_direct( BANKPSW + _R2_, source, true );");
		}
		if ($op_source == 9) { # R3
		    cfw("mem_write_direct( BANKPSW + _R3_, source, true );");
		}
		if ($op_source == 10) { # R4
		    cfw("mem_write_direct( BANKPSW + _R4_, source, true );");
		}
		if ($op_source == 11) { # R5
		    cfw("mem_write_direct( BANKPSW + _R5_, source, true );");
		}
		if ($op_source == 12) { # R6
		    cfw("mem_write_direct( BANKPSW + _R6_, source, true );");
		}
		if ($op_source == 13) { # R7
		    cfw("mem_write_direct( BANKPSW + _R7_, source, true );");
		}
		if ($op_source == 14) { # bitaddr
		    cfw("mem_write_bit( srcbitaddr, source, false );");
		}
		if ($op_source == 17) { # C
		    cfw("psw_write_cy(source);");
		}
		if ($op_source == 21) { # DPTR
                    cfw("mem_sfr_write_dptr(source, true);");
		}
		if ($op_source == 23) { # /bitaddr
		    cfw("mem_write_bit( srcbitaddr, source, false );");
		}
		if ($op_source == 24) { # @DPTR
		    cfw("mem_write_external(mem_sfr_read_dptr(), source, false);");
		}
	    }
	}
    }
    cfw("return $a_cycles[$i];");
    print INST_IMP "}\n";
    print INST_IMP "\n";
}
# ------------------------------------------------------------------------------


# Header for instructions_8051.h
print INST_DEF "/*\n";
print INST_DEF " * instructions_8051.h\n";
write_header(INST_DEF);
print INST_DEF "#ifndef INSTRUCTIONS_8051_H\n";
print INST_DEF "#define INSTRUCTIONS_8051_H 1\n\n\n";
print INST_DEF "#define BANKPSW (mem_read_direct(_PSW_, true) & 0x18)\n\n";
print INST_DEF "typedef int (*OPCODE_FP)(void);\n\n\n";
for( $i=0; $i<256; $i++ ) {
    print INST_DEF "int\n";
    print INST_DEF "cpu8051_OP_$a_opcodehex[$i](void);\n\n";
}
print INST_DEF "\n";
print INST_DEF "/* Exported variables. */\n";
print INST_DEF "#ifdef INSTRUCTIONS_8051_M\n";
print INST_DEF "OPCODE_FP opcode_table[256] = {\n";

for ($i = 0; $i < 256; $i++) {
    $ifunc = substr($instfunction[$i], 9);

    print INST_DEF "\tcpu8051_$ifunc,\n";
}
print INST_DEF "};\n";
print INST_DEF "#else\n";
print INST_DEF "OPCODE_FP opcode_table[256];\n";
print INST_DEF "#endif\n\n\n";

print INST_DEF "#endif /* INSTRUCTIONS_8051_H */\n";

print OPCODES_DEF "#endif /* OPCODES_IMP */\n";

close OPCODES_DEF;
close OPCODES_IMP;
close OPCODELST;
close INST_DEF;
close INST_IMP;
