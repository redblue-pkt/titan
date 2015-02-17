#!/bin/sh
KERNELDIR=$1
ROOTDIR=$2
TYPE=$3
SRCDIR=$4
CPU=$5

cd "$HOME"/flashimg/$SRCDIR/titan/tools

echo 000000 "$HOME"/flashimg/$SRCDIR/titan/tools $KERNELDIR

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

if [ "$CPU" = "sh4" ];then
	str=`strings tmp | grep "Linux version 2.6" | sed 's/Linux version //' | sed 's/(.*)//' | sed 's/  / /'`
else
	str=`strings tmp | grep "Linux version 3." | sed 's/Linux version //' | sed 's/(.*)//' | sed 's/  / /'`
fi

code=`./gettitancode "$str"`
code="$code"UL
echo 1111111 code1: $code
echo "[gettitancode.sh] $str -> $code"
cat ../titan.c | sed s/"^#define SYSCODE .*"/"#define SYSCODE $code"/ > titan.c.tmp
mv -f titan.c.tmp ../titan.c
echo 2222222 code2: $code ROOTDIR: $ROOTDIR
code=`cat $ROOTDIR/etc/image-version | cut -d= -f2`
cat ../titan.c | sed "s/^#define TIMECODE .*/#define TIMECODE \"$code\"/" > titan.c.tmp
mv -f titan.c.tmp ../titan.c
echo 3333333 code2: $code ROOTDIR: $ROOTDIR
cat ../security.h | sed "s/^#define BUILDCODE .*/#define BUILDCODE $code/" > security.h.tmp
mv -f security.h.tmp ../security.h