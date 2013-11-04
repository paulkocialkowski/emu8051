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

# Header for instructions_8051.c
print INST_IMP "/*\n";
print INST_IMP " * instructions_8051.c\n";
print INST_IMP " *\n";
print INST_IMP " * Do not modify this file directly, as it was created by opcode2c.pl\n";
print INST_IMP " * Any modifications made directly to this file will be lost.\n";
print INST_IMP " *\n";
print INST_IMP " * Copyright (C) 1999 Jonathan St-André\n";
print INST_IMP " * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>\n";
print INST_IMP " *\n";
print INST_IMP " * This program is free software; you can redistribute it and/or modify\n";
print INST_IMP " * it under the terms of the GNU General Public License as published by\n";
print INST_IMP " * the Free Software Foundation; either version 2 of the License, or\n";
print INST_IMP " * (at your option) any later version.\n";
print INST_IMP " *\n";
print INST_IMP " * This program is distributed in the hope that it will be useful,\n";
print INST_IMP " * but WITHOUT ANY WARRANTY; without even the implied warranty of\n";
print INST_IMP " * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n";
print INST_IMP " * GNU General Public License for more details.\n";
print INST_IMP " *\n";
print INST_IMP " * You should have received a copy of the GNU General Public License\n";
print INST_IMP " * along with this program; if not, write to the Free Software\n";
print INST_IMP " * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.\n";
print INST_IMP "*/\n\n";
print INST_IMP "/* Define only here, for not having extern scope on local variables. */\n";
print INST_IMP "#define INSTRUCTIONS_8051_M 1\n\n\n";
print INST_IMP "#include \"reg8051.h\"\n";
print INST_IMP "#include \"cpu8051.h\"\n";
print INST_IMP "#include \"memory.h\"\n";
print INST_IMP "#include \"instructions_8051.h\"\n\n\n";

# Header for disasm.h
print DISASM_H "/*\n";
print DISASM_H " * disasm.h\n";
print DISASM_H " *\n";
print DISASM_H " * Do not modify this file directly, as it was created by opcode2c.pl\n";
print DISASM_H " * Any modifications made directly to this file will be lost.\n";
print DISASM_H " *\n";
print DISASM_H " * Copyright (C) 1999 Jonathan St-André\n";
print DISASM_H " * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>\n";
print DISASM_H " *\n";
print DISASM_H " * This program is free software; you can redistribute it and/or modify\n";
print DISASM_H " * it under the terms of the GNU General Public License as published by\n";
print DISASM_H " * the Free Software Foundation; either version 2 of the License, or\n";
print DISASM_H " * (at your option) any later version.\n";
print DISASM_H " *\n";
print DISASM_H " * This program is distributed in the hope that it will be useful,\n";
print DISASM_H " * but WITHOUT ANY WARRANTY; without even the implied warranty of\n";
print DISASM_H " * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n";
print DISASM_H " * GNU General Public License for more details.\n";
print DISASM_H " *\n";
print DISASM_H " * You should have received a copy of the GNU General Public License\n";
print DISASM_H " * along with this program; if not, write to the Free Software\n";
print DISASM_H " * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.\n";
print DISASM_H "*/\n\n";
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
		print INST_IMP "  unsigned int addr11 = ( ( memory_read8( PGM_MEM_ID, cpu8051.pc - 1 ) << 3 ) & 0xF00 ) + memory_read8( PGM_MEM_ID, cpu8051.pc );\n";
		print INST_IMP "  cpu8051.pc++;\n";
	    }
	    if ($op_destination == 1) { # addr16
		print INST_IMP "unsigned int addr16 = ( memory_read8( PGM_MEM_ID, cpu8051.pc++ ) << 8 );\n";
		print INST_IMP "addr16 += memory_read8( PGM_MEM_ID, cpu8051.pc++ );\n";
	    }
	    if ($op_destination == 2) { # A
		print INST_IMP "unsigned char destination = cpu8051_ReadD( _ACC_ );\n";
	    }
	    if ($op_destination == 3) { # direct
		print INST_IMP "unsigned char destaddr = memory_read8( PGM_MEM_ID, cpu8051.pc++ );\n";
		print INST_IMP "unsigned char destination = cpu8051_ReadD( destaddr );\n";
	    }
	    if ($op_destination == 4) { # @R0
		print INST_IMP "unsigned char destination = cpu8051_ReadI ( cpu8051_ReadD( BANKPSW + _R0_ ) );\n";
	    }
	    if ($op_destination == 5) { # @R1
		print INST_IMP "unsigned char destination = cpu8051_ReadI ( cpu8051_ReadD( BANKPSW + _R1_ ) );\n";
	    }

	    if ($op_destination == 6) { # R0
		print INST_IMP "unsigned char destination = cpu8051_ReadD( BANKPSW + _R0_ );\n";
	    }
	    if ($op_destination == 7) { # R1
		print INST_IMP "unsigned char destination = cpu8051_ReadD( BANKPSW + _R1_ );\n";
	    }
	    if ($op_destination == 8) { # R2
		print INST_IMP "unsigned char destination = cpu8051_ReadD( BANKPSW + _R2_ );\n";
	    }
	    if ($op_destination == 9) { # R3
		print INST_IMP "unsigned char destination = cpu8051_ReadD( BANKPSW + _R3_ );\n";
	    }
	    if ($op_destination == 10) { # R4
		print INST_IMP "unsigned char destination = cpu8051_ReadD( BANKPSW + _R4_ );\n";
	    }
	    if ($op_destination == 11) { # R5
		print INST_IMP "unsigned char destination = cpu8051_ReadD( BANKPSW + _R5_ );\n";
	    }
	    if ($op_destination == 12) { # R6
		print INST_IMP "unsigned char destination = cpu8051_ReadD( BANKPSW + _R6_ );\n";
	    }
	    if ($op_destination == 13) { # R7
		print INST_IMP "unsigned char destination = cpu8051_ReadD( BANKPSW + _R7_ );\n";
	    }
	    if ($op_destination == 14) { # bitaddr
		print INST_IMP "unsigned char destination, dstbitaddr = memory_read8( PGM_MEM_ID, cpu8051.pc++ );\n";
		print INST_IMP "destination = cpu8051_ReadB( dstbitaddr );\n";
	    }
	    if ($op_destination == 15) { # reladdr
		print INST_IMP "cpu8051.pc++;\n";
		print INST_IMP "unsigned int destination = ((char) memory_read8(PGM_MEM_ID, cpu8051.pc - 1)) + cpu8051.pc;\n";
	    }
	    if ($op_destination == 16) { # #data
		print INST_IMP "unsigned char destination = memory_read8( PGM_MEM_ID, cpu8051.pc++ );\n";
	    }
	    if ($op_destination == 17) { # C
		print INST_IMP "unsigned char destination = ( cpu8051_ReadD( _PSW_ ) >> 7 );\n";
	    }
	    if ($op_destination == 18) { # @A+DPTR
		print INST_IMP "unsigned int destination = cpu8051_ReadD( _ACC_ ) + cpu8051_ReadD( _DPTRLOW_ ) + ( cpu8051_ReadD( _DPTRHIGH_ ) << 8 );\n";
	    }
