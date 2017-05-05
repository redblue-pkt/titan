
#!/bin/sh
#

#if [ `mount | grep ^"tmpfs on /var/volatile" | wc -l` -eq 0 ];then
#	logfile=/etc/mdev/mount.log
#else
	logfile=/tmp/autofs.mount.log
#fi

echo "######################## autofs start ##########################" > $logfile
echo inputline: $* >> $logfile

fstype=$1
device=$3
directory=$4

echo device: $device >> $logfile
echo directory: $directory >> $logfile

echo "[AutoFS-MOUNT] check directory pfad muss /autofs not /media/autofs"
echo "[AutoFS-MOUNT] device: $device"
echo "[AutoFS-MOUNT] directory: $directory"

if [ $device = "tmpfs" ];then
	echo "[AutoFS-MOUNT] skip"
	exit
fi

if [ ! -z "$fstype" ]; then
	if [ "$fstype" == "ftpfs" ]; then
		echo curlftpfs -o $2 $3 $4 >> $logfile
		timeout -t 15 -s 9 curlftpfs -o $2 $3 $4
#		sleep 5
		exit $?
	elif [ "$fstype" == "smbfs" ]; then
		echo fusesmb -o $2 $4 >> $logfile
		timeout -t 15 -s 9 fusesmb -o $2 $4
#		sleep 5
		exit $?
	elif [ "$fstype" == "upnpfs" ]; then
		echo djmount -o $2 $4 /tmp/ftp >> $logfile
		timeout -t 15 -s 9 djmount -o $2 $4
#		sleep 5
		exit $?		
	fi

	if [ ! -z "$device" ] && [ ! -z "$directory" ] ; then
		startconfig=/mnt/config/start-config
		if [ ! -e "$startconfig" ]; then startconfig="/etc/titan.restore/mnt/config/start-config"; fi

		ntfsrw=`cat "$startconfig" | grep ntfs_write= | cut -d "=" -f2 | cut -d'"' -f2`

		if [ "$ntfsrw" = "0" ]; then ntfsrw="disable"; fi
		if [ "$ntfsrw" = "1" ]; then ntfsrw="ntfs-3g"; fi
		if [ "$ntfsrw" = "2" ]; then ntfsrw="ntfsmount"; fi
				
		if [ "$ntfsrw" = "" ]; then ntfsrw="disable"; fi
		echo "ntfsrw: $ntfsrw" >> $logfile

		if [ "$fstype" == "ntfs" ] && [ "$ntfsrw" == "ntfsmount" ]; then
			echo "[AutoFS-MOUNT] mount $device on $directory (type=$ntfsrw)" >> $logfile
			echo "[AutoFS-MOUNT] ntfsmount $device $directory $opt" >> $logfile
			timeout -t 15 -s 9 ntfsmount "$device" "$directory" -o force,allow_other,use_ino,kernel_cache,nonempty,rw
			exit $?
		elif [ "$fstype" == "ntfs" ] && [ "$ntfsrw" == "ntfs-3g" ]; then
			echo "[AutoFS-MOUNT] mount $device on $directory (type=$ntfsrw)" >> $logfile
			echo "[AutoFS-MOUNT] ntfs-3g $device $directory $opt" >> $logfile
			timeout -t 15 -s 9 ntfs-3g "$device" "$directory" -o allow_other,use_ino,kernel_cache,nonempty,rw
			exit $?
		elif [ "$fstype" == "ntfs" ]; then
			echo "[AutoFS-MOUNT] mount $device on $directory (type=$fstype)" >> $logfile
			echo "[AutoFS-MOUNT] mount -t $fstype $device $directory" >> $logfile
			timeout -t 15 -s 9 mount -t "$fstype" "$device" "$directory"
			exit $?
		fi
	fi
fi

exit 1