/*
 * regwin.cpp
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
#include "reg8051.h"
#include "cpu8051.h"
#include "regwin.h"

static GtkWidget *reglist;

#define DATA_ROWS 24

enum
{
	COL_NAME = 0,
	COL_VAL,
	N_COLUMNS,
};

#define HEX_DIGITS_2 2
#define HEX_DIGITS_4 4

struct regwin_read_t {
	char *name; /* Register name */
	int w; /* Number of hex digits to display */
	unsigned int (*read_func)(int addr); /* Function pointer to read value */
	int param; /* Optional parameter to pass to read function. */
};

/* Generic read function */
static unsigned int
regwin_read_8(int addr)
{
	return cpu8051_ReadD(addr);
}

/* Specific read functions for special registers. */

static unsigned int
regwin_read_pc(int dummy)
{
	return cpu8051.pc;
}

static unsigned int
regwin_read_dptr(int dummy)
{
	return (cpu8051_ReadD(_DPTRHIGH_) << 8) +
		cpu8051_ReadD(_DPTRLOW_);
}

static unsigned int
regwin_read_bank(int dummy)
{
	return cpu8051_ReadD(_PSW_) & 0x18;
}

/* Read R0 - R7 in current bank. */
static unsigned int
regwin_read_rx(int addr)
{
	int rbank = regwin_read_bank(0); /* dummy */

	return cpu8051_ReadD(addr + rbank);
}

/* This array defines how to read value for each register. */
static struct regwin_read_t regwin_read[DATA_ROWS] = {
	{
		"PC",
		HEX_DIGITS_4,
		regwin_read_pc,
		0, /* Dummy */
	},
	{
		"SP",
		HEX_DIGITS_2,
		regwin_read_8,
		_SP_,
	},
	{
		"A",
		HEX_DIGITS_2,
		regwin_read_8,
		_ACC_,
	},
	{
		"B",
		HEX_DIGITS_2,
		regwin_read_8,
		_B_,
	},
	{
		"DPTR",
		HEX_DIGITS_4,
		regwin_read_dptr,
		0, /* Dummy */
	},
	{
		"PSW",
		HEX_DIGITS_2,
		regwin_read_8,
		_PSW_,
	},
	{
		"P0",
		HEX_DIGITS_2,
		regwin_read_8,
		_P0_,
	},
	{
		"P1",
		HEX_DIGITS_2,
		regwin_read_8,
		_P1_,
	},
	{
		"P2",
		HEX_DIGITS_2,
		regwin_read_8,
		_P2_,
	},
	{
		"P3",
		HEX_DIGITS_2,
		regwin_read_8,
		_P3_,
	},
	{
		"TCON",
		HEX_DIGITS_2,
		regwin_read_8,
		_TCON_,
	},
	{
		"TMOD",
		HEX_DIGITS_2,
		regwin_read_8,
		_TMOD_,
	},
	{
		"SCON",
		HEX_DIGITS_2,
		regwin_read_8,
		_SCON_,
	},
	{
		"IE",
		HEX_DIGITS_2,
		regwin_read_8,
		_IE_,
	},
	{
		"IP",
		HEX_DIGITS_2,
		regwin_read_8,
		_IP_,
	},
	{
		"BANK",
		HEX_DIGITS_2,
		regwin_read_bank,
		0, /* Dummy */
	},
	/* R0-R7 Registers in current Bank */
	{
		"R0",
		HEX_DIGITS_2,
		regwin_read_rx,
		_R0_,
	},
	{
		"R1",
		HEX_DIGITS_2,
		regwin_read_rx,
		_R1_,
	},
	{
		"R2",
		HEX_DIGITS_2,
		regwin_read_rx,
		_R2_,
	},
	{
		"R3",
		HEX_DIGITS_2,
		regwin_read_rx,
		_R3_,
	},
	{
		"R4",
		HEX_DIGITS_2,
		regwin_read_rx,
		_R4_,
	},
	{
		"R5",
		HEX_DIGITS_2,
		regwin_read_rx,
		_R5_,
	},
	{
		"R6",
		HEX_DIGITS_2,
		regwin_read_rx,
		_R6_,
	},
	{
		"R7",
		HEX_DIGITS_2,
		regwin_read_rx,
		_R7_,
	},
};

/* Creating a model */
static GtkListStore *
regwin_init_store(void)
{
	GtkTreeIter iter;
	int rows;
	int col;
	GtkListStore *store;
	GType col_types[N_COLUMNS];

	for (col = 0; col < N_COLUMNS; col++) {
		col_types[col] = G_TYPE_STRING;
	}

	store = gtk_list_store_newv(N_COLUMNS, col_types);

	/* Initialize with rows of dummy data... */
	for (rows = 0; rows < DATA_ROWS; rows++) {
		/* Add new row. */
		gtk_list_store_append(store, &iter);
	}

	return store;
}

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

	frame = gtk_frame_new("Registers");

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
regwin_Show(void)
{
	int row;
	GtkListStore *store;

	store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(reglist)));

	for (row = 0; row < DATA_ROWS; row++) {
		int valid;
		GtkTreeIter iter;
		int val;
		char str[8];

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

		/* Read register value using function pointer. */
		val = regwin_read[row].read_func(regwin_read[row].param);

		/* Convert to specified number of hex digits. */
		if (regwin_read[row].w == 2)
			sprintf(str , "%.2X", (u_int8_t) val);
		else if (regwin_read[row].w == 4)
			sprintf(str , "%.4X", (u_int16_t) val);

		gtk_list_store_set(store, &iter,
				   COL_NAME, regwin_read[row].name,
				   COL_VAL, str,
				   -1);
	}
}
