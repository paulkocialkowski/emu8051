/*
 * options.h
 *
 * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This file is released under the GPLv2
 */

#ifndef OPTIONS_H
#define OPTIONS_H 1

#if HAVE_CONFIG_H
#  include "config.h"
#endif

#define PACKAGE_COPYRIGHT "(c) Hugo Villeneuve"

#define PACKAGE_DESCRIPTION "Emulator for 8051 family microcontrollers"

struct options_t {
	char *g;
	int pram_size; /* Maximum program memory size. */
	int iram_size; /* Maximum internal ram size. */
	int xram_size; /* Maximum external ram size. */
	char *filename;
	char *iotrace;
	char *exectrace;
	char *serial;
	int interactive;
#if HAVE_DEVICE
	char *device;
#endif
	int log;
	uint16_t stop_address; /* Run program up to that adress and exit. */
	int tracing;
} options_t;

void
parse_command_line_options(int argc, char *argv[]);

extern struct options_t options;

#endif /* OPTIONS_H */
