#ifndef __INST_IMP_HPP_
#define __INST_IMP_HPP_

//#include "CPU8051.hpp"

// Do not modify this file directly, it was created by Opcode2cpp.pl
// Any modification made directly on this file will be lost


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_00( )
// Instruction "NOP" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_00( )
{
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_01( )
// Instruction "AJMP addr11" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_01( )
{
unsigned int addr11 = ( ( PGMMem->Read8( PC - 1 ) << 3 ) & 0xF00 ) + PGMMem->Read8( PC++ );
PC = ( PC & 0xF800 ) | addr11;
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_02( )
// Instruction "LJMP addr16" takes 2 cycle(s) and 3 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_02( )
{
unsigned int addr16 = ( PGMMem->Read8( PC++ ) << 8 );
addr16 += PGMMem->Read8( PC++ );
PC = addr16;
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_03( )
// Instruction "RR A" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_03( )
{
unsigned char destination = ReadD( _ACC_ );
destination = ( destination >> 1 ) | ( destination << 7 );
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_04( )
// Instruction "INC A" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_04( )
{
unsigned char destination = ReadD( _ACC_ );
destination++;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_05( )
// Instruction "INC direct" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_05( )
{
unsigned char destaddr = PGMMem->Read8( PC++ );
unsigned char destination = ReadD( destaddr );
destination++;
WriteD( destaddr, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_06( )
// Instruction "INC @R0" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_06( )
{
unsigned char destination = ReadI ( ReadD( BANKPSW + _R0_ ) );
destination++;
WriteI( ReadD( BANKPSW + _R0_ ), destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_07( )
// Instruction "INC @R1" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_07( )
{
unsigned char destination = ReadI ( ReadD( BANKPSW + _R1_ ) );
destination++;
WriteI( ReadD( BANKPSW + _R1_ ), destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_08( )
// Instruction "INC R0" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_08( )
{
unsigned char destination = ReadD( BANKPSW + _R0_ );
destination++;
WriteD( BANKPSW + _R0_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_09( )
// Instruction "INC R1" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_09( )
{
unsigned char destination = ReadD( BANKPSW + _R1_ );
destination++;
WriteD( BANKPSW + _R1_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_0A( )
// Instruction "INC R2" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_0A( )
{
unsigned char destination = ReadD( BANKPSW + _R2_ );
destination++;
WriteD( BANKPSW + _R2_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_0B( )
// Instruction "INC R3" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_0B( )
{
unsigned char destination = ReadD( BANKPSW + _R3_ );
destination++;
WriteD( BANKPSW + _R3_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_0C( )
// Instruction "INC R4" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_0C( )
{
unsigned char destination = ReadD( BANKPSW + _R4_ );
destination++;
WriteD( BANKPSW + _R4_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_0D( )
// Instruction "INC R5" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_0D( )
{
unsigned char destination = ReadD( BANKPSW + _R5_ );
destination++;
WriteD( BANKPSW + _R5_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_0E( )
// Instruction "INC R6" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_0E( )
{
unsigned char destination = ReadD( BANKPSW + _R6_ );
destination++;
WriteD( BANKPSW + _R6_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_0F( )
// Instruction "INC R7" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_0F( )
{
unsigned char destination = ReadD( BANKPSW + _R7_ );
destination++;
WriteD( BANKPSW + _R7_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_10( )
// Instruction "JBC bitaddr,reladdr" takes 2 cycle(s) and 3 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_10( )
{
unsigned char destination, dstbitaddr = PGMMem->Read8( PC++ );
destination = ReadB( dstbitaddr );
PC++;
unsigned int source = ( ( PGMMem->Read8( PC - 1 ) + PC ) & 0xFF ) + ( PC & 0xFF00 );
if ( destination == 1 ) { PC = source; destination = 0; }
WriteB( dstbitaddr, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_11( )
// Instruction "ACALL addr11" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_11( )
{
unsigned int addr11 = ( ( PGMMem->Read8( PC - 1 ) << 3 ) & 0xF00 ) + PGMMem->Read8( PC++ );
unsigned char SP = ReadD( _SP_ );
WriteI( ++SP, ( PC & 0x00FF ) );
WriteI( ++SP, ( PC >> 8 ) );
WriteD( _SP_, SP );
PC = ( PC & 0xF800 ) | addr11;
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_12( )
// Instruction "LCALL addr16" takes 2 cycle(s) and 3 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_12( )
{
unsigned int addr16 = ( PGMMem->Read8( PC++ ) << 8 );
addr16 += PGMMem->Read8( PC++ );
unsigned char SP = ReadD( _SP_ );
WriteI( ++SP, ( PC & 0x00FF ) );
WriteI( ++SP, ( PC >> 8 ) );
WriteD( _SP_, SP );
PC = addr16;
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_13( )
// Instruction "RRC A" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_13( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char tmpval = destination;
destination = ( destination >> 1 ) | ( ReadD( _PSW_ ) & 0x80 );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x7F ) | ( tmpval << 7 ) );
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_14( )
// Instruction "DEC A" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_14( )
{
unsigned char destination = ReadD( _ACC_ );
destination--;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_15( )
// Instruction "DEC direct" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_15( )
{
unsigned char destaddr = PGMMem->Read8( PC++ );
unsigned char destination = ReadD( destaddr );
destination--;
WriteD( destaddr, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_16( )
// Instruction "DEC @R0" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_16( )
{
unsigned char destination = ReadI ( ReadD( BANKPSW + _R0_ ) );
destination--;
WriteI( ReadD( BANKPSW + _R0_ ), destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_17( )
// Instruction "DEC @R1" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_17( )
{
unsigned char destination = ReadI ( ReadD( BANKPSW + _R1_ ) );
destination--;
WriteI( ReadD( BANKPSW + _R1_ ), destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_18( )
// Instruction "DEC R0" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_18( )
{
unsigned char destination = ReadD( BANKPSW + _R0_ );
destination--;
WriteD( BANKPSW + _R0_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_19( )
// Instruction "DEC R1" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_19( )
{
unsigned char destination = ReadD( BANKPSW + _R1_ );
destination--;
WriteD( BANKPSW + _R1_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_1A( )
// Instruction "DEC R2" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_1A( )
{
unsigned char destination = ReadD( BANKPSW + _R2_ );
destination--;
WriteD( BANKPSW + _R2_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_1B( )
// Instruction "DEC R3" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_1B( )
{
unsigned char destination = ReadD( BANKPSW + _R3_ );
destination--;
WriteD( BANKPSW + _R3_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_1C( )
// Instruction "DEC R4" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_1C( )
{
unsigned char destination = ReadD( BANKPSW + _R4_ );
destination--;
WriteD( BANKPSW + _R4_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_1D( )
// Instruction "DEC R5" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_1D( )
{
unsigned char destination = ReadD( BANKPSW + _R5_ );
destination--;
WriteD( BANKPSW + _R5_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_1E( )
// Instruction "DEC R6" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_1E( )
{
unsigned char destination = ReadD( BANKPSW + _R6_ );
destination--;
WriteD( BANKPSW + _R6_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_1F( )
// Instruction "DEC R7" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_1F( )
{
unsigned char destination = ReadD( BANKPSW + _R7_ );
destination--;
WriteD( BANKPSW + _R7_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_20( )
// Instruction "JB bitaddr,reladdr" takes 2 cycle(s) and 3 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_20( )
{
unsigned char destination, dstbitaddr = PGMMem->Read8( PC++ );
destination = ReadB( dstbitaddr );
PC++;
unsigned int source = ( ( PGMMem->Read8( PC - 1 ) + PC ) & 0xFF ) + ( PC & 0xFF00 );
if ( destination == 1 ) { PC = source; }
WriteB( dstbitaddr, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_21( )
// Instruction "AJMP addr11" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_21( )
{
unsigned int addr11 = ( ( PGMMem->Read8( PC - 1 ) << 3 ) & 0xF00 ) + PGMMem->Read8( PC++ );
PC = ( PC & 0xF800 ) | addr11;
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_22( )
// Instruction "RET" takes 2 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_22( )
{
unsigned char SP = ReadD( _SP_ );
PC = ( ReadI( SP-- ) << 8 );
PC += ReadI ( SP-- );
WriteD( _SP_, SP );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_23( )
// Instruction "RL A" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_23( )
{
unsigned char destination = ReadD( _ACC_ );
destination = ( destination << 1 ) | ( destination >> 7 );
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_24( )
// Instruction "ADD A,#data" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_24( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = PGMMem->Read8( PC++ );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );
if ( destination + source > 0xFF ) {
   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
   if ( ( destination & 0x7F ) + ( source & 0x7F ) < 0x80 )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
} else if ( ( destination & 0x7F ) + ( source & 0x7F ) > 0x7F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
if ( ( destination & 0x0F ) + ( source & 0x0F ) > 0x0F )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
destination += source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_25( )
// Instruction "ADD A,direct" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_25( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char srcaddr = PGMMem->Read8( PC++ );
unsigned char source = ReadD( srcaddr );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );
if ( destination + source > 0xFF ) {
   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
   if ( ( destination & 0x7F ) + ( source & 0x7F ) < 0x80 )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
} else if ( ( destination & 0x7F ) + ( source & 0x7F ) > 0x7F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
if ( ( destination & 0x0F ) + ( source & 0x0F ) > 0x0F )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
destination += source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_26( )
// Instruction "ADD A,@R0" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_26( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadI ( ReadD( BANKPSW + _R0_ ) );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );
if ( destination + source > 0xFF ) {
   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
   if ( ( destination & 0x7F ) + ( source & 0x7F ) < 0x80 )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
} else if ( ( destination & 0x7F ) + ( source & 0x7F ) > 0x7F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
if ( ( destination & 0x0F ) + ( source & 0x0F ) > 0x0F )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
destination += source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_27( )
// Instruction "ADD A,@R1" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_27( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadI ( ReadD( BANKPSW + _R1_ ) );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );
if ( destination + source > 0xFF ) {
   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
   if ( ( destination & 0x7F ) + ( source & 0x7F ) < 0x80 )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
} else if ( ( destination & 0x7F ) + ( source & 0x7F ) > 0x7F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
if ( ( destination & 0x0F ) + ( source & 0x0F ) > 0x0F )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
destination += source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_28( )
// Instruction "ADD A,R0" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_28( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R0_ );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );
if ( destination + source > 0xFF ) {
   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
   if ( ( destination & 0x7F ) + ( source & 0x7F ) < 0x80 )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
} else if ( ( destination & 0x7F ) + ( source & 0x7F ) > 0x7F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
if ( ( destination & 0x0F ) + ( source & 0x0F ) > 0x0F )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
destination += source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_29( )
// Instruction "ADD A,R1" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_29( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R1_ );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );
if ( destination + source > 0xFF ) {
   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
   if ( ( destination & 0x7F ) + ( source & 0x7F ) < 0x80 )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
} else if ( ( destination & 0x7F ) + ( source & 0x7F ) > 0x7F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
if ( ( destination & 0x0F ) + ( source & 0x0F ) > 0x0F )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
destination += source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_2A( )
// Instruction "ADD A,R2" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_2A( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R2_ );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );
if ( destination + source > 0xFF ) {
   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
   if ( ( destination & 0x7F ) + ( source & 0x7F ) < 0x80 )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
} else if ( ( destination & 0x7F ) + ( source & 0x7F ) > 0x7F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
if ( ( destination & 0x0F ) + ( source & 0x0F ) > 0x0F )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
destination += source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_2B( )
// Instruction "ADD A,R3" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_2B( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R3_ );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );
if ( destination + source > 0xFF ) {
   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
   if ( ( destination & 0x7F ) + ( source & 0x7F ) < 0x80 )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
} else if ( ( destination & 0x7F ) + ( source & 0x7F ) > 0x7F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
if ( ( destination & 0x0F ) + ( source & 0x0F ) > 0x0F )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
destination += source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_2C( )
// Instruction "ADD A,R4" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_2C( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R4_ );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );
if ( destination + source > 0xFF ) {
   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
   if ( ( destination & 0x7F ) + ( source & 0x7F ) < 0x80 )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
} else if ( ( destination & 0x7F ) + ( source & 0x7F ) > 0x7F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
if ( ( destination & 0x0F ) + ( source & 0x0F ) > 0x0F )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
destination += source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_2D( )
// Instruction "ADD A,R5" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_2D( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R5_ );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );
if ( destination + source > 0xFF ) {
   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
   if ( ( destination & 0x7F ) + ( source & 0x7F ) < 0x80 )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
} else if ( ( destination & 0x7F ) + ( source & 0x7F ) > 0x7F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
if ( ( destination & 0x0F ) + ( source & 0x0F ) > 0x0F )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
destination += source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_2E( )
// Instruction "ADD A,R6" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_2E( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R6_ );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );
if ( destination + source > 0xFF ) {
   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
   if ( ( destination & 0x7F ) + ( source & 0x7F ) < 0x80 )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
} else if ( ( destination & 0x7F ) + ( source & 0x7F ) > 0x7F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
if ( ( destination & 0x0F ) + ( source & 0x0F ) > 0x0F )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
destination += source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_2F( )
// Instruction "ADD A,R7" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_2F( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R7_ );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );
if ( destination + source > 0xFF ) {
   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
   if ( ( destination & 0x7F ) + ( source & 0x7F ) < 0x80 )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
} else if ( ( destination & 0x7F ) + ( source & 0x7F ) > 0x7F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
if ( ( destination & 0x0F ) + ( source & 0x0F ) > 0x0F )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
destination += source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_30( )
// Instruction "JNB bitaddr,reladdr" takes 2 cycle(s) and 3 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_30( )
{
unsigned char destination, dstbitaddr = PGMMem->Read8( PC++ );
destination = ReadB( dstbitaddr );
PC++;
unsigned int source = ( ( PGMMem->Read8( PC - 1 ) + PC ) & 0xFF ) + ( PC & 0xFF00 );
if ( destination == 0 ) { PC = source; }
WriteB( dstbitaddr, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_31( )
// Instruction "ACALL addr11" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_31( )
{
unsigned int addr11 = ( ( PGMMem->Read8( PC - 1 ) << 3 ) & 0xF00 ) + PGMMem->Read8( PC++ );
unsigned char SP = ReadD( _SP_ );
WriteI( ++SP, ( PC & 0x00FF ) );
WriteI( ++SP, ( PC >> 8 ) );
WriteD( _SP_, SP );
PC = ( PC & 0xF800 ) | addr11;
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_32( )
// Instruction "RETI" takes 2 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_32( )
{
ActivePriority = -1;
unsigned char SP = ReadD( _SP_ );
PC = ( ReadI( SP-- ) << 8 );
PC += ReadI( SP-- );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_33( )
// Instruction "RLC A" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_33( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char tmpval = destination;
destination = ( destination << 1 ) | ( ( ReadD( _PSW_ ) & 0x80 ) >> 7 );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x7F ) | ( tmpval & 0x80 ) );
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_34( )
// Instruction "ADDC A,#data" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_34( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = PGMMem->Read8( PC++ );
unsigned char carryflag = ( ReadD( _PSW_ ) >> 7 );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );
if ( destination + source + carryflag > 0xFF ) {
   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
   if ( ( destination & 0x7F ) + ( source & 0x7F ) + carryflag < 0x80 )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
} else if ( ( destination & 0x7F ) + ( source & 0x7F ) + carryflag > 0x7F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
if ( ( destination & 0x0F ) + ( source & 0x0F ) + carryflag > 0x0F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x40 ) );
destination += source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_35( )
// Instruction "ADDC A,direct" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_35( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char srcaddr = PGMMem->Read8( PC++ );
unsigned char source = ReadD( srcaddr );
unsigned char carryflag = ( ReadD( _PSW_ ) >> 7 );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );
if ( destination + source + carryflag > 0xFF ) {
   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
   if ( ( destination & 0x7F ) + ( source & 0x7F ) + carryflag < 0x80 )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
} else if ( ( destination & 0x7F ) + ( source & 0x7F ) + carryflag > 0x7F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
if ( ( destination & 0x0F ) + ( source & 0x0F ) + carryflag > 0x0F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x40 ) );
destination += source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_36( )
// Instruction "ADDC A,@R0" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_36( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadI ( ReadD( BANKPSW + _R0_ ) );
unsigned char carryflag = ( ReadD( _PSW_ ) >> 7 );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );
if ( destination + source + carryflag > 0xFF ) {
   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
   if ( ( destination & 0x7F ) + ( source & 0x7F ) + carryflag < 0x80 )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
} else if ( ( destination & 0x7F ) + ( source & 0x7F ) + carryflag > 0x7F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
if ( ( destination & 0x0F ) + ( source & 0x0F ) + carryflag > 0x0F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x40 ) );
destination += source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_37( )
// Instruction "ADDC A,@R1" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_37( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadI ( ReadD( BANKPSW + _R1_ ) );
unsigned char carryflag = ( ReadD( _PSW_ ) >> 7 );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );
if ( destination + source + carryflag > 0xFF ) {
   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
   if ( ( destination & 0x7F ) + ( source & 0x7F ) + carryflag < 0x80 )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
} else if ( ( destination & 0x7F ) + ( source & 0x7F ) + carryflag > 0x7F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
if ( ( destination & 0x0F ) + ( source & 0x0F ) + carryflag > 0x0F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x40 ) );
destination += source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_38( )
// Instruction "ADDC A,R0" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_38( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R0_ );
unsigned char carryflag = ( ReadD( _PSW_ ) >> 7 );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );
if ( destination + source + carryflag > 0xFF ) {
   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
   if ( ( destination & 0x7F ) + ( source & 0x7F ) + carryflag < 0x80 )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
} else if ( ( destination & 0x7F ) + ( source & 0x7F ) + carryflag > 0x7F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
if ( ( destination & 0x0F ) + ( source & 0x0F ) + carryflag > 0x0F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x40 ) );
destination += source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_39( )
// Instruction "ADDC A,R1" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_39( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R1_ );
unsigned char carryflag = ( ReadD( _PSW_ ) >> 7 );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );
if ( destination + source + carryflag > 0xFF ) {
   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
   if ( ( destination & 0x7F ) + ( source & 0x7F ) + carryflag < 0x80 )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
} else if ( ( destination & 0x7F ) + ( source & 0x7F ) + carryflag > 0x7F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
if ( ( destination & 0x0F ) + ( source & 0x0F ) + carryflag > 0x0F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x40 ) );
destination += source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_3A( )
// Instruction "ADDC A,R2" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_3A( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R2_ );
unsigned char carryflag = ( ReadD( _PSW_ ) >> 7 );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );
if ( destination + source + carryflag > 0xFF ) {
   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
   if ( ( destination & 0x7F ) + ( source & 0x7F ) + carryflag < 0x80 )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
} else if ( ( destination & 0x7F ) + ( source & 0x7F ) + carryflag > 0x7F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
if ( ( destination & 0x0F ) + ( source & 0x0F ) + carryflag > 0x0F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x40 ) );
destination += source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_3B( )
// Instruction "ADDC A,R3" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_3B( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R3_ );
unsigned char carryflag = ( ReadD( _PSW_ ) >> 7 );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );
if ( destination + source + carryflag > 0xFF ) {
   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
   if ( ( destination & 0x7F ) + ( source & 0x7F ) + carryflag < 0x80 )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
} else if ( ( destination & 0x7F ) + ( source & 0x7F ) + carryflag > 0x7F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
if ( ( destination & 0x0F ) + ( source & 0x0F ) + carryflag > 0x0F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x40 ) );
destination += source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_3C( )
// Instruction "ADDC A,R4" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_3C( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R4_ );
unsigned char carryflag = ( ReadD( _PSW_ ) >> 7 );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );
if ( destination + source + carryflag > 0xFF ) {
   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
   if ( ( destination & 0x7F ) + ( source & 0x7F ) + carryflag < 0x80 )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
} else if ( ( destination & 0x7F ) + ( source & 0x7F ) + carryflag > 0x7F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
if ( ( destination & 0x0F ) + ( source & 0x0F ) + carryflag > 0x0F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x40 ) );
destination += source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_3D( )
// Instruction "ADDC A,R5" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_3D( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R5_ );
unsigned char carryflag = ( ReadD( _PSW_ ) >> 7 );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );
if ( destination + source + carryflag > 0xFF ) {
   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
   if ( ( destination & 0x7F ) + ( source & 0x7F ) + carryflag < 0x80 )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
} else if ( ( destination & 0x7F ) + ( source & 0x7F ) + carryflag > 0x7F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
if ( ( destination & 0x0F ) + ( source & 0x0F ) + carryflag > 0x0F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x40 ) );
destination += source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_3E( )
// Instruction "ADDC A,R6" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_3E( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R6_ );
unsigned char carryflag = ( ReadD( _PSW_ ) >> 7 );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );
if ( destination + source + carryflag > 0xFF ) {
   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
   if ( ( destination & 0x7F ) + ( source & 0x7F ) + carryflag < 0x80 )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
} else if ( ( destination & 0x7F ) + ( source & 0x7F ) + carryflag > 0x7F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
if ( ( destination & 0x0F ) + ( source & 0x0F ) + carryflag > 0x0F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x40 ) );
destination += source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_3F( )
// Instruction "ADDC A,R7" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_3F( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R7_ );
unsigned char carryflag = ( ReadD( _PSW_ ) >> 7 );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );
if ( destination + source + carryflag > 0xFF ) {
   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
   if ( ( destination & 0x7F ) + ( source & 0x7F ) + carryflag < 0x80 )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
} else if ( ( destination & 0x7F ) + ( source & 0x7F ) + carryflag > 0x7F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
if ( ( destination & 0x0F ) + ( source & 0x0F ) + carryflag > 0x0F )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x40 ) );
destination += source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_40( )
// Instruction "JC reladdr" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_40( )
{
PC++;
unsigned int destination = ( ( PGMMem->Read8( PC - 1 ) + PC ) & 0xFF ) + ( PC & 0xFF00 );
if ( ReadD( _PSW_ ) > 0x7F) { PC = destination; }
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_41( )
// Instruction "AJMP addr11" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_41( )
{
unsigned int addr11 = ( ( PGMMem->Read8( PC - 1 ) << 3 ) & 0xF00 ) + PGMMem->Read8( PC++ );
PC = ( PC & 0xF800 ) | addr11;
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_42( )
// Instruction "ORL direct,A" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_42( )
{
unsigned char destaddr = PGMMem->Read8( PC++ );
unsigned char destination = ReadD( destaddr );
unsigned char source = ReadD( _ACC_ );
destination |= source;
WriteD( destaddr, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_43( )
// Instruction "ORL direct,#data" takes 2 cycle(s) and 3 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_43( )
{
unsigned char destaddr = PGMMem->Read8( PC++ );
unsigned char destination = ReadD( destaddr );
unsigned char source = PGMMem->Read8( PC++ );
destination |= source;
WriteD( destaddr, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_44( )
// Instruction "ORL A,#data" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_44( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = PGMMem->Read8( PC++ );
destination |= source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_45( )
// Instruction "ORL A,direct" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_45( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char srcaddr = PGMMem->Read8( PC++ );
unsigned char source = ReadD( srcaddr );
destination |= source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_46( )
// Instruction "ORL A,@R0" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_46( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadI ( ReadD( BANKPSW + _R0_ ) );
destination |= source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_47( )
// Instruction "ORL A,@R1" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_47( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadI ( ReadD( BANKPSW + _R1_ ) );
destination |= source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_48( )
// Instruction "ORL A,R0" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_48( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R0_ );
destination |= source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_49( )
// Instruction "ORL A,R1" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_49( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R1_ );
destination |= source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_4A( )
// Instruction "ORL A,R2" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_4A( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R2_ );
destination |= source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_4B( )
// Instruction "ORL A,R3" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_4B( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R3_ );
destination |= source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_4C( )
// Instruction "ORL A,R4" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_4C( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R4_ );
destination |= source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_4D( )
// Instruction "ORL A,R5" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_4D( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R5_ );
destination |= source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_4E( )
// Instruction "ORL A,R6" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_4E( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R6_ );
destination |= source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_4F( )
// Instruction "ORL A,R7" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_4F( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R7_ );
destination |= source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_50( )
// Instruction "JNC reladdr" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_50( )
{
PC++;
unsigned int destination = ( ( PGMMem->Read8( PC - 1 ) + PC ) & 0xFF ) + ( PC & 0xFF00 );
if ( ReadD( _PSW_ ) < 0x80 ) { PC = destination; }
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_51( )
// Instruction "ACALL addr11" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_51( )
{
unsigned int addr11 = ( ( PGMMem->Read8( PC - 1 ) << 3 ) & 0xF00 ) + PGMMem->Read8( PC++ );
unsigned char SP = ReadD( _SP_ );
WriteI( ++SP, ( PC & 0x00FF ) );
WriteI( ++SP, ( PC >> 8 ) );
WriteD( _SP_, SP );
PC = ( PC & 0xF800 ) | addr11;
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_52( )
// Instruction "ANL direct,A" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_52( )
{
unsigned char destaddr = PGMMem->Read8( PC++ );
unsigned char destination = ReadD( destaddr );
unsigned char source = ReadD( _ACC_ );
destination &= source;
WriteD( destaddr, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_53( )
// Instruction "ANL direct,#data" takes 2 cycle(s) and 3 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_53( )
{
unsigned char destaddr = PGMMem->Read8( PC++ );
unsigned char destination = ReadD( destaddr );
unsigned char source = PGMMem->Read8( PC++ );
destination &= source;
WriteD( destaddr, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_54( )
// Instruction "ANL A,#data" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_54( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = PGMMem->Read8( PC++ );
destination &= source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_55( )
// Instruction "ANL A,direct" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_55( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char srcaddr = PGMMem->Read8( PC++ );
unsigned char source = ReadD( srcaddr );
destination &= source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_56( )
// Instruction "ANL A,@R0" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_56( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadI ( ReadD( BANKPSW + _R0_ ) );
destination &= source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_57( )
// Instruction "ANL A,@R1" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_57( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadI ( ReadD( BANKPSW + _R1_ ) );
destination &= source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_58( )
// Instruction "ANL A,R0" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_58( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R0_ );
destination &= source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_59( )
// Instruction "ANL A,R1" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_59( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R1_ );
destination &= source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_5A( )
// Instruction "ANL A,R2" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_5A( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R2_ );
destination &= source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_5B( )
// Instruction "ANL A,R3" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_5B( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R3_ );
destination &= source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_5C( )
// Instruction "ANL A,R4" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_5C( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R4_ );
destination &= source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_5D( )
// Instruction "ANL A,R5" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_5D( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R5_ );
destination &= source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_5E( )
// Instruction "ANL A,R6" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_5E( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R6_ );
destination &= source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_5F( )
// Instruction "ANL A,R7" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_5F( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R7_ );
destination &= source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_60( )
// Instruction "JZ reladdr" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_60( )
{
PC++;
unsigned int destination = ( ( PGMMem->Read8( PC - 1 ) + PC ) & 0xFF ) + ( PC & 0xFF00 );
if ( ReadD( _ACC_ ) == 0 ) { PC = destination; }
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_61( )
// Instruction "AJMP addr11" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_61( )
{
unsigned int addr11 = ( ( PGMMem->Read8( PC - 1 ) << 3 ) & 0xF00 ) + PGMMem->Read8( PC++ );
PC = ( PC & 0xF800 ) | addr11;
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_62( )
// Instruction "XRL direct,A" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_62( )
{
unsigned char destaddr = PGMMem->Read8( PC++ );
unsigned char destination = ReadD( destaddr );
unsigned char source = ReadD( _ACC_ );
destination ^= source;
WriteD( destaddr, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_63( )
// Instruction "XRL direct,#data" takes 2 cycle(s) and 3 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_63( )
{
unsigned char destaddr = PGMMem->Read8( PC++ );
unsigned char destination = ReadD( destaddr );
unsigned char source = PGMMem->Read8( PC++ );
destination ^= source;
WriteD( destaddr, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_64( )
// Instruction "XRL A,#data" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_64( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = PGMMem->Read8( PC++ );
destination ^= source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_65( )
// Instruction "XRL A,direct" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_65( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char srcaddr = PGMMem->Read8( PC++ );
unsigned char source = ReadD( srcaddr );
destination ^= source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_66( )
// Instruction "XRL A,@R0" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_66( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadI ( ReadD( BANKPSW + _R0_ ) );
destination ^= source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_67( )
// Instruction "XRL A,@R1" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_67( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadI ( ReadD( BANKPSW + _R1_ ) );
destination ^= source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_68( )
// Instruction "XRL A,R0" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_68( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R0_ );
destination ^= source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_69( )
// Instruction "XRL A,R1" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_69( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R1_ );
destination ^= source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_6A( )
// Instruction "XRL A,R2" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_6A( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R2_ );
destination ^= source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_6B( )
// Instruction "XRL A,R3" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_6B( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R3_ );
destination ^= source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_6C( )
// Instruction "XRL A,R4" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_6C( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R4_ );
destination ^= source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_6D( )
// Instruction "XRL A,R5" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_6D( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R5_ );
destination ^= source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_6E( )
// Instruction "XRL A,R6" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_6E( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R6_ );
destination ^= source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_6F( )
// Instruction "XRL A,R7" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_6F( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R7_ );
destination ^= source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_70( )
// Instruction "JNZ reladdr" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_70( )
{
PC++;
unsigned int destination = ( ( PGMMem->Read8( PC - 1 ) + PC ) & 0xFF ) + ( PC & 0xFF00 );
if ( ReadD( _ACC_ ) != 0 ) { PC = destination; }
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_71( )
// Instruction "ACALL addr11" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_71( )
{
unsigned int addr11 = ( ( PGMMem->Read8( PC - 1 ) << 3 ) & 0xF00 ) + PGMMem->Read8( PC++ );
unsigned char SP = ReadD( _SP_ );
WriteI( ++SP, ( PC & 0x00FF ) );
WriteI( ++SP, ( PC >> 8 ) );
WriteD( _SP_, SP );
PC = ( PC & 0xF800 ) | addr11;
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_72( )
// Instruction "ORL C,bitaddr" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_72( )
{
unsigned char destination = ( ReadD( _PSW_ ) >> 7 );
unsigned char source, srcbitaddr = PGMMem->Read8( PC++ );
source = ReadB( srcbitaddr );
WriteD( _PSW_ , ( ( destination | source ) << 7 ) );
WriteD( _PSW_, ( ( ReadD( _PSW_ ) & 0x7F) | ( destination << 7 ) ) );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_73( )
// Instruction "JMP @A+DPTR" takes 2 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_73( )
{
unsigned int destination = ReadI( ReadD( _ACC_ ) + ReadD( _DPTRLOW_ ) + ( ReadD( _DPTRHIGH_ ) << 8 ) );
PC = destination;
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_74( )
// Instruction "MOV A,#data" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_74( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = PGMMem->Read8( PC++ );
destination = source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_75( )
// Instruction "MOV direct,#data" takes 2 cycle(s) and 3 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_75( )
{
unsigned char destaddr = PGMMem->Read8( PC++ );
unsigned char destination = ReadD( destaddr );
unsigned char source = PGMMem->Read8( PC++ );
destination = source;
WriteD( destaddr, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_76( )
// Instruction "MOV @R0,#data" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_76( )
{
unsigned char destination = ReadI ( ReadD( BANKPSW + _R0_ ) );
unsigned char source = PGMMem->Read8( PC++ );
destination = source;
WriteI( ReadD( BANKPSW + _R0_ ), destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_77( )
// Instruction "MOV @R1,#data" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_77( )
{
unsigned char destination = ReadI ( ReadD( BANKPSW + _R1_ ) );
unsigned char source = PGMMem->Read8( PC++ );
destination = source;
WriteI( ReadD( BANKPSW + _R1_ ), destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_78( )
// Instruction "MOV R0,#data" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_78( )
{
unsigned char destination = ReadD( BANKPSW + _R0_ );
unsigned char source = PGMMem->Read8( PC++ );
destination = source;
WriteD( BANKPSW + _R0_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_79( )
// Instruction "MOV R1,#data" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_79( )
{
unsigned char destination = ReadD( BANKPSW + _R1_ );
unsigned char source = PGMMem->Read8( PC++ );
destination = source;
WriteD( BANKPSW + _R1_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_7A( )
// Instruction "MOV R2,#data" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_7A( )
{
unsigned char destination = ReadD( BANKPSW + _R2_ );
unsigned char source = PGMMem->Read8( PC++ );
destination = source;
WriteD( BANKPSW + _R2_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_7B( )
// Instruction "MOV R3,#data" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_7B( )
{
unsigned char destination = ReadD( BANKPSW + _R3_ );
unsigned char source = PGMMem->Read8( PC++ );
destination = source;
WriteD( BANKPSW + _R3_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_7C( )
// Instruction "MOV R4,#data" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_7C( )
{
unsigned char destination = ReadD( BANKPSW + _R4_ );
unsigned char source = PGMMem->Read8( PC++ );
destination = source;
WriteD( BANKPSW + _R4_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_7D( )
// Instruction "MOV R5,#data" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_7D( )
{
unsigned char destination = ReadD( BANKPSW + _R5_ );
unsigned char source = PGMMem->Read8( PC++ );
destination = source;
WriteD( BANKPSW + _R5_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_7E( )
// Instruction "MOV R6,#data" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_7E( )
{
unsigned char destination = ReadD( BANKPSW + _R6_ );
unsigned char source = PGMMem->Read8( PC++ );
destination = source;
WriteD( BANKPSW + _R6_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_7F( )
// Instruction "MOV R7,#data" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_7F( )
{
unsigned char destination = ReadD( BANKPSW + _R7_ );
unsigned char source = PGMMem->Read8( PC++ );
destination = source;
WriteD( BANKPSW + _R7_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_80( )
// Instruction "SJMP reladdr" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_80( )
{
PC++;
unsigned int destination = ( ( PGMMem->Read8( PC - 1 ) + PC ) & 0xFF ) + ( PC & 0xFF00 );
PC = destination;
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_81( )
// Instruction "AJMP addr11" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_81( )
{
unsigned int addr11 = ( ( PGMMem->Read8( PC - 1 ) << 3 ) & 0xF00 ) + PGMMem->Read8( PC++ );
PC = ( PC & 0xF800 ) | addr11;
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_82( )
// Instruction "ANL C,bitaddr" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_82( )
{
unsigned char destination = ( ReadD( _PSW_ ) >> 7 );
unsigned char source, srcbitaddr = PGMMem->Read8( PC++ );
source = ReadB( srcbitaddr );
WriteD( _PSW_, ( ( destination & source) << 7 ) );
WriteD( _PSW_, ( ( ReadD( _PSW_ ) & 0x7F) | ( destination << 7 ) ) );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_83( )
// Instruction "MOVC A,@A+PC" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_83( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = PGMMem->Read8( ReadD( _ACC_ ) + ( ++PC ) );
destination = source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_84( )
// Instruction "DIV AB" takes 4 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_84( )
{
unsigned char A = ReadD( _ACC_ ), B = ReadD( _B_ );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x7B ) );
if ( B != 0 ) {
WriteD( _ACC_, A/B ); WriteD( _B_, A%B );
} else WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
return 4;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_85( )
// Instruction "MOV direct,direct" takes 1 cycle(s) and 3 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_85( )
{
unsigned char srcaddr = PGMMem->Read8( PC++ );
unsigned char source = ReadD( srcaddr );
unsigned char destaddr = PGMMem->Read8( PC++ );
unsigned char destination = ReadD( destaddr );
destination = source;
WriteD( destaddr, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_86( )
// Instruction "MOV direct,@R0" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_86( )
{
unsigned char destaddr = PGMMem->Read8( PC++ );
unsigned char destination = ReadD( destaddr );
unsigned char source = ReadI ( ReadD( BANKPSW + _R0_ ) );
destination = source;
WriteD( destaddr, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_87( )
// Instruction "MOV direct,@R1" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_87( )
{
unsigned char destaddr = PGMMem->Read8( PC++ );
unsigned char destination = ReadD( destaddr );
unsigned char source = ReadI ( ReadD( BANKPSW + _R1_ ) );
destination = source;
WriteD( destaddr, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_88( )
// Instruction "MOV direct,R0" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_88( )
{
unsigned char destaddr = PGMMem->Read8( PC++ );
unsigned char destination = ReadD( destaddr );
unsigned char source = ReadD( BANKPSW + _R0_ );
destination = source;
WriteD( destaddr, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_89( )
// Instruction "MOV direct,R1" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_89( )
{
unsigned char destaddr = PGMMem->Read8( PC++ );
unsigned char destination = ReadD( destaddr );
unsigned char source = ReadD( BANKPSW + _R1_ );
destination = source;
WriteD( destaddr, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_8A( )
// Instruction "MOV direct,R2" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_8A( )
{
unsigned char destaddr = PGMMem->Read8( PC++ );
unsigned char destination = ReadD( destaddr );
unsigned char source = ReadD( BANKPSW + _R2_ );
destination = source;
WriteD( destaddr, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_8B( )
// Instruction "MOV direct,R3" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_8B( )
{
unsigned char destaddr = PGMMem->Read8( PC++ );
unsigned char destination = ReadD( destaddr );
unsigned char source = ReadD( BANKPSW + _R3_ );
destination = source;
WriteD( destaddr, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_8C( )
// Instruction "MOV direct,R4" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_8C( )
{
unsigned char destaddr = PGMMem->Read8( PC++ );
unsigned char destination = ReadD( destaddr );
unsigned char source = ReadD( BANKPSW + _R4_ );
destination = source;
WriteD( destaddr, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_8D( )
// Instruction "MOV direct,R5" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_8D( )
{
unsigned char destaddr = PGMMem->Read8( PC++ );
unsigned char destination = ReadD( destaddr );
unsigned char source = ReadD( BANKPSW + _R5_ );
destination = source;
WriteD( destaddr, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_8E( )
// Instruction "MOV direct,R6" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_8E( )
{
unsigned char destaddr = PGMMem->Read8( PC++ );
unsigned char destination = ReadD( destaddr );
unsigned char source = ReadD( BANKPSW + _R6_ );
destination = source;
WriteD( destaddr, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_8F( )
// Instruction "MOV direct,R7" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_8F( )
{
unsigned char destaddr = PGMMem->Read8( PC++ );
unsigned char destination = ReadD( destaddr );
unsigned char source = ReadD( BANKPSW + _R7_ );
destination = source;
WriteD( destaddr, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_90( )
// Instruction "MOV DPTR,#data16" takes 2 cycle(s) and 3 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_90( )
{
unsigned int destination = ( ReadD( _DPTRHIGH_ ) << 8 ) + ReadD( _DPTRLOW_ );
unsigned char source = ( PGMMem->Read8( PC++ ) << 8 );
source += PGMMem->Read8( PC++ );
destination = source;
WriteD( _DPTRHIGH_, ( destination >> 8 ) );
WriteD( _DPTRLOW_, ( destination & 0xFF ) );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_91( )
// Instruction "ACALL addr11" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_91( )
{
unsigned int addr11 = ( ( PGMMem->Read8( PC - 1 ) << 3 ) & 0xF00 ) + PGMMem->Read8( PC++ );
unsigned char SP = ReadD( _SP_ );
WriteI( ++SP, ( PC & 0x00FF ) );
WriteI( ++SP, ( PC >> 8 ) );
WriteD( _SP_, SP );
PC = ( PC & 0xF800 ) | addr11;
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_92( )
// Instruction "MOV bitaddr,C" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_92( )
{
unsigned char destination, dstbitaddr = PGMMem->Read8( PC++ );
destination = ReadB( dstbitaddr );
unsigned char source = ( ReadD( _PSW_ ) >> 7 );
destination = source;
WriteB( dstbitaddr, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_93( )
// Instruction "MOVC A,@A+DPTR" takes 2 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_93( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = PGMMem->Read8( ReadD( _ACC_ ) + ReadD( _DPTRLOW_ ) + ( ReadD( _DPTRHIGH_ ) << 8 ) );
destination = source;
WriteD( _ACC_, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_94( )
// Instruction "SUBB A,#data" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_94( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = PGMMem->Read8( PC++ );
unsigned char carryflag = ReadD( _PSW_ ) >> 7;
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );
if ( destination < ( source + carryflag ) ) {
  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
  if ( ( destination & 0x7F ) > ( ( source + carryflag ) & 0x7F ) )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
} else if ( ( destination & 0x7F ) < ( ( source + carryflag ) & 0x7F ) )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
if ( ( destination & 0x0F ) < ( ( source + carryflag ) & 0x0F ) )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x40 ) );
destination -= source + carryflag;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_95( )
// Instruction "SUBB A,direct" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_95( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char srcaddr = PGMMem->Read8( PC++ );
unsigned char source = ReadD( srcaddr );
unsigned char carryflag = ReadD( _PSW_ ) >> 7;
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );
if ( destination < ( source + carryflag ) ) {
  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
  if ( ( destination & 0x7F ) > ( ( source + carryflag ) & 0x7F ) )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
} else if ( ( destination & 0x7F ) < ( ( source + carryflag ) & 0x7F ) )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
if ( ( destination & 0x0F ) < ( ( source + carryflag ) & 0x0F ) )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x40 ) );
destination -= source + carryflag;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_96( )
// Instruction "SUBB A,@R0" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_96( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadI ( ReadD( BANKPSW + _R0_ ) );
unsigned char carryflag = ReadD( _PSW_ ) >> 7;
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );
if ( destination < ( source + carryflag ) ) {
  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
  if ( ( destination & 0x7F ) > ( ( source + carryflag ) & 0x7F ) )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
} else if ( ( destination & 0x7F ) < ( ( source + carryflag ) & 0x7F ) )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
if ( ( destination & 0x0F ) < ( ( source + carryflag ) & 0x0F ) )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x40 ) );
destination -= source + carryflag;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_97( )
// Instruction "SUBB A,@R1" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_97( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadI ( ReadD( BANKPSW + _R1_ ) );
unsigned char carryflag = ReadD( _PSW_ ) >> 7;
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );
if ( destination < ( source + carryflag ) ) {
  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
  if ( ( destination & 0x7F ) > ( ( source + carryflag ) & 0x7F ) )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
} else if ( ( destination & 0x7F ) < ( ( source + carryflag ) & 0x7F ) )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
if ( ( destination & 0x0F ) < ( ( source + carryflag ) & 0x0F ) )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x40 ) );
destination -= source + carryflag;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_98( )
// Instruction "SUBB A,R0" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_98( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R0_ );
unsigned char carryflag = ReadD( _PSW_ ) >> 7;
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );
if ( destination < ( source + carryflag ) ) {
  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
  if ( ( destination & 0x7F ) > ( ( source + carryflag ) & 0x7F ) )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
} else if ( ( destination & 0x7F ) < ( ( source + carryflag ) & 0x7F ) )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
if ( ( destination & 0x0F ) < ( ( source + carryflag ) & 0x0F ) )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x40 ) );
destination -= source + carryflag;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_99( )
// Instruction "SUBB A,R1" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_99( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R1_ );
unsigned char carryflag = ReadD( _PSW_ ) >> 7;
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );
if ( destination < ( source + carryflag ) ) {
  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
  if ( ( destination & 0x7F ) > ( ( source + carryflag ) & 0x7F ) )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
} else if ( ( destination & 0x7F ) < ( ( source + carryflag ) & 0x7F ) )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
if ( ( destination & 0x0F ) < ( ( source + carryflag ) & 0x0F ) )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x40 ) );
destination -= source + carryflag;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_9A( )
// Instruction "SUBB A,R2" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_9A( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R2_ );
unsigned char carryflag = ReadD( _PSW_ ) >> 7;
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );
if ( destination < ( source + carryflag ) ) {
  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
  if ( ( destination & 0x7F ) > ( ( source + carryflag ) & 0x7F ) )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
} else if ( ( destination & 0x7F ) < ( ( source + carryflag ) & 0x7F ) )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
if ( ( destination & 0x0F ) < ( ( source + carryflag ) & 0x0F ) )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x40 ) );
destination -= source + carryflag;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_9B( )
// Instruction "SUBB A,R3" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_9B( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R3_ );
unsigned char carryflag = ReadD( _PSW_ ) >> 7;
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );
if ( destination < ( source + carryflag ) ) {
  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
  if ( ( destination & 0x7F ) > ( ( source + carryflag ) & 0x7F ) )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
} else if ( ( destination & 0x7F ) < ( ( source + carryflag ) & 0x7F ) )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
if ( ( destination & 0x0F ) < ( ( source + carryflag ) & 0x0F ) )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x40 ) );
destination -= source + carryflag;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_9C( )
// Instruction "SUBB A,R4" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_9C( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R4_ );
unsigned char carryflag = ReadD( _PSW_ ) >> 7;
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );
if ( destination < ( source + carryflag ) ) {
  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
  if ( ( destination & 0x7F ) > ( ( source + carryflag ) & 0x7F ) )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
} else if ( ( destination & 0x7F ) < ( ( source + carryflag ) & 0x7F ) )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
if ( ( destination & 0x0F ) < ( ( source + carryflag ) & 0x0F ) )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x40 ) );
destination -= source + carryflag;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_9D( )
// Instruction "SUBB A,R5" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_9D( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R5_ );
unsigned char carryflag = ReadD( _PSW_ ) >> 7;
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );
if ( destination < ( source + carryflag ) ) {
  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
  if ( ( destination & 0x7F ) > ( ( source + carryflag ) & 0x7F ) )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
} else if ( ( destination & 0x7F ) < ( ( source + carryflag ) & 0x7F ) )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
if ( ( destination & 0x0F ) < ( ( source + carryflag ) & 0x0F ) )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x40 ) );
destination -= source + carryflag;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_9E( )
// Instruction "SUBB A,R6" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_9E( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R6_ );
unsigned char carryflag = ReadD( _PSW_ ) >> 7;
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );
if ( destination < ( source + carryflag ) ) {
  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
  if ( ( destination & 0x7F ) > ( ( source + carryflag ) & 0x7F ) )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
} else if ( ( destination & 0x7F ) < ( ( source + carryflag ) & 0x7F ) )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
if ( ( destination & 0x0F ) < ( ( source + carryflag ) & 0x0F ) )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x40 ) );
destination -= source + carryflag;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_9F( )
// Instruction "SUBB A,R7" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_9F( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R7_ );
unsigned char carryflag = ReadD( _PSW_ ) >> 7;
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x3B ) );
if ( destination < ( source + carryflag ) ) {
  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
  if ( ( destination & 0x7F ) > ( ( source + carryflag ) & 0x7F ) )  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
} else if ( ( destination & 0x7F ) < ( ( source + carryflag ) & 0x7F ) )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
if ( ( destination & 0x0F ) < ( ( source + carryflag ) & 0x0F ) )   WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x40 ) );
destination -= source + carryflag;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_A0( )
// Instruction "ORL C,/bitaddr" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_A0( )
{
unsigned char destination = ( ReadD( _PSW_ ) >> 7 );
unsigned char source, srcbitaddr = PGMMem->Read8( PC++ );
source = ( ReadB( srcbitaddr ) ^ 1 );
WriteD( _PSW_ , ( ( destination | source ) << 7 ) );
WriteD( _PSW_, ( ( ReadD( _PSW_ ) & 0x7F) | ( destination << 7 ) ) );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_A1( )
// Instruction "AJMP addr11" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_A1( )
{
unsigned int addr11 = ( ( PGMMem->Read8( PC - 1 ) << 3 ) & 0xF00 ) + PGMMem->Read8( PC++ );
PC = ( PC & 0xF800 ) | addr11;
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_A2( )
// Instruction "MOV C,bitaddr" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_A2( )
{
unsigned char destination = ( ReadD( _PSW_ ) >> 7 );
unsigned char source, srcbitaddr = PGMMem->Read8( PC++ );
source = ReadB( srcbitaddr );
destination = source;
WriteD( _PSW_, ( ( ReadD( _PSW_ ) & 0x7F) | ( destination << 7 ) ) );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_A3( )
// Instruction "INC DPTR" takes 2 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_A3( )
{
unsigned int destination = ( ReadD( _DPTRHIGH_ ) << 8 ) + ReadD( _DPTRLOW_ );
destination++;
WriteD( _DPTRHIGH_, ( destination >> 8 ) );
WriteD( _DPTRLOW_, ( destination & 0xFF ) );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_A4( )
// Instruction "MUL AB" takes 4 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_A4( )
{
unsigned char A = ReadD( _ACC_ ), B = ReadD( _B_ );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x7B ) );
WriteD( _ACC_ , ( ( A * B ) & 0x00FF ) ); WriteD( _B_, ( A * B ) / 0x100 );
if ( ReadD( _B_ ) > 0) WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x04 ) );
return 4;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_A5( )
// Instruction "INVALID" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_A5( )
{
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_A6( )
// Instruction "MOV @R0,direct" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_A6( )
{
unsigned char destination = ReadI ( ReadD( BANKPSW + _R0_ ) );
unsigned char srcaddr = PGMMem->Read8( PC++ );
unsigned char source = ReadD( srcaddr );
destination = source;
WriteI( ReadD( BANKPSW + _R0_ ), destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_A7( )
// Instruction "MOV @R1,direct" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_A7( )
{
unsigned char destination = ReadI ( ReadD( BANKPSW + _R1_ ) );
unsigned char srcaddr = PGMMem->Read8( PC++ );
unsigned char source = ReadD( srcaddr );
destination = source;
WriteI( ReadD( BANKPSW + _R1_ ), destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_A8( )
// Instruction "MOV R0,direct" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_A8( )
{
unsigned char destination = ReadD( BANKPSW + _R0_ );
unsigned char srcaddr = PGMMem->Read8( PC++ );
unsigned char source = ReadD( srcaddr );
destination = source;
WriteD( BANKPSW + _R0_, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_A9( )
// Instruction "MOV R1,direct" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_A9( )
{
unsigned char destination = ReadD( BANKPSW + _R1_ );
unsigned char srcaddr = PGMMem->Read8( PC++ );
unsigned char source = ReadD( srcaddr );
destination = source;
WriteD( BANKPSW + _R1_, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_AA( )
// Instruction "MOV R2,direct" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_AA( )
{
unsigned char destination = ReadD( BANKPSW + _R2_ );
unsigned char srcaddr = PGMMem->Read8( PC++ );
unsigned char source = ReadD( srcaddr );
destination = source;
WriteD( BANKPSW + _R2_, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_AB( )
// Instruction "MOV R3,direct" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_AB( )
{
unsigned char destination = ReadD( BANKPSW + _R3_ );
unsigned char srcaddr = PGMMem->Read8( PC++ );
unsigned char source = ReadD( srcaddr );
destination = source;
WriteD( BANKPSW + _R3_, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_AC( )
// Instruction "MOV R4,direct" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_AC( )
{
unsigned char destination = ReadD( BANKPSW + _R4_ );
unsigned char srcaddr = PGMMem->Read8( PC++ );
unsigned char source = ReadD( srcaddr );
destination = source;
WriteD( BANKPSW + _R4_, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_AD( )
// Instruction "MOV R5,direct" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_AD( )
{
unsigned char destination = ReadD( BANKPSW + _R5_ );
unsigned char srcaddr = PGMMem->Read8( PC++ );
unsigned char source = ReadD( srcaddr );
destination = source;
WriteD( BANKPSW + _R5_, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_AE( )
// Instruction "MOV R6,direct" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_AE( )
{
unsigned char destination = ReadD( BANKPSW + _R6_ );
unsigned char srcaddr = PGMMem->Read8( PC++ );
unsigned char source = ReadD( srcaddr );
destination = source;
WriteD( BANKPSW + _R6_, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_AF( )
// Instruction "MOV R7,direct" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_AF( )
{
unsigned char destination = ReadD( BANKPSW + _R7_ );
unsigned char srcaddr = PGMMem->Read8( PC++ );
unsigned char source = ReadD( srcaddr );
destination = source;
WriteD( BANKPSW + _R7_, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_B0( )
// Instruction "ANL C,/bitaddr" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_B0( )
{
unsigned char destination = ( ReadD( _PSW_ ) >> 7 );
unsigned char source, srcbitaddr = PGMMem->Read8( PC++ );
source = ( ReadB( srcbitaddr ) ^ 1 );
WriteD( _PSW_, ( ( destination & source) << 7 ) );
WriteD( _PSW_, ( ( ReadD( _PSW_ ) & 0x7F) | ( destination << 7 ) ) );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_B1( )
// Instruction "ACALL addr11" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_B1( )
{
unsigned int addr11 = ( ( PGMMem->Read8( PC - 1 ) << 3 ) & 0xF00 ) + PGMMem->Read8( PC++ );
unsigned char SP = ReadD( _SP_ );
WriteI( ++SP, ( PC & 0x00FF ) );
WriteI( ++SP, ( PC >> 8 ) );
WriteD( _SP_, SP );
PC = ( PC & 0xF800 ) | addr11;
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_B2( )
// Instruction "CPL bitaddr" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_B2( )
{
unsigned char destination, dstbitaddr = PGMMem->Read8( PC++ );
destination = ReadB( dstbitaddr );
destination ^= 0x01;
WriteB( dstbitaddr, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_B3( )
// Instruction "CPL C" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_B3( )
{
unsigned char destination = ( ReadD( _PSW_ ) >> 7 );
destination ^= 0x01;
WriteD( _PSW_, ( ( ReadD( _PSW_ ) & 0x7F) | ( destination << 7 ) ) );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_B4( )
// Instruction "CJNE A,#data,reladdr" takes 2 cycle(s) and 3 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_B4( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = PGMMem->Read8( PC++ );
unsigned int reladdr = ( ( PGMMem->Read8( PC++ ) + ( ( PC + 1 ) & 0x00FF ) ) & 0x00FF ) + ( ( PC + 1 ) & 0xFF00 );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x7F ) );
if ( destination < source ) WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
if ( destination != source ) PC = reladdr;
WriteD( _ACC_, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_B5( )
// Instruction "CJNE A,direct,reladdr" takes 2 cycle(s) and 3 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_B5( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char srcaddr = PGMMem->Read8( PC++ );
unsigned char source = ReadD( srcaddr );
unsigned int reladdr = ( ( PGMMem->Read8( PC++ ) + ( ( PC + 1 ) & 0x00FF ) ) & 0x00FF ) + ( ( PC + 1 ) & 0xFF00 );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x7F ) );
if ( destination < source ) WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
if ( destination != source ) PC = reladdr;
WriteD( _ACC_, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_B6( )
// Instruction "CJNE @R0,#data,reladdr" takes 2 cycle(s) and 3 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_B6( )
{
unsigned char destination = ReadI ( ReadD( BANKPSW + _R0_ ) );
unsigned char source = PGMMem->Read8( PC++ );
unsigned int reladdr = ( ( PGMMem->Read8( PC++ ) + ( ( PC + 1 ) & 0x00FF ) ) & 0x00FF ) + ( ( PC + 1 ) & 0xFF00 );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x7F ) );
if ( destination < source ) WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
if ( destination != source ) PC = reladdr;
WriteI( ReadD( BANKPSW + _R0_ ), destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_B7( )
// Instruction "CJNE @R1,#data,reladdr" takes 2 cycle(s) and 3 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_B7( )
{
unsigned char destination = ReadI ( ReadD( BANKPSW + _R1_ ) );
unsigned char source = PGMMem->Read8( PC++ );
unsigned int reladdr = ( ( PGMMem->Read8( PC++ ) + ( ( PC + 1 ) & 0x00FF ) ) & 0x00FF ) + ( ( PC + 1 ) & 0xFF00 );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x7F ) );
if ( destination < source ) WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
if ( destination != source ) PC = reladdr;
WriteI( ReadD( BANKPSW + _R1_ ), destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_B8( )
// Instruction "CJNE R0,#data,reladdr" takes 2 cycle(s) and 3 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_B8( )
{
unsigned char destination = ReadD( BANKPSW + _R0_ );
unsigned char source = PGMMem->Read8( PC++ );
unsigned int reladdr = ( ( PGMMem->Read8( PC++ ) + ( ( PC + 1 ) & 0x00FF ) ) & 0x00FF ) + ( ( PC + 1 ) & 0xFF00 );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x7F ) );
if ( destination < source ) WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
if ( destination != source ) PC = reladdr;
WriteD( BANKPSW + _R0_, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_B9( )
// Instruction "CJNE R1,#data,reladdr" takes 2 cycle(s) and 3 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_B9( )
{
unsigned char destination = ReadD( BANKPSW + _R1_ );
unsigned char source = PGMMem->Read8( PC++ );
unsigned int reladdr = ( ( PGMMem->Read8( PC++ ) + ( ( PC + 1 ) & 0x00FF ) ) & 0x00FF ) + ( ( PC + 1 ) & 0xFF00 );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x7F ) );
if ( destination < source ) WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
if ( destination != source ) PC = reladdr;
WriteD( BANKPSW + _R1_, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_BA( )
// Instruction "CJNE R2,#data,reladdr" takes 2 cycle(s) and 3 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_BA( )
{
unsigned char destination = ReadD( BANKPSW + _R2_ );
unsigned char source = PGMMem->Read8( PC++ );
unsigned int reladdr = ( ( PGMMem->Read8( PC++ ) + ( ( PC + 1 ) & 0x00FF ) ) & 0x00FF ) + ( ( PC + 1 ) & 0xFF00 );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x7F ) );
if ( destination < source ) WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
if ( destination != source ) PC = reladdr;
WriteD( BANKPSW + _R2_, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_BB( )
// Instruction "CJNE R3,#data,reladdr" takes 2 cycle(s) and 3 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_BB( )
{
unsigned char destination = ReadD( BANKPSW + _R3_ );
unsigned char source = PGMMem->Read8( PC++ );
unsigned int reladdr = ( ( PGMMem->Read8( PC++ ) + ( ( PC + 1 ) & 0x00FF ) ) & 0x00FF ) + ( ( PC + 1 ) & 0xFF00 );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x7F ) );
if ( destination < source ) WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
if ( destination != source ) PC = reladdr;
WriteD( BANKPSW + _R3_, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_BC( )
// Instruction "CJNE R4,#data,reladdr" takes 2 cycle(s) and 3 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_BC( )
{
unsigned char destination = ReadD( BANKPSW + _R4_ );
unsigned char source = PGMMem->Read8( PC++ );
unsigned int reladdr = ( ( PGMMem->Read8( PC++ ) + ( ( PC + 1 ) & 0x00FF ) ) & 0x00FF ) + ( ( PC + 1 ) & 0xFF00 );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x7F ) );
if ( destination < source ) WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
if ( destination != source ) PC = reladdr;
WriteD( BANKPSW + _R4_, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_BD( )
// Instruction "CJNE R5,#data,reladdr" takes 2 cycle(s) and 3 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_BD( )
{
unsigned char destination = ReadD( BANKPSW + _R5_ );
unsigned char source = PGMMem->Read8( PC++ );
unsigned int reladdr = ( ( PGMMem->Read8( PC++ ) + ( ( PC + 1 ) & 0x00FF ) ) & 0x00FF ) + ( ( PC + 1 ) & 0xFF00 );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x7F ) );
if ( destination < source ) WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
if ( destination != source ) PC = reladdr;
WriteD( BANKPSW + _R5_, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_BE( )
// Instruction "CJNE R6,#data,reladdr" takes 2 cycle(s) and 3 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_BE( )
{
unsigned char destination = ReadD( BANKPSW + _R6_ );
unsigned char source = PGMMem->Read8( PC++ );
unsigned int reladdr = ( ( PGMMem->Read8( PC++ ) + ( ( PC + 1 ) & 0x00FF ) ) & 0x00FF ) + ( ( PC + 1 ) & 0xFF00 );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x7F ) );
if ( destination < source ) WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
if ( destination != source ) PC = reladdr;
WriteD( BANKPSW + _R6_, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_BF( )
// Instruction "CJNE R7,#data,reladdr" takes 2 cycle(s) and 3 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_BF( )
{
unsigned char destination = ReadD( BANKPSW + _R7_ );
unsigned char source = PGMMem->Read8( PC++ );
unsigned int reladdr = ( ( PGMMem->Read8( PC++ ) + ( ( PC + 1 ) & 0x00FF ) ) & 0x00FF ) + ( ( PC + 1 ) & 0xFF00 );
WriteD( _PSW_, ( ReadD( _PSW_ ) & 0x7F ) );
if ( destination < source ) WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
if ( destination != source ) PC = reladdr;
WriteD( BANKPSW + _R7_, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_C0( )
// Instruction "PUSH direct" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_C0( )
{
unsigned char destaddr = PGMMem->Read8( PC++ );
unsigned char destination = ReadD( destaddr );
unsigned char SP = ReadD( _SP_ );
WriteI( ++SP, destination );
WriteD( _SP_, SP );
WriteD( destaddr, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_C1( )
// Instruction "AJMP addr11" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_C1( )
{
unsigned int addr11 = ( ( PGMMem->Read8( PC - 1 ) << 3 ) & 0xF00 ) + PGMMem->Read8( PC++ );
PC = ( PC & 0xF800 ) | addr11;
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_C2( )
// Instruction "CLR bitaddr" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_C2( )
{
unsigned char destination, dstbitaddr = PGMMem->Read8( PC++ );
destination = ReadB( dstbitaddr );
destination = 0;
WriteB( dstbitaddr, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_C3( )
// Instruction "CLR C" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_C3( )
{
unsigned char destination = ( ReadD( _PSW_ ) >> 7 );
destination = 0;
WriteD( _PSW_, ( ( ReadD( _PSW_ ) & 0x7F) | ( destination << 7 ) ) );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_C4( )
// Instruction "SWAP A" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_C4( )
{
unsigned char destination = ReadD( _ACC_ );
destination = ( destination << 4 ) + ( destination >> 4 );
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_C5( )
// Instruction "XCH A,direct" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_C5( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char srcaddr = PGMMem->Read8( PC++ );
unsigned char source = ReadD( srcaddr );
unsigned char tmpval = destination;
destination = source; source = tmpval;
WriteD( _ACC_, destination );
WriteD( srcaddr, source );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_C6( )
// Instruction "XCH A,@R0" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_C6( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadI ( ReadD( BANKPSW + _R0_ ) );
unsigned char tmpval = destination;
destination = source; source = tmpval;
WriteD( _ACC_, destination );
WriteI( ReadD( BANKPSW + _R0_ ), source );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_C7( )
// Instruction "XCH A,@R1" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_C7( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadI ( ReadD( BANKPSW + _R1_ ) );
unsigned char tmpval = destination;
destination = source; source = tmpval;
WriteD( _ACC_, destination );
WriteI( ReadD( BANKPSW + _R1_ ), source );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_C8( )
// Instruction "XCH A,R0" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_C8( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R0_ );
unsigned char tmpval = destination;
destination = source; source = tmpval;
WriteD( _ACC_, destination );
WriteD( BANKPSW + _R0_, source );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_C9( )
// Instruction "XCH A,R1" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_C9( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R1_ );
unsigned char tmpval = destination;
destination = source; source = tmpval;
WriteD( _ACC_, destination );
WriteD( BANKPSW + _R1_, source );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_CA( )
// Instruction "XCH A,R2" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_CA( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R2_ );
unsigned char tmpval = destination;
destination = source; source = tmpval;
WriteD( _ACC_, destination );
WriteD( BANKPSW + _R2_, source );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_CB( )
// Instruction "XCH A,R3" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_CB( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R3_ );
unsigned char tmpval = destination;
destination = source; source = tmpval;
WriteD( _ACC_, destination );
WriteD( BANKPSW + _R3_, source );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_CC( )
// Instruction "XCH A,R4" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_CC( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R4_ );
unsigned char tmpval = destination;
destination = source; source = tmpval;
WriteD( _ACC_, destination );
WriteD( BANKPSW + _R4_, source );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_CD( )
// Instruction "XCH A,R5" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_CD( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R5_ );
unsigned char tmpval = destination;
destination = source; source = tmpval;
WriteD( _ACC_, destination );
WriteD( BANKPSW + _R5_, source );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_CE( )
// Instruction "XCH A,R6" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_CE( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R6_ );
unsigned char tmpval = destination;
destination = source; source = tmpval;
WriteD( _ACC_, destination );
WriteD( BANKPSW + _R6_, source );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_CF( )
// Instruction "XCH A,R7" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_CF( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R7_ );
unsigned char tmpval = destination;
destination = source; source = tmpval;
WriteD( _ACC_, destination );
WriteD( BANKPSW + _R7_, source );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_D0( )
// Instruction "POP direct" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_D0( )
{
unsigned char destaddr = PGMMem->Read8( PC++ );
unsigned char destination = ReadD( destaddr );
unsigned char SP = ReadD( _SP_ );
destination = ReadI( SP-- );
WriteD( _SP_, SP );
WriteD( destaddr, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_D1( )
// Instruction "ACALL addr11" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_D1( )
{
unsigned int addr11 = ( ( PGMMem->Read8( PC - 1 ) << 3 ) & 0xF00 ) + PGMMem->Read8( PC++ );
unsigned char SP = ReadD( _SP_ );
WriteI( ++SP, ( PC & 0x00FF ) );
WriteI( ++SP, ( PC >> 8 ) );
WriteD( _SP_, SP );
PC = ( PC & 0xF800 ) | addr11;
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_D2( )
// Instruction "SETB bitaddr" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_D2( )
{
unsigned char destination, dstbitaddr = PGMMem->Read8( PC++ );
destination = ReadB( dstbitaddr );
destination = 1;
WriteB( dstbitaddr, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_D3( )
// Instruction "SETB C" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_D3( )
{
unsigned char destination = ( ReadD( _PSW_ ) >> 7 );
destination = 1;
WriteD( _PSW_, ( ( ReadD( _PSW_ ) & 0x7F) | ( destination << 7 ) ) );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_D4( )
// Instruction "DA A" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_D4( )
{
unsigned char destination = ReadD( _ACC_ );
if ( ( ( destination & 0x0F ) > 9) || ( ReadD( _PSW_ ) | 0x40)) {
   if ( ( destination + 6 ) > 0xFF)  WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
   destination += 6;
}
if ( ( ReadD( _PSW_ ) & 0x80) || ( ( destination & 0xF0 ) > 0x90 ) ) {
   if ( ( destination + 0x60 ) > 0xFF ) WriteD( _PSW_, ( ReadD( _PSW_ ) | 0x80 ) );
   destination += 0x60;
}
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_D5( )
// Instruction "DJNZ direct,reladdr" takes 2 cycle(s) and 3 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_D5( )
{
unsigned char destaddr = PGMMem->Read8( PC++ );
unsigned char destination = ReadD( destaddr );
PC++;
unsigned int source = ( ( PGMMem->Read8( PC - 1 ) + PC ) & 0xFF ) + ( PC & 0xFF00 );
destination--;
if ( destination != 0 ) PC = source;
WriteD( destaddr, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_D6( )
// Instruction "XCHD A,@R0" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_D6( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadI ( ReadD( BANKPSW + _R0_ ) );
unsigned char tmpval = ( destination & 0x0F );
destination = ( destination & 0xF0 ) + ( source & 0x0F );
source = ( source & 0xF0 ) + tmpval;
WriteD( _ACC_, destination );
WriteI( ReadD( BANKPSW + _R0_ ), source );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_D7( )
// Instruction "XCHD A,@R1" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_D7( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadI ( ReadD( BANKPSW + _R1_ ) );
unsigned char tmpval = ( destination & 0x0F );
destination = ( destination & 0xF0 ) + ( source & 0x0F );
source = ( source & 0xF0 ) + tmpval;
WriteD( _ACC_, destination );
WriteI( ReadD( BANKPSW + _R1_ ), source );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_D8( )
// Instruction "DJNZ R0,reladdr" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_D8( )
{
unsigned char destination = ReadD( BANKPSW + _R0_ );
PC++;
unsigned int source = ( ( PGMMem->Read8( PC - 1 ) + PC ) & 0xFF ) + ( PC & 0xFF00 );
destination--;
if ( destination != 0 ) PC = source;
WriteD( BANKPSW + _R0_, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_D9( )
// Instruction "DJNZ R1,reladdr" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_D9( )
{
unsigned char destination = ReadD( BANKPSW + _R1_ );
PC++;
unsigned int source = ( ( PGMMem->Read8( PC - 1 ) + PC ) & 0xFF ) + ( PC & 0xFF00 );
destination--;
if ( destination != 0 ) PC = source;
WriteD( BANKPSW + _R1_, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_DA( )
// Instruction "DJNZ R2,reladdr" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_DA( )
{
unsigned char destination = ReadD( BANKPSW + _R2_ );
PC++;
unsigned int source = ( ( PGMMem->Read8( PC - 1 ) + PC ) & 0xFF ) + ( PC & 0xFF00 );
destination--;
if ( destination != 0 ) PC = source;
WriteD( BANKPSW + _R2_, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_DB( )
// Instruction "DJNZ R3,reladdr" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_DB( )
{
unsigned char destination = ReadD( BANKPSW + _R3_ );
PC++;
unsigned int source = ( ( PGMMem->Read8( PC - 1 ) + PC ) & 0xFF ) + ( PC & 0xFF00 );
destination--;
if ( destination != 0 ) PC = source;
WriteD( BANKPSW + _R3_, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_DC( )
// Instruction "DJNZ R4,reladdr" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_DC( )
{
unsigned char destination = ReadD( BANKPSW + _R4_ );
PC++;
unsigned int source = ( ( PGMMem->Read8( PC - 1 ) + PC ) & 0xFF ) + ( PC & 0xFF00 );
destination--;
if ( destination != 0 ) PC = source;
WriteD( BANKPSW + _R4_, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_DD( )
// Instruction "DJNZ R5,reladdr" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_DD( )
{
unsigned char destination = ReadD( BANKPSW + _R5_ );
PC++;
unsigned int source = ( ( PGMMem->Read8( PC - 1 ) + PC ) & 0xFF ) + ( PC & 0xFF00 );
destination--;
if ( destination != 0 ) PC = source;
WriteD( BANKPSW + _R5_, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_DE( )
// Instruction "DJNZ R6,reladdr" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_DE( )
{
unsigned char destination = ReadD( BANKPSW + _R6_ );
PC++;
unsigned int source = ( ( PGMMem->Read8( PC - 1 ) + PC ) & 0xFF ) + ( PC & 0xFF00 );
destination--;
if ( destination != 0 ) PC = source;
WriteD( BANKPSW + _R6_, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_DF( )
// Instruction "DJNZ R7,reladdr" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_DF( )
{
unsigned char destination = ReadD( BANKPSW + _R7_ );
PC++;
unsigned int source = ( ( PGMMem->Read8( PC - 1 ) + PC ) & 0xFF ) + ( PC & 0xFF00 );
destination--;
if ( destination != 0 ) PC = source;
WriteD( BANKPSW + _R7_, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_E0( )
// Instruction "MOVX A,@DPTR" takes 2 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_E0( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadI( ( ReadD( _DPTRHIGH_ ) << 8 ) + ReadD( _DPTRLOW_) );
destination = source;
WriteD( _ACC_, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_E1( )
// Instruction "AJMP addr11" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_E1( )
{
unsigned int addr11 = ( ( PGMMem->Read8( PC - 1 ) << 3 ) & 0xF00 ) + PGMMem->Read8( PC++ );
PC = ( PC & 0xF800 ) | addr11;
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_E2( )
// Instruction "MOVX A,@R0" takes 2 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_E2( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadI ( ReadD( BANKPSW + _R0_ ) );
destination = source;
WriteD( _ACC_, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_E3( )
// Instruction "MOVX A,@R1" takes 2 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_E3( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadI ( ReadD( BANKPSW + _R1_ ) );
destination = source;
WriteD( _ACC_, destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_E4( )
// Instruction "CLR A" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_E4( )
{
unsigned char destination = ReadD( _ACC_ );
destination = 0;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_E5( )
// Instruction "MOV A,direct" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_E5( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char srcaddr = PGMMem->Read8( PC++ );
unsigned char source = ReadD( srcaddr );
destination = source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_E6( )
// Instruction "MOV A,@R0" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_E6( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadI ( ReadD( BANKPSW + _R0_ ) );
destination = source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_E7( )
// Instruction "MOV A,@R1" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_E7( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadI ( ReadD( BANKPSW + _R1_ ) );
destination = source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_E8( )
// Instruction "MOV A,R0" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_E8( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R0_ );
destination = source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_E9( )
// Instruction "MOV A,R1" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_E9( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R1_ );
destination = source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_EA( )
// Instruction "MOV A,R2" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_EA( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R2_ );
destination = source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_EB( )
// Instruction "MOV A,R3" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_EB( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R3_ );
destination = source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_EC( )
// Instruction "MOV A,R4" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_EC( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R4_ );
destination = source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_ED( )
// Instruction "MOV A,R5" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_ED( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R5_ );
destination = source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_EE( )
// Instruction "MOV A,R6" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_EE( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R6_ );
destination = source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_EF( )
// Instruction "MOV A,R7" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_EF( )
{
unsigned char destination = ReadD( _ACC_ );
unsigned char source = ReadD( BANKPSW + _R7_ );
destination = source;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_F0( )
// Instruction "MOVX @DPTR,A" takes 2 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_F0( )
{
unsigned char destination = ReadI( ( ReadD( _DPTRHIGH_ ) << 8 ) + ReadD( _DPTRLOW_) );
unsigned char source = ReadD( _ACC_ );
destination = source;
WriteI( ( ReadD( _DPTRHIGH_ ) << 8 ) + ReadD( _DPTRLOW_ ), destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_F1( )
// Instruction "ACALL addr11" takes 2 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_F1( )
{
unsigned int addr11 = ( ( PGMMem->Read8( PC - 1 ) << 3 ) & 0xF00 ) + PGMMem->Read8( PC++ );
unsigned char SP = ReadD( _SP_ );
WriteI( ++SP, ( PC & 0x00FF ) );
WriteI( ++SP, ( PC >> 8 ) );
WriteD( _SP_, SP );
PC = ( PC & 0xF800 ) | addr11;
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_F2( )
// Instruction "MOVX @R0,A" takes 2 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_F2( )
{
unsigned char destination = ReadI ( ReadD( BANKPSW + _R0_ ) );
unsigned char source = ReadD( _ACC_ );
destination = source;
WriteI( ReadD( BANKPSW + _R0_ ), destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_F3( )
// Instruction "MOVX @R1,A" takes 2 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_F3( )
{
unsigned char destination = ReadI ( ReadD( BANKPSW + _R1_ ) );
unsigned char source = ReadD( _ACC_ );
destination = source;
WriteI( ReadD( BANKPSW + _R1_ ), destination );
return 2;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_F4( )
// Instruction "CPL A" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_F4( )
{
unsigned char destination = ReadD( _ACC_ );
destination ^= 0xFF;
WriteD( _ACC_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_F5( )
// Instruction "MOV direct,A" takes 1 cycle(s) and 2 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_F5( )
{
unsigned char destaddr = PGMMem->Read8( PC++ );
unsigned char destination = ReadD( destaddr );
unsigned char source = ReadD( _ACC_ );
destination = source;
WriteD( destaddr, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_F6( )
// Instruction "MOV @R0,A" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_F6( )
{
unsigned char destination = ReadI ( ReadD( BANKPSW + _R0_ ) );
unsigned char source = ReadD( _ACC_ );
destination = source;
WriteI( ReadD( BANKPSW + _R0_ ), destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_F7( )
// Instruction "MOV @R1,A" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_F7( )
{
unsigned char destination = ReadI ( ReadD( BANKPSW + _R1_ ) );
unsigned char source = ReadD( _ACC_ );
destination = source;
WriteI( ReadD( BANKPSW + _R1_ ), destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_F8( )
// Instruction "MOV R0,A" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_F8( )
{
unsigned char destination = ReadD( BANKPSW + _R0_ );
unsigned char source = ReadD( _ACC_ );
destination = source;
WriteD( BANKPSW + _R0_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_F9( )
// Instruction "MOV R1,A" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_F9( )
{
unsigned char destination = ReadD( BANKPSW + _R1_ );
unsigned char source = ReadD( _ACC_ );
destination = source;
WriteD( BANKPSW + _R1_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_FA( )
// Instruction "MOV R2,A" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_FA( )
{
unsigned char destination = ReadD( BANKPSW + _R2_ );
unsigned char source = ReadD( _ACC_ );
destination = source;
WriteD( BANKPSW + _R2_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_FB( )
// Instruction "MOV R3,A" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_FB( )
{
unsigned char destination = ReadD( BANKPSW + _R3_ );
unsigned char source = ReadD( _ACC_ );
destination = source;
WriteD( BANKPSW + _R3_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_FC( )
// Instruction "MOV R4,A" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_FC( )
{
unsigned char destination = ReadD( BANKPSW + _R4_ );
unsigned char source = ReadD( _ACC_ );
destination = source;
WriteD( BANKPSW + _R4_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_FD( )
// Instruction "MOV R5,A" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_FD( )
{
unsigned char destination = ReadD( BANKPSW + _R5_ );
unsigned char source = ReadD( _ACC_ );
destination = source;
WriteD( BANKPSW + _R5_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_FE( )
// Instruction "MOV R6,A" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_FE( )
{
unsigned char destination = ReadD( BANKPSW + _R6_ );
unsigned char source = ReadD( _ACC_ );
destination = source;
WriteD( BANKPSW + _R6_, destination );
return 1;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::OP_FF( )
// Instruction "MOV R7,A" takes 1 cycle(s) and 1 byte(s)
//////////////////////////////////////////////////////////////////////////////
int CPU8051::OP_FF( )
{
unsigned char destination = ReadD( BANKPSW + _R7_ );
unsigned char source = ReadD( _ACC_ );
destination = source;
WriteD( BANKPSW + _R7_, destination );
return 1;
}




//////////////////////////////////////////////////////////////////////////////
// void CPU8051::InitFuncPtr( )
// Initialize Functions Pointers
//////////////////////////////////////////////////////////////////////////////
void CPU8051::InitFuncPtr( )
{
 funcptr[0]=&CPU8051::OP_00;
 funcptr[1]=&CPU8051::OP_01;
 funcptr[2]=&CPU8051::OP_02;
 funcptr[3]=&CPU8051::OP_03;
 funcptr[4]=&CPU8051::OP_04;
 funcptr[5]=&CPU8051::OP_05;
 funcptr[6]=&CPU8051::OP_06;
 funcptr[7]=&CPU8051::OP_07;
 funcptr[8]=&CPU8051::OP_08;
 funcptr[9]=&CPU8051::OP_09;
 funcptr[10]=&CPU8051::OP_0A;
 funcptr[11]=&CPU8051::OP_0B;
 funcptr[12]=&CPU8051::OP_0C;
 funcptr[13]=&CPU8051::OP_0D;
 funcptr[14]=&CPU8051::OP_0E;
 funcptr[15]=&CPU8051::OP_0F;
 funcptr[16]=&CPU8051::OP_10;
 funcptr[17]=&CPU8051::OP_11;
 funcptr[18]=&CPU8051::OP_12;
 funcptr[19]=&CPU8051::OP_13;
 funcptr[20]=&CPU8051::OP_14;
 funcptr[21]=&CPU8051::OP_15;
 funcptr[22]=&CPU8051::OP_16;
 funcptr[23]=&CPU8051::OP_17;
 funcptr[24]=&CPU8051::OP_18;
 funcptr[25]=&CPU8051::OP_19;
 funcptr[26]=&CPU8051::OP_1A;
 funcptr[27]=&CPU8051::OP_1B;
 funcptr[28]=&CPU8051::OP_1C;
 funcptr[29]=&CPU8051::OP_1D;
 funcptr[30]=&CPU8051::OP_1E;
 funcptr[31]=&CPU8051::OP_1F;
 funcptr[32]=&CPU8051::OP_20;
 funcptr[33]=&CPU8051::OP_21;
 funcptr[34]=&CPU8051::OP_22;
 funcptr[35]=&CPU8051::OP_23;
 funcptr[36]=&CPU8051::OP_24;
 funcptr[37]=&CPU8051::OP_25;
 funcptr[38]=&CPU8051::OP_26;
 funcptr[39]=&CPU8051::OP_27;
 funcptr[40]=&CPU8051::OP_28;
 funcptr[41]=&CPU8051::OP_29;
 funcptr[42]=&CPU8051::OP_2A;
 funcptr[43]=&CPU8051::OP_2B;
 funcptr[44]=&CPU8051::OP_2C;
 funcptr[45]=&CPU8051::OP_2D;
 funcptr[46]=&CPU8051::OP_2E;
 funcptr[47]=&CPU8051::OP_2F;
 funcptr[48]=&CPU8051::OP_30;
 funcptr[49]=&CPU8051::OP_31;
 funcptr[50]=&CPU8051::OP_32;
 funcptr[51]=&CPU8051::OP_33;
 funcptr[52]=&CPU8051::OP_34;
 funcptr[53]=&CPU8051::OP_35;
 funcptr[54]=&CPU8051::OP_36;
 funcptr[55]=&CPU8051::OP_37;
 funcptr[56]=&CPU8051::OP_38;
 funcptr[57]=&CPU8051::OP_39;
 funcptr[58]=&CPU8051::OP_3A;
 funcptr[59]=&CPU8051::OP_3B;
 funcptr[60]=&CPU8051::OP_3C;
 funcptr[61]=&CPU8051::OP_3D;
 funcptr[62]=&CPU8051::OP_3E;
 funcptr[63]=&CPU8051::OP_3F;
 funcptr[64]=&CPU8051::OP_40;
 funcptr[65]=&CPU8051::OP_41;
 funcptr[66]=&CPU8051::OP_42;
 funcptr[67]=&CPU8051::OP_43;
 funcptr[68]=&CPU8051::OP_44;
 funcptr[69]=&CPU8051::OP_45;
 funcptr[70]=&CPU8051::OP_46;
 funcptr[71]=&CPU8051::OP_47;
 funcptr[72]=&CPU8051::OP_48;
 funcptr[73]=&CPU8051::OP_49;
 funcptr[74]=&CPU8051::OP_4A;
 funcptr[75]=&CPU8051::OP_4B;
 funcptr[76]=&CPU8051::OP_4C;
 funcptr[77]=&CPU8051::OP_4D;
 funcptr[78]=&CPU8051::OP_4E;
 funcptr[79]=&CPU8051::OP_4F;
 funcptr[80]=&CPU8051::OP_50;
 funcptr[81]=&CPU8051::OP_51;
 funcptr[82]=&CPU8051::OP_52;
 funcptr[83]=&CPU8051::OP_53;
 funcptr[84]=&CPU8051::OP_54;
 funcptr[85]=&CPU8051::OP_55;
 funcptr[86]=&CPU8051::OP_56;
 funcptr[87]=&CPU8051::OP_57;
 funcptr[88]=&CPU8051::OP_58;
 funcptr[89]=&CPU8051::OP_59;
 funcptr[90]=&CPU8051::OP_5A;
 funcptr[91]=&CPU8051::OP_5B;
 funcptr[92]=&CPU8051::OP_5C;
 funcptr[93]=&CPU8051::OP_5D;
 funcptr[94]=&CPU8051::OP_5E;
 funcptr[95]=&CPU8051::OP_5F;
 funcptr[96]=&CPU8051::OP_60;
 funcptr[97]=&CPU8051::OP_61;
 funcptr[98]=&CPU8051::OP_62;
 funcptr[99]=&CPU8051::OP_63;
 funcptr[100]=&CPU8051::OP_64;
 funcptr[101]=&CPU8051::OP_65;
 funcptr[102]=&CPU8051::OP_66;
 funcptr[103]=&CPU8051::OP_67;
 funcptr[104]=&CPU8051::OP_68;
 funcptr[105]=&CPU8051::OP_69;
 funcptr[106]=&CPU8051::OP_6A;
 funcptr[107]=&CPU8051::OP_6B;
 funcptr[108]=&CPU8051::OP_6C;
 funcptr[109]=&CPU8051::OP_6D;
 funcptr[110]=&CPU8051::OP_6E;
 funcptr[111]=&CPU8051::OP_6F;
 funcptr[112]=&CPU8051::OP_70;
 funcptr[113]=&CPU8051::OP_71;
 funcptr[114]=&CPU8051::OP_72;
 funcptr[115]=&CPU8051::OP_73;
 funcptr[116]=&CPU8051::OP_74;
 funcptr[117]=&CPU8051::OP_75;
 funcptr[118]=&CPU8051::OP_76;
 funcptr[119]=&CPU8051::OP_77;
 funcptr[120]=&CPU8051::OP_78;
 funcptr[121]=&CPU8051::OP_79;
 funcptr[122]=&CPU8051::OP_7A;
 funcptr[123]=&CPU8051::OP_7B;
 funcptr[124]=&CPU8051::OP_7C;
 funcptr[125]=&CPU8051::OP_7D;
 funcptr[126]=&CPU8051::OP_7E;
 funcptr[127]=&CPU8051::OP_7F;
 funcptr[128]=&CPU8051::OP_80;
 funcptr[129]=&CPU8051::OP_81;
 funcptr[130]=&CPU8051::OP_82;
 funcptr[131]=&CPU8051::OP_83;
 funcptr[132]=&CPU8051::OP_84;
 funcptr[133]=&CPU8051::OP_85;
 funcptr[134]=&CPU8051::OP_86;
 funcptr[135]=&CPU8051::OP_87;
 funcptr[136]=&CPU8051::OP_88;
 funcptr[137]=&CPU8051::OP_89;
 funcptr[138]=&CPU8051::OP_8A;
 funcptr[139]=&CPU8051::OP_8B;
 funcptr[140]=&CPU8051::OP_8C;
 funcptr[141]=&CPU8051::OP_8D;
 funcptr[142]=&CPU8051::OP_8E;
 funcptr[143]=&CPU8051::OP_8F;
 funcptr[144]=&CPU8051::OP_90;
 funcptr[145]=&CPU8051::OP_91;
 funcptr[146]=&CPU8051::OP_92;
 funcptr[147]=&CPU8051::OP_93;
 funcptr[148]=&CPU8051::OP_94;
 funcptr[149]=&CPU8051::OP_95;
 funcptr[150]=&CPU8051::OP_96;
 funcptr[151]=&CPU8051::OP_97;
 funcptr[152]=&CPU8051::OP_98;
 funcptr[153]=&CPU8051::OP_99;
 funcptr[154]=&CPU8051::OP_9A;
 funcptr[155]=&CPU8051::OP_9B;
 funcptr[156]=&CPU8051::OP_9C;
 funcptr[157]=&CPU8051::OP_9D;
 funcptr[158]=&CPU8051::OP_9E;
 funcptr[159]=&CPU8051::OP_9F;
 funcptr[160]=&CPU8051::OP_A0;
 funcptr[161]=&CPU8051::OP_A1;
 funcptr[162]=&CPU8051::OP_A2;
 funcptr[163]=&CPU8051::OP_A3;
 funcptr[164]=&CPU8051::OP_A4;
 funcptr[165]=&CPU8051::OP_A5;
 funcptr[166]=&CPU8051::OP_A6;
 funcptr[167]=&CPU8051::OP_A7;
 funcptr[168]=&CPU8051::OP_A8;
 funcptr[169]=&CPU8051::OP_A9;
 funcptr[170]=&CPU8051::OP_AA;
 funcptr[171]=&CPU8051::OP_AB;
 funcptr[172]=&CPU8051::OP_AC;
 funcptr[173]=&CPU8051::OP_AD;
 funcptr[174]=&CPU8051::OP_AE;
 funcptr[175]=&CPU8051::OP_AF;
 funcptr[176]=&CPU8051::OP_B0;
 funcptr[177]=&CPU8051::OP_B1;
 funcptr[178]=&CPU8051::OP_B2;
 funcptr[179]=&CPU8051::OP_B3;
 funcptr[180]=&CPU8051::OP_B4;
 funcptr[181]=&CPU8051::OP_B5;
 funcptr[182]=&CPU8051::OP_B6;
 funcptr[183]=&CPU8051::OP_B7;
 funcptr[184]=&CPU8051::OP_B8;
 funcptr[185]=&CPU8051::OP_B9;
 funcptr[186]=&CPU8051::OP_BA;
 funcptr[187]=&CPU8051::OP_BB;
 funcptr[188]=&CPU8051::OP_BC;
 funcptr[189]=&CPU8051::OP_BD;
 funcptr[190]=&CPU8051::OP_BE;
 funcptr[191]=&CPU8051::OP_BF;
 funcptr[192]=&CPU8051::OP_C0;
 funcptr[193]=&CPU8051::OP_C1;
 funcptr[194]=&CPU8051::OP_C2;
 funcptr[195]=&CPU8051::OP_C3;
 funcptr[196]=&CPU8051::OP_C4;
 funcptr[197]=&CPU8051::OP_C5;
 funcptr[198]=&CPU8051::OP_C6;
 funcptr[199]=&CPU8051::OP_C7;
 funcptr[200]=&CPU8051::OP_C8;
 funcptr[201]=&CPU8051::OP_C9;
 funcptr[202]=&CPU8051::OP_CA;
 funcptr[203]=&CPU8051::OP_CB;
 funcptr[204]=&CPU8051::OP_CC;
 funcptr[205]=&CPU8051::OP_CD;
 funcptr[206]=&CPU8051::OP_CE;
 funcptr[207]=&CPU8051::OP_CF;
 funcptr[208]=&CPU8051::OP_D0;
 funcptr[209]=&CPU8051::OP_D1;
 funcptr[210]=&CPU8051::OP_D2;
 funcptr[211]=&CPU8051::OP_D3;
 funcptr[212]=&CPU8051::OP_D4;
 funcptr[213]=&CPU8051::OP_D5;
 funcptr[214]=&CPU8051::OP_D6;
 funcptr[215]=&CPU8051::OP_D7;
 funcptr[216]=&CPU8051::OP_D8;
 funcptr[217]=&CPU8051::OP_D9;
 funcptr[218]=&CPU8051::OP_DA;
 funcptr[219]=&CPU8051::OP_DB;
 funcptr[220]=&CPU8051::OP_DC;
 funcptr[221]=&CPU8051::OP_DD;
 funcptr[222]=&CPU8051::OP_DE;
 funcptr[223]=&CPU8051::OP_DF;
 funcptr[224]=&CPU8051::OP_E0;
 funcptr[225]=&CPU8051::OP_E1;
 funcptr[226]=&CPU8051::OP_E2;
 funcptr[227]=&CPU8051::OP_E3;
 funcptr[228]=&CPU8051::OP_E4;
 funcptr[229]=&CPU8051::OP_E5;
 funcptr[230]=&CPU8051::OP_E6;
 funcptr[231]=&CPU8051::OP_E7;
 funcptr[232]=&CPU8051::OP_E8;
 funcptr[233]=&CPU8051::OP_E9;
 funcptr[234]=&CPU8051::OP_EA;
 funcptr[235]=&CPU8051::OP_EB;
 funcptr[236]=&CPU8051::OP_EC;
 funcptr[237]=&CPU8051::OP_ED;
 funcptr[238]=&CPU8051::OP_EE;
 funcptr[239]=&CPU8051::OP_EF;
 funcptr[240]=&CPU8051::OP_F0;
 funcptr[241]=&CPU8051::OP_F1;
 funcptr[242]=&CPU8051::OP_F2;
 funcptr[243]=&CPU8051::OP_F3;
 funcptr[244]=&CPU8051::OP_F4;
 funcptr[245]=&CPU8051::OP_F5;
 funcptr[246]=&CPU8051::OP_F6;
 funcptr[247]=&CPU8051::OP_F7;
 funcptr[248]=&CPU8051::OP_F8;
 funcptr[249]=&CPU8051::OP_F9;
 funcptr[250]=&CPU8051::OP_FA;
 funcptr[251]=&CPU8051::OP_FB;
 funcptr[252]=&CPU8051::OP_FC;
 funcptr[253]=&CPU8051::OP_FD;
 funcptr[254]=&CPU8051::OP_FE;
 funcptr[255]=&CPU8051::OP_FF;

}


#endif
