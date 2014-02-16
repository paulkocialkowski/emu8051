/*
 * regwin.c
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

#include "common.h"
#include "reg8051.h"
#include "cpu8051.h"
#include "sfr.h"
#include "memory.h"
#include "regwin.h"
#include "memwin.h"
#include "pgmwin.h"
#include "instructions_8051.h"
#include "hexfile.h"
#include "main.h"

static GtkWidget *reglist;

#define LIST_VIEW_NAME "Registers"
#define DATA_ROWS SFR_REGS

enum {
	COL_NAME = 0,
	COL_VAL,
	N_COLUMNS,
};

/* Creating a model */
static GtkListStore *
regwin_init_store(void)
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
regwin_cell_edited(GtkCellRendererText *cell, gchar *path_string,
		   gchar *new_str, gpointer model)
{
	GtkTreeIter iter;
	int old;
	int new;
	char *str;
	int rc;
	struct regwin_infos_t *regwin_infos;

	(void) cell; /* Remove compiler warning about unused variables. */

	if (!model)
		log_err("Unable to get model from cell renderer");

	/* Get the iterator */
	gtk_tree_model_get_iter_from_string(model, &iter, path_string);

	/* Get register name. */
	gtk_tree_model_get(model, &iter, COL_NAME, &str, -1);

	log_info("Register: %s", str);
	regwin_infos = sfr_get_infos(str);
	log_info("  width:     %d bits", 4 * regwin_infos->w);

	/* Read current (old) value. */
	gtk_tree_model_get(model, &iter, COL_VAL, &str, -1);

	/* No need to check error, has already been validated. */
	old = asciihex2int(str);
	log_info("  old value: $%04X", old);

	new = asciihex2int(new_str);
	if (asciihex2int_get_error()) {
		log_warn("  new value: invalid");
		return;
	}

	log_info("  new value: $%04X", new);

	/* Store new value in emulator register (if in range). */
	rc = regwin_write(regwin_infos, new);
	if (rc == 0) {
		/* Store new value in gtk model. */
		int2asciihex(new, str, regwin_infos->w);
		gtk_list_store_set(GTK_LIST_STORE(model), &iter, COL_VAL, str,
				   -1);

		/*
		 * Make sure to update all windows.
		 * For example, R0-R7 values depends on internal memory values.
		 */
		emugtk_update_display();
	}
};

static void
regwin_init_columns(void)
{
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;

	/* Columns and cell renderers */
	renderer = gtk_cell_renderer_text_new();

	/* Add Register column */
	column = gtk_tree_view_column_new_with_attributes(
		"Name", renderer, "text", COL_NAME, NULL);
	gtk_tree_view_column_set_sizing(column, GTK_TREE_VIEW_COLUMN_AUTOSIZE);
	gtk_tree_view_append_column(GTK_TREE_VIEW(reglist), column);

	/* Add Value column */

	/* Create new renderer for value column (editable). */
	renderer = gtk_cell_renderer_text_new();

	/* Allow edition, align to right side. */
	g_object_set(renderer, "editable", TRUE, "xalign", 1.0, NULL);

	g_signal_connect(renderer, "edited",
			 G_CALLBACK(regwin_cell_edited),
			 gtk_tree_view_get_model(GTK_TREE_VIEW(reglist)));

	column = gtk_tree_view_column_new_with_attributes(
		"Value", renderer, "text", COL_VAL, NULL);
	gtk_tree_view_column_set_sizing(column, GTK_TREE_VIEW_COLUMN_AUTOSIZE);
	gtk_tree_view_append_column(GTK_TREE_VIEW(reglist), column);
}

GtkWidget *
regwin_init(void)
{
	GtkWidget *frame;
	GtkWidget *scrollwin;
	GtkListStore *store;

	frame = gtk_frame_new(LIST_VIEW_NAME);

	scrollwin = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrollwin),
					    GTK_SHADOW_ETCHED_OUT);

	/* Automatically add scrollbars when necessary. */
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollwin),
				       GTK_POLICY_AUTOMATIC,
				       GTK_POLICY_AUTOMATIC);

	gtk_container_add(GTK_CONTAINER(frame), scrollwin);

	/* Creating a model */
	store = regwin_init_store();

	/* Creating the view component */
	reglist = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(reglist), TRUE);
	gtk_container_add(GTK_CONTAINER(scrollwin), reglist);

	regwin_init_columns();

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
regwin_refresh(void)
{
	int row;
	GtkListStore *store;

	store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(reglist)));

	for (row = 0; row < DATA_ROWS; row++) {
		int valid;
		GtkTreeIter iter;
		int val;
		char str[8];
		struct regwin_infos_t *regwin_infos;

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
			log_err("Tree model: invalid iter");
			return;
		}

		regwin_infos = sfr_get_infos_from_row(row);

		val = regwin_read(row);

		/* Convert to specified number of hex digits. */
		int2asciihex(val, str, regwin_infos->w);

		gtk_list_store_set(store, &iter,
				   COL_NAME, regwin_infos->name,
				   COL_VAL, str,
				   -1);
	}
}
