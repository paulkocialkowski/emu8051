/*
 * emu8051.h
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
