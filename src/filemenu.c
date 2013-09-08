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

void
FileOpenEvent(GtkObject *object, gpointer data)
{
	GtkWidget *file_dialog;

#if defined(DEBUG)
	g_print("FileOpenEvent()\n");
#endif

	/* Create a new file selection widget. */
	file_dialog = gtk_file_chooser_dialog_new(
		FILENAME_DESCRIPTION, NULL, GTK_FILE_CHOOSER_ACTION_OPEN,
		GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
		GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);

	if (gtk_dialog_run(GTK_DIALOG(file_dialog)) == GTK_RESPONSE_ACCEPT) {
		char *selected_file;

		selected_file = gtk_file_chooser_get_filename(
			GTK_FILE_CHOOSER(file_dialog));

		if (selected_file != NULL) {
#if defined(DEBUG)
			g_print("emugtk_File = %s\n", selected_file);
#endif

			emugtk_new_file(selected_file);
			g_free(selected_file);
		}
	}

	gtk_widget_destroy(file_dialog);
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
