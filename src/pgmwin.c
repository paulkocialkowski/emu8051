/*
 * pgmwin.c
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

#if HAVE_CONFIG_H
#  include "config.h"
#endif

#include <stdio.h>

#include "common.h"
#include "cpu8051.h"
#include "pgmwin.h"


static GtkWidget *pgmclist;
static int NbBreakpoints;
static unsigned int Breakpoints[ MAXBP ];
static unsigned int DisasmAddresses[ 24 ];


/* Disasm 24 lines from CPU */
void
pgmwin_Disasm( )
{
  char TextTmp[255];
  int row;
  int InstSize;
  unsigned int Address;

  Address = cpu8051.pc;

  gtk_clist_freeze( GTK_CLIST( pgmclist ) );
  for ( row = 0; row < 24; row++ ) {
    InstSize = cpu8051_Disasm( Address, TextTmp );
    if ( pgmwin_IsBreakpoint( Address ) ) TextTmp[0] = '*';
    gtk_clist_set_text( GTK_CLIST( pgmclist ), row, 0, TextTmp );
    DisasmAddresses[ row ] = Address;
    Address += InstSize;
  }
  gtk_clist_select_row( GTK_CLIST( pgmclist ), 0, 0 );
  gtk_clist_thaw( GTK_CLIST( pgmclist ) );
}


/* Clear Breakpoint at Address from list */
static void
pgmwin_ClearBreakpoint( unsigned int Address )
{
  int Index = 0;

  while( Index < NbBreakpoints && Breakpoints[ Index ] != Address ) {
    Index++;
  }
  if( Breakpoints[ Index ] != Address ) {
    return;
  }
  Breakpoints[ Index ] = Breakpoints[ NbBreakpoints - 1 ];
  NbBreakpoints--;
}


/* Set Breakpoint at Address from list. */
static void
pgmwin_SetBreakpoint( unsigned int Address )
{
  if( pgmwin_IsBreakpoint( Address ) ) {
    return;
  }
  if( NbBreakpoints < MAXBP ) {
    Breakpoints[ NbBreakpoints++ ] = Address;
  }
}

/* Toggle the breakpoint at Address. */
static void
pgmwin_ToggleBreakpoint( unsigned int Address )
{
  if( pgmwin_IsBreakpoint( Address ) ) {
    pgmwin_ClearBreakpoint( Address );
  }
  else {
    pgmwin_SetBreakpoint( Address );
  }
}


#ifdef dsfdsfs
/* Show Breakpoints list. */
static void
pgmwin_ShowBreakpoints( )
{
  int Index;

  for ( Index = 0; Index < NbBreakpoints ; Index++ ) {
    printf( "Breakpoint at Address = %.4X\n", Breakpoints[ Index ] );
  }
}
#endif


/* Is the a breakpoint at Address. */
int
pgmwin_IsBreakpoint( unsigned int Address )
{
  int Index = 0;
  while( Index < NbBreakpoints && Breakpoints[ Index ] != Address ) {
    Index++;
  }

  return ( Breakpoints[ Index ] == Address && Index < NbBreakpoints );
}


/* Mouse button pressed in the window. */
static gint
pgmwin_ButtonPressEvent( GtkWidget *widget, GdkEvent *event, gpointer data )
{
  gint row, column;
  char TextTmp[ 255 ];

  //g_print( "pgmwin_ButtonPressEvent(...)\n" );

  gtk_clist_get_selection_info( GTK_CLIST( pgmclist ), ( int )event->button.x ,( int )event->button.y, &row, &column );
  if (row >= 24 || row < 0)
    return TRUE;
  if (column >= 1 || column < 0)
    return TRUE;
  sprintf( TextTmp, "pgmwin_ButtonPressEvent( ) at %d,%d\n", column, row );
  g_print( TextTmp );
  pgmwin_ToggleBreakpoint( DisasmAddresses[ row ] );
  pgmwin_Disasm();
  
  return FALSE;
}


GtkWidget *
pgmwin_init( int width, int height )
{
  int i;
  GtkWidget *fixed_frame;

  fixed_frame = gtk_frame_new(0);
  gtk_frame_set_shadow_type( GTK_FRAME( fixed_frame ), GTK_SHADOW_ETCHED_OUT );
  gtk_widget_set_usize( GTK_WIDGET( fixed_frame ), width, height );

  pgmclist = gtk_clist_new( 1 );
  gtk_clist_set_selection_mode( GTK_CLIST( pgmclist ), GTK_SELECTION_SINGLE );
  gtk_widget_set_usize( GTK_WIDGET( pgmclist ), width, height );
  gtk_clist_set_column_justification( GTK_CLIST( pgmclist ), 0, GTK_JUSTIFY_LEFT );
  gtk_clist_set_column_width( GTK_CLIST( pgmclist ), 0, width-10 );

#if ( GTK_MAJOR_VERSION == 2)
  PangoFontDescription *pango_font;
  pango_font = pango_font_description_from_string( FIXED_FONT );
  gtk_widget_modify_font( pgmclist, pango_font );
#else
  {
    GtkStyle *style;
    /* Setting font for the widget. */
    style = gtk_style_new();
    gdk_font_unref( style->font );
    
    /* Load a fixed font */
    style->font = gdk_font_load( FIXED_FONT );
    gtk_widget_set_style( GTK_WIDGET( pgmclist ), style );
  }
#endif

  char *pgmdummy[] = { 0 };
  for( i = 0; i < 24; i++ ) {
    gtk_clist_append( GTK_CLIST( pgmclist ), pgmdummy );
  }

  gtk_container_add( GTK_CONTAINER( fixed_frame ), pgmclist );

  NbBreakpoints = 0;

  gtk_signal_connect( GTK_OBJECT( pgmclist ), "button-press-event",
		      GTK_SIGNAL_FUNC( pgmwin_ButtonPressEvent ), NULL );

  return fixed_frame;
}
