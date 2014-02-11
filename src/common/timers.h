/*
 * timers.h
 *
 * Copyright (C) 2013 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This file is released under the GPLv2
 */

#ifndef TIMERS_H
#define TIMERS_H 1

#include <stdint.h>

/* Maximum of 4 for CLI version */
#define GP_TIMERS_COUNT 2

void
gp_timer_reset(int id);

void
gp_timers_increment(int count);

int
gp_timer_read(int id);

void
timers_check(void);

#endif /* TIMERS_H */
