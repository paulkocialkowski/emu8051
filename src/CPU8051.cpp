// CPU8051.cpp

#include <stdio.h>
#include <iostream>
#include "CPU8051.hpp"
#include "disasm.hpp"

//////////////////////////////////////////////////////////////////////////////
// CPU8051::CPU8051( )
// CPU8051 constructor
//////////////////////////////////////////////////////////////////////////////
CPU8051::CPU8051( )
{
  InitFuncPtr( );
  // Cree les objets Memory
  SFRMem = new Memory( 128 );
  PGMMem = new Memory( 65536 );
  IntMem = new Memory( 128 );
  ExtMem = new Memory( 65536 );
  PC = 0; CLOCK = 0; ActivePriority = -1;
}

//////////////////////////////////////////////////////////////////////////////
// CPU8051::~CPU8051( )
// CPU8051 destructor
//////////////////////////////////////////////////////////////////////////////
CPU8051::~CPU8051( )
{
  // Detruit les objets Memory
  delete SFRMem;
  delete PGMMem;
  delete IntMem;
  delete ExtMem;

  SFRMem = 0;
  PGMMem = 0;
  IntMem = 0;
  ExtMem = 0;
  PC = 0;
  CLOCK = 0;
}

//////////////////////////////////////////////////////////////////////////////
// void CPU8051::Exec( )
// Execute at address PC from PGMMem
//////////////////////////////////////////////////////////////////////////////
void CPU8051::Exec( )
{
int i;
unsigned char opcode = PGMMem->Read8( PC++ );
int insttiming = ( this->*funcptr[ opcode ] )();

for ( i = 0; i < insttiming; i++)
  {
    CheckInterrupts();
    DoTimers();
    CLOCK++;    
  }
}

//////////////////////////////////////////////////////////////////////////////
// unsigned int CPU8051::GetNextAddress( )
// Return PC + size in bytes of current instruction
//////////////////////////////////////////////////////////////////////////////
unsigned int CPU8051::GetNextAddress( )
{
return ( PC + InstSizesTbl[ PGMMem->Read8( PC ) ] );
}

//////////////////////////////////////////////////////////////////////////////
// void CPU8051::Reset( )
// Reset the registers and CPU state
//////////////////////////////////////////////////////////////////////////////
void CPU8051::Reset( )
{
  PC = 0; CLOCK = 0; ActivePriority = -1;
  // Reinitialisation des registres
  int i;
  for ( i = 0; i < 128; i++ )
    {
      SFRMem->Write8( i, 0 );
      IntMem->Write8( i, 0 );
    }
  SFRMem->Write8( _P0_ - 0x80, 0xFF );
  SFRMem->Write8( _P1_ - 0x80, 0xFF );
  SFRMem->Write8( _P2_ - 0x80, 0xFF );
  SFRMem->Write8( _P3_ - 0x80, 0xFF );
  SFRMem->Write8( _SP_ - 0x80, 0x07 );
}

//////////////////////////////////////////////////////////////////////////////
// unsigned int CPU8051::GetPC( )
// Return the value of PC register
//////////////////////////////////////////////////////////////////////////////
unsigned int CPU8051::GetPC( )
{
  return PC;
}

//////////////////////////////////////////////////////////////////////////////
// void CPU8051::SetPC( unsigned int NewPC )
// Set the new value of PC register
//////////////////////////////////////////////////////////////////////////////
void CPU8051::SetPC( unsigned int NewPC )
{
  PC = NewPC;
}

//////////////////////////////////////////////////////////////////////////////
// void CPU8051::WriteD( unsigned int Address, unsigned char Value )
// Write with a direct addressing mode at Address the new Value
//////////////////////////////////////////////////////////////////////////////
void CPU8051::WriteD( unsigned int Address, unsigned char Value )
{
  if ( Address > 0x7F ) { SFRMem->Write8( Address - 0x80, Value ); return; }
  IntMem->Write8( Address, Value );
}

