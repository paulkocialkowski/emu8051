/*
 * log.h
 *
 * Copyright (C) 2011 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef LOG_H
#define LOG_H 1

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

#include "common.h"

enum LOG_LEVEL {
	LOG_LEVEL_ERR,
	LOG_LEVEL_WARN,
	LOG_LEVEL_INFO,
	LOG_LEVEL_DEBUG
};

void
log_set_level(int new_log_level);

void
log_debug(const char *format, ...);

void
log_info(const char *format, ...);

void
log_warn(const char *format, ...);

void
log_fail(const char *format, ...);

void
log_fail_no_exit(const char *format, ...);

#endif /* LOG_H */
