/*
 * pgmwin.c
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
#include "cpu8051.h"
#include "pgmwin.h"

static GtkWidget *pgmlist;

#define DATA_ROWS 24

enum
{
	COL_BREAKPT = 0,
	COL_ADDR,
	COL_B0,
	COL_B1,
	COL_B2,
	COL_INST,
	COL_ARGS,
	N_COLUMNS,
};

char *col_names[N_COLUMNS] = {
	"BPT",
	"Address",
	"B0",
	"B1",
	"B2",
	"Mnemonic",
	"Arguments",
};

/* Creating a model */
static GtkListStore *
pgmwin_init_store(void)
{
	GtkTreeIter iter;
	int rows;
	int col;
	GtkListStore *store;
	GType col_types[N_COLUMNS];

	for (col = 0; col < N_COLUMNS; col++)
		col_types[col] = G_TYPE_STRING;

	store = gtk_list_store_newv(N_COLUMNS, col_types);

	/* Initialize with rows of dummy data... */
	for (rows = 0; rows < DATA_ROWS; rows++)
		gtk_list_store_append(store, &iter);

	return store;
}

static void
pgmwin_init_columns(void)
{
	int k;
	GtkCellRenderer *renderer;

	/* Columns and cell renderers */
	renderer = gtk_cell_renderer_text_new();

	/* Add columns */
	for (k = 0; k < N_COLUMNS; k++) {
		GtkTreeViewColumn *col;

		col = gtk_tree_view_column_new_with_attributes(
			col_names[k], renderer, "text", k, NULL);
		gtk_tree_view_column_set_sizing(col,
						GTK_TREE_VIEW_COLUMN_AUTOSIZE);
		gtk_tree_view_append_column(GTK_TREE_VIEW(pgmlist), col);
	}
}

/* Mouse button pressed in the window. */
static gint
pgmwin_sel_changed_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	GtkTreeSelection *selection;
	GtkTreeModel     *model;
	GtkTreeIter       iter;

	log_debug("pgmwin_sel_changed_event()");

	/* This will only work in single or browse selection mode! */
	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(pgmlist));

	if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
		char *str_addr;
		int val;

		gtk_tree_model_get(model, &iter, COL_ADDR, &str_addr, -1);

		/* Convert hex address in ASCII to integer. */
		sscanf(str_addr, "%x", &val);

		log_debug("  row address is: $%04X", val);

		ToggleBreakpoint(val);
		pgmwin_Disasm();

		g_free(str_addr);
	} else {
		log_debug("  no row selected");
	}

	return FALSE;
}

GtkWidget *
pgmwin_init(void)
{
	GtkWidget *frame;
	GtkWidget *scrollwin;
	GtkListStore *store;
	GtkTreeSelection *selection;

	frame = gtk_frame_new("Program");

	scrollwin = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrollwin),
					    GTK_SHADOW_ETCHED_OUT);

	/* Automatically add scrollbars when necessary. */
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollwin),
				       GTK_POLICY_AUTOMATIC,
				       GTK_POLICY_AUTOMATIC);

	gtk_container_add(GTK_CONTAINER(frame), scrollwin);

	/* Creating a model */
	store = pgmwin_init_store();

	/* Creating the view component */
	pgmlist = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(pgmlist), TRUE);
	gtk_container_add(GTK_CONTAINER(scrollwin), pgmlist);

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(pgmlist));

	/* Only one row can be selected at a time. */
	gtk_tree_selection_set_mode(selection, GTK_SELECTION_BROWSE);

	g_signal_connect(selection, "changed",
		G_CALLBACK(pgmwin_sel_changed_event), NULL);

	pgmwin_init_columns();

	/*
	 * The tree view has acquired its own reference to the model, so we can
	 * drop ours. That way the model will be freed automatically when the
	 * tree view is destroyed.
	 */
	g_object_unref(store);

	return frame;
}

/* Show disassembled program. */
void
pgmwin_Disasm(void)
{
	int row;
	GtkListStore *store;
	unsigned int Address;

	Address = cpu8051.pc;

	store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(pgmlist)));

	for (row = 0; row < DATA_ROWS; row++) {
		int valid;
		GtkTreeIter iter;
		char str[128];
		int k;
		int col_id;
		int InstSize;
		unsigned char OpCode;

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

		/* Display breakpoints. */
		if (IsBreakpoint(Address))
			sprintf(str, "*");
		else
			str[0] = '\0';

		gtk_list_store_set(store, &iter, COL_BREAKPT, str, -1);

		/* Display base address. */
		sprintf(str, "%.4X", Address);
		gtk_list_store_set(store, &iter, COL_ADDR, str, -1);

		OpCode = memory_read8(PGM_MEM_ID, Address);
		InstSize = cpu8051_get_instruction_size(OpCode);

		/* Display instruction hex bytes. */
		for (k = 0, col_id = COL_B0; k < InstSize; k++, col_id++) {
			sprintf(str, "%.2X", memory_read8(PGM_MEM_ID,
							  Address + k));
			gtk_list_store_set(store, &iter, col_id, str, -1);
		}

		/* Display instruction menmonic. */
		cpu8051_disasm_mnemonic(OpCode, str);
		gtk_list_store_set(store, &iter, COL_INST, str, -1);

		/* Display instruction arguments (if applicable). */
		str[0] = '\0';
		cpu8051_disasm_args(Address, str);
		gtk_list_store_set(store, &iter, COL_ARGS, str, -1);

		Address += InstSize;
	}
}
