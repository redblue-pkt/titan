#!/bin/sh

rm tmp
rm tmp.gz
dd if=uImage of=tmp.gz bs=1 skip=64
gzip -d tmp.gz

str=`strings tmp | grep "Linux version 2.6" | sed 's/Linux version //' | sed 's/(.*)//' | sed 's/  / /'`

code=`gettitancode "$str"`
echo "[gettitancode.sh] $str -> $code"

cat ca.h | sed s/"^#define SYSCODE .*"/"#define SYSCODE $code"/ > ca.h.tmp
mv ca.h.tmp ca.h
