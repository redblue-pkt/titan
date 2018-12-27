#!/bin/sh 

INPUT="$1"
REBOOT="$2"
startconfig=/mnt/config/start-config
if [ ! -e "$startconfig" ]; then startconfig="/etc/titan.restore/mnt/config/start-config"; fi

. $startconfig
. /sbin/start-progress
. /sbin/start-function

model=`cat /etc/model`
realbox=`cat /proc/stb/info/boxtype`
arch=`cat /etc/.arch`
board=`cat /etc/.board`

if [ $board = "dm7020hd" ];then
#	mtd=0
	mtd=4
else
	mtd=1
fi
ubi=1

echo model: $model

event=event1
if [ "$model" = "vusolo2" ]; then
	event=event0
fi
if [ "$board" = "dm7020hd" ];then
	event=event2
fi

startReloadconfig()
{
	startup_progress "$INPUT"
	startconfig=/mnt/config/start-config
	if [ ! -e "$startconfig" ]; then startconfig="/etc/titan.restore/mnt/config/start-config"; fi
	. $startconfig
}

startFlash()
{
	if [ -e /etc/.oebuild ];then
		case $debug in on|full) echo "[$0] [$INPUT] skip startFlash";; esac
	else
		case $debug in on|full) echo "[$0] [$INPUT] startFlash: ubiattach MNT-FLASH (MTD$mtd)";; esac
		ubiattach -p /dev/mtd$mtd
		case $debug in on|full) echo "[$0] [$INPUT] startFlash: mount MNT-FLASH (MTD$mtd)";; esac
		mount -t ubifs ubi$ubi:mnt /mnt
	fi
}

startBootlogo()
{
	startup_progress "$INPUT"
	echo "$CMD [$INPUT] startBootlogo"

	#fix green bootpicture
	case $bootrgbfix in y) echo rgb > /proc/stb/avs/0/colorformat;; esac

	#Some televisions can not 1080p
	if [ -f /etc/videomode ]; then
		cat /etc/videomode > /proc/stb/video/videomode
	else
		echo "720p50" > /proc/stb/video/videomode
	fi

	if [ "$bootlogo" == 'y' ]; then
	 	if [ -e /mnt/swapextensions/etc/boot/bootlogo.jpg ]; then
			infobox 200 "nobox#/mnt/swapextensions/etc/boot/bootlogo.jpg" &
		else
			if [ -e /var/etc/boot/bootlogo.jpg ]; then
				infobox 200 "nobox#/var/etc/boot/bootlogo.jpg" &
			fi
		fi
	fi
}

startMicomUpdate() {
	startup_progress "$INPUT"
	echo "$CMD [$INPUT] startMicomUpdate"

	INSTALLED=`cat /proc/stb/fp/version`
	REALBOX=`cat /proc/stb/info/boxtype`

	if [ "$model" = "atemio-nemesis" ];then
		UPDATE=20049
		MD5=4c8cfeaff0b23f4fc755dd9821071a00
		FILE="/var/micom_update/INI800AM_Micom.bin"
		REALBOXCHECK=ini-8000am
	fi

	MD5CHECK=`md5sum "$FILE" | cut -d"/" -f1 | tr -d " "`
	STARTUPDATE=1

	echo "$CMD [$INPUT] startMicomUpdate INSTALLED: $INSTALLED"
	echo "$CMD [$INPUT] startMicomUpdate UPDATE: $UPDATE"

	if [ -z "$INSTALLED" ];then
		echo "$CMD [$INPUT] startMicomUpdate exit zero INSTALLED: $INSTALLED"
		rm -rf /var/micom_update
		STARTUPDATE=0
	fi

	if [ -z "$UPDATE" ];then
		echo "$CMD [$INPUT] startMicomUpdate exit zero UPDATE: $UPDATE"
		rm -rf /var/micom_update
		STARTUPDATE=0
	fi

	if [ "$REALBOX" != "$REALBOXCHECK" ]; then
		echo "$CMD [$INPUT] startMicomUpdate exit found not correctly REALBOX: $REALBOX"
		rm -rf /var/micom_update
		STARTUPDATE=0
	fi

	if [ "$MD5" != "$MD5CHECK" ]; then
		echo "$CMD [$INPUT] startMicomUpdate exit MD5 Error: $MD5 <> $MD5CHECK"
		rm -rf /var/micom_update
		STARTUPDATE=0
	fi

	sync

	if [ ! -e "$FILE" ];then
		echo "$CMD [$INPUT] startMicomUpdate exit File: $FILE not found"
		STARTUPDATE=0
	fi

	if [ "$STARTUPDATE" = "1" ] && [ "$INSTALLED" -lt "$UPDATE" ];then
		echo "$CMD [$INPUT] startMicomUpdate update MICOM to $UPDATE"
		killall -9 infobox
		infobox 9999 INFO "Micom Update" "Update FrontProcessor Firmware Version: $INSTALLED > $UPDATE" "Please do not power off!!" &
		sleep 3
		echo fpupload > /proc/vfd && sleep 7 && cat $FILE > /dev/mcu
		sleep 10
		sync
	else
		echo "$CMD [$INPUT] startMicomUpdate update not needed INSTALLED VERSION ok"
	fi
}

