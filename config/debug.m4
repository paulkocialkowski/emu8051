dnl
dnl Macro for adding an option to 'configure' for enabling debugging messages
dnl
AC_DEFUN([HV_CHECK_FOR_DEBUG],[
AC_ARG_ENABLE(debug, AC_HELP_STRING([--enable-debug],
                                    [enable debugging messages on console
                                     (default is NO)]),[
  if test x"${enableval}" = xyes; then
    debug_messages=1
    AC_DEFINE([DEBUG],1,[Set to 1 to enable debugging messages.])
  elif test x"${enableval}" = xno; then
    debug_messages=0
  else
    AC_MSG_ERROR(bad value for --enable-debug option)
  fi
], debug_messages=0 )
])

