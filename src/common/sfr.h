/*
 * sfr.h
 *
 * Copyright (C) 2013 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This file is released under the GPLv2
 */

#ifndef SFR_H
#define SFR_H 1

#define SFR_REGS 26

struct regwin_infos_t {
	char *name; /* Register name */
	int w; /* Number of hex digits to display */
	unsigned int (*read_func)(int addr); /* Function to read value */
	void (*write_func)(int param, int val); /* Function to write value */
	int param; /* Optional parameter to pass to read function. */
};

int
regwin_read(int row);

int
regwin_write(struct regwin_infos_t *p, int new);

struct regwin_infos_t *
sfr_get_infos(const char *regname);

struct regwin_infos_t *
sfr_get_infos_from_row(int row);

#endif /* SFR_H */
