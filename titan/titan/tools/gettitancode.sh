#!/bin/sh
KERNELDIR=$1
ROOTDIR=$2
TYPE=$3
SRCDIR=$4

cd "$HOME"/flashimg/$SRCDIR/titan/tools

rm tmp
rm tmp.gz
rm tmp.lzma

#if [ $TYPE == "ufs910" ];then
#	dd if=$KERNELDIR of=tmp.lzma bs=1 skip=64
#	lzma -d tmp.lzma
#else
	dd if=$KERNELDIR of=tmp.gz bs=1 skip=64
	gzip -d tmp.gz
#fi

str=`strings tmp | grep "Linux version 2.6" | sed 's/Linux version //' | sed 's/(.*)//' | sed 's/  / /'`

code=`./gettitancode "$str"`
code="$code"UL
echo "[gettitancode.sh] $str -> $code"
cat ../titan.c | sed s/"^#define SYSCODE .*"/"#define SYSCODE $code"/ > titan.c.tmp
mv -f titan.c.tmp ../titan.c

code=`cat $ROOTDIR/etc/image-version | cut -d= -f2`
cat ../titan.c | sed s/"^#define TIMECODE .*"/"#define TIMECODE \"$code\""/ > titan.c.tmp
mv -f titan.c.tmp ../titan.c

cat ../security.h | sed s/"^#define TIMECODE .*"/"#define TIMECODE $code"/ > security.h.tmp
mv -f security.h.tmp ../security.h