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
#include <ctype.h> /* For isprint */

#include "common.h"
#include "memory.h"
#include "hexfile.h"
#include "cpu8051.h"
#include "regwin.h"
#include "memwin.h"
#include "emugtk.h"

#define DATA_COLS 16 /* Must be a power of 2 */
#define DATA_ROWS_INT (INT_MEM_SIZE / DATA_COLS)
#define DATA_ROWS_EXT (1024 / DATA_COLS)

enum
{
	COL_ADDRESS = 0,
	COL_DATA0,
	COL_ASCII = DATA_COLS + 1,
	N_COLUMNS,
};

static GtkWidget *memlist_internal;
static GtkWidget *memlist_external;

/* Creating a model */
static GtkListStore *
memwin_init_store(int data_rows)
{
	GtkTreeIter iter;
	int row;
	int col;
	GtkListStore *store;
	GType col_types[N_COLUMNS];

	/* No need for static array, all our columns are of the same type. */
	for (col = 0; col < N_COLUMNS; col++)
		col_types[col] = G_TYPE_STRING;

	store = gtk_list_store_newv(N_COLUMNS, col_types);

	/* Add rows. */
	for (row = 0; row < data_rows; row++)
		gtk_list_store_append(store, &iter);

	return store;
}

static void
memwin_cell_edited(GtkCellRendererText *cell, gchar *path_string,
		   gchar *new_str, gpointer model)
{
	guint column;
	guint memory_id;
	gpointer columnptr;
	gpointer memory_id_ptr;
	GtkTreeIter iter;
	int address;
	u_int8_t old;
	int new;
	char *str;

	if (!model) {
		g_error("Unable to get model from cell renderer");
	}

	/* Column number is passed as renderer object data */
        columnptr = g_object_get_data(G_OBJECT(cell), "column");
        column = GPOINTER_TO_UINT(columnptr);

	memory_id_ptr = g_object_get_data(G_OBJECT(cell), "memory_id");
	memory_id = GPOINTER_TO_UINT(memory_id_ptr);

	/* Get the iterator */
        gtk_tree_model_get_iter_from_string(model, &iter, path_string);

	/* Get base address. */
	gtk_tree_model_get(model, &iter, COL_ADDRESS, &str, -1);
	address = asciihex2int(str);

	/* Convert column number (1, 2, 3...) to index (0, 1, 2...) */
	address += (column - COL_DATA0);
	old = memory_read8(memory_id, address);

	log_info("Address: $%02X", address);
	log_info("  old value: $%02X", old);

	/* Convert new value (asciihex) to integer. */
	new = asciihex2int(new_str);
	if ((new < 0) || (new > 255)) {
		log_info("  new value: out of range");
		new = old; /* Put back old value... */
	} else {
		log_info("  new value: $%02X", new);
	}

	/* Store new value in emulator memory. */
	memory_write8(memory_id, address, new);

	/* Convert to text. */
	int2asciihex(new, str, 2);

	/* Store new value in gtk model. */
        gtk_list_store_set(GTK_LIST_STORE(model), &iter, column, str, -1);

	/*
	 * Make sure to update all registers and memory.
	 * For example, BANKed registers depends on internal memory.
	 */
	emugtk_UpdateDisplay();
};

static void
memwin_init_columns(GtkWidget *listview, int memory_id)
{
	int i;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;

	/* Columns and cell renderers */
	renderer = gtk_cell_renderer_text_new();

	/* Add address column */
	column = gtk_tree_view_column_new_with_attributes(
		"Address", renderer, "text", COL_ADDRESS, NULL);
	gtk_tree_view_column_set_sizing(column, GTK_TREE_VIEW_COLUMN_AUTOSIZE);
	gtk_tree_view_append_column(GTK_TREE_VIEW(listview), column);

	for (i = COL_DATA0; i < (COL_DATA0 + DATA_COLS); i++) {
		char col_name[8];

		/* Create new renderer for each editable cell. */
		renderer = gtk_cell_renderer_text_new();

		/* Allow edition, align to right side. */
		g_object_set(renderer, "editable", TRUE, "xalign", 1.0, NULL);

		g_signal_connect(renderer, "edited",
				 G_CALLBACK(memwin_cell_edited),
				 gtk_tree_view_get_model(
					 GTK_TREE_VIEW(listview)));

		/* Add column index, used when editing the cell. */
		g_object_set_data(G_OBJECT(renderer), "column",
				  GUINT_TO_POINTER(i));
		g_object_set_data(G_OBJECT(renderer), "memory_id",
				  GUINT_TO_POINTER(memory_id));

		sprintf(col_name, "B%02d", i - COL_DATA0);

		column = gtk_tree_view_column_new_with_attributes(
			col_name, renderer, "text", i, NULL);
		gtk_tree_view_column_set_sizing(column,
						GTK_TREE_VIEW_COLUMN_AUTOSIZE);
		gtk_tree_view_append_column(GTK_TREE_VIEW(listview), column);
	}

	/* Add ASCII column */
	column = gtk_tree_view_column_new_with_attributes(
		"ASCII", renderer, "text", COL_ASCII, NULL);
	gtk_tree_view_column_set_sizing(column, GTK_TREE_VIEW_COLUMN_AUTOSIZE);
	gtk_tree_view_append_column(GTK_TREE_VIEW(listview), column);
}

