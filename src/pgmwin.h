/* pgmwin.h */


#ifndef PGMWIN_H
#define PGMWIN_H 1


#include <gtk/gtk.h>
#include "gtksizes.h"


#define MAXBP 32


void
pgmwin_init( GtkWidget *parentwin );

void
pgmwin_Disasm( void );

gint
pgmwin_ButtonPressEvent( GtkWidget *widget, GdkEvent *event, gpointer data );

void
pgmwin_ShowBreakpoints( void );

void
pgmwin_SetBreakpoint( unsigned int Address );

void
pgmwin_ClearBreakpoint( unsigned int Address );

int
pgmwin_IsBreakpoint( unsigned int Address );

void
pgmwin_ToggleBreakpoint( unsigned int Address );

void
pgmwin_PgmWinButtonPress( GtkWidget *widget, GdkEvent *event, gpointer data );


#endif /* PGMWIN_H */
