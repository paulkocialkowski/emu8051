/* help.c */


#if HAVE_CONFIG_H
#  include "config.h"
#endif

#if STDC_HEADERS
#  include <string.h>
#elif HAVE_STRINGS_H
#  include <strings.h>
#endif

#include <gtk/gtk.h>

#include "common.h"
#include "options.h"
#include "emugtk.h"
#include "messagebox.h"
#include "helpmenu.h"


static void
HelpCommandsEvent( gchar *string )
{
  ShowMessage( "Command Line Options", COMMAND_LINE_OPTIONS, GTK_JUSTIFY_LEFT,
	       MESSAGE_DIALOG_FIXED_FONT );
}


static void
HelpAboutEvent( gchar *string )
{
  ShowMessage( "About", VERSION_STRING, GTK_JUSTIFY_CENTER, MESSAGE_DIALOG_NORMAL_FONT );
}


void
HelpAddMenu( GtkWidget *menu_bar )
{
  GtkWidget *item;
  GtkWidget *menu;
  
  menu = gtk_menu_new();

  /* Create the 'Help Command Line Options' item. */
  item = gtk_menu_item_new_with_label("Command Line Options");
  gtk_menu_append( GTK_MENU(menu), item );
  /* Attach the callback functions to the activate signal. */
  gtk_signal_connect_object( GTK_OBJECT(item), "activate",
			     GTK_SIGNAL_FUNC(HelpCommandsEvent),
			     NULL );

  AddMenuSeparator(menu);

  /* Create the 'Help About' item. */
  item = gtk_menu_item_new_with_label( "About " PACKAGE );
  gtk_menu_append( GTK_MENU(menu), item );
  /* Attach the callback functions to the activate signal. */
  gtk_signal_connect_object( GTK_OBJECT(item), "activate",
			     GTK_SIGNAL_FUNC(HelpAboutEvent),
			     NULL );
  
  /* Adding submenu title. */
  item = gtk_menu_item_new_with_label( "Help" );
  gtk_menu_item_set_submenu( GTK_MENU_ITEM(item), menu );
  gtk_menu_bar_append( GTK_MENU_BAR( menu_bar ), item );
}
