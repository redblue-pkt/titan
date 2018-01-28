lib_LTLIBRARIES = libhbbtv.la
libhbbtv_la_SOURCES = hbbtv.c

AM_CPPFLAGS = -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE
ALL_CFLAGS= -DARM -DMIPSEL -DCAMSUPP -DEPLAYER3AUS -DEXTEPLAYER3AUS -Os -mhard-float -export-dynamic -Wall -Wno-unused-but-set-variable -Wno-implicit-function-declaration

CFLAGS = \
	-I../../../../../../sysroots/hd51/usr/usr/include \
	-I../../../../../../sysroots/hd51/usr/usr/include/freetype2 \
	-I../../../../../../sysroots/hd51/usr/usr/include/gstreamer-1.0 \
	-I../../../../../../sysroots/hd51/usr/usr/include/glib-2.0 \
	-I../../../../../../sysroots/hd51/usr/usr/include/libxml2 \
	-I../../../../../../sysroots/hd51/usr/usr/lib/glib-2.0/include \ 
	-I../../../../../../sysroots/hd51/usr/usr/lib/gstreamer-1.0/include \
	-I../../../driver/bpamem \
	-I../../../../apps/titan/libdreamdvd \
	-I../../../../apps/tools/libeplayer3/include \
	-I../../../../apps/tools/libmmeimage \
	-I../../../../apps/titan/titan