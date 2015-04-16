## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\"

AM_CFLAGS =\
	 -Wall\
	 -g

lib_LTLIBRARIES = libjsn.la


libjsn_la_SOURCES = \
	jlist.c \
	lib.c

libjsn_la_LDFLAGS =

libjsn_la_LIBADD =


libjsnsubincludedir=$(libjsn_includedir)/libjsn
libjsnsubinclude_HEADERS= \
	libjsn.h \
	jlist.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libjsn-0.1.pc

EXTRA_DIST = \
	libjsn-0.1.pc.in

clean-local:
	indent -kr -ts4 -nut *.c *.h
	rm *~
