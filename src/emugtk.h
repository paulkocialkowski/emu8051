/* emugtk.h */


#ifndef EMUGTK_H
#define EMUGTK_H 1


#include <gtk/gtk.h>
#include "gtksizes.h"
/*#include "exceptions.hpp"*/


void emugtk_Reset( );
void emugtk_Step( );
//  void Step( );
//  void Exec( );

void emugtk_AddButtons( );
void emugtk_ShowMenu( );

gboolean emugtk_DeleteEvent( GtkWidget *widget, GdkEvent *event, gpointer data );
void emugtk_DestroyEvent( GtkWidget *widget, gpointer data );

void emugtk_OpenEvent( GtkWidget *widget, gpointer data );
void emugtk_QuitEvent( GtkWidget *widget, gpointer data );
void emugtk_AboutEvent( GtkWidget *widget, gpointer data );

void emugtk_ResetEvent( GtkWidget *widget, GdkEvent *event, gpointer data );
void emugtk_RunEvent( GtkWidget *widget, GdkEvent *event, gpointer data );
void emugtk_StopEvent( GtkWidget *widget, GdkEvent *event, gpointer data );
void emugtk_StepEvent( GtkWidget *widget, GdkEvent *event, gpointer data );

void emugtk_StartRunning( );
void emugtk_StopRunning( );
void emugtk_Running( );

void emugtk_UpdateDisplay();








void EmuGtkSignalStub3( GtkWidget *widget, GdkEvent *event, gpointer data );
void EmuGtkSignalStub2( GtkWidget *widget, gpointer data );
void FileOpenDialog_OK( GtkButton *button, gpointer data );
void FileOpenDialog_CANCEL( GtkButton *button, gpointer data );


#endif /* EMUGTK_H */
