/*
 * emugtk.h
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

#ifndef EMUGTK_H
#define EMUGTK_H 1

#include <gtk/gtk.h>

void
AddMenuSeparator(GtkWidget *menu);

void
emugtk_new_file(char *file);

void
emugtk_UpdateDisplay(void);

void
emugtk_restart_gui(void);

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

#endif /* EMUGTK_H */
