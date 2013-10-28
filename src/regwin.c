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
#include "memory.h"
#include "regwin.h"
#include "memwin.h"
#include "pgmwin.h"
#include "instructions_8051.h"
#include "hexfile.h"
#include "emugtk.h"

static GtkWidget *reglist;

#define DATA_ROWS 26

enum
{
	COL_NAME = 0,
	COL_VAL,
	N_COLUMNS,
};

#define HEX_DIGITS_2 2
#define HEX_DIGITS_4 4

struct regwin_infos_t {
	char *name; /* Register name */
	int w; /* Number of hex digits to display */
	unsigned int (*read_func)(int addr); /* Function to read value */
	void (*write_func)(int param, int val); /* Function to write value */
	int param; /* Optional parameter to pass to read function. */
};

/* Generic read/write functions */
static unsigned int
regwin_read(int addr, int width)
{
	if (width == 2)
		return memory_sfr_read8(addr);
	else if (width == 4) {
		/* Address is low address. */
		return (memory_sfr_read8(addr + 1) << 8) |
			memory_sfr_read8(addr);
	} else
		return 0xFFFFFFFF;
}

static void
regwin_write(int addr, int val, int width)
{
	if (width == 2)
		memory_sfr_write8(addr, (u_int8_t) val);
	else if (width == 4) {
		/* Address is low address. */
		memory_sfr_write8(addr + 1, (u_int8_t) ((val & 0x0000FFFF) >> 8));
		memory_sfr_write8(addr, (u_int8_t) val);
	}
};

/* Specific read/write functions for special registers. */

static unsigned int
regwin_read_pc(int dummy)
{
	return cpu8051.pc;
}

static void
regwin_write_pc(int param, int val)
{
	cpu8051.pc = (u_int16_t) val;
}

static unsigned int
regwin_read_timer(int timer_low_addr)
{
	return (memory_sfr_read8(timer_low_addr + 2) << 8) |
		memory_sfr_read8(timer_low_addr);
}

static void
regwin_write_timer(int timer_low_addr, int val)
{
	memory_sfr_write8(timer_low_addr + 2, (u_int8_t) ((val & 0x0000FFFF) >> 8));
	memory_sfr_write8(timer_low_addr, (u_int8_t) val);
}

static u_int8_t
regwin_read_bank_offset(void)
{
	return memory_sfr_read8(_PSW_) & 0x18;
}

static unsigned int
regwin_read_bank(int dummy)
{
	return regwin_read_bank_offset() >> 3;
}

static void
regwin_write_bank(int param, int bank_number)
{
	u_int8_t psw = memory_sfr_read8(_PSW_);

	if ((bank_number < 0) || (bank_number > 3)) {
		log_info("Error: invalid bank number: %d", bank_number);
		bank_number = 0;
	}

	memory_sfr_write8(_PSW_, (psw & ~0x18) | (bank_number << 3));
}

/* Indirect read of R0 - R7 in current bank from internal memory. */
static unsigned int
regwin_read_rx(int offset)
{
	return memory_read8(INT_MEM_ID, regwin_read_bank_offset() + offset);
}

/* Indirect write to R0 - R7 in current bank to internal memory. */
static void
regwin_write_rx(int offset, int val)
{
	memory_write8(INT_MEM_ID, regwin_read_bank_offset() + offset, (u_int8_t) val);
}

