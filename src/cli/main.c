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
		rc = LoadHexFile(options.filename);
		if (rc == false)
			exit(1);
	}

	console_reset();

	if (options.stop_address != 0) {
		/* Automatically run program and stop at specified address. */
		console_exec(-1);
	} else {
		menu_display_usage();
		console_show_registers();
		yyparse();
	}

	return 0;
}
