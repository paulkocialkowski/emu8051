/* cpu8051.h */


#ifndef CPU8051_H
#define CPU8051_H 1


typedef struct cpu8051_t
{
  unsigned int pc;
  unsigned long clock;
  int active_priority;
} cpu8051_t;


/* Exported variables */
#undef _SCOPE_
#ifdef CPU8051_M
#  define _SCOPE_ /**/
#else
#  define _SCOPE_ extern
#endif

_SCOPE_ cpu8051_t cpu8051;


void
cpu8051_init( void );

void
cpu8051_Exec( void );

void
cpu8051_Reset( void );

void
cpu8051_WriteD( unsigned int Address, unsigned char Value );

void
cpu8051_WriteInt( unsigned int Address, unsigned char Value );

void
cpu8051_WriteI( unsigned int Address, unsigned char Value );

unsigned char
cpu8051_ReadD( unsigned int Address );

unsigned char
cpu8051_ReadInt( unsigned int Address );

unsigned char
cpu8051_ReadI( unsigned int Address );

unsigned int
cpu8051_GetNextAddress( void );

void
cpu8051_WriteB( unsigned int BitAddress, unsigned char Value );

unsigned char
cpu8051_ReadB( unsigned int BitAddress );

void
cpu8051_CheckInterrupts( void );

void
cpu8051_DoTimers( void );

int
cpu8051_SFRMemInfo( unsigned int Address, char *Text );

void
cpu8051_IntMemBitInfo( unsigned int BitAddress, char *Text );

int
cpu8051_Disasm( unsigned int Address, char *Text );


#endif /* CPU8051_H */
