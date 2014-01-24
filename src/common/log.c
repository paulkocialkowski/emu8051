/*
 * log.c -- debug functions for logging.
 *
 * Copyright (C) 2011 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This file is released under the GPLv2
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "common.h"
#include "options.h"
#include "log.h"

#define PREFIX_PACKAGE_NAME 1
#define ADD_LINEFEED        1

extern struct options_t options;

static void
log_prefix_package_name(FILE *stream, const char *severity)
{
#if PREFIX_PACKAGE_NAME
	/* Printing the name of the program first if desired. */
	fprintf(stream, "%s %s: ", PACKAGE_NAME, severity);
#endif /* PREFIX_PACKAGE_NAME */
}

static void
log_suffix_newline(FILE *stream)
{
#if ADD_LINEFEED
	fprintf(stream, "\n");
#endif /* ADD_LINEFEED */
}

void
log_debug(const char *format, ...)
{
	FILE *stream = stdout;

	if (options.log >= LOG_LEVEL_DEBUG) {
		va_list ap;

		log_prefix_package_name(stream, "debug");

		va_start(ap, format);
		(void) vfprintf(stream, format, ap);
		va_end(ap);

		log_suffix_newline(stream);
	}
}

void
log_info(const char *format, ...)
{
	FILE *stream = stdout;

	if (options.log >= LOG_LEVEL_INFO) {
		va_list ap;

		log_prefix_package_name(stream, "info");

		va_start(ap, format);
		(void) vfprintf(stream, format, ap);
		va_end(ap);

		log_suffix_newline(stream);
	}
}

void
log_warn(const char *format, ...)
{
	FILE *stream = stderr;

	if (options.log >= LOG_LEVEL_WARN) {
		va_list ap;

		log_prefix_package_name(stream, "warn");

		va_start(ap, format);
		(void) vfprintf(stream, format, ap);
		va_end(ap);

		log_suffix_newline(stream);
	}
}

void
log_err(const char *format, ...)
{
	FILE *stream = stderr;
	va_list ap;

	log_prefix_package_name(stream, "error");

	va_start(ap, format);
	(void) vfprintf(stream, format, ap);
	va_end(ap);

	log_suffix_newline(stream);
}

/* Log error message and exits with error code. */
void
log_fail(const char *format, ...)
{
	FILE *stream = stderr;
	va_list ap;

	log_prefix_package_name(stream, "error");

	va_start(ap, format);
	(void) vfprintf(stream, format, ap);
	va_end(ap);

	log_suffix_newline(stream);

	exit(EXIT_FAILURE);
}