startMountSwap()
{
	startup_progress "$INPUT"

	if [ -e /var/etc/.erasemtd ] || [ ! -e /mnt/swapextensions ]; then
		#/usr/bin/showiframe /usr/share/bootlogo.mvi
		startBootlogo

		if [ "$board" = "inihde" ] || [ $board = "inihde2" ] || [ $board = "inihde2am" ];then
			size=250MiB
		elif [ "$board" = "inihdp" ];then
			size=1400MiB
		elif [ "$model" = "dm7020hd" ]; then
			size=574MiB
		elif [ "$model" = "dm7020hdv2" ]; then
#			size=569MiB
			size=700MiB
		elif [ "$board" = "dm520" ];then
			size=4MiB
		else
			size=20MiB
		fi

		if [ -e /var/etc/.backupmtd ]; then
			case $debug in on|full) echo "[$0] [$INPUT] startMountSwap: backupmtd (MTD$mtd)";; esac
			mkdir /tmp/backupmtd
			cp -a /mnt/settings /tmp/backupmtd
			cp -a /mnt/config /tmp/backupmtd
			cp -a /mnt/network /tmp/backupmtd
			cp -a /mnt/script /tmp/backupmtd
			if [ -e "/usr/bin/enigma2" ];then
				cp -a /mnt/enigma2 /tmp/backupmtd
			fi
			mkdir /tmp/backupmtd/swapextensions
			cp -a /mnt/swapextensions/player /tmp/backupmtd/swapextensions
			case $debug in on|full) echo "[$0] [$INPUT] startMountSwap: backuptpk (MTD$mtd)";; esac
			backuptpk
		fi

		if [ -e /etc/.oebuild ];then
			rm -rf /mnt/*
		else
			case $debug in on|full) echo "[$0] [$INPUT] startMountSwap: umount SWAP-FLASH (MTD$mtd)";; esac
			umount -fl /mnt
			case $debug in on|full) echo "[$0] [$INPUT] startMountSwap: erase SWAP-FLASH (MTD$mtd)";; esac
			cp -aL /dev/vfd /tmp/vfd
			infobox -pos -1 75% 10015 "MNT" "            Formatiere Laufwerk            " &
			if [ "$board" = "dm7020hd_alt" ];then
				ubirmvol /dev/ubi"$mtd" -n 1
			else
				ubidetach /dev/ubi_ctrl -m "$mtd"
				ubiformat /dev/mtd"$mtd"
				ubiattach -p /dev/mtd"$mtd"
			fi
			ubimkvol /dev/ubi"$ubi" -N mnt -s "$size"
			case $debug in on|full) echo "[$0] [$INPUT] startMountSwap: mount SWAP-FLASH (MTD$mtd)";; esac
			killall -9 infobox
			infobox 9999 INFO "Initializing MNT" "" "Do not power off!" &
			mount -t ubifs ubi"$ubi":mnt /mnt
		fi

		case $debug in on|full) echo "[$0] [$INPUT] startMountSwap: create swapextensions folder SWAP-FLASH (MTD$mtd)";; esac		
		mkdir /mnt/swapextensions
		mkdir /mnt/bin

		case $debug in on|full) echo "[$0] [$INPUT] startMountSwap: hotplug.sh startStickextensions SWAP-FLASH (MTD$mtd)";; esac

#		if [ "$arch" = "arm" ];then
			/etc/mdev/mdev-mount.sh startStickextensionsMNT
#		else
#			hotplug.sh startStickextensionsMNT
#		fi

		if [ -e /var/etc/.backupmtd ]; then
			case $debug in on|full) echo "[$0] [$INPUT] startMountSwap: restoretpk (MTD$mtd)";; esac
			restoretpk
			case $debug in on|full) echo "[$0] [$INPUT] startMountSwap: backupmtd (MTD$mtd)";; esac

			rm -rf /mnt/settings
			rm -rf /mnt/config
			rm -rf /mnt/network
			rm -rf /mnt/script
			rm -rf /mnt/swapextensions/player
			if [ -e "/usr/bin/enigma2" ];then
				rm -rf /mnt/enigma2
			fi
			cp -a /tmp/backupmtd/* /mnt
		else
			mkdir /mnt/tpk
			mkdir /mnt/script
			reset.sh
		fi

		rm -r /var/etc/.erasemtd
		rm -r /var/etc/.backupmtd

		#ubifs needs sync
		sync

		#startMicomUpdate

		killall infobox
		infobox 9999 INFO "Initializing MNT" "" "you can power off the receiver now,"  "in case it does not reboot" &
		sleep 2
		reboot
	fi
}

startAutofs()
{
	startup_progress "$INPUT"
	case $debug in on|full) echo "[$0] [$INPUT] startAutofs: start";; esac
	/etc/init.d/autofs start
}

startInit()
{
	if [ -e /etc/init.d/$1 ];then
		startup_progress "$INPUT"
		case $debug in on|full) echo "[$0] [$INPUT] startInit: (flash) $1 start";; esac
		/etc/init.d/$1 start
	elif [ -e /mnt/swapextensions/etc/init.d/$1 ];then
		startup_progress "$INPUT"
		case $debug in on|full) echo "[$0] [$INPUT] startInit: (mnt) $1 start";; esac
		/mnt/swapextensions/etc/init.d/$1 start
	else
		case $debug in on|full) echo "[$0] [$INPUT] startInit: $1 skip";; esac
		startup_progress "$INPUT"
	fi
}

startWorkaround()
{
	startup_progress "$INPUT"
	case $debug in on|full) echo "[$0] [$INPUT] startWorkaround: start";; esac
	cp /lib/titan/default/skinconfig /lib/titan/default/skinconfig.default

	sed '/^smbfs.*/d' -i /mnt/network/auto.misc
	sed '/^upnpfs.*/d' -i /mnt/network/auto.misc

	pcount=0
	while [ "$pcount" -lt "9" ]
	do
		LIST="a b c d e f g h"
		pcount=`expr $pcount + 1`

		for ROUND in $LIST; do
			if ! grep -q "^sd$ROUND$pcount" /mnt/network/auto.misc; then
				echo "add sd$ROUND$pcount > auto.misc"
				echo "sd$ROUND$pcount	-fstype=auto,noatime,nodiratime	:/dev/sd$ROUND$pcount" >> /mnt/network/auto.misc
			fi
		done
	done
}

