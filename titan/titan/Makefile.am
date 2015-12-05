ALL_CFLAGS= -DSH4 -Deplayer3 -DDVDPLAYER -Os -export-dynamic -Wall -Wno-unused-but-set-variable

INCLUDES = \
	-I../../../tufsbox/cdkroot/usr/include \
	-I../../../tufsbox/cdkroot/usr/include/freetype2 \
	-I../../driver/bpamem \
	-I../../../apps/titan/libdreamdvd \
	-I../../../apps/tools/libeplayer3/include \
	-I../../../apps/tools/libmmeimage \
	-I../../../apps/titan/titan

bin_PROGRAMS = titan

titan_SOURCES =	titan.c

titan_LDADD = -lm -lpthread -ldl -lpng -lfreetype -leplayer3 -ldreamdvd -ljpeg -lmmeimage -lmme_host -lz
titan_CFLAGS = $(ALL_CFLAGS)
