/*
 * EmuConsole.hpp
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
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef _EMUCONSOLE_HPP_
#define _EMUCONSOLE_HPP_

#include "CPU8051.hpp"
#include <string>
#include "exceptions.hpp"

using namespace std;


//////////////////////////////////////////////////////////////////////////////
// EmuConsole
// Implements the Console User Interface as an Object
//////////////////////////////////////////////////////////////////////////////
class EmuConsole {
public:

  EmuConsole( int argc, char **argv, CPU8051 *mCPU );
  ~EmuConsole( );

  CPU8051 *CPU;

  void Main( );

  void Reset( );
  void Trace( string Address );
  void Exec( string Address, string NumberInst );
  void ShowBreakpoints( );
  void SetBreakpoint( unsigned int Address );
  void ClearBreakpoint( unsigned int Address );
  int IsBreakpoint( unsigned int Address );
  void Disasm( string Address, string NumberInst );
  void DisasmN( unsigned int Address, int NumberInst );
  void DumpPGM( string Address );
  void DumpD( string Address );
  void DumpInt( string Address );
  void DumpExt( string Address );
  void DumpI( string Address );
  void ShowRegisters( );
  void SetRegister( string Register, string NewValue );
  


private:
  int NbBreakpoints;
  unsigned int Breakpoints[ MAXBP ];

  void Capitalize( string *InputString );
  void RemoveSpaces( string *InputString );

};



#endif



