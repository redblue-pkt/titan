#!/bin/sh

[ ! -f /etc/Wireless/wlan.usbmap ] && exit 2

MEM=$IFS
IFS=$'\012'

USBIDS=`lsusb | awk {'print $6'}`
for i in $USBIDS; do
	VID=`echo $i | cut -d':' -f1`
	PID=`echo $i | cut -d':' -f2`
	WLAN_MODULE=
	j=`cat /etc/Wireless/wlan.usbmap | grep -w 0x$VID | grep -w 0x$PID`
	if [ ! -z "$j" ]; then
		WLAN_MODULE=`echo $j | awk {'print $1'}`
	fi	
	if [ ! -z $WLAN_MODULE ]; then 
		echo "$WLAN_MODULE"
		if [ -e /lib/modules/$WLAN_MODULE.ko ]; then
			exit 0
		fi
		if [ -e /var/lib/modules/$WLAN_MODULE.ko ]; then
			exit 0
		fi
		if [ -e /var/swap/lib/modules/$WLAN_MODULE.ko ]; then
			exit 0
		fi
		if [ -e /mnt/swapextensions/lib/modules/$WLAN_MODULE.ko ]; then
			exit 0
		fi
		exit 1
	fi
done

IFS=$MEM
exit 2
