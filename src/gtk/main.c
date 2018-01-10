/*
 * main.c
 *
 * Copyright (C) 1999 Jonathan St-André
 * Copyright (C) 1999 Hugo Villeneuve <hugo@hugovil.com>
 *
 * This file is released under the GPLv2
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
#include "iotrace.h"
#include "exectrace.h"
#include "serial.h"
#include "device.h"
#include "gp_timers.h"
#include "main.h"
#include "reset.xpm"
#include "run.xpm"
#include "stop.xpm"
#include "step.xpm"
#include "filemenu.h"
#include "viewmenu.h"
#include "helpmenu.h"
#include "messagebox.h"
#include "regwin.h"
#include "pgmwin.h"
#include "memwin.h"
#include "pswwin.h"
#include "timerwin.h"
#include "app-config.h"

#define BUTTONS_BORDER 2

static int running;
static int running_function_tag;

static int emugtk_window_init_complete;
static GtkWidget *vpaned1;
static GtkWidget *scrollwin_int;
static GtkWidget *scrollwin_ext;

GtkWidget *mainwin;

extern struct app_config_t *cfg;
extern struct options_t options;

void
emugtk_update_display(void)
{
	log_debug("update display");
	regwin_refresh();
	pgmwin_refresh();
	pswwin_refresh();
	timerwin_update();

	if (cfg->view_int_memory && scrollwin_int)
		memwin_refresh(INT_MEM_ID);

	if (cfg->view_ext_memory && scrollwin_ext)
		memwin_refresh(EXT_MEM_ID);
}

/* Step out of running state */
static void
emugtk_stop_running()
{
	if (running) {
		log_info("stop running");
		g_source_remove(running_function_tag);
		running = 0;
		emugtk_update_display();
	}
}

/* Running function called when idle from gtk_main */
static gboolean
emugtk_running(gpointer data)
{
	int breakpoint_hit;

	(void) data; /* Remove compiler warning about unused variable. */

	breakpoint_hit = cpu8051_run(1, NULL);

	if (breakpoint_hit)
		emugtk_stop_running();

	return TRUE;
}

/* Get in the running state */
static void
emugtk_start_running(void)
{
	if (!running) {
		log_info("start running");
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
emugtk_reset(void)
{
	cpu8051_reset();
	emugtk_update_display();
}

/* Signal ResetEvent (ResetButton) */
static void
emugtk_reset_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	/* Remove compiler warning about unused variables. */
	(void) widget;
	(void) event;
	(void) data;

	log_info("ResetEvent()");
	emugtk_stop_running();
	emugtk_reset();
}

/* CPU Step and Gtk UI update */
static void
emugtk_step(void)
{
	cpu8051_exec();
	emugtk_update_display();
}

/* Signal RunEvent (RunButton) */
static void
emugtk_run_event(GtkWidget *widget, GdkEvent *event, gpointer data)
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
emugtk_stop_event(GtkWidget *widget, GdkEvent *event, gpointer data)
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
emugtk_step_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	/* Remove compiler warning about unused variables. */
	(void) widget;
	(void) event;
	(void) data;

	log_info("StepEvent()");
	emugtk_stop_running();
	emugtk_step();
}

/* Creates the Reset, Run, Stop and Step buttons. */
static GtkWidget *
add_buttons(void)
{
	GtkWidget *button_hbox;
	GtkWidget *button;

	/* The buttons of the hbox are NOT given equal space in the box. */
	button_hbox = gtk_hbox_new(FALSE, 0);

	/* Creating the RESET button. */
	button = button_add_pix(button_hbox, reset_xpm);
	g_signal_connect(button, "clicked",
			 G_CALLBACK(emugtk_reset_event),
			 NULL);

	/* Creating the RUN button. */
	button = button_add_pix(button_hbox, run_xpm);
	g_signal_connect(button, "clicked",
			 G_CALLBACK(emugtk_run_event),
			 NULL);

	/* Creating STOP button. */
	button = button_add_pix(button_hbox, stop_xpm);
	g_signal_connect(GTK_OBJECT(button), "clicked",
			 G_CALLBACK(emugtk_stop_event),
			 NULL);

	/* Creating STEP button. */
	button = button_add_pix(button_hbox, step_xpm);
	g_signal_connect(GTK_OBJECT(button), "clicked",
			 G_CALLBACK(emugtk_step_event),
			 NULL);

	return button_hbox;
}

