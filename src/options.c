/*
 * Processing command-line options and arguments.
 *
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

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#if STDC_HEADERS
#  include <string.h>
#elif HAVE_STRINGS_H
#  include <strings.h>
#endif

#include "common.h"
#include "options.h"

static char *hex_file;

/*
 * Return the hex file name
 */
char *
get_hex_filename(void)
{
	return hex_file;
}

/*******************************************************************************
 * Display the help message and exit
 ******************************************************************************/
static void
DisplayUsage( void )
{
  printf( COMMAND_LINE_OPTIONS );
}


/*******************************************************************************
 * Display version information and exit
 ******************************************************************************/
static void
DisplayVersion( void )
{
  printf( "\n" );
  printf( "  %s, version %s\n", PACKAGE, VERSION );
  printf( "  Written by Jonathan St-André, Pascal Fecteau and Hugo Villeneuve\n\n" );
}


static void
InvalidOption( const char *message, /*@null@*/ const char *string )
{
  if( string == NULL ) {
    fprintf(stderr, "%s: %s\n", PACKAGE, message );
  }
  else {
    fprintf(stderr, "%s: %s %s\n", PACKAGE, message, string );
  }

  fprintf(stderr, "Try `%s -h' for more information.\n", PACKAGE );

  exit( EXIT_FAILURE );
}


/*******************************************************************************
 * Initializes the different options passed as arguments on the command line.
 ******************************************************************************/
void
ParseCommandLineOptions( int argc, char *argv[] )
{
  int i;
  char *token;

  for( i = 1; i < argc; i++ ) {
    token = argv[i];
    switch( token[0] ) {
    case '-':
      /* Processing options names */
      switch( token[1] ) {
      case 'h':
	if( strlen( &token[1] ) == 1 ) {
	  DisplayUsage();
	  exit( EXIT_SUCCESS );
	}
	InvalidOption( "invalid option", token );
	break;
      case 'v' :
	if( STREQ( "version", &token[1] ) ) {
	  DisplayVersion();
	  exit( EXIT_SUCCESS );
	}
	else {
	  InvalidOption( "invalid option", token );
	}
	break;
      default:
	InvalidOption( "invalid option", token );
	break;
      } /* end switch( token[1] ) */
      break;
    default:
      /* Processing options arguments */
      /* Must be the filename... */
      hex_file = token;
      break;
    } /* end switch( token[0] ) */
  } /* end for */
  
}
