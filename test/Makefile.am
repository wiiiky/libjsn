## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	-DPACKAGE_TEST_DIR=\""$(top_srcdir)/test"\"

AM_CFLAGS =\
	 -Wall\
	 -g \
	 -I"$(top_srcdir)/src" \
	 -std=gnu11

AM_LDFLAGS = \
	-L"$(top_builddir)/src" -ljsn

noinst_PROGRAMS= \
	test-sample \
	test-file \
	test-generator

test_sample_SOURCES = \
	test-sample.c

test_file_SOURCES = \
	test-file.c

test_generator_SOURCES = \
    test-generator.c

TESTS = \
    test-sample \
    test-file \
    test-generator

dist_check_DATA = \
    1.json

