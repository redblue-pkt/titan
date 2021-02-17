startconfig=/mnt/config/start-config
if [ ! -e "$startconfig" ]; then startconfig="/etc/titan.restore/mnt/config/start-config"; fi

. $startconfig
. /sbin/start-progress
. /sbin/start-function

model=`cat /etc/model`
realbox=`cat /proc/stb/info/boxtype`
arch=`cat /etc/.arch`
board=`cat /etc/.board`
distro=`cat /etc/.distro`

starthotplug()
{
	echo "[$0] starthotplug"
	hotplug.sh first
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

	if [ -e /usr/lib/libssl.so.1.0.2 ] && [ ! -e /usr/lib/libssl.so.0.9.8 ];then
		echo "create symlink /usr/lib/libssl.so.1.0.2 -> /usr/lib/libssl.so.0.9.8"
		ln -s /usr/lib/libssl.so.1.0.2 /usr/lib/libssl.so.0.9.8
	fi

	if [ -e /usr/lib/libcrypto.so.1.0.2 ] && [ ! -e /usr/lib/libcrypto.so.0.9.8 ];then
		echo "create symlink /usr/lib/libcrypto.so.1.0.2 -> /usr/lib/libcrypto.so.0.9.80"
		ln -s /usr/lib/libcrypto.so.1.0.2 /usr/lib/libcrypto.so.0.9.8
	fi
}

startdate()
{
# dm7020hdv2 ok
#		(ntpdate -b ptbtime1.ptb.de; time=`date +%s`; echo -e `expr $time + 7200`  > /proc/stb/fp/rtc) &
#		echo "[startdate] ntpdate -b ptbtime1.ptb.de; /bin/fake-hwclock save force"
#		(ntpdate -b ptbtime1.ptb.de; /bin/fake-hwclock save force; date; date -u) &
		if [ -e /bin/fake-hwclock ]; then
			opkg remove fake-hwclock --force-depends
			date -d "1974-01-04"
		fi
#		if [ ! -e /tmp/.timefix ] && [ "$board" == "spark7162" ]; then
#			time=`date +%s`
#			echo -e `expr $time + 7200` > /proc/stb/fp/rtc
#			touch /tmp/.timefix
#		fi
}

startdelpack()
{
	if [ -e /var/etc/.firstok ]; then
		opkg remove minidlna --force-depends
		rm /var/etc/.firstok
	fi
}

startbootlogo()
{
	if [ -e /proc/stb/info/boxtype ]; then
		stbcheck=`cat /proc/stb/info/boxtype`
		if [ $stbcheck == "sf8008" ] || [ $stbcheck == "sf8008m" ] || [ $stbcheck == "ustym4kpro" ] || [ $stbcheck == "cc1" ]; then
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

startemu()
{
	emu.sh "start" "" &
}

checkemu()
{
	emuret=`emu.sh check | grep "checkemu running emu="`
	if [ -z "$emuret" ]; then
		startemu restart
	fi
}

startusercmd()
{
	if [ -e /mnt/config/usercmd.sh ]; then
		/mnt/config/usercmd.sh
	fi
}

startdropcaches()
{
	echo 3 > /proc/sys/vm/drop_caches
}

startrcreboot()
{
	RCOK=$(cat /mnt/config/rcconfig | grep rcok | cut -d= -f2)
	RCPOWER=$(cat /mnt/config/rcconfig | grep rcpower | cut -d= -f2)
	RCDEV=$(grep "rcdev=" /mnt/config/titan.cfg | cut -d "=" -f 2)
	VFDDEV=$(grep "vfddev=" /mnt/config/titan.cfg | cut -d "=" -f 2)
	VFDTYPE=$(grep "vfdtype=" /mnt/config/titan.cfg | cut -d "=" -f 2)
	evtest $RCDEV | LC_ALL=C awk -v VFDTYPE=$VFDTYPE -v VFDDEV=$VFDDEV -v RCOK=$RCOK -v RCPOWER=$RCPOWER -f /etc/init.d/getfb.awk &
}

workarounds()
{
	if [ "$board" == "hd51" ] || [ "$board" == "multibox" ]; then
		if [ ! -e "/usr/bin/enigma2" ];then
			touch /usr/bin/enigma2
			/etc/init.d/partitions-by-name &
		fi
	fi

	if [ "$distro" == "4.4" ] && [ "$board" == "sf8008" ]; then
		echo "[$0] enable 100mbit LAN workaround"
		ethtool -s eth0 speed 100 duplex full autoneg on
	fi
}

startCi()
{
	if [ "$board" == "hd51" ]; then
		if [ -e /mnt/bin/ciplushelper ];then
			/mnt/bin/ciplushelper &
			sleep 1
		fi
	fi
}

startled()
{
	if [ "$model" == "sf8008" ]; then
		echo "[$0] startled ser power > blue"
		echo 1 > /proc/stb/fp/ledpowercolor
	fi
}

startminisatip()
{
	echo "[$0] startminisatip: check"
	activ=$(cat /mnt/config/start-config | sed -nr "s/.*minisatip=([^*]+)*/\1/p")
	if [ "$activ" == "y" ];	then
		echo "[$0] startminisatip: starting"
		/etc/init.d/minisatip restart
	fi
}

startsatipclient()
{
#	echo "[$0] startsatipclient: check"
#	activ=$(cat /mnt/config/start-config | sed -nr "s/.*satipclient=([^*]+)*/\1/p")
#	if [ "$activ" == "y" ];	then
		if [ -e /mnt/network/vtuner.conf ];then cp /mnt/network/vtuner.conf /etc;fi
		echo "[$0] startsatipclient: starting"
		/etc/init.d/satipclient restart
#	fi
}

startgui()
{
	STARTDEFAULT="/usr/local/bin/titan /mnt/config/titan.cfg"

	if [ -e "/var/etc/.checkdualboot" ] && [ -e "/usr/bin/enigma2" ] && [ -e "/usr/share/enigma2/skin.xml" ];then
#		startbootlogo
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
#	else
#		rm -f /etc/rc2.d/*
#		rm -f /etc/rc3.d/*
#		rm -f /etc/rcS.d/*
	fi

	# kill webif
	fuser -k 80/tcp

	if [ -e /mnt/config/dualboot ] && [ -e "/usr/bin/enigma2" ] && [ -e "/usr/share/enigma2/skin.xml" ];then
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

#	if [ "$START" = "/usr/bin/enigma2" ];then
#		startInit3
#		#/usr/bin/showiframe /usr/share/bootlogo.mvi
#		startbootlogo
#	fi

	if [ -z "$START" ]; then START="$STARTDEFAULT"; fi

	LD_PRELOAD=$LIBS $START

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
			if [ $stbcheck == "sf8008" ] || [ $stbcheck == "sf8008m" ] || [ $stbcheck == "ustym4kpro" ] || [ $stbcheck == "cc1" ] ; then
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
			/var/etc/autostart/start.sh reboot 
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

case $1 in
	first)
#		starthyprid
#		startmnt
#		startplugins
		starthotplug
		startemu
#		startopkg
		startdate
		startbootlogo
		startlibs
		starthomedir
		startrcreboot
#		startdelpack
		startCi
		workarounds
		startled
		startsatipclient
		startgui;;
	last)
		startminisatip
		checkemu
		startusercmd;;
	reboot)
		starthyprid
		startdropcaches;;
esac

