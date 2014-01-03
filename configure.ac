# configure.ac -- Process this file with autoconf to produce configure

dnl Initialization stuff.
AC_INIT([emu8051], [1.1.2], [hugo@hugovil.com], [emu8051],
	[http://www.hugovil.com/fr/emu8051/])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_SRCDIR(src/common/cpu8051.c)
AM_CONFIG_HEADER(config.h:config-h.in)
dnl Checking if the NEWS file has been updated to reflect the current version.
AM_INIT_AUTOMAKE(check-news -Wall std-options color-tests parallel-tests)
AM_SILENT_RULES([yes])

dnl Testing the C compiler.
AC_LANG_C

dnl Testing for libtool support.
AC_PROG_RANLIB
AM_PROG_AR

dnl Checking for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_PID_T
AC_TYPE_SIZE_T

dnl Basic warning CFLAGS values
WARNINGCFLAGS="-Wall -Wextra -Wformat -Wformat-security"

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
        PKG_CHECK_MODULES(GTK, gtk+-2.0 >= 2.4.0, [], dnl
        ac_cv_enable_gui=no)

	if test x$ac_cv_enable_gui = xyes; then
	    AC_DEFINE([HAVE_GTK],1,[Set to 1 to enable GTK+ support for building GUI.])

	    GTK_CFLAGS="${GTK_CFLAGS} -DGDK_PIXBUF_DISABLE_DEPRECATED -DGDK_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED"

            AC_SUBST(GTK_CFLAGS)
            AC_SUBST(GTK_LIBS)
	fi
else
	AC_MSG_RESULT(no)
fi

AM_CONDITIONAL([USE_GTK], [test x${ac_cv_enable_gui} = xyes])

dnl Try to locate valid 8051 assembler to generate test files (.hex)
run_tests=no

AC_CHECK_PROG(AS504_CHECK,as504,yes)
if test x"$AS504_CHECK" = x"yes" ; then
    run_tests=yes

    dnl Check if as504 has been patched to support option -O:
    dnl See http://www.hugovil.com/repository/hvlinux/patches/as504-add-output-file-option.patch
    AS504HV_CHECK=no
    if as504 2>&1 | grep -q Ooutfile; then
        AS504HV_CHECK=yes
    fi
fi

AC_CHECK_PROG(ASEM51_CHECK,asem,yes)
if test x"$ASEM51_CHECK" = x"yes" ; then
    run_tests=yes
fi

if test x"$run_tests" != x"yes" ; then
    AC_MSG_WARN([Please install as504 (http://www.vanwal.nl/as504/) or ASEM-51 (http://plit.de/asem-51) to run test suite.])
fi

AM_CONDITIONAL([RUN_TESTS],[test x"$run_tests" = x"yes"])
AM_CONDITIONAL([USE_AS504HV],[test x"$AS504HV_CHECK" = x"yes"])
AM_CONDITIONAL([USE_AS504],[test x"$AS504_CHECK" = x"yes"])
AM_CONDITIONAL([USE_AS51],[test x"$ASEM51_CHECK" = x"yes"])

AC_SUBST(WARNINGCFLAGS)
AC_SUBST(LIBS)
AC_SUBST(ac_aux_dir)

dnl Creating output file(s)
AC_OUTPUT(Makefile
    src/common/Makefile
    src/cli/Makefile
    src/gtk/Makefile
    data/Makefile
    doc/Makefile
    tests/Makefile)
