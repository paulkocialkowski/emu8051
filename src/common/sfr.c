/*
 * sfr.c
 *
 * Copyright (C) 2013 Hugo Villeneuve <hugo@hugovil.com>
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

#if HAVE_CONFIG_H
#  include "config.h"
#endif

#include <stdio.h>

#include "common.h"
#include "reg8051.h"
#include "cpu8051.h"
#include "sfr.h"
#include "memory.h"
#include "instructions_8051.h"

#define HEX_DIGITS_2 2
#define HEX_DIGITS_4 4

/* Specific read/write functions for special registers. */

static unsigned int
regwin_read_pc(int dummy)
{
	(void) dummy; /* Remove compiler warning about unused variable. */

	return cpu8051.pc;
}

static void
regwin_write_pc(int param, int val)
{
	(void) param; /* Remove compiler warning about unused variable. */

	cpu8051.pc = (u_int16_t) val;
}

static unsigned int
regwin_read_timer(int timer_low_addr)
{
	return (memory_sfr_read8(timer_low_addr + 2) << 8) |
		memory_sfr_read8(timer_low_addr);
}

static void
regwin_write_timer(int timer_low_addr, int val)
{
	memory_sfr_write8(timer_low_addr + 2, (u_int8_t) ((val & 0x0000FFFF) >> 8));
	memory_sfr_write8(timer_low_addr, (u_int8_t) val);
}

static u_int8_t
regwin_read_bank_offset(void)
{
	return memory_sfr_read8(_PSW_) & 0x18;
}

static unsigned int
regwin_read_bank(int dummy)
{
	(void) dummy; /* Remove compiler warning about unused variable. */

	return regwin_read_bank_offset() >> 3;
}

static void
regwin_write_bank(int param, int bank_number)
{
	u_int8_t psw = memory_sfr_read8(_PSW_);

	(void) param; /* Remove compiler warning about unused variable. */

	if ((bank_number < 0) || (bank_number > 3)) {
		log_info("Error: invalid bank number: %d", bank_number);
		bank_number = 0;
	}

	memory_sfr_write8(_PSW_, (psw & ~0x18) | (bank_number << 3));
}

/* Indirect read of R0 - R7 in current bank from internal memory. */
static unsigned int
regwin_read_rx(int offset)
{
	return memory_read8(INT_MEM_ID, regwin_read_bank_offset() + offset);
}

/* Indirect write to R0 - R7 in current bank to internal memory. */
static void
regwin_write_rx(int offset, int val)
{
	memory_write8(INT_MEM_ID, regwin_read_bank_offset() + offset, (u_int8_t) val);
}

