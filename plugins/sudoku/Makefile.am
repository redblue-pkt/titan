lib_LTLIBRARIES = libsudoku.la
libsudoku_la_SOURCES = sudoku.c

bin_PROGRAMS = sudoku_core
sudoku_core_SOURCES = sudoku_core.c

AM_CPPFLAGS = -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE
ALL_CFLAGS= -DSH4 -Deplayer3 -DDVDPLAYER -Os -export-dynamic -Wall -Wno-unused-but-set-variable
