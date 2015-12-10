/*
 * memwin.c
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
#include <stdbool.h>
#include <ctype.h> /* For isprint */
#include <zlib.h> /* For crc32 routine */

#include "common.h"
#include "memory.h"
#include "hexfile.h"
#include "cpu8051.h"
#include "regwin.h"
#include "memwin.h"
#include "main.h"
#include "options.h"
#include "log.h"
#include "app-config.h"

extern struct app_config_t *cfg;
extern struct options_t options;

static int COL_ASCII;
static int N_COLUMNS;

enum {
	COL_ADDRESS = 0,
	COL_DATA0,
};

/* Contains informations for the two different memory window types. */
struct memwin_infos_t {
	GtkWidget *memlist;
	int data_rows;
	uint32_t *crc;
	int crc_init;
};

static struct memwin_infos_t memwin_infos_internal;
static struct memwin_infos_t memwin_infos_external;
static struct memwin_infos_t *memwin_infos;

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
	enum mem_id_t memory_id;
	gpointer columnptr;
	gpointer memory_id_ptr;
	GtkTreeIter iter;
	int address;
	uint8_t old;
	int new;
	char *str;

	if (!model)
		log_err("Unable to get model from cell renderer");

	/* Column number is passed as renderer object data */
	columnptr = g_object_get_data(G_OBJECT(cell), "column");
	column = GPOINTER_TO_UINT(columnptr);

	/* Memory ID  is passed as renderer object data */
	memory_id_ptr = g_object_get_data(G_OBJECT(cell), "memory_id");
	memory_id = GPOINTER_TO_UINT(memory_id_ptr);

	/* Get the iterator */
	gtk_tree_model_get_iter_from_string(model, &iter, path_string);

	/* Get base address. */
	gtk_tree_model_get(model, &iter, COL_ADDRESS, &str, -1);

	/* No need to check error, has already been validated. */
	address = asciihex2int(str);

	/* Convert column number (1, 2, 3...) to index (0, 1, 2...) */
	address += (column - COL_DATA0);
	old = mem_read8(memory_id, address, true);

	log_info("Address: $%02X", address);
	log_info("  old value: $%02X", old);

	/* Convert new value (asciihex) to integer. */
	new = asciihex2int(new_str);
	if (asciihex2int_get_error()) {
		log_warn("  new value: invalid");
		return;
	} else if ((new < 0) || (new > 255)) {
		log_warn("  new value: out of range");
		return;
	} else {
		log_info("  new value: $%02X", new);
	}

	/* Store new value in emulator memory. */
	mem_write8(memory_id, address, new, false);

	/* Convert to text. */
	int2asciihex(new, str, 2);

	/* Store new value in gtk model. */
	gtk_list_store_set(GTK_LIST_STORE(model), &iter, column, str, -1);

	/*
	 * Make sure to update all registers and memory.
	 * For example, BANKed registers depends on internal memory.
	 */
	emugtk_update_display();
};

static void
memwin_init_columns(GtkWidget *listview, enum mem_id_t memory_id)
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

	for (i = COL_DATA0; i < (COL_DATA0 + cfg->bytes_per_row); i++) {
		char col_name[8];

		/* Create new renderer for each editable cell. */
		renderer = gtk_cell_renderer_text_new();

		/* Allow edition, align to left side. */
		g_object_set(renderer, "editable", TRUE, "xalign", 0, NULL);

		g_signal_connect(renderer, "edited",
				 G_CALLBACK(memwin_cell_edited),
				 gtk_tree_view_get_model(
					 GTK_TREE_VIEW(listview)));

		/* Add column index and memory_id, used when editing the cell */
		g_object_set_data(G_OBJECT(renderer), "column",
				  GUINT_TO_POINTER(i));
		g_object_set_data(G_OBJECT(renderer), "memory_id",
				  GUINT_TO_POINTER(memory_id));

		/* Use two digits only if DATA_ROWS > 10 */
		if (cfg->bytes_per_row < 10)
			sprintf(col_name, "B%1d", i - COL_DATA0);
		else
			sprintf(col_name, "B%02d", i - COL_DATA0);

		column = gtk_tree_view_column_new_with_attributes(
			col_name, renderer, "text", i, NULL);
		gtk_tree_view_column_set_sizing(column,
						GTK_TREE_VIEW_COLUMN_AUTOSIZE);
		gtk_tree_view_append_column(GTK_TREE_VIEW(listview), column);
	}

	/* Add ASCII column, using fixed-font. */
	renderer = gtk_cell_renderer_text_new();
	g_object_set(renderer, "family", "Monospace", NULL);
	column = gtk_tree_view_column_new_with_attributes(
		"ASCII", renderer, "text", COL_ASCII, NULL);
	gtk_tree_view_column_set_sizing(column, GTK_TREE_VIEW_COLUMN_AUTOSIZE);
	gtk_tree_view_append_column(GTK_TREE_VIEW(listview), column);
}

