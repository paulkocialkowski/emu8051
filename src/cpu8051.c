/* cpu8051.c */


/* Define only here, for not having extern scope on local variables. */
#define CPU8051_M 1


#include <stdio.h>

#include "reg8051.h"
#include "cpu8051.h"
#include "memory.h"
#include "disasm.h"
#include "instructions_8051.h"


void
cpu8051_init( void )
{
  cpu8051.pc = 0;
  cpu8051.clock = 0;
  cpu8051.active_priority = -1;
}


//////////////////////////////////////////////////////////////////////////////
// Execute at address cpu8051.pc from PGMMem
//////////////////////////////////////////////////////////////////////////////
void
cpu8051_Exec( void )
{
  int i;
  unsigned char opcode;
  int insttiming;

  opcode = memory_read8( PGM_MEM_ID, cpu8051.pc );
  cpu8051.pc++;
  insttiming = (*opcode_table[opcode])(); /* Function callback. */
  
  for( i = 0; i < insttiming; i++ ) {
    cpu8051_CheckInterrupts();
    cpu8051_DoTimers();
    cpu8051.clock++;
  }
}


//////////////////////////////////////////////////////////////////////////////
// Return cpu8051.pc + size in bytes of current instruction
//////////////////////////////////////////////////////////////////////////////
unsigned int
cpu8051_GetNextAddress( void )
{
#ifdef DECPP
  return ( cpu8051.pc + InstSizesTbl[ memory_read8( PGM_MEM_ID, cpu8051.pc ) ] );
#endif
  return 0; /* temp */
}


//////////////////////////////////////////////////////////////////////////////
// Reset the registers and CPU state
//////////////////////////////////////////////////////////////////////////////
void
cpu8051_Reset( void )
{
  cpu8051.pc = 0;
  cpu8051.clock= 0;
  cpu8051.active_priority = -1;

  // Reinitialisation des registres
  int i;
  for ( i = 0; i < 128; i++ ) {

    memory_write8( SFR_MEM_ID, i, 0 );

    memory_write8( INT_MEM_ID, i, 0 );
  }

  memory_write8( SFR_MEM_ID, _P0_ - 0x80, 0xFF );
  memory_write8( SFR_MEM_ID, _P1_ - 0x80, 0xFF );
  memory_write8( SFR_MEM_ID, _P2_ - 0x80, 0xFF );
  memory_write8( SFR_MEM_ID, _P3_ - 0x80, 0xFF );
  memory_write8( SFR_MEM_ID, _SP_ - 0x80, 0x07 );
}


//////////////////////////////////////////////////////////////////////////////
// Write with a direct addressing mode at Address the new Value
//////////////////////////////////////////////////////////////////////////////
void
cpu8051_WriteD( unsigned int Address, unsigned char Value )
{
  if ( Address > 0x7F ) {
    memory_write8( SFR_MEM_ID, Address - 0x80, Value );
    return;
  }
  
  memory_write8( INT_MEM_ID, Address, Value );
}


//////////////////////////////////////////////////////////////////////////////
// Ecriture d'une valeur dans la memoire interne ( Address = $00 a $FF )
//////////////////////////////////////////////////////////////////////////////
void
cpu8051_WriteInt( unsigned int Address, unsigned char Value )
{
  if ( Address > 0x7F ) {
    memory_write8( SFR_MEM_ID, Address - 0x80, Value );
  }
  else {
    memory_write8( INT_MEM_ID, Address, Value );
  }

  return;
}


//////////////////////////////////////////////////////////////////////////////
// Write with an indirect addressing mode at Address the new Value
//////////////////////////////////////////////////////////////////////////////
void
cpu8051_WriteI( unsigned int Address, unsigned char Value )
{
  if ( Address > 0x7F ) {
    memory_write8( EXT_MEM_ID, Address, Value );
    return;
  }

  memory_write8( INT_MEM_ID, Address, Value );
}


//////////////////////////////////////////////////////////////////////////////
// Read with a direct addressing mode at Address
//////////////////////////////////////////////////////////////////////////////
unsigned char
cpu8051_ReadD( unsigned int Address )
{
  if ( Address > 0xFF ) {
    return memory_read8( EXT_MEM_ID, Address );
  }

  if ( Address > 0x7F ) {
    return memory_read8( SFR_MEM_ID, Address - 0x80 );
  }

  return memory_read8( INT_MEM_ID, Address );
}


