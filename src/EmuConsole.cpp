/*
 * EmuConsole.cpp
 *
 * Copyright (C) 1999 Jonathan St-André
 * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <iostream>
#include <fstream>
#include "config.h"
#include "EmuConsole.hpp"
#include "CPU8051.hpp"
#include "Reg8051.hpp"
#include "Keyboard.hpp"

extern "C" {
#include "options.h"
#include "file.h"
}


EmuConsole *EmuConsolePtr;


static void
cpu_write_pgm( unsigned int Address, unsigned char Value )
{
  EmuConsolePtr->CPU->WritePGM( Address, Value );
}


int main( int argc, char **argv )
{
  CPU8051 *maincpu = new CPU8051;
  
  EmuConsole *emuUI = new EmuConsole( argc, argv, maincpu );
  
  emuUI->Main();
  printf( "End of program.\n" );
  
  delete emuUI;
  delete maincpu;
  
  return 0;
}


unsigned int
Ascii2Hex_TEMP( string istring, unsigned int length )
{
  if ( !length || ( length > istring.size() ) )
    length = istring.size();
  
  if ( istring.empty() )
    throw MissingParameter();
  
  unsigned int result = 0;
  unsigned int i, ascii_code;
  for ( i = 0; i < length; i++ ) {
    ascii_code = istring[ i ];
    if ( ascii_code > 0x39 )
      ascii_code &= 0xDF;
    if ( ( ascii_code >= 0x30 && ascii_code <= 0x39 ) || ( ascii_code >= 0x41 && ascii_code <= 0x46 ) ) {
      ascii_code -= 0x30;
      if ( ascii_code > 9 )
	ascii_code -= 7;
      result <<= 4;
      result += ascii_code;
    }
    else {
      throw SyntaxError();
    }
  }
  return result;
}



//////////////////////////////////////////////////////////////////////////////
// EmuConsole::EmuConsole( int argc, char **argv, CPU8051 *mCPU )
// EmuConsole constructor
//////////////////////////////////////////////////////////////////////////////
EmuConsole::EmuConsole( int argc, char **argv, CPU8051 *mCPU )
{
  CPU = mCPU;
  CPU->Reset( );
  NbBreakpoints = 0;

  EmuConsolePtr = this;

  ParseCommandLineOptions( argc, argv );

  if( GetHexFileName() != NULL ) {
    LoadHexFile( GetHexFileName(), cpu_write_pgm );
  }
}

//////////////////////////////////////////////////////////////////////////////
// EmuConsole::~EmuConsole( )
// EmuConsole destructor
//////////////////////////////////////////////////////////////////////////////
EmuConsole::~EmuConsole( )
{
}

//////////////////////////////////////////////////////////////////////////////
// void EmuConsole::Main( )
// EmuConsole main loop
//////////////////////////////////////////////////////////////////////////////
void EmuConsole::Main( )
{
  /*int ASCII_Code;*/
  unsigned int Index;
  
  string InputString;
  string Command;
  string Parameter1;
  string Parameter2;
  char prompt[] = "-> ";

  char *Title[] = { "      *******************",
		    "      *  8051 Emulator  *",
		    "      *******************",
		    "", 0 };
  
  char *Menu[] = {
      "      Available commands, [ ] = options",
      "",
      "  Set Breakpoint.............. SB [address]",
      "  Remove Breakpoint........... RB [address]",
      "  Display Breakpoint(s)....... DB",
      "  Dump External Data Memory... DE [address]",
      "  Dump Internal Data Memory... DI [address]",
      "  Dump Program Memory......... DP [address]",
      "  Display Registers content... DR",
      "  Execute..................... EM [address [number of instructions]]",
      "  Help........................ H",
      "  Modify External Data Memory. ME address value",
      "  Modify Internal Data Memory. MI address value",
      "  Modify Program Memory....... MP address value",
      "  Modify Register............. MR register value",
      "  Quit Emulator............... Q",
      "  Trace mode.................. T [address]",
      "  Unassemble.................. U [address [numberof instructions]]",
      "  Reset processor............. Z", 0 };

  
  Index = 0;
  while ( Title[ Index ] != 0 ) printf( "%s%s", Title[ Index++ ], ENDLINE );
  Index = 0;
  while ( Menu[ Index ] != 0 ) printf( "%s%s", Menu[ Index++ ], ENDLINE );
  
  Reset( );
  int QuitRequest = 0;
  
  while( !QuitRequest ) {
    try {
      
      printf( prompt );
      getline ( cin, InputString, '\n' );
      Capitalize( &InputString );          
      RemoveSpaces( &InputString );

      for ( Index = 0; Index < InputString.size( ); Index++ ) {
	if ( InputString[ Index ] < 'A' || InputString[ Index ] > 'z' )
	  break;
      }

      Command = InputString;
      // Keep only the Command part from the input line
      Command.replace( Index, Command.size( ), 0, ( char )0 );
      // Keep only the arguments
      InputString.replace( 0, Index, 0, ( char )0 );
      
      RemoveSpaces ( &InputString );
      Index = 0;
      while ( ( Index < InputString.size( ) ) && ( InputString [ Index ] != ' ' ) ) Index++;
      Parameter1 = InputString;
      Parameter1.replace( Index, Parameter1.size( ), 0, ( char )0 );
      InputString.replace( 0, Index, 0, ( char )0 );
      
      RemoveSpaces ( &InputString );
      Index = 0;
      while ( ( Index < InputString.size( ) ) && ( InputString [ Index ] != ' ' ) ) Index++;
      Parameter2 = InputString;
      Parameter2.replace( Index, Parameter2.size( ), 0, ( char )0 );
      InputString.replace( 0, Index, 0, ( char )0 );

      RemoveSpaces ( &InputString );
      if ( !InputString.empty( ) )
	throw SyntaxError( );
      
      if ( Command.empty( ) && !Parameter1.empty( ) )
	throw SyntaxError( );
      
      if ( ( Parameter1.size( ) > 4 ) || ( Parameter2.size( ) > 4 ) )
	throw InvalidParameter( );
      
      if ( !Command.empty( ) ) {
	switch ( Command [ 0 ] ) {

	case 'D' :
	  if ( Parameter2.empty( ) ) {
	    if ( Command == "DB" && Parameter1.empty( ) )
              ShowBreakpoints( );	 
	    else if ( Command == "DE" )
	      DumpExt( Parameter1 );
	    else if ( Command == "DI" )
	      DumpInt( Parameter1 );
	    else if ( Command == "DP" ) {
	      if ( Parameter1.empty( ) )
		Parameter1 = "PC";
	      DumpPGM( Parameter1 );	
	    }
	    else if ( Command == "DR" && Parameter1.empty( ) )
	      ShowRegisters( );
	    else
	      throw SyntaxError( );
	  }
	  else
	    throw SyntaxError( );
	  break;
	  
	case 'E' :
	  if ( Command == "EM" )
	    Exec( Parameter1, Parameter2 );
	  else
	    throw SyntaxError( );
	  break;
	case 'H' :
	  if ( Command == "H" && Parameter1.empty( ) && Parameter2.empty( ) )
	    {
	      Index = 0;
	      while ( Menu[ Index ] != 0 ) printf( "%s%s", Menu[ Index++ ], ENDLINE );
	    }
	  else
	    throw SyntaxError( );
	  break;
	case 'M' :
	  if ( Parameter1.empty() || Parameter2.empty() )
	    throw MissingParameter();
	  else if ( Command == "ME" ) {
	    unsigned int adresse = Ascii2Hex_TEMP( Parameter1, 4 );
	    unsigned char valeur = Ascii2Hex_TEMP( Parameter2, 2 );
	    CPU->WriteExt( adresse, valeur );
	  }
	  else if ( Command == "MI" ) {
	    unsigned char adresse = Ascii2Hex_TEMP( Parameter1, 2 );
	    unsigned char valeur = Ascii2Hex_TEMP( Parameter2, 2 );
	    CPU->WriteInt( adresse, valeur );
	  }
	  else if ( Command == "MP" ) {
	    unsigned int adresse = Ascii2Hex_TEMP( Parameter1, 4 );
	    unsigned char valeur = Ascii2Hex_TEMP( Parameter2, 2 );
	    CPU->WritePGM( adresse, valeur );
	  }
	  else if ( Command == "MR" )
	    SetRegister( Parameter1, Parameter2 );  
	  else
	    throw SyntaxError();
	  break;
	case 'Q' :
	  if ( Command == "Q" && Parameter1.empty( ) && Parameter2.empty( ) )
	    QuitRequest = 1;
	  else
	    throw SyntaxError( );
	  break;      
	case 'R' :
	  if ( !Parameter2.empty( ) )
	    throw TooMuchParameters( );
	  if ( Command == "RB" ) {
	    if ( Parameter1.empty( ) )
	      ClearBreakpoint( CPU->GetPC( ) );
	    else
	      ClearBreakpoint( Ascii2Hex_TEMP( Parameter1, 4 ) );
	  }
	  else
	    throw SyntaxError( );
	  break;
	case 'S' :
	  if ( !Parameter2.empty( ) )
	    throw TooMuchParameters( );
	  if ( Command == "SB" ) {
	    if ( Parameter1.empty( ) )
	      SetBreakpoint( CPU->GetPC( ) );
	    else
	      SetBreakpoint( Ascii2Hex_TEMP( Parameter1, 4 ) );
	  }
	  else
	    throw SyntaxError( );
	  break;
	case 'T' :
	  if ( !Parameter2.empty( ) )
	    throw TooMuchParameters( );
	  if ( Command == "T" )
	    Trace( Parameter1 );
	  else
	    throw SyntaxError( );
	  break;
	case 'U' :
	  if ( Command == "U" )
	    Disasm( Parameter1, Parameter2 );
	  else
	    throw SyntaxError( );
	  break;
	case 'Z' :
	  if ( Command == "Z" && Parameter1.empty( ) && Parameter2.empty( ) )
	    Reset( );
	  else
	    throw SyntaxError( );
	  break;
	case '\n' :
	  break;
	default :
	  throw SyntaxError( );
	}
      }
    }
    catch ( SyntaxError ) {
      printf( "Syntax Error!%s", ENDLINE );
    }
    catch ( MissingParameter ) {
      printf( "Missing Parameter!%s", ENDLINE );
    }
    catch ( InvalidParameter ) {
      printf( "Invalid Parameter Format!%s", ENDLINE );
    }
    catch ( TooMuchParameters ) {
      printf( "Wrong Number of Parameters!%s", ENDLINE );
    }
    catch ( ResetRequest ) {
      printf( "Resetting Microcontroler...%s", ENDLINE );
    }
    catch ( InvalidRegister ) {
      printf( "%sInvalid register name!%s", ENDLINE, ENDLINE );
      printf( "Valid registers are A, B, PC and SP.%s", ENDLINE );
      }
    }
}

