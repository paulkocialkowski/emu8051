/*
 * main.c
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

#include <stdbool.h>
#include <stdio.h>

#include "config.h"
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
		menu_prompt();
		yyparse();
	}

	return 0;
}
