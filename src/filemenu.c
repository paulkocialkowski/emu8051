/*
 * filemenu.c
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

#include <unistd.h>  /* UNIX standard function definitions */
#include <pwd.h>
#include <gtk/gtk.h>

#include "common.h"
#include "emugtk.h"
#include "messagebox.h"
#include "filemenu.h"

#define FILENAME_DESCRIPTION "Open Intel Hex file"

static void
FileOpenDialog_OK(GtkWidget *widget, gpointer file_dialog)
{
	char *selected_file;

#if defined(DEBUG)
	g_print("FileOpenDialog_OK()\n");
#endif

	/*
	 * The cast to (char *) is to remove a warning in GTK2, because the
	 * return value of the gtk_file_selection_get_filename() function is
	 * 'G_CONST_RETURN gchar *'.
	 */
	selected_file = (char *) gtk_file_selection_get_filename(
		GTK_FILE_SELECTION(file_dialog));

	g_print("emugtk_File = %s\n", selected_file);

	emugtk_new_file(selected_file);

	gtk_widget_destroy(GTK_WIDGET(file_dialog));
}

void
FileOpenEvent(GtkObject *object, gpointer data)
{
	GtkWidget *file_dialog;

#if defined(DEBUG)
	g_print("FileOpenEvent()\n");
#endif

	/* Create a new file selection widget. */
	file_dialog = gtk_file_selection_new(FILENAME_DESCRIPTION);

	/* Connect the file dialog's OK button up to a handler. */
	gtk_signal_connect(
		GTK_OBJECT(GTK_FILE_SELECTION(file_dialog)->ok_button),
		"clicked", GTK_SIGNAL_FUNC(FileOpenDialog_OK), file_dialog);

	/*
	 * Ensure that the file selection dialog box is destroyed when the user
	 * clicks CANCEL.
	 */
	gtk_signal_connect_object(
		GTK_OBJECT(GTK_FILE_SELECTION(file_dialog)->cancel_button),
		"clicked", GTK_SIGNAL_FUNC(gtk_widget_destroy),
		(gpointer) file_dialog);

	/* Show the dialog. */
	gtk_widget_show(GTK_WIDGET(file_dialog));

	/*
	 * To have the main window of our application being unusable while
	 * using the dialog.
	 */
	gtk_window_set_modal(GTK_WINDOW(file_dialog), TRUE);
}

static void
FileQuitEvent(gchar *string)
{
#if defined(DEBUG)
	g_print("%s\n", string);
#endif

	gtk_main_quit();
}

void
FileAddMenu(GtkWidget *menu_bar)
{
	GtkWidget *item;
	GtkWidget *menu;

	menu = gtk_menu_new();

	/* Create the 'open' item. */
	item = gtk_menu_item_new_with_label(FILENAME_DESCRIPTION);
	gtk_menu_append(GTK_MENU(menu), item);
	/* Attach the callback functions to the activate signal. */
	gtk_signal_connect_object(GTK_OBJECT(item), "activate",
				  GTK_SIGNAL_FUNC(FileOpenEvent), NULL);

	AddMenuSeparator(menu);

	item = gtk_menu_item_new_with_label("Exit");
	gtk_menu_append(GTK_MENU(menu), item);
	/* We can attach the Quit menu item to our exit function */
	gtk_signal_connect_object(GTK_OBJECT(item), "activate",
				  GTK_SIGNAL_FUNC(FileQuitEvent),
				  (gpointer) "file.quit");

	/* Adding submenu title. */
	item = gtk_menu_item_new_with_label("File");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(item), menu);
	gtk_menu_bar_append(GTK_MENU_BAR(menu_bar), item);
}
