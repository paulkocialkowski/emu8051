/*
 * regwin.h
 *
 * Copyright (C) 1999 Jonathan St-André
 * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This file is released under the GPLv2
 */

#ifndef REGWIN_H
#define REGWIN_H 1

#include <gtk/gtk.h>

GtkWidget *
regwin_init(void);

void
regwin_refresh(void);

#endif /* REGWIN_H */
