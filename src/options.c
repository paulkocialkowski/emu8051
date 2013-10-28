/*
 * options.c -- functions for processing command-line options and arguments
 *
 * Copyright (C) 2011 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#if HAVE_CONFIG_H
#  include "config.h"
#endif

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <argp.h>

#if STDC_HEADERS
#  include <string.h>
#elif HAVE_STRINGS_H
#  include <strings.h>
#endif

#include "common.h"
#include "options.h"
#include "memory.h"

const char *argp_program_version = PACKAGE_VERSION;
const char *argp_program_bug_address = PACKAGE_BUGREPORT;

#define PACKAGE_DOC_LENGTH 128

/* Program documentation. */
static char str_doc[PACKAGE_DOC_LENGTH];

/* How many arguments we accept. */
#define ARGS_COUNT 1

/* A description of the arguments we accept. */
static const char args_doc[] = "[FILENAME]";

/* The options we understand. */
static struct argp_option argp_options[] = {
	{"debug", 'd', "level", 0,  "Produce debugging output" },
	{"iram",  'i', "size",  0,  "Set internal ram size" },
	{"xram",  'x', "size",  0,  "Set external ram size (default is 1024)" },
	{ 0 }
};

struct options_t options;

const char *
get_package_description(void)
{
	return "Emulator for 8051 family microcontrollers";
}

static void
decode_debug_option(char *arg, struct argp_state *state)
{
	char *endptr;
	int log_level;

	log_level = strtol(arg, &endptr, 0);

	if (*endptr != '\0') {
		log_fail_no_exit("Invalid log level");
		argp_usage(state);
	}

	if (log_level > LOG_LEVEL_DEBUG) {
		log_fail_no_exit("Invalid log level");
		argp_usage(state);
	}

	log_set_level(log_level);
}

static void
decode_memory_size(char *arg, struct argp_state *state, int memid)
{
	char *endptr;
	int max_size;
	int *dest;

	if (memid == INT_MEM_ID) {
		max_size = INT_MEM_SIZE;
		dest = &options.iram_size;
	} else {
		max_size = EXT_MEM_SIZE;
		dest = &options.xram_size;
	}

	*dest = strtol(arg, &endptr, 0);

	if (*endptr != '\0') {
		log_fail_no_exit("Invalid memory size");
		argp_usage(state);
	}

	if (*dest > max_size) {
		log_fail_no_exit("Invalid maximum memory size (max = %d)",
			max_size);
		argp_usage(state);
	}
}

/* Parse a single option. */
static error_t
parse_opt(int key, char *arg, struct argp_state *state)
{
	switch (key) {
	case 'd':
		decode_debug_option(arg, state);
		break;
	case 'i':
		decode_memory_size(arg, state, INT_MEM_ID);
		break;
	case 'x':
		decode_memory_size(arg, state, EXT_MEM_ID);
		break;
	case ARGP_KEY_ARG:
		if (state->arg_num >= ARGS_COUNT) {
			/* Too many arguments. */
			argp_usage(state);
		}

		options.filename = arg;
		break;
	case ARGP_KEY_END:
		if (state->arg_num < ARGS_COUNT) {
			/* Not enough arguments, but the filename is optional.
			   So no error. */
		}
		break;
	default:
		return ARGP_ERR_UNKNOWN;
	}

	return 0;
}

/* Our argp parser. */
static struct argp argp = { argp_options, parse_opt, args_doc, str_doc };

/* Initializes the different options passed as arguments on the command line. */
void
parse_command_line_options(int argc, char *argv[])
{
	snprintf(str_doc, PACKAGE_DOC_LENGTH, "%s -- %s", PACKAGE_NAME,
		 get_package_description());

	/* Setting default values. */
	options.filename = NULL;
	options.iram_size = INT_MEM_SIZE;
	options.xram_size = EXT_MEM_DEFAULT_SIZE;

	/* Parse our arguments. */
	argp_parse(&argp, argc, argv, 0, 0, NULL);
}