startNtpdate()
{
	startup_progress "$INPUT"
	case $ntpdate in
		y)
			case $debug in on|full) echo "[$0] [$INPUT] startDate: start";; esac
			
			(ntpdate -b ptbtime1.ptb.de; time=`date +%s`; echo -e `expr $time + 7200`  > /proc/stb/fp/rtc) &
			
#			ntpdate -b ptbtime1.ptb.de &

#			time=`date +%s`
#			echo -e `expr $time + 7200` > /proc/stb/fp/rtc
			;;
	esac
}

startTimefix()
{
	echo skip startTimefix
#	case $model in
#		*)	case $debug in on|full) echo "$CMD [$INPUT] startTimefix last";; esac
#			#date=`date "+%H:%M:%S %m-%d-%Y"`
#			date=`date "+%H:%M:%S %d-%m-%Y"`
#			echo "$CMD [$INPUT] current Date : $date"
#			/bin/fp_control -s $date;;
#	esac
}

startSetTime()
{
          if [ -e "/mnt/script/settime.sh" ]; then
                  /mnt/script/settime&
          fi
}

startSamba() {
	case $sambaserver in
		y)
			case $debug in on|full) echo "$CMD [$INPUT] start Samba";; esac
			/etc/init.d/samba start &
#			smbd -D -s /mnt/config/smb.conf
#			sleep 1
#			nmbd -D -s /mnt/config/smb.conf;;
	esac
}

startNFS() {
	case $nfsserver in
		y)
			case $debug in on|full) echo "$CMD [$INPUT] start startNFS";; esac
			/etc/init.d/nfsserver start &
	esac
}

startOpenVPN() {
	case $openvpn in
		y)
			case $debug in on|full) echo "$CMD [$INPUT] start startOpenVPN";; esac
			/etc/init.d/openvpn start &
	esac
}

startCi()
{
	if [ "$model" == "hd51" ] || [ "$model" == "mutant51" ]; then
		if [ -e /mnt/bin/ciplushelper ];then
			/mnt/bin/ciplushelper &
			sleep 1
		fi
	fi
}

startPluginScript()
{
	if [ -d /mnt/plugin ]; then
		for i in /mnt/plugin/ps[0-9][0-9]*
		do
			$i 
		done
	fi
}

