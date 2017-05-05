#!/bin/sh

startmode="$1"
SWTYPE="$2"
model="$3"

SWTYPE=titan

case $model in "") read model < /etc/model;; esac

#. /sbin/start-function

startWaitfor()
{
	# $1 = file auf das wir warten
	# $2 = maximale zeit
	w=$(($2*4))
	a=0
	while [ ! -e "$1" ]; do
		usleep 250000
		a=$(($a+1))
		if [ $a -gt $w ]; then
			echo "[$0] startWaitfor timed out: maxwaittime=$2 sec waitfor=$1" >> /etc/mdev/mdev.log
			sync
			return
		fi
	done
}

fastgrep()
{
	# fastgrep input search
	# fgr = output
	oldifs="$IFS"
	IFS=$'\n'
	fgr=""
	for line in $1; do
		line1=${line/$2}
		if [ "$line" != "$line1" ]; then
				fgr="$line"
		fi
	done
	IFS="$oldifs"
}

fastcut()
{
	# fastcut input delimiter field
	#	fcr = output
	oldifs="$IFS"
	IFS="$2"
	fcr=""
	count=0
	for part in $1; do
		count=$((count+1))
		if [ $3 -eq $count ]; then
				fcr="$part"
				break
		fi
	done
	IFS="$oldifs"
}

startRepairdev()
{
	echo "[$0] startRepairdev: device=$1" >> /etc/mdev/mdev.log
	sync
	#make a second mount check
	sleep 2
	ls -d "/autofs/$device" > /dev/null
	case $? in
		0) 
				echo "[$0] startRepairdev: second mount for $device ok, no fschk" >> /etc/mdev/mdev.log
				sync;;
		*)
				killall -stop rcS
				killall -stop "$SWTYPE"			
				infobox 9999 "INFO" "Das Geraet /dev/$device konnte nicht eingebunden werden" "Soll ein Filesystemcheck durchgefuehrt werden ?" "In 30 Sek. wird automatisch ein Filesystemcheck durchgefuehrt" "" "OK = Filesystemcheck durchfuehren" "EXIT = kein Filesystemcheck" &
				echo "FSCK:$device ?" > /dev/vfd
				(sleep 30; killall abfrage) &
				pid=$!
				abfrage /dev/input/event0; ret=$?
				echo " " > /dev/vfd
				kill -9 $pid
				killall infobox
				case $ret in
					1)																	
						echo "FSCK: NO" > /dev/vfd;;
					*)	
						echo "FSCK: $device" > /dev/vfd
						startFsck "$device";;
				esac
				killall -cont "$SWTYPE"
				killall -cont rcS;;
	esac
}

startStickextensions() {
	echo "[$0] startStickextensions: device=$device label=$label type=$type" >> /etc/mdev/mdev.log
	sync
	
	if [ "$1" != "/var/swap" ] && [ "$1" != "/mnt/swapextensions" ]; then
		echo "[$0] startStickextensions: parameter not ok ($1)"
		return
	fi
	
	if [ "$1" == "/var/swap" ]; then
		if [ ! -L "$1" ]; then
			echo "[$0] startStickextensions: $1 is not a link device=$device label=$label type=$type" >> /etc/mdev/mdev.log
			sync
			return
		fi
	fi
	
	if [ ! -e "$1/etc" ]; then mkdir -p "$1/etc"; fi
	if [ ! -e "$1/bin" ]; then mkdir -p "$1/bin"; fi
	if [ ! -e "$1/lib/modules" ]; then mkdir -p "$1/lib/modules"; fi
	if [ ! -e "$1/keys" ]; then mkdir -p "$1/keys"; fi
	if [ ! -e "$1/boot" ]; then mkdir -p "$1/boot"; fi

	if [ ! -e "$1/usr/local/share/titan/saver" ]; then mkdir -p "$1/usr/local/share/titan/saver"; fi
	if [ ! -e "$1/usr/local/share/titan/picons" ]; then mkdir -p "$1/usr/local/share/titan/picons"; fi
	if [ ! -e "$1/usr/local/share/titan/skin" ]; then mkdir -p "$1/usr/local/share/titan/skin"; fi
	if [ ! -e "$1/usr/local/share/titan/plugins" ]; then mkdir -p "$1/usr/local/share/titan/plugins"; fi
	if [ ! -e "$1/usr/local/share/titan/imdbfolder" ]; then mkdir -p "$1/usr/local/share/titan/imdbfolder"; fi

	LIST=`ls -1 "$1/usr/local/share/titan/plugins"`
	for ROUND in $LIST; do
		if [ ! -e "/var/usr/local/share/titan/plugins/$ROUND" ]; then
			ln -s "$1/usr/local/share/titan/plugins/$ROUND" "/var/usr/local/share/titan/plugins/$ROUND"
		fi
	done

	LIST=`ls -1 "$1/lib/modules"`
	for ROUND in $LIST; do
		if [ ! -e "/var/lib/modules/$ROUND" ]; then
			ln -s "$1/lib/modules/$ROUND" "/var/lib/modules/$ROUND"
		fi
	done
}

