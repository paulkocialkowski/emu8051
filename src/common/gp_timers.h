/*
 * gp_timers.h
 *
 * Copyright (C) 2013 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This file is released under the GPLv2
 */

#ifndef GP_TIMERS_H
#define GP_TIMERS_H 1

#include <stdint.h>

/* Maximum of 4 for CLI version */
#define GP_TIMERS_COUNT 2

int
gp_timer_read(int id);

void
gp_timer_reset(int id);

void
gp_timers_increment(int count);

void
gp_timers_reset(void);

#endif /* GP_TIMERS_H */
