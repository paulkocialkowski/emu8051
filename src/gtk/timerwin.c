/*
 * timerwin.c
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
#include <stdbool.h>
#include <stdint.h>

#include "common.h"
#include "timers.h"
#include "main.h"

static GtkWidget *label;

static GtkWidget *
button_add_stock(GtkWidget *box, gchar *stock_id, int display_label)
{
	GtkWidget *button;

	/* Create the button. */
	if (display_label != false)
		button = gtk_button_new_from_stock(stock_id);
	else {
		GtkWidget *icon;

		button = gtk_button_new();
		gtk_button_set_relief(GTK_BUTTON(button), GTK_RELIEF_NORMAL);
		icon = gtk_image_new_from_stock(stock_id,
						GTK_ICON_SIZE_SMALL_TOOLBAR);
		gtk_container_add(GTK_CONTAINER(button), icon);
	}

	gtk_box_pack_start(GTK_BOX(box), button, false, false, 2);

	return button;
}

void
timerwin_update(void)
{
	char buf[128];

	/* Display textin bold, with big font size. */
	sprintf(buf , "<b><big>%08d</big></b> cycles", gp_timer_read());

	gtk_label_set_markup(GTK_LABEL(label), buf);
}

static void
timer_reset_callback(GtkWidget *widget, gpointer data)
{
	/* Remove compiler warning about unused variables. */
	(void) widget;
	(void) data;

	gp_timer_reset();
	timerwin_update();
}

GtkWidget *
timerwin_init(void)
{
	GtkWidget *frame;
	GtkWidget *hbox;
	GtkWidget *vbox;
	GtkWidget *timer_reset_button;

	log_debug("timer window init");

	frame = gtk_frame_new("General-purpose Timer");

	/* The items of the hbox are NOT given equal space in the box. */
	hbox = gtk_hbox_new(false, 0);

	/*
	 * If the button was added directly to the hbox, it would be as high
	 * as the frame widget (ugly). Adding it first to a vbox makes it have
	 * a box shape.
	 */
	vbox = gtk_vbox_new(true, 0);
	timer_reset_button = button_add_stock(vbox, GTK_STOCK_REFRESH, false);
	g_signal_connect(G_OBJECT(timer_reset_button), "clicked",
			 G_CALLBACK(timer_reset_callback), NULL);
	gtk_box_pack_start(GTK_BOX(hbox), vbox, false, false, 3);

	label = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(label), "<small>Small text</small>");
	gtk_box_pack_start(GTK_BOX(hbox), label, false, false, 10);

	gtk_container_add(GTK_CONTAINER(frame), hbox);

	return frame;
}