static GtkWidget *
add_menu(void)
{
	GtkWidget *menu_bar;

	/* Creating menu item. */
	menu_bar = gtk_menu_bar_new();

	/* Adding the 'File' submenu */
	file_add_menu(menu_bar);

	/* Adding the 'View' submenu */
	view_add_menu(menu_bar);

	/* Adding the 'Help' submenu */
	help_add_menu(menu_bar);

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
	cfg->win_pos_x = event->configure.x;
	cfg->win_pos_y = event->configure.y;

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
emugtk_quit_gui(void)
{
	gtk_main_quit();
}

static void
emugtk_show_memory_paned(void)
{
	gtk_widget_show_all(mainwin);
	emugtk_update_display();
}

void
emugtk_create_int_memory_paned(void)
{
	scrollwin_int = memwin_init("Internal memory (IRAM)",
				    INT_MEM_ID);
	gtk_paned_pack1(GTK_PANED(vpaned1), scrollwin_int,
			FALSE, FALSE);
	if (emugtk_window_init_complete)
		emugtk_show_memory_paned();
}

void
emugtk_destroy_int_memory_paned(void)
{
	if (scrollwin_int == NULL)
		return;

	gtk_widget_destroy(scrollwin_int);
	scrollwin_int = NULL;
}

void
emugtk_create_ext_memory_paned(void)
{
	scrollwin_ext = memwin_init("External memory (XRAM)",
				    EXT_MEM_ID);

	gtk_paned_pack2(GTK_PANED(vpaned1), scrollwin_ext,
			TRUE, FALSE);

	if (emugtk_window_init_complete)
		emugtk_show_memory_paned();
}

void
emugtk_destroy_ext_memory_paned(void)
{
	if (scrollwin_ext == NULL)
		return;

	gtk_widget_destroy(scrollwin_ext);
	scrollwin_ext = NULL;
}

void
emugtk_recreate_memory_paned(void)
{
	if (cfg->view_int_memory) {
		emugtk_destroy_int_memory_paned();
		emugtk_create_int_memory_paned();
	}

	if (cfg->view_ext_memory) {
		emugtk_destroy_ext_memory_paned();
		emugtk_create_ext_memory_paned();
	}

	if (emugtk_window_init_complete)
		emugtk_show_memory_paned();
}

static GtkWidget *
emugtk_create_memory_paned(void)
{
	/* Create vpaned (memory windows) only if necessary. */
	if (cfg->view_int_memory || cfg->view_ext_memory) {
		vpaned1 = gtk_vpaned_new();

		gtk_paned_set_position(GTK_PANED(vpaned1), cfg->vpane_pos);
		g_signal_connect(G_OBJECT(vpaned1), "notify::position",
				 G_CALLBACK(vpaned_notify_event), vpaned1);

		emugtk_recreate_memory_paned();

		return vpaned1;
	} else {
		return NULL;
	}
}

static void
emugtk_set_geometry_hints(GtkWidget *window)
{
	GdkGeometry hints;

        hints.min_width = 100;
        hints.min_height = 100;

	/* Set reference point to top left corner */
	hints.win_gravity = GDK_GRAVITY_NORTH_WEST;

	gtk_window_set_geometry_hints(GTK_WINDOW(window), window, &hints,
				      GDK_HINT_MIN_SIZE);
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
	int geometry_ok = false;
	int id;
	GtkWidget *vbox;
	GtkWidget *menu_bar;
	GtkWidget *buttons_bar;
	GtkWidget *scrollwin;
	GtkWidget *hpaned;
	GtkWidget *vpaned;
	GtkWidget *main_paned;

	emugtk_window_init_complete = false;

	mainwin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(mainwin), PACKAGE_NAME);
	gtk_container_set_border_width(GTK_CONTAINER(mainwin), 0);

	/* Window DESTROY event. */
	g_signal_connect(mainwin, "destroy",
			 G_CALLBACK(gtk_main_quit), NULL);

	g_signal_connect(G_OBJECT(mainwin), "configure-event",
			 G_CALLBACK(mainwin_configure_event), NULL);

	/* Creating the menu bar. */
	menu_bar = add_menu();

	/* Creating the buttons bar. */
	buttons_bar = add_buttons();

	scrollwin = pswwin_init();
	gtk_box_pack_start(GTK_BOX(buttons_bar), scrollwin, FALSE, FALSE, 50);

	for (id = 0; id < GP_TIMERS_COUNT; id++) {
		scrollwin = timerwin_init(id);
		gtk_box_pack_start(GTK_BOX(buttons_bar), scrollwin,
				   FALSE, FALSE, 15);
	}

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

	emugtk_set_geometry_hints(mainwin);

	/*
	 * If either a size or a position can be extracted from the geometry
	 * string, gtk_window_parse_geometry() returns TRUE and calls
	 * gtk_window_set_default_size() and/or gtk_window_move() to resize/move
	 * the window.
	 */
	if (options.g != NULL) {
		geometry_ok = gtk_window_parse_geometry(GTK_WINDOW(mainwin),
							options.g);
		if (!geometry_ok)
			log_err("Failed to parse geometry argument: %s",
				options.g);
	}

	/*
	 * If geometry was not specified, or was improperly parsed, use
	 * saved window size.
	 */
	if (geometry_ok == false) {
		log_err("Use saved window size");
		gtk_window_set_default_size(GTK_WINDOW(mainwin),
					    cfg->win_width, cfg->win_height);
		gtk_window_move(GTK_WINDOW(mainwin),
				cfg->win_pos_x, cfg->win_pos_y);
	}

	gtk_widget_show_all(mainwin);

	emugtk_window_init_complete = true;
}

void
add_menu_separator(GtkWidget *menu)
{
	GtkWidget *item;

	item = gtk_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
}

void
emugtk_new_file(char *file)
{
	int rc;

	emugtk_stop_running();

	rc = hexfile_load(file);
	if (!rc) {
		message_show_error("Error parsing hex file");
	} else {
		if (cfg->clear_ram_on_file_load)
			emugtk_reset();

		emugtk_update_display();
	}
}

int
main(int argc, char **argv)
{
	int rc_load_hexfile = true;

	parse_command_line_options(argc, argv);
	app_config_load();

	cpu8051_init();

	running = 0;

	gtk_init(&argc, &argv);

	if (options.filename != NULL)
		rc_load_hexfile = hexfile_load(options.filename);

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

	cpu8051_reset();

	log_info("Init GUI");
	emugtk_window_init();
	emugtk_update_display();

	if (!rc_load_hexfile)
		message_show_error("Error parsing hex file");

	gtk_main();

	log_info("Terminate");

	app_config_save();

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

	return EXIT_SUCCESS;
}
