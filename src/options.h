/* options.h */

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
