/*
 * common.h
 *
 * Copyright (C) 1999 Jonathan St-André
 * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef COMMON_H
#define COMMON_H 1

/* Define this to have debugging output. */
/* #define EMU8051_DEBUG 1 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#if STDC_HEADERS
#  include <string.h>
#elif HAVE_STRINGS_H
#  include <strings.h>
#endif

#define FIXED_FONT "monospace 12"

/* Common constants. */
#ifndef EXIT_SUCCESS
#  define EXIT_SUCCESS 0
#  define EXIT_FAILURE 1
#endif

typedef int bool;
#ifndef FALSE
#  define FALSE 0
#  define TRUE  1
#endif


/* Returns TRUE if the strings 'a' and 'b' are equal. */
#define STREQ(a, b) (strcmp((a), (b)) == 0)

/* Returns TRUE if the first 'c' characters of strings 'a' and 'b' are equal. */
#define STREQ_LEN(a, b, c) (strncmp((a), (b), (c)) == 0)


inline void
ErrorLocation( const char *file, int line );

/*@out@*/ /*@only@*/
void *
xmalloc( size_t size, const char *filename, int line_number );


#endif /* COMMON_H */
