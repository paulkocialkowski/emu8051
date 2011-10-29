# configure.ac -- Process this file with autoconf to produce configure

dnl Initialization stuff.
AC_INIT(emu8051, 1.1.0)
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR(src/cpu8051.c)
AM_CONFIG_HEADER(config.h:config-h.in)
dnl Checking if the NEWS file has been updated to reflect the current version.
AM_INIT_AUTOMAKE(check-news)
AM_SILENT_RULES([yes])

dnl Testing the C compiler.
AM_PROG_CC_C_O
AC_LANG_C

dnl Checking for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_PID_T
AC_TYPE_SIZE_T

dnl Basic CFLAGS values
CFLAGS="${CFLAGS} -Wall"

dnl Checks for '--enable-debug' option
HV_CHECK_FOR_DEBUG

if test x"${debug_messages}" = x1; then
dnl -g is for GDB debugging
  CFLAGS="${CFLAGS} -g -gdwarf-2 -g3"
fi

dnl Checks for Gtk+-2.0
AC_ARG_ENABLE(gui,
	[  --enable-gui     Enable building the GUI (default=yes)],
	[ac_cv_enable_gui=$enableval], [ac_cv_enable_gui=yes])
AC_MSG_CHECKING([whether to build GUI])
if test x$ac_cv_enable_gui = xyes; then
	AC_MSG_RESULT(yes)
        PKG_CHECK_MODULES(GTK, gtk+-2.0 >= 2.0.5, :, ac_cv_enable_gui=no)
	if test x$ac_cv_enable_gui = xyes; then
	    AC_DEFINE([HAVE_GTK],1,[Set to 1 to enable GTK+ support for building GUI.])
            AC_SUBST(GTK_CFLAGS)
            AC_SUBST(GTK_LIBS)
	fi
else
	AC_MSG_RESULT(no)
fi

AM_CONDITIONAL([USE_GTK], [test x${ac_cv_enable_gui} = xyes])

AC_SUBST(CFLAGS)
AC_SUBST(LIBS)
AC_SUBST(ac_aux_dir)

dnl Creating output file(s)
AC_OUTPUT(Makefile src/Makefile doc/Makefile)

echo \
"---------------------------------------------------------------------------
Configuration:

  Install path:        ${prefix}
  Compiler:            ${CC}
  Compiler flags:      ${CFLAGS}
  Linker flags:        ${LIBS}"

echo -n "  Build GUI:           "
if test x"${ac_cv_enable_gui}" = xyes; then
  echo "yes"
else
  echo "no"
fi

echo -n "  Debugging messages:  "
if test x"${debug_messages}" = x1; then
  echo "yes"
else
  echo "no"
fi

echo \
"
  See config.h for further configuration information.
---------------------------------------------------------------------------"
