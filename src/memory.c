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

#include <stdio.h>
#include <string.h>

#include "common.h"
#include "cpu8051.h"
#include "hexfile.h"
#include "memory.h"

static u_int8_t pgm_mem[PGM_MEM_MAX_SIZE];
static u_int8_t int_mem[INT_MEM_MAX_SIZE];
static u_int8_t ext_mem[EXT_MEM_MAX_SIZE];

void
memory_write8(int memory_id, unsigned long address, u_int8_t value)
{
	switch (memory_id) {
	case PGM_MEM_ID:
		if (address >= PGM_MEM_MAX_SIZE) {
			printf("Address (%lu) is greater than PGM_MEM_MAX_SIZE\n",
			       address);
			return;
		} else
			pgm_mem[address] = value;
		break;
	case INT_MEM_ID:
		if (address >= INT_MEM_MAX_SIZE) {
			printf("Address (%lu) is greater than INT_MEM_MAX_SIZE\n",
			       address);
			return;
		} else
			int_mem[address] = value;
		break;
	case EXT_MEM_ID:
		if (address >= EXT_MEM_MAX_SIZE) {
			printf("Address (%lu) is greater than EXT_MEM_MAX_SIZE\n",
			       address);
			return;
		} else
			ext_mem[address] = value;
		break;
	default:
		/* Error. */
		break;
	}
}

u_int8_t
memory_read8(int memory_id, unsigned long address)
{
	switch (memory_id) {
	case PGM_MEM_ID:
		if (address < PGM_MEM_MAX_SIZE)
			return pgm_mem[address];
		else {
			printf("Address (%lu) is greater than PGM_MEM_MAX_SIZE\n",
				address);
			return 0;
		}
		break;
	case INT_MEM_ID:
		if (address < INT_MEM_MAX_SIZE)
			return int_mem[address];
		else {
			printf("Address (%lu) is greater than INT_MEM_MAX_SIZE\n",
			       address);
			return 0;
		}
		break;
	case EXT_MEM_ID:
		if (address < EXT_MEM_MAX_SIZE)
			return ext_mem[address];
		else {
			printf("Address (%lu) is greater than EXT_MEM_MAX_SIZE\n",
			       address);
			return 0;
		}
		break;
	default:
		/* Error. */
		return 0;
		break;
	}
}

/* Dump memory */
void
DumpMem(char *Address, char *Asize, int memory_id)
{
	unsigned int MemAddress;
	int size;
	int Offset, Column;

	if (strlen(Address) != 0) {
		if (STREQ(Address, "PC"))
			MemAddress = cpu8051.pc;
		else
			MemAddress = Ascii2Hex(Address, strlen(Address));
	} else {
		MemAddress = 0;
	}

	if (strlen(Asize) != 0) {
		size = Ascii2Hex(Asize, strlen(Asize));
	} else {
		size = 256; /* Default size if not specified. */
	}

	for (Offset = 0; Offset < size; Offset += 16) {
		unsigned char data[16];

		printf("%.4X ", MemAddress + Offset);

		for (Column = 0; Column < 16; Column++) {
			data[Column] = memory_read8(memory_id, MemAddress +
						    Offset + Column);
			printf(" %.2X", (int) data[Column]);
		}
		printf("  ");

		/* Display any ASCII characters */
		for (Column = 0; Column < 16; Column++) {
			if ((int) data[Column] >= 32 &&
			    (int) data[Column] <= 126) {
				printf("%c", data[Column]);
			} else
				printf(".");
		}
		printf("\n");
	}
}
