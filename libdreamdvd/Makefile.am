ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libdreamdvd.la

libdreamdvd_la_SOURCES = \
	a52_dec.c \
	a52dec.h \
	logo.h \
	main.c \
	main.h \
	mpegaudio_enc.c \
	mpegaudio_enc.h \
	mpegaudioenc.h

libdreamdvd_la_LDFLAGS = -ldvdnav

pkgincludedir = /usr/${includedir}/dreamdvd
pkginclude_HEADERS = ddvdlib.h

pkgconfigdir = /usr/${libdir}/pkgconfig
pkgconfig_DATA = libdreamdvd.pc
