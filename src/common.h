/* common.h */

#ifndef COMMON_H
#define COMMON_H 1


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#if STDC_HEADERS
#  include <string.h>
#elif HAVE_STRINGS_H
#  include <strings.h>
#endif


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