startCheckdev()
{
	#in dieser funkton können device checks aufgerufen werden
	
	echo "[$0] startCheckdev: device=$device label=$label type=$type" >> /etc/mdev/mdev.log
	sync
		
	test=`pidof automount`
	
	if [ ! -z $test ]; then
			case $device in
			*)
				ls -d "/autofs/$device" >/dev/null
				case $? in 
					0)
						checkdev=0;;
					*)
						echo "[$0] startCheckdev: can not mount device=$device label=$label type=$type" >> /etc/mdev/mdev.log
						sync
						startRepairdev "$device"
						#after repair we test if we can mount the device
						ls -d "/autofs/$device" >/dev/null
						case $? in 
							0) checkdev=0;;
							*) checkdev=1;;
						esac;;						
				esac;;
		esac
	else
		echo "[$0] startCheckdev: can not check ... automount is not running" >> /etc/mdev/mdev.log
		sync
		checkdev=1
	fi
}

startIsreadonly()
{
	echo "[$0] startIsreadonly: device=$device label=$label type=$type" >> /etc/mdev/mdev.log
	sync
	timeout -t 30 -s 9 touch "/autofs/$device/checkrw.aaf" > /dev/null 2>&1
	
	if [ $? -eq 0 ]; then
		rm -f "/autofs/$device/checkrw.aaf"
		isreadonly=0
	else
		echo "[$0] startIsreadonly: found read only device=$device label=$label type=$type" >> /etc/mdev/mdev.log
		sync
		isreadonly=1
	fi
}

startChangedev()
{
	#in dieser funktion können deviceinfos wie z.B. label geändert werden (wird bei jeder partition aufgerufen)
	
	echo "[$0] startChangedev: device=$device label=$label type=$type" >> /etc/mdev/mdev.log
	sync
}

