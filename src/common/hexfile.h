/*
 * hexfile.h
 *
 * Copyright (C) 1999 Jonathan St-André
 * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This file is released under the GPLv2
 */

#ifndef HEXFILE_H
#define HEXFILE_H 1

int
asciihex2int_get_error(void);

void
int2asciihex(int val, char *str, int width);

unsigned int
asciihex2int(char *str);

int
hexfile_load(const char *filename);

#endif /* HEXFILE_H */
