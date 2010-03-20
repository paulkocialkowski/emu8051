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

#define PGM_MEM_SIZE 65536
#define INT_MEM_SIZE 256 /* IRAM = Direct addresses $00 to $7F
			  * SFR  = Direct addresses $80 to $FF */
#define EXT_MEM_SIZE 65536

static u_int8_t pgm_mem[PGM_MEM_SIZE];
static u_int8_t int_mem[INT_MEM_SIZE];
static u_int8_t ext_mem[EXT_MEM_SIZE];

void
memory_write8(int memory_id, unsigned long address, u_int8_t value)
{
	switch (memory_id) {
	case PGM_MEM_ID:
		if (address >= PGM_MEM_SIZE) {
			printf("Address is greater than PGM_MEM_SIZE\n");
			return;
		} else
			pgm_mem[address] = value;
		break;
	case INT_MEM_ID:
		if (address >= INT_MEM_SIZE) {
			printf("Address is greater than INT_MEM_SIZE\n");
			return;
		} else
			int_mem[address] = value;
		break;
	case EXT_MEM_ID:
		if (address >= EXT_MEM_SIZE) {
			printf("Address is greater than EXT_MEM_SIZE\n");
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
		if (address < PGM_MEM_SIZE)
			return pgm_mem[address];
		else {
			printf("Address is greater than PGM_MEM_SIZE\n");
			return 0;
		}
		break;
	case INT_MEM_ID:
		if (address < INT_MEM_SIZE)
			return int_mem[address];
		else {
			printf("Address is greater than INT_MEM_SIZE\n");
			return 0;
		}
		break;
	case EXT_MEM_ID:
		if (address < EXT_MEM_SIZE)
			return ext_mem[address];
		else {
			printf("Address is greater than EXT_MEM_SIZE\n");
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
DumpMem(char *buf, char *Address, int memory_id)
{
	unsigned int MemAddress;
	int Offset, Column;
	int size = 256;
	int k = 0;

	if (strlen(Address) != 0) {
		if (STREQ(Address, "PC"))
			MemAddress = cpu8051.pc;
		else
			MemAddress = Ascii2Hex(Address, strlen(Address));
	} else {
		MemAddress = 0;
	}

	for (Offset = 0; Offset < size; Offset += 16) {
		unsigned char data[16];

		sprintf(&buf[k], "%.4X ", MemAddress + Offset);
		k = strlen(buf);

		for (Column = 0; Column < 16; Column++) {
			data[Column] = memory_read8(memory_id, MemAddress +
						    Offset + Column);
			sprintf(&buf[k], " %.2X", (int) data[Column]);
			k = strlen(buf);
		}
		sprintf(&buf[k], "  ");
		k = strlen(buf);

		/* Display any ASCII characters */
		for (Column = 0; Column < 16; Column++) {
			if ((int) data[Column] >= 32 &&
			    (int) data[Column] <= 126) {
				sprintf(&buf[k], "%c", data[Column]);
				k = strlen(buf);
			} else {
				sprintf(&buf[k], ".");
				k = strlen(buf);
			}
		}
		sprintf(&buf[k], "\n");
		k = strlen(buf);
	}
}