startNewdev()
{
	startconfig=/mnt/config/start-config
	if [ ! -e "$startconfig" ]; then startconfig="/etc/titan.restore/mnt/config/start-config"; fi

	. $startconfig

	blkidline=`echo -e "$blkid" | grep -m1 "^/dev/$device:"`
	
	fastgrep "$blkidline" "LABEL=";	fgr=${fgr/*LABEL=\"/}; fgr=${fgr/\"*/}; label=${fgr//[ -]/}
	fastgrep "$blkidline" "TYPE="; fgr=${fgr/*TYPE=\"/}; fgr=${fgr/\"*/}; type=${fgr//[ -]/}
	fastgrep "$blkidline" "UUID="; fgr=${fgr/*UUID=\"/}; fgr=${fgr/\"*/}; uuid=${fgr}
	
	echo "[$0] startNewdev: device=$device label=$label type=$type uuid=$uuid" >> /etc/mdev/mdev.log
	sync

	case $label in
		"") smblabel="$device NONLABEL"
		    label="NONLABEL-$device";;
		*)  smblabel="$device $label"
		    label="$label-$device";;
	esac

	startChangedev
	
	if [ -z "$swapdir" ] && [ "$label" == "SWAPPART-$device" ]; then
		echo "[$0] startNewdev: found SWAPPART device=$device label=$label type=$type" >> /etc/mdev/mdev.log
		sync
		echo "$device#$type#$label" > /tmp/.swappartdev
		swapdir=1
		swap.sh "" "$SWTYPE" "$model"
	fi
	
	if [ "$type" != "vfat" ] && [ "$type" != "ext2" ] && [ "$type" != "ext3" ]  && [ "$type" != "ext4" ] && [ "$type" != "msdos" ] && [ "$type" != "jfs" ] && [ "$type" != "ntfs" ] && [ "$device" != "mtdblock5" ] && [ "$device" != "mtdblock4" ] && [ "$device" != "mtdblock6" ]; then
		echo "[$0] startNewdev: found unsupportet medium device=$device label=$label type=$type" >> /etc/mdev/mdev.log
		sync
		return
	fi
	
	if [ "${device:0:8}" != "mtdblock" ]; then
		rm -f "/media/usb/$label"
		if [ ! -e "/media/usb" ];then
			mkdir /media/usb
		fi
		ln -sn "/autofs/$device" "/media/usb/$label"
	fi
	
	isreadonly=""
	
	if [ -z "$swapstick" ] && [ "$label" == "SWAP-$device" ]; then swapstick="$device"; fi
	if [ -z "$record" ] && [ "$label" == "RECORD-$device" ]; then
		startCheckdev
		if [ $checkdev -eq 0 ]; then
			record="$device"
		else
			return
		fi
	fi
	
	case $swapdir in
		"")
			if [ -d "/autofs/$device/swapdir" ]; then
				case $isreadonly in "") startIsreadonly;; esac
				case $isreadonly in
					0)
						echo "[$0] startNewdev: found swapdir device=$device label=$label type=$type" >> /etc/mdev/mdev.log
						sync
						echo "$device#$type#$label" > /tmp/.swapdirdev
						swapdir=1
						rm -f /var/swapdir
						ln -sn "/autofs/$device/swapdir" /var/swapdir
						swap.sh "" "$SWTYPE" "$model";;
					*)
						return;;
				esac
			fi;;
	esac

	case $backup in
		"")
			if [ -d "/autofs/$device/backup" ]; then
				case $isreadonly in "") startIsreadonly;; esac
				case $isreadonly in
					0)
						echo "[$0] startNewdev: found backup device=$device label=$label type=$type" >> /etc/mdev/mdev.log
						sync
						echo "$device#$type#$label" > /tmp/.backupdev
						backup=1
						rm -f /var/backup
						ln -sn "/autofs/$device/backup" /var/backup
						case $swap_max_sectors in
							240) ;;
							*)   echo "$swap_max_sectors" > "/sys/block/${device:0:3}/device/max_sectors" 2>/dev/null;;
						esac;;
					*)
						return;;
				esac
			fi;;
	esac

	case $swapextensions in
		"")	
			if [ -d "/autofs/$device/swapextensions" ]; then
				case $isreadonly in "") startIsreadonly;; esac
				case $isreadonly in
					0)
						echo "[$0] startNewdev: found swapextensions device=$device label=$label type=$type" >> /etc/mdev/mdev.log
						sync
						echo "$device#$type#$label" > /tmp/.swapextensionsdev
						swapextensions=1
						rm -f /var/swap
						ln -sn "/autofs/$device/swapextensions" /var/swap
						case $swap_max_sectors in
							240) ;;
							*)   echo "$swap_max_sectors" > "/sys/block/${device:0:3}/device/max_sectors" 2>/dev/null;;
						esac
						startStickextensions /var/swap &;;
					*)
						return;;
				esac
			fi;;
	esac

	# remove all device entrys
	sed -e '/./{H;$!d;}' -e "x;/$device/d;" -i /mnt/config/smb.conf
	sed -e '/./,/^$/!d' -i /mnt/config/smb.conf
	
	sed -e "s!/autofs/$device.*!!g" -i /mnt/config/exports
	sed -e '/^ *$/d' -i /mnt/config/exports

	case $movie in
		"")
			if [ -d "/autofs/$device/movie" ]; then
				case $isreadonly in "") startIsreadonly;; esac
				case $isreadonly in
					0)
						echo "[$0] startNewdev: found movie device=$device label=$label type=$type" >> /etc/mdev/mdev.log
						sync
						echo "$device#$type#$label" > /tmp/.moviedev
						movie=1
						rm -f /media/hdd
						ln -sn "/autofs/$device" /media/hdd

						#sed -e '/./{H;$!d;}' -e "x;/\[media\]/d;" -i /mnt/config/smb.conf

						if [ `cat /mnt/config/smb.conf | grep "$device" | wc -l` -eq 0 ];then
							if [ `cat /mnt/config/titan.cfg | grep "lang=po/de" | wc -l` -eq 1 ];then
								meta=Aufnahme 
							else
								meta=record 
							fi					
							echo "[$0] startNewdev: add dir: /autofs/$device name: $meta to smb.conf"
							echo -e "\n" >> /mnt/config/smb.conf
							echo "[$meta]" >> /mnt/config/smb.conf
							echo "comment = Harddisk /autofs/$device linked to /media/hdd" >> /mnt/config/smb.conf
							echo "path = /autofs/$device" >> /mnt/config/smb.conf
							echo "read only = no" >> /mnt/config/smb.conf
							echo "public = yes" >> /mnt/config/smb.conf
							echo "guest ok = yes" >> /mnt/config/smb.conf
						fi

						sed -e "s!/media .*!!g" -i /mnt/config/exports
						if [ `cat /mnt/config/exports | grep "$device" | wc -l` -eq 0 ];then
							echo "/autofs/$device *(rw,no_root_squash,no_subtree_check,sync)" >> /mnt/config/exports
						fi

						case $model in
							ipbox91|ipbox900|ipbox910|ipbox9000) vfdctl +music;;
							*)   vfdctl +hdd;;
						esac
				
						echo "RECMOUNT: OK" > /dev/vfd
						case $record_max_sectors in
							240) ;;
							*)   echo "$record_max_sectors" > "/sys/block/${device:0:3}/device/max_sectors" 2>/dev/null;;
						esac;;
					*)
						return;;
				esac
			fi;;
	esac

	if [ "${device:0:8}" != "mtdblock" ] && [ `cat /mnt/config/smb.conf | grep "$device" | wc -l` -eq 0 ];then
		meta=`echo $smblabel | tr "/" "\n" | tail -n1` 
		echo "[$0] startNewdev: add dir: /autofs/$device name: $meta to smb.conf"
		echo -e "\n" >> /mnt/config/smb.conf  
		echo "[$meta]" >> /mnt/config/smb.conf  
		echo "comment = Media Files on $label Drive" >> /mnt/config/smb.conf
		echo "path = /autofs/$device" >> /mnt/config/smb.conf
		echo "read only = no" >> /mnt/config/smb.conf
		echo "public = yes" >> /mnt/config/smb.conf
		echo "guest ok = yes" >> /mnt/config/smb.conf
	fi

	if [ "${device:0:8}" != "mtdblock" ] && [ `cat /mnt/config/exports | grep "$device" | wc -l` -eq 0 ];then
		echo "/autofs/$device *(rw,no_root_squash,no_subtree_check,sync)" >> /mnt/config/exports
	fi
}

