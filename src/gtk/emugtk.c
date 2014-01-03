/*
 * emugtk.c
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

#include <stdio.h>
#include "config.h"

#include <stdint.h>
#include <stdbool.h>

#include <gtk/gtk.h>

#include "common.h"
#include "cpu8051.h"
#include "memory.h"
#include "options.h"
#include "hexfile.h"

#include "emugtk.h"
#include "reset.xpm"
#include "run.xpm"
#include "stop.xpm"
#include "step.xpm"
#include "filemenu.h"
#include "viewmenu.h"
#include "helpmenu.h"
#include "regwin.h"
#include "pgmwin.h"
#include "memwin.h"
#include "pswwin.h"
#include "app-config.h"

#define BUTTONS_BORDER 2

static int running;
static int running_function_tag;
static int restart_gui = true;

GtkWidget *mainwin;

extern struct app_config_t *cfg;
extern struct options_t options;

void
emugtk_UpdateDisplay(void)
{
	log_debug("UpdateDisplay()");
	regwin_refresh();
	pgmwin_refresh();
	pswwin_refresh();

	if (cfg->view_int_memory)
		memwin_refresh(INT_MEM_ID);

	if (cfg->view_ext_memory)
		memwin_refresh(EXT_MEM_ID);
}

/* Step out of running state */
static void
emugtk_stop_running()
{
	if (running) {
		log_info("StopRunning()");
		g_source_remove(running_function_tag);
		running = 0;
		emugtk_UpdateDisplay();
	}
}

/* Running function called when idle from gtk_main */
static gboolean
emugtk_running(gpointer data)
{
	(void) data; /* Remove compiler warning about unused variable. */

	cpu8051_Exec();
	if (IsBreakpoint(cpu8051.pc)) {
		log_info("Breakpoint Hit");
		emugtk_stop_running();
	}

	return TRUE;
}

/* Get in the running state */
static void
emugtk_start_running(void)
{
	if (!running) {
		log_info("StartRunning()");
		running_function_tag = g_idle_add(emugtk_running, 0);
		running = 1;
	}
}

/* Taken from the Gxine source code. */
static GtkWidget *
button_add_pix(GtkWidget *box, char **xpm)
{
	GtkWidget *button, *icon;

	button = gtk_button_new();
	gtk_button_set_relief(GTK_BUTTON(button), GTK_RELIEF_NORMAL);

	icon = gtk_image_new_from_pixbuf(
		gdk_pixbuf_new_from_xpm_data((const char **) xpm));
	gtk_container_add(GTK_CONTAINER(button), icon);

	gtk_box_pack_start(GTK_BOX(box), button, FALSE, FALSE, BUTTONS_BORDER);

	return button;
}

/* CPU reset and Gtk UI update */
static void
emugtk_Reset(void)
{
	cpu8051_Reset();
	emugtk_UpdateDisplay();
}

/* Signal ResetEvent (ResetButton) */
static void
emugtk_ResetEvent(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	/* Remove compiler warning about unused variables. */
	(void) widget;
	(void) event;
	(void) data;

	log_info("ResetEvent()");
	emugtk_stop_running();
	emugtk_Reset();
}

/* CPU Step and Gtk UI update */
static void
emugtk_Step(void)
{
	cpu8051_Exec();
	emugtk_UpdateDisplay();
}

/* Signal RunEvent (RunButton) */
static void
emugtk_RunEvent(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	/* Remove compiler warning about unused variables. */
	(void) widget;
	(void) event;
	(void) data;

	log_info("RunEvent()");

	if (running)
		emugtk_stop_running();
	else
		emugtk_start_running();
}

/* Signal StopEvent (StopButton) */
static void
emugtk_StopEvent(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	/* Remove compiler warning about unused variables. */
	(void) widget;
	(void) event;
	(void) data;

	log_info("StopEvent()");
	emugtk_stop_running();
}

/* Signal StepEvent (StepButton) */
static void
emugtk_StepEvent(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	/* Remove compiler warning about unused variables. */
	(void) widget;
	(void) event;
	(void) data;

	log_info("StepEvent()");
	emugtk_stop_running();
	emugtk_Step();
}

