AM_CFLAGS = \
	-Wall

AM_CPPFLAGS = \
	-include Python.h

AM_CXXFLAGS = \
	-Wall \
	-fno-exceptions \
	@ENIGMA2_CFLAGS@ \
	@GSTREAMER_CFLAGS@ \
	@LIBCRYPTO_CFLAGS@ \
	@PTHREAD_CFLAGS@

PLUGIN_LIBTOOL_FLAGS = \
	-avoid-version \
	-module \
	-shared
