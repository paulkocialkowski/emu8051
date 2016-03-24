/*
 * kb9012.c -- KB9012 functions.
 *
 * Copyright (C) 2016 Paul Kocialkowski <contact@paulk.fr>
 *
 * This file is released under the GPLv2
 */

#include <stdlib.h>
#include <stdbool.h>

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include "common.h"
#include "memory.h"
#include "iotrace.h"
#include "kb9012.h"
#include "reg8051.h"
#include "regkb9012.h"

bool
kb9012_memory_allowed(enum mem_id_t id, unsigned int address, bool write)
{
	if (id == EXT_MEM_ID)
		if (address < (_XRAM_ + XRAM_SIZE))
			return false;

	switch (id) {
		case SFR_MEM_ID:
			switch (address) {
				case _IVHA_:
				case _SCON2_:
				case _SCON3_:
				case _SCON4_:
					return false;
				case _PCON2_:
					return write;
				default:
					return true;
			}
		case EXT_MEM_ID:
			if (address >= 0xfe80 && address <= 0xfe8f) // wdt
				return false;

			if (address >= _XBISEG0_ && address <= _XBIMISC_)
				return !write;

			switch (address) {
				case _XBISEG0_:
				case _XBISEG3_:
				case _XBIMISC_:
				case _CLKCFG_:
				case _PLLCFG_:
				case _CLKCFG2_:
				case _PLLCFG2_:
					return false;
				case _GPIO_FS10_:
				case _GPIO_OE10_:
				case _GPIO_IE10_:
					return write;
				default:
					return true;
			}
		default:
			return true;
	}
}

uint8_t
kb9012_memory_write_filter(enum mem_id_t id, unsigned int address, uint8_t value)
{
	uint8_t flags_always = 0;
	uint8_t flags_never = 0;

	switch (id) {
		case SFR_MEM_ID:
			switch (address) {
				case _PCON2_:
					flags_always = PCON2_FLAG_EXT_ENABLE;
					break;
			}
			break;
		case EXT_MEM_ID:
			switch (address) {
				case _GPIO_FS10_:
				case _GPIO_OE10_:
				case _GPIO_IE10_:
					flags_always = GPIO_FLAG_E51TXD |
						       GPIO_FLAG_E51RXD;
					break;
			}
		default:
			break;
	}

	return (value | flags_always) & ~flags_never;
}

void
kb9012_memory_map(enum mem_id_t *id, unsigned long *address)
{
	uint8_t value;

	if (*id == EXT_MEM_ID) {
		if (*address < _XRAM_) {
			/*
			 * Values below XRAM are mapped to program memory.
			 * Segment remapping also applies with external memory
			 * access.
			 */

			*id = PGM_MEM_ID;
		}
	}

	if (*id == PGM_MEM_ID) {
		if (*address <= XBISEG_SIZE) {
			value = mem_read8(EXT_MEM_ID, _XBISEG0_, true);
			if (value & XBISEG_FLAG_ENABLE)
				*address = (value & XBISEG_MASK) * XBISEG_SIZE +
					   (*address & XBISEG_ADDRESS_MASK);
		} else if (*address >= (3 * XBISEG_SIZE)) {
			value = mem_read8(EXT_MEM_ID, _XBISEG3_, true);
			if (value & XBISEG_FLAG_ENABLE)
				*address = (value & XBISEG_MASK) * XBISEG_SIZE +
					   (*address & XBISEG_ADDRESS_MASK);
		}
	}
}

void
kb9012_memory_read(enum mem_id_t id, unsigned int address, uint8_t *value)
{
	switch (id) {
		case EXT_MEM_ID:
			if (address >= _XBISEG0_ && address <= _XBIMISC_)
				iotrace_message("XBI module access");
			break;
		default:
			break;
	}
}

void
kb9012_memory_write(enum mem_id_t id, unsigned int address, uint8_t value)
{
	switch (id) {
		case SFR_MEM_ID:
			if (address == _IVHA_)
				iotrace_message("Interrupt vector high address "
						"change");
			break;
		case EXT_MEM_ID:
			if (address >= _XBISEG0_ && address <= _XBIMISC_)
				iotrace_message("XBI module access");
			break;
		default:
			break;
	}
}
