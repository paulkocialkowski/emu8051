/*
 * timers.c
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
#include <stdbool.h>
#include <stdint.h>
#include "config.h"

#include "common.h"
#include "reg8051.h"
#include "cpu8051.h"
#include "memory.h"
#include "psw.h"
#include "options.h"
#include "instructions_8051.h"

extern struct options_t options;

static void
timer_increment_check_overflow(uint8_t counter_address, uint8_t tf_mask)
{
	unsigned int tmp;

	tmp = cpu8051_ReadD(counter_address);
	tmp++;
	tmp &= 0xFF;
	if (tmp == 0) {
		/* If overflow set TFx */
		cpu8051_WriteD(_TCON_, cpu8051_ReadD(_TCON_) | tf_mask);
	}

	cpu8051_WriteD(counter_address, tmp); /* Save new value. */
}

static void
timer_with_prescaler(uint8_t tl, uint8_t th, uint8_t tf_mask, int prescaler_width)
{
	unsigned int prescaler;

	prescaler = cpu8051_ReadD(tl);
	prescaler++;

	prescaler &= (1 << prescaler_width) - 1; /* Keep only required bits */
	cpu8051_WriteD(tl, prescaler);

	if (prescaler == 0)
		timer_increment_check_overflow(th, tf_mask);
}

static void
process_timer(uint8_t tl, uint8_t th, uint8_t tf_mask, uint8_t TR, uint8_t mode,
	      uint8_t GATE, uint32_t TimerCounter)
{
	unsigned int tmp;

	switch (mode) {
	case 0:
		/* Mode 0, 8-bit timer "TH" with "TL" as 5-bit prescaler. */
		timer_with_prescaler(tl, th, tf_mask, 5);
		break;
	case 1:
		/* Mode 1, 16-bits counter */
		timer_with_prescaler(tl, th, tf_mask, 8);
		break;
	case 2:
		/* Mode 2, 8-bits counter with Auto-Reload */
		tmp = cpu8051_ReadD(tl);
		tmp++;
		tmp &= 0xFF;
		if (tmp == 0) {
			/* If overflow -> reload and set TF0 */
			cpu8051_WriteD(_TCON_, cpu8051_ReadD(_TCON_) | tf_mask);
			cpu8051_WriteD(tl, cpu8051_ReadD(th));
		} else
			cpu8051_WriteD(tl, tmp);
		break;
	case 3:
		/*
		 * Mode 3:
		 *   inactive mode for timer 1
		 *   2 independents 8-bits timers for timer 0.
		 */

		if (tl == _TL1_)
			break;

		if (TR && !GATE && !TimerCounter)
			timer_increment_check_overflow(tl, tf_mask);

		/* TH0 uses TR1 et TF1. */
		TR = cpu8051_ReadD(_TCON_) & 0x40;

		if (TR)
			timer_increment_check_overflow(th, 0x80);

		break;
	}
}

/* Run timers */
void
timers_check(void)
{
	unsigned int TR;
	unsigned int MODE;
	unsigned int GATE;
	unsigned int TimerCounter;

	/* Timer 0 */
	TR = cpu8051_ReadD(_TCON_) & 0x10;
	MODE = cpu8051_ReadD(_TMOD_) & 0x03;
	GATE = cpu8051_ReadD(_TMOD_) & 0x08;
	TimerCounter = cpu8051_ReadD(_TMOD_) & 0x04;

	if ((TR && !GATE && !TimerCounter) || (MODE == 3))
		process_timer(_TL0_, _TH0_, 0x20, TR, MODE, GATE, TimerCounter);

	/* Timer 1 */
	TR = cpu8051_ReadD(_TCON_) & 0x40;
	MODE = (cpu8051_ReadD(_TMOD_) & 0x30) >> 4 ;
	GATE = cpu8051_ReadD(_TMOD_) & 0x80;
	TimerCounter = cpu8051_ReadD(_TMOD_) & 0x40;

	if (TR && !GATE && !TimerCounter)
		process_timer(_TL1_, _TH1_, 0x80, TR, MODE, GATE, TimerCounter);
}