/* Creates the Reset, Run, Stop and Step buttons. */
static GtkWidget *
AddButtons(void)
{
	GtkWidget *button_hbox;
	GtkWidget *button;

	/* The buttons of the hbox are NOT given equal space in the box. */
	button_hbox = gtk_hbox_new(FALSE, 0);

	/* Creating the RESET button. */
	button = button_add_pix(button_hbox, reset_xpm);
	g_signal_connect(button, "clicked",
			 G_CALLBACK(emugtk_ResetEvent),
			 NULL);

	/* Creating the RUN button. */
	button = button_add_pix(button_hbox, run_xpm);
	g_signal_connect(button, "clicked",
			 G_CALLBACK(emugtk_RunEvent),
			 NULL);

	/* Creating STOP button. */
	button = button_add_pix(button_hbox, stop_xpm);
	g_signal_connect(GTK_OBJECT(button), "clicked",
			 G_CALLBACK(emugtk_StopEvent),
			 NULL);

	/* Creating STEP button. */
	button = button_add_pix(button_hbox, step_xpm);
	g_signal_connect(GTK_OBJECT(button), "clicked",
			 G_CALLBACK(emugtk_StepEvent),
			 NULL);

	return button_hbox;
}

static GtkWidget *
AddMenu(void)
{
	GtkWidget *menu_bar;

	/* Creating menu item. */
	menu_bar = gtk_menu_bar_new();

	/* Adding the 'File' submenu */
	FileAddMenu(menu_bar);

	/* Adding the 'View' submenu */
	ViewAddMenu(menu_bar);

	/* Adding the 'Help' submenu */
	HelpAddMenu(menu_bar);

	return menu_bar;
}

static int
mainwin_configure_event(GtkWindow *window, GdkEvent *event, gpointer data)
{
	/* Remove compiler warning about unused variables. */
	(void) window;
	(void) data;

	cfg->win_width = event->configure.width;
	cfg->win_height = event->configure.height;

	/*
	 * Important:
	 * Returning false allows event to propagate to children. If not, they
	 * will not be resized when we resize the main window.
	 */
	return FALSE;
}

static void
hpaned_notify_event(GtkWindow *window, GdkEvent *event, gpointer data)
{
	GtkWidget *paned = data;

	/* Remove compiler warning about unused variables. */
	(void) window;
	(void) event;

	cfg->hpane_pos = gtk_paned_get_position(GTK_PANED(paned));
}

static void
vpaned_notify_event(GtkWindow *window, GdkEvent *event, gpointer data)
{
	GtkWidget *paned = data;

	/* Remove compiler warning about unused variables. */
	(void) window;
	(void) event;

	cfg->vpane_pos = gtk_paned_get_position(GTK_PANED(paned));
}

static void
main_paned_notify_event(GtkWindow *window, GdkEvent *event, gpointer data)
{
	GtkWidget *paned = data;

	/* Remove compiler warning about unused variables. */
	(void) window;
	(void) event;

	cfg->main_pane_pos = gtk_paned_get_position(GTK_PANED(paned));
}

void
emugtk_restart_gui(void)
{
	emugtk_stop_running();

	gtk_widget_destroy(mainwin);

	restart_gui = true;
}

void
emugtk_quit_gui(void)
{
	gtk_main_quit();

	restart_gui = false;
}

