/*
 * timers.c
 *
 * Copyright (C) 1999 Jonathan St-André
 * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This file is released under the GPLv2
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
#include "timers.h"

static int gp_timer_value[GP_TIMERS_COUNT];

extern struct options_t options;

void
gp_timer_reset(int id)
{
	log_debug("gp timer %d reset", id);
	gp_timer_value[id] = 0;
}

void
gp_timers_increment(int count)
{
	int id;

	log_debug("gp timers increment");

	for (id = 0; id < GP_TIMERS_COUNT; id++)
		gp_timer_value[id] += count;
}

int
gp_timer_read(int id)
{
	return gp_timer_value[id];
}

static void
timer_increment_check_overflow(uint8_t counter_address, uint8_t tf_mask)
{
	unsigned int tmp;

	tmp = memory_read_direct(counter_address);
	tmp++;
	tmp &= 0xFF;
	if (tmp == 0) {
		/* If overflow set TFx */
		memory_write_direct(_TCON_, memory_read_direct(_TCON_) | tf_mask);
	}

	memory_write_direct(counter_address, tmp); /* Save new value. */
}

static void
timer_with_prescaler(uint8_t tl, uint8_t th, uint8_t tf_mask,
		     int prescaler_width)
{
	unsigned int prescaler;

	prescaler = memory_read_direct(tl);
	prescaler++;

	prescaler &= (1 << prescaler_width) - 1; /* Keep only required bits */
	memory_write_direct(tl, prescaler);

	if (prescaler == 0)
		timer_increment_check_overflow(th, tf_mask);
}

static void
process_timer(uint8_t tl, uint8_t th, uint8_t tf_mask, uint8_t TR, uint8_t mode,
	      uint8_t gate, uint32_t timer_counter)
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
		tmp = memory_read_direct(tl);
		tmp++;
		tmp &= 0xFF;
		if (tmp == 0) {
			/* If overflow -> reload and set TF0 */
			memory_write_direct(_TCON_, memory_read_direct(_TCON_) | tf_mask);
			memory_write_direct(tl, memory_read_direct(th));
		} else {
			memory_write_direct(tl, tmp);
		}
		break;
	case 3:
		/*
		 * Mode 3:
		 *   inactive mode for timer 1
		 *   2 independents 8-bits timers for timer 0.
		 */

		if (tl == _TL1_)
			break;

		if (TR && !gate && !timer_counter)
			timer_increment_check_overflow(tl, tf_mask);

		/* TH0 uses TR1 et TF1. */
		TR = memory_read_direct(_TCON_) & 0x40;

		if (TR)
			timer_increment_check_overflow(th, 0x80);

		break;
	}
}

/* Run 8051 timers */
void
timers_check(void)
{
	unsigned int tr;
	unsigned int mode;
	unsigned int gate;
	unsigned int timer_counter;

	/* Timer 0 */
	tr = memory_read_direct(_TCON_) & 0x10;
	mode = memory_read_direct(_TMOD_) & 0x03;
	gate = memory_read_direct(_TMOD_) & 0x08;
	timer_counter = memory_read_direct(_TMOD_) & 0x04;

	if ((tr && !gate && !timer_counter) || (mode == 3))
		process_timer(_TL0_, _TH0_, 0x20, tr, mode, gate,
			      timer_counter);

	/* Timer 1 */
	tr = memory_read_direct(_TCON_) & 0x40;
	mode = (memory_read_direct(_TMOD_) & 0x30) >> 4 ;
	gate = memory_read_direct(_TMOD_) & 0x80;
	timer_counter = memory_read_direct(_TMOD_) & 0x40;

	if (tr && !gate && !timer_counter)
		process_timer(_TL1_, _TH1_, 0x80, tr, mode, gate,
			      timer_counter);
}
