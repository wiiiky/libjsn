## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\"

AM_CFLAGS =\
	 -Wall\
	 -g \
	 -I../src/ \
	 -std=gnu11

AM_LDFLAGS = \
	-L../src/ -ljsn

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