# Mis en commentaire car donnait un warning (destination et source unused variables...)
#	if ($op_destination == 20) { # AB
#	    print INST_IMP "unsigned char destination = cpu8051_ReadD( _ACC_ );\n";
#	    print INST_IMP "unsigned char source = cpu8051_ReadD( _B_ );\n";
#	}
	    if ($op_destination == 21) { # DPTR
		print INST_IMP "unsigned int destination = ( cpu8051_ReadD( _DPTRHIGH_ ) << 8 ) + cpu8051_ReadD( _DPTRLOW_ );\n";
	    }
	    if ($op_destination == 22) { # #data16
		print INST_IMP "unsigned char destination = ( memory_read8( PGM_MEM_ID, (cpu8051.pc)++ ) << 8 );\n";
		print INST_IMP "destination += memory_read8( PGM_MEM_ID, (cpu8051.pc)++ );\n";
	    }
	    if ($op_destination == 23) { # /bitaddr
		print INST_IMP "unsigned char destination, dstbitaddr = memory_read8( PGM_MEM_ID, (cpu8051.pc)++ );\n";
		print INST_IMP "destination = ( cpu8051_ReadB( dstbitaddr ) ^ 1 );\n";
	    }
	    if ($op_destination == 24) { # @DPTR
		print INST_IMP "unsigned char destination = cpu8051_ReadI( ( cpu8051_ReadD( _DPTRHIGH_ ) << 8 ) + cpu8051_ReadD( _DPTRLOW_) );\n";
	    }
	}

	if ($instargs[$i*4] > 1) {
	    $op_source=$instargs[$i*4+2];
	    if ($op_source == 0) { # addr11
		print INST_IMP "unsigned int addr11 = ( ( memory_read8( PGM_MEM_ID, cpu8051.pc - 1 ) << 3 ) & 0xF00 ) + memory_read8( PGM_MEM_ID, (cpu8051.pc)++ );\n";
	    }
	    if ($op_source == 1) { # addr16
		print INST_IMP "unsigned int addr16 = ( memory_read8( PGM_MEM_ID, (cpu8051.pc)++ ) << 8 );\n";
		print INST_IMP "addr16 += memory_read8( PGM_MEM_ID, (cpu8051.pc)++ );\n";
	    }
	    if ($op_source == 2) { # A
		print INST_IMP "unsigned char source = cpu8051_ReadD( _ACC_ );\n";
	    }
	    if ($op_source == 3) { # direct
		print INST_IMP "unsigned char srcaddr = memory_read8( PGM_MEM_ID, (cpu8051.pc)++ );\n";
		print INST_IMP "unsigned char source = cpu8051_ReadD( srcaddr );\n";
	    }
	    if ($op_source == 4) { # @R0
		print INST_IMP "unsigned char source = cpu8051_ReadI ( cpu8051_ReadD( BANKPSW + _R0_ ) );\n";
	    }
	    if ($op_source == 5) { # @R1
		print INST_IMP "unsigned char source = cpu8051_ReadI ( cpu8051_ReadD( BANKPSW + _R1_ ) );\n";
	    }
	    if ($op_source == 6) { # R0
		print INST_IMP "unsigned char source = cpu8051_ReadD( BANKPSW + _R0_ );\n";
	    }
	    if ($op_source == 7) { # R1
		print INST_IMP "unsigned char source = cpu8051_ReadD( BANKPSW + _R1_ );\n";
	    }
	    if ($op_source == 8) { # R2
		print INST_IMP "unsigned char source = cpu8051_ReadD( BANKPSW + _R2_ );\n";
	    }
	    if ($op_source == 9) { # R3
		print INST_IMP "unsigned char source = cpu8051_ReadD( BANKPSW + _R3_ );\n";
	    }
	    if ($op_source == 10) { # R4
		print INST_IMP "unsigned char source = cpu8051_ReadD( BANKPSW + _R4_ );\n";
	    }
	    if ($op_source == 11) { # R5
		print INST_IMP "unsigned char source = cpu8051_ReadD( BANKPSW + _R5_ );\n";
	    }
	    if ($op_source == 12) { # R6
		print INST_IMP "unsigned char source = cpu8051_ReadD( BANKPSW + _R6_ );\n";
	    }
	    if ($op_source == 13) { # R7
		print INST_IMP "unsigned char source = cpu8051_ReadD( BANKPSW + _R7_ );\n";
	    }

	    if ($op_source == 14) { # bitaddr
		print INST_IMP "unsigned char source, srcbitaddr = memory_read8( PGM_MEM_ID, (cpu8051.pc)++ );\n";
		print INST_IMP "source = cpu8051_ReadB( srcbitaddr );\n";
	    }
	    if ($op_source == 15) { # reladdr
		print INST_IMP "(cpu8051.pc)++;\n";
		print INST_IMP "unsigned int source = ((char) memory_read8(PGM_MEM_ID, cpu8051.pc - 1)) + cpu8051.pc;\n";
	    }
	    if ($op_source == 16) { # #data
		print INST_IMP "unsigned char source = memory_read8( PGM_MEM_ID, (cpu8051.pc)++ );\n";
	    }
	    if ($op_source == 17) { # C
		print INST_IMP "unsigned char source = ( cpu8051_ReadD( _PSW_ ) >> 7 );\n";
	    }
	    if ($op_source == 18) { # @A+DPTR
		print INST_IMP "unsigned char source = memory_read8( PGM_MEM_ID, cpu8051_ReadD( _ACC_ ) + cpu8051_ReadD( _DPTRLOW_ ) + ( cpu8051_ReadD( _DPTRHIGH_ ) << 8 ) );\n";
	    }
	    if ($op_source == 19) { # @A+PC
		print INST_IMP "unsigned char source = memory_read8( PGM_MEM_ID, cpu8051_ReadD( _ACC_ ) + ( ++cpu8051.pc ) );\n";
	    }
	    if ($op_source == 21) { # DPTR
		print INST_IMP "unsigned int source = ( cpu8051_ReadD( _DPTRHIGH_ ) << 8 ) + cpu8051_ReadD( _DPTRLOW_ );\n";
	    }
	    if ($op_source == 22) { # #data16
		print INST_IMP "unsigned char source = ( memory_read8( PGM_MEM_ID, (cpu8051.pc)++ ) << 8 );\n";
		print INST_IMP "source += memory_read8( PGM_MEM_ID, (cpu8051.pc)++ );\n";
	    }
	    if ($op_source == 23) { # /bitaddr
		print INST_IMP "unsigned char source, srcbitaddr = memory_read8( PGM_MEM_ID, (cpu8051.pc)++ );\n";
		print INST_IMP "source = ( cpu8051_ReadB( srcbitaddr ) ^ 1 );\n";
	    }
	    if ($op_source == 24) { # @DPTR
		print INST_IMP "unsigned char source = cpu8051_ReadI( ( cpu8051_ReadD( _DPTRHIGH_ ) << 8 ) + cpu8051_ReadD( _DPTRLOW_) );\n";
	    }
	}