//////////////////////////////////////////////////////////////////////////////
// Read Internal data memory at Address
//////////////////////////////////////////////////////////////////////////////
unsigned char
cpu8051_ReadInt( unsigned int Address )
{
  if ( Address > 0x7F ) {
    return memory_read8( SFR_MEM_ID, Address - 0x80 );
  }
  else {
    return memory_read8( INT_MEM_ID, Address );
  }
}


//////////////////////////////////////////////////////////////////////////////
// unsigned char cpu8051_ReadI( unsigned int Address )
// Read with a indirect addressing mode at Address
//////////////////////////////////////////////////////////////////////////////
unsigned char
cpu8051_ReadI( unsigned int Address )
{
  if ( Address > 0x7F ) {
    return memory_read8( EXT_MEM_ID, Address );
  }
  else {
    return memory_read8( INT_MEM_ID, Address );
  }
}


//////////////////////////////////////////////////////////////////////////////
// Write with a bit addressing mode at BitAddress the new Value
//////////////////////////////////////////////////////////////////////////////
void
cpu8051_WriteB( unsigned int BitAddress, unsigned char Value )
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
  ByteValue = cpu8051_ReadD( ByteAddress ) & ByteMask;
  ByteValue += Value << BitNumber;
  cpu8051_WriteD( ByteAddress, ByteValue );
}


//////////////////////////////////////////////////////////////////////////////
// Read with a bit addressing mode at BitAddress
//////////////////////////////////////////////////////////////////////////////
unsigned char
cpu8051_ReadB( unsigned int BitAddress )
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
  BitValue = ( cpu8051_ReadD( ByteAddress ) >> BitNumber );
  BitValue &= 1;
  return BitValue;
}


//////////////////////////////////////////////////////////////////////////////
// Check interrupts state and process them as needed
//////////////////////////////////////////////////////////////////////////////
void
cpu8051_CheckInterrupts()
{
  unsigned char SP;
  int i;

  if ( cpu8051_ReadD( _IE_ ) & 0x80 ) {
    for ( i = 1; i >= 0; i-- )
      if ( cpu8051.active_priority < i ) {
	//-------------------------  External interrupt 0 ----------------------------     
	//        if ( ( cpu8051_ReadD( _IE_ ) & 0x01 ) && ( ( cpu8051_ReadD( _IP_ ) & 0x01 ) ? i : !i ) && pin0 )
	//-------------------------- Interrupt timer 0 -------------------------------
	if ( ( cpu8051_ReadD( _IE_ ) & 0x02 ) && ( ( cpu8051_ReadD( _IP_ & 0x02 ) ? i : !i ) && ( cpu8051_ReadD( _TCON_ ) & 0x20 ) ) ){
	  cpu8051_WriteD( _TCON_, cpu8051_ReadD( _TCON_ ) & 0xDF );
	  SP = cpu8051_ReadD( _SP_ );
	  cpu8051_WriteI( ++SP, ( cpu8051.pc & 0xFF ) );
	  cpu8051_WriteI( ++SP, ( cpu8051.pc >> 8 ) );
	  cpu8051_WriteD( _SP_, SP );
	  cpu8051.pc = 0x0B;  
	  cpu8051.active_priority = i;
	  return;          
	}
	//-------------------------- External interrupt 1 ----------------------------     
	//        if ( ( cpu8051_ReadD( _IE_ ) & 0x04 ) && ( ( cpu8051_ReadD( _IP_ ) & 0x04 ) ? i : !i ) && pin1 )
	//-------------------------- Interrupt timer 1 -------------------------------      
	if ( ( cpu8051_ReadD( _IE_ ) & 0x08 ) && ( ( cpu8051_ReadD( _IP_ ) & 0x08 ) ? i : !i ) && ( cpu8051_ReadD( _TCON_ ) & 0x80 ) ) {
	  cpu8051_WriteD( _TCON_, cpu8051_ReadD( _TCON_ ) & 0x7F );
	  SP = cpu8051_ReadD( _SP_ );
	  cpu8051_WriteI( ++SP, ( cpu8051.pc & 0xFF ) );
	  cpu8051_WriteI( ++SP, ( cpu8051.pc >> 8 ) );
	  cpu8051_WriteD( _SP_, SP );
	  cpu8051.pc = 0x1B;
	  cpu8051.active_priority = i;
	  return;
	}
	//-------------------------- Serial Interrupts -------------------------------
	if ( ( cpu8051_ReadD( _IE_ ) & 0x10 ) && ( ( cpu8051_ReadD( _IP_ ) & 0x10 ) ? i : !i ) && ( cpu8051_ReadD( _SCON_ ) & 0x03 ) ) {
	  SP = cpu8051_ReadD( _SP_ );
	  cpu8051_WriteI( ++SP, ( cpu8051.pc & 0xFF ) );
	  cpu8051_WriteI( ++SP, ( cpu8051.pc >> 8 ) );
	  cpu8051_WriteD( _SP_, SP );
	  cpu8051.pc = 0x23;
	  cpu8051.active_priority = i;
	  return;
	}
	//-------------------------- Interrupt timer 2 -------------------------------
	if ( ( cpu8051_ReadD( _IE_ ) & 0x20 ) && ( ( cpu8051_ReadD( _IP_ ) & 0x20 ) ? i : !i ) && ( cpu8051_ReadD( _T2CON_ ) & 0x80 ) ) {          
	  SP = cpu8051_ReadD( _SP_ );
	  cpu8051_WriteI( ++SP, ( cpu8051.pc & 0xFF ) );
	  cpu8051_WriteI( ++SP, ( cpu8051.pc >> 8 ) );
	  cpu8051_WriteD( _SP_, SP );
	  cpu8051.pc = 0x2B;
	  cpu8051.active_priority = i;
	  return;
	}
      }
  }
}