//////////////////////////////////////////////////////////////////////////////
// void EmuConsole::Reset( )
// CPU reset and Console UI update
//////////////////////////////////////////////////////////////////////////////
void EmuConsole::Reset( )
{
  printf( "Resetting... " );
  CPU->Reset( );
  printf( "Done.%s", ENDLINE );
  ShowRegisters( );
}

//////////////////////////////////////////////////////////////////////////////
// void EmuConsole::Trace( string Address )
// CPU trace and Console UI update
//////////////////////////////////////////////////////////////////////////////
void EmuConsole::Trace( string Address )
{
  if ( !Address.empty( ) ) CPU->SetPC( Ascii2Hex_TEMP( Address, Address.size( ) ) );
  CPU->Exec( );
  ShowRegisters( );
  DisasmN( CPU->GetPC( ), 1 );
}

//////////////////////////////////////////////////////////////////////////////
// void EmuConsole::Exec( string Address, string NumberInst )
// CPU exec and Console UI update
//////////////////////////////////////////////////////////////////////////////
void EmuConsole::Exec( string Address, string NumberInst )
{
  char dummy;
  int NbInst = -1;     // -1 is infinity
  if ( !Address.empty( ) ) {
    Capitalize( &Address );
    if ( Address != "PC" ) CPU->SetPC( Ascii2Hex_TEMP( Address, Address.size( ) ) );
  }

  if ( !NumberInst.empty( ) ) NbInst = Ascii2Hex_TEMP( NumberInst, NumberInst.size( ) );

  InitUnixKB( );

  printf( "Program executing...%s", ENDLINE );

  do {
    CPU->Exec( );
    if ( NbInst > 0 ) NbInst--;
  } while ( !IsBreakpoint( CPU->GetPC( ) ) && ( NbInst != 0 ) && !kbhit( ) );
  if ( kbhit( ) ) {
    dummy = getch( );   // flush key
    printf( "Caught break signal!%s", ENDLINE );
  }
  if ( NbInst == 0 ) printf( "Number of instructions reached! Stopping!%s", ENDLINE );
  if ( IsBreakpoint( CPU->GetPC( ) ) ) printf( "Breakpoint hit at %.4X! Stopping!%s", CPU->GetPC( ), ENDLINE );

  ResetUnixKB( );
}

