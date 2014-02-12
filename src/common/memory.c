/*
 * memory.c
 *
 * Copyright (C) 1999 Jonathan St-André
 * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This file is released under the GPLv2
 */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "cpu8051.h"
#include "reg8051.h"
#include "hexfile.h"
#include "memory.h"
#include "options.h"

struct mem_infos_t {
	int size;
	int max_size;
	u_int8_t *buf;
};

struct mem_infos_t mem_infos[MEM_ID_COUNT];

extern struct options_t options;

/* Init each 8051 memory sections. */
void
memory_init(void)
{
	int k;
	struct mem_infos_t *m;

	/* Set desired and maximum allowable sizes for each memory type. */
	mem_infos[PGM_MEM_ID].size = options.pram_size;
	mem_infos[PGM_MEM_ID].max_size = PGM_MEM_MAX_SIZE;

	mem_infos[INT_MEM_ID].size = options.iram_size;
	mem_infos[INT_MEM_ID].max_size = INT_MEM_MAX_SIZE;

	mem_infos[EXT_MEM_ID].size = options.xram_size;
	mem_infos[EXT_MEM_ID].max_size = EXT_MEM_MAX_SIZE;

	/* Verify if desired sizes are valid, and if so allocate memory. */
	for (k = 0; k < MEM_ID_COUNT; k++) {
		m = &mem_infos[k];

		if (m->size > m->max_size) {
			log_err("Memory size invalid (max = %d)", m->max_size);
			exit(1);
		}

		m->buf = malloc(m->size);
		if (m->buf == NULL) {
			log_err("%s", strerror(errno));
			exit(1);
		}

		memset(m->buf, 0x00, m->size);
	}
}

/* Return true if address is valid, false otherwise. */
int
memory_check_address(enum mem_id_t id, unsigned long address, int display_error)
{
	if (address >= (unsigned long) mem_infos[id].max_size) {
		if (display_error == DISPLAY_ERROR_YES)
			log_err("Address out of range ($%X >= $%X", address,
				mem_infos[id].max_size - 1);
		return false;
	} else {
		return true;
	}
}

u_int8_t *
memory_getbuf(enum mem_id_t id, unsigned long address)
{
	return &mem_infos[id].buf[address];
}

void
memory_clear(enum mem_id_t id)
{
	memset(mem_infos[id].buf, 0, mem_infos[id].size);
}

void
memory_write8(enum mem_id_t id, unsigned long address, u_int8_t value)
{
	if (address >= (unsigned long) mem_infos[id].max_size) {
		log_err("Error writing to memory ID: %d\n"
			"  Address (%lu) greater than maximum memory size",
			id, address);
		return;
	} else {
		mem_infos[id].buf[address] = value;
	}
}

void
memory_sfr_write8(unsigned long address, u_int8_t value)
{
	/* SFR registers are from addresses $80 to $FF. */
	memory_write8(INT_MEM_ID, address, value);
}

void
memory_sfr_write_dptr(u_int16_t value)
{
	memory_write8(INT_MEM_ID, _DPTRHIGH_, value >> 8);
	memory_write8(INT_MEM_ID, _DPTRLOW_, (uint8_t) value);
}

u_int8_t
memory_read8(enum mem_id_t id, unsigned long address)
{
	if (address >= (unsigned long) mem_infos[id].max_size) {
		log_err("Error reading from memory ID: %d\n"
			"  Address (%lu) greater than maximum memory size",
			id, address);
		return 0;
	} else {
		return mem_infos[id].buf[address];
	}
}

u_int8_t
memory_sfr_read8(unsigned long address)
{
	/* SFR registers are from addresses $80 to $FF. */
	return memory_read8(INT_MEM_ID, address);
}

u_int16_t
memory_sfr_read_dptr(void)
{
	return (memory_read8(INT_MEM_ID, _DPTRHIGH_) << 8) +
		memory_read8(INT_MEM_ID, _DPTRLOW_);
}

void
stack_push8(uint8_t value)
{
	uint8_t sp;

	sp = memory_read8(INT_MEM_ID, _SP_);

	memory_write8(INT_MEM_ID, ++sp, value);
	memory_write8(INT_MEM_ID, _SP_, sp); /* Save new stack pointer */
}

void
stack_push16(uint16_t value)
{
	uint8_t sp;

	sp = memory_read8(INT_MEM_ID, _SP_);

	memory_write8(INT_MEM_ID, ++sp, (uint8_t) value); /* Write LSB */
	memory_write8(INT_MEM_ID, ++sp, value >> 8);      /* Write MSB */
	memory_write8(INT_MEM_ID, _SP_, sp); /* Save new stack pointer */
}

uint8_t
stack_pop8(void)
{
	uint8_t sp;
	uint8_t value;

	sp = memory_read8(INT_MEM_ID, _SP_);

	value = memory_read8(INT_MEM_ID, sp--);
	memory_write8(INT_MEM_ID, _SP_, sp); /* Save new stack pointer */

	return value;
}

uint16_t
stack_pop16(void)
{
	uint8_t sp;
	uint16_t value;

	sp = memory_read8(INT_MEM_ID, _SP_);

	value = memory_read8(INT_MEM_ID, sp--) << 8; /* Read MSB */
	value |= memory_read8(INT_MEM_ID, sp--);     /* Read LSB */
	memory_write8(INT_MEM_ID, _SP_, sp); /* Save new stack pointer */

	return value;
}

/* Read a 16-bit address from PGM memory, starting at <base> offset */
uint16_t
pgm_read_addr16(uint16_t base)
{
	uint16_t addr;

	addr = memory_read8(PGM_MEM_ID, base) << 8; /* MSB */
	addr |= memory_read8(PGM_MEM_ID, base + 1); /* LSB */

	return addr;
}

/* Dump memory */
void
memory_dump(unsigned int address, int size, int memory_id)
{
	int rc;
	int offset, col;

	if (size == 0) {
		log_err("invalid size: 0");
		return;
	}

	/* Validate start address. */
	rc = memory_check_address(memory_id, address, DISPLAY_ERROR_YES);
	if (!rc) {
		/* Validate end address. */
		rc = memory_check_address(memory_id, address + (size - 1),
					  DISPLAY_ERROR_NO);
		if (!rc)
			log_err("Trying to read beyond memory limit");
	}

	if (!rc)
		return;

	for (offset = 0; offset < size; offset += 16) {
		unsigned char data[16];

		printf("%.4X ", address + offset);

		for (col = 0; col < 16; col++) {
			data[col] = memory_read8(memory_id, address +
						 offset + col);
			printf(" %.2X", (int) data[col]);
		}
		printf("  ");

		/* Display any ASCII characters */
		for (col = 0; col < 16; col++) {
			if ((int) data[col] >= 32 &&
			    (int) data[col] <= 126)
				printf("%c", data[col]);
			else
				printf(".");
		}
		printf("\n");
	}
}
