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
#include "iotrace.h"
#include "hardware.h"
#include "memory.h"
#include "options.h"

struct mem_infos_t {
	int size;
	int max_size;
	uint8_t *buf;
};

struct mem_infos_t mem_infos[MEM_ID_COUNT];

extern struct options_t options;

/* Init each 8051 memory sections. */
void
mem_init(void)
{
	int k;
	struct mem_infos_t *m;

	/* Set desired and maximum allowable sizes for each memory type. */
	mem_infos[PGM_MEM_ID].size = options.pram_size;
	mem_infos[PGM_MEM_ID].max_size = PGM_MEM_MAX_SIZE;

	mem_infos[INT_MEM_ID].size = options.iram_size;
	mem_infos[INT_MEM_ID].max_size = INT_MEM_MAX_SIZE;

	mem_infos[SFR_MEM_ID].size = SFR_MEM_DEFAULT_SIZE;
	mem_infos[SFR_MEM_ID].max_size = SFR_MEM_MAX_SIZE;

	mem_infos[EXT_MEM_ID].size = options.xram_size;
	mem_infos[EXT_MEM_ID].max_size = EXT_MEM_MAX_SIZE;

	/* Verify if desired sizes are valid, and if so allocate memory. */
	for (k = 0; k < MEM_ID_COUNT; k++) {
		m = &mem_infos[k];

		if (m->size > m->max_size) {
			log_fail("Memory size invalid (max = %d)", m->max_size);
		}

		m->buf = malloc(m->size);
		if (m->buf == NULL) {
			log_fail("%s", strerror(errno));
		}

		memset(m->buf, 0x00, m->size);
	}
}

/* Return true if address is valid, false otherwise. */
int
mem_check_address(enum mem_id_t id, unsigned long address, int display_error)
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

void
mem_convert_bit_address(uint8_t bit_address, uint8_t *byte_address,
			uint8_t *bit_number)
{
	if (bit_address > 0x7F) {
		/* SFR 80-FF */
		*byte_address = bit_address & 0xF8;
		*bit_number = bit_address & 0x07;
	} else {
		/* 20-2F */
		*byte_address = (bit_address >> 3) + 0x20;
		*bit_number = bit_address & 0x07;
	}
}

uint8_t *
mem_getbuf(enum mem_id_t id, unsigned long address)
{
	return &mem_infos[id].buf[address];
}

void
mem_clear(enum mem_id_t id)
{
	memset(mem_infos[id].buf, 0, mem_infos[id].size);
}

void
mem_write8(enum mem_id_t id, unsigned long address, uint8_t value, int cached)
{
	enum mem_id_t mapped_id;
	unsigned long mapped_address;

	if (address >= (unsigned long) mem_infos[id].max_size) {
		log_err("Error writing to memory ID: %d\n"
			"  Address (%lu) greater than maximum memory size",
			id, address);
		return;
	} else {
		if (!cached) {
			hardware_memory_write(id, address, value);

			iotrace_memory_write(id, address, value);
		}

		mapped_id = id;
		mapped_address = address;
		hardware_memory_map(&mapped_id, &mapped_address);

		mem_infos[mapped_id].buf[mapped_address] = value;
	}
}

/* Write with a direct addressing mode at Address the new Value */
void
mem_write_direct(unsigned int address, unsigned char value, int cached)
{
	if (address > 0x7F)
		mem_write8(SFR_MEM_ID, address, value, cached);
	else
		mem_write8(INT_MEM_ID, address, value, cached);
}

/* Write with an indirect addressing mode at Address the new Value */
void
mem_write_indirect(unsigned int address, unsigned char value, int cached)
{
	mem_write8(INT_MEM_ID, address, value, cached);
}

/* Write to external memory space at Address the new Value */
void
mem_write_external(unsigned int address, unsigned char value, int cached)
{
	mem_write8(EXT_MEM_ID, address, value, cached);
}

/* Write with a bit addressing mode at BitAddress the new Value */
void
mem_write_bit(uint8_t bit_address, uint8_t value, int cached)
{
	uint8_t byte_address;
	uint8_t bit_number;
	unsigned char byte_val, byte_mask;

	mem_convert_bit_address(bit_address, &byte_address, &bit_number);

	byte_mask = ((1 << bit_number) ^ 0xFF);
	byte_val = mem_read_direct(byte_address, cached) & byte_mask;
	byte_val += value << bit_number;
	mem_write_direct(byte_address, byte_val, cached);
}

void
mem_sfr_write8(unsigned long address, uint8_t value, int cached)
{
	/* SFR registers are from addresses $80 to $FF. */
	mem_write8(SFR_MEM_ID, address, value, cached);
}

void
mem_sfr_write_dptr(uint16_t value, int cached)
{
	mem_write8(SFR_MEM_ID, _DPTRHIGH_, value >> 8, cached);
	mem_write8(SFR_MEM_ID, _DPTRLOW_, (uint8_t) value, cached);
}

