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

#define BUTTONS_BORDER 2

static int running;
static int running_function_tag;
static GtkWidget *mainwin;

/* Signal DestroyEvent */
static void
WindowDestroyEvent(GtkWidget *widget, gpointer data)
{
#ifdef EMU8051_DEBUG
	g_print("emugtk_DestroyEvent(...)\n");
#endif

	gtk_main_quit();
}

/* Step out of running state */
static void
emugtk_stop_running()
{
	if (running) {
#ifdef EMU8051_DEBUG
		printf("emugtk_StopRunning()\n");
#endif
		g_source_remove(running_function_tag);
		running = 0;
		regwin_Show();
		pgmwin_Disasm();
		memwin_DumpD("00");
	}
}

/* Running function called when idle from gtk_main */
static gboolean
emugtk_running(gpointer data)
{
	cpu8051_Exec();
	if (IsBreakpoint(cpu8051.pc)) {
#ifdef EMU8051_DEBUG
		g_print("Breakpoint Hit, stopping!\n");
#endif
		emugtk_stop_running();
	}

	return TRUE;
}

/* Get in the running state */
static void
emugtk_start_running(void)
{
	if (!running) {
#ifdef EMU8051_DEBUG
		printf("emugtk_StartRunning()\n");
#endif
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
	regwin_Show();
	pgmwin_Disasm();
	memwin_DumpD("00");
}

/* Signal ResetEvent (ResetButton) */
static void
emugtk_ResetEvent(GtkWidget *widget, GdkEvent *event, gpointer data)
{
#ifdef EMU8051_DEBUG
	g_print("emugtk_ResetEvent(...)\n");
#endif
	emugtk_stop_running();
	emugtk_Reset();
}

/* CPU Step and Gtk UI update */
static void
emugtk_Step(void)
{
	cpu8051_Exec();
	regwin_Show();
	pgmwin_Disasm();
	memwin_DumpD("00");
}

/* Signal RunEvent (RunButton) */
static void
emugtk_RunEvent(GtkWidget *widget, GdkEvent *event, gpointer data)
{
#ifdef EMU8051_DEBUG
	g_print("emugtk_RunEvent(...)\n");
#endif
	if (running)
		emugtk_stop_running();
	else
		emugtk_start_running();
}

/* Signal StopEvent (StopButton) */
static void
emugtk_StopEvent(GtkWidget *widget, GdkEvent *event, gpointer data)
{
#ifdef EMU8051_DEBUG
	g_print("emugtk_StopEvent(...)\n");
#endif
	emugtk_stop_running();
}

/* Signal StepEvent (StepButton) */
static void
emugtk_StepEvent(GtkWidget *widget, GdkEvent *event, gpointer data)
{
#ifdef EMU8051_DEBUG
	g_print("emugtk_StepEvent(...)\n");
#endif
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

	gtk_widget_show_all(GTK_WIDGET(menu_bar));

	return menu_bar;
}

static void
emugtk_window_init(void)
{
	GtkWidget *main_vbox;
	GtkWidget *menu_bar;
	GtkWidget *buttons_bar;
	GtkWidget *scrollwin;
	GtkWidget *hpaned;
	GtkWidget *vpaned;

	mainwin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(mainwin), PACKAGE);
	gtk_container_set_border_width(GTK_CONTAINER(mainwin), 0);

	/* Window DESTROY event. */
	g_signal_connect(mainwin, "destroy",
			 G_CALLBACK(WindowDestroyEvent), NULL);

	/*
	 * main_vbox contains the menu bar and body_vbox (for all remaining
	 * items).
	 */
	main_vbox = gtk_vbox_new(FALSE, 1);

	/* Creating the menu bar. */
	menu_bar = AddMenu();
	/* Adding menu bar to main_vbox */
	gtk_box_pack_start(GTK_BOX(main_vbox), menu_bar, FALSE, FALSE, 1);

	/* Creating the buttons bar. */
	buttons_bar = AddButtons();
	/* Adding buttons bar to main_vbox */
	gtk_box_pack_start(GTK_BOX(main_vbox), buttons_bar, FALSE, FALSE, 1);

	/*
	 * vpaned will contain:
	 *   Top:    registers and disassembly windows.
	 *   Bottom: memory window
	 */
	vpaned = gtk_vpaned_new();

	/* hpaned will contain registers and disassembly windows. */
	hpaned = gtk_hpaned_new();

	/* 8051 registers frame. */
	scrollwin = regwin_init();
	gtk_paned_pack1(GTK_PANED(hpaned), scrollwin, FALSE, FALSE);

	/* Program disassembly frame. */
	scrollwin = pgmwin_init();
	gtk_paned_pack2(GTK_PANED(hpaned), scrollwin, TRUE, FALSE);

	gtk_paned_pack1(GTK_PANED(vpaned), hpaned, FALSE, FALSE);

	/* Memory dump frame. */
	scrollwin = memwin_init();
	gtk_paned_pack2(GTK_PANED(vpaned), scrollwin, TRUE, FALSE);

	/* Adding vpaned window to main_vbox */
	gtk_box_pack_start(GTK_BOX(main_vbox), vpaned, true, true, 1);

	/* Adding the main_vbox to the main window. */
	gtk_container_add(GTK_CONTAINER(mainwin), main_vbox);

	gtk_widget_show_all(mainwin);
}

static void
emugtk_UpdateDisplay(void)
{
#ifdef EMU8051_DEBUG
	g_print("emugtk_UpdateDisplay()\n");
#endif

	regwin_Show();
	pgmwin_Disasm();
	memwin_DumpD("00");
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

	emugtk_Reset();
	emugtk_UpdateDisplay();
}

int
main(int argc, char **argv)
{
	char *hex_file;

	ParseCommandLineOptions(argc, argv);

	cpu8051_init();

	running = 0;

	gtk_init(&argc, &argv);

	emugtk_window_init();

	hex_file = get_hex_filename();

	if (hex_file != NULL)
		emugtk_new_file(hex_file);

	gtk_main();

#ifdef EMU8051_DEBUG
	printf("End of program.\n");
#endif

	return EXIT_SUCCESS;
}
