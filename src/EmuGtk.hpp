#ifndef _EMUGTK_HPP_
#define _EMUGTK_HPP_

#include <gtk/gtk.h>
#include <string>
#include <fstream>
#include "CPU8051.hpp"
#include "MemWin.hpp"
#include "PgmWin.hpp"
#include "RegWin.hpp"
#include "GtkSizes.hpp"
#include "exceptions.hpp"

using namespace std;


//////////////////////////////////////////////////////////////////////////////
// EmuGtk
// Implements the Gtk+ Graphical User Interface as an Object
//////////////////////////////////////////////////////////////////////////////
class EmuGtk {
public:
  EmuGtk( int argc , char **argv, CPU8051 *mCPU );
  ~EmuGtk( );
  
  void Main( );

  void Reset( );
  void Step( );
  //  void Step( );
  //  void Exec( );

  void AddButtons( );
  void ShowMenu( );

  gboolean DeleteEvent( GtkWidget *widget, GdkEvent *event, gpointer data );
  void DestroyEvent( GtkWidget *widget, gpointer data );

  void OpenEvent( GtkWidget *widget, gpointer data );
  void QuitEvent( GtkWidget *widget, gpointer data );
  void AboutEvent( GtkWidget *widget, gpointer data );

  void ResetEvent( GtkWidget *widget, GdkEvent *event, gpointer data );
  void RunEvent( GtkWidget *widget, GdkEvent *event, gpointer data );
  void StopEvent( GtkWidget *widget, GdkEvent *event, gpointer data );
  void StepEvent( GtkWidget *widget, GdkEvent *event, gpointer data );

  void StartRunning( );
  void StopRunning( );
  void Running( );

  void LoadHexFile( string Filename );
  void UpdateDisplay();

private:
  int     EmuGtkID;
  CPU8051 *CPU;
  int     RunningState;
  int     RunFuncTag;
  MemWin  *memwin;
  PgmWin  *pgmwin;
  RegWin  *regwin;
  GtkWidget *emuwin, *emufixed, *emumainfixed;
  GtkWidget *regfrm, *pgmfrm, *memfrm;
  GtkWidget *ButtonTable;
  
  //  GdkPixmap *PixMapT, *PixMapRun, *PixMapR, *PixMapQ;
  //  GtkWidget *PixMapWidT, *PixMapWidRun, *PixMapWidR, *PixMapWidQ;
  //  GdkBitmap *mask;
  GtkWidget *FileMenu, *OpenItem, *QuitItem, *FileItem;
  GtkWidget *ViewMenu, *ExtMemItem, *IntMemItem, *ViewItem;
  //  GtkWidget *ViewMenu, *ExtMemItem, *IntMemItem, *PgmMemItem, *ViewItem;
  GtkWidget *HelpMenu, *AboutItem, *LicenseItem, *HelpItem;
  GtkWidget *MenuBar;

  // RESET button
  GdkBitmap *RESET_mask;
  GdkPixmap *RESET_pixmap;
  GtkWidget *RESET_widget;
  GtkWidget *ButtonReset;

  // RUN button
  GdkBitmap *RUN_mask;
  GdkPixmap *RUN_pixmap;
  GtkWidget *RUN_widget;
  GtkWidget *ButtonRun;

  // STOP button
  GdkBitmap *STOP_mask;
  GdkPixmap *STOP_pixmap;
  GtkWidget *STOP_widget;
  GtkWidget *ButtonStop;

  // STEP button
  GdkBitmap *STEP_mask;
  GdkPixmap *STEP_pixmap;
  GtkWidget *STEP_widget;
  GtkWidget *ButtonStep;
 
  unsigned int Ascii2Hex( string istring, int length );
};

void EmuGtkSignalStub3( GtkWidget *widget, GdkEvent *event, gpointer data );
void EmuGtkSignalStub2( GtkWidget *widget, gpointer data );
void FileOpenDialog_OK( GtkButton *button, gpointer data );
void FileOpenDialog_CANCEL( GtkButton *button, gpointer data );

gint RunningFunction( );


#endif
