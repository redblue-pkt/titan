AUTOMAKE_OPTIONS = subdir-objects
AM_CFLAGS = -Wall -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -D_LARGEFILE_SOURCE -DHAVE_FLV2MPEG4_CONVERTER
#AM_CFLAGS = -Wall -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE

CXXFLAGS = -Wall

AM_CPPFLAGS = \
	-Iinclude \
	-Iexternal \
	-Iexternal/flv2mpeg4	

lib_LTLIBRARIES = libeplayer3.la
libeplayer3_la_SOURCES = \
container/container.c \
container/container_ffmpeg.c \
manager/manager.c \
manager/audio.c \
manager/video.c \
manager/subtitle.c \
output/output_subtitle.c \
output/output.c \
output/writer/common/pes.c \
output/writer/common/misc.c \
output/writer/common/writer.c \
output/linuxdvb_buffering.c \
output/graphic_subtitle.c \
output/linuxdvb_sh4.c \
output/writer/sh4/writer.c \
output/writer/sh4/aac.c \
output/writer/sh4/ac3.c \
output/writer/sh4/divx2.c \
output/writer/sh4/dts.c \
output/writer/sh4/h263.c \
output/writer/sh4/h264.c \
output/writer/sh4/mp3.c \
output/writer/sh4/mpeg2.c \
output/writer/sh4/pcm.c \
output/writer/sh4/vc1.c \
output/writer/sh4/wma.c \
output/writer/sh4/wmv.c \
playback/playback.c \
external/ffmpeg/src/bitstream.c \
external/ffmpeg/src/latmenc.c \
external/ffmpeg/src/mpeg4audio.c \
external/ffmpeg/src/xiph.c \
external/flv2mpeg4/src/m4vencode.c \
external/flv2mpeg4/src/flvdecoder.c \
external/flv2mpeg4/src/dcprediction.c \
external/flv2mpeg4/src/flv2mpeg4.c \
external/plugins/src/png.c

#libeplayer3_la_LIBADD = -lpthread -lavformat -lavcodec -lavutil -lswresample -lz -lass -lm -lpng
libeplayer3_la_LIBADD = -lswscale -ldl -lpthread -lavformat -lavcodec -lavutil -lswresample
#libeplayer3_la_LIBADD = -lswscale -ldl -lpthread -lavformat -lavcodec -lavutil -lswresample -lz -lass -lm -lpng

bin_PROGRAMS = eplayer3
eplayer3_SOURCES = main/exteplayer.c
eplayer3_LDADD = -leplayer3 -lpthread
#eplayer3_LDADD = -leplayer3 -lpthread -lass -lm -lpng
#eplayer3_LDADD = -leplayer3 -lswscale -ldl -lpthread -lavformat -lavcodec -lavutil -lswresample
eplayer3_DEPENDENCIES = libeplayer3.la


#bin_PROGRAMS = exteplayer
#exteplayer_SOURCES = main/exteplayer.c
#exteplayer_LDADD = -leplayer3 -lpthread -lass -lm -lpng
#exteplayer_DEPENDENCIES = libeplayer3.la

#meta_SOURCES = tools/meta.c
#meta_LDADD = -leplayer3 -lpthread -lavformat -lavcodec -lavutil -lass -lm -lpng
#meta_DEPENDENCIES = libeplayer3.la