//////////////////////////////////////////////////////////////////////////////
// Execute les timers
//////////////////////////////////////////////////////////////////////////////
void
cpu8051_DoTimers( )
{
  unsigned int tmp;
  unsigned int TR;
  unsigned int MODE;
  unsigned int GATE;
  unsigned int TimerCounter;

  // ----- Timer 0
  TR = cpu8051_ReadD( _TCON_ ) & 0x10;
  MODE = cpu8051_ReadD( _TMOD_ ) & 0x03;
  GATE = cpu8051_ReadD( _TMOD_ ) & 0x08;
  TimerCounter = cpu8051_ReadD( _TMOD_ ) & 0x04;
  
  if ( ( TR && !GATE && !TimerCounter ) || ( MODE == 3 ) )
    switch( MODE ) {
      // Mode 0, compteur de 13 bits.
    case 0 :
      tmp = cpu8051_ReadD( _TH0_ ) * 0x100 + cpu8051_ReadD( _TL0_ );

      tmp++;
      tmp &= 0x1FFF;   // On ne garde que 13 bits.

      if ( tmp == 0 )    // If overflow set TF0
	cpu8051_WriteD( _TCON_, cpu8051_ReadD( _TCON_ ) | 0x20 );
      cpu8051_WriteD( _TH0_, tmp / 0x100 );
      cpu8051_WriteD( _TL0_, tmp & 0xFF  );
      break;
      
      // Mode 1, compteur de 16 bits.
    case 1 :
      tmp = cpu8051_ReadD( _TH0_ ) * 0x100 + cpu8051_ReadD( _TL0_ );
      tmp++;
      tmp &= 0xFFFF;   // On ne garde que 16 bits.
      if ( tmp == 0 )   // If overflow set TF0
	cpu8051_WriteD( _TCON_, cpu8051_ReadD( _TCON_ ) | 0x20 );
      cpu8051_WriteD( _TH0_, ( tmp / 0x100 ) );
      cpu8051_WriteD( _TL0_, ( tmp & 0xFF ) );
      break;
      
      // Mode 2, Compteur de 8 bits avec Auto-Reload
    case 2 :
      tmp = cpu8051_ReadD( _TL0_ );
      tmp++;
      tmp &= 0xFF;
      if ( tmp == 0 ) {    // If overflow -> reload et set TF0
	cpu8051_WriteD( _TCON_, cpu8051_ReadD( _TCON_ ) | 0x20 );
	cpu8051_WriteD( _TL0_, cpu8051_ReadD( _TH0_ ) );
      }
      else
	cpu8051_WriteD( _TL0_, tmp );
      break;
    
    // Mode 3 : TL0 et TH0 sont 2 Timers independants de 8 bits chacuns.
    case 3 :
      if ( TR && !GATE && !TimerCounter ) {
	tmp = cpu8051_ReadD( _TL0_ );
	tmp++;
	tmp &= 0xFF;
	if ( tmp == 0 )  // If TL0 overflow set TF0
	  cpu8051_WriteD( _TCON_, cpu8051_ReadD( _TCON_ ) | 0x20 );
	cpu8051_WriteD( _TL0_, tmp );
      }  // TH0 utilise TR1 et TF1.
      TR = cpu8051_ReadD( _TCON_ ) & 0x40;
      if ( TR ) {
	tmp = cpu8051_ReadD( _TH0_ );
	tmp++;
	tmp &= 0xFF;
	if ( tmp == 0 )  // If TH0 overflow set TF1
	  cpu8051_WriteD( _TCON_, cpu8051_ReadD( _TCON_ ) | 0x80 );  // TF1 = 1.
	cpu8051_WriteD( _TH0_, tmp );
      }
      break;
    };
  

  // ----- Timer 1
  TR = cpu8051_ReadD( _TCON_ ) & 0x40;
  MODE = ( cpu8051_ReadD( _TMOD_ ) & 0x30 ) >> 4 ;
  GATE = cpu8051_ReadD( _TMOD_ ) & 0x80;
  TimerCounter = cpu8051_ReadD( _TMOD_ ) & 0x40;
  
  if ( TR && !GATE && !TimerCounter )
    switch( MODE ) {
      // Mode 0, compteur de 13 bits.
    case 0 :
      tmp = cpu8051_ReadD( _TH1_ ) * 0x100 + cpu8051_ReadD( _TL1_ );
      tmp++;
      tmp &= 0x1FFF;   // On ne garde que 13 bits.
      if ( tmp == 0 )    // If overflow set TF1
	cpu8051_WriteD( _TCON_, cpu8051_ReadD( _TCON_ ) | 0x80 );
      cpu8051_WriteD( _TH1_, tmp / 0x100 );
      cpu8051_WriteD( _TL1_, tmp & 0xFF  );
      break;
      
      // Mode 1, compteur de 16 bits.
    case 1 :
      tmp = cpu8051_ReadD( _TH1_ ) * 0x100 + cpu8051_ReadD( _TL1_ );
      tmp++;
      tmp &= 0xFFFF;   // On ne garde que 16 bits.
      if ( tmp == 0 )   // If overflow set TF1
	cpu8051_WriteD( _TCON_, cpu8051_ReadD( _TCON_ ) | 0x80 );
      cpu8051_WriteD( _TH1_, ( tmp / 0x100 ) );
      cpu8051_WriteD( _TL1_, ( tmp & 0xFF ) );
      break;
      
      // Mode 2, Compteur de 8 bits avec Auto-Reload
    case 2 :
      tmp = cpu8051_ReadD( _TL1_ );
      tmp++;
      tmp &= 0xFF;
      if ( tmp == 0 ) {    // If overflow -> reload et set TF1
	cpu8051_WriteD( _TCON_, cpu8051_ReadD( _TCON_ ) | 0x80 );
	cpu8051_WriteD( _TL1_, cpu8051_ReadD( _TH1_ ) );
      }
      else
	cpu8051_WriteD( _TL1_, tmp );
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
// Return as Text the name of the SFR register at Address if any
//////////////////////////////////////////////////////////////////////////////
int
cpu8051_SFRMemInfo( unsigned int Address, char *Text )
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
// Return as Text the decoded BitAddress
//////////////////////////////////////////////////////////////////////////////
void
cpu8051_IntMemBitInfo( unsigned int BitAddress, char *Text )
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
  
  TextLength = cpu8051_SFRMemInfo( ByteAddress, Text );
  // sprintf( &Text[ TextLength ], ".%X" );
  // Modified by Hugo Villeneuve to remove compilation warning
  sprintf( &Text[ TextLength ], ".%X", BitAddress );
}


//////////////////////////////////////////////////////////////////////////////
// Disasm one instruction at Address into a Text string
//////////////////////////////////////////////////////////////////////////////
int
cpu8051_Disasm( unsigned int Address, char *Text )
{
  int TextLength=0;
  char TextTmp[20];
  unsigned char OpCode;
  int ArgTblOfs;
  int InstSize;
  int i;
  
  OpCode = memory_read8( PGM_MEM_ID, Address );
  InstSize = InstSizesTbl[ OpCode ];
  //printf("%.4X\n", Address);
  
  TextLength += sprintf( Text, " %.4X ", Address );
  
  for (i = 0; i < InstSize; i++ )
    TextLength += sprintf( &Text[TextLength], " %.2X", memory_read8( PGM_MEM_ID, Address + i ) );
  
  Address++;
  
  for (; TextLength < 17; ) TextLength += sprintf( &Text[ TextLength ], " " );
  
  TextLength += sprintf( &Text[ TextLength ], "%s ", InstTextTbl[ InstTypesTbl[ OpCode ] ] );
  ArgTblOfs = OpCode << 2;
  
  for (; TextLength < 25; ) TextLength += sprintf( &Text[ TextLength ], " " );
  
  // MOV direct, direct (OpCode 85h) is peculiar, the operands are inverted
  if ( OpCode == 0x85 ) {
    cpu8051_SFRMemInfo( memory_read8( PGM_MEM_ID, Address + 1 ), TextTmp );
    TextLength += sprintf( &Text[ TextLength ], "%s,", TextTmp );
    cpu8051_SFRMemInfo( memory_read8( PGM_MEM_ID, Address ), TextTmp );
    TextLength += sprintf( &Text[ TextLength ], "%s", TextTmp );
    Address += 2;
    return InstSize;
  }
  
  for ( i = 1; i <= InstArgTbl[ ArgTblOfs ]; i++ ) {
    switch( InstArgTbl[ ArgTblOfs + i ] ) {
    case ADDR11  : {
      TextLength += sprintf( &Text[ TextLength ], "%.4XH", ( ( OpCode << 3) & 0xF00 ) + ( memory_read8( PGM_MEM_ID, Address ) ) );
      Address++;
      break;
    }
    case ADDR16  : {
      TextLength += sprintf( &Text[ TextLength ], "%.4XH", ( ( memory_read8( PGM_MEM_ID, Address ) << 8 ) + memory_read8( PGM_MEM_ID, Address + 1 ) ) );
      Address += 2;
      break;
    }
    case DIRECT  : {
      cpu8051_SFRMemInfo( memory_read8( PGM_MEM_ID, Address ), TextTmp );
      TextLength += sprintf( &Text[ TextLength ], "%s", TextTmp );
      Address++;
      break;
    }
    case BITADDR : {
      cpu8051_IntMemBitInfo( ( memory_read8( PGM_MEM_ID, Address ) & 0xF8 ), TextTmp );
      TextLength += sprintf( &Text[ TextLength ], "%s.%X" , TextTmp, ( memory_read8( PGM_MEM_ID, Address ) & 7 ) );
      Address++;
      break;
    }
    case RELADDR : {
      Address++;
      TextLength += sprintf( &Text[ TextLength ], "%.4XH", ( Address & 0xFF00 ) + ( ( ( Address & 0xFF ) + memory_read8( PGM_MEM_ID, Address - 1 ) ) & 0xFF ) );
      break;
    }
    case DATAIMM : {
      TextLength += sprintf( &Text[ TextLength ], "#%.2XH", memory_read8( PGM_MEM_ID, Address ) );
      Address++;
      break;
    }
    case DATA16  : {
      TextLength += sprintf( &Text[ TextLength ],"#%.4XH", ( ( memory_read8( PGM_MEM_ID, Address ) << 8 ) + memory_read8( PGM_MEM_ID, Address+1 ) ) );
      Address += 2;
      break;
    }
    case CBITADDR : {
      cpu8051_IntMemBitInfo( ( memory_read8( PGM_MEM_ID, Address ) & 0xF8 ), TextTmp );
      TextLength += sprintf( &Text[ TextLength ], "/%s.%X", TextTmp, ( memory_read8( PGM_MEM_ID, Address ) & 7 ) );
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
