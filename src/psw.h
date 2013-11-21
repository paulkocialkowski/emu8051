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

int
psw_read_bit(int bit);

void
psw_write_bit(int bit, int val);

int
psw_read_cy(void);

void
psw_write_cy(int cy);

void
psw_clr_cy(void);

void
psw_set_cy(void);

int
psw_read_ac(void);

void
psw_write_ac(int ac);

void
psw_clr_ac(void);

void
psw_set_ac(void);

int
psw_read_ov(void);

void
psw_write_ov(int ov);

void
psw_clr_ov(void);

void
psw_set_ov(void);

void
psw_compute_parity_bit(void);

#endif /* PSW_H */
