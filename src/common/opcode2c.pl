#!/usr/bin/perl
#
# Copyright (C) 1999 Jonathan St-André
# Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.

open INST_DEF, ">instructions_8051.h" or die "Error creating <instructions_8051.h> : $!\n";
open INST_IMP, ">instructions_8051.c" or die "Error creating <instructions_8051.c> : $!\n";
open OPCODELST, "opcodes.lst" or die "Error opening <opcodes.lst> : $!\n";
open DISASM_H, ">disasm.h" or die "Error creating <disasm.h> : $!\n";

# Write GPL license
# Argument 0 is file descriptor
sub write_header
{
    my $fd = $_[0];

    print $fd " *\n";
    print $fd " * Do not modify this file directly, as it was created by opcode2c.pl\n";
    print $fd " * Any modifications made directly to this file will be lost.\n";
    print $fd " *\n";
    print $fd " * Copyright (C) 1999 Jonathan St-André\n";
    print $fd " * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>\n";
    print $fd " *\n";
    print $fd " * This program is free software; you can redistribute it and/or modify\n";
    print $fd " * it under the terms of the GNU General Public License as published by\n";
    print $fd " * the Free Software Foundation; either version 2 of the License, or\n";
    print $fd " * (at your option) any later version.\n";
    print $fd " *\n";
    print $fd " * This program is distributed in the hope that it will be useful,\n";
    print $fd " * but WITHOUT ANY WARRANTY; without even the implied warranty of\n";
    print $fd " * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n";
    print $fd " * GNU General Public License for more details.\n";
    print $fd " *\n";
    print $fd " * You should have received a copy of the GNU General Public License\n";
    print $fd " * along with this program; if not, write to the Free Software\n";
    print $fd " * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.\n";
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
print INST_IMP "#include \"reg8051.h\"\n";
print INST_IMP "#include \"cpu8051.h\"\n";
print INST_IMP "#include \"memory.h\"\n";
print INST_IMP "#include \"psw.h\"\n";
print INST_IMP "#include \"instructions_8051.h\"\n\n\n";

# Header for disasm.h
print DISASM_H "/*\n";
print DISASM_H " * disasm.h\n";
write_header(DISASM_H);
print DISASM_H "#ifndef DISASM_H\n";
print DISASM_H "#define DISASM_H 1\n\n";

$nbinst=0;
$nbaddr=0;
$nbargs=0;
$instnumb=0;

$ligne=<OPCODELST>;
$ligne=<OPCODELST>;
while($ligne=<OPCODELST>) {
    chop $ligne;
    if (length $ligne < 2) {next;}
    @wordlist=split ' ',$ligne;
    $nbword=@wordlist;
    $instruction=$wordlist[2];
    for($i=3;$i<($nbword-2);$i++) {$instruction="$instruction $wordlist[$i]";}

    $a_instruction[$instnumb]=$instruction;
    $a_bytes[$instnumb]=$wordlist[$nbword-2];
    $a_cycles[$instnumb]=$wordlist[$nbword-1];
    $a_opcodehex[$instnumb]=$wordlist[1];
    $a_opcodebin[$instnumb]=$wordlist[0];

    $instfunction[$instnumb]="CPU8051::OP_$wordlist[1]";

    $instargs[$instnumb << 2]=$instargs[($instnumb << 2) + 1]=$instargs[($instnumb << 2) + 2]=$instargs[($instnumb << 2) + 3]=0;
    if ($nbword > 5) {
	@argslist=split /\,/,$wordlist[3];
	$argslistsize=@argslist;
	$instargs[$instnumb << 2]=$argslistsize;
	for ($i=0;$i<$argslistsize;$i++) {
	    if (not exists $argstypes{$argslist[$i]}) {
		$argstypes[$nbargs]=$argslist[$i];
		$argstypes{$argslist[$i]}=$nbargs++;
	    }
	    $instargs[($instnumb << 2) + $i + 1]=$argstypes{$argslist[$i]};
	}
    }

    if (not exists $insttext{$wordlist[2]}) {
	$insttext[$nbinst]=$wordlist[2];
	$insttext{$wordlist[2]}=$nbinst++;
    }

    $insttype[$instnumb]=$insttext{$wordlist[2]};

    if ( not exists $addrmode{$wordlist[3]}) {
	$addrmode[$nbaddr]=$wordlist[3];
	$addrmode{$wordlist[3]}=$nbaddr++;
    }

    $nbbytes[$instnumb]=$wordlist[$nbword-2];

    $instaddr[$instnumb]=$addrmode{$wordlist[3]};

    $instnumb++;
}
# ------------------------------------------------------------------------------
print DISASM_H "/*\n";
print DISASM_H " * For all 256 opcodes, the value in this table gives the instruction type\n";
print DISASM_H " * ex.: MOV, INC, CLR, CPL,...\n";
print DISASM_H " * To know what is the instruction type, use\n";
print DISASM_H " * the number as an offset in the InstTextTbl[]\n";
print DISASM_H " */\n";
print DISASM_H "static int InstTypesTbl[] = {\n";
for($i=0;$i<256;$i++) {
    print DISASM_H " $insttype[$i]";
    ($i != 255) and print DISASM_H ",";
    if (($i+1) % 16 == 0) { print DISASM_H "\n"; }
}
print DISASM_H "};\n";
print DISASM_H "\n";
# ------------------------------------------------------------------------------
print DISASM_H "/* Size(in bytes) of each instruction (offset in table is instruction opcode) */\n";
print DISASM_H "static int InstSizesTbl[] = {\n";
for($i=0;$i<256;$i++) {
    print DISASM_H " $nbbytes[$i]";
    ($i != 255) and print DISASM_H ",";
    if (($i+1) % 16 == 0) { print DISASM_H "\n"; }
}
print DISASM_H "};\n";
print DISASM_H "\n";
# ------------------------------------------------------------------------------
print DISASM_H "/* List of instructions types referenced by InstTypesTbl[] */\n";
$nbelement=@insttext;
print DISASM_H "\#define InstTextTblLength $nbelement\n";
$elementnb=0;
print DISASM_H "static char *InstTextTbl[] = {\n";
foreach $instruc (@insttext) {
    print DISASM_H " \"$instruc\"";
    ($elementnb++ < $nbelement-1) and print DISASM_H ",";
    print DISASM_H "\n";
}
print DISASM_H "};\n";
print DISASM_H "\n";
# ------------------------------------------------------------------------------
print DISASM_H "/*\n";
print DISASM_H " * Table describing all arguments types of an instruction\n";
print DISASM_H " * The table is indexed InstArgTbl[ opcode * 4]\n";
print DISASM_H " * InstArgTbl[opcode*4 + 1] gives the number of arguments the instruction has\n";
print DISASM_H " * InstArgTbl[opcode*4 + i] for i=1,2 and 3 give the type of each argument\n";
print DISASM_H " * for most instructions, the 3rd argument isn't used\n";
print DISASM_H " * the argument type is referecing to ArgsTextTbl[]\n";
print DISASM_H " */\n";
print DISASM_H "\#define InstArgTblLength 256\n";
print DISASM_H "static int InstArgTbl[] = {\n";
for($i=0;$i<1024;$i++) {
    print DISASM_H " $instargs[$i]";
    ($i != 1023) and print DISASM_H ",";
    if (($i+1) % 16 == 0) { print DISASM_H "\n"; }
}
print DISASM_H "};\n";
print DISASM_H "\n";
# ------------------------------------------------------------------------------
print DISASM_H "/*\n";
print DISASM_H " * List all types of arguments available to instructions\n";
print DISASM_H " * Referenced by InstArgsTbl[]\n";
print DISASM_H " */\n";
$nbelement=@argstypes;
print DISASM_H "\#define ArgsTextTblLength $nbelement\n";
$elementnb=0;
print DISASM_H "static char *ArgsTextTbl[] = {\n";
foreach $args (@argstypes) {
    print DISASM_H " \"$args\"";
    ($elementnb++ < $nbelement-1) and print DISASM_H ",";
    print DISASM_H "\n";
}
print DISASM_H "};\n";
print DISASM_H "\n";

# ------------------------------------------------------------------------------
for ($i=0 ; $i< 256; $i++) {
    print INST_IMP "/*","*"x76,"\n";
    print INST_IMP " * Instruction \"$a_instruction[$i]\" takes $a_cycles[$i] cycle(s) and $a_bytes[$i] byte(s).\n";
    print INST_IMP " ","*"x76,"/\n";
    print INST_IMP "int\n";
    print INST_IMP "cpu8051_OP_$a_opcodehex[$i](void)\n";
    print INST_IMP "{\n";

    if( $i == 0x85 ) {
	# Cas particulier pour MOV direct,direct -> src et dest sont inverses dans la memoire
        print INST_IMP "  unsigned char srcaddr = memory_read8( PGM_MEM_ID, cpu8051.pc++ );\n";
	print INST_IMP "  unsigned char source = cpu8051_ReadD( srcaddr );\n";
	print INST_IMP "  unsigned char destaddr = memory_read8( PGM_MEM_ID, cpu8051.pc++ );\n";
	print INST_IMP "  unsigned char destination = cpu8051_ReadD( destaddr );\n";
	print INST_IMP "  destination = source;\n";
	print INST_IMP "  cpu8051_WriteD( destaddr, destination );\n";
    }
    else {
	if ($instargs[$i*4] > 0) {
	    $op_destination=$instargs[$i*4+1];
	    if ($op_destination == 0) { # addr11
		cfw("unsigned int addr11 = ( ( memory_read8( PGM_MEM_ID, cpu8051.pc - 1 ) << 3 ) & 0xF00 ) + memory_read8( PGM_MEM_ID, cpu8051.pc );");
		cfw("cpu8051.pc++;");
	    }
	    if ($op_destination == 1) { # addr16
		cfw("uint16_t addr16 = pgm_read_addr16(cpu8051.pc);");
		cfw("cpu8051.pc += 2;");
	    }
	    if ($op_destination == 2) { # A
		cfw("unsigned char destination = cpu8051_ReadD( _ACC_ );");
	    }
	    if ($op_destination == 3) { # direct
		cfw("unsigned char destaddr = memory_read8( PGM_MEM_ID, cpu8051.pc++ );");
		cfw("unsigned char destination = cpu8051_ReadD( destaddr );");
	    }
	    if ($op_destination == 4) { # @R0
		cfw("unsigned char destination = cpu8051_ReadI ( cpu8051_ReadD( BANKPSW + _R0_ ) );");
	    }
	    if ($op_destination == 5) { # @R1
		cfw("unsigned char destination = cpu8051_ReadI ( cpu8051_ReadD( BANKPSW + _R1_ ) );");
	    }
	    if ($op_destination == 6) { # R0
		cfw("unsigned char destination = cpu8051_ReadD( BANKPSW + _R0_ );");
	    }
	    if ($op_destination == 7) { # R1
		cfw("unsigned char destination = cpu8051_ReadD( BANKPSW + _R1_ );");
	    }
	    if ($op_destination == 8) { # R2
		cfw("unsigned char destination = cpu8051_ReadD( BANKPSW + _R2_ );");
	    }
	    if ($op_destination == 9) { # R3
		cfw("unsigned char destination = cpu8051_ReadD( BANKPSW + _R3_ );");
	    }
	    if ($op_destination == 10) { # R4
		cfw("unsigned char destination = cpu8051_ReadD( BANKPSW + _R4_ );");
	    }
	    if ($op_destination == 11) { # R5
		cfw("unsigned char destination = cpu8051_ReadD( BANKPSW + _R5_ );");
	    }
	    if ($op_destination == 12) { # R6
		cfw("unsigned char destination = cpu8051_ReadD( BANKPSW + _R6_ );");
	    }
	    if ($op_destination == 13) { # R7
		cfw("unsigned char destination = cpu8051_ReadD( BANKPSW + _R7_ );");
	    }
	    if ($op_destination == 14) { # bitaddr
		cfw("unsigned char destination, dstbitaddr = memory_read8( PGM_MEM_ID, cpu8051.pc++ );");
		cfw("destination = cpu8051_ReadB( dstbitaddr );");
	    }
	    if ($op_destination == 15) { # reladdr
		cfw("cpu8051.pc++;");
		cfw("unsigned int destination = ((char) memory_read8(PGM_MEM_ID, cpu8051.pc - 1)) + cpu8051.pc;");
	    }
	    if ($op_destination == 16) { # #data
		cfw("unsigned char destination = memory_read8( PGM_MEM_ID, cpu8051.pc++ );");
	    }
	    if ($op_destination == 17) { # C
		cfw("unsigned char destination = psw_read_cy();");
	    }
	    if ($op_destination == 18) { # @A+DPTR
		cfw("unsigned int destination = cpu8051_ReadD( _ACC_ ) + memory_sfr_read_dptr();");
	    }
            if ($op_destination == 20) { # AB
                cfw("unsigned char destination = cpu8051_ReadD( _ACC_ );");
                cfw("unsigned char source = cpu8051_ReadD( _B_ );");
            }
	    if ($op_destination == 21) { # DPTR
		cfw("unsigned int destination = memory_sfr_read_dptr();");
	    }
	    if ($op_destination == 23) { # /bitaddr
		cfw("unsigned char destination, dstbitaddr = memory_read8( PGM_MEM_ID, (cpu8051.pc)++ );");
		cfw("destination = ( cpu8051_ReadB( dstbitaddr ) ^ 1 );");
	    }
	    if ($op_destination == 24) { # @DPTR
		cfw("unsigned char destination = cpu8051_ReadI(memory_sfr_read_dptr());");
	    }
	}

	if ($instargs[$i*4] > 1) {
	    $op_source=$instargs[$i*4+2];
	    if ($op_source == 0) { # addr11
		cfw("unsigned int addr11 = ( ( memory_read8( PGM_MEM_ID, cpu8051.pc - 1 ) << 3 ) & 0xF00 ) + memory_read8( PGM_MEM_ID, (cpu8051.pc)++ );");
	    }
	    if ($op_source == 1) { # addr16
		cfw("uint16_t addr16 = pgm_read_addr16(cpu8051.pc);");
		cfw("cpu8051.pc += 2;");
	    }
	    if ($op_source == 2) { # A
		cfw("unsigned char source = cpu8051_ReadD( _ACC_ );");
	    }
	    if ($op_source == 3) { # direct
		cfw("unsigned char srcaddr = memory_read8( PGM_MEM_ID, (cpu8051.pc)++ );");
		cfw("unsigned char source = cpu8051_ReadD( srcaddr );");
	    }
	    if ($op_source == 4) { # @R0
		cfw("unsigned char source = cpu8051_ReadI ( cpu8051_ReadD( BANKPSW + _R0_ ) );");
	    }
	    if ($op_source == 5) { # @R1
		cfw("unsigned char source = cpu8051_ReadI ( cpu8051_ReadD( BANKPSW + _R1_ ) );");
	    }
	    if ($op_source == 6) { # R0
		cfw("unsigned char source = cpu8051_ReadD( BANKPSW + _R0_ );");
	    }
	    if ($op_source == 7) { # R1
		cfw("unsigned char source = cpu8051_ReadD( BANKPSW + _R1_ );");
	    }
	    if ($op_source == 8) { # R2
		cfw("unsigned char source = cpu8051_ReadD( BANKPSW + _R2_ );");
	    }
	    if ($op_source == 9) { # R3
		cfw("unsigned char source = cpu8051_ReadD( BANKPSW + _R3_ );");
	    }
	    if ($op_source == 10) { # R4
		cfw("unsigned char source = cpu8051_ReadD( BANKPSW + _R4_ );");
	    }
	    if ($op_source == 11) { # R5
		cfw("unsigned char source = cpu8051_ReadD( BANKPSW + _R5_ );");
	    }
	    if ($op_source == 12) { # R6
		cfw("unsigned char source = cpu8051_ReadD( BANKPSW + _R6_ );");
	    }
	    if ($op_source == 13) { # R7
		cfw("unsigned char source = cpu8051_ReadD( BANKPSW + _R7_ );");
	    }

	    if ($op_source == 14) { # bitaddr
		cfw("unsigned char source, srcbitaddr = memory_read8( PGM_MEM_ID, (cpu8051.pc)++ );");
		cfw("source = cpu8051_ReadB( srcbitaddr );");
	    }
	    if ($op_source == 15) { # reladdr
		cfw("(cpu8051.pc)++;");
		cfw("unsigned int source = ((char) memory_read8(PGM_MEM_ID, cpu8051.pc - 1)) + cpu8051.pc;");
	    }
	    if ($op_source == 16) { # #data
		cfw("unsigned char source = memory_read8( PGM_MEM_ID, (cpu8051.pc)++ );");
	    }
	    if ($op_source == 17) { # C
		cfw("unsigned char source = psw_read_cy();");
	    }
	    if ($op_source == 18) { # @A+DPTR
		cfw("unsigned char source = memory_read8( PGM_MEM_ID, cpu8051_ReadD( _ACC_ ) + memory_sfr_read_dptr());");
	    }
	    if ($op_source == 19) { # @A+PC
		cfw("unsigned char source = memory_read8( PGM_MEM_ID, cpu8051_ReadD( _ACC_ ) + ( ++cpu8051.pc ) );");
	    }
	    if ($op_source == 21) { # DPTR
		cfw("unsigned int source = memory_sfr_read_dptr();");
	    }
	    if ($op_source == 22) { # #data16
		cfw("uint16_t source = pgm_read_addr16(cpu8051.pc);");
		cfw("cpu8051.pc += 2;");
	    }
	    if ($op_source == 23) { # /bitaddr
		cfw("unsigned char source, srcbitaddr = memory_read8( PGM_MEM_ID, (cpu8051.pc)++ );");
		cfw("source = ( cpu8051_ReadB( srcbitaddr ) ^ 1 );");
	    }
	    if ($op_source == 24) { # @DPTR
		cfw("unsigned char source = cpu8051_ReadI(memory_sfr_read_dptr());");
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
	    cfw("psw_clr_cy();");
	    cfw("psw_clr_ac();");
	    cfw("psw_clr_ov();");
            # The Overflow (OV) bit is set if there is a carry-out of bit 6 or
            # out of bit 7, but not both. In other words, if the addition of the
            # Accumulator, operand and (in the case of ADDC) the Carry flag
            # treated as signed values results in a value that is out of the
            # range of a signed byte (-128 through +127) the Overflow flag is
            # set. Otherwise, the Overflow flag is cleared.
	    cfw("if ( destination + source > 0xFF ) {");
            # Carry from bit 7
	    cfw("   psw_set_cy();");
            # If no carry from bit 6, set OV
	    cfw("   if (((destination & 0x7F) + (source & 0x7F)) < 0x80)");
	    cfw("       psw_set_ov();");
            # If no carry from bit 7, but caary from bit 6, set OV
	    cfw("} else if (((destination & 0x7F) + (source & 0x7F)) > 0x7F )  psw_set_ov();");
	    cfw("if (((destination & 0x0F) + (source & 0x0F)) > 0x0F)  psw_set_ac();");
	    cfw("destination += source;");
	}

	# JNB
	if ($insttype[$i] == 14) {
	    cfw("if ( destination == 0 ) { cpu8051.pc = source; }");
	}

	# RETI
	if ($insttype[$i] == 15) {
	    cfw("cpu8051.active_priority = -1;");
            cfw("cpu8051.pc = stack_pop16();");
	}

	# RLC
	if ($insttype[$i] == 16) {
	    cfw("unsigned char new_cy = destination & 0x80;");
	    cfw("destination = ( destination << 1 ) | psw_read_cy();");
	    cfw("psw_write_cy(new_cy);");
	}

	# ADDC
	if ($insttype[$i] == 17) {
	    cfw("unsigned char carryflag = psw_read_cy();");
	    cfw("psw_clr_cy();");
	    cfw("psw_clr_ac();");
	    cfw("psw_clr_ov();");
	    cfw("if ( destination + source + carryflag > 0xFF ) {");
	    cfw("   psw_set_cy();");
	    cfw("   if (((destination & 0x7F) + (source & 0x7F) + carryflag) < 0x80)");
	    cfw("       psw_set_ov();");
	    cfw("} else if (((destination & 0x7F) + (source & 0x7F) + carryflag) > 0x7F)  psw_set_ov();");
	    cfw("if (((destination & 0x0F) + (source & 0x0F) + carryflag) > 0x0F)  psw_set_ac();");
	    cfw("destination += source;");
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
	    cfw("if ( cpu8051_ReadD( _ACC_ ) == 0 ) { cpu8051.pc = destination; }");
	}

	# XRL
	if ($insttype[$i] == 23) {
	    cfw("destination ^= source;");
	}

	# JNZ
	if ($insttype[$i] == 24) {
	    cfw("if ( cpu8051_ReadD( _ACC_ ) != 0 ) { cpu8051.pc = destination; }");
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
	    cfw("    cpu8051_WriteD(_ACC_, destination/source);");
            cfw("    cpu8051_WriteD( _B_, destination%source);");
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
	    cfw("cpu8051_WriteD(_ACC_, destination * source);");
            cfw("cpu8051_WriteD(_B_, (destination * source) / 0x100);");
	    cfw("if (cpu8051_ReadD(_B_) > 0)");
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
	    cfw("unsigned int reladdr = ((char) memory_read8(PGM_MEM_ID, cpu8051.pc)) + (cpu8051.pc + 1);");
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
	    if ($op_destination == 0) { # addr11
		cfw("cpu8051.pc = ( cpu8051.pc & 0xF800 ) | addr11;");
	    }
	    if ($op_destination == 1) { # addr16
		cfw("cpu8051.pc = addr16;");
	    }
	    if ($op_destination == 2) { # A
		cfw("cpu8051_WriteD( _ACC_, destination );");
	    }
	    if ($op_destination == 3) { # direct
		cfw("cpu8051_WriteD( destaddr, destination );");
	    }
	    if ($op_destination == 4) { # @R0
		cfw("cpu8051_WriteI( cpu8051_ReadD( BANKPSW + _R0_ ), destination );");
	    }
	    if ($op_destination == 5) { # @R1
		cfw("cpu8051_WriteI( cpu8051_ReadD( BANKPSW + _R1_ ), destination );");
	    }
	    if ($op_destination == 6) { # R0
		cfw("cpu8051_WriteD( BANKPSW + _R0_, destination );");
	    }
	    if ($op_destination == 7) { # R1
		cfw("cpu8051_WriteD( BANKPSW + _R1_, destination );");
	    }
	    if ($op_destination == 8) { # R2
		cfw("cpu8051_WriteD( BANKPSW + _R2_, destination );");
	    }
	    if ($op_destination == 9) { # R3
		cfw("cpu8051_WriteD( BANKPSW + _R3_, destination );");
	    }
	    if ($op_destination == 10) { # R4
		cfw("cpu8051_WriteD( BANKPSW + _R4_, destination );");
	    }
	    if ($op_destination == 11) { # R5
		cfw("cpu8051_WriteD( BANKPSW + _R5_, destination );");
	    }
	    if ($op_destination == 12) { # R6
		cfw("cpu8051_WriteD( BANKPSW + _R6_, destination );");
	    }
	    if ($op_destination == 13) { # R7
		cfw("cpu8051_WriteD( BANKPSW + _R7_, destination );");
	    }

	    if ($op_destination == 14) { # bitaddr
		cfw("cpu8051_WriteB( dstbitaddr, destination );");
	    }
	    if ($op_destination == 17) { # C
		cfw("psw_write_cy(destination);");
	    }
	    if ($op_destination == 21) { # DPTR
		cfw("memory_sfr_write_dptr(destination);");
	    }
	    if ($op_destination == 23) { # /bitaddr
		cfw("cpu8051_WriteB( dstbitaddr, destination );");
	    }
	    if ($op_destination == 24) { # @DPTR
		cfw("cpu8051_WriteI(memory_sfr_read_dptr(), destination);");
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
		    cfw("cpu8051_WriteD( _ACC_, source );");
		}
		if ($op_source == 3) { # direct
		    cfw("cpu8051_WriteD( srcaddr, source );");
		}
		if ($op_source == 4) { # @R0
		    cfw("cpu8051_WriteI( cpu8051_ReadD( BANKPSW + _R0_ ), source );");
		}
		if ($op_source == 5) { # @R1
		    cfw("cpu8051_WriteI( cpu8051_ReadD( BANKPSW + _R1_ ), source );");
		}
		if ($op_source == 6) { # R0
		    cfw("cpu8051_WriteD( BANKPSW + _R0_, source );");
		}
		if ($op_source == 7) { # R1
		    cfw("cpu8051_WriteD( BANKPSW + _R1_, source );");
		}
		if ($op_source == 8) { # R2
		    cfw("cpu8051_WriteD( BANKPSW + _R2_, source );");
		}
		if ($op_source == 9) { # R3
		    cfw("cpu8051_WriteD( BANKPSW + _R3_, source );");
		}
		if ($op_source == 10) { # R4
		    cfw("cpu8051_WriteD( BANKPSW + _R4_, source );");
		}
		if ($op_source == 11) { # R5
		    cfw("cpu8051_WriteD( BANKPSW + _R5_, source );");
		}
		if ($op_source == 12) { # R6
		    cfw("cpu8051_WriteD( BANKPSW + _R6_, source );");
		}
		if ($op_source == 13) { # R7
		    cfw("cpu8051_WriteD( BANKPSW + _R7_, source );");
		}
		if ($op_source == 14) { # bitaddr
		    cfw("cpu8051_WriteB( srcbitaddr, source );");
		}
		if ($op_source == 17) { # C
		    cfw("psw_write_cy(source);");
		}
		if ($op_source == 21) { # DPTR
                    cfw("memory_sfr_write_dptr(source);");
		}
		if ($op_source == 23) { # /bitaddr
		    cfw("cpu8051_WriteB( srcbitaddr, source );");
		}
		if ($op_source == 24) { # @DPTR
		    cfw("cpu8051_WriteI(memory_sfr_read_dptr(), source);");
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
print INST_DEF "#define BANKPSW (cpu8051_ReadD(_PSW_) & 0x18)\n\n";
print INST_DEF "typedef int (*OPCODE_FP)(void);\n\n\n";
for( $i=0; $i<256; $i++ ) {
    print INST_DEF "int\n";
    print INST_DEF "cpu8051_OP_$a_opcodehex[$i](void);\n\n";
}
print INST_DEF "\n";
print INST_DEF "/* Exported variables. */\n";
print INST_DEF "#ifdef INSTRUCTIONS_8051_M\n";
print INST_DEF "OPCODE_FP opcode_table[256] = {\n";
for( $i=0; $i<256; $i++ ) {
    $ifunc=substr($instfunction[$i], 9);
    if( $i < 255 ) {
	print INST_DEF "  cpu8051_$ifunc,\n";
    }
    else {
	print INST_DEF "  cpu8051_$ifunc\n";
	print INST_DEF "};\n";
    }
}

print INST_DEF "#else\n";
print INST_DEF "OPCODE_FP opcode_table[256];\n";
print INST_DEF "#endif\n\n\n";

print INST_DEF "#endif /* INSTRUCTIONS_8051_H */\n";

print DISASM_H "#endif /* DISASM_H */\n";

close DISASM_H;
close OPCODELST;
close INST_DEF;
close INST_IMP;
