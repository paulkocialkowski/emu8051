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

static int RunningState;
static int RunFuncTag;
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

/* Taken from the Gxine source code. */
static GtkWidget *
AddPixButton(GtkWidget *box, gchar **pixmap_array)
{
	GtkWidget *button, *icon;
	GdkPixmap *image;
	GdkBitmap *transparent;

	button = gtk_button_new();
	gtk_button_set_relief(GTK_BUTTON(button), GTK_RELIEF_NORMAL);
	image = gdk_pixmap_colormap_create_from_xpm_d(
		NULL, gdk_colormap_get_system(),
		&transparent, NULL, pixmap_array);
	icon = gtk_pixmap_new(image, transparent);
	gtk_container_add(GTK_CONTAINER(button), icon);

	gtk_box_pack_start(GTK_BOX(box), button, FALSE, FALSE, 0);

	return button;
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
	button = AddPixButton(button_hbox, reset_xpm);
	gtk_signal_connect(GTK_OBJECT(button), "clicked",
			   GTK_SIGNAL_FUNC(emugtk_ResetEvent),
			   NULL);

	/* Creating the RUN button. */
	button = AddPixButton(button_hbox, run_xpm);
	gtk_signal_connect(GTK_OBJECT(button), "clicked",
			   GTK_SIGNAL_FUNC(emugtk_RunEvent),
			   NULL);

	/* Creating STOP button. */
	button = AddPixButton(button_hbox, stop_xpm);
	gtk_signal_connect(GTK_OBJECT(button), "clicked",
			   GTK_SIGNAL_FUNC(emugtk_StopEvent),
			   NULL);

	/* Creating STEP button. */
	button = AddPixButton(button_hbox, step_xpm);
	gtk_signal_connect(GTK_OBJECT(button), "clicked",
			   GTK_SIGNAL_FUNC(emugtk_StepEvent),
			   NULL);

	return button_hbox;
}

GtkWidget *
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
	GtkWidget *emufixed;
	GtkWidget *fixed_frame;

	mainwin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(mainwin), PACKAGE);
	gtk_widget_set_usize(GTK_WIDGET(mainwin),
			     MAIN_WIN_WIDTH, MAIN_WIN_HEIGHT);
	gtk_container_set_border_width(GTK_CONTAINER(mainwin), 0);

	/* Window DESTROY event. */
	gtk_signal_connect(GTK_OBJECT(mainwin), "destroy",
			   GTK_SIGNAL_FUNC(WindowDestroyEvent), NULL);

	/*
	 * Setting main window geometry based on command line options
	 * (if specified).
	*/
	/*MainWindowSetGeometry();*/

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

	/* Emulator fixed window. */
	emufixed = gtk_fixed_new();
	gtk_widget_set_usize(GTK_WIDGET(emufixed), MAIN_WIN_WIDTH,
			     REG_WIN_HEIGHT + MEM_WIN_HEIGHT + 10);

	/* 8051 registers frame. */
	fixed_frame = regwin_init(REG_WIN_WIDTH, REG_WIN_HEIGHT);
	gtk_fixed_put(GTK_FIXED(emufixed), fixed_frame, 0, 0);

	/* Program disassembly frame. */
	fixed_frame = pgmwin_init(PGM_WIN_WIDTH, PGM_WIN_HEIGHT);
	gtk_fixed_put(GTK_FIXED(emufixed), fixed_frame, REG_WIN_WIDTH + 10, 0);

	/* Memory dump frame. */
	fixed_frame = memwin_init(MEM_WIN_WIDTH, MEM_WIN_HEIGHT);
	gtk_fixed_put(GTK_FIXED(emufixed), fixed_frame, 0, REG_WIN_HEIGHT);

	/* Adding fixed window to main_vbox */
	gtk_box_pack_start(GTK_BOX(main_vbox), emufixed, FALSE, FALSE, 1);

	/* Adding the main_vbox to the main window. */
	gtk_container_add(GTK_CONTAINER(mainwin), main_vbox);

	gtk_widget_show_all(mainwin);
}

