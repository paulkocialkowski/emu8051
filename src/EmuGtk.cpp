// EmuGtk.cpp

#include <iostream>
#include <stdio.h>
#include "config.h"
#include "CPU8051.hpp"
#include "EmuGtk.hpp"
#include "exceptions.hpp"
#include "reset.xpm"
#include "run.xpm"
#include "stop.xpm"
#include "step.xpm"


int EmuGtkNumber = 0;
int NbSignals = 0;
int SignalsData[ 32 ];
EmuGtk *EmuGtkPtr;


enum
{
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


int main( int argc, char **argv )
{
  CPU8051 *maincpu = new CPU8051;
  EmuGtk *emuUI = new EmuGtk( argc, argv, maincpu );
  
  emuUI->Main();
  printf( "End of program.\n" );
  
  delete emuUI;
  delete maincpu;
  
  return 0;
}

//////////////////////////////////////////////////////////////////////////////
// EmuGtk::EmuGtk(  )
// EmuGtk constructor
//////////////////////////////////////////////////////////////////////////////
EmuGtk::EmuGtk( int argc, char **argv, CPU8051 *mCPU )
{
  CPU = mCPU;
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

  ShowMenu();

  AddButtons();
  
  // Registers frame
  regfrm = gtk_frame_new( 0 );
  gtk_frame_set_shadow_type( GTK_FRAME( regfrm ), GTK_SHADOW_ETCHED_OUT );
  gtk_widget_set_usize( GTK_WIDGET( regfrm ), REG_WIN_WIDTH, REG_WIN_HEIGHT );
  gtk_fixed_put( GTK_FIXED( emumainfixed ), regfrm, 0, BUTTONS_BAR_HEIGHT );
  regwin = new RegWin( regfrm );
  gtk_widget_show( regfrm );

  // Program disassembly frame
  pgmfrm = gtk_frame_new( 0 );
  gtk_frame_set_shadow_type( GTK_FRAME( pgmfrm ), GTK_SHADOW_ETCHED_OUT );
  gtk_widget_set_usize( GTK_WIDGET( pgmfrm ), PGM_WIN_WIDTH, PGM_WIN_HEIGHT );
  gtk_fixed_put( GTK_FIXED( emumainfixed ), pgmfrm, REG_WIN_WIDTH + 10, BUTTONS_BAR_HEIGHT );
  pgmwin = new PgmWin( pgmfrm, CPU );
  gtk_widget_show( pgmfrm );

  // Memory dump frame
  memfrm = gtk_frame_new( 0 );
  gtk_frame_set_shadow_type( GTK_FRAME( memfrm ), GTK_SHADOW_ETCHED_OUT );
  gtk_widget_set_usize( GTK_WIDGET( memfrm ), MEM_WIN_WIDTH, MEM_WIN_HEIGHT );
  gtk_fixed_put( GTK_FIXED( emumainfixed ), memfrm, 0, REG_WIN_HEIGHT + BUTTONS_BAR_HEIGHT );
  memwin = new MemWin( memfrm );
  gtk_widget_show( memfrm );

  
  if ( EmuGtkNumber >= 1 )
    g_print( "WARNING! Signal too much EmuGtk Objects to handle signals!\n");
  else
    {
      EmuGtkPtr = this;
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
      
      EmuGtkNumber++;
    }

  if ( argc > 1 )
    LoadHexFile( argv[1] );
}



//////////////////////////////////////////////////////////////////////////////
// void AddButtons()
// Create and show the Reset, Run, Stop, Trace and Step buttons
//////////////////////////////////////////////////////////////////////////////
void EmuGtk::AddButtons( void )
{
  //GtkStyle *Style = gtk_widget_get_style( GTK_WIDGET( emuwin ) );

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
// EmuGtk::~EmuGtk( )
// EmuGtk destructor
//////////////////////////////////////////////////////////////////////////////
EmuGtk::~EmuGtk( )
{
  g_print( "EmuGtk::~EmuGtk( )\n" );
}

//////////////////////////////////////////////////////////////////////////////
// void EmuGtk::Reset( )
// CPU reset and Gtk UI update
//////////////////////////////////////////////////////////////////////////////
void EmuGtk::Reset( )
{
  CPU->Reset( );
  regwin->Show( CPU );
  pgmwin->Disasm( );
  memwin->DumpD( CPU, 0 );
}

//////////////////////////////////////////////////////////////////////////////
// void EmuGtk::Step( )
// CPU Step and Gtk UI update
//////////////////////////////////////////////////////////////////////////////
void EmuGtk::Step( )
{
  CPU->Exec( );
  regwin->Show( CPU );
  pgmwin->Disasm( );
  memwin->DumpD( CPU, 0 );
}

//////////////////////////////////////////////////////////////////////////////
// void EmuGtk::Main( )
// Gtk UI Main function
//////////////////////////////////////////////////////////////////////////////
void EmuGtk::Main( )
{
  Reset( );
  gtk_main();
  g_print( "End of EmuGtk::Main( )\n" );
}


//////////////////////////////////////////////////////////////////////////////
// void EmuGtk::ShowMenu( )
// Show the menu
//////////////////////////////////////////////////////////////////////////////
void EmuGtk::ShowMenu( )
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
// gint EmuGtk::DeleteEvent( GtkWidget *widget, GdkEvent *event, gpointer data )
// Signal DeleteEvent
//////////////////////////////////////////////////////////////////////////////
gboolean EmuGtk::DeleteEvent( GtkWidget *widget, GdkEvent *event, gpointer data )
{
  g_print( "EmuGtk::DeleteEvent(...)\n" );
  StopRunning( );
  return FALSE;
}

//////////////////////////////////////////////////////////////////////////////
// gint EmuGtk::DestroyEvent( GtkWidget *widget, gpointer data )
// Signal DestroyEvent
//////////////////////////////////////////////////////////////////////////////
void EmuGtk::DestroyEvent( GtkWidget *widget, gpointer data )
{
  g_print( "EmuGtk::DestroyEvent(...)\n" );
  gtk_main_quit();
}

//////////////////////////////////////////////////////////////////////////////
// gint EmuGtk::AboutEvent( GtkWidget *widget, gpointer data )
// Signal AboutEvent ( Help->About in menu )
//////////////////////////////////////////////////////////////////////////////
void EmuGtk::AboutEvent( GtkWidget *widget, gpointer data )
{
  char about_string[256];
  GtkWidget *about_window;
  GtkWidget *text_window;

  sprintf( about_string, "%s\n\nversion %s\n\n\nAuthors:\nHugo Villeneuve\nJonathan St-André\n", PACKAGE, VERSION );
  
  about_window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
  gtk_window_set_title( GTK_WINDOW( about_window ), "About" );
  gtk_container_set_border_width( GTK_CONTAINER( about_window ), 20 );
  
  text_window = gtk_label_new( about_string );
  gtk_container_add( GTK_CONTAINER( about_window ), text_window );
  
  gtk_widget_show_all( GTK_WIDGET( about_window ) );
}

//////////////////////////////////////////////////////////////////////////////
// gint EmuGtk::OpenEvent( GtkWidget *widget, gpointer data )
// Signal OpenEvent ( File->Open in menu )
//////////////////////////////////////////////////////////////////////////////
void EmuGtk::OpenEvent( GtkWidget *widget, gpointer data )
{
  GtkWidget *FileOpendialog;

  // g_print( "EmuGtk::OpenEvent(...)\n" );

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

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void FileOpenDialog_OK( GtkButton *button, gpointer data )
{
  g_print( "EmuGtk::FileOpenDialog_OK Event(...)\n" );

  const gchar *SelectedFile;

  SelectedFile = gtk_file_selection_get_filename ( GTK_FILE_SELECTION ( data ) );
  
  g_print( "EmuGtk::File = %s\n", SelectedFile );
  
  EmuGtkPtr->StopRunning( );

  EmuGtkPtr->LoadHexFile( SelectedFile );

  gtk_widget_destroy( GTK_WIDGET( data ) );

  EmuGtkPtr->Reset( );
  EmuGtkPtr->UpdateDisplay();
}


void EmuGtk::UpdateDisplay( void )
{
  regwin->Show( CPU );
  pgmwin->Disasm( );
  memwin->DumpD( CPU, 0 );
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void FileOpenDialog_CANCEL( GtkButton *button, gpointer data )
{
  g_print( "EmuGtk::FileOpenDialog_CANCEL Event(...)\n" );

  gtk_widget_destroy( GTK_WIDGET( data ) );
}



//////////////////////////////////////////////////////////////////////////////
// gint EmuGtk::QuitEvent( GtkWidget *widget, gpointer data )
// Signal QuitEvent ( File->Quit in menu )
//////////////////////////////////////////////////////////////////////////////
void EmuGtk::QuitEvent( GtkWidget *widget, gpointer data )
{
  g_print( "EmuGtk::QuitEvent(...)\n" );
  StopRunning( );
  gtk_main_quit( );
}

//////////////////////////////////////////////////////////////////////////////
// gint EmuGtk::ResetEvent( GtkWidget *widget, GdkEvent *event, gpointer data )
// Signal ResetEvent ( ResetButton )
//////////////////////////////////////////////////////////////////////////////
void EmuGtk::ResetEvent( GtkWidget *widget, GdkEvent *event, gpointer data )
{
  g_print( "EmuGtk::ResetEvent(...)\n" );
  StopRunning( );
  Reset( );
}

//////////////////////////////////////////////////////////////////////////////
// gint EmuGtk::RunEvent( GtkWidget *widget, GdkEvent *event, gpointer data )
// Signal RunEvent ( RunButton )
//////////////////////////////////////////////////////////////////////////////
void EmuGtk::RunEvent( GtkWidget *widget, GdkEvent *event, gpointer data )
{
  g_print( "EmuGtk::RunEvent(...)\n" );
  if ( RunningState ) {
    //   g_print( "Getting out of RunningState! \n" );
    StopRunning( );
  }
  else {
    //   g_print( "Going In RunningState! \n" );
    StartRunning( );
  }
}

//////////////////////////////////////////////////////////////////////////////
// gint EmuGtk::StopEvent( GtkWidget *widget, GdkEvent *event, gpointer data )
// Signal StopEvent ( StopButton )
//////////////////////////////////////////////////////////////////////////////
void EmuGtk::StopEvent( GtkWidget *widget, GdkEvent *event, gpointer data )
{
  g_print( "EmuGtk::StopEvent(...)\n" );
  StopRunning( );
}

//////////////////////////////////////////////////////////////////////////////
// gint EmuGtk::StepEvent( GtkWidget *widget, GdkEvent *event, gpointer data )
// Signal StepEvent ( StepButton )
//////////////////////////////////////////////////////////////////////////////
void EmuGtk::StepEvent( GtkWidget *widget, GdkEvent *event, gpointer data )
{
  g_print( "EmuGtk::StepEvent(...)\n" );
  StopRunning( );
  Step( );
}

//////////////////////////////////////////////////////////////////////////////
// unsigned int EmuGtk::Ascii2Hex( string istring, int length = 0 )
// Convert an ascii string to an hexadecimal number
//////////////////////////////////////////////////////////////////////////////
unsigned int EmuGtk::Ascii2Hex( string istring, int length = 0 )
{
  if ( !length || ( length > (int) istring.size() ) )
    length = istring.size();
  
  if ( istring.empty() )
    throw MissingParameter();
  
  unsigned int result = 0;
  int i, ascii_code;
  for ( i = 0; i < length; i++ ) {
    ascii_code = istring[ i ];
    if ( ascii_code > 0x39 )
      ascii_code &= 0xDF;
    if ( ( ascii_code >= 0x30 && ascii_code <= 0x39 ) || ( ascii_code >= 0x41 && ascii_code <= 0x46 ) ) {
      ascii_code -= 0x30;
      if ( ascii_code > 9 )
	ascii_code -= 7;
      result <<= 4;
      result += ascii_code;
    }
    else {
      throw SyntaxError();
    }
  }
  return result;
}

//////////////////////////////////////////////////////////////////////////////
// void EmuGtk::LoadHexFile( string Filename )
// Load an HEX file into program memory
//////////////////////////////////////////////////////////////////////////////
void EmuGtk::LoadHexFile( string Filename )
{
  printf("LoadHex\n");
  int i, j, RecLength, LoadOffset, RecType, Data, Checksum;
  char Line[ 250 ];

  ifstream HexFile( Filename.c_str() );
  try { 
    if ( ! HexFile )
      throw ErrorOpeningFile();
    
    while( ! HexFile.eof() ) {
      i = 0;
      Checksum = 0;
      HexFile.getline( Line, 250, '\n' );  

      if ( Line[ i++ ] != ':' )
	throw ErrorHexFileFormat();
      
      RecLength = Ascii2Hex( &Line[ i ], 2 );
      i += 2;
      Checksum += RecLength;
      
      LoadOffset = Ascii2Hex( &Line[i], 4 );
      Checksum += LoadOffset / 256;
      Checksum += LoadOffset % 256;
      i += 4;

      RecType = Ascii2Hex( &Line[i],2);
      i += 2;
      Checksum += RecType;

      if ( RecType == 1 ) {
	Checksum += Ascii2Hex( &Line[ i ], 2 );
	if ( Checksum &= 0x000000FF )
	  throw ErrorHexFileFormat();
	throw FinishedLoading();
      }   
      if ( RecType )
	throw ErrorHexFileFormat();
      
      for ( j = 0; j < RecLength; j++ ) {
	Data = Ascii2Hex( &Line[ i ], 2 );
	CPU->WritePGM( (unsigned int)(LoadOffset + j), (unsigned char)Data );
	i += 2;
	Checksum += Data;
      }
      RecType = Ascii2Hex( &Line[ i ], 2 );
      Checksum += RecType;      

      if ( Checksum &= 0x000000FF )
	throw ErrorHexFileFormat();
    }
    throw ErrorHexFileFormat();
  }
  catch ( ErrorOpeningFile ) {
    cout << "Error opening file " << Filename << endl;
  }
  catch ( ErrorHexFileFormat ) {
    cout << "Invalid format for " << Filename << " file..." << endl;
  }
  catch ( SyntaxError ) {
    cout << "Invalid format for " << Filename << " file..." << endl;
  }
  catch ( MissingParameter ) {
    cout << "Invalid format for " << Filename << " file..." << endl;
  }
  catch ( FinishedLoading ) {
    cout << "Using file " << Filename << " as input program." << endl;  
  }
  HexFile.close();  
}

//////////////////////////////////////////////////////////////////////////////
// gint EmuGtkSignalStub2( GtkWidget *widget, gpointer data )
// Signal Stub with 2 parameters
//////////////////////////////////////////////////////////////////////////////
void EmuGtkSignalStub2( GtkWidget *widget, gpointer data )
{
  //g_print( "EmuGtkSignalStub2(...)\n");
  int SigNumber = (* ( static_cast< int * >( data ) ) );
  
  switch( SigNumber )
    {
    case DestroySignal:
      EmuGtkPtr->DestroyEvent( widget, 0 );
      break;
    case AboutISignal:
      EmuGtkPtr->AboutEvent( widget, 0 );
      break;
    case OpenISignal:
      EmuGtkPtr->OpenEvent( widget, 0 );
      break;
    case QuitISignal:
      EmuGtkPtr->QuitEvent( widget, 0 );
      break;
    default:
      g_print( "*** error: EmuGtkSignalStub2: default case reached\n" );
      break;
    };
}


//////////////////////////////////////////////////////////////////////////////
// gint EmuGtkSignalStub3( GtkWidget *widget, GdkEvent *event, gpointer data )
// Signal Stub with 3 parameters
//////////////////////////////////////////////////////////////////////////////
void EmuGtkSignalStub3( GtkWidget *widget, GdkEvent *event, gpointer data )
{
  //g_print( "EmuGtkSignalStub3(...)\n");
  int SigNumber = (* ( static_cast< int * >( data ) ) );
  
  switch( SigNumber )
    {
    case DeleteSignal:
      EmuGtkPtr->DeleteEvent( widget, event, 0 );
      break;
    case ResetBSignal:
      EmuGtkPtr->ResetEvent( widget, event, 0 );
      break;
    case RunBSignal:
      EmuGtkPtr->RunEvent( widget, event, 0 );
      break;
    case StopBSignal:
      EmuGtkPtr->StopEvent( widget, event, 0 );
      break;
    case StepBSignal:
      EmuGtkPtr->StepEvent( widget, event, 0 );
      break;
    default:
      g_print( "*** error: EmuGtkSignalStub3: default case reached\n" );
      break;
    };
}

//////////////////////////////////////////////////////////////////////////////
// void EmuGtk::Running( )
// Running called by RunningFunction( )
//////////////////////////////////////////////////////////////////////////////
void EmuGtk::Running( )
{
  CPU->Exec( );
  if ( pgmwin->IsBreakpoint( CPU->GetPC( ) ) ) {
    g_print( "Breakpoint Hit, stopping!\n" );
    StopRunning( );
  }
}

//////////////////////////////////////////////////////////////////////////////
// gint RunningFunction( )
// RunningFunction called when idle from gtk_main
//////////////////////////////////////////////////////////////////////////////
gint RunningFunction( )
{
  EmuGtkPtr->Running( );
  return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
// void EmuGtk::StartRunning( )
// Get in the RunningState
//////////////////////////////////////////////////////////////////////////////
void EmuGtk::StartRunning( )
{
  if ( !RunningState ) {
    printf( "EmuGtk::StartRunning( )\n" );
    RunFuncTag = gtk_idle_add( GtkFunction( RunningFunction ),0 );
    RunningState = 1;
    // gtk_widget_hide( GTK_WIDGET( ButtonRun ) );
    // gtk_widget_show_now( GTK_WIDGET( ButtonStop ) );
    // gtk_table_attach_defaults( GTK_TABLE( ButtonTable ), ButtonStop, 3, 4, 0, 1);
  }
}

//////////////////////////////////////////////////////////////////////////////
// void EmuGtk::StopRunning( )
// Step out of RunningState
//////////////////////////////////////////////////////////////////////////////
void EmuGtk::StopRunning( )
{
  if (RunningState) {
    printf( "EmuGtk::StopRunning( )\n" );
    gtk_idle_remove( RunFuncTag );
    RunningState = 0;
    //gtk_widget_hide( GTK_WIDGET( ButtonStop ) );
    //gtk_widget_show( GTK_WIDGET( ButtonRun ) );
    //    gtk_table_attach_defaults( GTK_TABLE( ButtonTable ), ButtonRun, 3, 4, 0, 1);
    regwin->Show( CPU );
    pgmwin->Disasm( );
    memwin->DumpD( CPU, 0 );
  }
}