stopPluginScript()
{
	if [ -d /mnt/plugin ]; then
		for i in /mnt/plugin/pe[0-9][0-9]*
		do
			$i 
		done
	fi
}

startGui()
{
	if [ ! -e /tmp/.update ];then
		if [ ! -e /tmp/.init3 ];then
			startup_progress "$INPUT"
		fi
		case $debug in on|full) echo "[$0] [$INPUT] startGui: start";; esac

		LIBS="/lib/libopen.so.0.0.0"
		STARTDEFAULT="/usr/local/bin/titan /mnt/config/titan.cfg"

		if [ -e "/var/etc/.checkdualboot" ] && [ -e "/usr/bin/enigma2" ];then
			startBootlogo
			(sleep 10; killall infobox) &
			pid=$!
			if [ -e "/mnt/config/dualboot" ];then
				infobox -pos -1 75% GUI#2 "Aktiviere Auswahlmenu (TitanNit/Enigma2)" "  Dualboot - nein" "  Dualboot - ja" ; ret=$?
			else
				infobox -pos -1 75% GUI#1 "Aktiviere Auswahlmenu (TitanNit/Enigma2)" "  Dualboot - nein" "  Dualboot - ja" ; ret=$?
			fi
			kill -9 $pid
			echo " " > /dev/vfd

			if [ "$ret" = "2" ];then
				infobox -pos -1 75% 2 "Dualboot"  "  an" &
				echo enable dualboot
				touch /mnt/config/dualboot
			else
				if [ "$ret" = "1" ];then
					rm /mnt/config/dualboot
					infobox -pos -1 75% 2 "Dualboot" "  aus" &
				else
					if [ -e "/mnt/config/dualboot" ];then
						infobox -pos -1 75% 2 "Dualboot"  "  an" &
					else
						infobox -pos -1 75% 2 "Dualboot" "  aus" &
					fi
				fi
			fi
			echo "Starting" > /dev/vfd
			echo remove checkdualboot flag
			rm -f /var/etc/.checkdualboot
			sync
			sleep 10
		else
			echo skip remove /etc/rc2.d /etc/rc3.d /etc/rcS.d
#			rm -f /etc/rc2.d/*
#			rm -f /etc/rc3.d/*
#			rm -f /etc/rcS.d/*
		fi

		fuser -k 80/tcp

		if [ -e /mnt/config/dualboot ] && [ -e "/usr/bin/enigma2" ];then
			if [ ! -e /mnt/config/dualboot.titan ] && [ ! -e /mnt/config/dualboot.enigma2 ] || [ -e /mnt/config/dualboot.titan ];then

				(sleep 10; killall infobox) &
				pid=$!
				infobox -pos -1 75% GUI#1 "++ DualBoot-Auswahl ++" "---- TitanNit  ----" "---- Enigma2 ----" ; ret=$?
				kill -9 $pid
				echo " " > /dev/vfd

				case $ret in
 					2)
						START="/usr/bin/enigma2"
						rm -f /mnt/config/dualboot.titan
						touch /mnt/config/dualboot.enigma2
						$STARTDEFAULT;;
					*)
						START="/usr/local/bin/titan /mnt/config/titan.cfg"
						rm -f /mnt/config/dualboot.enigma2
						touch /mnt/config/dualboot.titan;;
				esac
			else

				(sleep 10; killall infobox) &
				pid=$!
				infobox -pos -1 75% GUI#2 "++ DualBoot-Auswahl ++" "---- TitanNit  ----" "---- Enigma2 ----" ; ret=$?
				kill -9 $pid
				echo " " > /dev/vfd

				case $ret in
					1)
						START="/usr/local/bin/titan /mnt/config/titan.cfg"
						rm -f /mnt/config/dualboot.enigma2
						touch /mnt/config/dualboot.titan;;
					*)
						START="/usr/bin/enigma2"
						rm -f /mnt/config/dualboot.titan
						touch /mnt/config/dualboot.enigma2
						$STARTDEFAULT;;
				esac
			fi
		fi

		if [ "$START" = "/usr/bin/enigma2" ];then
			startInit3
			#/usr/bin/showiframe /usr/share/bootlogo.mvi
			startBootlogo
		fi

		if [ -z "$START" ]; then START="$STARTDEFAULT"; fi

		case $debug in on|full) echo "[$0] [$INPUT] startGui: ret($ret) $START";; esac
		$START

		# 1 - halt
		# 2 - reboot
		# 3 - restart gui
		#
		# >128 signal

		ret=$?
		case $ret in
			1)
				echo " " > /dev/vfd &
				stopPluginScript
				/sbin/halt &
				;;
			2)
				/sbin/reboot
				;;
			3)
				fuser -k 80/tcp
				rm -fR /home/root/.gstreamer-0.10
				rm -f /tmp/.listen.camd.socket.ignore
				;;
			4)
				/sbin/rmmod lcd
				/usr/sbin/fpupgrade --upgrade 2>&1 | tee /home/root/fpupgrade.log
				sleep 1;
				/sbin/rmmod fp
				/sbin/modprobe fp
				/sbin/reboot
				;;
			42)
				python /usr/lib/enigma2/python/upgrade.pyo
				;;
			43)
				init 1
				;;
			*)
				rm -f /tmp/.listen.camd.socket.ignore
				;;
		esac
	else
		case $debug in on|full) echo "[$0] [$INPUT] startGui: skip and freez";; esac
		sleep 200
	fi
}

