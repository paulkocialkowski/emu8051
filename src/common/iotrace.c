/*
 * iotrace.c -- I/O tracing functions.
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
#include "memory.h"
#include "cpu8051.h"
#include "options.h"
#include "iotrace.h"

static FILE *iotrace_fp = NULL;

static void
iotrace_memory(enum mem_id_t id)
{
	if (iotrace_fp == NULL)
		return;

	fprintf(iotrace_fp, "MEM");

	switch (id) {
		case PGM_MEM_ID:
			fprintf(iotrace_fp, " : PGM");
			break;
		case INT_MEM_ID:
			fprintf(iotrace_fp, " : INT");
			break;
		case SFR_MEM_ID:
			fprintf(iotrace_fp, " : SFR");
			break;
		case EXT_MEM_ID:
			fprintf(iotrace_fp, " : EXT");
			break;
		default:
			log_err("Unknown memory type: %d\n", id);
			break;
	}
}

static void
iotrace_pc(void)
{
	if (iotrace_fp == NULL)
		return;

	fprintf(iotrace_fp, " : PC : 0x%04x", cpu8051.pc);
}

static void
iotrace_data(enum mem_id_t id, unsigned int address, uint8_t value)
{
	if (iotrace_fp == NULL)
		return;

	if (id == INT_MEM_ID || id == SFR_MEM_ID)
		fprintf(iotrace_fp, " : A : 0x%02x", address);
	else
		fprintf(iotrace_fp, " : A : 0x%04x", address);

	fprintf(iotrace_fp, " : V : 0x%02x", value);
}

static void
iotrace_newline(void)
{
	if (iotrace_fp == NULL)
		return;

	fprintf(iotrace_fp, "\n");
}

void
iotrace_open(const char *iotrace)
{
	if (iotrace == NULL) {
		log_err("No file specified for I/O trace");
		return;
	}

	iotrace_fp = fopen(iotrace, "w");
	if (iotrace_fp == NULL) {
		log_err("Error opening I/O trace <%s>: %s", iotrace,
			strerror(errno));
	}

	setbuf(iotrace_fp, NULL);

	log_info("Tracing I/O to %s (%s)", iotrace, options.tracing ?
		 "started" : "stopped");
}

void
iotrace_close(void)
{
	if (iotrace_fp == NULL)
		return;

	fclose(iotrace_fp);

	iotrace_fp = NULL;
}

void
iotrace_interrupt(int index, unsigned int address)
{
	if (iotrace_fp == NULL || !options.tracing)
		return;

	fprintf(iotrace_fp, "INT : %d : A : 0x%04x\n", index, address);
}

void
iotrace_memory_read(enum mem_id_t id, unsigned int address, uint8_t value)
{
	if (iotrace_fp == NULL || !options.tracing)
		return;

	iotrace_memory(id);

	fprintf(iotrace_fp, " : R");
	iotrace_data(id, address, value);

	iotrace_pc();
	iotrace_newline();
}

void
iotrace_memory_write(enum mem_id_t id, unsigned int address, uint8_t value)
{
	if (iotrace_fp == NULL || !options.tracing)
		return;

	iotrace_memory(id);

	fprintf(iotrace_fp, " : W");
	iotrace_data(id, address, value);

	iotrace_pc();
	iotrace_newline();
}

void
iotrace_message(const char *message)
{
	if (iotrace_fp == NULL || !options.tracing)
		return;

	fprintf(iotrace_fp, "MSG : %s\n", message);
}
