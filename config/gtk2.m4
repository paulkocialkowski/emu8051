dnl
dnl Macro for adding an option to 'configure' for choosing GTK+-2 instead of the
dnl GTK+-1 default
dnl
AC_DEFUN([HV_CHECK_FOR_GTK2],[dnl
AC_ARG_WITH(gtk2, AC_HELP_STRING([--with-gtk2], [use GTK2 (default is GTK1)]),[dnl
  if test x"${withval}" = xyes; then
    use_gtk2=1
    AC_DEFINE([USE_GTK2],1,[Set to 1 to use the Gtk+-2 library.])
  elif test x"${withval}" = xno; then
    use_gtk2=0
  else
    AC_MSG_ERROR(bad value for --with-gtk2 option)
  fi
], use_gtk2=0 )
])