startCheckRecordShare() {
	echo "[$0] startCheckRecordShare: start" >> /etc/mdev/mdev.log
	if [ -e /mnt/network/.recordshare ]; then
		read recordshare < /mnt/network/.recordshare
		tmprecordshare=$recordshare
		
		echo "[$0] startCheckRecordShare: check /autofs/$tmprecordshare/movie"
		if [ ! -z "$tmprecordshare" ] && [ ! -e "/autofs/$tmprecordshare/movie" ];then
			echo "[$0] startCheckRecordShare: create /autofs/$tmprecordshare/movie" >> /etc/mdev/mdev.log
			sync
			timeout -t 30 -s 9 mkdir "/autofs/$tmprecordshare/movie"
		fi
		if [ -e "/autofs/$tmprecordshare/movie" ];then
			echo "[$0] startCheckRecordShare: found /autofs/$tmprecordshare/movie"
		fi
	fi

	sharelist=`awk '/(cifs,|nfs,|ftpfs,)/ { print $1 }' /mnt/network/auto.misc`

	fastgrep "$sharelist" "$recordshare"	
	if [ ! -z "$recordshare" ] && [ ! -z "$fgr" ]; then
		# used in NetworkBrowser hdd_replacement True this has prio
		device="$recordshare"; startIsreadonly
		case $isreadonly in
		0)
			echo "[$0] startCheckRecordShare: found NET movie device=$recordshare" >> /etc/mdev/mdev.log
			sync
			echo "$recordshare" > /tmp/.moviedev
			movie=1
			rm -f /media/hdd
			echo "[$0] hRecordShare: linking /media/hdd > /autofs/$recordshare" >> /etc/mdev/mdev.log
			sync
			ln -sn "/autofs/$recordshare" /media/hdd
			echo "RECMOUNT: OK" > /dev/vfd;;
		*)
			recordshare="";;
			esac
	fi

	for share in $sharelist; do
		if [ "$tmprecordshare" == "$share" ] && [ -z "$recordshare" ]; then
			continue
		fi

		echo "[$0] startCheckRecordShare: linking /media/net/$share > /autofs/$share" >> /etc/mdev/mdev.log
		sync
		if [ ! -e /media/net ];then
			mkdir /media/net
		fi
		rm -f "/media/net/$share"
		ln -sn "/autofs/$share" "/media/net/$share"
	done
	echo "[$0] startCheckRecordShare: ende" >> /etc/mdev/mdev.log
	sync
}

