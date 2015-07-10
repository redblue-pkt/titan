#!/bin/sh
KERNELDIR=$1
ROOTDIR=$2
TYPE=$3
SRCDIR=$4
CPU=$5
STM=$6
BOXNAME=$7

cd "$HOME"/flashimg/$SRCDIR/titan/tools

echo 000000 "$HOME"/flashimg/$SRCDIR/titan/tools $KERNELDIR

rm tmp
rm tmp.gz
rm tmp.lzma

if [ "$CPU" = "sh4" ];then
	#if [ $TYPE == "ufs910" ];then
	#	dd if=$KERNELDIR of=tmp.lzma bs=1 skip=64
	#	lzma -d tmp.lzma
	#else
		dd if=$KERNELDIR of=tmp.gz bs=1 skip=64
		gzip -d tmp.gz
	#fi
	str=`strings tmp | grep "Linux version 2.6" | sed 's/Linux version //' | sed 's/(.*)//' | sed 's/  / /'`
else
	str=`strings $KERNELDIR | grep "Linux version 3." | sed 's/Linux version //' | sed 's/(.*)//' | sed 's/  / /'`
fi

code=`./gettitancode "$str"`
code="$code"UL
echo 1111111 code1: $code
echo "[gettitancode.sh] $str -> $code"
cat ../titan.c | sed s/"^#define SYSCODE .*"/"#define SYSCODE $code"/ > titan.c.tmp
mv -f titan.c.tmp ../titan.c
echo 2222222 code2: $code ROOTDIR: $ROOTDIR
code=`cat $ROOTDIR/etc/image-version | cut -d= -f2`
if [ -z "$code" ];then
	code=`date +%s`
fi
cat ../titan.c | sed "s/^#define TIMECODE .*/#define TIMECODE \"$code\"/" > titan.c.tmp
mv -f titan.c.tmp ../titan.c
echo 3333333 code2: $code ROOTDIR: $ROOTDIR
cat ../security.h | sed "s/^#define BUILDCODE .*/#define BUILDCODE $code/" > security.h.tmp
mv -f security.h.tmp ../security.h

if [ "$CPU" = "mipsel" ];then
	drivername=`cat "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/meta-oe-alliance/meta-brands/meta-ini/conf/machine/include/ini-oem.inc | grep $BOXNAME | grep ini | cut -d "'" -f6`
	kv=`cat "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/meta-oe-alliance/meta-brands/meta-ini/recipes-drivers/ini-dvb-modules-$drivername.bb | grep ^KV | cut -d '"' -f2`
	pr=`cat "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/meta-oe-alliance/meta-brands/meta-ini/recipes-drivers/ini-dvb-modules-$drivername.bb | grep ^PR | cut -d '"' -f2`
	driverdate=`cat "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/meta-oe-alliance/meta-brands/meta-ini/recipes-drivers/ini-dvb-modules-$drivername.bb | grep ^SRCDATE | cut -d '"' -f2`
else
	drivername=""
	kv=""
	pr=""
	driverdate=`date`
fi

ls -al "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/meta-oe-alliance/meta-brands/meta-ini/conf/machine/include/ini-oem.inc
ls -al "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/meta-oe-alliance/meta-brands/meta-ini/recipes-drivers/

echo 44444 	STM: $STM BOXNAME: $BOXNAME drivername: $drivername kv: $kv pr: $pr driverdate: $driverdate ROOTDIR: $ROOTDIR
cat ../security.h | sed "s/^#define DRIVER .*/#define DRIVER \"$drivername $kv $pr $driverdate\"/" > security.h.tmp
mv -f security.h.tmp ../security.h

/home/atemio/flashimg/BUILDGIT/checkout_mips360/meta-oe-alliance/meta-brands/meta-ini/recipes-drivers

