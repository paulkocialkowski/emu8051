/*
 * timerwin.c
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
#include <stdbool.h>
#include <stdint.h>

#include "common.h"
#include "gp_timers.h"
#include "main.h"

static GtkWidget *label[GP_TIMERS_COUNT];

static GtkWidget *
button_add_stock(GtkWidget *box, gchar *stock_id, int display_label)
{
	GtkWidget *button;

	/* Create the button. */
	if (display_label) {
		/* By default, a label is appended to stock buttons. */
		button = gtk_button_new_from_stock(stock_id);
	} else {
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
	int id;
	char buf[128];

	for (id = 0; id < GP_TIMERS_COUNT; id++) {
		/* Display textin bold, with big font size. */
		sprintf(buf , "<b><big>%08d</big></b> cycles",
			gp_timer_read(id));

		gtk_label_set_markup(GTK_LABEL(label[id]), buf);
	}
}

static void
timer_reset_callback(GtkWidget *widget, gpointer data)
{
	int id = GPOINTER_TO_INT(data);

	/* Remove compiler warning about unused variables. */
	(void) widget;

	log_info("timer_reset_callback ID = %d", id);

	gp_timer_reset(id);
	timerwin_update();
}

GtkWidget *
timerwin_init(int id)
{
	GtkWidget *frame;
	GtkWidget *hbox;
	GtkWidget *vbox;
	GtkWidget *timer_reset_button;
	char title[100];

	log_debug("timer window init");

	sprintf(title, "Emulator timer %c", 'A' + id);
	frame = gtk_frame_new(title);

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
			 G_CALLBACK(timer_reset_callback), GINT_TO_POINTER(id));
	gtk_box_pack_start(GTK_BOX(hbox), vbox, false, false, 3);

	label[id] = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(label[id]), "<small>Small text</small>");
	gtk_box_pack_start(GTK_BOX(hbox), label[id], false, false, 10);

	gtk_container_add(GTK_CONTAINER(frame), hbox);

	return frame;
}