//////////////////////////////////////////////////////////////////////////////
// void EmuConsole::ShowBreakpoints( )
// Show Breakpoints list
//////////////////////////////////////////////////////////////////////////////
void EmuConsole::ShowBreakpoints( )
{
  for ( int Index = 0; Index < NbBreakpoints ; Index++ )
    printf( "Breakpoint at Address = %.4X%s", Breakpoints[ Index ], ENDLINE );
}

//////////////////////////////////////////////////////////////////////////////
// void EmuConsole::ClearBreakpoint( unsigned int Address )
// Clear Breakpoint at Address from list
//////////////////////////////////////////////////////////////////////////////
void EmuConsole::ClearBreakpoint( unsigned int Address )
{
  int Index = 0;
  while ( Index < NbBreakpoints && Breakpoints[ Index ] != Address ) Index++;
  if ( Breakpoints[ Index ] != Address ) return;
  Breakpoints[ Index ] = Breakpoints[ NbBreakpoints - 1 ];
  NbBreakpoints--;
}

//////////////////////////////////////////////////////////////////////////////
// void EmuConsole::SetBreakpoint( unsigned int Address )
// Set Breakpoint at Address from list
//////////////////////////////////////////////////////////////////////////////
void EmuConsole::SetBreakpoint( unsigned int Address )
{
  if ( IsBreakpoint( Address ) ) return;
  if ( NbBreakpoints < MAXBP ) Breakpoints[ NbBreakpoints++ ] = Address;
}

