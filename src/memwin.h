/* memwin.h */


#ifndef MEMWIN_H
#define MEMWIN_H 1


#include <gtk/gtk.h>


GtkWidget *
memwin_init( int width, int height );

void
memwin_DumpD( unsigned int Address );

void
memwin_DumpI( unsigned int Address );


#endif /* MEMWIN_H */
