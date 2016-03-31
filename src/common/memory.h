/*
 * memory.h
 *
 * Copyright (C) 1999 Jonathan St-André
 * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This file is released under the GPLv2
 */

#ifndef MEMORY_H
#define MEMORY_H 1

#include <stdint.h>

#if HAVE_CONFIG_H
#include "config.h"
#endif

#if HAVE_KB9012
#define PGM_MEM_MAX_SIZE 131072
#else
#define PGM_MEM_MAX_SIZE 65536
#endif

/*
 *   Direct addressing $00 to $7F = IRAM (8051)
 *   Direct addressing $80 to $FF = SFR  (8051)
 * Indirect addressing $80 to $FF = IRAM (8052)
 */
#define INT_MEM_MAX_SIZE 256
#define SFR_MEM_MAX_SIZE 256
#define EXT_MEM_MAX_SIZE 65536

#define PGM_MEM_DEFAULT_SIZE 65536
#define SFR_MEM_DEFAULT_SIZE 256
#if HAVE_KB9012
#define EXT_MEM_DEFAULT_SIZE 65536
#else
#define EXT_MEM_DEFAULT_SIZE 256
#endif

enum mem_id_t {
	PGM_MEM_ID,
	INT_MEM_ID,
	SFR_MEM_ID,
	EXT_MEM_ID,
	MEM_ID_COUNT
};

#define DISPLAY_ERROR_NO  0
#define DISPLAY_ERROR_YES 1

void
mem_init(void);

int
mem_check_address(enum mem_id_t id, unsigned long address, int display_error);

void
mem_convert_bit_address(uint8_t bit_address, uint8_t *byte_address,
			uint8_t *bit_number);

uint8_t *
mem_getbuf(enum mem_id_t id, unsigned long address);

void
mem_clear(enum mem_id_t id);

void
mem_write8(enum mem_id_t id, unsigned long address, uint8_t value, int cached);

void
mem_write_direct(unsigned int address, unsigned char value, int cached);

void
mem_write_indirect(unsigned int address, unsigned char value, int cached);

void
mem_write_external(unsigned int address, unsigned char value, int cached);

void
mem_write_bit(uint8_t bit_address, uint8_t value, int cached);

void
mem_sfr_write8(unsigned long address, uint8_t value, int cached);

void
mem_sfr_write_dptr(uint16_t value, int cached);

uint8_t
mem_read8(enum mem_id_t id, unsigned long address, int cached);

unsigned char
mem_read_direct(unsigned int address, int cached);

unsigned char
mem_read_indirect(unsigned int address, int cached);

unsigned char
mem_read_external(unsigned int address, int cached);

unsigned char
mem_read_bit(uint8_t bit_address, int cached);

uint8_t
mem_sfr_read8(unsigned long address, int cached);

uint16_t
mem_sfr_read_dptr(void);

void
stack_push8(uint8_t value);

void
stack_push16(uint16_t value);

uint8_t
stack_pop8(void);

uint16_t
stack_pop16(void);

uint16_t
pgm_read_addr16(uint16_t base, int cached);

void
mem_dump(unsigned int address, int size, enum mem_id_t id);

#endif /* MEMORY_H */
