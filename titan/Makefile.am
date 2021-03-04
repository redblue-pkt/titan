ALL_CFLAGS= -DSH4 -DEPLAYER3 -DDVDPLAYER -Os -export-dynamic -Wall -Wno-unused-but-set-variable

INCLUDES = \
	-I../../../tufsbox/cdkroot/usr/include \
	-I../../../tufsbox/cdkroot/usr/include/freetype2 \
	-I../../../tufsbox/cdkroot/usr/include/openssl \
	-I../../driver/bpamem \
	-I../../../apps/tools/libeplayer3/include \
	-I../../../apps/tools/libmme_image \
	-Ilibdreamdvd \
	-Ititan

bin_PROGRAMS = titan

titan_SOURCES =	titan.c

titan_LDADD = -lm -lpthread -ldl -lpng -lfreetype -leplayer3 -ldreamdvd -ljpeg -lz -lmmeimage -lmme_host -lssl -lcrypto
titan_CFLAGS = $(ALL_CFLAGS)
