ALL_CFLAGS= -DSH4 -Deplayer3 -DDVDPLAYER -Os -export-dynamic -Wall
LDFLAGS= -Wl,-soname,
AM_CFLAGS= -shared -Os -fPIC -Wall -nostartfiles -Deplayer3 -DDVDPLAYER

INCLUDES = \
	-I$(targetprefix)/usr/include \
	-I$(targetprefix)/usr/include/freetype2 \
	-I$(driverdir)/driver/bpamem \
	-I$(appsdir)/titan/libdreamdvd \
	-I$(appsdir)/misc/tools/libeplayer3/include \
	-I$(appsdir)/misc/tools/libmmeimage \
	-I$(appsdir)/titan/libdreamdvd \
	-I$(appsdir)/misc/tools/libmmeimage \
	-I$(appsdir)/titan/titan
bin_PROGRAMS = news.so

news_SOURCES = news.c
