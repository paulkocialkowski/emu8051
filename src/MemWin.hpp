#ifndef _MEMWIN_HPP_
#define _MEMWIN_HPP_

#include <gtk/gtk.h>
#include "CPU8051.hpp"

//////////////////////////////////////////////////////////////////////////////
// MemWin
// Implements a memory Window in Gtk+ as an Object
//////////////////////////////////////////////////////////////////////////////
class MemWin {
public:
  MemWin( GtkWidget *parentwin );
  ~MemWin( );

  void DumpD( CPU8051 *mCPU, unsigned int Address );
  void DumpI( CPU8051 *mCPU, unsigned int Address );

private:
  GtkWidget *memwin;
  GtkWidget *memclist;


};




#endif
