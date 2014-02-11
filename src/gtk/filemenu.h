/*
 * filemenu.h
 *
 * Copyright (C) 1999 Jonathan St-André
 * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This file is released under the GPLv2
 */

#ifndef FILEMENU_H
#define FILEMENU_H 1

#include <gtk/gtk.h>

void
FileOpenEvent(GtkObject *object, gpointer data);

void
FileResetEvent(GtkObject *object, gpointer data);

void
FileAddMenu(GtkWidget *menu_bar);

#endif /* FILEMENU_H */
