#!/bin/sh
#
# $1 = aktion [showInfoDirect | loop | swapalloff | swapfileoff | swapramoff | swappartoff] [SWTYPE] [model]

echo "[swap.sh] ---- start ----"
echo "[swap.sh] $*"

startconfig=/mnt/config/start-config
if [ ! -e "$startconfig" ]; then startconfig="/etc/titan.restore/mnt/config/start-config"; fi

. $startconfig
. /sbin/start-function

swapdir="/var/swapdir"

aktion=$1
INPUT=$1
SWTYPE="$2"
model="$3"
CMD="[swap.sh]"

case $SWTYPE in "") SWTYPE="titan";; esac
case $model in "") read model < /etc/model;; esac
case $debug in full) set -x;; esac

### general functions ###

startSwapinfo() {
	case $debug in on|full) echo "$CMD [$INPUT] SWAP mount swapfile ($retstr)";; esac
	case $aktion in
		showInfoDirect)
			infobox 3 SWAP "   SWAP: $retstr   " &
			if [ "$model" = "atemio520" ] || [ "$model" = "atemio530" ]; then
				echo "SWAP" > /dev/vfd
			else
				echo "SWAP: $retstr" > /dev/vfd
			fi;;
		*) echo "SWAP: $retstr" >> /tmp/.infobox;;
	esac
	> /tmp/.swapready
}

### ramswap functions ###

startMountramswap() {
	if grep -q /dev/ramzswap0 /proc/swaps; then return; fi
	case $debug in on|full) echo "$CMD [$INPUT] start startMountramswap";; esac

	case $model in
		ufs912|ufs913|spark|spark7162)
			echo "$CMD [$INPUT] skip Load ramswap compress/decompress";;
		*)
			echo "$CMD [$INPUT] Load ramswap compress/decompress"
			/sbin/insmod /lib/modules/lzo1x_compress.ko
			/sbin/insmod /lib/modules/lzo1x_decompress.ko;;
	esac

#	disksize=`cat /var/ramswap`
#	if [ -z "$disksize" ];then
#		disksize=65535
#	fi
#	/sbin/insmod /lib/modules/ramzswap.ko disksize_kb=$disksize
#	echo disksize: $disksize

	case $model in
		ufs910|atevio700|atevio7000) /sbin/insmod /lib/modules/ramzswap.ko disksize_kb=32768;;
		*)      /sbin/insmod /lib/modules/ramzswap.ko disksize_kb=65535;;
	esac

	sleepms 250
	swapon /dev/ramzswap0
	case $? in
		0)
			#echo 60 > /proc/sys/vm/swappiness
			retstr="RAM";;
		*)
			retstr="ERROR";;
	esac
	startSwapinfo
}

startUmountramswap() {
	if ! grep -q /dev/ramzswap0 /proc/swaps; then return; fi
	case $debug in on|full) echo "$CMD [$INPUT] start startUmountramswap";; esac
	swapoff /dev/ramzswap0
	/sbin/rmmod /lib/modules/ramzswap.ko

	case $model in
		ufs912|ufs913|spark|spark7162)
			echo "$CMD [$INPUT] skip Unload ramswap compress/decompress";;
		*)
			echo "$CMD [$INPUT] Unload ramswap compress/decompress"
			/sbin/rmmod /lib/modules/lzo1x_decompress.ko
			/sbin/rmmod /lib/modules/lzo1x_compress.ko;;
	esac
}

### swapfile functions ###

