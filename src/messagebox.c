/*
 * messagebox.c
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

#if HAVE_CONFIG_H
#  include "config.h"
#endif

#include <gtk/gtk.h>

#include "common.h"
#include "messagebox.h"


#define MESSAGE_DIALOG_BORDER 25

#define BUTTON_TEXT_BORDER 3


/* This function is used to adjust the border around the text in a button. */
static GtkWidget *
AddTextButton( gchar *button_text )
{
  GtkWidget *button;
  GtkWidget *label;
  GtkWidget *label_window;

  /* The GtkLabel widget is one of a few GTK+ widgets that don't create their own window to
     render themselves into. Instead, they draw themselves directly onto their parents
     window. This means that in order to set a property for a GtkLabel widget, you need to
     change the property of its parent, i.e. the object that you pack it into.
     Another solution (short term workaround) is to put the label widget inside another
     widget that does get its own window, like the 'ViewPort' or 'EventBox' widget. */

  /* Using workaround described above to set the border width of 'label' widget. */
  label_window = gtk_event_box_new();

  /* Creating our label. */
  label = gtk_label_new(button_text);

  /* Adding label widget to label_window widget. */
  gtk_container_add( GTK_CONTAINER(label_window), label );

  /* Changing border width of the label widget by way of label_window widget. */
  gtk_container_set_border_width( GTK_CONTAINER(label_window), BUTTON_TEXT_BORDER );
  
  /* Create the button. */
  button = gtk_button_new();

  /* Adding label to button. */
  gtk_container_add( GTK_CONTAINER(button), label_window );
  
  return button;
}


void
ShowMessage( gchar *title, gchar *message, int justification, int font_style )
{
  GtkWidget *dialog;
  GtkWidget *label;
  GtkWidget *okay_button;
  GtkWidget *label_window;
  GtkWidget *center;
  
  /* Set-up a dialog window, centered on the screen. */
  dialog = gtk_dialog_new();
  gtk_window_set_title( GTK_WINDOW(dialog), title );
  gtk_window_set_position( GTK_WINDOW(dialog), GTK_WIN_POS_CENTER );
  
  /* To have the main window of our application being unusable while using the dialog. */
  gtk_window_set_modal( GTK_WINDOW(dialog), TRUE );
  
  /* The GtkLabel widget is one of a few GTK+ widgets that don't create their own window to
     render themselves into. Instead, they draw themselves directly onto their parents
     window. This means that in order to set a property for a GtkLabel widget, you need to
     change the property of its parent, i.e. the object that you pack it into.
     Another solution (short term workaround) is to put the label widget inside another
     widget that does get its own window, like the 'ViewPort' or 'EventBox' widget. */

  /* Using workaround described above to set the border width of 'label' widget. */
  label_window = gtk_event_box_new();

  /* Creating our label. */
  label = gtk_label_new(message);
  gtk_label_set_justify( GTK_LABEL(label), justification );

  if( font_style == MESSAGE_DIALOG_FIXED_FONT ) {
#if ( GTK_MAJOR_VERSION == 2)
    PangoFontDescription *pango_font;

    pango_font = pango_font_description_from_string( FIXED_FONT );

    gtk_widget_modify_font( label, pango_font );
#else
    GtkStyle *style;
    
    /* Setting font for the label. */
    style = gtk_style_new();
    gdk_font_unref( style->font );
    
    /* Load a fixed font */
    style->font = gdk_font_load( FIXED_FONT );
    gtk_widget_set_style( label, style );
#endif
  }
    
  /* Adding label widget to label_window widget. */
  gtk_container_add( GTK_CONTAINER(label_window), label );

  /* Changing border width of the label widget by way of label_window widget. */
  gtk_container_set_border_width( GTK_CONTAINER(label_window), MESSAGE_DIALOG_BORDER );

  /* xalign, yalign, xscale, yscale */
  center = gtk_alignment_new( 0.5, 0.5, 0.0, 0.0 );

  /* Create the OK button. */
  okay_button = AddTextButton( "OK" );
  
  /* Ensure that the dialog box is destroyed when the user clicks ok. */
  gtk_signal_connect_object( GTK_OBJECT(okay_button), "clicked",
			     GTK_SIGNAL_FUNC(gtk_widget_destroy), (gpointer) dialog );
  
  /* Add the OK button to the alignment widget. */
  gtk_container_add( GTK_CONTAINER(center), okay_button );
  /* Add the alignment widget to the dialog window. */
  gtk_container_add( GTK_CONTAINER( GTK_DIALOG(dialog)->action_area ), center );

  /* Add the label_window to the dialog window. */
  gtk_container_add( GTK_CONTAINER( GTK_DIALOG(dialog)->vbox ), label_window );
  
  /* Show everything we've added to the dialog. */
  gtk_widget_show_all(dialog);
}
