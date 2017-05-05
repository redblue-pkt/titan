#!/bin/sh

if [ -z "$1" ]; then
	echo "Usage: ./usbspeed device"
	echo "device = sda or sdb or sdc ..."
	exit
fi

dev=$1
swap=0

echo "Start USB speed test for /dev/$dev"
echo "Please go to MediaCenter for testing"
echo

swap swapalloff
old_max_sectors=`cat /sys/block/$dev/device/max_sectors`
sync

step="80 160 240 320 400 480 560 640 720 800 880 960 1000"

hdparm -t "/dev/$dev" > /dev/null
for i in $step; do
	s=""
	if [ $i == 240 ]; then s=" (linux standard)"; fi
	echo "Test: /dev/$dev with max_sectors $i$s:"
	echo $i > /sys/block/$dev/device/max_sectors
	hdparm -t "/dev/$dev" | grep "^Timing"
done

swap
echo $old_max_sectors > /sys/block/$dev/device/max_sectors