startCreateswapfile() {
	case $debug in on|full) echo "$CMD [$INPUT] start startCreateswapfile";; esac
	size10=$(($1/10))
	rest=$(($1-($size10*10)))
	killall -stop rcS
	killall -stop "$SWTYPE"
	rm -f "$swapdir/swapfile"; sync
	dd if=/dev/zero of="$swapdir/swapfile.tmp" bs=1024 count=$size10; sync
	for i in 1 2 3 4 5 6 7 8 9 10; do
		cat "$swapdir/swapfile.tmp" >> "$swapdir/swapfile"; sync
		killall -4 infobox
		s=$(($s+9))
		if [ "$model" = "atemio520" ] || [ "$model" = "atemio530" ]; then
			echo "S"$s > /dev/vfd
		else
			echo "SWAPFILE ($s %)" > /dev/vfd
		fi
	done
	dd if=/dev/zero of="$swapdir/swapfile.tmp" bs=1024 count="$rest"; sync
	cat "$swapdir/swapfile.tmp" >> "$swapdir/swapfile"; sync
	rm -f "$swapdir/swapfile.tmp"; sync
	killall -4 infobox
	s=$(($s+9)) 
	if [ "$model" = "atemio520" ] || [ "$model" = "atemio530" ] || [ "$model" = "spark" ]; then
		echo "S"$s > /dev/vfd
	else
		echo "SWAPFILE ($s %)" > /dev/vfd
	fi
	mkswap "$swapdir/swapfile"
	killall -4 infobox
	s=$(($s+1))
	if [ "$model" = "atemio520" ] || [ "$model" = "atemio530" ] || [ "$model" = "spark" ]; then
		echo "S"$s > /dev/vfd
	else
		echo "SWAPFILE ($s %)" > /dev/vfd
	fi
	sync
	killall -cont "$SWTYPE"
	killall -cont rcS
}

startMountswapfile() {
	case $aktion in
		loop)
			if grep -q "^`losetup -a | grep -m1 swapdir/swapfile | cut -d":" -f1` " /proc/swaps; then return; fi
			swapfile=`losetup -f`
 			losetup "$swapfile" "$swapdir/swapfile";;
		*)
			if grep -q swapdir/swapfile /proc/swaps; then return; fi
			swapfile="$swapdir/swapfile";;
	esac
	swapon "$swapfile"
	if [ $? -ne 0 ]; then
		mkswap "$swapfile"
		sync
		swapon "$swapfile"
	fi
	case $? in
		0)
			#echo 100 > /proc/sys/vm/swappiness
			retstr="FILE"
			startUmountramswap;;
		*)		
			startMountramswap
			return;;
	esac
	startSwapinfo
}

startUmountswapfile() {
	case $aktion in
		loop)
			swapfile=`losetup -a | grep -m1 swapdir/swapfile`; fastcut "$swapfile" ":" 1; swapfile="$fcr"
			if grep -q "^$swapfile " /proc/swaps; then return; fi;;
		*)
			if ! grep -q swapdir/swapfile /proc/swaps; then return; fi
			swapfile="$swapdir/swapfile";;
	esac
	case $debug in on|full) echo "$CMD [$INPUT] start startUmountswapfile";; esac
	swapoff "$swapfile"
	case $aktion in loop) losetup -d "$swapfile";; esac
	#echo 60 > /proc/sys/vm/swappiness
}

startSwapfile() {
	case $debug in on|full) echo "$CMD [$INPUT] start startSwapfile";; esac
	if [ ! -e "$swapdir/swapfile" ]; then
		case $debug in on|full) echo "SWAP create $swapsize KB swapfile";; esac
		freesize=`getfreespace "$swapdir"`
		case $freesize in "") freesize=0;; esac
		if [ $freesize -lt $swapsize ]; then
			if [ "$model" = "atemio520" ] || [ "$model" = "atemio530" ] || [ "$model" = "spark" ]; then
				echo "ERR" > /dev/vfd
			else
				echo "CREATE SWAPF. ERR" > /dev/vfd
			fi
			infobox 5 SWAP "Create swapfile error: $swapdir/swapfile" "Space needed $swapsize KB" "Space free $freesize KB"
			startUmountswapfile
			startMountswapfile
			return 1
		fi
		if [ "$model" = "atemio520" ] || [ "$model" = "atemio530" ] || [ "$model" = "spark" ]; then
			echo "SWAP" > /dev/vfd
		else
			echo "CREATE SWAPFILE" > /dev/vfd
		fi
		infobox 15009 SWAP "Create swapfile $swapsize KB $swapdir/swapfile" &
		sleep 2
		startUmountswapfile
		startCreateswapfile $swapsize
		killall infobox
		startMountswapfile
	else
		swap=`ls -l "$swapdir/swapfile" 2>/dev/null | sed "s/ \+/ /g"`; fastcut "$swap" " " 5; swap="$fcr"
		case $swap in "") swap=0;; esac
		swap=$(($swap/1024))
		case $swap in
			$swapsize)
				startUmountswapfile
				startMountswapfile;;
			*)
				freesize=`getfreespace "$swapdir"`
				case $freesize in "") freesize=0;; esac
				if [ $freesize -lt $swapsize ]; then
					if [ "$model" = "atemio520" ] || [ "$model" = "atemio530" ] || [ "$model" = "spark" ]; then
						echo "ERR" > /dev/vfd
					else
						echo "CHANGE SWAPF. ERR" > /dev/vfd
					fi
					infobox 5 SWAP "Change swapfile error: $swapdir/swapfile" "Space needed $swapsize KB" "Space free $freesize KB"
					startUmountswapfile
					startMountswapfile
					return 1
				fi
				if [ "$model" = "atemio520" ] || [ "$model" = "atemio530" ] || [ "$model" = "spark" ]; then
					echo "SWAP" > /dev/vfd
				else
					echo "CHANGE SWAPFILE" > /dev/vfd
				fi
				infobox 15009 SWAP "Change swapfile $swap > $swapsize KB $swapdir/swapfile" &
				sleep 2
				startUmountswapfile
				startCreateswapfile $swapsize
				killall infobox
				startMountswapfile;;
		esac
	fi
}

