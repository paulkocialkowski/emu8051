#!/usr/bin/perl

open INST_DEF,">Inst_Def.hpp" or die "Erreur ouverture Inst_Def.hpp : $!\n";
open INST_IMP,">Inst_Imp.cpp" or die "Erreur ouverture Inst_Imp.hpp : $!\n";
open OPCODELST,"opcodes.lst" or die "Erreur ouverture opcodes.lst : $!\n";
open DISASM_HPP,">disasm.hpp" or die "Erreur ouverture disasm.hpp : $!\n";

print INST_IMP "#ifndef __INST_IMP_HPP_\n";
print INST_IMP "#define __INST_IMP_HPP_\n\n";
print INST_IMP "// Do not modify this file directly, it was created by Opcode2cpp.pl\n";
print INST_IMP "// Any modification made directly on this file will be lost\n\n\n";


print INST_DEF "#ifndef __INST_DEF_HPP_\n";
print INST_DEF "#define __INST_DEF_HPP_\n";
print INST_DEF "// Do not modify this file directly, it was created by Opcode2cpp.pl\n";
print INST_DEF "// Any modification made directly on this file will be lost\n\n\n";

print DISASM_HPP "#ifndef __DISASM_HPP_\n";
print DISASM_HPP "#define __DISASM_HPP_\n";
print DISASM_HPP "// Do not modify this file directly, it was created by Opcode2cpp.pl\n";
print DISASM_HPP "// Any modification made directly on this file will be lost\n\n\n";

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
print DISASM_HPP "// For all 256 opcodes, the value in this table gives the instruction type\n";
print DISASM_HPP "// ex.: MOV, INC, CLR, CPL,...\n";
print DISASM_HPP "// To know what is the instruction type, use the number as an offset in the InstTextTbl[]\n";
print DISASM_HPP "static int InstTypesTbl[] = {\n";
for($i=0;$i<256;$i++) {
    print DISASM_HPP " $insttype[$i]";
    ($i != 255) and print DISASM_HPP ",";
    if (($i+1) % 16 == 0) { print DISASM_HPP "\n"; }
}
print DISASM_HPP "};\n";
print DISASM_HPP "\n\n";
# ------------------------------------------------------------------------------
print DISASM_HPP "// Size(in bytes) of each instruction (offset in table is instruction opcode)\n";
print DISASM_HPP "static int InstSizesTbl[] = {\n";
for($i=0;$i<256;$i++) {
    print DISASM_HPP " $nbbytes[$i]";
    ($i != 255) and print DISASM_HPP ",";
    if (($i+1) % 16 == 0) { print DISASM_HPP "\n"; }
}
print DISASM_HPP "};\n";
print DISASM_HPP "\n\n";
# ------------------------------------------------------------------------------
print DISASM_HPP "// List of instructions types referenced by InstTypesTbl[]\n";
$nbelement=@insttext;
print DISASM_HPP "\#define InstTextTblLength $nbelement\n";
$elementnb=0;
print DISASM_HPP "static char *InstTextTbl[] = {\n";
foreach $instruc (@insttext) {
    print DISASM_HPP " \"$instruc\"";
    ($elementnb++ < $nbelement-1) and print DISASM_HPP ",";
    print DISASM_HPP "\n";
}
print DISASM_HPP "};\n";
print DISASM_HPP "\n\n";
# ------------------------------------------------------------------------------
print DISASM_HPP "// Table describing all arguments types of an instruction\n";
print DISASM_HPP "// The table is indexed InstArgTbl[ opcode * 4]\n";
print DISASM_HPP "// InstArgTbl[opcode*4 + 1] gives the number of arguments the instruction has\n";
print DISASM_HPP "// InstArgTbl[opcode*4 + i] for i=1,2 and 3 give the type of each argument\n";
print DISASM_HPP "// for most instructions, the 3rd argument isn't used\n";
print DISASM_HPP "// the argument type is referecing to ArgsTextTbl[]\n";
print DISASM_HPP "\#define InstArgTblLength 256\n";
print DISASM_HPP "static int InstArgTbl[] = {\n";
for($i=0;$i<1024;$i++) {
    print DISASM_HPP " $instargs[$i]";
    ($i != 1023) and print DISASM_HPP ",";
    if (($i+1) % 16 == 0) { print DISASM_HPP "\n"; }
}
print DISASM_HPP "};\n";
print DISASM_HPP "\n\n";
# ------------------------------------------------------------------------------
print DISASM_HPP "// List all types of arguments available to instructions\n";
print DISASM_HPP "// Referenced by InstArgsTbl[]\n";
$nbelement=@argstypes;
print DISASM_HPP "\#define ArgsTextTblLength $nbelement\n";
$elementnb=0;
print DISASM_HPP "static char *ArgsTextTbl[] = {\n";
foreach $args (@argstypes) {
    print DISASM_HPP " \"$args\"";
    ($elementnb++ < $nbelement-1) and print DISASM_HPP ",";
    print DISASM_HPP "\n";
}
print DISASM_HPP "};\n";
print DISASM_HPP "\n\n";
# ------------------------------------------------------------------------------
for ($i=0 ; $i< 256; $i++) {
    print INST_IMP "/"x78,"\n";
    print INST_IMP "// void CPU8051::OP_$a_opcodehex[$i]( )\n";
    print INST_IMP "// Instruction \"$a_instruction[$i]\" takes $a_cycles[$i] cycle(s) and $a_bytes[$i] byte(s)\n";
    print INST_IMP "/"x78,"\n";
    print INST_IMP "int CPU8051::OP_$a_opcodehex[$i]( )\n";
    print INST_DEF "int OP_$a_opcodehex[$i]( );\n";
    print INST_IMP "{\n";

    if ($i == 0x85) {
	# Cas particulier pour MOV direct,direct -> src et dest sont inverses dans la memoire
        print INST_IMP "unsigned char srcaddr = PGMMem->Read8( PC++ );\n";
	print INST_IMP "unsigned char source = ReadD( srcaddr );\n";
	print INST_IMP "unsigned char destaddr = PGMMem->Read8( PC++ );\n";
	print INST_IMP "unsigned char destination = ReadD( destaddr );\n";
	print INST_IMP "destination = source;\n";
	print INST_IMP "WriteD( destaddr, destination );\n";
    } else {
	
if ($instargs[$i*4] > 0) {
	$op_destination=$instargs[$i*4+1];
	if ($op_destination == 0) { # addr11
	    print INST_IMP "unsigned int addr11 = ( ( PGMMem->Read8( PC - 1 ) << 3 ) & 0xF00 ) + PGMMem->Read8( PC++ );\n";
	}
	if ($op_destination == 1) { # addr16
	    print INST_IMP "unsigned int addr16 = ( PGMMem->Read8( PC++ ) << 8 );\n";
            print INST_IMP "addr16 += PGMMem->Read8( PC++ );\n";
	}
	if ($op_destination == 2) { # A
	    print INST_IMP "unsigned char destination = ReadD( _ACC_ );\n";
	}
	if ($op_destination == 3) { # direct
	    print INST_IMP "unsigned char destaddr = PGMMem->Read8( PC++ );\n";
	    print INST_IMP "unsigned char destination = ReadD( destaddr );\n";
	}
	if ($op_destination == 4) { # @R0
	    print INST_IMP "unsigned char destination = ReadI ( ReadD( BANKPSW + _R0_ ) );\n";
	}
	if ($op_destination == 5) { # @R1
	    print INST_IMP "unsigned char destination = ReadI ( ReadD( BANKPSW + _R1_ ) );\n";
	}

	if ($op_destination == 6) { # R0
	    print INST_IMP "unsigned char destination = ReadD( BANKPSW + _R0_ );\n";
	}
	if ($op_destination == 7) { # R1
	    print INST_IMP "unsigned char destination = ReadD( BANKPSW + _R1_ );\n";
	}
	if ($op_destination == 8) { # R2
	    print INST_IMP "unsigned char destination = ReadD( BANKPSW + _R2_ );\n";
	}
	if ($op_destination == 9) { # R3
	    print INST_IMP "unsigned char destination = ReadD( BANKPSW + _R3_ );\n";
	}
	if ($op_destination == 10) { # R4
	    print INST_IMP "unsigned char destination = ReadD( BANKPSW + _R4_ );\n";
	}
	if ($op_destination == 11) { # R5
	    print INST_IMP "unsigned char destination = ReadD( BANKPSW + _R5_ );\n";
	}
	if ($op_destination == 12) { # R6
	    print INST_IMP "unsigned char destination = ReadD( BANKPSW + _R6_ );\n";
	}
	if ($op_destination == 13) { # R7
	    print INST_IMP "unsigned char destination = ReadD( BANKPSW + _R7_ );\n";
	}
	if ($op_destination == 14) { # bitaddr
	    print INST_IMP "unsigned char destination, dstbitaddr = PGMMem->Read8( PC++ );\n";
	    print INST_IMP "destination = ReadB( dstbitaddr );\n";
	}
	if ($op_destination == 15) { # reladdr
	    print INST_IMP "PC++;\n";
	    print INST_IMP "unsigned int destination = ( ( PGMMem->Read8( PC - 1 ) + PC ) & 0xFF ) + ( PC & 0xFF00 );\n";
	}
	if ($op_destination == 16) { # #data
	    print INST_IMP "unsigned char destination = PGMMem->Read8( PC++ );\n";
	}
	if ($op_destination == 17) { # C
	    print INST_IMP "unsigned char destination = ( ReadD( _PSW_ ) >> 7 );\n";
	}
	if ($op_destination == 18) { # @A+DPTR
	    print INST_IMP "unsigned int destination = ReadI( ReadD( _ACC_ ) + ReadD( _DPTRLOW_ ) + ( ReadD( _DPTRHIGH_ ) << 8 ) );\n";
	}
# Mis en commentaire car donnait un warning (destination et source unused variables...)
#	if ($op_destination == 20) { # AB
#	    print INST_IMP "unsigned char destination = ReadD( _ACC_ );\n";
#	    print INST_IMP "unsigned char source = ReadD( _B_ );\n";
#	}
	if ($op_destination == 21) { # DPTR
	    print INST_IMP "unsigned int destination = ( ReadD( _DPTRHIGH_ ) << 8 ) + ReadD( _DPTRLOW_ );\n";
	}
	if ($op_destination == 22) { # #data16
	    print INST_IMP "unsigned char destination = ( PGMMem->Read8( PC++ ) << 8 );\n";
	    print INST_IMP "destination += PGMMem->Read8( PC++ );\n";
	}
	if ($op_destination == 23) { # /bitaddr
	    print INST_IMP "unsigned char destination, dstbitaddr = PGMMem->Read8( PC++ );\n";
	    print INST_IMP "destination = ( ReadB( dstbitaddr ) ^ 1 );\n";
	}
	if ($op_destination == 24) { # @DPTR
	    print INST_IMP "unsigned char destination = ReadI( ( ReadD( _DPTRHIGH_ ) << 8 ) + ReadD( _DPTRLOW_) );\n";
	}
    }

    if ($instargs[$i*4] > 1) {
	$op_source=$instargs[$i*4+2];
	if ($op_source == 0) { # addr11
	    print INST_IMP "unsigned int addr11 = ( ( PGMMem->Read8( PC - 1 ) << 3 ) & 0xF00 ) + PGMMem->Read8( PC++ );\n";
	}
	if ($op_source == 1) { # addr16
	    print INST_IMP "unsigned int addr16 = ( PGMMem->Read8( PC++ ) << 8 );\n";
            print INST_IMP "addr16 += PGMMem->Read8( PC++ );\n";
	}
	if ($op_source == 2) { # A
	    print INST_IMP "unsigned char source = ReadD( _ACC_ );\n";
	}
	if ($op_source == 3) { # direct
	    print INST_IMP "unsigned char srcaddr = PGMMem->Read8( PC++ );\n";
	    print INST_IMP "unsigned char source = ReadD( srcaddr );\n";
	}
	if ($op_source == 4) { # @R0
	    print INST_IMP "unsigned char source = ReadI ( ReadD( BANKPSW + _R0_ ) );\n";
	}
	if ($op_source == 5) { # @R1
	    print INST_IMP "unsigned char source = ReadI ( ReadD( BANKPSW + _R1_ ) );\n";
	}
	if ($op_source == 6) { # R0
	    print INST_IMP "unsigned char source = ReadD( BANKPSW + _R0_ );\n";
	}
	if ($op_source == 7) { # R1
	    print INST_IMP "unsigned char source = ReadD( BANKPSW + _R1_ );\n";
	}
	if ($op_source == 8) { # R2
	    print INST_IMP "unsigned char source = ReadD( BANKPSW + _R2_ );\n";
	}
	if ($op_source == 9) { # R3
	    print INST_IMP "unsigned char source = ReadD( BANKPSW + _R3_ );\n";
	}
	if ($op_source == 10) { # R4
	    print INST_IMP "unsigned char source = ReadD( BANKPSW + _R4_ );\n";
	}
	if ($op_source == 11) { # R5
	    print INST_IMP "unsigned char source = ReadD( BANKPSW + _R5_ );\n";
	}
	if ($op_source == 12) { # R6
	    print INST_IMP "unsigned char source = ReadD( BANKPSW + _R6_ );\n";
	}
	if ($op_source == 13) { # R7
	    print INST_IMP "unsigned char source = ReadD( BANKPSW + _R7_ );\n";
	}

	if ($op_source == 14) { # bitaddr
	    print INST_IMP "unsigned char source, srcbitaddr = PGMMem->Read8( PC++ );\n";
	    print INST_IMP "source = ReadB( srcbitaddr );\n";
	}
	if ($op_source == 15) { # reladdr
	    print INST_IMP "PC++;\n";
	    print INST_IMP "unsigned int source = ( ( PGMMem->Read8( PC - 1 ) + PC ) & 0xFF ) + ( PC & 0xFF00 );\n";
	}
	if ($op_source == 16) { # #data
	    print INST_IMP "unsigned char source = PGMMem->Read8( PC++ );\n";
	}
	if ($op_source == 17) { # C
	    print INST_IMP "unsigned char source = ( ReadD( _PSW_ ) >> 7 );\n";
	}
	if ($op_source == 18) { # @A+DPTR
	    print INST_IMP "unsigned char source = PGMMem->Read8( ReadD( _ACC_ ) + ReadD( _DPTRLOW_ ) + ( ReadD( _DPTRHIGH_ ) << 8 ) );\n";
	}
	if ($op_source == 19) { # @A+PC
	    print INST_IMP "unsigned char source = PGMMem->Read8( ReadD( _ACC_ ) + ( ++PC ) );\n";
	}
	if ($op_source == 21) { # DPTR
	    print INST_IMP "unsigned int source = ( ReadD( _DPTRHIGH_ ) << 8 ) + ReadD( _DPTRLOW_ );\n";
	}
	if ($op_source == 22) { # #data16
	    print INST_IMP "unsigned char source = ( PGMMem->Read8( PC++ ) << 8 );\n";
	    print INST_IMP "source += PGMMem->Read8( PC++ );\n";
	}
	if ($op_source == 23) { # /bitaddr
	    print INST_IMP "unsigned char source, srcbitaddr = PGMMem->Read8( PC++ );\n";
	    print INST_IMP "source = ( ReadB( srcbitaddr ) ^ 1 );\n";
	}
	if ($op_source == 24) { # @DPTR
	    print INST_IMP "unsigned char source = ReadI( ( ReadD( _DPTRHIGH_ ) << 8 ) + ReadD( _DPTRLOW_) );\n";
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
	print INST_IMP "if ( destination == 1 ) { PC = source; destination = 0; }\n";
    }

    # ACALL
    if ($insttype[$i] == 6) {
	print INST_IMP "unsigned char SP = ReadD( _SP_ );\n";
	print INST_IMP "WriteI( ++SP, ( PC & 0x00FF ) );\n";
	print INST_IMP "WriteI( ++SP, ( PC >> 8 ) );\n";
	print INST_IMP "WriteD( _SP_, SP );\n";
   }

    # LCALL
    if ($insttype[$i] == 7) {
	print INST_IMP "unsigned char SP = ReadD( _SP_ );\n";
	print INST_IMP "WriteI( ++SP, ( PC & 0x00FF ) );\n";
	print INST_IMP "WriteI( ++SP, ( PC >> 8 ) );\n";
	print INST_IMP "WriteD( _SP_, SP );\n";
   }

    # RRC
    if ($insttype[$i] == 8) {
	print INST_IMP "unsigned char tmpval = destination;\n";
       	print INST_IMP "destination = ( destination >> 1 ) | ( ReadD( _PSW_ ) & 0x80 );\n";
	print INST_IMP "WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x7F ) | ( tmpval << 7 ) );\n";
    }

    # DEC
    if ($insttype[$i] == 9) {
	print INST_IMP "destination--;\n";
    }

    # JB
    if ($insttype[$i] == 10) {
	print INST_IMP "if ( destination == 1 ) { PC = source; }\n";
    }

    # RET
    if ($insttype[$i] == 11) {
	print INST_IMP "unsigned char SP = ReadD( _SP_ );\n";
	print INST_IMP "PC = ( ReadI( SP-- ) << 8 );\n";
	print INST_IMP "PC += ReadI ( SP-- );\n";
	print INST_IMP "WriteD( _SP_, SP );\n";
    }

    # RL
    if ($insttype[$i] == 12) {
	print INST_IMP "destination = ( destination << 1 ) | ( destination >> 7 );\n";
    }

    # ADD
    if ($insttype[$i] == 13) {
	print INST_IMP "WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );\n";
	print INST_IMP "if ( destination + source > 0xFF ) {\n";
	print INST_IMP "   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );\n";
	print INST_IMP "   if ( ( destination & 0x7F ) + ( source & 0x7F ) < 0x80 )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );\n";
	print INST_IMP "} else if ( ( destination & 0x7F ) + ( source & 0x7F ) > 0x7F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );\n";
	print INST_IMP "if ( ( destination & 0x0F ) + ( source & 0x0F ) > 0x0F )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );\n";
	print INST_IMP "destination += source;\n";
    }

    # JNB
    if ($insttype[$i] == 14) {
	print INST_IMP "if ( destination == 0 ) { PC = source; }\n";
    }

    # RETI
    if ($insttype[$i] == 15) {
	print INST_IMP "ActivePriority = -1;\n";
	print INST_IMP "unsigned char SP = ReadD( _SP_ );\n";
	print INST_IMP "PC = ( ReadI( SP-- ) << 8 );\n";
	print INST_IMP "PC += ReadI( SP-- );\n";
    }

    # RLC
    if ($insttype[$i] == 16) {
	print INST_IMP "unsigned char tmpval = destination;\n";
       	print INST_IMP "destination = ( destination << 1 ) | ( ( ReadD( _PSW_ ) & 0x80 ) >> 7 );\n";
	print INST_IMP "WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x7F ) | ( tmpval & 0x80 ) );\n";
    }

    # ADDC
    if ($insttype[$i] == 17) {
	print INST_IMP "unsigned char carryflag = ( ReadD( _PSW_ ) >> 7 );\n";
	print INST_IMP "WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );\n";
	print INST_IMP "if ( destination + source + carryflag > 0xFF ) {\n";
	print INST_IMP "   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );\n";
	print INST_IMP "   if ( ( destination & 0x7F ) + ( source & 0x7F ) + carryflag < 0x80 )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );\n";
	print INST_IMP "} else if ( ( destination & 0x7F ) + ( source & 0x7F ) + carryflag > 0x7F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );\n";
	print INST_IMP "if ( ( destination & 0x0F ) + ( source & 0x0F ) + carryflag > 0x0F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x40 ) );\n";
	print INST_IMP "destination += source;\n";
    }

    # JC
    if ($insttype[$i] == 18) {
	print INST_IMP "if ( ReadD( _PSW_ ) > 0x7F) { PC = destination; }\n";
    }

    # ORL
    if ($insttype[$i] == 19) {
	if ($instargs[$i*4+1] == 17) {
	    # sur des bits
	    print INST_IMP "WriteD( _PSW_ , ( ( destination | source ) << 7 ) );\n";
	} else {
	    # sur des bytes
	    print INST_IMP "destination |= source;\n";
	}
    }

    # JNC
    if ($insttype[$i] == 20) {
	print INST_IMP "if ( ReadD( _PSW_ ) < 0x80 ) { PC = destination; }\n";
    }

    # ANL
    if ($insttype[$i] == 21) {
	if ($instargs[$i*4+1] == 17) {
	    # sur des bits
	    print INST_IMP "WriteD( _PSW_, ( ( destination & source) << 7 ) );\n";
	} else {
	    # sur des bytes
	    print INST_IMP "destination &= source;\n";
	}
    }

    # JZ
    if ($insttype[$i] == 22) {
	print INST_IMP "if ( ReadD( _ACC_ ) == 0 ) { PC = destination; }\n";
    }

    # XRL
    if ($insttype[$i] == 23) {
	    print INST_IMP "destination ^= source;\n";
    }

    # JNZ
    if ($insttype[$i] == 24) {
	print INST_IMP "if ( ReadD( _ACC_ ) != 0 ) { PC = destination; }\n";
    }

    # JMP
    if ($insttype[$i] == 25) {
	print INST_IMP "PC = destination;\n";
    }

    # MOV
    if ($insttype[$i] == 26) {
	print INST_IMP "destination = source;\n";
    }

    # SJMP
    if ($insttype[$i] == 27) {
	print INST_IMP "PC = destination;\n";
    }

    # MOVC
    if ($insttype[$i] == 28) {
	print INST_IMP "destination = source;\n";
    }

    # DIV
    if ($insttype[$i] == 29) {
	print INST_IMP "unsigned char A = ReadD( _ACC_ ), B = ReadD( _B_ );\n";
	print INST_IMP "WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x7B ) );\n";
	print INST_IMP "if ( B != 0 ) {\n";
	print INST_IMP "WriteD( _ACC_, A/B ); WriteD( _B_, A%B );\n";
	print INST_IMP "} else WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );\n";
    }

    # SUBB
    if ($insttype[$i] == 30) {
	print INST_IMP "unsigned char carryflag = ReadD( _PSW_ ) >> 7;\n";
	print INST_IMP "WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );\n";
	print INST_IMP "if ( destination < ( source + carryflag ) ) {\n";
	print INST_IMP "  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );\n";
	print INST_IMP "  if ( ( destination & 0x7F ) > ( ( source + carryflag ) & 0x7F ) )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );\n";
	print INST_IMP "} else if ( ( destination & 0x7F ) < ( ( source + carryflag ) & 0x7F ) )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );\n";
	print INST_IMP "if ( ( destination & 0x0F ) < ( ( source + carryflag ) & 0x0F ) )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x40 ) );\n";
	print INST_IMP "destination -= source + carryflag;\n";
    }

    # MUL
    if ($insttype[$i] == 31) {
	print INST_IMP "unsigned char A = ReadD( _ACC_ ), B = ReadD( _B_ );\n";
	print INST_IMP "WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x7B ) );\n";
	print INST_IMP "WriteD( _ACC_ , ( ( A * B ) & 0x00FF ) ); WriteD( _B_, ( A * B ) / 0x100 );\n";
	print INST_IMP "if ( ReadD( _B_ ) > 0) WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );\n";
    }

    # CPL
    if ($insttype[$i] == 33) {
	if ($instargs[$i*4+1] == 2) { print INST_IMP "destination ^= 0xFF;\n"; }
	else { print INST_IMP "destination ^= 0x01;\n"; }
    }

    # CJNE
    if ($insttype[$i] == 34) {
	print INST_IMP "unsigned int reladdr = ( ( PGMMem->Read8( PC ) + ( ( PC + 1 ) & 0x00FF ) ) & 0x00FF ) + ( ( PC + 1 ) & 0xFF00 );\n";
	print INST_IMP "WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x7F ) );\n";
	print INST_IMP "if ( destination < source ) WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );\n";
	print INST_IMP "if ( destination != source ) PC = reladdr;\n";
    }

    # PUSH
    if ($insttype[$i] == 35) {
	print INST_IMP "unsigned char SP = ReadD( _SP_ );\n";
	print INST_IMP "WriteI( ++SP, destination );\n";
	print INST_IMP "WriteD( _SP_, SP );\n";
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
	print INST_IMP "unsigned char SP = ReadD( _SP_ );\n";
	print INST_IMP "destination = ReadI( SP-- );\n";
	print INST_IMP "WriteD( _SP_, SP );\n";
    }

    # SETB
    if ($insttype[$i] == 40) {
	print INST_IMP "destination = 1;\n";
    }

    # DA
    if ($insttype[$i] == 41) {
	print INST_IMP "if ( ( ( destination & 0x0F ) > 9) || ( ReadD( _PSW_ ) | 0x40)) {\n";
	print INST_IMP "   if ( ( destination + 6 ) > 0xFF)  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );\n";
	print INST_IMP "   destination += 6;\n";
	print INST_IMP "}\n";
	print INST_IMP "if ( ( ReadD( _PSW_ ) & 0x80) || ( ( destination & 0xF0 ) > 0x90 ) ) {\n";
	print INST_IMP "   if ( ( destination + 0x60 ) > 0xFF ) WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );\n";
	print INST_IMP "   destination += 0x60;\n";
	print INST_IMP "}\n";
    }

    # DJNZ
    if ($insttype[$i] == 42) {
	print INST_IMP "destination--;\n";
	print INST_IMP "if ( destination != 0 ) PC = source;\n";
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
	    print INST_IMP "PC = ( PC & 0xF800 ) | addr11;\n";
	}
	if ($op_destination == 1) { # addr16
	    print INST_IMP "PC = addr16;\n";
	}
	if ($op_destination == 2) { # A
	    print INST_IMP "WriteD( _ACC_, destination );\n";
	}
	if ($op_destination == 3) { # direct
	    print INST_IMP "WriteD( destaddr, destination );\n";
	}
	if ($op_destination == 4) { # @R0
	    print INST_IMP "WriteI( ReadD( BANKPSW + _R0_ ), destination );\n";
	}
	if ($op_destination == 5) { # @R1
	    print INST_IMP "WriteI( ReadD( BANKPSW + _R1_ ), destination );\n";
	}
	if ($op_destination == 6) { # R0
	    print INST_IMP "WriteD( BANKPSW + _R0_, destination );\n";
	}
	if ($op_destination == 7) { # R1
	    print INST_IMP "WriteD( BANKPSW + _R1_, destination );\n";
	}
	if ($op_destination == 8) { # R2
	    print INST_IMP "WriteD( BANKPSW + _R2_, destination );\n";
	}
	if ($op_destination == 9) { # R3
	    print INST_IMP "WriteD( BANKPSW + _R3_, destination );\n";
	}
	if ($op_destination == 10) { # R4
	    print INST_IMP "WriteD( BANKPSW + _R4_, destination );\n";
	}
	if ($op_destination == 11) { # R5
	    print INST_IMP "WriteD( BANKPSW + _R5_, destination );\n";
	}
	if ($op_destination == 12) { # R6
	    print INST_IMP "WriteD( BANKPSW + _R6_, destination );\n";
	}
	if ($op_destination == 13) { # R7
	    print INST_IMP "WriteD( BANKPSW + _R7_, destination );\n";
	}

	if ($op_destination == 14) { # bitaddr
	    print INST_IMP "WriteB( dstbitaddr, destination );\n";
	}
	if ($op_destination == 17) { # C
	    print INST_IMP "WriteD( _PSW_, ( ( ReadD( _PSW_ ) & 0x7F) | ( destination << 7 ) ) );\n";
	}
	if ($op_destination == 21) { # DPTR
	    print INST_IMP "WriteD( _DPTRHIGH_, ( destination >> 8 ) );\n";
	    print INST_IMP "WriteD( _DPTRLOW_, ( destination & 0xFF ) );\n";
	}
	if ($op_destination == 23) { # /bitaddr
	    print INST_IMP "WriteB( dstbitaddr, destination );\n";
	}
	if ($op_destination == 24) { # @DPTR
	    print INST_IMP "WriteI( ( ReadD( _DPTRHIGH_ ) << 8 ) + ReadD( _DPTRLOW_ ), destination );\n";
	}
    }

    if ($modifysrc == 1) {
    if ($instargs[$i*4] > 1) {
	$op_source=$instargs[$i*4+2];
	if ($op_source == 0) { # addr11
	    print INST_IMP "PC = ( PC & 0xF800 ) | addr11;\n";
	}
	if ($op_source == 1) { # addr16
	    print INST_IMP "PC = addr16;\n";
	}
	if ($op_source == 2) { # A
	    print INST_IMP "WriteD( _ACC_, source );\n";
	}
	if ($op_source == 3) { # direct
	    print INST_IMP "WriteD( srcaddr, source );\n";
	}
	if ($op_source == 4) { # @R0
	    print INST_IMP "WriteI( ReadD( BANKPSW + _R0_ ), source );\n";
	}
	if ($op_source == 5) { # @R1
	    print INST_IMP "WriteI( ReadD( BANKPSW + _R1_ ), source );\n";
	}
	if ($op_source == 6) { # R0
	    print INST_IMP "WriteD( BANKPSW + _R0_, source );\n";
	}
	if ($op_source == 7) { # R1
	    print INST_IMP "WriteD( BANKPSW + _R1_, source );\n";
	}
	if ($op_source == 8) { # R2
	    print INST_IMP "WriteD( BANKPSW + _R2_, source );\n";
	}
	if ($op_source == 9) { # R3
	    print INST_IMP "WriteD( BANKPSW + _R3_, source );\n";
	}
	if ($op_source == 10) { # R4
	    print INST_IMP "WriteD( BANKPSW + _R4_, source );\n";
	}
	if ($op_source == 11) { # R5
	    print INST_IMP "WriteD( BANKPSW + _R5_, source );\n";
	}
	if ($op_source == 12) { # R6
	    print INST_IMP "WriteD( BANKPSW + _R6_, source );\n";
	}
	if ($op_source == 13) { # R7
	    print INST_IMP "WriteD( BANKPSW + _R7_, source );\n";
	}
	if ($op_source == 14) { # bitaddr
	    print INST_IMP "WriteB( srcbitaddr, source );\n";
	}
	if ($op_source == 17) { # C
	    print INST_IMP "WriteD( _PSW_, ( ( ReadD( _PSW_ ) & 0x7F) | ( source << 7 ) ) );\n";
	}
	if ($op_source == 21) { # DPTR
	    print INST_IMP "WriteD( _DPTRHIGH_, ( source >> 8 ) );\n";
	    print INST_IMP "WriteD( _DPTRLOW_, ( source & 0xFF ) );\n";
	}
	if ($op_source == 23) { # /bitaddr
	    print INST_IMP "WriteB( srcbitaddr, source );\n";
	}
	if ($op_source == 24) { # @DPTR
	    print INST_IMP "WriteI( ( ReadD( _DPTRHIGH_ ) << 8 ) + ReadD( _DPTRLOW_ ), source );\n";
	}
    }
}
}
    print INST_IMP "return $a_cycles[$i];\n";
    print INST_IMP "}\n";
    print INST_IMP "\n\n";
}
# ------------------------------------------------------------------------------
print INST_IMP "\n\n";

    print INST_IMP "/"x78,"\n";
    print INST_IMP "// void CPU8051::InitFuncPtr( )\n";
    print INST_IMP "// Initialize Functions Pointers\n";
    print INST_IMP "/"x78,"\n";
    print INST_IMP "void CPU8051::InitFuncPtr( )\n";
    print INST_DEF "void InitFuncPtr( );\n";
    print INST_IMP "{\n";


#print INST_IMP "static int (*instfnc[])() = {\n";
for ($i=0;$i<256;$i++) {
    $ifunc=substr($instfunction[$i], 9);
    print INST_IMP " funcptr[$i]=&CPU8051::$ifunc;\n";
#    ($i < 255) and print INST_IMP ",\n";
#    (($i+1) % 4 == 0) and print INST_IMP "\n";
}
print INST_IMP "\n}\n";

print INST_IMP "\n\n#endif\n";
print INST_DEF "\n\n#endif\n";
print DISASM_HPP "\n\n#endif\n";

 
close DISASM_HPP;
close OPCODELST;
close INST_DEF;
close INST_IMP;


