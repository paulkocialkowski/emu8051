#ifndef _MEMORY_HPP_
#define _MEMORY_HPP_


//////////////////////////////////////////////////////////////////////////////
// Memory
// Implements a Memory array to be used by the CPU8051 as an Object
//////////////////////////////////////////////////////////////////////////////
class Memory {
public:
  Memory( unsigned long MemSize );
  ~Memory( );

  unsigned long GetSize( );

  void Write8( unsigned long Address, unsigned char Value );
  unsigned char Read8( unsigned long Address );

  unsigned char *memarray;
  unsigned long memsize;
};


#endif
