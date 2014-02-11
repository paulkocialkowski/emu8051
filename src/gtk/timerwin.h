/*
 * timerwin.h
 *
 * Copyright (C) 1999 Jonathan St-André
 * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This file is released under the GPLv2
 */

#ifndef TIMERWIN_H
#define TIMERWIN_H 1

#include <gtk/gtk.h>

GtkWidget *
timerwin_init(int id);

void
timerwin_update(void);

#endif /* TIMERWIN_H */
