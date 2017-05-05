#!/bin/sh

startconfig=/mnt/config/start-config
if [ ! -e "$startconfig" ]; then startconfig="/etc/titan.restore/mnt/config/start-config"; fi

. $startconfig
. /sbin/start-function

aktion="$1"
emuname="$2"

emupaths="/var/etc /var/swap/etc /mnt/swapextensions/etc"

case $emuname in "") emuname="$emu";; esac

getemupath()
{
	for path in $emupaths; do
		if [ -e "$path"/"$1" ]; then 
			emupath="$path"
			return 0
		fi
	done
	echo "not found"
	exit 1
}

checkemu()
{
	echo "[emu.sh] checkemu emu=$1"
	binname=`grep -m1 "^binname" "$1" | tr -d "\r"`
	binname=${binname/binname*=/}
	pidof "$binname" > /dev/null 2>&1
	case $? in
		0)
			echo "[emu.sh] checkemu running emu=$1"
			return 1;;
		*)
			echo "[emu.sh] checkemu not running emu=$1"
			return 0;;
	esac
}

startemu()
{
	echo "[emu.sh] startemu emu=$1"
	checkemu "$1"
	case $? in
		0)
			echo "[emu.sh] startemu starting emu=$1"
			startcam=`grep -m1 "^startcam" "$1" | tr -d "\r"`
			startcam=${startcam/startcam*=/}
			startcam="($startcam) &"
			#startcam="$startcam"
			eval $startcam
			sleep 3;;
		*)
			echo "[emu.sh] startemu not starting, is running emu=$1";;
	esac
}

stopemu()
{
	echo "[emu.sh] stopemu emu=$1"
	#if [ -e "/tmp/ecm.info" ]; then rm -f "/tmp/ecm.info"; fi
	#if [ -e "/tmp/ecm0.info" ]; then rm -f "/tmp/ecm0.info"; fi
	#if [ -e "/tmp/ecm1.info" ]; then rm -f "/tmp/ecm1.info"; fi
	stopcam=`grep -m1 "^stopcam" "$1" | tr -d "\r"`
	stopcam=${stopcam/stopcam*=/}
	eval $stopcam
	sleep 1
	checkemu "$1"
	case $? in
		1)
			echo "[emu.sh] stopemu can not stop, do a killall -9 $binname"
			killall -9 "$binname";;
	esac
}

case $aktion in
	start)
		getemupath "$emuname"
		startemu "$emupath"/"$emuname"
		exit 0;;
	stop)
		getemupath "$emuname"
		stopemu "$emupath"/"$emuname"
		exit 0;;
	restart)
		getemupath "$emuname"
		stopemu "$emupath"/"$emuname"
		sleep 1
		startemu "$emupath"/"$emuname"
		exit 0;;
	activate)
		sed s/"^emu=.*$"/"emu=$emuname"/ -i "$startconfig"
		getemupath "$emuname"
		startemu "$emupath"/"$emuname"
		exit 0;;
	deactivate)
		sed s/"^emu=.*$"/"emu=off"/ -i "$startconfig"
		getemupath "$emuname"
		stopemu "$emupath"/"$emuname"
		exit 0;;
	check)
		getemupath "$emuname"
		checkemu "$emupath"/"$emuname"
		exit $?;;
	list)
		for path in $emupaths; do
			ls -1 "$path"/*.emu 2>/dev/null | sed "s!$path/!!"
		done
		exit 0;;
	shareinfo)
		if [ -e /tmp/shareinfo ]; then
			cat /tmp/shareinfo
			exit $?
		fi
		exit 0;;
	ecminfo)
		if [ -e /tmp/ecm.info ]; then
			cat /tmp/ecm.info
			exit $?
		fi
		exit 0;;
	active)
		echo "$emu"
		exit 0;;
	infoname)
		getemupath "$emuname"
		infoname=`grep -m1 "^emuname" "$emupath"/"$emuname" | tr -d "\r"`
		infoname=${infoname/emuname*=/}; echo "$infoname"
		exit $?;;
	keydir)
		getemupath "$emuname"
		keydir=`grep -m1 "^startcam" "$emupath"/"$emuname" | grep -m1 "^startcam" | tr ' ' '\n' | grep /keys | tr -d "\r" | sed 's!/keys.*!/keys!'`
		echo "$keydir"
		exit $?;;
	checkinstall)
		ls -al /var/etc/*.emu > /tmp/.emu	
		ls -al /var/swap/etc/*.emu >> /tmp/.emu
		ls -al /mnt/swapextensions/etc/*.emu >> /tmp/.emu		
		if [ `cat /tmp/.emu | tail -n 1 | wc -l` -eq 1 ]; then
			exit 1
		fi
		exit 0;;
	halt)
		getemupath "$emuname"
		checkemu "$emupath"/"$emuname"
		if [ $? -eq 1 ]; then
			if [ "${emuname:0:5}" == "Camd3" ] || [ "${emuname:0:5}" == "camd3" ]; then
				stopemu "$emupath"/"$emuname"
				echo $?
			else
				binname=`grep -m1 "^binname" "$emupath"/"$emuname" | tr -d "\r"`
				binname=${binname/binname*=/}
				kill -stop `pidof $binname`
				exit $?
			fi
		fi
		exit 0;;
	unhalt)
		getemupath "$emuname"
		checkemu "$emupath/$emuname"
		if [ $? -eq 0 ]; then
			startemu "$emupath"/"$emuname"
			echo $?
		else
			binname=`grep -m1 "^binname" "$emupath/$emuname" | tr -d "\r"`
			binname=${binname/binname*=/}
			kill -cont `pidof $binname`
			exit $?
		fi
		exit 0;;
	*)
		echo "[emu.sh] usage: emu.sh [infoname|keydir|active|stop|start|restart|activate|deactivate|check|list|shareinfo|halt|unhalt] [emuname]";;
esac
