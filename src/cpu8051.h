/* cpu8051.h */


#ifndef CPU8051_H
#define CPU8051_H 1


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