startCheckhanging()
{
	echo "[$0] startCheckhanging: start" >> /etc/mdev/mdev.log
	sync
	
	sleep 60

	kill -9 "$1" 2>&1 >/dev/null
	if [ $? -eq 0 ]; then
		echo "[$0] startCheckhanging: timeout" >> /etc/mdev/mdev.log
		sync
		> /tmp/.hotplugready
	fi
	echo "[$0] startCheckhanging: ende" >> /etc/mdev/mdev.log
	sync
}

case $startmode in
	first)
		startCheckhanging $$ &

		if [ -e /tmp/.swapdirdev ] || [ -e /tmp/.swappartdev ]; then swapdir=1; fi
		if [ -e /tmp/.backupdev ]; then backup=1; fi
		if [ -e /tmp/.swapextensionsdev ]; then swapextensions=1; fi
		if [ -e /tmp/.moviedev ]; then movie=1; fi

		ln -s /autofs/sr0 /media/dvd

		# manche HDD werden nicht erkannt ohne diesen ls
		timeout -t 2 -s 9 ls /autofs > /dev/null
		ls /media/net > /dev/null
		ls /media/usb > /dev/null

		# move nas als recmount
		startCheckRecordShare

		sdlist=`awk '/(sd[a-z][0-9])/ { print substr($0,14,4) }' /proc/diskstats`
		naslist=`awk '/(nfs)|(cifs)/ { print substr($2,19) }' /proc/mounts`
		devlist="$sdlist $naslist"

		if [ "$devlist" != " " ]; then
			blkid=`blkid -w /dev/null -c /dev/null`

			for device in $devlist; do
				startNewdev
			done