static void
memwin_infos_select(enum mem_id_t memory_id)
{
	if (memory_id == INT_MEM_ID) {
		log_debug("memory ID = INTERNAL");
		memwin_infos = &memwin_infos_internal;
	} else if (memory_id == EXT_MEM_ID) {
		log_debug("memory ID = EXTERNAL");
		memwin_infos = &memwin_infos_external;
	} else {
		log_fail("Invalid memory type");
		exit(1);
	}
}

static void
compute_data_rows(int memory_id)
{
	if (memory_id == INT_MEM_ID)
		memwin_infos->data_rows = options.iram_size /
			cfg->bytes_per_row;
	else if (memory_id == EXT_MEM_ID)
		memwin_infos->data_rows = options.xram_size /
			cfg->bytes_per_row;

	if (memwin_infos->crc)
		free(memwin_infos->crc);

	memwin_infos->crc = malloc(memwin_infos->data_rows * sizeof(uint32_t));
	memwin_infos->crc_init = false;
}

GtkWidget *
memwin_init(char *title, enum mem_id_t memory_id)
{
	GtkWidget *frame;
	GtkWidget *scrollwin;
	GtkListStore *store;

	log_debug("memwin_init");

	COL_ASCII = cfg->bytes_per_row + 1;
	N_COLUMNS = COL_ASCII + 1;

	frame = gtk_frame_new(title);

	scrollwin = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrollwin),
					    GTK_SHADOW_ETCHED_OUT);

	/* Automatically add scrollbars when necessary. */
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollwin),
				       GTK_POLICY_AUTOMATIC,
				       GTK_POLICY_AUTOMATIC);

	gtk_container_add(GTK_CONTAINER(frame), scrollwin);

	memwin_infos_select(memory_id);
	compute_data_rows(memory_id);

	/* Creating a model */
	store = memwin_init_store(memwin_infos->data_rows);

	/* Creating the view component */
	memwin_infos->memlist = gtk_tree_view_new_with_model(
		GTK_TREE_MODEL(store));
	gtk_tree_view_set_headers_visible(
		GTK_TREE_VIEW(memwin_infos->memlist), TRUE);
	gtk_container_add(GTK_CONTAINER(scrollwin), memwin_infos->memlist);

	memwin_init_columns(memwin_infos->memlist, memory_id);

	/*
	 * The tree view has acquired its own reference to the model, so we can
	 * drop ours. That way the model will be freed automatically when the
	 * tree view is destroyed.
	 */
	g_object_unref(store);

	return frame;
}

/*
 * Use CRC to detect which rows have changed. This is only to
 * improve performance when using stepping mode, as we then only update
 * rows which have been modified.
 */
static int
memwin_row_changed(enum mem_id_t memory_id, int row, unsigned int address)
{
	int row_changed;
	uint32_t crc_new = 0;
	uint8_t *buf8;

	buf8 = mem_getbuf(memory_id, address);
	crc_new = crc32(0L, Z_NULL, 0);
	crc_new = crc32(crc_new, buf8, cfg->bytes_per_row);

	if ((memwin_infos->crc_init) &&
	    (crc_new == memwin_infos->crc[row])) {
		row_changed = false;
	} else {
		memwin_infos->crc[row] = crc_new;
		log_debug("  Row %02d value(s) change", row);
		row_changed = true;
	}

	return row_changed;
}

/* Dump internal or external memory. */
void
memwin_refresh(enum mem_id_t memory_id)
{
	int row;
	unsigned int address = 0;
	GtkListStore *store;

	log_debug("memwin_refresh");

	memwin_infos_select(memory_id);

	store = GTK_LIST_STORE(gtk_tree_view_get_model(
				       GTK_TREE_VIEW(memwin_infos->memlist)));

	for (row = 0; row < memwin_infos->data_rows;
	     row++, address += cfg->bytes_per_row) {
		int valid;
		GtkTreeIter iter;
		char str[4+1]; /* Max. str len for address column (4 digits) */
		char ascii_str[16+1]; /* Maximum 16 data columns. */
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
			log_err("Tree model: invalid iter");
			return;
		}

		/* Only update row if it has been modified. */
		if (memwin_row_changed(memory_id, row, address)) {
			/* Display base address. */
			int2asciihex(address, str, 4);

			gtk_list_store_set(store, &iter, COL_ADDRESS, str, -1);

			for (col = 0; col < cfg->bytes_per_row; col++) {
				uint8_t data;

				data = mem_read8(memory_id, address + col, true);

				/* Display hex data */
				int2asciihex(data, str, 2);

				gtk_list_store_set(store, &iter, col + 1, str,
						   -1);

				/* Append to ASCII string (if applicable). */
				if (!isprint(data))
					data = '.';
				sprintf(&ascii_str[col], "%c", data);
			}

			/* Display ASCII characters. */
			gtk_list_store_set(store, &iter, COL_ASCII, ascii_str,
					   -1);
		}
	}

	/* At this point we know all rows crc have been initialized. */
	memwin_infos->crc_init = true;
}
