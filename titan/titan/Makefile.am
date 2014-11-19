ALL_CFLAGS= -DMIPSEL -DCAMSUPP -DEPLAYER4 -Os -export-dynamic -Wall -Wno-unused-but-set-variable

bin_PROGRAMS = titan

titan_SOURCES =	titan.c

titan_LDADD = -lpthread -ldl -lpng -lfreetype -lgstreamer-0.10 -ldreamdvd -ljpeg
titan_CFLAGS = $(ALL_CFLAGS)