##############################################################################
	$modifysrc=0;
#    print INST_IMP "\n// Inserer le code ici\n\n";

	# RR
	if ($insttype[$i] == 3) {
	    print INST_IMP "destination = ( destination >> 1 ) | ( destination << 7 );\n";
	}

	# INC
	if ($insttype[$i] == 4) {
	    print INST_IMP "destination++;\n";
	}

	# JBC
	if ($insttype[$i] == 5) {
	    print INST_IMP "if ( destination == 1 ) { cpu8051.pc = source; destination = 0; }\n";
	}

	# ACALL
	if ($insttype[$i] == 6) {
	    print INST_IMP "unsigned char SP = cpu8051_ReadD( _SP_ );\n";
	    print INST_IMP "cpu8051_WriteI( ++SP, ( cpu8051.pc & 0x00FF ) );\n";
	    print INST_IMP "cpu8051_WriteI( ++SP, ( cpu8051.pc >> 8 ) );\n";
	    print INST_IMP "cpu8051_WriteD( _SP_, SP );\n";
	}

	# LCALL
	if ($insttype[$i] == 7) {
	    print INST_IMP "unsigned char SP = cpu8051_ReadD( _SP_ );\n";
	    print INST_IMP "cpu8051_WriteI( ++SP, ( cpu8051.pc & 0x00FF ) );\n";
	    print INST_IMP "cpu8051_WriteI( ++SP, ( cpu8051.pc >> 8 ) );\n";
	    print INST_IMP "cpu8051_WriteD( _SP_, SP );\n";
	}

	# RRC
	if ($insttype[$i] == 8) {
	    print INST_IMP "unsigned char tmpval = destination;\n";
	    print INST_IMP "destination = ( destination >> 1 ) | ( cpu8051_ReadD( _PSW_ ) & 0x80 );\n";
	    print INST_IMP "cpu8051_WriteD( _PSW_, ( cpu8051_ReadD( _PSW_ ) & 0x7F ) | ( tmpval << 7 ) );\n";
	}

	# DEC
	if ($insttype[$i] == 9) {
	    print INST_IMP "destination--;\n";
	}

	# JB
	if ($insttype[$i] == 10) {
	    print INST_IMP "if ( destination == 1 ) { cpu8051.pc = source; }\n";
	}

	# RET
	if ($insttype[$i] == 11) {
	    print INST_IMP "unsigned char SP = cpu8051_ReadD( _SP_ );\n";
	    print INST_IMP "cpu8051.pc = ( cpu8051_ReadI( SP-- ) << 8 );\n";
	    print INST_IMP "cpu8051.pc += cpu8051_ReadI ( SP-- );\n";
	    print INST_IMP "cpu8051_WriteD( _SP_, SP );\n";
	}

	# RL
	if ($insttype[$i] == 12) {
	    print INST_IMP "destination = ( destination << 1 ) | ( destination >> 7 );\n";
	}

	# ADD
	if ($insttype[$i] == 13) {
	    print INST_IMP "cpu8051_WriteD( _PSW_, ( cpu8051_ReadD( _PSW_ ) & 0x3B ) );\n";
	    print INST_IMP "if ( destination + source > 0xFF ) {\n";
	    print INST_IMP "   cpu8051_WriteD( _PSW_, ( cpu8051_ReadD( _PSW_ ) | 0x80 ) );\n";
	    print INST_IMP "   if ( ( destination & 0x7F ) + ( source & 0x7F ) < 0x80 )  cpu8051_WriteD( _PSW_, ( cpu8051_ReadD( _PSW_ ) | 0x04 ) );\n";
	    print INST_IMP "} else if ( ( destination & 0x7F ) + ( source & 0x7F ) > 0x7F )  cpu8051_WriteD( _PSW_, ( cpu8051_ReadD( _PSW_ ) | 0x04 ) );\n";
	    print INST_IMP "if ( ( destination & 0x0F ) + ( source & 0x0F ) > 0x0F )   cpu8051_WriteD( _PSW_, ( cpu8051_ReadD( _PSW_ ) | 0x04 ) );\n";
	    print INST_IMP "destination += source;\n";
	}

	# JNB
	if ($insttype[$i] == 14) {
	    print INST_IMP "if ( destination == 0 ) { cpu8051.pc = source; }\n";
	}

	# RETI
	if ($insttype[$i] == 15) {
	    print INST_IMP "cpu8051.active_priority = -1;\n";
	    print INST_IMP "unsigned char SP = cpu8051_ReadD( _SP_ );\n";
	    print INST_IMP "cpu8051.pc = ( cpu8051_ReadI( SP-- ) << 8 );\n";
	    print INST_IMP "cpu8051.pc += cpu8051_ReadI( SP-- );\n";
	    print INST_IMP "cpu8051_WriteD( _SP_, SP );\n";
	}

	# RLC
	if ($insttype[$i] == 16) {
	    print INST_IMP "unsigned char tmpval = destination;\n";
	    print INST_IMP "destination = ( destination << 1 ) | ( ( cpu8051_ReadD( _PSW_ ) & 0x80 ) >> 7 );\n";
	    print INST_IMP "cpu8051_WriteD( _PSW_, ( cpu8051_ReadD( _PSW_ ) & 0x7F ) | ( tmpval & 0x80 ) );\n";
	}

	# ADDC
	if ($insttype[$i] == 17) {
	    print INST_IMP "unsigned char carryflag = ( cpu8051_ReadD( _PSW_ ) >> 7 );\n";
	    print INST_IMP "cpu8051_WriteD( _PSW_, ( cpu8051_ReadD( _PSW_ ) & 0x3B ) );\n";
	    print INST_IMP "if ( destination + source + carryflag > 0xFF ) {\n";
	    print INST_IMP "   cpu8051_WriteD( _PSW_, ( cpu8051_ReadD( _PSW_ ) | 0x80 ) );\n";
	    print INST_IMP "   if ( ( destination & 0x7F ) + ( source & 0x7F ) + carryflag < 0x80 )  cpu8051_WriteD( _PSW_, ( cpu8051_ReadD( _PSW_ ) | 0x04 ) );\n";
	    print INST_IMP "} else if ( ( destination & 0x7F ) + ( source & 0x7F ) + carryflag > 0x7F )  cpu8051_WriteD( _PSW_, ( cpu8051_ReadD( _PSW_ ) | 0x04 ) );\n";
	    print INST_IMP "if ( ( destination & 0x0F ) + ( source & 0x0F ) + carryflag > 0x0F )  cpu8051_WriteD( _PSW_, ( cpu8051_ReadD( _PSW_ ) | 0x40 ) );\n";
	    print INST_IMP "destination += source;\n";
	}

	# JC
	if ($insttype[$i] == 18) {
	    print INST_IMP "if ( cpu8051_ReadD( _PSW_ ) > 0x7F) { cpu8051.pc = destination; }\n";
	}

	# ORL
	if ($insttype[$i] == 19) {
	    if ($instargs[$i*4+1] == 17) {
		# sur des bits
		print INST_IMP "cpu8051_WriteD( _PSW_ , ( ( destination | source ) << 7 ) );\n";
	    } else {
		# sur des bytes
		print INST_IMP "destination |= source;\n";
	    }
	}

	# JNC
	if ($insttype[$i] == 20) {
	    print INST_IMP "if ( cpu8051_ReadD( _PSW_ ) < 0x80 ) { cpu8051.pc = destination; }\n";
	}

	# ANL
	if ($insttype[$i] == 21) {
	    if ($instargs[$i*4+1] == 17) {
		# sur des bits
		print INST_IMP "cpu8051_WriteD( _PSW_, ( ( destination & source) << 7 ) );\n";
	    } else {
		# sur des bytes
		print INST_IMP "destination &= source;\n";
	    }
	}

	# JZ
	if ($insttype[$i] == 22) {
	    print INST_IMP "if ( cpu8051_ReadD( _ACC_ ) == 0 ) { cpu8051.pc = destination; }\n";
	}

	# XRL
	if ($insttype[$i] == 23) {
	    print INST_IMP "destination ^= source;\n";
	}

	# JNZ
	if ($insttype[$i] == 24) {
	    print INST_IMP "if ( cpu8051_ReadD( _ACC_ ) != 0 ) { cpu8051.pc = destination; }\n";
	}

	# JMP
	if ($insttype[$i] == 25) {
	    print INST_IMP "cpu8051.pc = destination;\n";
	}

	# MOV
	if ($insttype[$i] == 26) {
	    print INST_IMP "destination = source;\n";
	}

	# SJMP
	if ($insttype[$i] == 27) {
	    print INST_IMP "cpu8051.pc = destination;\n";
	}

	# MOVC
	if ($insttype[$i] == 28) {
	    print INST_IMP "destination = source;\n";
	}

	# DIV
	if ($insttype[$i] == 29) {
	    print INST_IMP "unsigned char A = cpu8051_ReadD( _ACC_ ), B = cpu8051_ReadD( _B_ );\n";
	    print INST_IMP "cpu8051_WriteD( _PSW_, ( cpu8051_ReadD( _PSW_ ) & 0x7B ) );\n";
	    print INST_IMP "if ( B != 0 ) {\n";
	    print INST_IMP "cpu8051_WriteD( _ACC_, A/B ); cpu8051_WriteD( _B_, A%B );\n";
	    print INST_IMP "} else cpu8051_WriteD( _PSW_, ( cpu8051_ReadD( _PSW_ ) | 0x04 ) );\n";
	}

	# SUBB
	if ($insttype[$i] == 30) {
	    print INST_IMP "unsigned char carryflag = cpu8051_ReadD( _PSW_ ) >> 7;\n";
	    print INST_IMP "cpu8051_WriteD( _PSW_, ( cpu8051_ReadD( _PSW_ ) & 0x3B ) );\n";
	    print INST_IMP "if ( destination < ( source + carryflag ) ) {\n";
	    print INST_IMP "  cpu8051_WriteD( _PSW_, ( cpu8051_ReadD( _PSW_ ) | 0x80 ) );\n";
	    print INST_IMP "  if ( ( destination & 0x7F ) > ( ( source + carryflag ) & 0x7F ) )  cpu8051_WriteD( _PSW_, ( cpu8051_ReadD( _PSW_ ) | 0x04 ) );\n";
	    print INST_IMP "} else if ( ( destination & 0x7F ) < ( ( source + carryflag ) & 0x7F ) )   cpu8051_WriteD( _PSW_, ( cpu8051_ReadD( _PSW_ ) | 0x04 ) );\n";
	    print INST_IMP "if ( ( destination & 0x0F ) < ( ( source + carryflag ) & 0x0F ) )   cpu8051_WriteD( _PSW_, ( cpu8051_ReadD( _PSW_ ) | 0x40 ) );\n";
	    print INST_IMP "destination -= source + carryflag;\n";
	}

	# MUL
	if ($insttype[$i] == 31) {
	    print INST_IMP "unsigned char A = cpu8051_ReadD( _ACC_ ), B = cpu8051_ReadD( _B_ );\n";
	    print INST_IMP "cpu8051_WriteD( _PSW_, ( cpu8051_ReadD( _PSW_ ) & 0x7B ) );\n";
	    print INST_IMP "cpu8051_WriteD( _ACC_ , ( ( A * B ) & 0x00FF ) ); cpu8051_WriteD( _B_, ( A * B ) / 0x100 );\n";
	    print INST_IMP "if ( cpu8051_ReadD( _B_ ) > 0) cpu8051_WriteD( _PSW_, ( cpu8051_ReadD( _PSW_ ) | 0x04 ) );\n";
	}

	# CPL
	if ($insttype[$i] == 33) {
	    if ($instargs[$i*4+1] == 2) { print INST_IMP "destination ^= 0xFF;\n"; }
	    else { print INST_IMP "destination ^= 0x01;\n"; }
	}

	# CJNE
	if ($insttype[$i] == 34) {
	    print INST_IMP "unsigned int reladdr = ((char) memory_read8(PGM_MEM_ID, cpu8051.pc)) + (cpu8051.pc + 1);\n";
	    print INST_IMP "cpu8051_WriteD( _PSW_, ( cpu8051_ReadD( _PSW_ ) & 0x7F ) );\n";
	    print INST_IMP "if ( destination < source ) cpu8051_WriteD( _PSW_, ( cpu8051_ReadD( _PSW_ ) | 0x80 ) );\n";
	    print INST_IMP "if ( destination != source ) cpu8051.pc = reladdr; else cpu8051.pc++; \n";
	}

	# PUSH
	if ($insttype[$i] == 35) {
	    print INST_IMP "unsigned char SP = cpu8051_ReadD( _SP_ );\n";
	    print INST_IMP "cpu8051_WriteI( ++SP, destination );\n";
	    print INST_IMP "cpu8051_WriteD( _SP_, SP );\n";
	}

	# CLR
	if ($insttype[$i] == 36) {
	    print INST_IMP "destination = 0;\n";
	}

	# SWAP
	if ($insttype[$i] == 37) {
	    print INST_IMP "destination = ( destination << 4 ) + ( destination >> 4 );\n";
	}

	# XCH
	if ($insttype[$i] == 38) {
	    print INST_IMP "unsigned char tmpval = destination;\n";
	    print INST_IMP "destination = source; source = tmpval;\n";
	    $modifysrc=1;
	}

	# POP
	if ($insttype[$i] == 39) {
	    print INST_IMP "unsigned char SP = cpu8051_ReadD( _SP_ );\n";
	    print INST_IMP "destination = cpu8051_ReadI( SP-- );\n";
	    print INST_IMP "cpu8051_WriteD( _SP_, SP );\n";
	}

	# SETB
	if ($insttype[$i] == 40) {
	    print INST_IMP "destination = 1;\n";
	}

	# DA
	if ($insttype[$i] == 41) {
	    print INST_IMP "if ( ( ( destination & 0x0F ) > 9) || ( cpu8051_ReadD( _PSW_ ) | 0x40)) {\n";
	    print INST_IMP "   if ( ( destination + 6 ) > 0xFF)  cpu8051_WriteD( _PSW_, ( cpu8051_ReadD( _PSW_ ) | 0x80 ) );\n";
	    print INST_IMP "   destination += 6;\n";
	    print INST_IMP "}\n";
	    print INST_IMP "if ( ( cpu8051_ReadD( _PSW_ ) & 0x80) || ( ( destination & 0xF0 ) > 0x90 ) ) {\n";
	    print INST_IMP "   if ( ( destination + 0x60 ) > 0xFF ) cpu8051_WriteD( _PSW_, ( cpu8051_ReadD( _PSW_ ) | 0x80 ) );\n";
	    print INST_IMP "   destination += 0x60;\n";
	    print INST_IMP "}\n";
	}

	# DJNZ
	if ($insttype[$i] == 42) {
	    print INST_IMP "destination--;\n";
	    print INST_IMP "if ( destination != 0 ) cpu8051.pc = source;\n";
	}

	# XCHD
	if ($insttype[$i] == 43) {
	    print INST_IMP "unsigned char tmpval = ( destination & 0x0F );\n";
	    print INST_IMP "destination = ( destination & 0xF0 ) + ( source & 0x0F );\n";
	    print INST_IMP "source = ( source & 0xF0 ) + tmpval;\n";
	    $modifysrc=1;
	}

	# MOVX
	if ($insttype[$i] == 44) {
	    print INST_IMP "destination = source;\n";
	}