uint8_t
mem_read8(enum mem_id_t id, unsigned long address, int cached)
{
	enum mem_id_t mapped_id;
	unsigned long mapped_address;
	uint8_t value;

	if (address >= (unsigned long) mem_infos[id].max_size) {
		log_err("Error reading from memory ID: %d\n"
			"  Address (%lu) greater than maximum memory size",
			id, address);
		return 0;
	} else {
		mapped_id = id;
		mapped_address = address;
		hardware_memory_map(&mapped_id, &mapped_address);

		value = mem_infos[mapped_id].buf[mapped_address];

		if (!cached) {
			hardware_memory_read(id, address, &value);

			iotrace_memory_read(id, address, value);
		}

		return value;
	}
}

/* Read with a direct addressing mode at Address */
unsigned char
mem_read_direct(unsigned int address, int cached)
{
	if (address > 0x7F)
		return mem_read8(SFR_MEM_ID, address, cached);
	else
		return mem_read8(INT_MEM_ID, address, cached);
}

/* Read with a indirect addressing mode at Address */
unsigned char
mem_read_indirect(unsigned int address, int cached)
{
	return mem_read8(INT_MEM_ID, address, cached);
}

/* Read to external memory space at Address the new Value */
unsigned char
mem_read_external(unsigned int address, int cached)
{
	return mem_read8(EXT_MEM_ID, address, cached);
}

/* Read with a bit addressing mode at bit_address */
unsigned char
mem_read_bit(uint8_t bit_address, int cached)
{
	uint8_t byte_address;
	uint8_t bit_number;
	unsigned char bit_value;

	mem_convert_bit_address(bit_address, &byte_address, &bit_number);
	bit_value = (mem_read_direct(byte_address, cached) >> bit_number);
	bit_value &= 1;
	return bit_value;
}

uint8_t
mem_sfr_read8(unsigned long address, int cached)
{
	/* SFR registers are from addresses $80 to $FF. */
	return mem_read8(SFR_MEM_ID, address, cached);
}

uint16_t
mem_sfr_read_dptr(void)
{
	return (mem_read8(SFR_MEM_ID, _DPTRHIGH_, true) << 8) +
		mem_read8(SFR_MEM_ID, _DPTRLOW_, true);
}

void
stack_push8(uint8_t value)
{
	uint8_t sp;

	sp = mem_read8(SFR_MEM_ID, _SP_, true);

	mem_write8(INT_MEM_ID, ++sp, value, true);
	mem_write8(SFR_MEM_ID, _SP_, sp, true); /* Save new stack pointer */
}

void
stack_push16(uint16_t value)
{
	uint8_t sp;

	sp = mem_read8(SFR_MEM_ID, _SP_, true);

	mem_write8(INT_MEM_ID, ++sp, (uint8_t) value, true); /* Write LSB */
	mem_write8(INT_MEM_ID, ++sp, value >> 8, true);      /* Write MSB */
	mem_write8(SFR_MEM_ID, _SP_, sp, true); /* Save new stack pointer */
}

uint8_t
stack_pop8(void)
{
	uint8_t sp;
	uint8_t value;

	sp = mem_read8(SFR_MEM_ID, _SP_, true);

	value = mem_read8(INT_MEM_ID, sp--, true);
	mem_write8(SFR_MEM_ID, _SP_, sp, true); /* Save new stack pointer */

	return value;
}

uint16_t
stack_pop16(void)
{
	uint8_t sp;
	uint16_t value;

	sp = mem_read8(SFR_MEM_ID, _SP_, true);

	value = mem_read8(INT_MEM_ID, sp--, true) << 8; /* Read MSB */
	value |= mem_read8(INT_MEM_ID, sp--, true);     /* Read LSB */
	mem_write8(SFR_MEM_ID, _SP_, sp, true); /* Save new stack pointer */

	return value;
}

/* Read a 16-bit address from PGM memory, starting at <base> offset */
uint16_t
pgm_read_addr16(uint16_t base, int cached)
{
	uint16_t addr;

	addr = mem_read8(PGM_MEM_ID, base, cached) << 8; /* MSB */
	addr |= mem_read8(PGM_MEM_ID, base + 1, cached); /* LSB */

	return addr;
}

/* Dump memory */
void
mem_dump(unsigned int address, int size, enum mem_id_t id)
{
	int rc;
	int offset, col;

	if (size == 0) {
		log_err("invalid size: 0");
		return;
	}

	/* Validate start address. */
	rc = mem_check_address(id, address, DISPLAY_ERROR_YES);
	if (!rc) {
		/* Validate end address. */
		rc = mem_check_address(id, address + (size - 1),
				       DISPLAY_ERROR_NO);
		if (!rc)
			log_err("Trying to read beyond memory limit");
	}

	if (!rc)
		return;

	for (offset = 0; offset < size; offset += 16) {
		uint8_t data[16];

		printf("%.4X ", address + offset);

		for (col = 0; col < 16; col++) {
			data[col] = mem_read8(id, address + offset + col, true);
			printf(" %.2X", data[col]);
		}
		printf("  ");

		/* Display any ASCII characters */
		for (col = 0; col < 16; col++) {
			if ((int) data[col] >= 32 &&
			    (int) data[col] <= 126)
				printf("%c", (char) data[col]);
			else
				printf(".");
		}
		printf("\n");
	}
}
