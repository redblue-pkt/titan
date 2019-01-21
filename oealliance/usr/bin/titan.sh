if [ -L /mnt ];then
	rm -f /mnt
fi

if [ -e /var/etc/.erasemtd ] || [ ! -e /mnt/swapextensions ]; then
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

date
echo npdate
ntpdate -b ptbtime1.ptb.de &
#sleep 5

echo "####################################"
date
echo "####################################"

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

if [ -d /home/root ]; then
	cd /home/root
	export HOME=/home/root
fi

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

LD_PRELOAD=$LIBS /usr/local/bin/titan /mnt/config/titan.cfg

# enigma2 exit codes:
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
