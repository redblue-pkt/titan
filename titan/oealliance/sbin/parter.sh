#!/bin/sh
echo "[parter.sh] ---- start ----"
echo "[parter.sh] $*"

model=`cat /etc/model`
arch=`cat /etc/.arch`

device=$1
type=$2
part=$3
fs=$4
format=$5

echo "[parter.sh] device: $device"
echo "[parter.sh] type: $type"
echo "[parter.sh] part: $part"
echo "[parter.sh] fs: $fs"
echo "[parter.sh] format: $format"

if [ -z $part ]; then
	part=1
fi

if [ -z $format ]; then
	format="small"
fi

#partcount=`fdisk -l $device | grep /dev/ | grep -v Disk | wc -l`
partlist=`fdisk -l $device | grep /dev/ | grep -v Disk | awk '{print $1}' | tr '[a-z]' ' ' | awk '{print $3}' | sort -r`
partcount=`fdisk -l $device | grep /dev/ | grep -v Disk | awk '{print $1}' | wc -l`

echo "[parter.sh] read partlist: $partlist"
echo "[parter.sh] read partcount: $partcount"

if [ -z "$device" ]; then
	echo "[parter.sh] error no device"
	exit
fi

#we need kill automount, so devices umounted and kernel can reread new partition table
killall automount
swap.sh "swapfileoff" "" "$model"
umount -fl /autofs > /dev/null 2>&1
partcount=0
for ROUND in $partlist; do
	partcount=`expr $partcount + 1`
	echo "[parter.sh] umount $device$ROUND"
	umount -fl "$device"$ROUND > /dev/null 2>&1
done

echo "[parter.sh] set partcount: $partcount"

if [ "$format" == "large" ]; then
	parted -s $device mklabel gpt
	if [ $? != 0 ]; then
		exit 1
	fi
	parted -s $device unit TB
	if [ $? != 0 ]; then
		exit 1
	fi
	parted -s $device mkpart "primary 0 -0"
	if [ $? != 0 ]; then
		exit 1
	fi
	echo "[parter.sh] ---> " 
	parted -s $device print
	echo " "
	exit 0
elif [ "$type" == "create" ]; then
	for ROUND in $partlist; do
		echo [parter.sh] delete $device$ROUND
		echo "p" > /tmp/del_part
		echo "d" >> /tmp/del_part
		echo "$ROUND" >> /tmp/del_part
		echo " " >> /tmp/del_part
		echo " " >> /tmp/del_part
		echo "p" >> /tmp/del_part
		echo "w" >> /tmp/del_part
		cat /tmp/del_part
		echo "fdisk $device < /tmp/del_part"
		fdisk $device < /tmp/del_part
		sleep 1
		echo [parter.sh] delete $device$ROUND done
	done
	part=1
	echo "p" > /tmp/create_part
	echo "n" >> /tmp/create_part
	echo "p" >> /tmp/create_part
	echo "$part" >> /tmp/create_part
	echo " " >> /tmp/create_part
	echo " " >> /tmp/create_part
	echo "p" >> /tmp/create_part
	echo "w" >> /tmp/create_part

	echo [parter.sh] create "$device"$part
	cat /tmp/create_part
	echo "fdisk $device < /tmp/create_part"
	fdisk $device < /tmp/create_part
fi

sleep 1
echo partcount: $partcount
echo part: $part
echo fs: $fs
echo type: $type

echo "[parter.sh1] #####################"
ls -al /dev/sda*

# set fs-type after create
if [ "$fs" == "vfat" ]; then
	echo "t" > /tmp/update_part
	if [ "$partcount" -gt "1" ] && [ "$type" == "update" ]; then
			echo "$part" >> /tmp/update_part
	fi
	echo "b" >> /tmp/update_part
else
	echo "t" > /tmp/update_part
	if [ "$partcount" -gt "1" ] && [ "$type" == "update" ]; then
		echo "$part" >> /tmp/update_part
	fi
	echo "83" >> /tmp/update_part
fi
echo " " >> /tmp/update_part
echo " " >> /tmp/update_part
echo "p" >> /tmp/update_part
echo "w" >> /tmp/update_part
echo "[parter.sh] /tmp/update_part"
cat /tmp/update_part
sync

sleep 1
fdisk $device < /tmp/update_part
echo [parter.sh] change "$device""$part" id to fs="$fs"

echo "[parter.sh2] #####################"
ls -al /dev/sda*

if [ "$arch" = "sh4" ];then
	count=0
	for ROUND in a b c d e f g h i j k l m n o p q r s t u v w x y z; do
		wcount=0
		while [ "$wcount" -lt "15" ]
		do
			count=`expr $count + 1`
			wcount=`expr $wcount + 1`
			if [ "$device""$part" = "/dev/sd$ROUND$count" ] && [ ! -e "/dev/sd$ROUND$count"	];then	
				echo [parter.sh] create lost dev /dev/sd$ROUND$count b 8 $count
				mknod /dev/sd$ROUND$count b 8 $count
			fi			
		done
	done
fi

echo "[parter.sh3] #####################"
ls -al /dev/sda*


echo "[parter.sh] ---- ende ----"