##############################################################################


	if ($instargs[$i*4] > 0) {
	    $op_destination=$instargs[$i*4+1];
	    if ($op_destination == 0) { # addr11
		print INST_IMP "cpu8051.pc = ( cpu8051.pc & 0xF800 ) | addr11;\n";
	    }
	    if ($op_destination == 1) { # addr16
		print INST_IMP "cpu8051.pc = addr16;\n";
	    }
	    if ($op_destination == 2) { # A
		print INST_IMP "cpu8051_WriteD( _ACC_, destination );\n";
	    }
	    if ($op_destination == 3) { # direct
		print INST_IMP "cpu8051_WriteD( destaddr, destination );\n";
	    }
	    if ($op_destination == 4) { # @R0
		print INST_IMP "cpu8051_WriteI( cpu8051_ReadD( BANKPSW + _R0_ ), destination );\n";
	    }
	    if ($op_destination == 5) { # @R1
		print INST_IMP "cpu8051_WriteI( cpu8051_ReadD( BANKPSW + _R1_ ), destination );\n";
	    }
	    if ($op_destination == 6) { # R0
		print INST_IMP "cpu8051_WriteD( BANKPSW + _R0_, destination );\n";
	    }
	    if ($op_destination == 7) { # R1
		print INST_IMP "cpu8051_WriteD( BANKPSW + _R1_, destination );\n";
	    }
	    if ($op_destination == 8) { # R2
		print INST_IMP "cpu8051_WriteD( BANKPSW + _R2_, destination );\n";
	    }
	    if ($op_destination == 9) { # R3
		print INST_IMP "cpu8051_WriteD( BANKPSW + _R3_, destination );\n";
	    }
	    if ($op_destination == 10) { # R4
		print INST_IMP "cpu8051_WriteD( BANKPSW + _R4_, destination );\n";
	    }
	    if ($op_destination == 11) { # R5
		print INST_IMP "cpu8051_WriteD( BANKPSW + _R5_, destination );\n";
	    }
	    if ($op_destination == 12) { # R6
		print INST_IMP "cpu8051_WriteD( BANKPSW + _R6_, destination );\n";
	    }
	    if ($op_destination == 13) { # R7
		print INST_IMP "cpu8051_WriteD( BANKPSW + _R7_, destination );\n";
	    }

	    if ($op_destination == 14) { # bitaddr
		print INST_IMP "cpu8051_WriteB( dstbitaddr, destination );\n";
	    }
	    if ($op_destination == 17) { # C
		print INST_IMP "cpu8051_WriteD( _PSW_, ( ( cpu8051_ReadD( _PSW_ ) & 0x7F) | ( destination << 7 ) ) );\n";
	    }
	    if ($op_destination == 21) { # DPTR
		print INST_IMP "cpu8051_WriteD( _DPTRHIGH_, ( destination >> 8 ) );\n";
		print INST_IMP "cpu8051_WriteD( _DPTRLOW_, ( destination & 0xFF ) );\n";
	    }
	    if ($op_destination == 23) { # /bitaddr
		print INST_IMP "cpu8051_WriteB( dstbitaddr, destination );\n";
	    }
	    if ($op_destination == 24) { # @DPTR
		print INST_IMP "cpu8051_WriteI( ( cpu8051_ReadD( _DPTRHIGH_ ) << 8 ) + cpu8051_ReadD( _DPTRLOW_ ), destination );\n";
	    }
	}

	if ($modifysrc == 1) {
	    if ($instargs[$i*4] > 1) {
		$op_source=$instargs[$i*4+2];
		if ($op_source == 0) { # addr11
		    print INST_IMP "cpu8051.pc = ( cpu8051.pc & 0xF800 ) | addr11;\n";
		}
		if ($op_source == 1) { # addr16
		    print INST_IMP "cpu8051.pc = addr16;\n";
		}
		if ($op_source == 2) { # A
		    print INST_IMP "cpu8051_WriteD( _ACC_, source );\n";
		}
		if ($op_source == 3) { # direct
		    print INST_IMP "cpu8051_WriteD( srcaddr, source );\n";
		}
		if ($op_source == 4) { # @R0
		    print INST_IMP "cpu8051_WriteI( cpu8051_ReadD( BANKPSW + _R0_ ), source );\n";
		}
		if ($op_source == 5) { # @R1
		    print INST_IMP "cpu8051_WriteI( cpu8051_ReadD( BANKPSW + _R1_ ), source );\n";
		}
		if ($op_source == 6) { # R0
		    print INST_IMP "cpu8051_WriteD( BANKPSW + _R0_, source );\n";
		}
		if ($op_source == 7) { # R1
		    print INST_IMP "cpu8051_WriteD( BANKPSW + _R1_, source );\n";
		}
		if ($op_source == 8) { # R2
		    print INST_IMP "cpu8051_WriteD( BANKPSW + _R2_, source );\n";
		}
		if ($op_source == 9) { # R3
		    print INST_IMP "cpu8051_WriteD( BANKPSW + _R3_, source );\n";
		}
		if ($op_source == 10) { # R4
		    print INST_IMP "cpu8051_WriteD( BANKPSW + _R4_, source );\n";
		}
		if ($op_source == 11) { # R5
		    print INST_IMP "cpu8051_WriteD( BANKPSW + _R5_, source );\n";
		}
		if ($op_source == 12) { # R6
		    print INST_IMP "cpu8051_WriteD( BANKPSW + _R6_, source );\n";
		}
		if ($op_source == 13) { # R7
		    print INST_IMP "cpu8051_WriteD( BANKPSW + _R7_, source );\n";
		}
		if ($op_source == 14) { # bitaddr
		    print INST_IMP "cpu8051_WriteB( srcbitaddr, source );\n";
		}
		if ($op_source == 17) { # C
		    print INST_IMP "cpu8051_WriteD( _PSW_, ( ( cpu8051_ReadD( _PSW_ ) & 0x7F) | ( source << 7 ) ) );\n";
		}
		if ($op_source == 21) { # DPTR
		    print INST_IMP "cpu8051_WriteD( _DPTRHIGH_, ( source >> 8 ) );\n";
		    print INST_IMP "cpu8051_WriteD( _DPTRLOW_, ( source & 0xFF ) );\n";
		}
		if ($op_source == 23) { # /bitaddr
		    print INST_IMP "cpu8051_WriteB( srcbitaddr, source );\n";
		}
		if ($op_source == 24) { # @DPTR
		    print INST_IMP "cpu8051_WriteI( ( cpu8051_ReadD( _DPTRHIGH_ ) << 8 ) + cpu8051_ReadD( _DPTRLOW_ ), source );\n";
		}
	    }
	}
    }
    print INST_IMP "return $a_cycles[$i];\n";
    print INST_IMP "}\n";
    print INST_IMP "\n\n";
}
# ------------------------------------------------------------------------------


