/* filemenu.h */


#ifndef FILEMENU_H
#define FILEMENU_H 1


#include <gtk/gtk.h>


void
FileOpenEvent( GtkObject *object, gpointer data );

void
FileResetEvent( GtkObject *object, gpointer data );

void
FileAddMenu( GtkWidget *menu_bar );


#endif /* FILEMENU_H */
