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

static char previous_folder[MAX_FILENAME_LENGTH + 1];

static void
remember_current_folder(GtkFileChooser *chooser)
{
	char *folder;

	folder = gtk_file_chooser_get_current_folder(chooser);

	if (folder != NULL) {
		if (strlen(folder) >= MAX_FILENAME_LENGTH) {
			/* Non-critical error */
			g_print("current folder name too long for buffer\n");
		} else {
#if defined(DEBUG)
			g_print("current folder = %s\n", folder);
#endif

			strncpy(previous_folder, folder, MAX_FILENAME_LENGTH);
		}

		g_free(folder);
	}
}

void
FileOpenEvent(GtkObject *object, gpointer data)
{
	GtkWidget *file_dialog;
	char *dir;
	char *cwd = NULL;

#if defined(DEBUG)
	g_print("FileOpenEvent()\n");
#endif

	/* Create a new file selection widget. */
	file_dialog = gtk_file_chooser_dialog_new(
		FILENAME_DESCRIPTION, NULL, GTK_FILE_CHOOSER_ACTION_OPEN,
		GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
		GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);

	if (strlen(previous_folder) == 0) {
		/* Opening file chooser to current working directory. */
		cwd = g_get_current_dir();
		dir = cwd;
	} else {
		/* Opening file chooser to previous opened directory. */
		dir = previous_folder;
	}

	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(file_dialog), dir);

	if (cwd)
		g_free(cwd);

	if (gtk_dialog_run(GTK_DIALOG(file_dialog)) == GTK_RESPONSE_ACCEPT) {
		char *selected_file;

		selected_file = gtk_file_chooser_get_filename(
			GTK_FILE_CHOOSER(file_dialog));

		if (selected_file != NULL) {
#if defined(DEBUG)
			g_print("emugtk_File = %s\n", selected_file);
#endif

			remember_current_folder(GTK_FILE_CHOOSER(file_dialog));

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
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	/* Attach the callback functions to the activate signal. */
	gtk_signal_connect_object(GTK_OBJECT(item), "activate",
				  GTK_SIGNAL_FUNC(FileOpenEvent), NULL);

	AddMenuSeparator(menu);

	item = gtk_menu_item_new_with_label("Exit");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	/* We can attach the Quit menu item to our exit function */
	gtk_signal_connect_object(GTK_OBJECT(item), "activate",
				  GTK_SIGNAL_FUNC(FileQuitEvent),
				  (gpointer) "file.quit");

	/* Adding submenu title. */
	item = gtk_menu_item_new_with_label("File");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(item), menu);
	gtk_menu_bar_append(GTK_MENU_BAR(menu_bar), item);
}
