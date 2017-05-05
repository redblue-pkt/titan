#!/bin/sh
model=`cat /etc/model`
echo "[cmd.sh] ---- start ----"
echo "[cmd.sh] $*"

arch=`cat /etc/.arch`
startconfig=/mnt/config/start-config
if [ ! -e "$startconfig" ]; then startconfig="/etc/titan.restore/mnt/config/start-config"; fi

. $startconfig

if [ ! "$debug" == 'off' ]; then
	echo "[cmd.sh] start /sbin/cmd"
fi
if [ "$debug" = 'full' ]; then
	set -x
fi

action="$1 $2"
chkret="0"

if [ "$arch" == "sh4" ];then
stfbcontrol a 255
fi

art=`echo "$action" | cut -d "." -f1`
type=`echo "$action" | cut -d "." -f2 | cut -d " " -f1`
dev=$2

record=$3
ext=$4
swap=$5
backup=$6

### function ###
killproc()
{
	count=0
	lastcount=`ps | tail -n1 | sed "s/ \+/ /g;s/^ //" | cut -d" " -f1`
	excludes="^ini inetd evremote2 telnetd cmd.sh infobox lircd"
#	excludes="^ini evremote2 cmd.sh infobox lircd"

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

if [ "$arch" == "mipsel" ] || [ "$arch" == "arm" ];then
	touch /tmp/.update
fi

if [ "$arch" == "sh4" ];then
	killall -9 rcS
fi
killall -9 automount

emu.sh stop
killproc
if [ "$art" == "mkfs" ]; then
	swap.sh "swapfileoff" "" "$model"
fi
### check for mipsel because of "umount: can't forcibly umount /autofs: Invalid argument"
if [ "$arch" == "mipsel" ] || [ "$arch" == "arm" ]; then
	echo stop autofs service
	/etc/init.d/autofs stop
	umount -l /autofs# > /dev/null 2>&1
	umount -l $dev# > /dev/null 2>&1
else
	umount -fl /autofs > /dev/null 2>&1
	umount -fl $dev > /dev/null 2>&1
fi

### check ob fsck parameter mit filesystem uebereinstimmt ###
if [ "$art" == "fsck" ]; then
 	chktype=`blkid -w /dev/null -c /dev/null -s TYPE | grep -m1 "$dev" | sed 's/^.*TYPE="//;s/".*$//'`
 	if [ "$chktype" == "fat" ] || [ "$chktype" == "vfat" ] || [ "$chktype" == "msdos" ]; then
 		if [ "$type" != "fat" ] && [ "$type" != "vfat" ] && [ "$type" != "msdos" ]; then
 			echo "[cmd.sh] fsck filesystem mismatch parameter:$type blkid:$chktype"
 			type=$chktype
 			infobox 100 "FSCK" "Filesystem Check ($type) on $dev" &
 			if [ "$model" = "atemio520" ] || [ "$model" = "atemio530" ]; then
 				echo "FSCK" > /dev/vfd
 			else
 				echo "FSCK: start" > /dev/vfd
 			fi
			action="/sbin/fsck.vfat -y $dev"
			chkret="1"
		fi
	fi
	
	if [ "$chktype" == "ext2" ] || [ "$chktype" == "ext3" ] || [ "$chktype" == "ext4" ] || [ "$chktype" == "jfs" ]; then
		if [ "$type" != "ext2" ] && [ "$type" != "ext3" ] && [ "$type" != "ext4" ] && [ "$type" != "jfs" ]; then
			echo "[cmd.sh] fsck filesystem mismatch parameter:$type blkid:$chktype"
 			type=$chktype
			action="/sbin/fsck.$type -f -p $dev"
		fi
	fi
fi

### write LOG ###
echo "[cmd.sh] --------------------"
echo "[cmd.sh] action: $action"
echo "[cmd.sh] art: $art"
echo "[cmd.sh] type: $type"
echo "[cmd.sh] dev: $dev"
echo "[cmd.sh] cmd: $action"
echo "[cmd.sh] --------------------"

mount

### execute CMD ###
$action
ret=$?

### check if CMD OK ###
if [ "$chkret" == "1" ]; then
	if [ $ret -ne 0 ]; then
		killall infobox
		infobox 9999 "FEHLER" "Es ist ein Fehler aufgetreten." " " "Die Fehlermeldung befindet sich in /var/cmd.log" &
		if [ "$model" = "atemio520" ] || [ "$model" = "atemio530" ]; then
			echo "Err" > /dev/vfd
		else
			echo "CMD: Fehler" > /dev/vfd
		fi
	else
		killall infobox
		infobox 9999 "ERFOLGREICH" "Der Befehl wurde erfolgreich ausgefuehrt." &
		if [ "$model" = "atemio520" ] || [ "$model" = "atemio530" ]; then
			echo "OK" > /dev/vfd
		else
			echo "CMD: OK" > /dev/vfd
		fi
	fi
fi

sleep 5

### create folders if cmd is mkfs ###
if [ "$art" == "mkfs" ] && [ $ret -eq 0 ] && ([ "$record" == "1" ] || [ "$ext" == "1" ] || [ "$swap" == "1" ] || [ "$backup" == "1" ]); then
	echo "[cmd.sh] mount -t $type $dev /tmp/mnt"
	mkdir /tmp/mnt
	killall infobox
	infobox 3 "MKFS" "  Mount -t $type $dev to /tmp/mnt  " &
	if [ "$model" = "atemio520" ] || [ "$model" = "atemio530" ]; then
		echo "Mnt" > /dev/vfd
	else
 		echo "Mount: $dev" > /dev/vfd
 	fi
 	mount -t $type $dev /tmp/mnt

	if [ "$record" == "1" ]; then
		sleep 3
		echo "[cmd.sh] mkdir /tmp/mnt/movie"
		killall infobox
		infobox 3 "MKFS" "   Create movie   " &
		if [ "$model" = "atemio520" ] || [ "$model" = "atemio530" ]; then
			echo "Mov" > /dev/vfd
		else
	 		echo "Create: movie" > /dev/vfd
	 	fi
	 	mkdir /tmp/mnt/movie
	fi
 	
 	if [ "$ext" == "1" ]; then
 		sleep 3
	 	echo "[cmd.sh] mkdir /tmp/mnt/swapextensions"
		killall infobox
		infobox 3 "MKFS" "   Create swapextensions   " &
		if [ "$model" = "atemio520" ] || [ "$model" = "atemio530" ]; then
			echo "Ext" > /dev/vfd
		else
	 		echo "Create: ext" > /dev/vfd
	 	fi
	 	mkdir /tmp/mnt/swapextensions
 	fi
 	
 	if [ "$swap" == "1" ]; then
 		sleep 3
	 	echo "[cmd.sh] mkdir /tmp/mnt/swapdir"
		killall infobox
		infobox 3 "MKFS" "   Create swapdir   " &
		if [ "$model" = "atemio520" ] || [ "$model" = "atemio530" ]; then
			echo "Swap" > /dev/vfd
		else
	 		echo "Create: swapdir" > /dev/vfd
	 	fi
	 	mkdir /tmp/mnt/swapdir
 	fi
 	
 	if [ "$backup" == "1" ]; then
 		sleep 3
	 	echo "[cmd.sh] mkdir /tmp/mnt/backup"
		killall infobox
		infobox 3 "MKFS" "   Create backup   " &
		if [ "$model" = "atemio520" ] || [ "$model" = "atemio530" ]; then
			echo "Back" > /dev/vfd
		else
	 		echo "Create: backup" > /dev/vfd
	 	fi
	 	mkdir /tmp/mnt/backup
	fi
	
	sleep 3
	echo "[cmd.sh] umount $dev"
	killall infobox
	infobox 3 "MKFS" "  Umount $dev from /tmp/mnt  " &
	if [ "$model" = "atemio520" ] || [ "$model" = "atemio530" ]; then
		echo "UMnt: $dev" > /dev/vfd
	else
 		echo "Umount: $dev" > /dev/vfd
 	fi
 	umount -fl $dev
	sleep 3
fi

### aufraeumen und restart
killall infobox
infobox 10005 "Reboot" "done Thx for waiting !!!" "Reboot in 5 seconds" &
for i in 4 3 2 1 0; do
	if [ "$model" = "atemio520" ] || [ "$model" = "atemio530" ]; then
		echo "R $i" > /dev/vfd
	else
		echo "Reboot in $i sec" > /dev/vfd
	fi
	sleep 1
done

echo "[cmd.sh] ---- ende ----"
/var/etc/autostart/start.sh reboot 2
reboot -f
