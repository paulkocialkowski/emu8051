#ifndef _CPU8051_HPP_
#define _CPU8051_HPP_

#include "Memory.hpp"
#include "Reg8051.hpp"

#define BANKPSW ( ReadD( _PSW_ ) & 0x18 )

//////////////////////////////////////////////////////////////////////////////
// CPU8051
// Implements the 8051 CPU Object
//////////////////////////////////////////////////////////////////////////////
class CPU8051 {
public:
  CPU8051( );
  ~CPU8051( );

  void Exec( );
  void Reset( );
  unsigned int GetPC( );
  void SetPC( unsigned int NewPC );
  void WriteD( unsigned int Address, unsigned char Value );
  void WriteExt( unsigned int Address, unsigned char Value );
  void WriteInt( unsigned int Address, unsigned char Value );
  void WriteI( unsigned int Address, unsigned char Value );
  void WritePGM( unsigned int Address, unsigned char Value );
  unsigned char ReadD( unsigned int Address );
  unsigned char ReadInt( unsigned int Address );
  unsigned char ReadExt( unsigned int Address );
  unsigned char ReadI( unsigned int Address );
  unsigned char ReadPGM( unsigned int Address );
  unsigned int GetNextAddress( );

  void WriteB( unsigned int BitAddress, unsigned char Value );
  unsigned char ReadB( unsigned int BitAddress );

  void CheckInterrupts( );
  void DoTimers( );

  int SFRMemInfo( unsigned int Address, char *Text );
  void IntMemBitInfo( unsigned int BitAddress, char *Text );
  int Disasm( unsigned int Address, char *Text );

private:
  Memory *SFRMem;
  Memory *PGMMem;
  Memory *IntMem;
  Memory *ExtMem;
  unsigned int PC;
  unsigned long CLOCK;
  int ActivePriority;
  int (CPU8051::*funcptr[256])();

  #include "Inst_Def.hpp"

};




#endif

