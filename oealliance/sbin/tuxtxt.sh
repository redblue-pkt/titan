#!/bin/sh

startconfig=/mnt/config/start-config
if [ ! -e "$startconfig" ]; then startconfig="/etc/titan.restore/mnt/config/start-config"; fi

. $startconfig

if [ "$tuxtxt_cpufrequ_workaround" == "2" ]; then
	videomode=`cat /proc/stb/video/videomode`
	if [ "$videomode" == "pal" ] || [ "$videomode" == "576p50" ] || [ "$videomode" == "576i50" ]; then
		echo "[tuxtxt.sh] do not use workaround on videomode $videomode"
	else
		echo 720p50 > /proc/stb/video/videomode
	fi
else
	infobox 3 "INFO" "    Teletext wird geladen ...    " &
fi

startCpu_frequ() {
	if [ $pll0_ndiv_mdiv -gt 0 ]; then
		echo $pll0_ndiv_mdiv > /proc/cpu_frequ/pll0_ndiv_mdiv
	fi
	if [ $pll1_ndiv_mdiv -gt 0 ]; then
		echo $pll1_ndiv_mdiv > /proc/cpu_frequ/pll1_ndiv_mdiv
	fi
	if [ $pll1_fdma_bypass -eq 1 ]; then
		echo 1 > /proc/cpu_frequ/pll1_fdma_bypass
	fi
}

startCpu_frequ_stop() {
	pll0_ndiv_mdiv=15110 # 531MHZ
	pll1_ndiv_mdiv=34057 # 399MHZ
	echo 0 > /proc/cpu_frequ/pll1_fdma_bypass
	startCpu_frequ
}

# setzte cpu frequ auf standard
if [ "$tuxtxt_cpufrequ_workaround" == "1" ]; then
	startCpu_frequ_stop
fi

echo /usr/bin/tuxtxt "$1" "$2"
/usr/bin/tuxtxt "$1" "$2"

if [ "$tuxtxt_cpufrequ_workaround" == "1" ]; then
	startCpu_frequ
fi

if [ "$tuxtxt_cpufrequ_workaround" == "2" ]; then
	echo $videomode > /proc/stb/video/videomode
fi	
