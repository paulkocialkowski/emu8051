/* emugtk.c */


#include <stdio.h>
#include "config.h"

#include <gtk/gtk.h>
#include "emugtk.h"
#include "reset.xpm"
#include "run.xpm"
#include "stop.xpm"
#include "step.xpm"

#include "cpu8051.h"
#include "options.h"
#include "file.h"
#include "regwin.h"
#include "pgmwin.h"
#include "memwin.h"


int NbSignals = 0;
int SignalsData[ 32 ];

enum {
  DestroySignal=0,
  DeleteSignal,
  OpenISignal,
  QuitISignal,
  AboutISignal,
  ResetBSignal,
  RunBSignal,
  StopBSignal,
  StepBSignal
};


/* private */
int     EmuGtkID;
int     RunningState;
int     RunFuncTag;

GtkWidget *emuwin, *emufixed, *emumainfixed;
GtkWidget *regfrm, *pgmfrm, *memfrm;
GtkWidget *ButtonTable;

GtkWidget *FileMenu, *OpenItem, *QuitItem, *FileItem;
GtkWidget *ViewMenu, *ExtMemItem, *IntMemItem, *ViewItem;

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



#define EXIT_SUCCESS 0


/* in cpu8051.c */
extern unsigned int PC;


//////////////////////////////////////////////////////////////////////////////
// EmuGtk constructor
//////////////////////////////////////////////////////////////////////////////
void
emugtk_init( int argc, char **argv )
{
  RunningState = 0;

  g_print( "\n" );

  gtk_init( &argc, &argv );

  emuwin = gtk_window_new( GTK_WINDOW_TOPLEVEL );
  gtk_window_set_title( GTK_WINDOW( emuwin ), "emu8051" );
  gtk_container_set_border_width( GTK_CONTAINER( emuwin ), 0 );
  gtk_widget_show( emuwin );

  emufixed = gtk_fixed_new();
  gtk_widget_set_usize( GTK_WIDGET( emufixed ), MAIN_WIN_WIDTH, MAIN_WIN_HEIGHT );
  gtk_container_add( GTK_CONTAINER( emuwin ), emufixed );
  gtk_widget_show( emufixed );

  //  EmuMenuBar( );

  // Main window
  emumainfixed = gtk_fixed_new();
  gtk_widget_set_usize( GTK_WIDGET( emumainfixed ), MAIN_WIN_WIDTH, REG_WIN_HEIGHT + MEM_WIN_HEIGHT + BUTTONS_BAR_HEIGHT + 10 );
  gtk_fixed_put( GTK_FIXED( emufixed ), emumainfixed, 0, 25 );
  gtk_widget_show( emumainfixed );

  emugtk_ShowMenu();

  emugtk_AddButtons();
  
  // Registers frame
  regfrm = gtk_frame_new( 0 );
  gtk_frame_set_shadow_type( GTK_FRAME( regfrm ), GTK_SHADOW_ETCHED_OUT );
  gtk_widget_set_usize( GTK_WIDGET( regfrm ), REG_WIN_WIDTH, REG_WIN_HEIGHT );
  gtk_fixed_put( GTK_FIXED( emumainfixed ), regfrm, 0, BUTTONS_BAR_HEIGHT );
  regwin_init( regfrm );
  gtk_widget_show( regfrm );

  // Program disassembly frame
  pgmfrm = gtk_frame_new( 0 );
  gtk_frame_set_shadow_type( GTK_FRAME( pgmfrm ), GTK_SHADOW_ETCHED_OUT );
  gtk_widget_set_usize( GTK_WIDGET( pgmfrm ), PGM_WIN_WIDTH, PGM_WIN_HEIGHT );
  gtk_fixed_put( GTK_FIXED( emumainfixed ), pgmfrm, REG_WIN_WIDTH + 10, BUTTONS_BAR_HEIGHT );

  pgmwin_init( pgmfrm );

  gtk_widget_show( pgmfrm );

  // Memory dump frame
  memfrm = gtk_frame_new( 0 );
  gtk_frame_set_shadow_type( GTK_FRAME( memfrm ), GTK_SHADOW_ETCHED_OUT );
  gtk_widget_set_usize( GTK_WIDGET( memfrm ), MEM_WIN_WIDTH, MEM_WIN_HEIGHT );
  gtk_fixed_put( GTK_FIXED( emumainfixed ), memfrm, 0, REG_WIN_HEIGHT + BUTTONS_BAR_HEIGHT );
  memwin_init( memfrm );
  gtk_widget_show( memfrm );

  
  NbSignals = 0;

  // Window DESTROY signal
  SignalsData[ NbSignals ] = DestroySignal;
  gtk_signal_connect( GTK_OBJECT( emuwin ), "destroy", GTK_SIGNAL_FUNC( EmuGtkSignalStub2 ), &SignalsData[ NbSignals ] );
  NbSignals++;
      
  // Window DELETE event
  SignalsData[ NbSignals ] = DeleteSignal;
  gtk_signal_connect( GTK_OBJECT( emuwin ), "delete_event", GTK_SIGNAL_FUNC( EmuGtkSignalStub3 ), &SignalsData[ NbSignals ] );
  NbSignals++;

  // File->Open
  SignalsData[ NbSignals ] = OpenISignal;
  gtk_signal_connect( GTK_OBJECT( OpenItem ), "activate", GTK_SIGNAL_FUNC( EmuGtkSignalStub2 ), &SignalsData[ NbSignals ] );
  NbSignals++;

  // File->Quit
  SignalsData[ NbSignals ] = QuitISignal;
  gtk_signal_connect( GTK_OBJECT( QuitItem ), "activate", GTK_SIGNAL_FUNC( EmuGtkSignalStub2 ), &SignalsData[ NbSignals ] );
  NbSignals++;

  // Help->About
  SignalsData[ NbSignals ] = AboutISignal;
  gtk_signal_connect( GTK_OBJECT( AboutItem ), "activate", GTK_SIGNAL_FUNC( EmuGtkSignalStub2 ), &SignalsData[ NbSignals ] );
  NbSignals++;

  // RESET button
  SignalsData[ NbSignals ] = ResetBSignal;
  gtk_signal_connect( GTK_OBJECT( ButtonReset ), "button-press-event", GTK_SIGNAL_FUNC( EmuGtkSignalStub3 ), &SignalsData[ NbSignals ] );
  NbSignals++;
      
  // RUN button
  SignalsData[ NbSignals ] = RunBSignal;
  gtk_signal_connect( GTK_OBJECT( ButtonRun ), "button-press-event", GTK_SIGNAL_FUNC( EmuGtkSignalStub3 ), &SignalsData[ NbSignals ] );
  NbSignals++;

  // STOP button
  SignalsData[ NbSignals ] = StopBSignal;
  gtk_signal_connect( GTK_OBJECT( ButtonStop ), "button-press-event", GTK_SIGNAL_FUNC( EmuGtkSignalStub3 ), &SignalsData[ NbSignals ] );
  NbSignals++;      

  // STEP button
  SignalsData[ NbSignals ] = StepBSignal;
  gtk_signal_connect( GTK_OBJECT( ButtonStep ), "button-press-event", GTK_SIGNAL_FUNC( EmuGtkSignalStub3 ), &SignalsData[ NbSignals ] );
  NbSignals++;
      

  if( GetHexFileName() != NULL ) {
    LoadHexFile( GetHexFileName() );
  }
}


