/* memory.h */


#ifndef MEMORY_H
#define MEMORY_H 1


#include <sys/types.h>


enum {
  SFR_MEM_ID,
  PGM_MEM_ID,
  INT_MEM_ID,
  EXT_MEM_ID
};


void
memory_write8( int memory_id, unsigned long address, u_int8_t value );

u_int8_t
memory_read8( int memory_id, unsigned long address );


#endif /* MEMORY_H */
