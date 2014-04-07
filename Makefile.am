## Makefile.am -- Process this file with automake to produce Makefile.in

.PHONY: changelog

SUBDIRS = \
    src/common \
    src/cli \
    src/gtk \
    data \
    doc \
    tests

## We want these in the dist tarball
EXTRA_DIST = pixmaps

ACLOCAL_AMFLAGS = -I m4

LIBTOOL_MACROS = \
    m4/libtool.m4 \
    m4/ltoptions.m4 \
    m4/ltsugar.m4 \
    m4/ltversion.m4 \
    m4/lt~obsolete.m4

CLEANFILES = *~

DISTCLEANFILES = .deps/*.P

MAINTAINERCLEANFILES = \
    Makefile.in \
    aclocal.m4 \
    configure \
    config-h.in \
    stamp-h.in \
    ChangeLog \
    $(ac_aux_dir)/depcomp \
    $(ac_aux_dir)/install-sh \
    $(ac_aux_dir)/missing \
    $(ac_aux_dir)/mkinstalldirs \
    $(ac_aux_dir)/config.guess \
    $(ac_aux_dir)/config.sub \
    $(ac_aux_dir)/ltmain.sh \
    $(ac_aux_dir)/compile \
    $(ac_aux_dir)/test-driver \
    $(ac_aux_dir)/ar-lib \
    $(ac_aux_dir)/ylwrap \
    $(LIBTOOL_MACROS)

changelog:
	@if test -d $(srcdir)/.git; then          \
	  $(srcdir)/build-aux/gitlog-to-changelog \
	    --format='%s%n%n%b%n'                 \
	    --no-cluster                          \
	    --strip-tab                           \
	    --strip-cherry-pick                   \
	    >ChangeLog;                           \
	fi
