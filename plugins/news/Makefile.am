ALL_CFLAGS= -DSH4 -Deplayer3 -DDVDPLAYER -Os -export-dynamic -Wall
LDFLAGS= -Wl,-soname,
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

bin_PROGRAMS = news.so

news_SOURCES = news.c
