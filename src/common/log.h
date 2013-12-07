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
	LOG_LEVEL_ERR = 0, /* Display only errors */
	LOG_LEVEL_WARN,    /* Display warnings */
	LOG_LEVEL_INFO,    /* Display information messages */
	LOG_LEVEL_DEBUG,   /* Display all messages */
};

void
log_debug(const char *format, ...);

void
log_info(const char *format, ...);

void
log_warn(const char *format, ...);

void
log_err(const char *format, ...);

/* Log error message and exits with error code. */
void
log_fail(const char *format, ...);

#endif /* LOG_H */
