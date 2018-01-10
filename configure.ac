# configure.ac -- Process this file with autoconf to produce configure

dnl Initialization stuff.
AC_INIT([emu8051], [2.0.1], [hugo@hugovil.com], [emu8051],
	[http://www.hugovil.com/emu8051/])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR(src/common/cpu8051.c)

dnl -Wno-extra-portability:
dnl   To get rid of message:
dnl     linking libraries using a non-POSIX archiver requires 'AM_PROG_AR'...
dnl -Wall:
dnl   Ask automake to turn on all warnings (not a gcc flag)
AM_INIT_AUTOMAKE([no-define gnits dist-bzip2 color-tests
                  -Wall -Wno-extra-portability])
AM_SILENT_RULES([yes])
AM_CONFIG_HEADER(config.h:config-h.in)

dnl Testing the C compiler.
AC_LANG_C

dnl Testing for libtool support.
AM_PROG_LIBTOOL

AC_ARG_WITH([readline],
            [AS_HELP_STRING([--without-readline], [disable support for readline])],
            [],
            [with_readline=yes])

LIBREADLINE=
    AS_IF([test "x$with_readline" != xno],
	  [AC_CHECK_LIB([readline], [main],
              [AC_SUBST([LIBREADLINE], ["-lreadline"])
               AC_DEFINE([HAVE_LIBREADLINE], [1],
                         [Define if you have libreadline])
              ],
              [AC_MSG_FAILURE(
                  [readline test failed (--without-readline to disable)])],
              []
)])

dnl Testing for Lex/Yacc
AM_PROG_LEX
AC_PROG_YACC

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

dnl Checks for emu8051-device
AC_ARG_ENABLE(device, AS_HELP_STRING([--enable-device], [enable device support]),
	[ac_cv_enable_device=$enableval], [ac_cv_enable_device=yes])
AC_MSG_CHECKING([whether to build device support])
if test x$ac_cv_enable_device = xyes; then
	AC_MSG_RESULT(yes)
	PKG_CHECK_MODULES(EMU8051_DEVICE, emu8051-device, [], [ac_cv_enable_device=no])

	if test x$ac_cv_enable_device = xyes; then
		AC_DEFINE([HAVE_DEVICE],1,[Set to 1 to enable device support.])
		AC_SUBST(EMU8051_DEVICE_CFLAGS)
		AC_SUBST(EMU8051_DEVICE_LIBS)
	fi
else
	AC_MSG_RESULT(no)
fi

AM_CONDITIONAL([DEVICE], [test x$ac_cv_enable_device = xyes])

dnl Checks for kb9012
AC_ARG_ENABLE(kb9012, AS_HELP_STRING([--enable-kb9012], [enable kb9012 emulation support]),
	[ac_cv_enable_kb9012=$enableval], [ac_cv_enable_kb9012=no])

AC_MSG_CHECKING([whether to build kb9012 emulation support])
if test x$ac_cv_enable_kb9012 = xyes; then
	AC_MSG_RESULT(yes)
	AC_DEFINE([HAVE_KB9012],1,[Set to 1 to enable kb9012 emulation support.])
else
	AC_MSG_RESULT(no)
fi

AM_CONDITIONAL([KB9012], [test x$ac_cv_enable_kb9012 = xyes])

dnl Checks for interactive
AC_ARG_ENABLE(interactive, AS_HELP_STRING([--enable-interactive], [enable interactive serial support]),
	[ac_cv_enable_interactive=$enableval], [ac_cv_enable_interactive=yes])

AC_MSG_CHECKING([whether to build interactive serial support])
if test x$ac_cv_enable_interactive = xyes; then
	AC_MSG_RESULT(yes)
	AC_DEFINE([HAVE_INTERACTIVE],1,[Set to 1 to enable interactive serial support.])
else
	AC_MSG_RESULT(no)
fi

AM_CONDITIONAL([INTERACTIVE], [test x$ac_cv_enable_interactive = xyes])

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

dnl zlib required for its crc32 function
ac_have_zlib=no

PKG_CHECK_MODULES([zlib], [zlib > 1.2.1],
		  [AC_CHECK_LIB([z], [crc32],
				[ac_have_zlib=yes],
				[ac_have_zlib=no])])

if test x"$ac_have_zlib" = x"yes" ; then
    ZLIB_LIBS='-lz'
    AC_SUBST(ZLIB_LIBS)
else
    AC_MSG_ERROR([Please install zlib and zlib-devel packages])
fi

AC_SUBST(WARNINGCFLAGS)
AC_SUBST(ac_aux_dir)

dnl Creating output file(s)
AC_OUTPUT(Makefile
    src/common/Makefile
    src/cli/Makefile
    src/gtk/Makefile
    data/Makefile
    doc/Makefile
    tests/Makefile)
