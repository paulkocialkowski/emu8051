/*
 * pgmwin.h
 *
 * Copyright (C) 1999 Jonathan St-André
 * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This file is released under the GPLv2
 */

#ifndef PGMWIN_H
#define PGMWIN_H 1

#include <gtk/gtk.h>

GtkWidget *
pgmwin_init(void);

void
pgmwin_refresh(void);

#endif /* PGMWIN_H */
