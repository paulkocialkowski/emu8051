/*
 * sfr.h
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

#ifndef SFR_H
#define SFR_H 1

#define SFR_REGS 25

struct regwin_infos_t {
	char *name; /* Register name */
	int w; /* Number of hex digits to display */
	unsigned int (*read_func)(int addr); /* Function to read value */
	void (*write_func)(int param, int val); /* Function to write value */
	int param; /* Optional parameter to pass to read function. */
};

int
regwin_read(int row);

void
regwin_write(struct regwin_infos_t *p, int new);

struct regwin_infos_t *
sfr_get_infos(const char *regname);

struct regwin_infos_t *
sfr_get_infos_from_row(int row);

#endif /* SFR_H */
