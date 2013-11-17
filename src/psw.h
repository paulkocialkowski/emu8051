/*
 * psw.h
 *
 * Copyright (C) 2013 Hugo Villeneuve <hugo@hugovil.com>
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

#ifndef PSW_H
#define PSW_H 1

#include <sys/types.h>

void
psw_set_cy(void);

void
psw_clr_cy(void);

void
psw_write_cy(int cy);

int
psw_read_cy(void);

void
psw_set_ac(void);

void
psw_clr_ac(void);

void
psw_write_ac(int ac);

int
psw_read_ac(void);

void
psw_set_ov(void);

void
psw_clr_ov(void);

void
psw_write_ov(int ov);

int
psw_read_ov(void);

#endif /* PSW_H */
