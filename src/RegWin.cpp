/* regwin.cpp */


#if HAVE_CONFIG_H
#  include "config.h"
#endif

#include <stdio.h>
#include "RegWin.hpp"


//////////////////////////////////////////////////////////////////////////////
// RegWin::RegWin( GtkWidget *parentwin )
// RegWin constructor
//////////////////////////////////////////////////////////////////////////////
RegWin::RegWin( GtkWidget *parentwin )
{
  int i;
  GtkStyle *style;
  GdkFont *fixedfont;
  fixedfont = gdk_font_load( "-adobe-courier-medium-r-normal--12-120-75-75-m-70-iso8859-1" );
  
  regclist = gtk_clist_new( 1 );
  gtk_clist_set_selection_mode( GTK_CLIST( regclist ), GTK_SELECTION_SINGLE );
  gtk_widget_set_usize( GTK_WIDGET( regclist ), REG_WIN_WIDTH, REG_WIN_HEIGHT );
  gtk_clist_set_column_justification( GTK_CLIST( regclist ), 0, GTK_JUSTIFY_LEFT );
  gtk_clist_set_column_width( GTK_CLIST( regclist ), 0, REG_WIN_WIDTH );

  style = gtk_widget_get_style( GTK_WIDGET( regclist ) );

#ifdef USE_GTK2
  gtk_style_set_font( style, fixedfont );
#else
  style->font = fixedfont;
#endif

  gtk_widget_set_style( GTK_WIDGET( regclist ), style );

  char *regdummy[] = { 0 };
  for ( i = 0; i < 24; i++ )
    gtk_clist_append( GTK_CLIST( regclist ), regdummy );
  
  gtk_container_add( GTK_CONTAINER( parentwin ), regclist );

  gtk_widget_show( regclist );
}


//////////////////////////////////////////////////////////////////////////////
// RegWin::~RegWin( )
// RegWin destructor
//////////////////////////////////////////////////////////////////////////////
RegWin::~RegWin( )
{
}

