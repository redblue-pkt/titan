if [ -L /mnt ];then
	mv -f /mnt /mnt_old
	cp -a /etc/titan.restore/mnt /
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

#sleep 5
/usr/local/bin/titan /mnt/config/titan.cfg

ret=$?# enigma2 exit codes:
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