/* This array defines how to read value for each register. */
static struct regwin_infos_t regwin_infos[SFR_REGS] = {
	{
		"PC",
		HEX_DIGITS_4,
		regwin_read_pc, regwin_write_pc,
		0, /* Dummy */
	},
	{
		"SP",
		HEX_DIGITS_2,
		NULL, NULL,
		_SP_,
	},
	{
		"PSW",
		HEX_DIGITS_2,
		NULL, NULL,
		_PSW_,
	},
	{
		"A",
		HEX_DIGITS_2,
		NULL, NULL,
		_ACC_,
	},
	{
		"B",
		HEX_DIGITS_2,
		NULL, NULL,
		_B_,
	},
	{
		"DPTR",
		HEX_DIGITS_4,
		NULL, NULL,
		_DPL_,
	},
	{
		"P0",
		HEX_DIGITS_2,
		NULL, NULL,
		_P0_,
	},
	{
		"P1",
		HEX_DIGITS_2,
		NULL, NULL,
		_P1_,
	},
	{
		"P2",
		HEX_DIGITS_2,
		NULL, NULL,
		_P2_,
	},
	{
		"P3",
		HEX_DIGITS_2,
		NULL, NULL,
		_P3_,
	},
	{
		"TCON",
		HEX_DIGITS_2,
		NULL, NULL,
		_TCON_,
	},
	{
		"TMOD",
		HEX_DIGITS_2,
		NULL, NULL,
		_TMOD_,
	},
	{
		"TIMER0",
		HEX_DIGITS_4,
		regwin_read_timer, regwin_write_timer,
		_TL0_,
	},
	{
		"TIMER1",
		HEX_DIGITS_4,
		regwin_read_timer, regwin_write_timer,
		_TL1_,
	},
	{
		"SCON",
		HEX_DIGITS_2,
		NULL, NULL,
		_SCON_,
	},
	{
		"IE",
		HEX_DIGITS_2,
		NULL, NULL,
		_IE_,
	},
	{
		"IP",
		HEX_DIGITS_2,
		NULL, NULL,
		_IP_,
	},
	{
		"BANK",
		HEX_DIGITS_2,
		regwin_read_bank, regwin_write_bank,
		0, /* Dummy */
	},
	/* R0-R7 Registers in current Bank */
	{
		"R0",
		HEX_DIGITS_2,
		regwin_read_rx, regwin_write_rx,
		_R0_,
	},
	{
		"R1",
		HEX_DIGITS_2,
		regwin_read_rx, regwin_write_rx,
		_R1_,
	},
	{
		"R2",
		HEX_DIGITS_2,
		regwin_read_rx, regwin_write_rx,
		_R2_,
	},
	{
		"R3",
		HEX_DIGITS_2,
		regwin_read_rx, regwin_write_rx,
		_R3_,
	},
	{
		"R4",
		HEX_DIGITS_2,
		regwin_read_rx, regwin_write_rx,
		_R4_,
	},
	{
		"R5",
		HEX_DIGITS_2,
		regwin_read_rx, regwin_write_rx,
		_R5_,
	},
	{
		"R6",
		HEX_DIGITS_2,
		regwin_read_rx, regwin_write_rx,
		_R6_,
	},
	{
		"R7",
		HEX_DIGITS_2,
		regwin_read_rx, regwin_write_rx,
		_R7_,
	},
};

/* Generic read/write functions */
static unsigned int
regwin_read_generic(int addr, int width)
{
	if (width == 2)
		return memory_sfr_read8(addr);
	else if (width == 4) {
		/* Address is low address. */
		return (memory_sfr_read8(addr + 1) << 8) |
			memory_sfr_read8(addr);
	} else
		return 0xFFFFFFFF;
}

static void
regwin_write_generic(int addr, int val, int width)
{
	if (width == 2)
		memory_sfr_write8(addr, (u_int8_t) val);
	else if (width == 4) {
		/* Address is low address. */
		memory_sfr_write8(addr + 1, (u_int8_t) ((val & 0x0000FFFF) >> 8));
		memory_sfr_write8(addr, (u_int8_t) val);
	}
};

int
regwin_read(int row)
{
	int val;

	if (regwin_infos[row].read_func == NULL) {
		/*
		 * Read register value using generic 8 or 16 bits read
		 * function, depending on width.
		 */
		val = regwin_read_generic(regwin_infos[row].param,
					  regwin_infos[row].w);
	} else {
		/* Read register value using custom function pointer. */
		val = regwin_infos[row].read_func(
			regwin_infos[row].param);
	}

	return val;
}

int
regwin_write(struct regwin_infos_t *p, int new)
{
	int max_value;

	max_value = (1 << (4 * p->w)) - 1; /* 16^w - 1 */

	/* Check that the new value is not too large for the register type. */
	if (new > max_value) {
		/* Display message for CLI version */
		printf("Value out of range\n");
		return -1;
	}

	if (p->write_func == NULL) {
		/*
		 * Write register value using generic 8 or 16 bits write
		 * function, depending on width.
		 */
		regwin_write_generic(p->param, new, p->w);
	} else {
		/* Write register value using custom function pointer. */
		p->write_func(p->param, new);
	}

	return 0;
}

struct regwin_infos_t *
sfr_get_infos(const char *regname)
{
	int row;

	for (row = 0; row < SFR_REGS; row++) {
		if (strcmp(regwin_infos[row].name, regname) == 0)
			return &regwin_infos[row];
	}

	return NULL; /* Register not found. */
}

struct regwin_infos_t *
sfr_get_infos_from_row(int row)
{
	return &regwin_infos[row];
}

