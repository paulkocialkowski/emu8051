/*
 * main.c
 *
 * Copyright (C) 1999 Jonathan St-André
 * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This file is released under the GPLv2
 */

#include "config.h"

#include <stdbool.h>
#include <stdio.h>

#include "common.h"
#include "cpu8051.h"
#include "options.h"
#include "hexfile.h"
#include "iotrace.h"
#include "exectrace.h"
#include "serial.h"
#include "device.h"
#include "menu.h"
#include "parser.h"

extern struct options_t options;

int
main(int argc, char **argv)
{
	int rc;

	parse_command_line_options(argc, argv);

	cpu8051_init();

	if (options.filename != NULL) {
		rc = hexfile_load(options.filename);
		if (!rc)
			exit(EXIT_FAILURE);
	}

	if (options.iotrace != NULL)
		iotrace_open(options.iotrace);

	if (options.exectrace != NULL)
		exectrace_open(options.exectrace);

	if (options.serial != NULL)
		serial_open(options.serial);

	if (options.interactive)
		serial_interactive_open();

#if HAVE_DEVICE
	device_open(options.device);
#endif

	console_reset();

	if (options.stop_address != 0) {
		/* Automatically run program and stop at specified address. */
		console_exec(-1);
	} else {
		menu_display_usage();
		(void) yyparse();
	}

	if (options.iotrace != NULL)
		iotrace_close();

	if (options.exectrace != NULL)
		exectrace_close();

	if (options.serial != NULL)
		serial_close();

	if (options.interactive)
		serial_interactive_close();

#if HAVE_DEVICE
	device_close();
#endif

	exit(EXIT_SUCCESS);
}