//////////////////////////////////////////////////////////////////////////////
// int EmuConsole::IsBreakpoint( unsigned int Address )
// Is the a breakpoint at Address
//////////////////////////////////////////////////////////////////////////////
int EmuConsole::IsBreakpoint( unsigned int Address )
{
  int Index = 0;
  while ( Index < NbBreakpoints && Breakpoints[ Index ] != Address ) Index++;
  return ( Breakpoints[ Index ] == Address && Index < NbBreakpoints );
}


//////////////////////////////////////////////////////////////////////////////
// void EmuConsole::Disasm( string Address, string NumberInst )
// Disassemble 16 instructions at Address
//////////////////////////////////////////////////////////////////////////////
void EmuConsole::Disasm( string Address, string NumberInst )
{
  unsigned int MemAddress, NbInst;
  Capitalize( &Address );
  if ( Address.empty( ) || ( Address == "PC" ) ) MemAddress = CPU->GetPC( );
  else MemAddress = Ascii2Hex_TEMP( Address, Address.size( ) );
  if ( NumberInst.empty( ) ) NumberInst = "10";
  NbInst = Ascii2Hex_TEMP( NumberInst, NumberInst.size( ) );
  DisasmN( MemAddress, NbInst );
}

//////////////////////////////////////////////////////////////////////////////
// void EmuConsole::DisasmN( unsigned int Address, int NumberInst )
// Disassemble NumberInst instructions at Address
//////////////////////////////////////////////////////////////////////////////
void EmuConsole::DisasmN( unsigned int Address, int NumberInst )
{
char TextTmp[255];
int Row;
 for ( Row = 0; Row < NumberInst ; Row++ ) {
   Address += CPU->Disasm( Address, TextTmp );
   printf( "%s%s", TextTmp, ENDLINE );
 }
}

//////////////////////////////////////////////////////////////////////////////
// void EmuConsole::DumpPGM( string Address )
// Dump Program memory
//////////////////////////////////////////////////////////////////////////////
void EmuConsole::DumpPGM( string Address )
{
  unsigned int MemAddress = 0;
  int Offset, Column;
  unsigned char Byte;
  if ( !Address.empty( ) ) {
    Capitalize( &Address );
    if ( Address == "PC" )
      MemAddress = CPU->GetPC( );
    else
      MemAddress = Ascii2Hex_TEMP( Address, Address.size( ) );
  }
  for ( Offset = 0; Offset < 256; Offset += 16 ) {
    printf( "%.4X ", MemAddress + Offset );
    for ( Column = 0; Column < 16; Column++ ) 
      printf( " %.2X", ( int )CPU->ReadPGM( MemAddress + Offset + Column ) );
    printf( "  " );
    for ( Column = 0; Column < 16; Column++ ) {
      Byte = CPU->ReadPGM( MemAddress + Offset + Column );
      if ( ( int )Byte >= 32 && ( int )Byte <= 126 )
	printf( "%c", Byte );
      else printf( "." );
    }
    printf( "%s", ENDLINE );
  }

}