int
main( int argc, char **argv )
{
  ParseCommandLineOptions( argc, argv );

  emugtk_init( argc, argv );

  emugtk_Reset( );
  gtk_main();

  printf( "End of program.\n" );
  
  return EXIT_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////////
// Create and show the Reset, Run, Stop, Trace and Step buttons
//////////////////////////////////////////////////////////////////////////////
void
emugtk_AddButtons( void )
{
  RESET_pixmap = gdk_pixmap_colormap_create_from_xpm_d( NULL,
							gtk_widget_get_default_colormap(),
							&RESET_mask,
							NULL,
							( gchar ** ) reset_xpm );
  RESET_widget = gtk_pixmap_new( RESET_pixmap, RESET_mask );
  
  RUN_pixmap = gdk_pixmap_colormap_create_from_xpm_d( NULL,
						      gtk_widget_get_default_colormap(),
						      &RUN_mask,
						      NULL,
						      ( gchar ** ) run_xpm );
  RUN_widget = gtk_pixmap_new( RUN_pixmap, RUN_mask );
  
  STOP_pixmap = gdk_pixmap_colormap_create_from_xpm_d( NULL,
						       gtk_widget_get_default_colormap(),
						       &STOP_mask,
						       NULL,
						       ( gchar ** ) stop_xpm );
  STOP_widget = gtk_pixmap_new( STOP_pixmap, STOP_mask );
  
  STEP_pixmap = gdk_pixmap_colormap_create_from_xpm_d( NULL,
						       gtk_widget_get_default_colormap(),
						       &STEP_mask,
						       NULL,
						       ( gchar ** ) step_xpm );
  STEP_widget = gtk_pixmap_new( STEP_pixmap, STEP_mask );
  
  ButtonTable = gtk_table_new( 1, 4, TRUE );
  gtk_widget_set_usize( GTK_WIDGET( ButtonTable ), BUTTONS_BAR_WIDTH, BUTTONS_BAR_HEIGHT );
  gtk_fixed_put( GTK_FIXED( emumainfixed ), ButtonTable, 0, 0 );
  
  ButtonReset = gtk_button_new();
  ButtonRun   = gtk_button_new();
  ButtonStop  = gtk_button_new();
  ButtonStep  = gtk_button_new();

  gtk_container_add( GTK_CONTAINER( ButtonReset ), RESET_widget );
  gtk_container_add( GTK_CONTAINER( ButtonRun ), RUN_widget );
  gtk_container_add( GTK_CONTAINER( ButtonStop ), STOP_widget );
  gtk_container_add( GTK_CONTAINER( ButtonStep ), STEP_widget );

  gtk_widget_set_usize( GTK_WIDGET( ButtonReset ), BUTTON_WIDTH, BUTTON_HEIGHT );
  gtk_widget_set_usize( GTK_WIDGET( ButtonRun ),   BUTTON_WIDTH, BUTTON_HEIGHT );
  gtk_widget_set_usize( GTK_WIDGET( ButtonStop ),  BUTTON_WIDTH, BUTTON_HEIGHT );
  gtk_widget_set_usize( GTK_WIDGET( ButtonStep ),  BUTTON_WIDTH, BUTTON_HEIGHT );

  gtk_table_attach_defaults( GTK_TABLE( ButtonTable ), ButtonReset, 0, 1, 0, 1);
  gtk_table_attach_defaults( GTK_TABLE( ButtonTable ), ButtonRun,   1, 2, 0, 1);
  gtk_table_attach_defaults( GTK_TABLE( ButtonTable ), ButtonStop,  2, 3, 0, 1);
  gtk_table_attach_defaults( GTK_TABLE( ButtonTable ), ButtonStep,  3, 4, 0, 1);

  gtk_widget_show( GTK_WIDGET( ButtonReset ) );
  gtk_widget_show( GTK_WIDGET( ButtonRun ) );
  gtk_widget_show( GTK_WIDGET( ButtonStop ) );  
  gtk_widget_show( GTK_WIDGET( ButtonStep ) );  

  gtk_widget_show_all( GTK_WIDGET( ButtonTable ) );
}


//////////////////////////////////////////////////////////////////////////////
// CPU reset and Gtk UI update
//////////////////////////////////////////////////////////////////////////////
void
emugtk_Reset( void )
{
  cpu8051_Reset( );
  regwin_Show();
  pgmwin_Disasm();
  memwin_DumpD( 0 );
}


//////////////////////////////////////////////////////////////////////////////
// CPU Step and Gtk UI update
//////////////////////////////////////////////////////////////////////////////
void
emugtk_Step( void )
{
  cpu8051_Exec();
  regwin_Show();
  pgmwin_Disasm();
  memwin_DumpD( 0 );
}



//////////////////////////////////////////////////////////////////////////////
// Show the menu
//////////////////////////////////////////////////////////////////////////////
void
emugtk_ShowMenu( void )
{
  FileMenu = gtk_menu_new( );
  OpenItem = gtk_menu_item_new_with_label( "Open" );
  QuitItem = gtk_menu_item_new_with_label( "Quit" );
  gtk_menu_append( GTK_MENU( FileMenu ), GTK_WIDGET( OpenItem ) );
  gtk_menu_append( GTK_MENU( FileMenu ), GTK_WIDGET( QuitItem ) );
  gtk_widget_show( GTK_WIDGET( OpenItem ) );
  gtk_widget_show( GTK_WIDGET( QuitItem ) );
  
  ViewMenu = gtk_menu_new( );
  ExtMemItem = gtk_menu_item_new_with_label( "External Memory Dump" );
  IntMemItem = gtk_menu_item_new_with_label( "Internal Memory Dump" );
  //PgmMemItem = gtk_menu_item_new_with_label( "Program Memory Dump" );
  gtk_menu_append( GTK_MENU( ViewMenu ), GTK_WIDGET( ExtMemItem ) );
  gtk_menu_append( GTK_MENU( ViewMenu ), GTK_WIDGET( IntMemItem ) );
  //gtk_menu_append( GTK_MENU( ViewMenu ), GTK_WIDGET( PgmMemItem ) );
  gtk_widget_show( GTK_WIDGET( ExtMemItem ) );
  gtk_widget_show( GTK_WIDGET( IntMemItem ) );
  //gtk_widget_show( GTK_WIDGET( PgmMemItem ) );
  
  HelpMenu = gtk_menu_new( );
  AboutItem = gtk_menu_item_new_with_label( "About" );
  gtk_menu_append( GTK_MENU( HelpMenu ), GTK_WIDGET( AboutItem ) );
  gtk_widget_show( GTK_WIDGET( AboutItem ) );
  
  MenuBar = gtk_menu_bar_new( );
  gtk_fixed_put( GTK_FIXED( emufixed ), MenuBar, 0, 0 );
  gtk_widget_show( GTK_WIDGET( MenuBar ) );
  
  FileItem = gtk_menu_item_new_with_label( "File" );
  ViewItem = gtk_menu_item_new_with_label( "View" );
  HelpItem = gtk_menu_item_new_with_label( "Help" );
  gtk_widget_show( GTK_WIDGET( FileItem ) );
  gtk_widget_show( GTK_WIDGET( ViewItem ) );
  gtk_widget_show( GTK_WIDGET( HelpItem ) );
  gtk_menu_item_set_submenu( GTK_MENU_ITEM( FileItem ), FileMenu );
  gtk_menu_item_set_submenu( GTK_MENU_ITEM( ViewItem ), ViewMenu );
  gtk_menu_item_set_submenu( GTK_MENU_ITEM( HelpItem ), HelpMenu );
  gtk_menu_bar_append( GTK_MENU_BAR( MenuBar ), FileItem );
  gtk_menu_bar_append( GTK_MENU_BAR( MenuBar ), ViewItem );
  gtk_menu_bar_append( GTK_MENU_BAR( MenuBar ), HelpItem );
}


//////////////////////////////////////////////////////////////////////////////
// Signal DeleteEvent
//////////////////////////////////////////////////////////////////////////////
gboolean
emugtk_DeleteEvent( GtkWidget *widget, GdkEvent *event, gpointer data )
{
  g_print( "emugtk_DeleteEvent(...)\n" );
  emugtk_StopRunning( );
  return FALSE;
}


//////////////////////////////////////////////////////////////////////////////
// Signal DestroyEvent
//////////////////////////////////////////////////////////////////////////////
void
emugtk_DestroyEvent( GtkWidget *widget, gpointer data )
{
  g_print( "emugtk_DestroyEvent(...)\n" );
  gtk_main_quit();
}


//////////////////////////////////////////////////////////////////////////////
// gint emugtk_AboutEvent( GtkWidget *widget, gpointer data )
// Signal AboutEvent ( Help->About in menu )
//////////////////////////////////////////////////////////////////////////////
void
emugtk_AboutEvent( GtkWidget *widget, gpointer data )
{
  char about_string[256];
  GtkWidget *about_window;
  GtkWidget *text_window;

  sprintf( about_string,
	   "%s\n\nversion %s\n\n\nAuthors:\nHugo Villeneuve\nJonathan St-André\n",
	   PACKAGE, VERSION );
  
  about_window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
  gtk_window_set_title( GTK_WINDOW( about_window ), "About" );
  gtk_container_set_border_width( GTK_CONTAINER( about_window ), 20 );
  
  text_window = gtk_label_new( about_string );
  gtk_container_add( GTK_CONTAINER( about_window ), text_window );
  
  gtk_widget_show_all( GTK_WIDGET( about_window ) );
}


//////////////////////////////////////////////////////////////////////////////
// Signal OpenEvent ( File->Open in menu )
//////////////////////////////////////////////////////////////////////////////
void
emugtk_OpenEvent( GtkWidget *widget, gpointer data )
{
  GtkWidget *FileOpendialog;

  // g_print( "emugtk_OpenEvent(...)\n" );

  FileOpendialog = gtk_file_selection_new( "Open Intel Hex file" );

  // Connect the file dialog's OK button up to a handler.
  gtk_signal_connect( GTK_OBJECT( GTK_FILE_SELECTION ( FileOpendialog ) -> ok_button ),
		      "clicked",
		      GTK_SIGNAL_FUNC( FileOpenDialog_OK ),
		      FileOpendialog );

  // Connect the file dialog's CANCEL button up to a handler.
  gtk_signal_connect( GTK_OBJECT( GTK_FILE_SELECTION ( FileOpendialog ) -> cancel_button ),
		      "clicked",
		      GTK_SIGNAL_FUNC( FileOpenDialog_CANCEL ),
		      FileOpendialog );

  // Set the 'File Open dialog' to show only Intel HEX files (.hex).
  // gtk_file_selection_complete( GTK_FILE_SELECTION( FileOpendialog ), "*.hex" );
  
  // Show the dialog
  gtk_widget_show( GTK_WIDGET( FileOpendialog ) );
}


void
FileOpenDialog_OK( GtkButton *button, gpointer data )
{
  g_print( "emugtk_FileOpenDialog_OK Event(...)\n" );

  const gchar *SelectedFile;

  SelectedFile = (const gchar *) gtk_file_selection_get_filename( GTK_FILE_SELECTION( data ) );
  
  g_print( "emugtk_File = %s\n", SelectedFile );
  
  emugtk_StopRunning();

  LoadHexFile( SelectedFile );

  gtk_widget_destroy( GTK_WIDGET( data ) );

  emugtk_Reset();
  emugtk_UpdateDisplay();
}


void
emugtk_UpdateDisplay( void )
{
  g_print( "emugtk_UpdateDisplay()\n" );

  regwin_Show();
  pgmwin_Disasm();
  memwin_DumpD( 0 );
}


void
FileOpenDialog_CANCEL( GtkButton *button, gpointer data )
{
  g_print( "emugtk_FileOpenDialog_CANCEL Event(...)\n" );
  
  gtk_widget_destroy( GTK_WIDGET( data ) );
}


//////////////////////////////////////////////////////////////////////////////
// Signal QuitEvent ( File->Quit in menu )
//////////////////////////////////////////////////////////////////////////////
void
emugtk_QuitEvent( GtkWidget *widget, gpointer data )
{
  g_print( "emugtk_QuitEvent(...)\n" );
  emugtk_StopRunning( );
  gtk_main_quit( );
}


//////////////////////////////////////////////////////////////////////////////
// Signal ResetEvent ( ResetButton )
//////////////////////////////////////////////////////////////////////////////
void
emugtk_ResetEvent( GtkWidget *widget, GdkEvent *event, gpointer data )
{
  g_print( "emugtk_ResetEvent(...)\n" );
  emugtk_StopRunning( );
  emugtk_Reset( );
}


//////////////////////////////////////////////////////////////////////////////
// Signal RunEvent ( RunButton )
//////////////////////////////////////////////////////////////////////////////
void
emugtk_RunEvent( GtkWidget *widget, GdkEvent *event, gpointer data )
{
  g_print( "emugtk_RunEvent(...)\n" );
  if ( RunningState ) {
    //   g_print( "Getting out of RunningState! \n" );
    emugtk_StopRunning( );
  }
  else {
    //   g_print( "Going In RunningState! \n" );
    emugtk_StartRunning( );
  }
}


//////////////////////////////////////////////////////////////////////////////
// Signal StopEvent ( StopButton )
//////////////////////////////////////////////////////////////////////////////
void
emugtk_StopEvent( GtkWidget *widget, GdkEvent *event, gpointer data )
{
  g_print( "emugtk_StopEvent(...)\n" );
  emugtk_StopRunning( );
}


//////////////////////////////////////////////////////////////////////////////
// Signal StepEvent ( StepButton )
//////////////////////////////////////////////////////////////////////////////
void
emugtk_StepEvent( GtkWidget *widget, GdkEvent *event, gpointer data )
{
  g_print( "emugtk_StepEvent(...)\n" );
  emugtk_StopRunning( );
  emugtk_Step();
}


//////////////////////////////////////////////////////////////////////////////
// Signal Stub with 2 parameters
//////////////////////////////////////////////////////////////////////////////
void
EmuGtkSignalStub2( GtkWidget *widget, gpointer data )
{
  g_print( "EmuGtkSignalStub2(...)\n");

  int SigNumber = *( (int *) data );

  switch( SigNumber ) {
  case DestroySignal:
    emugtk_DestroyEvent( widget, 0 );
    break;
  case AboutISignal:
    emugtk_AboutEvent( widget, 0 );
    break;
  case OpenISignal:
    emugtk_OpenEvent( widget, 0 );
    break;
  case QuitISignal:
    emugtk_QuitEvent( widget, 0 );
    break;
  default:
    g_print( "*** error: EmuGtkSignalStub2: default case reached\n" );
    break;
  };
}


//////////////////////////////////////////////////////////////////////////////
// Signal Stub with 3 parameters
//////////////////////////////////////////////////////////////////////////////
void EmuGtkSignalStub3( GtkWidget *widget, GdkEvent *event, gpointer data )
{
  g_print( "EmuGtkSignalStub3(...)\n");

  int SigNumber = *( (int *) data );
  
  switch( SigNumber ) {
  case DeleteSignal:
    emugtk_DeleteEvent( widget, event, 0 );
    break;
  case ResetBSignal:
    emugtk_ResetEvent( widget, event, 0 );
    break;
  case RunBSignal:
    emugtk_RunEvent( widget, event, 0 );
    break;
  case StopBSignal:
    emugtk_StopEvent( widget, event, 0 );
    break;
  case StepBSignal:
    emugtk_StepEvent( widget, event, 0 );
    break;
  default:
    g_print( "*** error: EmuGtkSignalStub3: default case reached\n" );
    break;
  };
}


//////////////////////////////////////////////////////////////////////////////
// Running called by RunningFunction( )
//////////////////////////////////////////////////////////////////////////////
void
emugtk_Running( )
{
  cpu8051_Exec( );
  if( pgmwin_IsBreakpoint( PC ) ) {
    g_print( "Breakpoint Hit, stopping!\n" );
    emugtk_StopRunning( );
  }
}


//////////////////////////////////////////////////////////////////////////////
// RunningFunction called when idle from gtk_main
//////////////////////////////////////////////////////////////////////////////
gboolean
RunningFunction( gpointer data )
{
  emugtk_Running( );
  return TRUE;
}


//////////////////////////////////////////////////////////////////////////////
// Get in the RunningState
//////////////////////////////////////////////////////////////////////////////
void
emugtk_StartRunning( void )
{
  if ( !RunningState ) {

    printf( "emugtk_StartRunning( )\n" );

    /*RunFuncTag = gtk_idle_add( GtkFunction( RunningFunction ), 0 );*/
    RunFuncTag = gtk_idle_add( RunningFunction, 0 );

    RunningState = 1;

    // gtk_widget_hide( GTK_WIDGET( ButtonRun ) );
    // gtk_widget_show_now( GTK_WIDGET( ButtonStop ) );
    // gtk_table_attach_defaults( GTK_TABLE( ButtonTable ), ButtonStop, 3, 4, 0, 1);
  }
}


//////////////////////////////////////////////////////////////////////////////
// Step out of RunningState
//////////////////////////////////////////////////////////////////////////////
void
emugtk_StopRunning( )
{
  if (RunningState) {
    printf( "emugtk_StopRunning( )\n" );
    gtk_idle_remove( RunFuncTag );
    RunningState = 0;
    //gtk_widget_hide( GTK_WIDGET( ButtonStop ) );
    //gtk_widget_show( GTK_WIDGET( ButtonRun ) );
    //    gtk_table_attach_defaults( GTK_TABLE( ButtonTable ), ButtonRun, 3, 4, 0, 1);
    regwin_Show();
    pgmwin_Disasm();
    memwin_DumpD( 0 );
  }
}
