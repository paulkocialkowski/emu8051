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
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#if HAVE_CONFIG_H
#  include "config.h"
#endif

#include <gtk/gtk.h>

#include "common.h"
#include "messagebox.h"

#define MESSAGE_DIALOG_BORDER 25

#define BUTTON_TEXT_BORDER 3

extern GtkWidget *mainwin;

void
ShowMessage(gchar *title, gchar *message, int justification, int font_style)
{
	GtkWidget *dialog;
	GtkWidget *label;
	GtkWidget *label_window;

	/* Keep the dialog on top of the main window, and centered. */
	dialog = gtk_dialog_new_with_buttons(
		title, GTK_WINDOW(mainwin),
		GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_OK,
		GTK_RESPONSE_NONE, NULL);

	/*
	 * The GtkLabel widget is one of a few GTK+ widgets that don't create
	 * their own window to render themselves into. Instead, they draw
	 * themselves directly onto their parents window. This means that in
	 * order to set a property for a GtkLabel widget, you need to change the
	 * property of its parent, i.e. the object that you pack it into.
	 * Another solution (short term workaround) is to put the label widget
	 * inside another widget that does get its own window, like the
	 * 'ViewPort' or 'EventBox' widget.
	 */

	/*
	 * Using workaround described above to set the border width of 'label'
	 * widget.
	 */
	label_window = gtk_event_box_new();

	/* Creating our label. */
	label = gtk_label_new(message);
	gtk_label_set_justify(GTK_LABEL(label), justification);

	if (font_style == MESSAGE_DIALOG_FIXED_FONT) {
		PangoFontDescription *pango_font;

		pango_font = pango_font_description_from_string(FIXED_FONT);
		gtk_widget_modify_font(label, pango_font);
	}

	/* Adding label widget to label_window widget. */
	gtk_container_add(GTK_CONTAINER(label_window), label);

	/*
	 * Changing border width of the label widget by way of label_window
	 * widget.
	 */
	gtk_container_set_border_width(GTK_CONTAINER(label_window),
				       MESSAGE_DIALOG_BORDER);

	/* Ensure that the dialog box is destroyed when the user responds */
	g_signal_connect_swapped(dialog, "response",
				 G_CALLBACK(gtk_widget_destroy), dialog);

	/* Add the label_window to the dialog window. */
	gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox),
			  label_window);

	/* Show everything we've added to the dialog. */
	gtk_widget_show_all(dialog);
}
