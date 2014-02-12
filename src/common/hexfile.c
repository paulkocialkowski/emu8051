/*
 * Functions for loading an Intel HEX file.
 *
 * Copyright (C) 1999 Jonathan St-André
 * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This file is released under the GPLv2
 */

#if HAVE_CONFIG_H
#  include "config.h"
#endif

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#if STDC_HEADERS
#  include <string.h>
#elif HAVE_STRINGS_H
#  include <strings.h>
#endif

#include "common.h"
#include "memory.h"

/* Maximum of 75 digits with 32-bytes data records. */
#define HEXFILE_LINE_BUFFER_LEN 128

static int asciihex2int_error;

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
	int rc;

	rc = sscanf(str, "%X", &val);

	if (rc == 0) {
		log_err("ASCII to hex conversion failure");
		asciihex2int_error = true;
	}

	return val;
}

/* Convert an ascii string to an hexadecimal number. */
static unsigned int
asciihex2int_len(char *istring, int length)
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
			log_err("ASCII to hex conversion failure");
			asciihex2int_error = true;
		}
	}
	return result;
}

/*
 * Return value:
 *   true:  success
 *   false: failure
 */
int
hexfile_load(const char *filename)
{
	int i, j, rec_len, load_offset, rec_type, data, checksum;
	FILE *fp;
	int status;
	char line[HEXFILE_LINE_BUFFER_LEN];
	int valid = false;
	int line_num = 1;

	log_debug("LoadHexFile");

	if (filename == NULL)
		log_fail("No file specified");

	/* Trying to open the file. */
	fp = fopen(filename, "r");
	if (fp == NULL)
		log_fail("Error opening hex file <%s>: %s", filename,
			 strerror(errno));

	/* Reading one line of data from the hex file. */
	/* char *fgets(char *s, int size, FILE *stream);
	   Reading stops after an EOF or a newline. If a newline is read, it is
	   stored into the buffer.  A '\0'  is  stored after the last character
	   in the buffer.
	*/
	while (fgets(line, HEXFILE_LINE_BUFFER_LEN, fp) != NULL) {
		i = 0;
		checksum = 0;

		if (line[i++] != ':') {
			log_err("hexfile line not beginning with \":\"");
			goto close_file;
		}

		rec_len = asciihex2int_len(&line[i], 2);
		i += 2;
		checksum += rec_len;

		load_offset = asciihex2int_len(&line[i], 4);
		checksum += load_offset / 256;
		checksum += load_offset % 256;
		i += 4;

		rec_type = asciihex2int_len(&line[i], 2);
		i += 2;
		checksum += rec_type;

		if (rec_type == 0) {
			for (j = 0; j < rec_len; j++) {
				data = asciihex2int_len(&line[i], 2);
				memory_write8(PGM_MEM_ID,
					      (unsigned int) (load_offset + j),
					      (unsigned char) data);
				i += 2;
				checksum += data;
			}
		}

		/* Read and add checksum value */
		checksum += asciihex2int_len(&line[i], 2);
		checksum &= 0x000000FF;

		if (asciihex2int_error) {
			log_err("hexfile parse error at line %d", line_num);
			goto close_file;
		} else if (checksum) {
			log_err("hexfile checksum mismatch");
			goto close_file;
		}

		if (rec_type == 0) {
			log_debug("hex record: data");
		} else if (rec_type == 1) {
			log_debug("hex record: End Of File");
			valid = true;
			goto close_file;
		} else {
			log_warn("hex record: Unsupported ($%02X)", rec_type);
		}

		line_num++;
	}

close_file:
	status = fclose(fp);
	if (status != EXIT_SUCCESS)
		log_fail("Error closing hex file");

	if (!valid)
		log_err("Error parsing hex file");

	return valid;
}
