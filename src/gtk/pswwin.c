/*
 * pswwin.c
 *
 * Copyright (C) 1999 Jonathan St-André
 * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This file is released under the GPLv2
 */

#if HAVE_CONFIG_H
#  include "config.h"
#endif

#include <stdio.h>
#include <stdint.h>

#include "common.h"
#include "reg8051.h"
#include "cpu8051.h"
#include "memory.h"
#include "pswwin.h"
#include "memwin.h"
#include "pgmwin.h"
#include "psw.h"
#include "instructions_8051.h"
#include "hexfile.h"
#include "main.h"

static GtkWidget *pswlist;

#define DATA_ROWS 1
#define LIST_VIEW_NAME "PSW"

enum {
	COL_CY = 0,
	COL_AC,
	COL_F0,
	COL_RS1,
	COL_RS0,
	COL_OV,
	COL_RESERVED,
	COL_P,
	N_COLUMNS,
};

static char *col_names[N_COLUMNS] = {
	"CY",
	"AC",
	"F0",
	"RS1",
	"RS0",
	"OV",
	"-",
	"P",
};

/* Creating a model */
static GtkListStore *
pswwin_init_store(void)
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
	for (row = 0; row < DATA_ROWS; row++)
		gtk_list_store_append(store, &iter);

	return store;
}

static void
pswwin_cell_edited(GtkCellRendererText *cell, gchar *path_string,
		   gchar *new_str, gpointer model)
{
	guint column;
	gpointer columnptr;
	GtkTreeIter iter;
	uint8_t old;
	int new;
	char str[10];
	int bit_index;

	if (!model)
		log_err("Unable to get model from cell renderer");

	/* Column number is passed as renderer object data */
	columnptr = g_object_get_data(G_OBJECT(cell), "column");
	column = GPOINTER_TO_UINT(columnptr);

	log_info("column = $%02X", column);

	/* Get the iterator */
	gtk_tree_model_get_iter_from_string(model, &iter, path_string);

	bit_index = 7 - column;

	old = psw_read_bit(bit_index);

	log_info("  old value: %d", old);

	/* Convert new value (asciihex) to integer. */
	new = asciihex2int(new_str);

	if (asciihex2int_get_error()) {
		log_warn("  new value: invalid");
		return;
	} else if ((new != 0) && (new != 1)) {
		log_warn("  new value: out of range");
		return;
	} else {
		log_info("  new value: %d", new);
	}

	/* Store new value in emulator memory. */
	psw_write_bit(bit_index, new);

	/* Convert to text. */
	int2asciihex(new, str, 1);

	/* Store new value in gtk model. */
	gtk_list_store_set(GTK_LIST_STORE(model), &iter, column, str, -1);

	/*
	 * Make sure to update all registers and memory.
	 * For example, BANKed registers depends on internal memory.
	 */
	emugtk_update_display();
};

static void
pswwin_init_columns(void)
{
	int k;

	/* Add column for each bit in PSW. */
	for (k = 0; k < N_COLUMNS; k++) {
		GtkCellRenderer *renderer;
		GtkTreeViewColumn *column;

		/* Create new renderer for value column (editable). */
		renderer = gtk_cell_renderer_text_new();

		/* Allow edition, align to center. */
		g_object_set(renderer, "editable", TRUE, "xalign", 0.5, NULL);

		g_signal_connect(renderer, "edited",
				 G_CALLBACK(pswwin_cell_edited),
				 gtk_tree_view_get_model(
					 GTK_TREE_VIEW(pswlist)));

		/* Add column index, used when editing the cell. */
		g_object_set_data(G_OBJECT(renderer), "column",
				  GUINT_TO_POINTER(k));

		column = gtk_tree_view_column_new_with_attributes(
			col_names[k], renderer, "text", k, NULL);

		/* Center column name */
		g_object_set(column, "alignment", 0.5, NULL);

		/* Hardcoded width... */
		gtk_tree_view_column_set_sizing(column,
						GTK_TREE_VIEW_COLUMN_FIXED);
		gtk_tree_view_column_set_fixed_width(column, 35);

		gtk_tree_view_append_column(GTK_TREE_VIEW(pswlist), column);
	}
}

GtkWidget *
pswwin_init(void)
{
	GtkWidget *frame;
	GtkListStore *store;

	frame = gtk_frame_new(LIST_VIEW_NAME);

	/* Creating a model */
	store = pswwin_init_store();

	/* Creating the view component */
	pswlist = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));

	gtk_tree_view_set_grid_lines(GTK_TREE_VIEW(pswlist),
				     GTK_TREE_VIEW_GRID_LINES_BOTH);

	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(pswlist), TRUE);
	gtk_container_add(GTK_CONTAINER(frame), pswlist);

	pswwin_init_columns();

	/*
	 * The tree view has acquired its own reference to the model, so we can
	 * drop ours. That way the model will be freed automatically when the
	 * tree view is destroyed.
	 */
	g_object_unref(store);

	return frame;
}

/* Show registers. */
void
pswwin_refresh(void)
{
	GtkListStore *store;
	int valid;
	GtkTreeIter iter;
	int k;

	store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(pswlist)));

	/* Get first row in list store */
	valid = gtk_tree_model_get_iter_first(
		GTK_TREE_MODEL(store), &iter);

	if (!valid) {
		log_err("Tree model: invalid iter");
		return;
	}

	/* Display bit values. */
	for (k = 0; k < N_COLUMNS; k++) {
		char str[4];
		int bit_index;

		bit_index = 7 - k;

		int2asciihex(psw_read_bit(bit_index), str, 1);
		gtk_list_store_set(store, &iter, k, str, -1);
	}
}
