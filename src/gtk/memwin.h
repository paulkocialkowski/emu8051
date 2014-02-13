/*
 * memwin.h
 *
 * Copyright (C) 1999 Jonathan St-André
 * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This file is released under the GPLv2
 */

#ifndef MEMWIN_H
#define MEMWIN_H 1

#include <gtk/gtk.h>

GtkWidget *
memwin_init(char *title, enum mem_id_t memory_id);

void
memwin_refresh(enum mem_id_t memory_id);

#endif /* MEMWIN_H */
