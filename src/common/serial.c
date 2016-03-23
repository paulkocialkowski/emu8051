/*
 * serial.c -- Serial functions.
 *
 * Copyright (C) 2015 Paul Kocialkowski <contact@paulk.fr>
 *
 * This file is released under the GPLv2
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>

#include "common.h"
#include "reg8051.h"
#include "serial.h"

static FILE *serial_fp = NULL;

void
serial_open(const char *serial)
{
	if (serial == NULL) {
		log_err("No file specified for serial output");
		return;
	}

	serial_fp = fopen(serial, "w");
	if (serial_fp == NULL) {
		log_err("Error opening serial output <%s>: %s", serial,
			strerror(errno));
	}

	setbuf(serial_fp, NULL);

	log_info("Outputting serial to %s", serial);
}

void
serial_close(void)
{
	if (serial_fp == NULL)
		return;

	fclose(serial_fp);

	serial_fp = NULL;
}

void
serial_write(char c)
{
	if (serial_fp == NULL)
		return;

	fputc(c, serial_fp);

	mem_write_bit(SCON_BIT_TI, 1, true);
}
