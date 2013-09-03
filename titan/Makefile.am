ALL_CFLAGS= -DSH4 -Deplayer3 -DDVDPLAYER -Os -export-dynamic -Wall -Wno-unused-but-set-variable

INCLUDES = \
	-I../../../../tufsbox/cdkroot/usr/include \
	-I../../../../tufsbox/cdkroot/usr/include/freetype2 \
	-I../../../driver/bpamem \
	-I../../../apps/titan/libdreamdvd \
	-I../../../apps/misc/tools/libeplayer3/include \
	-I../../../apps/misc/tools/libmmeimage \
	-I../../../apps/titan/titan/libdreamdvd \
	-I../../../apps/misc/tools/libmmeimage \
	-I../../../apps/titan/titan

bin_PROGRAMS = titan

titan_SOURCES =	titan.c

titan_LDADD = -lpthread -ldl -lpng -lfreetype -leplayer3 -ldreamdvd -ljpeg -lmmeimage -lmme_host
titan_CFLAGS = $(ALL_CFLAGS)