startUsercmd() {
	case $debug in on|full) echo "[$0] [$INPUT] startUsercmd";; esac
	if [ -e /mnt/config/usercmd.sh ]; then
		/mnt/config/usercmd.sh
	fi
}

startEvent() {
	startup_progress "$INPUT"
	case $debug in on|full) echo "[$0] [$INPUT] startEvent";; esac
	/sbin/event &
}

startWlan() {
	startup_progress "$INPUT"
	case $wlan in
		y)
			case $debug in on|full) echo "[$0] [$INPUT] startWlan";; esac
			wlan.sh
			if [ -e /sys/class/net/wlan0 ];then
				/etc/init.d/networking -i wlan0 restart
			else
				/etc/init.d/networking -i ra0 restart
			fi;;
	esac
}

startDepmod() {
	startup_progress "$INPUT"
	case $debug in on|full) echo "[$0] [$INPUT] startDepmod";; esac

	if [ $board = "inihde" ];then
		depmod 3.12.1
	elif [ $board = "inihde2" ] || [ $board = "inihde2am" ] || [ $board = "inihdp" ];then
		depmod 3.14.2
	elif [ $board = "vusolo2" ];then
		depmod 3.13.5
	fi
}

startRcreboot() {
	startup_progress "$INPUT"
	case $rcreboot in
		y)
			case $debug in on|full) echo "$CMD [$INPUT] start RC-Reboot";; esac
			RCDEV=`grep "rcdev=" /mnt/config/titan.cfg | cut -d "=" -f 2`
			awk -f /etc/init.d/getfb.awk $RCDEV &
	esac
}

startDevoled() {
	startup_progress "$INPUT"
	case $debug in on|full) echo "[$0] [$INPUT] startDevoled";; esac

	if [ -e /proc/vfd ];then
		rm /dev/vfd
		ln -s /proc/vfd /dev/
	else if [ -e /dev/dbox/oled0 ];then
		rm /dev/vfd
		ln -s /dev/dbox/oled0 /dev/vfd
	fi
}

startHotplug()
{
	startup_progress "$INPUT"
	echo "$CMD [$INPUT] start mdev-mount.sh first $SWTYPE $model"
	/etc/mdev/mdev-mount.sh first "$SWTYPE" "$model"
}

checkEmu() {
	emuret=`emu.sh check | grep "checkemu running emu="`
	if [ -z "$emuret" ]; then
		startEmu restart
	fi
}

startXUPNPD() {
	case $debug in on|full) echo "$CMD [$INPUT] startXUPNPD";; esac
	case $xupnpd in
		y)
			if [ -e /var/usr/local/share/titan/plugins/xupnpd/xupnpd.sh ]; then
				/var/usr/local/share/titan/plugins/xupnpd/xupnpd.sh start /var/etc/xupnpd &
			elif [ -e /var/swap/usr/local/share/titan/plugins/xupnpd/xupnpd.sh ]; then
				/var/swap/usr/local/share/titan/plugins/xupnpd/xupnpd.sh start /var/swap/etc/xupnpd &
			elif [ -e /mnt/swapextensions/usr/local/share/titan/plugins/xupnpd/xupnpd.sh ]; then
			  /mnt/swapextensions/usr/local/share/titan/plugins/xupnpd/xupnpd.sh start /mnt/swapextensions/etc/xupnpd &
			fi
		;;
	esac
}

