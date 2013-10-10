/*
 * log.c -- debug functions for logging.
 *
 * Copyright (C) 2011 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "config.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "common.h"
#include "options.h"

static int log_level = LOG_LEVEL_ERR; /* LEVEL = ERROR */

void
log_set_level(int new_log_level)
{
	log_level = new_log_level;
}

void
log_debug(const char *format, ...)
{
	FILE *stream = stdout;

	if (log_level >= LOG_LEVEL_DEBUG) {
		va_list ap;

		/* Printing the name of the program first. */
		fprintf(stream, "%s debug  : ", PACKAGE_NAME);

		va_start(ap, format);
		vfprintf(stream, format, ap);
		va_end(ap);

		fprintf(stream, "\n");
	}
}

void
log_info(const char *format, ...)
{
	FILE *stream = stdout;

	if (log_level >= LOG_LEVEL_INFO) {
		va_list ap;

		/* Printing the name of the program first. */
		fprintf(stream, "%s info   : ", PACKAGE_NAME);

		va_start(ap, format);
		vfprintf(stream, format, ap);
		va_end(ap);

		fprintf(stream, "\n");
	}
}

void
log_warn(const char *format, ...)
{
	FILE *stream = stderr;

	if (log_level >= LOG_LEVEL_WARN) {
		va_list ap;

		/* Printing the name of the program first. */
		fprintf(stream, "%s warning: ", PACKAGE_NAME);

		va_start(ap, format);
		vfprintf(stream, format, ap);
		va_end(ap);

		fprintf(stream, "\n");
	}
}

void
log_fail(const char *format, ...)
{
	FILE *stream = stderr;
	va_list ap;

	/* Printing the name of the program first. */
	fprintf(stream, "%s error  : ", PACKAGE_NAME);

	va_start(ap, format);
	vfprintf(stream, format, ap);
	va_end(ap);

	fprintf(stream, "\n");

	exit(EXIT_FAILURE);
}

void
log_fail_no_exit(const char *format, ...)
{
	FILE *stream = stderr;
	va_list ap;

	/* Printing the name of the program first. */
	fprintf(stream, "%s error: ", PACKAGE_NAME);

	va_start(ap, format);
	vfprintf(stream, format, ap);
	va_end(ap);

	fprintf(stream, "\n");
}