GtkWidget *
memwin_init(char *title, int memory_id)
{
	GtkWidget *frame;
	GtkWidget *scrollwin;
	GtkListStore *store;
	GtkWidget *memlist;
	int data_rows;

	frame = gtk_frame_new(title);

	scrollwin = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrollwin),
					    GTK_SHADOW_ETCHED_OUT);

	/* Automatically add scrollbars when necessary. */
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollwin),
				       GTK_POLICY_AUTOMATIC,
				       GTK_POLICY_AUTOMATIC);

	gtk_container_add(GTK_CONTAINER(frame), scrollwin);

	if (memory_id == INT_MEM_ID) {
		data_rows = DATA_ROWS_INT;
	} else if (memory_id == EXT_MEM_ID) {
		data_rows = DATA_ROWS_EXT;
	} else {
		log_fail("Invalid memory type");
		exit(1);
	}

	/* Creating a model */
	store = memwin_init_store(data_rows);

	/* Creating the view component */
	memlist = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(memlist), TRUE);
	gtk_container_add(GTK_CONTAINER(scrollwin), memlist);

	if (memory_id == INT_MEM_ID) {
		memlist_internal = memlist;
	} else if (memory_id == EXT_MEM_ID) {
		memlist_external = memlist;
	} else {
		log_fail("Invalid memory type");
		exit(1);
	}

	memwin_init_columns(memlist, memory_id);

	/*
	 * The tree view has acquired its own reference to the model, so we can
	 * drop ours. That way the model will be freed automatically when the
	 * tree view is destroyed.
	 */
	g_object_unref(store);

	return frame;
}

/* Dump internal or external memory. */
void
memwin_refresh(int memory_id)
{
	int row;
	unsigned int Address;
	GtkListStore *store;
	GtkWidget *memlist;
	int data_rows;

	Address = 0;

	if (memory_id == INT_MEM_ID) {
		memlist = memlist_internal;
		data_rows = DATA_ROWS_INT;
	} else if (memory_id == EXT_MEM_ID) {
		memlist = memlist_external;
		data_rows = DATA_ROWS_EXT;
	} else {
		log_fail("Invalid memory type");
		exit(1);
	}

	store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(memlist)));

	for (row = 0; row < data_rows; row++) {
		int valid;
		GtkTreeIter iter;
		char str[4+1]; /* Maximum str len is for address column (4 digits) */
		char ascii_str[DATA_COLS+1];
		int col;

		if (row == 0) {
			/* Get first row in list store */
			valid = gtk_tree_model_get_iter_first(
				GTK_TREE_MODEL(store), &iter);
		} else {
			/* Get next row in list store */
			valid = gtk_tree_model_iter_next(
				GTK_TREE_MODEL(store), &iter);
		}

		if (!valid) {
			printf("Invalid iter...\n");
			return;
		}

		/* Display base address. */
		int2asciihex(Address, str, 4);

		gtk_list_store_set(store, &iter, COL_ADDRESS, str, -1);

		for (col = 0; col < DATA_COLS; col++) {
			u_int8_t data;

			data = memory_read8(memory_id, Address + col);

			/* Display hex data */
			int2asciihex(data, str, 2);

			gtk_list_store_set(store, &iter, col + 1, str, -1);

			/* Append to ASCII string (if applicable). */
			if (!isprint(data))
				data = '.';
			sprintf(&ascii_str[col], "%c", data);
		}

		/* Display ASCII characters. */
		gtk_list_store_set(store, &iter, COL_ASCII, ascii_str, -1);

		Address += DATA_COLS;
	}
}
