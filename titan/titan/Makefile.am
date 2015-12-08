ALL_CFLAGS= -DSH4 -DEPLAYER4 -DDVDPLAYER -Os -export-dynamic -Wall -Wno-unused-but-set-variable

INCLUDES = \
	-I../../../tufsbox/cdkroot/usr/include \
	-I../../../tufsbox/cdkroot/usr/include/freetype2 \
	-I../../../tufsbox/cdkroot/usr/include/gstreamer-1.0 \
	-I../../../tufsbox/cdkroot/usr/include/glib-2.0 \
	-I../../../tufsbox/cdkroot/usr/include/libxml2 \
	-I../../../tufsbox/cdkroot/usr/lib/glib-2.0/include \ 
	-I../../../tufsbox/cdkroot/usr/lib/gstreamer-1.0/include \
	-I../../driver/bpamem \
	-I../../../apps/titan/libdreamdvd \
	-I../../../apps/tools/libeplayer3/include \
	-I../../../apps/tools/libmmeimage \
	-I../../../apps/titan/titan

bin_PROGRAMS = titan

titan_SOURCES =	titan.c

titan_LDADD = -lm -lglib-2.0 -lgobject-2.0 -lgio-2.0 -lpthread -ldl -lz -lpng -lfreetype -lgstreamer-1.0 -ldreamdvd -ljpeg -lmmeimage -lmme_host -lz
titan_CFLAGS = $(ALL_CFLAGS)
