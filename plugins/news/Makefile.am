ALL_CFLAGS= -DSH4 -Deplayer3 -DDVDPLAYER -Os -export-dynamic -Wall

#AM_CFLAGS= -nostartfiles
#AM_LDFLAGS= -shared -nostartfiles
LDFLAGS= -Wl,-soname,
#LDFLAGS= -shared

#AM_CFLAGS= -fpic -nostartfiles -shared -fPIC -static-libgcc
AM_CFLAGS= -shared -Os -fPIC -Wall -nostartfiles -Deplayer3 -DDVDPLAYER

INCLUDES = \
	-I/home/atemio/flashimg/BUILDGIT/checkout_stm24/tdt/tufsbox/cdkroot/usr/include \
	-I/home/atemio/flashimg/BUILDGIT/checkout_stm24/tdt/cvs/apps/misc/tools/libeplayer3/include \
	-I/home/atemio/flashimg/BUILDGIT/checkout_stm24/tdt/tufsbox/cdkroot/usr/include/freetype2 \
	-I/home/atemio/flashimg/BUILDGIT/checkout_stm24/tdt/cvs/apps/misc/tools/libmmeimage \
	-I/home/atemio/flashimg/BUILDGIT/checkout_stm24/tdt/cvs/driver/bpamem \
	-I/home/atemio/flashimg/BUILDGIT/checkout_stm24/tdt/cvs/apps/titan/titan/libdreamdvd \
	-I/home/atemio/flashimg/BUILDGIT/checkout_stm24/tdt/cvs/apps/misc/tools/libmmeimage \
	-I/home/atemio/flashimg/BUILDGIT/checkout_stm24/tdt/cvs/apps/titan/titan

#lib_LTLIBRARIES = libExample.la 
#libExample_la_SOURCES = news.c

bin_PROGRAMS = news

news_SOURCES = news.c
#news_LDADD = libExample.la

#news_so_LDADD= $(ALL_CFLAGS)-shared
#news_OBJECTS= -shared

#titan_LDADD = -lpthread -ldl -lpng -lfreetype -leplayer3 -ldreamdvd -ljpeg -lmmeimage -lmme_host -lipkg
#titan_CFLAGS = $(ALL_CFLAGS)