//////////////////////////////////////////////////////////////////////////////
// void EmuConsole::DumpI( string Address )
// Dump using Indirect read access
//////////////////////////////////////////////////////////////////////////////
void EmuConsole::DumpI( string Address )
{
  unsigned int MemAddress = 0;
  int Offset, Column;
  unsigned char Byte;
  if ( !Address.empty( ) ) MemAddress = Ascii2Hex_TEMP( Address, Address.size( ) );
  for ( Offset = 0; Offset < 256; Offset += 16 ) {
    printf( "%.4X ", MemAddress + Offset );
    for ( Column = 0; Column < 16; Column++ ) 
      printf( " %.2X", ( int )CPU->ReadI( MemAddress + Offset + Column ) );
    printf( "  " );
    for ( Column = 0; Column < 16; Column++ ) {
      Byte = CPU->ReadI( MemAddress + Offset + Column );
      if ( ( int )Byte >= 32 && ( int )Byte <= 126 )
	printf( "%c", Byte );
      else printf( "." );
    }
    printf( "%s", ENDLINE );
  }
}

//////////////////////////////////////////////////////////////////////////////
// void EmuConsole::DumpInt( string Address )
// Dump internal Data memory
//////////////////////////////////////////////////////////////////////////////
void EmuConsole::DumpInt( string Address )
{
  unsigned int MemAddress = 0;
  int Offset, Column;
  unsigned char Byte;
  if ( !Address.empty( ) )
    MemAddress = Ascii2Hex_TEMP( Address, 4 );
  for ( Offset = 0; Offset < 256; Offset += 16 ) {
    printf( "%.4X ", MemAddress + Offset );
    for ( Column = 0; Column < 16; Column++ ) 
      printf( " %.2X", ( int )CPU->ReadInt( MemAddress + Offset + Column ) );
    printf( "  " );
    for ( Column = 0; Column < 16; Column++ ) {
      Byte = CPU->ReadInt( MemAddress + Offset + Column );
      if ( ( int )Byte >= 32 && ( int )Byte <= 126 )
	printf( "%c", Byte );
      else printf( "." );
    }
    printf( "%s", ENDLINE );
  }
}


//////////////////////////////////////////////////////////////////////////////
// void EmuConsole::DumpExt( string Address )
// Dump de la memoire externe ( $00 a $FFFF)
//////////////////////////////////////////////////////////////////////////////
void EmuConsole::DumpExt( string Address )
{
  unsigned int MemAddress = 0;
  int Offset, Column;
  unsigned char Byte;
  if ( !Address.empty( ) )
    MemAddress = Ascii2Hex_TEMP( Address, 4 );
  for ( Offset = 0; Offset < 256; Offset += 16 ) {
    printf( "%.4X ", MemAddress + Offset );
    for ( Column = 0; Column < 16; Column++ ) 
      printf( " %.2X", ( int )CPU->ReadExt( MemAddress + Offset + Column ) );
    printf( "  " );
    for ( Column = 0; Column < 16; Column++ ) {
      Byte = CPU->ReadExt( MemAddress + Offset + Column );
      if ( ( int )Byte >= 32 && ( int )Byte <= 126 )
	printf( "%c", Byte );
      else printf( "." );
    }
    printf( "%s", ENDLINE );
  }
}

//////////////////////////////////////////////////////////////////////////////
// void EmuConsole::DumpD( string Address )
// Dump using Direct read access
//////////////////////////////////////////////////////////////////////////////
void EmuConsole::DumpD( string Address )
{
  unsigned int MemAddress = 0;
  int Offset, Column;
  unsigned char Byte;
  if ( !Address.empty( ) ) MemAddress = Ascii2Hex_TEMP( Address, Address.size( ) );
  for ( Offset = 0; Offset < 256; Offset += 16 ) {
    printf( "%.4X ", MemAddress + Offset );
    for ( Column = 0; Column < 16; Column++ ) 
      printf( " %.2X", ( int )CPU->ReadD( MemAddress + Offset + Column ) );
    printf( "  " );
    for ( Column = 0; Column < 16; Column++ ) {
      Byte = CPU->ReadD( MemAddress + Offset + Column );
      if ( ( int )Byte >= 32 && ( int )Byte <= 126 )
	printf( "%c", Byte );
      else printf( "." );
    }
    printf( "%s", ENDLINE );
  }
}

