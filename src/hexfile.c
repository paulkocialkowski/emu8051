/*
 * Functions for loading an Intel HEX file.
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

#if HAVE_CONFIG_H
#  include "config.h"
#endif

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#if STDC_HEADERS
#  include <string.h>
#elif HAVE_STRINGS_H
#  include <strings.h>
#endif

#include "common.h"
#include "memory.h"

/* Convert integer to ASCII hex string. */
void
int2asciihex(int val, char *str, int width)
{
	if (width == 1)
		sprintf(str , "%.1X", (u_int8_t) val);
	else if (width == 2)
		sprintf(str , "%.2X", (u_int8_t) val);
	else if (width == 4)
		sprintf(str , "%.4X", (u_int16_t) val);
	else
		sprintf(str , "Err");
}

/* Convert ASCII hex string to integer. */
int
asciihex2int(char *str)
{
	int val;

	sscanf(str, "%X", &val);

	return val;
}

/* Convert an ascii string to an hexadecimal number. */
unsigned int
Ascii2Hex(char *istring, int length)
{
	unsigned int result = 0;
	int i, ascii_code;

	if (!length || (length > (int) strlen(istring)))
		length = strlen(istring);

	for (i = 0; i < length; i++) {
		ascii_code = istring[i];
		if (ascii_code > 0x39)
			ascii_code &= 0xDF;

		if ((ascii_code >= 0x30 && ascii_code <= 0x39) ||
		     (ascii_code >= 0x41 && ascii_code <= 0x46)) {
			ascii_code -= 0x30;
			if (ascii_code > 9)
				ascii_code -= 7;

			result <<= 4;
			result += ascii_code;
		} else {
			printf("%s: In Ascii2Hex(), syntax error.\n", PACKAGE);
			printf("  str=%s, length=%d\n", istring, length);
		}
	}
	return result;
}

void
LoadHexFile(const char *filename)
{
	int i, j, RecLength, LoadOffset, RecType, Data, Checksum;

#define LINE_BUFFER_LEN 256
	FILE *fp;
	int status;
	char line[LINE_BUFFER_LEN];

	if (filename == NULL) {
		printf("%s: No file specified\n", PACKAGE);
		exit(EXIT_FAILURE);
	}

	/* Trying to open the file. */
	fp = fopen(filename, "r");
	if (fp == NULL) {
		perror(PACKAGE);
		/*ErrorLocation(__FILE__, __LINE__);*/
		exit(EXIT_FAILURE);
	}

	/* Reading one line of data from the hex file. */
	/* char *fgets(char *s, int size, FILE *stream);
	   Reading stops after an EOF or a newline. If a newline is read, it is
	   stored into the buffer.  A '\0'  is  stored after the last character
	   in the buffer.
	*/
	while (fgets(line, LINE_BUFFER_LEN, fp) != NULL) {
		i = 0;
		Checksum = 0;

		if (line[i++] != ':') {
			printf("%s: line not beginning with \":\"\n", PACKAGE);
			goto close_file;
		}

		RecLength = Ascii2Hex(&line[i], 2);
		i += 2;
		Checksum += RecLength;

		LoadOffset = Ascii2Hex(&line[i], 4);
		Checksum += LoadOffset / 256;
		Checksum += LoadOffset % 256;
		i += 4;

		RecType = Ascii2Hex(&line[i], 2);
		i += 2;
		Checksum += RecType;

		if (RecType == 1) {
			Checksum += Ascii2Hex(&line[i], 2);
			Checksum &= 0x000000FF;

			if (Checksum) {
				/* Error. */
				printf("%s: Invalid format\n", PACKAGE);
				goto close_file;
			} else {
				/* OK */
				goto close_file;
			}
		}

		for (j = 0; j < RecLength; j++) {
			Data = Ascii2Hex(&line[i], 2);
			memory_write8(PGM_MEM_ID,
				      (unsigned int)(LoadOffset + j),
				      (unsigned char)Data);
			i += 2;
			Checksum += Data;
		}

		RecType = Ascii2Hex(&line[i], 2);
		Checksum += RecType;
		Checksum &= 0x000000FF;

		if (Checksum) {
			printf("%s: Invalid format\n", PACKAGE);
			goto close_file;
		}
	}

close_file:
	status = fclose(fp);
	if (status != EXIT_SUCCESS) {
		fprintf(stderr, "%s: Error closing hex file.\n", PACKAGE);
		/*ErrorLocation(__FILE__, __LINE__);*/
		exit(EXIT_FAILURE);
	}
}
