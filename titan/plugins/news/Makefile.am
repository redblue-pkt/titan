ALL_CFLAGS= -DSH4 -Deplayer3 -DDVDPLAYER -Os -export-dynamic -Wall
LDFLAGS= -Wl,-soname,
AM_CFLAGS= -shared -Os -fPIC -Wall -nostartfiles -Deplayer3 -DDVDPLAYER

INCLUDES = \
	-I../../../../../tufsbox/cdkroot/usr/include \
	-I../../../../../tufsbox/cdkroot/usr/include/freetype2 \
	-I../../../../driver/bpamem \
	-I../../../../apps/titan/libdreamdvd \
	-I../../../../apps/misc/tools/libeplayer3/include \
	-I../../../../apps/misc/tools/libmmeimage \
	-I../../../../apps/titan/titan/libdreamdvd \
	-I../../../../apps/misc/tools/libmmeimage \
	-I../../../../apps/titan/titan
bin_PROGRAMS = news.so

news_SOURCES = news.c
