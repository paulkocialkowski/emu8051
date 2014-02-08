/*
 * menu.h
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
SetRegister(char *Register, int NewValue);

void
console_reset(void);

void
console_exec(int NbInst);

void
console_trace(void);

void
DisasmN(unsigned int Address, int NumberInst);

#endif /* _MENU_H_ */