void
emugtk_new_file(char *file)
{
	emugtk_StopRunning();

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

	RunningState = 0;

	gtk_init(&argc, &argv);

	emugtk_window_init();

	hex_file = get_hex_filename();

	if (hex_file != NULL)
		emugtk_new_file(hex_file);

	gtk_main();

	printf("End of program.\n");

	return EXIT_SUCCESS;
}

void
AddMenuSeparator(GtkWidget *menu)
{
	GtkWidget *item;

	item = gtk_menu_item_new();
	gtk_menu_append(GTK_MENU(menu), item);
}

void
emugtk_UpdateDisplay(void)
{
#ifdef EMU8051_DEBUG
	g_print("emugtk_UpdateDisplay()\n");
#endif

	regwin_Show();
	pgmwin_Disasm();
	memwin_DumpD("0x00");
}

/* CPU reset and Gtk UI update */
void
emugtk_Reset(void)
{
	cpu8051_Reset();
	regwin_Show();
	pgmwin_Disasm();
	memwin_DumpD("0x00");
}

/* CPU Step and Gtk UI update */
void
emugtk_Step(void)
{
	cpu8051_Exec();
	regwin_Show();
	pgmwin_Disasm();
	memwin_DumpD("0x00");
}

/* Signal ResetEvent (ResetButton) */
void
emugtk_ResetEvent(GtkWidget *widget, GdkEvent *event, gpointer data)
{
#ifdef EMU8051_DEBUG
	g_print("emugtk_ResetEvent(...)\n");
#endif
	emugtk_StopRunning();
	emugtk_Reset();
}

/* Signal RunEvent (RunButton) */
void
emugtk_RunEvent(GtkWidget *widget, GdkEvent *event, gpointer data)
{
#ifdef EMU8051_DEBUG
	g_print("emugtk_RunEvent(...)\n");
#endif
	if (RunningState)
		emugtk_StopRunning();
	else
		emugtk_StartRunning();
}

/* Signal StopEvent (StopButton) */
void
emugtk_StopEvent(GtkWidget *widget, GdkEvent *event, gpointer data)
{
#ifdef EMU8051_DEBUG
	g_print("emugtk_StopEvent(...)\n");
#endif
	emugtk_StopRunning();
}

/* Signal StepEvent (StepButton) */
void
emugtk_StepEvent(GtkWidget *widget, GdkEvent *event, gpointer data)
{
#ifdef EMU8051_DEBUG
	g_print("emugtk_StepEvent(...)\n");
#endif
	emugtk_StopRunning();
	emugtk_Step();
}

/* Running called by RunningFunction() */
void
emugtk_Running()
{
	cpu8051_Exec();
	if (IsBreakpoint(cpu8051.pc)) {
#ifdef EMU8051_DEBUG
		g_print("Breakpoint Hit, stopping!\n");
#endif
		emugtk_StopRunning();
	}
}

/* RunningFunction called when idle from gtk_main */
gboolean
RunningFunction(gpointer data)
{
	emugtk_Running();
	return TRUE;
}

/* Get in the RunningState */
void
emugtk_StartRunning(void)
{
	if (!RunningState) {
#ifdef EMU8051_DEBUG
		printf("emugtk_StartRunning()\n");
#endif
		RunFuncTag = gtk_idle_add(RunningFunction, 0);

		RunningState = 1;
	}
}

/* Step out of RunningState */
void
emugtk_StopRunning()
{
	if (RunningState) {
#ifdef EMU8051_DEBUG
		printf("emugtk_StopRunning()\n");
#endif
		gtk_idle_remove(RunFuncTag);
		RunningState = 0;
		regwin_Show();
		pgmwin_Disasm();
		memwin_DumpD("0x00");
	}
}