startDLNA() {
	case $debug in on|full) echo "$CMD [$INPUT] startDLNA";; esac
	case $dlna in
		y)
			if [ -e /var/usr/local/share/titan/plugins/dlna/dlna.sh ]; then
				/var/usr/local/share/titan/plugins/dlna/dlna.sh start /var/etc/minidlna.conf &
			elif [ -e /var/swap/usr/local/share/titan/plugins/dlna/dlna.sh ]; then
				/var/swap/usr/local/share/titan/plugins/dlna/dlna.sh start /var/swap/etc/minidlna.conf &
			elif [ -e /mnt/swapextensions/usr/local/share/titan/plugins/dlna/dlna.sh ]; then
			  /mnt/swapextensions/usr/local/share/titan/plugins/dlna/dlna.sh start /mnt/swapextensions/etc/minidlna.conf &
			fi
		;;
	esac
}

startInaDyn() {
	case $debug in on|full) echo "$CMD [$INPUT] startInaDyn";; esac
	case $inadyn in
		y)
			inadyn --input_file /mnt/config/inadyn.conf &
		;;
	esac
}

startDropcaches()
{
	echo "$CMD [$INPUT] startDropcaches"
	echo 3 > /proc/sys/vm/drop_caches
}

startSystemdown() {
	start_sec.sh 2 /sbin/fuser -k /dev/dvb/adapter0/frontend0
	start_sec.sh 2 pkill tuxtxt
	start_sec.sh 2 pkill -9 tuxtxt
	start_sec.sh 10 sync
	case $debug in on|full) echo "$CMD [$INPUT] Swapoff";; esac
	start_sec.sh 10 swap.sh swapfileoff "$SWTYPE" "$model"
	case $debug in on|full) echo "$CMD [$INPUT] Stop Emu";; esac
	start_sec.sh 2 emu.sh stop
}

startReboot() {
	echo [start.sh] startReboot $REBOOT >/tmp/log
	case $REBOOT in
		1)
			echo "$CMD [$INPUT] $SWTYPE SHUTDOWN"
			case $model in
				atemio530|atemio520|ipbox91|atemio6000|atemio6100) startDigit4Display &;;
			*)
				echo "SHUTDOWN" > /dev/vfd;;
			esac
			startTimefix
			#case $model in ufs922) fp_control -ww;; esac #write only wakeUp File
			startSystemdown
			#startRemoveLeds #gibt es hier nicht
#			init 0
			;;
		2)
			echo "$CMD [$INPUT] $SWTYPE REBOOT"
			case $model in
				atemio530|atemio520|ipbox91|atemio6000|atemio6100) startDigit4Display &;;
			*)
				echo "REBOOT" > /dev/vfd;;
			esac
			#case $model in ufs922) fp_control -ww;; esac #write only wakeUp File
			startSystemdown
			#startRemoveLeds #gibt es hier nicht
#			init 6
			;;
		3)
			echo "$CMD [$INPUT] $SWTYPE RESTART"
			start_sec.sh 2 /sbin/fuser -k /dev/dvb/adapter0/frontend0
			case $model in
				atemio530|atemio520|ipbox91|atemio6000|atemio6100) startDigit4Display &;;
			*)
				echo "RESTART" > /dev/vfd;;
			esac
			start_sec.sh 2 pkill tuxtxt
			start_sec.sh 2 pkill -9 tuxtxt;;
		*)
			sleep $errorstop
			#startRemoveLeds #gibt es hier nicht
			echo "$CMD [$INPUT] ERROR $SWTYPE REBOOT"
			case $model in
				atemio530|atemio520|ipbox91|atemio6000|atemio6100) startDigit4Display &;;
			*)
				echo "ERROR REBOOT" > /dev/vfd;;
			esac
			startSystemdown
#			init 6
			;;
	esac
}