# Header for instructions_8051.h
print INST_DEF "/*\n";
print INST_DEF " * instructions_8051.h\n";
print INST_DEF " *\n";
print INST_DEF " * Do not modify this file directly, as it was created by opcode2c.pl\n";
print INST_DEF " * Any modifications made directly to this file will be lost.\n";
print INST_DEF " *\n";
print INST_DEF " * Copyright (C) 1999 Jonathan St-André\n";
print INST_DEF " * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>\n";
print INST_DEF " *\n";
print INST_DEF " * This program is free software; you can redistribute it and/or modify\n";
print INST_DEF " * it under the terms of the GNU General Public License as published by\n";
print INST_DEF " * the Free Software Foundation; either version 2 of the License, or\n";
print INST_DEF " * (at your option) any later version.\n";
print INST_DEF " *\n";
print INST_DEF " * This program is distributed in the hope that it will be useful,\n";
print INST_DEF " * but WITHOUT ANY WARRANTY; without even the implied warranty of\n";
print INST_DEF " * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n";
print INST_DEF " * GNU General Public License for more details.\n";
print INST_DEF " *\n";
print INST_DEF " * You should have received a copy of the GNU General Public License\n";
print INST_DEF " * along with this program; if not, write to the Free Software\n";
print INST_DEF " * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.\n";
print INST_DEF "*/\n\n";

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