/* This array defines how to read value for each register. */
static struct regwin_infos_t regwin_infos[DATA_ROWS] = {
	{
		"PC",
		HEX_DIGITS_4,
		regwin_read_pc, regwin_write_pc,
		0, /* Dummy */
	},
	{
		"SP",
		HEX_DIGITS_2,
		NULL, NULL,
		_SP_,
	},
	{
		"A",
		HEX_DIGITS_2,
		NULL, NULL,
		_ACC_,
	},
	{
		"B",
		HEX_DIGITS_2,
		NULL, NULL,
		_B_,
	},
	{
		"DPTR",
		HEX_DIGITS_4,
		NULL, NULL,
		_DPL_,
	},
	{
		"PSW",
		HEX_DIGITS_2,
		NULL, NULL,
		_PSW_,
	},
	{
		"P0",
		HEX_DIGITS_2,
		NULL, NULL,
		_P0_,
	},
	{
		"P1",
		HEX_DIGITS_2,
		NULL, NULL,
		_P1_,
	},
	{
		"P2",
		HEX_DIGITS_2,
		NULL, NULL,
		_P2_,
	},
	{
		"P3",
		HEX_DIGITS_2,
		NULL, NULL,
		_P3_,
	},
	{
		"TCON",
		HEX_DIGITS_2,
		NULL, NULL,
		_TCON_,
	},
	{
		"TMOD",
		HEX_DIGITS_2,
		NULL, NULL,
		_TMOD_,
	},
	{
		"TIMER0",
		HEX_DIGITS_4,
		regwin_read_timer, regwin_write_timer,
		_TL0_,
	},
	{
		"TIMER1",
		HEX_DIGITS_4,
		regwin_read_timer, regwin_write_timer,
		_TL1_,
	},
	{
		"SCON",
		HEX_DIGITS_2,
		NULL, NULL,
		_SCON_,
	},
	{
		"IE",
		HEX_DIGITS_2,
		NULL, NULL,
		_IE_,
	},
	{
		"IP",
		HEX_DIGITS_2,
		NULL, NULL,
		_IP_,
	},
	{
		"BANK",
		HEX_DIGITS_2,
		regwin_read_bank, regwin_write_bank,
		0, /* Dummy */
	},
	/* R0-R7 Registers in current Bank */
	{
		"R0",
		HEX_DIGITS_2,
		regwin_read_rx, regwin_write_rx,
		_R0_,
	},
	{
		"R1",
		HEX_DIGITS_2,
		regwin_read_rx, regwin_write_rx,
		_R1_,
	},
	{
		"R2",
		HEX_DIGITS_2,
		regwin_read_rx, regwin_write_rx,
		_R2_,
	},
	{
		"R3",
		HEX_DIGITS_2,
		regwin_read_rx, regwin_write_rx,
		_R3_,
	},
	{
		"R4",
		HEX_DIGITS_2,
		regwin_read_rx, regwin_write_rx,
		_R4_,
	},
	{
		"R5",
		HEX_DIGITS_2,
		regwin_read_rx, regwin_write_rx,
		_R5_,
	},
	{
		"R6",
		HEX_DIGITS_2,
		regwin_read_rx, regwin_write_rx,
		_R6_,
	},
	{
		"R7",
		HEX_DIGITS_2,
		regwin_read_rx, regwin_write_rx,
		_R7_,
	},
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

static int
regwin_find_row(const char *regname)
{
	int row;

	for (row = 0; row < DATA_ROWS; row++) {
		if (strcmp(regwin_infos[row].name, regname) == 0)
			return row;
	}

	return -1; /* Programming error. */
}

static void
regwin_cell_edited(GtkCellRendererText *cell, gchar *path_string,
		   gchar *new_str, gpointer model)
{
	GtkTreeIter iter;
	int old;
	int new;
	char *str;
	int row;

	if (!model) {
		g_error("Unable to get model from cell renderer");
	}

	/* Get the iterator */
        gtk_tree_model_get_iter_from_string(model, &iter, path_string);

	/* Get register name. */
	gtk_tree_model_get(model, &iter, COL_NAME, &str, -1);

	log_info("Register: %s", str);
	row = regwin_find_row(str);
	log_info("  row = %d", row);

	/* Read current (old) value. */
	gtk_tree_model_get(model, &iter, COL_VAL, &str, -1);

	old = asciihex2int(str);

	if (regwin_infos[row].w == 2)
		log_info("  old value: $%02X", old);
	else if (regwin_infos[row].w == 4)
		log_info("  old value: $%04X", old);

	new = asciihex2int(new_str);

	if (regwin_infos[row].w == 2) {
		if ((new < 0) || (new > 0xFF)) {
			log_info("  new value: out of range");
			new = old; /* Put back old value... */
		} else {
			log_info("  new value: $%02X", new);
		}
	} else if (regwin_infos[row].w == 4) {
		if ((new < 0) || (new > 0xFFFF)) {
			log_info("  new value: out of range");
			new = old; /* Put back old value... */
		} else {
			log_info("  new value: $%04X", new);
		}
	}

	/* Convert new value to text. */
	int2asciihex(new, str, regwin_infos[row].w);

	/* Store new value in emulator register. */
	if (regwin_infos[row].write_func == NULL) {
		/*
		 * Write register value using generic 8 or 16 bits write
		 * function, depending on width.
		 */
		regwin_write(regwin_infos[row].param, new, regwin_infos[row].w);
	} else {
		/* Write register value using custom function pointer. */
		regwin_infos[row].write_func(regwin_infos[row].param, new);
	}

	/* Store new value in gtk model. */
        gtk_list_store_set(GTK_LIST_STORE(model), &iter, COL_VAL, str, -1);

	/*
	 * Make sure to update all windows.
	 * For example, R0-R7 values depends on internal memory values.
	 */
	emugtk_UpdateDisplay();
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

		if (regwin_infos[row].read_func == NULL) {
			/*
			 * Read register value using generic 8 or 16 bits read
			 * function, depending on width.
			 */
			val = regwin_read(regwin_infos[row].param,
					  regwin_infos[row].w);
		} else {
			/* Read register value using custom function pointer. */
			val = regwin_infos[row].read_func(
				regwin_infos[row].param);
		}

		/* Convert to specified number of hex digits. */
		int2asciihex(val, str, regwin_infos[row].w);

		gtk_list_store_set(store, &iter,
				   COL_NAME, regwin_infos[row].name,
				   COL_VAL, str,
				   -1);
	}
}
