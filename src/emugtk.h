/* emugtk.h */


#ifndef EMUGTK_H
#define EMUGTK_H 1


#include <gtk/gtk.h>
#include "gtksizes.h"




void
AddMenuSeparator( GtkWidget *menu );

void
emugtk_new_file( char *file );



void
emugtk_StopRunning( void );

void
emugtk_Reset( void );

void
emugtk_UpdateDisplay( void );




void emugtk_Step( );




void emugtk_ResetEvent( GtkWidget *widget, GdkEvent *event, gpointer data );
void emugtk_RunEvent( GtkWidget *widget, GdkEvent *event, gpointer data );
void emugtk_StopEvent( GtkWidget *widget, GdkEvent *event, gpointer data );
void emugtk_StepEvent( GtkWidget *widget, GdkEvent *event, gpointer data );

void emugtk_StartRunning( );

void emugtk_Running( );





#endif /* EMUGTK_H */
