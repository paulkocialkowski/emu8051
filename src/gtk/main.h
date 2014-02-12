/*
 * main.h
 *
 * Copyright (C) 1999 Jonathan St-André
 * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This file is released under the GPLv2
 */

#ifndef MAIN_H
#define MAIN_H 1

#include <gtk/gtk.h>

void
add_menu_separator(GtkWidget *menu);

void
emugtk_new_file(char *file);

void
emugtk_update_display(void);

void
emugtk_quit_gui(void);

void
emugtk_create_int_memory_paned(void);

void
emugtk_destroy_int_memory_paned(void);

void
emugtk_create_ext_memory_paned(void);

void
emugtk_destroy_ext_memory_paned(void);

void
emugtk_recreate_memory_paned(void);

#endif /* MAIN_H */
