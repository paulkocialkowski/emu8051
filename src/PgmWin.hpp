#ifndef _PGMWIN_HPP_
#define _PGMWIN_HPP_

#include <gtk/gtk.h>
#include "CPU8051.hpp"
#include "GtkSizes.hpp"

#define MAXBP 32

//////////////////////////////////////////////////////////////////////////////
// PgmWin
// Implements a Program Window in Gtk+ as an Object
//////////////////////////////////////////////////////////////////////////////
class PgmWin {
public:
  PgmWin( GtkWidget *parentwin, CPU8051 *mCPU );
  ~PgmWin( );

  void Disasm( );
  gint ButtonPressEvent( GtkWidget *widget, GdkEvent *event, gpointer data );

  void ShowBreakpoints( );
  void SetBreakpoint( unsigned int Address );
  void ClearBreakpoint( unsigned int Address );
  int IsBreakpoint( unsigned int Address );
  void ToggleBreakpoint( unsigned int Address );

private:
  CPU8051 *CPU;
  GtkWidget *pgmwin;
  GtkWidget *pgmclist;
  int NbBreakpoints;
  unsigned int Breakpoints[ MAXBP ];
  unsigned int DisasmAddresses[ 24 ];

};

void PgmWinButtonPress( GtkWidget *widget, GdkEvent *event, gpointer data );


#endif
