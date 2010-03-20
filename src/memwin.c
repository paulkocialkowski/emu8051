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

static GtkWidget *memclist;

GtkWidget *
memwin_init(int width, int height)
{
	int i;
	GtkWidget *fixed_frame;
	PangoFontDescription *pango_font;
	char *memdummy[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0,
			     0, 0, 0, 0, 0, 0, 0, 0, 0 };

	fixed_frame = gtk_frame_new(0);
	gtk_frame_set_shadow_type(GTK_FRAME(fixed_frame),
				  GTK_SHADOW_ETCHED_OUT);
	gtk_widget_set_usize(GTK_WIDGET(fixed_frame), width, height);

	memclist = gtk_clist_new(18);
	gtk_clist_set_selection_mode(GTK_CLIST(memclist), GTK_SELECTION_SINGLE);
	gtk_widget_set_usize(GTK_WIDGET(memclist), 620, 250);

	for (i = 0; i < 18; i++) {
		gtk_clist_set_column_justification(
			GTK_CLIST(memclist), i, GTK_JUSTIFY_LEFT);
	}

	gtk_clist_set_column_width(GTK_CLIST(memclist), 0, 5*8);

	for (i = 1; i < 17; i++)
		gtk_clist_set_column_width(GTK_CLIST(memclist), i, 2 * 8);

	gtk_clist_set_column_width(GTK_CLIST(memclist), 17, 16 * 8);


	pango_font = pango_font_description_from_string(FIXED_FONT);
	gtk_widget_modify_font(memclist, pango_font);

	for (i = 0; i < 16; i++)
		gtk_clist_append(GTK_CLIST(memclist), memdummy);

	gtk_container_add(GTK_CONTAINER(fixed_frame), memclist);

	return fixed_frame;
}

/* Dump 16 rows of 16 bytes from Address in Memory (direct addressing) */
void
memwin_DumpD(char *MemAddress)
{
	char TextTmp[1024];
	int row, column, TextLength;
	unsigned int Address;

	if (strlen(MemAddress) != 0) {
		if (STREQ(MemAddress, "PC"))
			Address = cpu8051.pc;
		else
			Address = Ascii2Hex(MemAddress, strlen(MemAddress));
	} else {
		Address = 0;
	}

	gtk_clist_freeze(GTK_CLIST(memclist));

	for (row = 0; row < 16; row++) {
		sprintf(TextTmp, "%.4X", Address);
		gtk_clist_set_text(GTK_CLIST(memclist), row, 0, TextTmp);

		for (column = 0; column < 16; column++) {
			sprintf(TextTmp, "%.2X",
				(int) cpu8051_ReadD(Address + column));
			gtk_clist_set_text(GTK_CLIST(memclist), row,
					   column + 1, TextTmp);
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
		gtk_clist_set_text(GTK_CLIST(memclist), row, 17, TextTmp);

		Address += 16;
	}

	gtk_clist_select_row(GTK_CLIST(memclist), 0, 0);
	gtk_clist_thaw(GTK_CLIST(memclist));
}
