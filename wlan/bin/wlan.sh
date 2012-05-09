#!/bin/sh

WLAN_LOG=/tmp/wlan.log
rm $WLAN_LOG

[ ! -f /etc/Wireless/wlan.usbmap ] && rm -f /etc/Wireless/wlan_module && exit 1

WLAN_MODULE=
MEM=$IFS
IFS=$'\012'
echo "[wlan] searching wireless-module ..." | tee -a $WLAN_LOG
USBIDS=`lsusb | awk {'print $6'}`
if [ -f /etc/Wireless/wlan_module ]; then
	WLAN_MODULE=`cat /etc/Wireless/wlan_module | awk {'print $1'}`
	VID=`cat /etc/Wireless/wlan_module | awk {'print $2'}`
	PID=`cat /etc/Wireless/wlan_module | awk {'print $3'}`
fi

if [ -z $WLAN_MODULE ] || [ -z "`echo $USBIDS | grep $VID | grep $PID`" ]; then
	for i in $USBIDS; do
		VID=`echo $i | cut -d':' -f1`
		PID=`echo $i | cut -d':' -f2`
		WLAN_MODULE=
		for j in `cat /etc/Wireless/wlan.usbmap`; do
			if [ `echo $j | awk {'print $3'}` == 0x$VID ] && [ `echo $j | awk {'print $4'}` == 0x$PID ]; then
				WLAN_MODULE=`echo $j | awk {'print $1'}`
				break
			fi
		done
		[ ! -z $WLAN_MODULE ] && break
	done
	[ ! -z $WLAN_MODULE ] && echo $WLAN_MODULE $VID $PID > /etc/Wireless/wlan_module || rm -f /etc/Wireless/wlan_module
fi

[ -z $WLAN_MODULE ] && echo "[wlan] no applicable USB WLAN device found" | tee -a $WLAN_LOG && exit 1
IFS=$MEM
echo "[wlan] loading $WLAN_MODULE.ko" | tee -a $WLAN_LOG
insmod /lib/modules/$WLAN_MODULE.ko
[ -z "`lsmod | grep $WLAN_MODULE`" ] && echo "[wlan] could not load $WLAN_MODULE" | tee -a $WLAN_LOG && exit 1
case $WLAN_MODULE in
	rt2870sta | rt3070sta )	WLAN_IF=ra0
				WLAN_DRV=ralink
				;;
	8192cu )		WLAN_IF=wlan0
				WLAN_DRV=wext
				;;
	* )			echo "[wlan] Unknown module $WLAN_MODULE" | tee -a $WLAN_LOG && exit 1
				;;
esac

ifconfig $WLAN_IF 0.0.0.0 up
[ -z "`ifconfig | grep $WLAN_IF`" ] && rmmod $WLAN_MODULE && echo "[wlan] $WLAN_IF not ready" | tee -a $WLAN_LOG && exit 1

if [ "$1" == "notstart" ]; then
	exit 0
fi

if [ ! -f /etc/wpa_supplicant.conf ]; then
	echo "[wlan] wireless lan not yet configured" | tee -a $WLAN_LOG
	ifconfig $WLAN_IF down
	exit 1
fi

rm -Rf /var/run/wpa_supplicant*
killall wpa_supplicant
sleep 1
AP_TRIES=40
AP=`cat /etc/wpa_supplicant.conf | grep ssid | grep -v scan_ssid | cut -d"=" -f2`
echo "[wlan] starting wpa_supplicant ..." | tee -a $WLAN_LOG
wpa_supplicant -B -i $WLAN_IF -P /var/run/wpa_supplicant.$WLAN_IF -D $WLAN_DRV -c /etc/wpa_supplicant.conf
while [ $AP_TRIES -gt 0 ]; do
	AP_STATE=`wpa_cli -i $WLAN_IF -p /var/run/wpa_supplicant status | grep wpa_state | cut -d"=" -f2`
	[ "$AP_STATE" == "COMPLETED" ] && echo "[wlan] association with $AP completed" | tee -a $WLAN_LOG && break
	AP_TRIES=$((AP_TRIES-1))
	sleep 1
done

if [ $AP_TRIES -lt 1 ]; then
	case "$AP_STATE" in
		"SCANNING"  )	echo -e "[wlan] access point $AP not found (scanning aborted...)" | tee -a $WLAN_LOG
		;;
		*           )	echo -e "[wlan] authentification to access point $AP failed" | tee -a $WLAN_LOG
		;;
	esac
	kill -9 `cat /var/run/wpa_supplicant.$WLAN_IF`
	rm /var/run/wpa_supplicant.$WLAN_IF
	ifconfig $WLAN_IF down
	exit 1
fi

exit 0
