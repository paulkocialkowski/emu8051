/*
 * options.h
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
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef OPTIONS_H
#define OPTIONS_H 1


#define COMMAND_LINE_OPTIONS \
  "Usage: " PACKAGE " [OPTION]... [FILENAME]\n" \
  "Simulator/emulator for 8051 family microcontrollers.\n\n" \
  "  -h                    display this help and exit\n" \
  "  -version              display version information and exit\n"

#define VERSION_STRING \
  PACKAGE "\n" \
  "Version " VERSION "\n" \
  "\n" \
  "Written by\n" \
  "Hugo Villeneuve\n" \
  "Jonathan St-André\n" \
  "Pascal Fecteau"


void
ParseCommandLineOptions( int argc, char *argv[] );


#endif /* OPTIONS_H */
