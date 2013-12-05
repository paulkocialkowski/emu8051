/*
 * cpu8051.h
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

#include <stdint.h>

/* Maximum number of BreakPoints */
#define MAXBP 32

#define INTERRUPT_0 (0)
#define INTERRUPT_1 (1)
#define INTERRUPT_2 (2)
#define INTERRUPT_3 (3)
#define INTERRUPT_4 (4)
#define INTERRUPT_5 (5)
#define INTERRUPT_MASK(n) (1 << n)

#define INTERRUPT_PRIORITY_HIGH     (1)
#define INTERRUPT_PRIORITY_LOW      (0)

struct cpu8051_t {
	unsigned int pc; /* Program counter */
	unsigned long clock;
	int active_priority;
	int bp_count;
	unsigned int bp[MAXBP]; /* List of breakpoints */
};

/* Exported variables */
#undef _SCOPE_
#ifdef CPU8051_M
#  define _SCOPE_ /**/
#else
#  define _SCOPE_ extern
#endif

_SCOPE_ struct cpu8051_t cpu8051;

int
IsBreakpoint(unsigned int Address);

int
IsStoppoint(unsigned int address);

void
ShowBreakpoints(void);

void
SetBreakpoint(unsigned int Address);

void
ClearBreakpoint(unsigned int Address);

void
ToggleBreakpoint(unsigned int Address);

void
cpu8051_init(void);

void
cpu8051_Exec(void);

void
cpu8051_Reset(void);

void
cpu8051_WriteD(unsigned int Address, unsigned char Value);

void
cpu8051_WriteI(unsigned int Address, unsigned char Value);

void
cpu8051_WriteB(uint8_t bit_address, uint8_t value);

unsigned char
cpu8051_ReadD(unsigned int Address);

unsigned char
cpu8051_ReadI(unsigned int Address);

unsigned char
cpu8051_ReadB(uint8_t bit_address);

int
cpu8051_get_instruction_size(unsigned char opcode);

void
cpu8051_disasm_mnemonic(unsigned char OpCode, char *buf);

void
cpu8051_disasm_args(unsigned int address, char *buf);

int
cpu8051_Disasm(unsigned int Address, char *Text);

#endif /* CPU8051_H */
