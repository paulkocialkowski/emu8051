/* pgmwin.cpp */


#if HAVE_CONFIG_H
#  include "config.h"
#endif

#include "PgmWin.hpp"
#include <stdio.h>

int PgmWinNumber = 0;
int PgmWinNumbers[ 1 ];
PgmWin *PgmWinPtrs[ 1 ];

//////////////////////////////////////////////////////////////////////////////
// PgmWin::PgmWin( GtkWidget *parentwin, CPU8051 *mCPU )
// PgmWin constructor
//////////////////////////////////////////////////////////////////////////////
PgmWin::PgmWin( GtkWidget *parentwin, CPU8051 *mCPU )
{
  CPU = mCPU;
  int i;
  GtkStyle *style;
  GdkFont *fixedfont;
  fixedfont = gdk_font_load( "-adobe-courier-medium-r-normal--12-120-75-75-m-70-iso8859-1" );

  pgmclist = gtk_clist_new( 1 );
  gtk_clist_set_selection_mode( GTK_CLIST( pgmclist ), GTK_SELECTION_SINGLE );
  gtk_widget_set_usize( GTK_WIDGET( pgmclist ), PGM_WIN_WIDTH, PGM_WIN_HEIGHT );
  gtk_clist_set_column_justification( GTK_CLIST( pgmclist ), 0, GTK_JUSTIFY_LEFT );
  gtk_clist_set_column_width( GTK_CLIST( pgmclist ), 0, PGM_WIN_WIDTH-10 );

  style = gtk_widget_get_style( GTK_WIDGET( pgmclist ) );

#ifdef USE_GTK2
  gtk_style_set_font( style, fixedfont );
#else
  style->font = fixedfont;
#endif

  gtk_widget_set_style( GTK_WIDGET( pgmclist ), style );

  char *pgmdummy[] = { 0 };
  for ( i = 0; i < 24; i++ ) gtk_clist_append( GTK_CLIST( pgmclist ), pgmdummy );

  gtk_container_add( GTK_CONTAINER( parentwin ), pgmclist );

  gtk_widget_show( pgmclist );

  NbBreakpoints = 0;

  if ( PgmWinNumber >= 1 ) g_print( "WARNING! Too many PgmWin objects to handle signals!\n");
  else {
    PgmWinPtrs[ PgmWinNumber ] = this;
    PgmWinNumbers[ PgmWinNumber ] = PgmWinNumber;
    gtk_signal_connect( GTK_OBJECT( pgmclist ), "button-press-event", GTK_SIGNAL_FUNC( PgmWinButtonPress ), &PgmWinNumbers[ PgmWinNumber++ ] );
  }
}


//////////////////////////////////////////////////////////////////////////////
// PgmWin::~PgmWin( )
// PgmWin destructor
//////////////////////////////////////////////////////////////////////////////
PgmWin::~PgmWin( )
{

}

//////////////////////////////////////////////////////////////////////////////
// void PgmWin::Disasm( )
// Disasm 24 lines from CPU
//////////////////////////////////////////////////////////////////////////////
void PgmWin::Disasm( )
{
char TextTmp[255];
int row;
//int TextLength;
int InstSize;
unsigned int Address;
Address = CPU->GetPC( );

gtk_clist_freeze( GTK_CLIST( pgmclist ) );
 for ( row = 0; row < 24; row++ ) {
   InstSize = CPU->Disasm( Address, TextTmp );
   if ( IsBreakpoint( Address ) ) TextTmp[0] = '*';
   gtk_clist_set_text( GTK_CLIST( pgmclist ), row, 0, TextTmp );
   DisasmAddresses[ row ] = Address;
   Address += InstSize;
 }
gtk_clist_select_row( GTK_CLIST( pgmclist ), 0, 0 );
gtk_clist_thaw( GTK_CLIST( pgmclist ) );
}

//////////////////////////////////////////////////////////////////////////////
// gint PgmWin::ButtonPressEvent( GtkWidget *widget, GdkEvent *event, gpointer data )
// Mouse button pressed in the window
//////////////////////////////////////////////////////////////////////////////
gint PgmWin::ButtonPressEvent( GtkWidget *widget, GdkEvent *event, gpointer data )
{
  gint row, column;
  char TextTmp[ 255 ];
  //g_print( "PgmWin::ButtonPressEvent(...)\n" );
  gtk_clist_get_selection_info( GTK_CLIST( pgmclist ), ( int )event->button.x ,( int )event->button.y, &row, &column );
  if (row >= 24 || row < 0)
    return TRUE;
  if (column >= 1 || column < 0)
    return TRUE;
  sprintf( TextTmp, "PgmWin::ButtonPressEvent( ) at %d,%d\n", column, row );
  g_print( TextTmp );
  ToggleBreakpoint( DisasmAddresses[ row ] );
  Disasm( );
  return FALSE;
}

//////////////////////////////////////////////////////////////////////////////
// gint PgmWinButtonPress( GtkWidget *widget, GdkEvent *event, gpointer data )
// Signal Stub with 3 parameters
//////////////////////////////////////////////////////////////////////////////
void PgmWinButtonPress( GtkWidget *widget, GdkEvent *event, gpointer data )
{
int PWNumber = (* ( static_cast< int * >( data ) ) );
PgmWinPtrs[ PWNumber ]->ButtonPressEvent( widget, event, 0 );
}


//////////////////////////////////////////////////////////////////////////////
// void PgmWin::ShowBreakpoints( )
// Show Breakpoints list
//////////////////////////////////////////////////////////////////////////////
void PgmWin::ShowBreakpoints( )
{
  for ( int Index = 0; Index < NbBreakpoints ; Index++ )
    printf( "Breakpoint at Address = %.4X\n", Breakpoints[ Index ] );
}

//////////////////////////////////////////////////////////////////////////////
// void PgmWin::ClearBreakpoint( unsigned int Address )
// Clear Breakpoint at Address from list
//////////////////////////////////////////////////////////////////////////////
void PgmWin::ClearBreakpoint( unsigned int Address )
{
  int Index = 0;
  while ( Index < NbBreakpoints && Breakpoints[ Index ] != Address ) Index++;
  if ( Breakpoints[ Index ] != Address ) return;
  Breakpoints[ Index ] = Breakpoints[ NbBreakpoints - 1 ];
  NbBreakpoints--;
}

//////////////////////////////////////////////////////////////////////////////
// void PgmWin::SetBreakpoint( unsigned int Address )
// Set Breakpoint at Address from list
//////////////////////////////////////////////////////////////////////////////
void PgmWin::SetBreakpoint( unsigned int Address )
{
  if ( IsBreakpoint( Address ) ) return;
  if ( NbBreakpoints < MAXBP ) Breakpoints[ NbBreakpoints++ ] = Address;
}

//////////////////////////////////////////////////////////////////////////////
// int PgmWin::IsBreakpoint( unsigned int Address )
// Is the a breakpoint at Address
//////////////////////////////////////////////////////////////////////////////
int PgmWin::IsBreakpoint( unsigned int Address )
{
  int Index = 0;
  while ( Index < NbBreakpoints && Breakpoints[ Index ] != Address ) Index++;
  return ( Breakpoints[ Index ] == Address && Index < NbBreakpoints );
}

//////////////////////////////////////////////////////////////////////////////
// void PgmWin::ToggleBreakpoint( unsigned int Address )
// Toggle the breakpoint at Address
//////////////////////////////////////////////////////////////////////////////
void PgmWin::ToggleBreakpoint( unsigned int Address )
{
  if ( IsBreakpoint( Address ) ) ClearBreakpoint( Address );
  else SetBreakpoint( Address );
}