static GtkWidget *
emugtk_create_memory_paned(void)
{
	GtkWidget *vpaned;
	GtkWidget *scrollwin;

	/* Create vpaned (memory windows) only if necessary. */
	if (cfg->view_int_memory || cfg->view_ext_memory) {
		vpaned = gtk_vpaned_new();
		gtk_paned_set_position(GTK_PANED(vpaned), cfg->vpane_pos);
		g_signal_connect(G_OBJECT(vpaned), "notify::position",
				 G_CALLBACK(vpaned_notify_event), vpaned);

		/* Internal memory dump frame. */
		if (cfg->view_int_memory) {
			scrollwin = memwin_init("Internal memory (IRAM)",
						INT_MEM_ID);
			gtk_paned_pack1(GTK_PANED(vpaned), scrollwin,
					FALSE, FALSE);
		}

		/* External memory dump frame. */
		if (cfg->view_ext_memory) {
			scrollwin = memwin_init("External memory (XRAM)",
						EXT_MEM_ID);
			gtk_paned_pack2(GTK_PANED(vpaned), scrollwin,
					TRUE, FALSE);
		}

		return vpaned;
	} else
		return NULL;
}

/*
 *  mainwin
 * +---------------------------------------------------------------------+
 * |                                                                     |
 * |  vbox                                                               |
 * |  +---------------------------------------------------------------+  |
 * |  |                                                               |  |
 * |  |  menu_bar                                                     |  |
 * |  |  +----------------------+                                     |  |
 * |  |  | File  View  Help     |                                     |  |
 * |  |  +----------------------+                                     |  |
 * |  |                                                               |  |
 * |  |---------------------------------------------------------------|  |
 * |  |                                                               |  |
 * |  |  buttons_bar                                                  |  |
 * |  |  +-----------------------+                                    |  |
 * |  |  | RST  RUN  STOP  STEP  |                                    |  |
 * |  |  +-----------------------+                                    |  |
 * |  |                                                               |  |
 * |  |---------------------------------------------------------------|  |
 * |  |                                                               |  |
 * |  |  main_paned                                                   |  |
 * |  |  +---------------------------------------------------------+  |  |
 * |  |  |                                                         |  |  |
 * |  |  |  hpaned                                                 |  |  |
 * |  |  |  +---------------------------------------------------+  |  |  |
 * |  |  |  |                        |                          |  |  |  |
 * |  |  |  |  scrollwin             |  scrollwin               |  |  |  |
 * |  |  |  |  +------------------+  *  +--------------------+  |  |  |  |
 * |  |  |  |  | REGISTERS window |  *  | Disassembly window |  |  |  |  |
 * |  |  |  |  +------------------+  |  +--------------------+  |  |  |  |
 * |  |  |  |                        |                          |  |  |  |
 * |  |  |  +---------------------------------------------------+  |  |  |
 * |  |  |                                                         |  |  |
 * |  |  |--------------------------***-----------------------------  |  |
 * |  |  |                                                         |  |  |
 * |  |  |  vpaned                                                 |  |  |
 * |  |  |  +---------------------------------------------------+  |  |  |
 * |  |  |  |                                                   |  |  |  |
 * |  |  |  |  scrollwin                                        |  |  |  |
 * |  |  |  |  +---------------------------------------------+  |  |  |  |
 * |  |  |  |  | Internal memory window                      |  |  |  |  |
 * |  |  |  |  +---------------------------------------------+  |  |  |  |
 * |  |  |  |                                                   |  |  |  |
 * |  |  |  +-----------------------***-------------------------|  |  |  |
 * |  |  |  |                                                   |  |  |  |
 * |  |  |  |  scrollwin                                        |  |  |  |
 * |  |  |  |  +---------------------------------------------+  |  |  |  |
 * |  |  |  |  | External memory window                      |  |  |  |  |
 * |  |  |  |  +---------------------------------------------+  |  |  |  |
 * |  |  |  |                                                   |  |  |  |
 * |  |  |  +---------------------------------------------------+  |  |  |
 * |  |  |                                                         |  |  |
 * |  |  +---------------------------------------------------------+  |  |
 * |  |                                                               |  |
 * |  |                                                               |  |
 * |  +---------------------------------------------------------------+  |
 * |                                                                     |
 * |                                                                     |
 * +---------------------------------------------------------------------+
 */
