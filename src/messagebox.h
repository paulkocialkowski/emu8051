/* messagebox.h */


#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H 1

#include <gtk/gtk.h>


#define MESSAGE_DIALOG_NORMAL_FONT 0
#define MESSAGE_DIALOG_FIXED_FONT  1


void
ShowMessage( gchar *title, gchar *message, int justification, int font_style );


#endif /* MESSAGEBOX_H */