startUsbImage()
{
	startup_progress "$INPUT"
	if [ `lsmod | grep dvb | wc -l` -gt 0 ]; then
		return
	fi

	titan_multi=xxx

	if [ $titan_multi == "xxx" ];then
		if [ `cat /proc/partitions | grep mmcblk1p1 | wc -l` -gt 0 ]; then
			mount /dev/mmcblk1p1 /ba
			if [ `ls /ba/titan_multi | wc -l` -gt 0 ]; then
				titan_multi=mmcblk1p1
			fi
			umount /ba
		fi
	fi
	if [ $titan_multi == "xxx" ];then
		if [ `cat /proc/partitions | grep sda1 | wc -l` -gt 0 ]; then
			mount /dev/sda1 /ba
			if [ `ls /ba/titan_multi | wc -l` -gt 0 ]; then
				titan_multi=sda1
			fi
			umount /ba
		fi
	fi
	if [ $titan_multi == "xxx" ];then
		if [ `cat /proc/partitions | grep sdb1 | wc -l` -gt 0 ]; then
			mount /dev/sdb1 /ba
			if [ `ls /ba/titan_multi | wc -l` -gt 0 ]; then
				titan_multi=sdb1
			fi
			umount /ba
		fi
	fi

	if [ $titan_multi != "xxx" ];then
		echo "multi found"
		#mkdir /ba
		multidev="/dev/"$titan_multi
		mount $multidev /ba
		BAHOME=/ba/titan_multi
		#TARGET=/test1

		mod=`cat /lib/modules/\`uname -r\`/modules.dep | grep extra  | cut -d: -f1`
		insmod /lib/modules/`uname -r`/$mod
		#insmod /lib/modules/3.14.2/extra/dvb_atemionemesis.ko
		if [ -f /etc/videomode ]; then
			cat /etc/videomode > /proc/stb/video/videomode
		else
			echo "720p50" > /proc/stb/video/videomode
		fi

		TARGET=`cat $BAHOME/.multi_image`
		if [ -z $TARGET ]; then
			TARGET=Flash
		fi
		images=`ls $BAHOME | while read m
		do
		 echo $m" "
		done`
		images="Flash "$images

		imagenum=1
		i=0
		for test in $images
		do
			i=`expr $i + 1`
  		if [ $test == $TARGET ]; then
				imagenum=$i
  			break
 			fi
		done
		if [ $imagenum -eq 1 ]; then
			TARGET=Flash
		fi

		(sleep 10; killall infobox) &
		pid=$!
		infobox GUI#$imagenum "MultiImage" $images; ret=$?
		kill -9 $pid
		if [ $ret != "0" ]; then
			TARGET=`echo $images | cut -d" " -f$ret`
		fi
		if [ -z $TARGET ]; then
			TARGET=Flash
		fi
		echo -n $TARGET > $BAHOME/.multi_image
		infobox 1 "Info" "Starting $TARGET"
		if [ $TARGET == "Flash" ]; then
			rmmod dvb
			return
		fi

		#Titan USB-Image ?
		if [ -e "$BAHOME/$TARGET/etc/titan.restore/mnt" ]; then
			if [ ! -f $BAHOME/$TARGET/var/etc/autostart/start.sh ]; then
				infobox 5 "ERROR" "Image $TARGET not ok.... reboot"
				reboot
			fi
			rmmod dvb
			rm $BAHOME/$TARGET/var/etc/.erasemtd
			#firststart ?
			if [ ! -e "$BAHOME/$TARGET/mnt/swapextensions" ]; then
				cp -RP "$BAHOME/$TARGET/etc/titan.restore/mnt" "$BAHOME/$TARGET"
				mkdir $BAHOME/$TARGET/mnt/swapextensions
				mkdir $BAHOME/$TARGET/mnt/bin
				rm $BAHOME/$TARGET/var/etc/.first
				rm $BAHOME/$TARGET/var/etc/.firstnet
				rm $BAHOME/$TARGET/var/etc/.firststart
				touch $BAHOME/$TARGET/var/etc/.usbimage
				mkdir $BAHOME/$TARGET/sys
				mkdir $BAHOME/$TARGET/proc
				mkdir $BAHOME/$TARGET/dev
				mkdir $BAHOME/$TARGET/tmp
			fi
			mount -o bind /proc "$BAHOME/$TARGET/proc"
			mount -o bind /dev "$BAHOME/$TARGET/dev"
			mount -o bind /sys "$BAHOME/$TARGET/sys"
			mount -o bind /tmp "$BAHOME/$TARGET/tmp"
			#mount -o bind /mnt "$BAHOME/$TARGET/mnt"
			exec chroot "$BAHOME/$TARGET" /var/etc/autostart/start.sh first
			echo "***** Multiimage ended"
			reboot
		else
			mkdir $BAHOME/$TARGET/sys
			mkdir $BAHOME/$TARGET/proc
			mkdir $BAHOME/$TARGET/dev
			mkdir $BAHOME/$TARGET/tmp
			cp /usr/bin/dummy.sh $BAHOME/$TARGET/usr/sbin/nandwrite
			cp /usr/bin/titan_e2.sh $BAHOME/$TARGET/usr/bin/titan_e2.sh
			if [ ! -f $BAHOME/$TARGET/usr/bin/titan_e2.sh ]; then
				infobox 5 "ERROR" "Image $TARGET not ok.... reboot"
				reboot
			fi
			rmmod dvb
			chmod 755 $BAHOME/$TARGET/usr/bin/titan_e2.sh
			mount -o bind /proc "$BAHOME/$TARGET/proc"
			mount -o bind /dev "$BAHOME/$TARGET/dev"
			mount -o bind /sys "$BAHOME/$TARGET/sys"
			mount -o bind /tmp "$BAHOME/$TARGET/tmp"
			#mount -o bind /mnt "$BAHOME/$TARGET/mnt"
			#exec chroot "$BAHOME/$TARGET" /etc/init.d/rcS
			exec chroot "$BAHOME/$TARGET" /usr/bin/titan_e2.sh
			echo "***** Multiimage ended"
			reboot
		fi
	fi
}

