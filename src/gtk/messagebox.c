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

extern GtkWidget *mainwin;

void
message_show_error(char *message)
{
	GtkWidget *dialog;

	dialog = gtk_message_dialog_new(GTK_WINDOW(mainwin),
					GTK_DIALOG_DESTROY_WITH_PARENT,
					GTK_MESSAGE_ERROR,
					GTK_BUTTONS_CLOSE,
					message, NULL);
	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);
}

void
message_show_information(char *message)
{
	GtkWidget *dialog;

	dialog = gtk_message_dialog_new(GTK_WINDOW(mainwin),
					GTK_DIALOG_DESTROY_WITH_PARENT,
					GTK_MESSAGE_INFO,
					GTK_BUTTONS_CLOSE,
					message, NULL);
	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);
}
