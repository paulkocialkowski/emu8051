/*
 * app_config.h
 *
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

#ifndef APP_CONFIG_H
#define APP_CONFIG_H 1

/*
 *    Layout1:              Layout2:
 *
 * REGS | PROGRAM           |         | IRAM
 * --------------      REGS | PROGRAM | ----
 *     IRAM                 |         | XRAM
 * --------------
 *     XRAM
 */
enum layout_t {
	UI_LAYOUT1 = 1,
	UI_LAYOUT2,
};

struct app_config_t
{
	/* Emulation options */
	int clear_ram_on_file_load;

	/* UI settings */
	int win_width;
	int win_height;
	int hpane_pos;     /* For registers and program windows. */
	int vpane_pos; /* For internal and external memory windows. */
	int main_pane_pos; /* Between hpane and vpane. */

	/* View menu options */
	int layout; /* UI Layout 1 or 2 */
	int view_int_memory;
	int view_ext_memory;
	int bytes_per_row; /* 8 or 16 */
};

int
app_config_load(void);

int
app_config_save(void);

#endif /* APP_CONFIG_H */
