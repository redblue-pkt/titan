#!/bin/sh

outfile=/tmp/sysinfo.txt
rm -f $outfile

header() {
(
	echo
	echo "********************************************************"
	echo "$1"
	echo "********************************************************"
) >> $outfile

	echo "=====>> $1 <<====="
}

start() {
(
	echo
	echo "=====>> STARTE: $1 <<====="
	echo
	$1
) >> $outfile 2>&1

	echo "STARTE: $1" > /dev/stderr
}

echo "################# sysinfo (start) ####################" > /tmp/sysinfo.txt
echo "Running sysinfo"
echo

header "Process Info"
start "ps as"
start "top -n1"
start "lsmod"

header "Network Info"
start "ifconfig"
start "route -n"
start "ip -o addr"
start "ping -c4 www.google.de"
start "ping -c4 195.96.0.1"
start "netstat -n"
start "cat /proc/smsc911x/info"
start "cat /mnt/network/interfaces"

header "Boot Info"
start "fw_printenv"
start "cat /proc/cmdline"

header "Memory Info"
start "free"
start "cat /proc/meminfo"
start "cat /proc/bpa2"
start "cat /proc/swaps"
start "cat /proc/ioports"
start "cat /proc/iomem"
start "cat /proc/mtd"

header "Media Info"
start "cat /proc/diskstats"
start "cat /proc/partitions"
start "mount"
start "df -h"
start "blkid -w /dev/null -c /dev/null"
start "fdisk -l"
start "ls -al /media/hdd"

header "Titan Info"
start "cat /mnt/config/start-config"
start "cat /mnt/config/titan.cfg"
	
header "Sonstige Info"
start "cat /proc/dma"
start "cat /proc/version"
start "cat /mnt/network/auto.misc"

echo
echo "you can find the output in /tmp/sysinfo.txt"
echo "################# sysinfo (end) ######################" >> /tmp/sysinfo.txt