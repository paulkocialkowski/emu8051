/*
 * app_config.h
 *
 * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This file is released under the GPLv2
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

struct app_config_t {
	/* Emulation options */
	int clear_ram_on_file_load;

	/* UI settings */
	int win_width;
	int win_height;
	int win_pos_x;
	int win_pos_y;
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
