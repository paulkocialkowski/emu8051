/*
 * memwin.c
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

#include <stdio.h>

#include "common.h"
#include "memory.h"
#include "hexfile.h"
#include "cpu8051.h"
#include "memwin.h"

static GtkWidget *memlist;

enum
{
	COL_ADDRESS = 0,
	COL_ASCII = 17,
	N_COLUMNS,
};

GtkWidget *
memwin_init(void)
{
	int i;
	int rows;
	GtkWidget *scrollwin;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkListStore *store;
	GtkTreeIter iter;

	scrollwin = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrollwin),
					    GTK_SHADOW_ETCHED_OUT);

	/* Automatically add scrollbars when necessary. */
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollwin),
				       GTK_POLICY_AUTOMATIC,
				       GTK_POLICY_AUTOMATIC);

	/* Creating a model */
	store = gtk_list_store_new(N_COLUMNS,
				   G_TYPE_STRING,
				   G_TYPE_STRING,
				   G_TYPE_STRING,
				   G_TYPE_STRING,
				   G_TYPE_STRING,
				   G_TYPE_STRING,
				   G_TYPE_STRING,
				   G_TYPE_STRING,
				   G_TYPE_STRING,
				   G_TYPE_STRING,
				   G_TYPE_STRING,
				   G_TYPE_STRING,
				   G_TYPE_STRING,
				   G_TYPE_STRING,
				   G_TYPE_STRING,
				   G_TYPE_STRING,
				   G_TYPE_STRING,
				   G_TYPE_STRING);

	/* Creating the view component */
	memlist = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(memlist), TRUE);

	gtk_container_add(GTK_CONTAINER(scrollwin), memlist);

	g_object_unref(store);


	/* Columns and cell renderers */
	renderer = gtk_cell_renderer_text_new();

	/* Add address column */
	column = gtk_tree_view_column_new_with_attributes(
		"Address", renderer, "text", COL_ADDRESS, NULL);
	gtk_tree_view_column_set_sizing(column, GTK_TREE_VIEW_COLUMN_AUTOSIZE);
	gtk_tree_view_append_column(GTK_TREE_VIEW(memlist), column);

	for (i = 1; i < 17; i++) {
		column = gtk_tree_view_column_new_with_attributes(
			"Val", renderer, "text", i, NULL);
		gtk_tree_view_append_column(GTK_TREE_VIEW(memlist), column);
	}

	column = gtk_tree_view_column_new_with_attributes(
		"ASCII", renderer, "text", COL_ASCII, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(memlist), column);

	/* Initialize with 16 rows of dummy data... */
	for (rows = 0; rows < 16; rows++) {
		/* Add new row. */
		gtk_list_store_append(store, &iter);
		gtk_list_store_set(store, &iter, COL_ADDRESS, "0x0000", -1);
	}

	return scrollwin;
}

/* Dump 16 rows of 16 bytes from Address in Memory (direct addressing) */
void
memwin_DumpD(char *MemAddress)
{
	int row;
	unsigned int Address;
	GtkListStore *store;

#ifdef EMU8051_DEBUG
	printf("memwin_DumpD, address = %s\n", MemAddress);
#endif

	if (strlen(MemAddress) != 0) {
		if (STREQ(MemAddress, "PC"))
			Address = cpu8051.pc;
		else
			Address = Ascii2Hex(MemAddress, strlen(MemAddress));
	} else {
		Address = 0;
	}

	store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(memlist)));

	for (row = 0; row < 16; row++) {
		int valid;
		GtkTreeIter iter;
		char TextTmp[1024];
		int column, TextLength;

		if (row == 0) {
			/* Get first row in list store */
			valid = gtk_tree_model_get_iter_first(
				GTK_TREE_MODEL(store), &iter);
		} else {
			/* Get next row in list store */
			valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(store), &iter);
		}

		if (!valid) {
			printf("Invalid iter...\n");
			return;
		}

		sprintf(TextTmp, "%.4X", Address);
		gtk_list_store_set(store, &iter, COL_ADDRESS, TextTmp, -1);

		for (column = 0; column < 16; column++) {
			sprintf(TextTmp, "%.2X",
				(int) cpu8051_ReadD(Address + column));

			gtk_list_store_set(store, &iter, column + 1, TextTmp,
					   -1);
		}

		TextLength = 0;
		for (column = 0; column < 16; column++) {
			if (((int) cpu8051_ReadD(Address + column) >= 32) &&
			    ((int) cpu8051_ReadD(Address + column) <= 126))
				TextLength += sprintf(
					&TextTmp[TextLength],
					"%c", cpu8051_ReadD(Address + column));
			else
				TextLength +=
					sprintf(&TextTmp[TextLength], ".");
		}

		gtk_list_store_set(store, &iter, 17, TextTmp, -1);

		Address += 16;
	}
}
