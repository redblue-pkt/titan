ALL_CFLAGS= -DSH4 -Deplayer3 -DDVDPLAYER -Os -export-dynamic -Wall

INCLUDES = \
	-I$(targetprefix)/usr/include \
	-I$(targetprefix)/usr/include/freetype2 \
	-I$(driver)/driver/bpamem \
	-I$(appsdir)/titan/libdreamdvd \
	-I$(appsdir)/misc/tools/libeplayer3/include \
	-I$(appsdir)/misc/tools/libmmeimage \
	-I$(appsdir)/titan/titan/libdreamdvd \
	-I$(appsdir)/misc/tools/libmmeimage \
	-I$(appsdir)/titan/titan

bin_PROGRAMS = titan

titan_SOURCES =	titan.c

titan_LDADD = -lpthread -ldl -lpng -lfreetype -leplayer3 -ldreamdvd -ljpeg -lmmeimage -lmme_host -lipkg
titan_CFLAGS = $(ALL_CFLAGS)