startSethyprid()
{
	if [ "$model" == "dm7020hd" ] || [ "$model" == "dm7020hdv2" ] || [ "$board" == "dm520" ] || [ "$board" == "dm525" ];then
		echo `cat /mnt/config/titan.cfg | grep fe_00_hyprid= | sed 's/fe_00_hyprid=//'` > /proc/stb/frontend/0/mode
		echo `cat /mnt/config/titan.cfg | grep fe_01_hyprid= | sed 's/fe_01_hyprid=//'` > /proc/stb/frontend/1/mode
	fi
}

startRootWork()
{
#	startup_progress "$INPUT"
	lastdir=$(pwd)
	LIST=$(ls -1 / | grep -v proc | grep -v media | grep -v var)
	cd /
	for ROUND in $LIST; do
		chown -R root:root $ROUND
	done

	LIST=$(ls -1 /var | grep -v cache | grep -v media | grep -v keys)
	cd /
	for ROUND in $LIST; do
		chown -R root:root /var/$ROUND
	done
	cd $lastdir
}

setLibraryPath()
{
	if [ `cat /etc/ld.so.conf | grep "/mnt/swapextensions/lib" | wc -l` -lt 1 ]; then
		echo "" >> /etc/ld.so.conf
		echo "/mnt/swapextensions/lib" >> /etc/ld.so.conf
	fi
	ldconfig
}

case $1 in
	first)
		if [ ! -e /tmp/.init3 ];then
			if [ ! -e /var/etc/.usbimage ]; then
				startInit banner.sh
				startInit sysfs.sh
				startInit volatile-media.sh
				startInit mdev
				startDevoled
				startDevoled starting
				startFlash
				startWorkaround
				startReloadconfig
				startInit mountall.sh
				#startUsbImage
			fi
			startDepmod
			startInit modutils.sh
			startInit modload.sh
			startUsbImage
			startInit bootlogo
			startDevoled Atemio
			if [ "$board" == "dm900" ] || [ "$board" == "dm920" ; then mount /dev/mmcblk0p3 /mnt; fi
			startBootlogo
			startInit checkroot.sh
			startInit early-configure.sh
			startInit read-only-rootfs-hook.sh
			startInit urandom
			startInit read-only-rootfs-hook.sh
			startInit urandom
			startRootWork
	
			startMountSwap
			startReloadconfig
			startInit hotplug.sh
			
			if ([ "$board" = "dm900" ] || [ "$board" == "dm920" ] || [ "$board" == "wetek" ] || [ "$board" == "hd51" ] || [ "$board" == "mutant51" ]) && [ ! -e /mnt/swapextensions ]; then mkdir /mnt/swapextensions; fi
			if ([ "$board" = "dm900" ] || [ "$board" == "dm920" ]) && [ ! -e /ba ]; then mkdir /ba; fi
			startUserSettings $INPUT
			startInit populate-volatile.sh
			startInit devpts.sh
			startInit dmesg.sh
			setLibraryPath
			startPluginScript
			startInit alsa-state
			startInit hostname.sh
			startInit bootmisc.sh
			startInit networking
			startWlan
			startNtpdate
			startSetTime
			startEmu $INPUT
			startSetFB
			startAutofs
			startHotplug
			#startEvent
			startRcreboot
			#startInit vsftpd
			chown root /etc/vsftpd.conf
			startInit telnetd.busybox
		fi
		startCi;
		startSethyprid;
		startGui;;
	last)
		if [ ! -e /tmp/.init3 ];then
			startInit dbus-1
			startInit dropbear
			startInit portmap
			startInit mountnfs.sh
	#		startInit autofs
			startInit busybox-cron
			startInit busybox-udhcpc
			startOpenVPN
			startNFS
			startInit smbnetfs.sh
			startInit djmount
			startSamba
			startInit syslog
			startInit avahi-daemon
			startInit rmnologin.sh
			touch /tmp/.init3
			sleep 3
			checkEmu
			startDLNA
			startXUPNPD
			startInaDyn &
			startUsercmd
			sleep 10
			startHotplug
		fi
		;;
	reboot)
		startDropcaches
		startReboot;;
esac


echo "[$0] exit"
