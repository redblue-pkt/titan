lib_LTLIBRARIES = libgmediarender.la
libgmediarender_la_SOURCES = gmediarender.c

AM_CPPFLAGS = -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE
ALL_CFLAGS= -DSH4 -DEPLAYER4 -DDVDPLAYER -Os -export-dynamic -Wall -Wno-unused-but-set-variable

CFLAGS = \
	-I../../../../tufsbox/cdkroot/usr/include \
	-I../../../../tufsbox/cdkroot/usr/include/freetype2 \
	-I../../../../tufsbox/cdkroot/usr/include/gstreamer-1.0 \
	-I../../../../tufsbox/cdkroot/usr/include/glib-2.0 \
	-I../../../../tufsbox/cdkroot/usr/include/libxml2 \
	-I../../../../tufsbox/cdkroot/usr/lib/glib-2.0/include \ 
	-I../../../../tufsbox/cdkroot/usr/lib/gstreamer-1.0/include \
	-I../../../driver/bpamem \
	-I../../../../apps/titan/libdreamdvd \
	-I../../../../apps/tools/libeplayer3/include \
	-I../../../../apps/tools/libmmeimage \
	-I../../../../apps/titan/titan