/* pgmwin.h */


#ifndef PGMWIN_H
#define PGMWIN_H 1


#include <gtk/gtk.h>
#include "gtksizes.h"


#define MAXBP 32


GtkWidget *
pgmwin_init( int width, int height );

void
pgmwin_Disasm( void );

int
pgmwin_IsBreakpoint( unsigned int Address );


#endif /* PGMWIN_H */
