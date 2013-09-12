/*
 * Handle loading and saving of application configuration settings
 *
 * Copyright (C) 2013 Hugo Villeneuve <hugo@hugovil.com>
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

#if HAVE_CONFIG_H
#  include "config.h"
#endif

#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#if STDC_HEADERS
#  include <string.h>
#elif HAVE_STRINGS_H
#  include <strings.h>
#endif

#include <glib.h>

#include "common.h"
#include "app-config.h"

static const char profile_name[] = "default";
static struct app_config_t app_config;

struct app_config_t *cfg = &app_config;

static void
app_config_init(void)
{
	/* Emulation options */
	cfg->clear_ram_on_file_load = false;

	/* UI settings */
	cfg->win_width = 640;
	cfg->win_height = 480;
	cfg->hpane_pos = 100;
	cfg->vpane_pos = 200;
}

static int
app_config_key_file_get_int(GKeyFile *kf, const char *grp, const char *key, int *value)
{
    char *str = g_key_file_get_value(kf, grp, key, NULL);

#ifdef EMU8051_DEBUG
    printf("key: %s\n", key);
#endif

    if (G_LIKELY(str)) {
	    *value = atoi(str);

#ifdef EMU8051_DEBUG
	    printf("  value = %d\n", *value);
#endif

	    g_free(str);
    }

    return str != NULL;
}

static void
app_config_load_from_key_file(GKeyFile *kf)
{
	/* Emulation options */
	app_config_key_file_get_int(kf, "emulation", "clear_ram_on_file_load",
				    &cfg->clear_ram_on_file_load);

	/* ui */
	app_config_key_file_get_int(kf, "ui", "win_width",  &cfg->win_width);
	app_config_key_file_get_int(kf, "ui", "win_height", &cfg->win_height);
	app_config_key_file_get_int(kf, "ui", "hpane_pos",  &cfg->hpane_pos);
	app_config_key_file_get_int(kf, "ui", "vpane_pos",  &cfg->vpane_pos);
}

static char *
app_config_get_dir_path(void)
{
	char *dir_path;

	dir_path = g_build_filename(g_get_user_config_dir(), PACKAGE,
				    profile_name, NULL);

	return dir_path;
}

static char *
app_config_get_file_path(void)
{
	char *file_path;
	char *dir_path;
	char file[MAX_FILENAME_LENGTH];

	sprintf(file, "%s.conf", PACKAGE);

	dir_path = app_config_get_dir_path();

	file_path = g_build_filename(dir_path, file, NULL);

#ifdef EMU8051_DEBUG
	printf("app. config file = %s\n", file_path);
#endif

	g_free(dir_path);

	return file_path;
}

int
app_config_load(void)
{
	char *file_path;
	GKeyFile *kf;

	/* Load default values before config file */
	app_config_init();

	kf = g_key_file_new();

	file_path = app_config_get_file_path();

	if (g_key_file_load_from_file(kf, file_path, 0, NULL))
		app_config_load_from_key_file(kf);

	g_free(file_path);

	g_key_file_free(kf);

	/* ??? */
	return 0;
}

int
app_config_save(void)
{
	char *dir_path;

	dir_path = app_config_get_dir_path();

	if (g_mkdir_with_parents(dir_path, 0700) != -1)
	{
		char *file_path;
		GString* buf = g_string_sized_new(1024);

		g_string_append(buf, "\n[emulation]\n");

		g_string_append_printf(buf, "clear_ram_on_file_load=%d\n",
				       cfg->clear_ram_on_file_load);

		g_string_append(buf, "\n[ui]\n");

		g_string_append_printf(buf, "win_width=%d\n", cfg->win_width);
		g_string_append_printf(buf, "win_height=%d\n", cfg->win_height);
		g_string_append_printf(buf, "hpane_pos=%d\n", cfg->hpane_pos);
		g_string_append_printf(buf, "vpane_pos=%d\n", cfg->vpane_pos);

		file_path = app_config_get_file_path();

		g_file_set_contents(file_path, buf->str, buf->len, NULL);
		g_string_free(buf, TRUE);
		g_free(file_path);
	}

	g_free(dir_path);

	/* ??? */
	return 0;
}