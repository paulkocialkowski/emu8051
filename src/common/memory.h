/*
 * memory.h
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

#ifndef MEMORY_H
#define MEMORY_H 1

#include <sys/types.h>

#define PGM_MEM_MAX_SIZE 65536
/*
 *   Direct addressing $00 to $7F = IRAM (8051)
 *   Direct addressing $80 to $FF = SFR  (8051)
 * Indirect addressing $80 to $FF = IRAM (8052)
 */
#define INT_MEM_MAX_SIZE   256
#define EXT_MEM_MAX_SIZE 65536

#define PGM_MEM_DEFAULT_SIZE PGM_MEM_MAX_SIZE
#define EXT_MEM_DEFAULT_SIZE 256

enum mem_id_t {
	PGM_MEM_ID,
	INT_MEM_ID,
	EXT_MEM_ID,
	MEM_ID_COUNT
};

#define DISPLAY_ERROR_NO  0
#define DISPLAY_ERROR_YES 1

void
memory_init(void);

int
memory_check_address(enum mem_id_t id, unsigned long address, int display_error);

u_int8_t *
memory_getbuf(enum mem_id_t id, unsigned long address);

void
memory_clear(enum mem_id_t id);

void
memory_write8(enum mem_id_t id, unsigned long address, u_int8_t value);

void
memory_sfr_write8(unsigned long address, u_int8_t value);

void
memory_sfr_write_dptr(u_int16_t value);

u_int8_t
memory_read8(enum mem_id_t id, unsigned long address);

u_int8_t
memory_sfr_read8(unsigned long address);

u_int16_t
memory_sfr_read_dptr(void);

void
stack_push8(uint8_t value);

void
stack_push16(uint16_t value);

uint8_t
stack_pop8(void);

uint16_t
stack_pop16(void);

uint16_t
pgm_read_addr16(uint16_t base);

void
DumpMem(unsigned int address, int size, int memory_id);

#endif /* MEMORY_H */