//////////////////////////////////////////////////////////////////////////////
// void RegWin::Show( CPU8051 *CPU )
// Show registers
//////////////////////////////////////////////////////////////////////////////
void RegWin::Show( CPU8051 *CPU )
{
  char TextTmp[255];
  int row = 0;
  unsigned char PSW = CPU->ReadD( _PSW_ );
  unsigned char Rbank;
  
  gtk_clist_freeze( GTK_CLIST( regclist ) );
  
  // Main registers
  sprintf( TextTmp , "PC   = %.4X", CPU->GetPC( ) );
  gtk_clist_set_text( GTK_CLIST( regclist ), row++, 0, TextTmp );
  sprintf( TextTmp , "SP   =   %.2X", CPU->ReadD( _SP_ ) );
  gtk_clist_set_text( GTK_CLIST( regclist ), row++, 0, TextTmp );
  sprintf( TextTmp , "A    =   %.2X", CPU->ReadD( _ACC_ ) );
  gtk_clist_set_text( GTK_CLIST( regclist ), row++, 0, TextTmp );
  sprintf( TextTmp , "B    =   %.2X", CPU->ReadD( _B_ ) );
  gtk_clist_set_text( GTK_CLIST( regclist ), row++, 0, TextTmp );
  sprintf( TextTmp , "DPTR = %.4X", ( CPU->ReadD( _DPTRHIGH_ )  << 8 ) +  CPU->ReadD( _DPTRLOW_ ) );
  gtk_clist_set_text( GTK_CLIST( regclist ), row++, 0, TextTmp );
  
  // Program Status Word
  sprintf( TextTmp , "PSW  =   %.2X",PSW);
  gtk_clist_set_text( GTK_CLIST( regclist ), row++, 0, TextTmp );

  // Ports registers
  sprintf( TextTmp , "P0 =     %.2X", CPU->ReadD( _P0_ ) );
  gtk_clist_set_text( GTK_CLIST( regclist ), row++, 0, TextTmp );
  sprintf( TextTmp , "P1 =     %.2X", CPU->ReadD( _P1_ ) );
  gtk_clist_set_text( GTK_CLIST( regclist ), row++, 0, TextTmp );
  sprintf( TextTmp , "P2 =     %.2X", CPU->ReadD( _P2_ ) );
  gtk_clist_set_text( GTK_CLIST( regclist ), row++, 0, TextTmp );
  sprintf( TextTmp , "P3 =     %.2X", CPU->ReadD( _P3_ ) );
  gtk_clist_set_text( GTK_CLIST( regclist ), row++, 0, TextTmp );

  // Misc Registers
  sprintf( TextTmp , "TCON =   %.2X", CPU->ReadD( _TCON_ ) );
  gtk_clist_set_text( GTK_CLIST( regclist ), row++, 0, TextTmp );
  sprintf( TextTmp , "TMOD =   %.2X", CPU->ReadD( _TMOD_ ) );
  gtk_clist_set_text( GTK_CLIST( regclist ), row++, 0, TextTmp );
  sprintf( TextTmp , "SCON =   %.2X", CPU->ReadD( _SCON_ ) );
  gtk_clist_set_text( GTK_CLIST( regclist ), row++, 0, TextTmp );
  sprintf( TextTmp , "IE   =   %.2X", CPU->ReadD( _IE_ ) );
  gtk_clist_set_text( GTK_CLIST( regclist ), row++, 0, TextTmp );
  sprintf( TextTmp , "IP   =   %.2X", CPU->ReadD( _IP_ ) );
  gtk_clist_set_text( GTK_CLIST( regclist ), row++, 0, TextTmp );

  // R0-R7 Registers in current Bank
  Rbank = CPU->ReadD( _PSW_ ) & 0x18;
  sprintf( TextTmp , "Bank =   %.2X", Rbank);
  gtk_clist_set_text( GTK_CLIST( regclist ), row++, 0, TextTmp );
  sprintf( TextTmp , "R0   =   %.2X", CPU->ReadD( _R0_ + Rbank ) );
  gtk_clist_set_text( GTK_CLIST( regclist ), row++, 0, TextTmp );
  sprintf( TextTmp , "R1   =   %.2X", CPU->ReadD( _R1_ + Rbank ) );
  gtk_clist_set_text( GTK_CLIST( regclist ), row++, 0, TextTmp );
  sprintf( TextTmp , "R2   =   %.2X", CPU->ReadD( _R2_ + Rbank ) );
  gtk_clist_set_text( GTK_CLIST( regclist ), row++, 0, TextTmp );
  sprintf( TextTmp , "R3   =   %.2X", CPU->ReadD( _R3_ + Rbank ) );
  gtk_clist_set_text( GTK_CLIST( regclist ), row++, 0, TextTmp );
  sprintf( TextTmp , "R4   =   %.2X", CPU->ReadD( _R4_ + Rbank ) );
  gtk_clist_set_text( GTK_CLIST( regclist ), row++, 0, TextTmp );
  sprintf( TextTmp , "R5   =   %.2X", CPU->ReadD( _R5_ + Rbank ) );
  gtk_clist_set_text( GTK_CLIST( regclist ), row++, 0, TextTmp );
  sprintf( TextTmp , "R6   =   %.2X", CPU->ReadD( _R6_ + Rbank ) );
  gtk_clist_set_text( GTK_CLIST( regclist ), row++, 0, TextTmp );
  sprintf( TextTmp , "R7   =   %.2X", CPU->ReadD( _R7_ + Rbank ) );
  gtk_clist_set_text( GTK_CLIST( regclist ), row++, 0, TextTmp );
  
  gtk_clist_select_row(GTK_CLIST(regclist),0,0);
  gtk_clist_thaw( GTK_CLIST( regclist ) );
}
