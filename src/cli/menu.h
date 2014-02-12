/*
 * menu.h
 *
 * Copyright (C) 1999 Jonathan St-André
 * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This file is released under the GPLv2
 */

#ifndef _MENU_H_
#define _MENU_H_

int
yyparse(void);

int
menu_get_input(char *buf, ssize_t size);

void
menu_display_usage(void);

void
console_show_registers(void);

void
register_set(char *name, int value);

void
console_reset(void);

void
console_exec(int num);

void
console_trace(void);

void
disassemble_num(unsigned int address, int num);

#endif /* _MENU_H_ */
