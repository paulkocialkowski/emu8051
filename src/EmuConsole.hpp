#ifndef _EMUCONSOLE_HPP_
#define _EMUCONSOLE_HPP_

#include "CPU8051.hpp"
#include <string>
#include "exceptions.hpp"

using namespace std;

// Maximum number of BreakPoints
#define MAXBP 32

#define ENDLINE "\n"



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