#			if [ ! -z "$recordshare" ]; then
			for nasshare in $naslist; do
				if [ ! -z "$nasshare" ]; then
#					das backup auf recordshare macht keinen sinn, da auch die .recordshare dort liegt
# 				edit obi add nasshars this is better as recordshare, we can used backup/swapdir on nas
					case $backup in
						"")
							if [ -e "/autofs/$nasshare/backup" ]; then
								echo "[$0] first: found NET backup device=$nasshare"
								echo "$nasshare" > /tmp/.backupdev
								backup=1
								rm -f /var/backup
								ln -sn "/autofs/$nasshare/backup" /var/backup
							fi;;
					esac
					if [ -e "/autofs/$nasshare/swapextensions" ]; then
						echo "[$0] first: found NET swapextensions device=$nasshare"
						echo "$nasshare" > /tmp/.swapextensionsdev
						rm -rf /var/swap
						ln -sn "/autofs/$nasshare/swapextensions" /var/swap
					fi
	#				swapfile on network only running with swapon on loop device
					case $swapdir in
						"")
							if [ -e "/autofs/$nasshare/swapdir" ]; then
								echo "[$0] first: found NET swapdir device=$nasshare"
								echo "$nasshare" > /tmp/.swapdirdev
								swapdir=1
								rm -f /var/swapdir
								ln -sn "/autofs/$nasshare/swapdir" /var/swapdir
							fi;;
					esac
				fi
			done

			> /tmp/.hotplugready
			case $loadpartition in
				y)
					for dev in sda sdb sdc sdd; do
						hdparm -z "/dev/$dev" > /dev/null 2>&1
					done;;
			esac
		else
			> /tmp/.hotplugready
		fi

		exit;;

	checkRecordShare)
		startCheckRecordShare
		exit;;
	startStickextensionsSWAP)
		startStickextensions /var/swap
		exit;;
	startStickextensionsMNT)
		startStickextensions /mnt/swapextensions
		exit;;	
	delRecord)
		rm -f /media/hdd
		rm -f /tmp/.moviedev
		exit;;
	delSwap)
		rm -f /tmp/.swapdirdev
		swap.sh "swapfileoff" "$SWTYPE" "$model"
		rm -f /var/swapdir
		swap.sh "" "$SWTYPE" "$model"
		exit;;
	delSwapextensions)
		rm -f /tmp/.swapextensionsdev
		rm -f /var/swap
		exit;;
	delBackup)
		rm -f /var/backup
		rm -f /tmp/.backupdev
		exit;;
esac

