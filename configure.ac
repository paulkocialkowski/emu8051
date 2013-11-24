# configure.ac -- Process this file with autoconf to produce configure

dnl Initialization stuff.
AC_INIT([emu8051], [1.1.2], [hugo@hugovil.com], [emu8051],
	[http://www.hugovil.com/fr/emu8051/])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_SRCDIR(src/cpu8051.c)
AM_CONFIG_HEADER(config.h:config-h.in)
dnl Checking if the NEWS file has been updated to reflect the current version.
AM_INIT_AUTOMAKE(check-news -Wall std-options color-tests parallel-tests)
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

PKG_CHECK_MODULES(GLIB, [glib-2.0 >= 2.26.0])
AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)

dnl Checks for Gtk+-2.0
AC_ARG_ENABLE(gui,
	[  --enable-gui     Enable building the GUI (default=yes)],
	[ac_cv_enable_gui=$enableval], [ac_cv_enable_gui=yes])
AC_MSG_CHECKING([whether to build GUI])
if test x$ac_cv_enable_gui = xyes; then
	AC_MSG_RESULT(yes)
        PKG_CHECK_MODULES(GTK, gtk+-2.0 >= 2.4.0, CFLAGS="${CFLAGS} -DGDK_PIXBUF_DISABLE_DEPRECATED -DGDK_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED", dnl
        ac_cv_enable_gui=no)

	if test x$ac_cv_enable_gui = xyes; then
	    AC_DEFINE([HAVE_GTK],1,[Set to 1 to enable GTK+ support for building GUI.])
            AC_SUBST(GTK_CFLAGS)
            AC_SUBST(GTK_LIBS)
	fi
else
	AC_MSG_RESULT(no)
fi

AM_CONDITIONAL([USE_GTK], [test x${ac_cv_enable_gui} = xyes])

dnl Check if tests are enabled (disabled by default)
AC_ARG_ENABLE([tests],
              [AS_HELP_STRING([--enable-tests],[Run test suite])],
              [run_tests=${enableval}],
              [run_tests="no"])

dnl If tests are enabled, try to locate asem 8051 compiler
if test x"${run_tests}" = xyes; then
    dnl Tests for 8051 assembler to generate hex test files
    dnl TODO: add argument to specify location of asem executable...
    AC_CHECK_PROG(ASEM_CHECK,asem,yes)
    if test x"$ASEM_CHECK" != x"yes" ; then
	 AC_MSG_ERROR([Please install asem (http://plit.de/asem-51) to run test suite.])
    fi
fi

AM_CONDITIONAL([RUN_TESTS],[test "x$run_tests" = "xyes"])

AC_SUBST(CFLAGS)
AC_SUBST(LIBS)
AC_SUBST(ac_aux_dir)

dnl Creating output file(s)
AC_OUTPUT(Makefile src/Makefile data/Makefile doc/Makefile)

if test x"${run_tests}" = xyes; then
    AC_OUTPUT(tests/Makefile)
fi