//////////////////////////////////////////////////////////////////////////////
// void EmuConsole::SetRegister( string Register, string NewValue )
// Set NewValue to Register
//////////////////////////////////////////////////////////////////////////////
void EmuConsole::SetRegister( string Register, string NewValue )
{
  Capitalize( &Register );
  if ( Register == "PC" ) CPU->SetPC( Ascii2Hex_TEMP( NewValue, 4 ) );
  else if ( Register == "A" ) CPU->WriteD( _ACC_, Ascii2Hex_TEMP( NewValue, 2 ) );
  else if ( Register == "B" ) CPU->WriteD( _B_, Ascii2Hex_TEMP( NewValue, 2 ) );
  else if ( Register == "SP" ) CPU->WriteD( _SP_, Ascii2Hex_TEMP( NewValue, 2 ) );
  else throw InvalidRegister( );
}

//////////////////////////////////////////////////////////////////////////////
// void EmuConsole::Capitalize( string *InputString )
// Capitalize all letters in InputString
//////////////////////////////////////////////////////////////////////////////
void EmuConsole::Capitalize( string *InputString )
{
  for (unsigned int Index = 0; Index < InputString->size( ); Index++ ) {
    {
      if ( ( ( *InputString )[ Index ] >= 'a' ) && ( ( *InputString )[ Index ] <= 'z' ) )
	( *InputString )[ Index ] -= ( ( int )'a'- ( int )'A' );
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
// void EmuConsole::RemoveSpaces( string *InputString )
// Remove spaces from InputString
//////////////////////////////////////////////////////////////////////////////
void EmuConsole::RemoveSpaces( string *InputString )
{
  unsigned int Index = 0;

  while ( ( Index < ( *InputString ).size( ) ) && ( *InputString )[ Index ] == ' ' ) {
    Index++;
  }
  ( *InputString ).replace( 0, Index, 0, ( char )0 );
}

//////////////////////////////////////////////////////////////////////////////
// void EmuConsole::ShowRegisters( )
// Show CPU registers
//////////////////////////////////////////////////////////////////////////////
void EmuConsole::ShowRegisters( )
{
  unsigned char PSW = CPU->ReadD( _PSW_ );
  int BankSelect = ( PSW & 0x18 );

  printf( "----------------------------------------------------------------------%s", ENDLINE );
  printf( "|  PC  | SP | DPTR | ACC |  B | PSW:  CY  AC  F0 RS1 RS0  OV   -   P |%s", ENDLINE );
  printf( "| %.4X | %.2X | %.4X |  %.2X | %.2X |", CPU->GetPC( ), CPU->ReadD( _SP_ ), ( CPU->ReadD( _DPTRHIGH_ ) << 8 ) + CPU->ReadD( _DPTRLOW_ ), CPU->ReadD( _ACC_ ), CPU->ReadD( _B_ ) );
  printf( "        %d   %d   %d   %d   %d   %d   %d   %d |",  ( PSW >> 7 ) & 1, ( PSW >> 6 ) & 1, ( PSW >> 5 ) & 1, ( PSW >> 4 ) & 1, ( PSW >> 3 ) & 1, ( PSW >> 2 ) & 1, ( PSW >> 1 ) & 1, PSW & 1 );
  printf( "%s", ENDLINE );
  printf( "----------------------------------------------------------------------%s", ENDLINE );
  
  printf( "| TCON | TMOD | IE | IP | R0 | R1 | R2 | R3 | R4 | R5 | R6 | R7 |    |%s", ENDLINE );
  printf( "|   %.2X |   %.2X | %.2X | %.2X ", CPU->ReadD( _TCON_ ), CPU->ReadD( _TMOD_ ), CPU->ReadD( _IE_ ), CPU->ReadD( _IP_ ) );
  printf( "| %.2X | %.2X | %.2X | %.2X ", CPU->ReadD( BankSelect + _R0_ ), CPU->ReadD( BankSelect + _R1_ ), CPU->ReadD( BankSelect + _R2_ ), CPU->ReadD( BankSelect + _R3_ ) );
  printf( "| %.2X | %.2X | %.2X | %.2X ", CPU->ReadD( BankSelect + _R4_ ), CPU->ReadD( BankSelect + _R5_ ), CPU->ReadD( BankSelect + _R6_ ), CPU->ReadD( BankSelect + _R7_ ) );
  printf( "|    |%s", ENDLINE );

  printf( "----------------------------------------------------------------------%s", ENDLINE );

}
