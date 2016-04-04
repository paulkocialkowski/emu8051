/*
 * interrupt.c
 *
 * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>
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
#include "cpu8051.h"
#include "memory.h"
#include "interrupt.h"
#include "reg8051.h"
#include "hardware.h"

static int
interrupt_trigger(int index)
{
	switch (index) {
		case 0:
			return ((mem_read_direct(_TCON_, true) &
				TCON_FLAG_IE0) != 0);
		case 1:
			return ((mem_read_direct(_TCON_, true) &
				TCON_FLAG_TF0) != 0);
		case 2:
			return ((mem_read_direct(_TCON_, true) &
				TCON_FLAG_IE1) != 0);
		case 3:
			return ((mem_read_direct(_TCON_, true) &
				TCON_FLAG_TF1) != 0);
		case 4:
			return ((mem_read_direct(_SCON_, true) &
				(SCON_FLAG_RI | SCON_FLAG_TI)) != 0);
		default:
			return 0;
	}
}

static void
interrupt_clear(int index)
{
	/* Those interrupts are automatically cleared by hardware */
	switch (index) {
		case 0:
			mem_write_direct(_TCON_, mem_read_direct(_TCON_, true) &
					 ~TCON_FLAG_IE0, true);
			break;
		case 1:
			mem_write_direct(_TCON_, mem_read_direct(_TCON_, true) &
					 ~TCON_FLAG_TF0, true);
			break;
		case 2:
			mem_write_direct(_TCON_, mem_read_direct(_TCON_, true) &
					 ~TCON_FLAG_IE1, true);
			break;
		case 3:
			mem_write_direct(_TCON_, mem_read_direct(_TCON_, true) &
					 ~TCON_FLAG_TF1, true);
			break;
	}
}

void
interrupt(int *index, int *priority)
{
	int p;
	int i;

	if (*index > 0)
		return;

	if ((mem_read_direct(_IE_, true) & IE_FLAG_EA) == 0)
		return;

	for (i = 0; i < INTERRUPTS_COUNT; i++) {
		if ((mem_read_direct(_IE_, true) & INTERRUPT_MASK(i)) == 0)
			continue;

		p = (mem_read_direct(_IP_, true) & INTERRUPT_MASK(i)) ?
			INTERRUPT_PRIORITY_HIGH : INTERRUPT_PRIORITY_LOW;

		if (p <= *priority)
			continue;

		/*
		 * Current interrupt is enabled, its priority is strictly higher
		 * than previously triggered interrupts (either already running
		 * or previously selected within this loop) and it's ready to
		 * trigger, so it can become the most likely candidate.
		 */

		if (interrupt_trigger(i)) {
			interrupt_clear(i);

			*index = i;
			*priority = p;
		}
	}
}

void
interrupt_address(int index, unsigned int *address)
{
	*address = INTERRUPT_VECTOR_BASE + index * INTERRUPT_VECTOR_SIZE;
}
