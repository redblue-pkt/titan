startconfig=/mnt/config/start-config
if [ ! -e "$startconfig" ]; then startconfig="/etc/titan.restore/mnt/config/start-config"; fi

. $startconfig
. /sbin/start-progress
. /sbin/start-function

model=`cat /etc/model`
realbox=`cat /proc/stb/info/boxtype`
arch=`cat /etc/.arch`
board=`cat /etc/.board`

starthotplug()
{
	echo "[$0] starthotplug"
	hotplug.sh first
}

startautofsrestart()
{
		echo "[$0] startautofsrestart"
		mkdir -p /mnt/network
		cp /etc/titan.restore/mnt/network/auto.misc /mnt/network
		if [ ! -L /etc/auto.network ];then
			rm /etc/auto.network
			ln -s /mnt/network/auto.misc /etc/auto.network
		fi
		if [ ! -L /etc/udev/mount-helper.sh ];then
			mv /etc/udev/mount-helper.sh /etc/udev/mount-helper_oe.sh
			ln -s /sbin/hotplug.sh /etc/udev/mount-helper.sh
		fi
		/etc/init.d/autofs restart
		starthotplug
}

startnetworkrestart()
{
		echo "[$0] startnetworkrestart"
		if [ ! -L /etc/network/interfaces ];then
			rm /etc/network/interfaces
			ln -s /mnt/network/interfaces /etc/network/interfaces
		fi
}

