/* memwin.h */


#ifndef MEMWIN_H
#define MEMWIN_H 1


#include <gtk/gtk.h>


void
memwin_init( GtkWidget *parentwin );

void
memwin_DumpD( unsigned int Address );

void
memwin_DumpI( unsigned int Address );


#endif /* MEMWIN_H */
