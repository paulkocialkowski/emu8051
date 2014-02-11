/*
 * pswwin.h
 *
 * Copyright (C) 1999 Jonathan St-André
 * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This file is released under the GPLv2
 */

#ifndef PSWWIN_H
#define PSWWIN_H 1

#include <gtk/gtk.h>

GtkWidget *
pswwin_init(void);

void
pswwin_refresh(void);

#endif /* PSWWIN_H */