startmnt()
{
	echo "[$0] startmnt"
	if [ -L /mnt ];then
		rm -f /mnt
		startautofsrestart
		startnetworkrestart
	fi

	if [ -e /var/etc/.erasemtd ] || [ ! -e /mnt/swapextensions ]; then
		if [ -e /var/etc/.backupmtd ]; then
			mkdir /tmp/backupmtd
			cp -a /mnt/settings /tmp/backupmtd
			cp -a /mnt/config /tmp/backupmtd
			cp -a /mnt/network /tmp/backupmtd
			cp -a /mnt/script /tmp/backupmtd
			mkdir /tmp/backupmtd/swapextensions
			cp -a /mnt/swapextensions/player /tmp/backupmtd/swapextensions
			backuptpk
		fi

		if [ -e /media/hdd/.update/.last ];then
			BACKUPDIR=/media/hdd/.update
			BACKUPFILE=$(cat $BACKUPDIR/.last)
			cp -a $BACKUPFILE/* /mnt
			mv -f $BACKUPDIR/.last $BACKUPDIR/.last.restored
			sync
		elif [ -e /var/backup/.update/.last ];then
			BACKUPDIR=/var/backup/.update
			BACKUPFILE=$(cat $BACKUPDIR/.last)
			rm -f /mnt
			cp -a $BACKUPFILE/* /mnt
			mv -f $BACKUPDIR/.last $BACKUPDIR/.last.restored
			sync
		elif [ -e /var/swap/.update/.last ];then
			BACKUPDIR=/var/swap/.update
			BACKUPFILE=$(cat $BACKUPDIR/.last)
			cp -a $BACKUPFILE/* /mnt
			mv -f $BACKUPDIR/.last $BACKUPDIR/.last.restored
			sync
		else
			infobox -pos -1 75% 10015 "MNT" "            Formatiere Laufwerk            " &
			if [ -e /mnt ];then
				echo "remove mnt files"
				rm -rf /mnt
			fi
			cp -a /etc/titan.restore/mnt /
			mkdir /mnt/swapextensions
			mkdir /mnt/bin
			mkdir /mnt/tpk
			sleep 10

			if [ -e /var/etc/.backupmtd ]; then
				restoretpk

				rm -rf /mnt/settings
				rm -rf /mnt/config
				rm -rf /mnt/network
				rm -rf /mnt/script
				rm -rf /mnt/swapextensions/player
				cp -a /tmp/backupmtd/* /mnt
#			else
#				mkdir /mnt/tpk
#				mkdir /mnt/script
#				reset.sh
			fi

			rm -r /var/etc/.erasemtd
			rm -r /var/etc/.backupmtd

			#ubifs needs sync
			sync

			#startMicomUpdate

		#	killall infobox
		#	infobox 9999 INFO "Initializing MNT" "" "you can power off the receiver now,"  "in case it does not reboot" &
		#	sleep 2
			reboot
		fi
	fi
}

startopkg()
{
	if [ $(cat /etc/opkg/opkg.conf | grep "dest / /" | wc -l) -eq 0 ];then
		echo "dest /var /var" >> /etc/opkg/opkg.conf
	fi
	if [ $(cat /etc/opkg/opkg.conf | grep "dest /mnt/swapextensions /mnt/swapextensions" | wc -l) -eq 0 ];then
		echo "dest /mnt/swapextensions /mnt/swapextensions" >> /etc/opkg/opkg.conf
	fi
	if [ $(cat /etc/opkg/opkg.conf | grep "dest /var/swap /var/swap" | wc -l) -eq 0 ];then
		echo "dest /var/swap /var/swap" >> /etc/opkg/opkg.conf
	fi
}

startlibs()
{
	#check for sundtek tuner helper lib
	if [ -e /opt/lib/libmediaclient.so ]; then
		LIBS="/opt/lib/libmediaclient.so /usr/lib/libopen.so.0.0.0"
	elif [ -e /usr/lib/libopen.so.0.0.0 ]; then
		LIBS="/usr/lib/libopen.so.0.0.0"
	fi

	#check for specific pagecache helper lib
	if [ -e /usr/lib/libpagecache.so ]; then
		LIBS="$LIBS /usr/lib/libpagecache.so"
	fi

	#check for receiver specific passthrough helper lib
	if [ -e /usr/lib/libpassthrough.so ]; then
		LIBS="$LIBS /usr/lib/libpassthrough.so"
	fi

	# in case sysctl.conf is not properly loaded - load sysctl.conf here again...
	sysctl -p

	if [ -e /usr/lib/libssl.so.1.0.2 ] && [ ! -e /usr/lib/libssl.so.1.0.0 ];then
		echo "create symlink /usr/lib/libssl.so.1.0.2 -> /usr/lib/libssl.so.1.0.0"
		ln -s /usr/lib/libssl.so.1.0.2 /usr/lib/libssl.so.1.0.0
	fi

	if [ -e /usr/lib/libcrypto.so.1.0.2 ] && [ ! -e /usr/lib/libcrypto.so.1.0.0 ];then
		echo "create symlink /usr/lib/libcrypto.so.1.0.2 -> /usr/lib/libcrypto.so.1.0.0"
		ln -s /usr/lib/libcrypto.so.1.0.2 /usr/lib/libcrypto.so.1.0.0
	fi
}

startdate()
{
# dm7020hdv2 ok
#		(ntpdate -b ptbtime1.ptb.de; time=`date +%s`; echo -e `expr $time + 7200`  > /proc/stb/fp/rtc) &
		ntpdate -b ptbtime1.ptb.de &
}

startbootlogo()
{
	if [ -e /proc/stb/info/boxtype ]; then
		stbcheck=`cat /proc/stb/info/boxtype`
		if [ $stbcheck == "sf8008" ] || [ $stbcheck == "ustym4kpro" ] || [ $stbcheck == "cc1" ]; then
			count=`ps -ef |grep libreader |grep -v "grep" |wc -l`
			if [ 0 == $count ];then
				libreader 720P_50
			fi
		fi
	fi

	if [ -x /usr/bin/showiframe ]; then
		if [ -e /proc/stb/info/boxtype ]; then
			stbcheck=`cat /proc/stb/info/boxtype | cut -c1-2`
			if [ ! $stbcheck == "u5" ]; then
				killall -9 showiframe; sleep 1
			fi
		else
			killall -9 showiframe; sleep 1
		fi
		if [ -f /etc/enigma2/backdrop.mvi ]; then
			/usr/bin/showiframe /etc/enigma2/backdrop.mvi
		elif [ -f /usr/share/backdrop.mvi ]; then
			/usr/bin/showiframe /usr/share/backdrop.mvi
		fi
	fi
}

starthomedir()
{
	if [ -d /home/root ]; then
		cd /home/root
		export HOME=/home/root
	fi
}

startplugins()
{
	if [ ! -L /var/usr/local/share/titan/plugins ];then
		rm -rf /var/usr/local/share/titan/plugins
		ln -sf /usr/local/share/titan/plugins /var/usr/local/share/titan/plugins
	fi
}

startEmu() {
	emu.sh "start" "" &
}

startgui()
{
	fuser -k 80/tcp
	LD_PRELOAD=$LIBS /usr/local/bin/titan /mnt/config/titan.cfg

	# titan exit codes:
	#
	# 1 - halt
	# 2 - reboot
	# 3 - restart enigma
	#
	# >128 signal

	ret=$?

	if [ "$ret" -ne "1" ]; then
		if [ -e /proc/stb/info/boxtype ]; then
			stbcheck=`cat /proc/stb/info/boxtype | cut -c1-2`
			if [ $stbcheck == "u5" ]; then
				killall -9 showiframe; sleep 5
			fi
			stbcheck=`cat /proc/stb/info/boxtype`
			if [ $stbcheck == "sf8008" ] || [ $stbcheck == "ustym4kpro" ] || [ $stbcheck == "cc1" ] ; then
				killall -9 libreader; sleep 5
			fi
		fi
	fi

	case $ret in
		1)
			/sbin/halt
			;;
		2)
			if [ -f /proc/stb/fp/force_restart ]; then
				echo 1 > /proc/stb/fp/force_restart
			fi
			/sbin/reboot
			;;
		3)
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
		16)
			echo "rescue" > /proc/stb/fp/boot_mode
			/sbin/reboot
			;;
		42)
			python /usr/lib/enigma2/python/upgrade.pyo
			;;
		43)
			init 1
			;;
		44)
			# little hack but it will be fixed soon in drivers
			/usr/lib/enigma2/python/Plugins/SystemPlugins/MICOMUpgrade/bin/fbclear
			/usr/bin/showiframe /usr/lib/enigma2/python/Plugins/SystemPlugins/MICOMUpgrade/wait.mvi
			echo fpupload >/proc/vfd && sleep 3 && dd bs=256k if=/tmp/micom.bin of=/dev/mcu
			/usr/bin/showiframe /usr/lib/enigma2/python/Plugins/SystemPlugins/MICOMUpgrade/reboot.mvi
			# Wait forever for the user to power off
			while(true) ; do sleep 60 ; done
			;;
		45)
			echo "    " > /dev/lcd0
			/usr/bin/gigablue_wol
			;;
		*)
			rm -f /tmp/.listen.camd.socket.ignore
			;;
	esac
}

startmnt
startplugins
starthotplug
startEmu
startopkg
startdate
startbootlogo
startlibs
starthomedir
startgui
