/*
 * common.h
 *
 * Copyright (C) 1999 Jonathan St-André
 * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This file is released under the GPLv2
 */

#ifndef COMMON_H
#define COMMON_H 1

#if HAVE_CONFIG_H
#  include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#if STDC_HEADERS
#  include <string.h>
#elif HAVE_STRINGS_H
#  include <strings.h>
#endif

#include "log.h"

#define FIXED_FONT "monospace 12"

#define MAX_FILENAME_LENGTH 1024

/* Common constants. */
#ifndef EXIT_SUCCESS
#  define EXIT_SUCCESS 0
#  define EXIT_FAILURE 1
#endif

/* Returns TRUE if the strings 'a' and 'b' are equal. */
#define STREQ(a, b) (strcasecmp((a), (b)) == 0)

/* Returns TRUE if the first 'c' characters of strings 'a' and 'b' are equal. */
#define STREQ_LEN(a, b, c) (strncasecmp((a), (b), (c)) == 0)

void *
xmalloc(size_t size, const char *filename, int line_number);

#endif /* COMMON_H */
