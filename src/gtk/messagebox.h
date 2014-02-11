/*
 * messagebox.h
 *
 * Copyright (C) 1999 Jonathan St-André
 * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This file is released under the GPLv2
 */

#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H 1

#include <gtk/gtk.h>

void
message_show_error(char *message);

void
message_show_information(char *message);

#endif /* MESSAGEBOX_H */
