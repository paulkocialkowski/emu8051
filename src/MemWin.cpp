/* memwin.cpp */


#if HAVE_CONFIG_H
#  include "config.h"
#endif

#include "MemWin.hpp"
#include <stdio.h>


//////////////////////////////////////////////////////////////////////////////
// MemWin::MemWin( GtkWidget *parentwin )
// MemWin constructor
//////////////////////////////////////////////////////////////////////////////
MemWin::MemWin( GtkWidget *parentwin )
{
  int i;
  GtkStyle *style;
  GdkFont *fixedfont;
  fixedfont = gdk_font_load( "-adobe-courier-medium-r-normal--12-120-75-75-m-70-iso8859-1" );

  memclist = gtk_clist_new( 18 );
  gtk_clist_set_selection_mode( GTK_CLIST( memclist ), GTK_SELECTION_SINGLE );
  gtk_widget_set_usize( GTK_WIDGET( memclist ), 620, 250 );

  for ( i = 0; i < 18; i++ ) gtk_clist_set_column_justification( GTK_CLIST( memclist ), i, GTK_JUSTIFY_LEFT );
  gtk_clist_set_column_width( GTK_CLIST( memclist ), 0, 5*8 );
  for ( i = 1; i < 17; i++ ) gtk_clist_set_column_width( GTK_CLIST( memclist ), i, 2*8 );
  gtk_clist_set_column_width( GTK_CLIST( memclist ), 17, 16*8 );

  style = gtk_widget_get_style( GTK_WIDGET( memclist ) );

#ifdef USE_GTK2
  gtk_style_set_font( style, fixedfont );
#else
  style->font = fixedfont;
#endif

  gtk_widget_set_style( GTK_WIDGET( memclist ), style );

  char *memdummy[] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
  for ( i = 0; i < 16; i++ ) gtk_clist_append( GTK_CLIST( memclist ), memdummy );

  gtk_container_add( GTK_CONTAINER( parentwin ), memclist );

  gtk_widget_show( memclist );
}


//////////////////////////////////////////////////////////////////////////////
// MemWin::~MemWin( )
// MemWin destructor
//////////////////////////////////////////////////////////////////////////////
MemWin::~MemWin( )
{

}

//////////////////////////////////////////////////////////////////////////////
// void MemWin::DumpD( CPU8051 *mCPU, unsigned int Address )
// Dump 16 rows of 16 bytes from Address in Memory (direct addressing)
//////////////////////////////////////////////////////////////////////////////
void MemWin::DumpD( CPU8051 *mCPU, unsigned int Address)
{
char TextTmp[255];
int row, column, TextLength;

gtk_clist_freeze( GTK_CLIST( memclist ) );
 for ( row = 0; row < 16; row++ ) {
   sprintf( TextTmp, "%.4X", Address );
   gtk_clist_set_text( GTK_CLIST( memclist ), row, 0, TextTmp );

   for ( column = 0; column < 16; column++ ) {
     sprintf( TextTmp, "%.2X", ( int ) mCPU->ReadD( Address + column ) );
     gtk_clist_set_text( GTK_CLIST( memclist ), row, column + 1, TextTmp );
   }

   TextLength = 0;
   for ( column = 0; column < 16; column++ ) {
     if ( ( ( int ) mCPU->ReadD( Address + column ) >= 32 ) && ( ( int ) mCPU->ReadD( Address + column ) <= 126 ) )
       TextLength += sprintf( &TextTmp[ TextLength ], "%c", mCPU->ReadD( Address + column ) );
     else TextLength += sprintf( &TextTmp[ TextLength ], "." );
   }
   gtk_clist_set_text( GTK_CLIST( memclist ), row, 17, TextTmp );

   Address += 16;
 }

gtk_clist_select_row( GTK_CLIST( memclist ), 0, 0 );
gtk_clist_thaw( GTK_CLIST( memclist ) );
}

//////////////////////////////////////////////////////////////////////////////
// void MemWin::DumpI( CPU8051 *mCPU, unsigned int Address )
// Dump 16 rows of 16 bytes from Address in Memory (indirect addressing)
//////////////////////////////////////////////////////////////////////////////
void MemWin::DumpI( CPU8051 *mCPU, unsigned int Address)
{
char TextTmp[255];
int row, column, TextLength;

gtk_clist_freeze( GTK_CLIST( memclist ) );
 for ( row = 0; row < 16; row++ ) {
   sprintf( TextTmp, "%.4X", Address );
   gtk_clist_set_text( GTK_CLIST( memclist ), row, 0, TextTmp );

   for ( column = 0; column < 16; column++ ) {
     sprintf( TextTmp, "%.2X", ( int ) mCPU->ReadI( Address + column ) );
     gtk_clist_set_text( GTK_CLIST( memclist ), row, column + 1, TextTmp );
   }

   TextLength = 0;
   for ( column = 0; column < 16; column++ ) {
     if ( ( ( int ) mCPU->ReadI( Address + column ) >= 32 ) && ( ( int ) mCPU->ReadI( Address + column ) <= 126 ) )
       TextLength += sprintf( &TextTmp[ TextLength ], "%c", mCPU->ReadI( Address + column ) );
     else TextLength += sprintf( &TextTmp[ TextLength ], "." );
   }
   gtk_clist_set_text( GTK_CLIST( memclist ), row, 17, TextTmp );

   Address += 16;
 }
gtk_clist_thaw( GTK_CLIST( memclist ) );
}




