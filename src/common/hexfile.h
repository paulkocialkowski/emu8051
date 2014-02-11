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

void
int2asciihex(int val, char *str, int width);

int
asciihex2int(char *str);

unsigned int
Ascii2Hex(char *istring, int length);

int
LoadHexFile(const char *filename);

#endif /* HEXFILE_H */
