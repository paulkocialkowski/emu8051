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

static uint8_t
kb9012_gpio_base(unsigned int address)
{
	uint8_t offset;
	uint8_t base;

	offset = address & GPIO_OFFSET_SPAN;

	if (offset >= GPIO_OFFSET_A0 && offset < GPIO_OFFSET_D0)
		base = 0xA0 + (offset - GPIO_OFFSET_A0) << 3;
	else if (offset == GPIO_OFFSET_D0)
		base = 0xD0;
	else
		base = offset << GPIO_OFFSET_SHIFT;

	return base;
}

void
kb9012_interrupt_address(int index, unsigned int *address)
{
	*address += (mem_sfr_read8(_IVHA_, true) & ~IVHA_MASK) << 8;
}

bool
kb9012_memory_allowed(enum mem_id_t id, unsigned int address, bool write)
{
	if (id == EXT_MEM_ID)
		if (address < (XRAM_BASE + XRAM_SIZE))
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
//			if (address >= 0xfe80 && address <= 0xfe8f) // wdt
//				return false;

//			if (address >= XBISEG0 && address <= XBIMISC)
//				return !write;

			switch (address) {
				case XBISEG0:
				case XBISEG3:
				case XBIMISC:
				case CLKCFG:
				case PLLCFG:
				case CLKCFG2:
				case PLLCFG2:
					return false;
				case GPIO_FS10:
				case GPIO_OE10:
				case GPIO_IE10:
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
				case GPIO_FS10:
				case GPIO_OE10:
				case GPIO_IE10:
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
		if (*address < XRAM_BASE) {
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
			value = mem_read8(EXT_MEM_ID, XBISEG0, true);
			if (value & XBISEG_FLAG_ENABLE)
				*address = (value & XBISEG_MASK) * XBISEG_SIZE +
					   (*address & XBISEG_ADDRESS_MASK);
		} else if (*address >= (3 * XBISEG_SIZE)) {
			value = mem_read8(EXT_MEM_ID, XBISEG3, true);
			if (value & XBISEG_FLAG_ENABLE)
				*address = (value & XBISEG_MASK) * XBISEG_SIZE +
					   (*address & XBISEG_ADDRESS_MASK);
		}
	}
}

static void
kb9012_memory_read_ext(unsigned int address, uint8_t *value)
{
	uint8_t gpio_base;

			if (address >= XBISEG0 && address <= XBIMISC)
				iotrace_message("XBI module access");

			if (address >= GPIO_IN_BASE && address < GPIO_PU_BASE) {
				gpio_base = kb9012_gpio_base(address);
				// TODO iotrace_message
				printf("GPIO%02X read at 0x%x\n",
						gpio_base, address);
			}
}

void
kb9012_memory_read(enum mem_id_t id, unsigned int address, uint8_t *value)
{
	switch (id) {
		case EXT_MEM_ID:
			kb9012_memory_read_ext(address, value);
			break;
		default:
			break;
	}
}

static void
kb9012_memory_write_ext(unsigned int address, uint8_t value)
{
	const char *gpio_prefixes[] = {
		"GPIOFS",
		"GPIOOE",
		"GPIOD",
		"GPIOIN",
		"GPIOPU",
		"GPIOOD",
		"GPIOIE",
		NULL
	};
	uint8_t gpio_base;
	unsigned int limit = GPIO_FS_BASE;
	unsigned int index;

	if (address >= XBISEG0 && address <= XBIMISC) {
		iotrace_message("XBI module access");
		return;
	}

	for (index = 0; gpio_prefixes[index] != NULL; index++) {
		limit += 0x10;

		if (address < (limit - 0x10) || address >= limit)
			continue;

		gpio_base = kb9012_gpio_base(address);
		// TODO iotrace_message
		printf("%s%02X write at 0x%x\n",
				gpio_prefixes[index], gpio_base,
				address);
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
			kb9012_memory_write_ext(address, value);
			break;
		default:
			break;
	}
}