case $ACTION in
	add|"")
		ACTION="add"
		device=${MDEV}
		echo "[$0] add: ACTION=add device=$device" > /etc/mdev/mdev.log
		sync

		if [ `mount | grep ^"tmpfs on /var/volatile" | wc -l` -eq 0 ];then
			echo "[$0] skiped >> found no tmpfs" >> /etc/mdev/mdev.log
			sync
			exit
			return
		fi
	
		if [ ${#device} -ne 4 ]; then exit; fi
		startWaitfor "/tmp/.hotplugready" 1000; rm -f /tmp/.hotplugready

		ls -1 /media/usb/*-"$device" > /dev/null 2>&1

		if [ $? -eq 0 ]; then
			> /tmp/.hotplugready
			exit
		fi

		blkid=`blkid -w /dev/null -c /dev/null "/dev/$device"`
		if [ -e /tmp/.swapdirdev ] || [ -e /tmp/.swappartdev ]; then swapdir=1; fi
		if [ -e /tmp/.backupdev ]; then backup=1; fi
		if [ -e /tmp/.swapextensionsdev ]; then swapextensions=1; fi
		if [ -e /tmp/.moviedev ]; then movie=1; fi
		startNewdev
		> /tmp/.hotplugready
		exit;;
	remove)
		device=${MDEV}
		if [ -e "/sys$DEVPATH" ]; then exit; fi
		if [ ${#device} -ne 4 ]; then exit; fi

		startWaitfor "/tmp/.hotplugready" 1000; rm -f /tmp/.hotplugready
		echo "[$0] remove: ACTION=remove device=$device" > /etc/mdev/mdev.log
		sync
	
		if [ -e /tmp/.swappartdev ]; then
			tmpdev=""; read tmpdev < /tmp/.swappartdev; fastcut "$tmpdev" "#" 1; tmpdev="$fcr"
			case $device in
				$tmpdev)
					rm -f /tmp/.swappartdev
					swap.sh "" "$SWTYPE" "$model"
					> /tmp/.hotplugready
					exit;;
			esac
		fi
	
		rm -f /media/usb/*-"$device" > /dev/null 2>&1

# show only
#		sed -e '/./{H;$!d;}' -e "x;/-$device/!d;" -i /mnt/config/smb.conf
# remove only
		sed -e '/./{H;$!d;}' -e "x;/$device/d;" -i /mnt/config/smb.conf
		sed -e "s!/autofs/$device.*!!g" -i /mnt/config/exports

		if [ `cat /mnt/config/smb.conf | grep "autofs" | wc -l` -eq 0 ];then
			echo "[$0] startNewdev: add mountdir $mountdir to smb.conf"
			meta=`echo media | tr "/" "\n" | tail -n1` 
			echo -e "\n" >> /mnt/config/smb.conf
			echo "[$meta]" >> /mnt/config/smb.conf
			echo "comment = Media Dir" >> /mnt/config/smb.conf
			echo "path = /media" >> /mnt/config/smb.conf
			echo "read only = no" >> /mnt/config/smb.conf
			echo "public = yes" >> /mnt/config/smb.conf
			echo "guest ok = yes" >> /mnt/config/smb.conf
		fi

		if [ `cat /mnt/config/exports | grep "autofs" | wc -l` -eq 0 ];then
			echo "/media *(rw,no_root_squash,no_subtree_check,sync)" >> /mnt/config/exports
		fi

		if [ -e /tmp/.swapdirdev ]; then
			tmpdev=""; read tmpdev < /tmp/.swapdirdev; fastcut "$tmpdev" "#" 1; tmpdev="$fcr"
			case $device in
				$tmpdev)
					rm -f /tmp/.swapdirdev
					swap.sh "swapfileoff" "$SWTYPE" "$model"
					rm -f /var/swapdir
					swap.sh "" "$SWTYPE" "$model";;
			esac
		fi
		
		if [ -e /tmp/.swapextensionsdev ]; then
			tmpdev=""; read tmpdev < /tmp/.swapextensionsdev; fastcut "$tmpdev" "#" 1; tmpdev="$fcr"
			case $device in
				$tmpdev)
					rm -f /var/swap
					rm -f /tmp/.swapextensionsdev;;
			esac
		fi
		
		if [ -e /tmp/.backupdev ]; then
			tmpdev=""; read tmpdev < /tmp/.backupdev; fastcut "$tmpdev" "#" 1; tmpdev="$fcr"
			case $device in
				$tmpdev)
					rm -f /var/backup
					rm -f /tmp/.backupdev;;
			esac
		fi
		
		if [ -e /tmp/.moviedev ]; then
			tmpdev=""; read tmpdev < /tmp/.moviedev; fastcut "$tmpdev" "#" 1; tmpdev="$fcr"
			case $device in
				$tmpdev)
					rm -f /media/hdd
					rm -f /tmp/.moviedev
					
				case $model in
					ipbox91|ipbox900|ipbox910|ipbox9000) vfdctl -music;;
					*)   vfdctl -hdd;;
				esac;;
			esac
		fi
		
		> /tmp/.hotplugready
		exit;;
esac