### swappart functions ###

startCreateswappart() {
	case $debug in on|full) echo "$CMD [$INPUT] start startCreateswappart";; esac
	mkswap "/dev/$1"
	case $? in
		0)
			echo "SWAPPART" > /tmp/swaplabel
			dd if=/tmp/swaplabel of="/dev/$1" seek=1052 bs=1 count=8
			rm -f /tmp/swaplabel
			echo "$1#swap#SWAPPART-$1" > /tmp/.swappartdev;;
	esac
}

startMountswappart() {
	if grep -q "^/dev/$1" /proc/swaps; then return; fi
	swapon "/dev/$1"
	if [ $? -ne 0 ]; then
			startCreateswappart "$1"
			sync
			swapon "/dev/$1"
	fi
	case $? in
		0)
			#echo 100 > /proc/sys/vm/swappiness
			retstr="PARTITION"
			startUmountramswap;;
		*)
			startMountswapfile
			return;;
	esac
	startSwapinfo
}

startUmountswappart() {
	if [ -e /tmp/.swappartdev ]; then
		read swappartdev < /tmp/.swappartdev; fastcut "$swappartdev" "#" 1; swappartdev="$fcr"
	else
		return
	fi
	if ! grep -q "^/dev/$swappartdev" /proc/swaps; then return; fi
	case $debug in on|full) echo "$CMD [$INPUT] start startUmountswappart";; esac
	swapoff "/dev/$swappartdev"
}

startSwappart() {
	case $debug in on|full) echo "$CMD [$INPUT] start startSwappart";; esac
	read swappart < /tmp/.swappartdev
	fastcut "$swappart" "#" 1; swappartdev="$fcr"
	fastcut "$swappart" "#" 2; swapparttype="$fcr"
	if [ "$swapparttype" != "swap" ]; then
		infobox 15009 SWAP "create Swappart on /dev/$swappartdev" &
		sleep 2
		startUmountswappart
		startCreateswappart "$swappartdev"
		killall infobox
	fi
	startMountswappart "$swappartdev"
}

### end functions ###

case $aktion in
	swapalloff)
		startUmountswappart
		startUmountswapfile
		startUmountramswap
		echo "[swap.sh] ---- ende 1 ----"
		exit;;
	swapfileoff)
		startUmountswapfile
		echo "[swap.sh] ---- ende 2 ----"
		exit;;
	swapramoff)
		startUmountramswap
		echo "[swap.sh] ---- ende 3 ----"
		exit;;
	swappartoff)
		startUmountswappart
		echo "[swap.sh] ---- ende 4 ----"
		exit;;
esac

case $swap in
	y)
		if [ -e "/tmp/.swappartdev" ]; then
			startSwappart
		elif [ -e "/tmp/.swapdirdev" ] ; then
			if [ -e "/mnt/network/.recordshare" ]; then
				if [ -z "$aktion" ] && [ "`cat /tmp/.swapdirdev`" == "`cat /mnt/network/.recordshare`" ]; then aktion="loop"; fi
			fi
			startSwapfile
		else
			startUmountswappart
			startUmountswapfile
			startMountramswap
		fi;;
	*)
		startUmountswappart
		startUmountswapfile
		startMountramswap;;
esac

echo "[swap.sh] ---- ende ----"