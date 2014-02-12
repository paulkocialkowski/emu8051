/*
 * viewmenu.c
 *
 * Copyright (C) 1999 Jonathan St-André
 * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This file is released under the GPLv2
 */

#if HAVE_CONFIG_H
#  include "config.h"
#endif

#include <gtk/gtk.h>

#include "common.h"
#include "main.h" /* For AddMenuSeparator() function. */
#include "messagebox.h"
#include "viewmenu.h"
#include "app-config.h"

extern struct app_config_t *cfg;

void toggle_layout(GtkWidget *widget, gpointer data)
{
	int id;

	id = GPOINTER_TO_UINT(data);

	if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget))) {
		log_info("  Switching to layout %d", id);
		cfg->layout = id;

		message_show_information(
			"You must restart for the changes to take effect");
	}
}

void toggle_bytes_per_row(GtkWidget *widget, gpointer data)
{
	int bytes_per_row;

	bytes_per_row = GPOINTER_TO_UINT(data);

	if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget))) {
		log_info("  Bytes per row = %d", bytes_per_row);
		cfg->bytes_per_row = bytes_per_row;
		emugtk_destroy_int_memory_paned();
		emugtk_destroy_ext_memory_paned();
		emugtk_recreate_memory_paned();
	}
}

void toggle_int_memory(GtkWidget *widget, gpointer data)
{
	(void) data; /* Remove compiler warning about unused variables. */

	if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget))) {
		log_info("  View internal memory");
		cfg->view_int_memory = 1;
		emugtk_create_int_memory_paned();
	} else {
		cfg->view_int_memory = 0;
		emugtk_destroy_int_memory_paned();
	}
}

void toggle_ext_memory(GtkWidget *widget, gpointer data)
{
	(void) data; /* Remove compiler warning about unused variables. */

	if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget))) {
		log_info("  View external memory");
		cfg->view_ext_memory = 1;
		emugtk_create_ext_memory_paned();
	} else {
		cfg->view_ext_memory = 0;
		emugtk_destroy_ext_memory_paned();
	}
}

void
view_add_layout_submenu(GtkWidget *parent)
{
	GtkWidget *submenu;
	GtkWidget *layout;
	GtkWidget *layout1;
	GtkWidget *layout2;
	GSList *group = NULL;

	submenu = gtk_menu_new();

	layout  = gtk_menu_item_new_with_label("Layout");

	layout1 = gtk_radio_menu_item_new_with_label(group, "Layout1");
	group = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(layout1));
	layout2 = gtk_radio_menu_item_new_with_label(group, "Layout2");

	if (cfg->layout == UI_LAYOUT1)
		gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(layout1),
					       TRUE);
	else
		gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(layout2),
					       TRUE);

	g_signal_connect(G_OBJECT(layout1), "activate",
			 G_CALLBACK(toggle_layout), (gpointer) UI_LAYOUT1);
	g_signal_connect(G_OBJECT(layout2), "activate",
			 G_CALLBACK(toggle_layout), (gpointer) UI_LAYOUT2);

	gtk_menu_item_set_submenu(GTK_MENU_ITEM(layout), submenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(submenu), layout1);
	gtk_menu_shell_append(GTK_MENU_SHELL(submenu), layout2);
	gtk_menu_shell_append(GTK_MENU_SHELL(parent), layout);
}

void
view_add_bytes_per_row_submenu(GtkWidget *parent)
{
	GtkWidget *submenu;
	GtkWidget *item;
	GtkWidget *item1;
	GtkWidget *item2;
	GSList *group = NULL;

	submenu = gtk_menu_new();

	item  = gtk_menu_item_new_with_label("Bytes per row");

	item1 = gtk_radio_menu_item_new_with_label(group, "8");
	group = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(item1));
	item2 = gtk_radio_menu_item_new_with_label(group, "16");

	if (cfg->bytes_per_row == 8)
		gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item1),
					       TRUE);
	else
		gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item2),
					       TRUE);

	g_signal_connect(G_OBJECT(item1), "activate",
			 G_CALLBACK(toggle_bytes_per_row), (gpointer) 8);
	g_signal_connect(G_OBJECT(item2), "activate",
			 G_CALLBACK(toggle_bytes_per_row), (gpointer) 16);

	gtk_menu_item_set_submenu(GTK_MENU_ITEM(item), submenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(submenu), item1);
	gtk_menu_shell_append(GTK_MENU_SHELL(submenu), item2);
	gtk_menu_shell_append(GTK_MENU_SHELL(parent), item);
}

void
view_add_menu(GtkWidget *menu_bar)
{
	GtkWidget *item;
	GtkWidget *menu;
	GtkWidget *view;

	menu = gtk_menu_new();

	view = gtk_menu_item_new_with_label("View");

	item = gtk_check_menu_item_new_with_label("Internal Memory");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item),
				       cfg->view_int_memory);
	g_signal_connect(G_OBJECT(item), "activate",
			 G_CALLBACK(toggle_int_memory), NULL);

	item = gtk_check_menu_item_new_with_label("External Memory");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item),
				       cfg->view_ext_memory);
	g_signal_connect(G_OBJECT(item), "activate",
			 G_CALLBACK(toggle_ext_memory), NULL);

	add_menu_separator(menu);

	/* Add layout submenu */
	view_add_layout_submenu(menu);

	add_menu_separator(menu);

	/* Add bytes per row submenu */
	view_add_bytes_per_row_submenu(menu);

	gtk_menu_item_set_submenu(GTK_MENU_ITEM(view), menu);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), view);
}
