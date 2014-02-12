/*
 * keyboard.h
 *
 * Copyright (C) 1999 Jonathan St-André
 * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This file is released under the GPLv2
 */

#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

int
kbhit(void);

int
getch(void);

void
keyboard_init(void);

void
keyboard_reset(void);

#endif /* _KEYBOARD_H_ */
