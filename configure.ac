# configure.ac -- Process this file with autoconf to produce configure

dnl Initialization stuff.
AC_INIT(emu8051, 1.0.2)
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR(src/cpu8051.c)
AM_CONFIG_HEADER(config.h:config-h.in)
dnl Checking if the NEWS file has been updated to reflect the current version.
AM_INIT_AUTOMAKE(check-news)

dnl Tests the C compiler
AC_PROG_CC
AC_LANG_C

dnl Checking for header files.
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS(strings.h unistd.h)

dnl Checking for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_PID_T
AC_TYPE_SIZE_T

dnl Basic CFLAGS values
CFLAGS="-Wall"

dnl Checks for '--enable-debug' option
HV_CHECK_FOR_DEBUG

if test x"${debug_messages}" = x1; then
dnl -g is for GDB debugging
  CFLAGS="${CFLAGS} -g -gdwarf-2 -g3"
fi

dnl Checks for Gtk+-2.0
PKG_CHECK_MODULES(GTK, gtk+-2.0 >= 2.0.5)
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

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
