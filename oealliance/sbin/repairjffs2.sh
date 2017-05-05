#!/bin/sh

#for test from telnet/shell use
#/tmp/repairjffs2.sh var test > /tmp/out 2>&1 &

testmodus=$2

killproc()
{
	count=0
	lastcount=`ps | tail -n1 | sed "s/ \+/ /g;s/^ //" | cut -d" " -f1`
	excludes="^ini repairjffs2.sh"
	while true; do
		count=$((count + 1))
		if [ -e "/proc/$count/cmdline" ]; then
			cmd=`cat "/proc/$count/cmdline"`
			if [ ! -z "$cmd" ]; then
				treffer=0
				for exclude in $excludes; do
					if echo "$cmd" | grep -q "$exclude"; then treffer=1; fi
				done
				if [ $treffer -eq 0 ]; then
					echo "[cmd.sh] killing pid=$count cmd=$cmd"
					kill -9 "$count"
				fi
			fi
		fi
	if [ $count -ge $lastcount ]; then break; fi
	done
}

doreboot()
{
	killall infobox

	if [ "$ret" == 0 ]; then
		infobox 9999 INFO "Wiederherstellung erfolgreich" "" "Neustart in 3 Sekunden" &
	else
		infobox 9999 INFO "Wiederherstellung nicht erfolgreich" "" "Neustart in 3 Sekunden" &
	fi

	if [ ! -z "$testmodus" ]; then exit; fi
	sleep 3
	reboot -f
}

got_mtd_part()
{
	mtd=`cat /proc/mtd | grep -i "$part" | cut -d":" -f1 | tail -n1`
	echo "[repairjffs2.sh] Part $part: mtd=$mtd"
	### check mtd part ###
	if [ -z "$mtd" ] || [ `echo -e "$mtd" | wc -l` -ne 1 ] || [ "${mtd:0:3}" != "mtd" ] || [ "$mtd" == "mtd0" ]; then
		echo "[repairjffs2.sh] Uncorrect MTD Partition: $mtd" "MTD err"
		ret=1
		doreboot
	fi
	mtd="${mtd:3:1}"
}

model=`cat /etc/model`
ret=0

if [ "$1" == "var" ]; then
	part="Var"
	got_mtd_part
elif [ "$1" == "mnt" ]; then
	part="Swap"
	got_mtd_part
else
	echo "[repairjffs2.sh] var|mnt [test]"
	ret=1
	doreboot
fi

cp /var/usr/share/fonts/default.ttf /tmp # for infobox

rm -r /tmp/"$1".sich > /dev/null 2>&1
rm -r /tmp/"$1".restore > /dev/null 2>&1

mkdir /tmp/"$1".sich
if [ $? -ne 0 ]; then
	echo "[repairjffs2.sh] can't create dir /tmp/$1.sich"
	ret=1
	doreboot
fi

killall -9 rcS
killall -9 start.sh
killall automount
emu.sh stop
killproc
if [ "$1" == "var" ]; then
	swap.sh "swapfileoff" "" "$model"
fi
umount -fl /autofs > /dev/null 2>&1
cd /tmp

#close filedescriptors from titan
exec 3>&-; exec 4>&-; exec 5>&-; exec 6>&-; exec 7>&-
exec 8>&-; exec 9>&-; exec 10>&-; exec 11>&-; exec 12>&-
exec 13>&-; exec 14>&-; exec 15>&-; exec 16>&-; exec 17>&-
exec 18>&-; exec 19>&-; exec 20>&-; exec 21>&-; exec 22>&-
exec 23>&-; exec 24>&-; exec 25>&-; exec 26>&-; exec 27>&-

infobox 9999 INFO "Erzeuge Backup" "" "Receiver bitte nicht ausschalten" &
sleep 2
cp -a /"$1"/* /tmp/"$1".sich
if [ $? -ne 0 ]; then
	echo "[repairjffs2.sh] can't copy /$1 /tmp/$1.sich"
	ret=1
	doreboot
fi
killall infobox

umount -fl /dev/mtdblock"$mtd" > /dev/null 2>&1
umount -fl /dev/mtdblock"$mtd" > /dev/null 2>&1
umount -fl /dev/mtdblock"$mtd" > /dev/null 2>&1
umount -fl /dev/mtdblock"$mtd" > /dev/null 2>&1
umount -fl /dev/mtdblock"$mtd" > /dev/null 2>&1

mount | grep -q mtdblock"$mtd" > /dev/null 2>&1
if [ $? -eq 0 ]; then
        echo "[repairjffs2.sh] can't umount /$1"
        ret=1
        doreboot
fi

echo 3 > /proc/sys/vm/drop_caches
flash_eraseall -l "Lösche: $1" -x "$model" /dev/mtd"$mtd"
busybox mount -n -t jffs2 -o rw /dev/mtdblock"$mtd" /"$1"

infobox 9999 INFO "Formatiere und Initialisiere $1" "" "Receiver bitte nicht ausschalten" &
cp -a /tmp/"$1".sich/* /"$1"
if [ $? -ne 0 ]; then
	echo "[repairjffs2.sh] can't copy /tmp/$1.sich/* /$1"
	ret=1
fi

#start inetd
if [ ! -z "$testmodus" ]; then
	if [ "$1" == "var" ]; then
		/usr/sbin/inetd /tmp/"$1".sich/etc/inetd.conf
	else
		/usr/sbin/inetd
	fi
fi

sync
doreboot