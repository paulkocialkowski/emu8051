#ifndef _REGWIN_HPP_
#define _REGWIN_HPP_

#include <gtk/gtk.h>
#include "CPU8051.hpp"
#include "GtkSizes.hpp"

//////////////////////////////////////////////////////////////////////////////
// RegWin
// Implements a Registers Window in Gtk+ as an Object
//////////////////////////////////////////////////////////////////////////////
class RegWin {
public:
  RegWin( GtkWidget *parentwin );
  ~RegWin( );

  void Show( CPU8051 *CPU );

private:
  GtkWidget *regwin;
  GtkWidget *regclist;


};

#endif