//////////////////////////////////////////////////////////////////////////////
// void CPU8051::WriteExt( unsigned int Address, unsigned char Value )
// Ecriture d'une valeur dans la memoire externe ( Address = $00 a $FFFF )
//////////////////////////////////////////////////////////////////////////////
void CPU8051::WriteExt( unsigned int Address, unsigned char Value )
{
  ExtMem->Write8( Address, Value );
  return;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::WriteInt( unsigned int Address, unsigned char Value )
// Ecriture d'une valeur dans la memoire interne ( Address = $00 a $FF )
//////////////////////////////////////////////////////////////////////////////
void CPU8051::WriteInt( unsigned int Address, unsigned char Value )
{
  if ( Address > 0x7F )
    SFRMem->Write8( Address - 0x80, Value );
  else
    IntMem->Write8( Address, Value );
  return;
}


//////////////////////////////////////////////////////////////////////////////
// void CPU8051::WriteI( unsigned int Address, unsigned char Value )
// Write with an indirect addressing mode at Address the new Value
//////////////////////////////////////////////////////////////////////////////
void CPU8051::WriteI( unsigned int Address, unsigned char Value )
{
  if ( Address > 0x7F ) { ExtMem->Write8( Address, Value ); return; }
  IntMem->Write8( Address, Value );
}

//////////////////////////////////////////////////////////////////////////////
// void CPU8051::WritePGM( unsigned int Address, unsigned char Value )
// Write at Address the new Value in PGMMem
//////////////////////////////////////////////////////////////////////////////
void CPU8051::WritePGM( unsigned int Address, unsigned char Value )
{
  PGMMem->Write8( Address, Value );
}

//////////////////////////////////////////////////////////////////////////////
// unsigned char CPU8051::ReadD( unsigned int Address )
// Read with a direct addressing mode at Address
//////////////////////////////////////////////////////////////////////////////
unsigned char CPU8051::ReadD( unsigned int Address )
{
  if ( Address > 0xFF ) return ExtMem->Read8( Address );
  if ( Address > 0x7F ) return SFRMem->Read8( Address - 0x80 );
  return IntMem->Read8( Address );
}


//////////////////////////////////////////////////////////////////////////////
// unsigned char CPU8051::ReadInt( unsigned int Address )
// Read Internal data memory at Address
//////////////////////////////////////////////////////////////////////////////
unsigned char CPU8051::ReadInt( unsigned int Address )
{
  if ( Address > 0x7F )
    return SFRMem->Read8( Address - 0x80 );
  return IntMem->Read8( Address );
}

//////////////////////////////////////////////////////////////////////////////
// unsigned char CPU8051::ReadExt( unsigned int Address )
// Lecture du contenu de la memoire externe
//////////////////////////////////////////////////////////////////////////////
unsigned char CPU8051::ReadExt( unsigned int Address )
{
  return ExtMem->Read8( Address );
}


//////////////////////////////////////////////////////////////////////////////
// unsigned char CPU8051::ReadI( unsigned int Address )
// Read with a indirect addressing mode at Address
//////////////////////////////////////////////////////////////////////////////
unsigned char CPU8051::ReadI( unsigned int Address )
{
  if ( Address > 0x7F ) return ExtMem->Read8( Address );
  return IntMem->Read8( Address );
}

//////////////////////////////////////////////////////////////////////////////
// unsigned char CPU8051::ReadPGM( unsigned int Address )
// Read at Address from PGMMem
//////////////////////////////////////////////////////////////////////////////
unsigned char CPU8051::ReadPGM( unsigned int Address )
{
  return PGMMem->Read8( Address );
}

//////////////////////////////////////////////////////////////////////////////
// void CPU8051::WriteB( unsigned int BitAddress, unsigned char Value )
// Write with a bit addressing mode at BitAddress the new Value
//////////////////////////////////////////////////////////////////////////////
void CPU8051::WriteB( unsigned int BitAddress, unsigned char Value )
{
unsigned int ByteAddress, BitNumber;
unsigned char ByteValue, ByteMask;
  if ( BitAddress > 0x7F ) {
    // SFR 80-FF
    ByteAddress = BitAddress & 0xF8;
    BitNumber = BitAddress & 0x07;
  }
  else {
    // 20-2F
    ByteAddress = ( BitAddress >> 3 ) + 0x20;
    BitNumber = BitAddress & 0x07;
  }
  ByteMask = ( ( 1 << BitNumber ) ^ 0xFF );
  ByteValue = ReadD( ByteAddress ) & ByteMask;
  ByteValue += Value << BitNumber;
  WriteD( ByteAddress, ByteValue );
}


//////////////////////////////////////////////////////////////////////////////
// unsigned char CPU8051::ReadB( unsigned int BitAddress )
// Read with a bit addressing mode at BitAddress
//////////////////////////////////////////////////////////////////////////////
unsigned char CPU8051::ReadB( unsigned int BitAddress )
{
unsigned int ByteAddress, BitNumber;
unsigned char BitValue;
  if ( BitAddress > 0x7F ) {
    // SFR 80-FF
    ByteAddress = BitAddress & 0xF8;
    BitNumber = BitAddress & 0x07;
  }
  else {
    // 20-2F
    ByteAddress = ( BitAddress >> 3 ) + 0x20;
    BitNumber = BitAddress & 0x07;
  }
  BitValue = ( ReadD( ByteAddress ) >> BitNumber );
  BitValue &= 1;
  return BitValue;
}

//////////////////////////////////////////////////////////////////////////////
// void CPU8051::CheckInterrupts()
// Check interrupts state and process them as needed
//////////////////////////////////////////////////////////////////////////////
void CPU8051::CheckInterrupts()
{
unsigned char SP;

  if ( ReadD( _IE_ ) & 0x80 ) {
    for ( int i = 1; i >= 0; i-- )
      if ( ActivePriority < i ) {
//-------------------------  External interrupt 0 ----------------------------     
//        if ( ( ReadD( _IE_ ) & 0x01 ) && ( ( ReadD( _IP_ ) & 0x01 ) ? i : !i ) && pin0 )
//-------------------------- Interrupt timer 0 -------------------------------
          if ( ( ReadD( _IE_ ) & 0x02 ) && ( ( ReadD( _IP_ & 0x02 ) ? i : !i ) && ( ReadD( _TCON_ ) & 0x20 ) ) ){
              WriteD( _TCON_, ReadD( _TCON_ ) & 0xDF );
	      SP = ReadD( _SP_ );
	      WriteI( ++SP, ( PC & 0xFF ) );
	      WriteI( ++SP, ( PC >> 8 ) );
	      WriteD( _SP_, SP );
              PC = 0x0B;  
              ActivePriority = i;
              return;          
          }
//-------------------------- External interrupt 1 ----------------------------     
//        if ( ( ReadD( _IE_ ) & 0x04 ) && ( ( ReadD( _IP_ ) & 0x04 ) ? i : !i ) && pin1 )
//-------------------------- Interrupt timer 1 -------------------------------      
          if ( ( ReadD( _IE_ ) & 0x08 ) && ( ( ReadD( _IP_ ) & 0x08 ) ? i : !i ) && ( ReadD( _TCON_ ) & 0x80 ) ) {
              WriteD( _TCON_, ReadD( _TCON_ ) & 0x7F );
	      SP = ReadD( _SP_ );
	      WriteI( ++SP, ( PC & 0xFF ) );
	      WriteI( ++SP, ( PC >> 8 ) );
	      WriteD( _SP_, SP );
	      PC = 0x1B;
              ActivePriority = i;
              return;
          }
//-------------------------- Serial Interrupts -------------------------------
          if ( ( ReadD( _IE_ ) & 0x10 ) && ( ( ReadD( _IP_ ) & 0x10 ) ? i : !i ) && ( ReadD( _SCON_ ) & 0x03 ) ) {
	      SP = ReadD( _SP_ );
	      WriteI( ++SP, ( PC & 0xFF ) );
	      WriteI( ++SP, ( PC >> 8 ) );
	      WriteD( _SP_, SP );
              PC = 0x23;
              ActivePriority = i;
              return;
          }
//-------------------------- Interrupt timer 2 -------------------------------
          if ( ( ReadD( _IE_ ) & 0x20 ) && ( ( ReadD( _IP_ ) & 0x20 ) ? i : !i ) && ( ReadD( _T2CON_ ) & 0x80 ) ) {          
	      SP = ReadD( _SP_ );
	      WriteI( ++SP, ( PC & 0xFF ) );
	      WriteI( ++SP, ( PC >> 8 ) );
	      WriteD( _SP_, SP );
              PC = 0x2B;
              ActivePriority = i;
              return;
          }
      }
  }
}

//////////////////////////////////////////////////////////////////////////////
// void CPU8051::DoTimers( )
// Execute les timers
//////////////////////////////////////////////////////////////////////////////
void CPU8051::DoTimers( )
{
  unsigned int tmp;
  unsigned int TR;
  unsigned int MODE;
  unsigned int GATE;
  unsigned int TimerCounter;

  // ----- Timer 0
  TR = ReadD( _TCON_ ) & 0x10;
  MODE = ReadD( _TMOD_ ) & 0x03;
  GATE = ReadD( _TMOD_ ) & 0x08;
  TimerCounter = ReadD( _TMOD_ ) & 0x04;
  
  if ( ( TR && !GATE && !TimerCounter ) || ( MODE == 3 ) )
    switch( MODE ) {
      // Mode 0, compteur de 13 bits.
    case 0 :
      tmp = ReadD( _TH0_ ) * 0x100 + ReadD( _TL0_ );
      ++tmp &= 0x1FFF;   // On ne garde que 13 bits.
      if ( tmp == 0 )    // If overflow set TF0
	WriteD( _TCON_, ReadD( _TCON_ ) | 0x20 );
      WriteD( _TH0_, tmp / 0x100 );
      WriteD( _TL0_, tmp & 0xFF  );
      break;
      
      // Mode 1, compteur de 16 bits.
    case 1 :
      tmp = ReadD( _TH0_ ) * 0x100 + ReadD( _TL0_ );
      ++tmp &= 0xFFFF;   // On ne garde que 16 bits.
      if ( tmp == 0 )   // If overflow set TF0
	WriteD( _TCON_, ReadD( _TCON_ ) | 0x20 );
      WriteD( _TH0_, ( tmp / 0x100 ) );
      WriteD( _TL0_, ( tmp & 0xFF ) );
      break;
      
      // Mode 2, Compteur de 8 bits avec Auto-Reload
    case 2 :
      tmp = ReadD( _TL0_ );
      ++tmp &= 0xFF;
      if ( tmp == 0 ) {    // If overflow -> reload et set TF0
	WriteD( _TCON_, ReadD( _TCON_ ) | 0x20 );
	WriteD( _TL0_, ReadD( _TH0_ ) );
      }
      else
	WriteD( _TL0_, tmp );
      break;
    
    // Mode 3 : TL0 et TH0 sont 2 Timers independants de 8 bits chacuns.
    case 3 :
      if ( TR && !GATE && !TimerCounter ) {
	tmp = ReadD( _TL0_ );
	++tmp &= 0xFF;
	if ( tmp == 0 )  // If TL0 overflow set TF0
	  WriteD( _TCON_, ReadD( _TCON_ ) | 0x20 );
	WriteD( _TL0_, tmp );
      }  // TH0 utilise TR1 et TF1.
      TR = ReadD( _TCON_ ) & 0x40;
      if ( TR ) {
	tmp = ReadD( _TH0_ );
	++tmp &= 0xFF;
	if ( tmp == 0 )  // If TH0 overflow set TF1
	  WriteD( _TCON_, ReadD( _TCON_ ) | 0x80 );  // TF1 = 1.
	WriteD( _TH0_, tmp );
      }
      break;
    };
  

  // ----- Timer 1
  TR = ReadD( _TCON_ ) & 0x40;
  MODE = ( ReadD( _TMOD_ ) & 0x30 ) >> 4 ;
  GATE = ReadD( _TMOD_ ) & 0x80;
  TimerCounter = ReadD( _TMOD_ ) & 0x40;
  
  if ( TR && !GATE && !TimerCounter )
    switch( MODE ) {
      // Mode 0, compteur de 13 bits.
    case 0 :
      tmp = ReadD( _TH1_ ) * 0x100 + ReadD( _TL1_ );
      ++tmp &= 0x1FFF;   // On ne garde que 13 bits.
      if ( tmp == 0 )    // If overflow set TF1
	WriteD( _TCON_, ReadD( _TCON_ ) | 0x80 );
      WriteD( _TH1_, tmp / 0x100 );
      WriteD( _TL1_, tmp & 0xFF  );
      break;
      
      // Mode 1, compteur de 16 bits.
    case 1 :
      tmp = ReadD( _TH1_ ) * 0x100 + ReadD( _TL1_ );
      ++tmp &= 0xFFFF;   // On ne garde que 16 bits.
      if ( tmp == 0 )   // If overflow set TF1
	WriteD( _TCON_, ReadD( _TCON_ ) | 0x80 );
      WriteD( _TH1_, ( tmp / 0x100 ) );
      WriteD( _TL1_, ( tmp & 0xFF ) );
      break;
      
      // Mode 2, Compteur de 8 bits avec Auto-Reload
    case 2 :
      tmp = ReadD( _TL1_ );
      ++tmp &= 0xFF;
      if ( tmp == 0 ) {    // If overflow -> reload et set TF1
	WriteD( _TCON_, ReadD( _TCON_ ) | 0x80 );
	WriteD( _TL1_, ReadD( _TH1_ ) );
      }
      else
	WriteD( _TL1_, tmp );
      break;
      
      // Mode 3 : mode inactif: retient la valeur de TH1 et TL1.
      // Equivalent a TR1 = 0.
    case 3 :
      break;
      
    };
}



// Addressing modes defined in the order as they appear in disasm.hpp
// from table argstext[]
#define ADDR11 0
#define ADDR16 1
#define DIRECT 3
#define BITADDR 14
#define RELADDR 15
#define DATAIMM 16
#define DATA16 22
#define CBITADDR 23

// SFR Memory map [80h - FFh]
// ---------------------------------------------------------------
// F8 |      |      |      |      |      |      |      |      | FF
// F0 |   B  |      |      |      |      |      |      |      | F7
// E8 |      |      |      |      |      |      |      |      | EF
// E0 |  ACC |      |      |      |      |      |      |      | E7
// D8 |      |      |      |      |      |      |      |      | DF
// D0 |  PSW |      |      |      |      |      |      |      | D7
// C8 | T2CON|      |RCAP2L|RCAP2H|  TL2 |  TH2 |      |      | CF
// C0 |      |      |      |      |      |      |      |      | C7
// B8 |  IP  |      |      |      |      |      |      |      | BF
// B0 |  P3  |      |      |      |      |      |      |      | B7
// A8 |  IE  |      |      |      |      |      |      |      | AF
// A0 |  P2  |      |      |      |      |      |      |      | A7
// 98 | SCON | SBUF |      |      |      |      |      |      | 9F
// 90 |  P1  |      |      |      |      |      |      |      | 97
// 88 | TCON | TMOD |  TL0 |  TL1 |  TH0 |  TH1 |      |      | 8F
// 80 |  P0  |  SP  |  DPL |  DPH |      |      |      | PCON | 87
// ---------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////////
// int CPU8051::SFRMemInfo( unsigned int Address, char *Text )
// Return as Text the name of the SFR register at Address if any
//////////////////////////////////////////////////////////////////////////////
    int CPU8051::SFRMemInfo( unsigned int Address, char *Text )
  {
    switch( Address ) {
  case 0x80 : return sprintf( Text, "P0" );
  case 0x81 : return sprintf( Text, "SP" );
  case 0x82 : return sprintf( Text, "DPL" );
  case 0x83 : return sprintf( Text, "DPH" );
  case 0x87 : return sprintf( Text, "PCON" );
  case 0x88 : return sprintf( Text, "TCON" );
  case 0x89 : return sprintf( Text, "TMOD" );
  case 0x8A : return sprintf( Text, "TL0" );
  case 0x8B : return sprintf( Text, "TL1" );
  case 0x8C : return sprintf( Text, "TH0" );
  case 0x8D : return sprintf( Text, "TH1" );
  case 0x90 : return sprintf( Text, "P1" );
  case 0x98 : return sprintf( Text, "SCON" );
  case 0x99 : return sprintf( Text, "SBUF" );
  case 0xA0 : return sprintf( Text, "P2" );
  case 0xA8 : return sprintf( Text, "IE" );
  case 0xB0 : return sprintf( Text, "P3" );
  case 0xB8 : return sprintf( Text, "IP" );
  case 0xC8 : return sprintf( Text, "T2CON" );
  case 0xCA : return sprintf( Text, "RCAP2L" );
  case 0xCB : return sprintf( Text, "RCAP2H" );
  case 0xCC : return sprintf( Text, "TL2" );
  case 0xCD : return sprintf( Text, "TH2" );
  case 0xD0 : return sprintf( Text, "PSW" );
  case 0xE0 : return sprintf( Text, "ACC" );
  case 0xF0 : return sprintf( Text, "B" );
  default : return sprintf( Text, "%.2XH", Address );
  }
}

//////////////////////////////////////////////////////////////////////////////
// void CPU8051::IntMemBitInfo( unsigned int BitAddress, char *Text )
// Return as Text the decoded BitAddress
//////////////////////////////////////////////////////////////////////////////
void CPU8051::IntMemBitInfo( unsigned int BitAddress, char *Text )
{
unsigned int ByteAddress, BitNumber;
int TextLength;
  if ( BitAddress > 0x7F ) {
    // SFR 80-FF
    ByteAddress = BitAddress & 0xF8;
    BitNumber = BitAddress & 0x07;
  }
  else {
    // 20-2F
    ByteAddress = ( BitAddress >> 3 ) + 0x20;
    BitNumber = BitAddress & 0x07;
  }

  TextLength = SFRMemInfo( ByteAddress, Text );
  // sprintf( &Text[ TextLength ], ".%X" );
  // Modified by Hugo Villeneuve to remove compilation warning
  sprintf( &Text[ TextLength ], ".%X", BitAddress );
}


//////////////////////////////////////////////////////////////////////////////
// int CPU8051::Disasm( unsigned int Address, char *Text )
// Disasm one instruction at Address into a Text string
//////////////////////////////////////////////////////////////////////////////
int CPU8051::Disasm( unsigned int Address, char *Text )
{
int TextLength=0;
char TextTmp[20];
unsigned char OpCode;
int ArgTblOfs;
int InstSize;
int i;

OpCode = PGMMem->Read8( Address );
InstSize = InstSizesTbl[ OpCode ];
//printf("%.4X\n", Address);

TextLength += sprintf( Text, " %.4X ", Address );

for (i = 0; i < InstSize; i++ )
  TextLength += sprintf( &Text[TextLength], " %.2X", PGMMem->Read8( Address + i ) );

Address++;

for (; TextLength < 17; ) TextLength += sprintf( &Text[ TextLength ], " " );

TextLength += sprintf( &Text[ TextLength ], "%s ", InstTextTbl[ InstTypesTbl[ OpCode ] ] );
ArgTblOfs = OpCode << 2;

for (; TextLength < 25; ) TextLength += sprintf( &Text[ TextLength ], " " );

 // MOV direct, direct (OpCode 85h) is peculiar, the operands are inverted
 if ( OpCode == 0x85 ) {
   SFRMemInfo( PGMMem->Read8( Address + 1 ), TextTmp );
   TextLength += sprintf( &Text[ TextLength ], "%s,", TextTmp );
   SFRMemInfo( PGMMem->Read8( Address ), TextTmp );
   TextLength += sprintf( &Text[ TextLength ], "%s", TextTmp );
   Address += 2;
   return InstSize;
 }

 for ( i = 1; i <= InstArgTbl[ ArgTblOfs ]; i++ ) {
   switch( InstArgTbl[ ArgTblOfs + i ] ) {
   case ADDR11  : {
     TextLength += sprintf( &Text[ TextLength ], "%.4XH", ( ( OpCode << 3) & 0xF00 ) + ( PGMMem->Read8( Address ) ) );
     Address++;
     break;
   }
   case ADDR16  : {
     TextLength += sprintf( &Text[ TextLength ], "%.4XH", ( ( PGMMem->Read8( Address ) << 8 ) + PGMMem->Read8( Address + 1 ) ) );
     Address += 2;
     break;
   }
   case DIRECT  : {
     SFRMemInfo( PGMMem->Read8( Address ), TextTmp );
     TextLength += sprintf( &Text[ TextLength ], "%s", TextTmp );
     Address++;
     break;
   }
   case BITADDR : {
     IntMemBitInfo( ( PGMMem->Read8( Address ) & 0xF8 ), TextTmp );
     TextLength += sprintf( &Text[ TextLength ], "%s.%X" , TextTmp, ( PGMMem->Read8( Address ) & 7 ) );
     Address++;
     break;
   }
   case RELADDR : {
     Address++;
     TextLength += sprintf( &Text[ TextLength ], "%.4XH", ( Address & 0xFF00 ) + ( ( ( Address & 0xFF ) + PGMMem->Read8( Address - 1 ) ) & 0xFF ) );
     break;
   }
   case DATAIMM : {
     TextLength += sprintf( &Text[ TextLength ], "#%.2XH", PGMMem->Read8( Address ) );
     Address++;
     break;
   }
   case DATA16  : {
     TextLength += sprintf( &Text[ TextLength ],"#%.4XH", ( ( PGMMem->Read8( Address ) << 8 ) + PGMMem->Read8( Address+1 ) ) );
     Address += 2;
     break;
   }
   case CBITADDR : {
     IntMemBitInfo( ( PGMMem->Read8( Address ) & 0xF8 ), TextTmp );
     TextLength += sprintf( &Text[ TextLength ], "/%s.%X", TextTmp, ( PGMMem->Read8( Address ) & 7 ) );
     Address++;
     break;
   }
   default : {
     TextLength += sprintf( &Text[ TextLength ],"%s", ArgsTextTbl[ InstArgTbl[ ArgTblOfs + i ] ] );
   }
   }
   if (i < InstArgTbl[ ArgTblOfs ]) { TextLength += sprintf( &Text[ TextLength ], "," ); }
 }

return InstSize;
}


#include "Inst_Imp.cpp"






