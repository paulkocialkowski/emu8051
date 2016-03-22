/*
 * gp_timers.c
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
#include "gp_timers.h"

static int gp_timer_value[GP_TIMERS_COUNT];

int
gp_timer_read(int id)
{
	return gp_timer_value[id];
}

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

void
gp_timers_reset(void)
{
	int id;

	log_debug("gp timers reset");

	for (id = 0; id < GP_TIMERS_COUNT; id++)
		gp_timer_reset(id);
}