static void
emugtk_window_init(void)
{
	GtkWidget *vbox;
	GtkWidget *menu_bar;
	GtkWidget *buttons_bar;
	GtkWidget *scrollwin;
	GtkWidget *hpaned;
	GtkWidget *vpaned;
	GtkWidget *main_paned;

	mainwin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(mainwin), PACKAGE);
	gtk_window_set_default_size(GTK_WINDOW(mainwin),
				    cfg->win_width, cfg->win_height);
	gtk_container_set_border_width(GTK_CONTAINER(mainwin), 0);

	/* Window DESTROY event. */
	g_signal_connect(mainwin, "destroy",
			 G_CALLBACK(gtk_main_quit), NULL);

	g_signal_connect(G_OBJECT(mainwin), "configure-event",
			 G_CALLBACK(mainwin_configure_event), NULL);

	/* Creating the menu bar. */
	menu_bar = AddMenu();

	/* Creating the buttons bar. */
	buttons_bar = AddButtons();

	scrollwin = pswwin_init();
	gtk_box_pack_start(GTK_BOX(buttons_bar), scrollwin, FALSE, FALSE, 100);

	/* hpaned will contain registers and disassembly windows. */
	hpaned = gtk_hpaned_new();
	gtk_paned_set_position(GTK_PANED(hpaned), cfg->hpane_pos);
	g_signal_connect(G_OBJECT(hpaned), "notify::position",
			 G_CALLBACK(hpaned_notify_event), hpaned);

	/* 8051 registers frame. */
	scrollwin = regwin_init();
	gtk_paned_pack1(GTK_PANED(hpaned), scrollwin, FALSE, FALSE);

	/* Program disassembly frame. */
	scrollwin = pgmwin_init();
	gtk_paned_pack2(GTK_PANED(hpaned), scrollwin, TRUE, FALSE);

	/*
	 * main_paned will contain two groups:
	 *   group1:    registers and disassembly windows.
	 *   group2:    memory windows
	 */
	if (cfg->layout == UI_LAYOUT1)
		main_paned = gtk_vpaned_new();
	else
		main_paned = gtk_hpaned_new();

	gtk_paned_set_position(GTK_PANED(main_paned), cfg->main_pane_pos);
	g_signal_connect(G_OBJECT(main_paned), "notify::position",
			 G_CALLBACK(main_paned_notify_event), main_paned);
	gtk_paned_pack1(GTK_PANED(main_paned), hpaned, FALSE, FALSE);

	vpaned = emugtk_create_memory_paned();
	if (vpaned != NULL)
		gtk_paned_pack2(GTK_PANED(main_paned), vpaned,
				TRUE, FALSE);

	/*
	 * vbox contains the menu bar and body_vbox (for all remaining
	 * items).
	 */
	vbox = gtk_vbox_new(FALSE, 1);
	gtk_box_pack_start(GTK_BOX(vbox), menu_bar, FALSE, FALSE, 1);
	gtk_box_pack_start(GTK_BOX(vbox), buttons_bar, FALSE, FALSE, 1);
	gtk_box_pack_start(GTK_BOX(vbox), main_paned, true, true, 1);

	/* Adding the vbox to the main window. */
	gtk_container_add(GTK_CONTAINER(mainwin), vbox);

	g_signal_connect(mainwin, "destroy", G_CALLBACK(emugtk_quit_gui), NULL);

	gtk_widget_show_all(mainwin);
}

void
AddMenuSeparator(GtkWidget *menu)
{
	GtkWidget *item;

	item = gtk_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
}

void
emugtk_new_file(char *file)
{
	emugtk_stop_running();

	LoadHexFile(file);

	if (cfg->clear_ram_on_file_load)
		emugtk_Reset();

	emugtk_UpdateDisplay();
}

int
main(int argc, char **argv)
{
	parse_command_line_options(argc, argv);
	app_config_load();

	cpu8051_init();

	running = 0;

	gtk_init(&argc, &argv);

	if (options.filename != NULL)
		LoadHexFile(options.filename);

	cpu8051_Reset();

	while (restart_gui == true) {
		log_info("Init GUI");

		emugtk_window_init();
		emugtk_UpdateDisplay();
		gtk_main();
	}

	log_info("Terminate");

	app_config_save();

	return EXIT_SUCCESS;
}
