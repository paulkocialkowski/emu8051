/* viewmenu.c */


#if HAVE_CONFIG_H
#  include "config.h"
#endif

#include <gtk/gtk.h>

#include "common.h"
#include "emugtk.h" /* For AddMenuSeparator() function. */
#include "messagebox.h"
#include "viewmenu.h"


static void
ViewMenuExternalDump( gchar *string )
{
  ShowMessage( "External Memory Dump", "Not implemented yet!", GTK_JUSTIFY_CENTER,
	       MESSAGE_DIALOG_NORMAL_FONT );
}


static void
ViewMenuInternalDump( gchar *string )
{
  ShowMessage( "Internal Memory Dump", "Not implemented yet!", GTK_JUSTIFY_CENTER,
	       MESSAGE_DIALOG_NORMAL_FONT );
}


void
ViewAddMenu( GtkWidget *menu_bar )
{
  GtkWidget *item;
  GtkWidget *menu;
  
  menu = gtk_menu_new();

  /* Create the 'Viewmenu External Memory Dump' item. */
  item = gtk_menu_item_new_with_label("External Memory Dump");
  gtk_menu_append( GTK_MENU(menu), item );
  /* Attach the callback functions to the activate signal. */
  gtk_signal_connect_object( GTK_OBJECT(item), "activate",
			     GTK_SIGNAL_FUNC(ViewMenuExternalDump),
			     NULL );

  AddMenuSeparator(menu);

  /* Create the 'Viewmenu Internal Memory Dump' item. */
  item = gtk_menu_item_new_with_label( "Internal Memory Dump" );
  gtk_menu_append( GTK_MENU(menu), item );
  /* Attach the callback functions to the activate signal. */
  gtk_signal_connect_object( GTK_OBJECT(item), "activate",
			     GTK_SIGNAL_FUNC(ViewMenuInternalDump),
			     NULL );
  
  /* Adding submenu title. */
  item = gtk_menu_item_new_with_label( "View" );
  gtk_menu_item_set_submenu( GTK_MENU_ITEM(item), menu );
  gtk_menu_bar_append( GTK_MENU_BAR( menu_bar ), item );
}
