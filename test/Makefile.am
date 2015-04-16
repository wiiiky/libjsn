## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\"

AM_CFLAGS =\
	 -Wall\
	 -g \
	 -I../src/

AM_LDFLAGS = \
	-L../src/ -ljsn

noinst_PROGRAMS= \
	test-sample

test_sample_SOURCES = \
	test.c 

clean-local:
	rm -f *~ test-sample
