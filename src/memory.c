/*
 * memory.c
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

#include "memory.h"


#define SFR_MEM_SIZE 128
#define PGM_MEM_SIZE 65536
#define INT_MEM_SIZE 128
#define EXT_MEM_SIZE 65536


static u_int8_t sfr_mem[SFR_MEM_SIZE];
static u_int8_t pgm_mem[PGM_MEM_SIZE];
static u_int8_t int_mem[INT_MEM_SIZE];
static u_int8_t ext_mem[EXT_MEM_SIZE];


void
memory_write8( int memory_id, unsigned long address, u_int8_t value )
{
  switch( memory_id ) {
  case SFR_MEM_ID:
    if( address >= SFR_MEM_SIZE ) {
      return;
    }
    else {
      sfr_mem[address] = value;
    }
    break;
  case PGM_MEM_ID:
    if( address >= PGM_MEM_SIZE ) {
      return;
    }
    else {
      pgm_mem[address] = value;
    }
    break;
  case INT_MEM_ID:
    if( address >= INT_MEM_SIZE ) {
      return;
    }
    else {
      int_mem[address] = value;
    }
    break;
  case EXT_MEM_ID:
    if( address >= EXT_MEM_SIZE ) {
      return;
    }
    else {
      ext_mem[address] = value;
    }
    break; 
  default:
    /* Error. */
    break;
  }
}


u_int8_t
memory_read8( int memory_id, unsigned long address )
{
  switch( memory_id ) {
  case SFR_MEM_ID:
    if( address < SFR_MEM_SIZE ) {
      return sfr_mem[address];
    }
    else {
      return 0;
    }
    break;
  case PGM_MEM_ID:
    if( address < PGM_MEM_SIZE ) {
      return pgm_mem[address];
    }
    else {
      return 0;
    }
    break;
  case INT_MEM_ID:
    if( address < INT_MEM_SIZE ) {
      return int_mem[address];
    }
    else {
      return 0;
    }
    break;
  case EXT_MEM_ID:
    if( address < EXT_MEM_SIZE ) {
      return ext_mem[address];
    }
    else {
      return 0;
    }
    break; 
  default:
    /* Error. */
    return 0;
    break;
  }
}
