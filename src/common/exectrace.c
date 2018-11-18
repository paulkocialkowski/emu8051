/*
 * exectrace.c -- I/O tracing functions.
 *
 * Copyright (C) 2015 Paul Kocialkowski <contact@paulk.fr>
 *
 * This file is released under the GPLv2
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>

#include "common.h"
#include "cpu8051.h"
#include "options.h"
#include "exectrace.h"

static FILE *exectrace_fp = NULL;

void
exectrace_open(const char *exectrace)
{
	if (exectrace == NULL) {
		log_err("No file specified for execution trace");
		return;
	}

	exectrace_fp = fopen(exectrace, "w");
	if (exectrace_fp == NULL) {
		log_err("Error opening execution trace <%s>: %s", exectrace,
			strerror(errno));
	}

	setbuf(exectrace_fp, NULL);

	log_info("Tracing execution to %s (%s)", exectrace, options.tracing ?
		 "started" : "stopped");
}

void
exectrace_close(void)
{
	if (exectrace_fp == NULL)
		return;

	fclose(exectrace_fp);

	exectrace_fp = NULL;
}

void
exectrace_instruction(unsigned int address)
{
	char buffer[255];

	if (exectrace_fp == NULL || !options.tracing)
		return;

	cpu8051_disasm(address, buffer);
	if (buffer[0] == '\0')
		return;

	fprintf(exectrace_fp, "%s\n", buffer);
}
