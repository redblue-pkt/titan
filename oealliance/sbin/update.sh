#!/bin/sh
# update by obi

#echo "[update.sh] $*"

startconfig=/mnt/config/start-config
if [ ! -e "$startconfig" ]; then startconfig="/etc/titan.restore/mnt/config/start-config"; fi

. $startconfig
cd /tmp
boxtype=`cat /etc/model`
buildgroup=`cat /etc/.buildgroup`
arch=`cat /etc/.arch`
event=`cat /mnt/config/titan.cfg | grep rcdev= | tr '/' '\n' | tail -n1`
swtype="titan"
board=`cat /etc/.board`
bootversion=`cat /etc/version-svn`
echo "[update.sh] booted version $bootversion"

source=$1			# getfilelist|tmp|online
target=$2			# kernel|var|root|full|backup or label or device or empty
file=$3				# file to flash/update or empty
user=$4
pass=$5
imgtype=$6		# release|dev
url=$7
targetfull=$target

vfd=vfd

if [ "$source" = "getfilelist" ];then
	user=$2
	pass=$3
	imgtype=$4
	url=$5
fi
### got parameters for online ###
#url=`cat /var/etc/ipkg/official-feed.conf | cut -d '/' -f4 | head -n1`
#urldir=`cat /var/etc/ipkg/official-feed.conf | cut -d '/' -f5 | head -n1`
urldir="svn"

url="http://$user:$pass@$url"
folder="image"

if [ "$imgtype" == "dev" ]; then
	folder="image-beta"
fi

tversion=`cat /etc/version-svn | tr '_' '\n' | tail -n1`

getboxbranding()
{
	cat /etc/image-version | sed -nr "s/.*$1=([^*]+)*/\1/p"
}

if [ -e /etc/.oebuild ];then
	boxtype=$(getboxbranding box_type)
fi

### get online filelist if source=getfilelist ###
if [ "$source" == "getfilelist" ]; then
	umount -fl /tmp/mnt > /dev/null 2>&1
	umount -fl /tmp/mnt > /dev/null 2>&1
	cd /tmp
	mkdir -p online
	rm -f online/*
	rm -f list."$boxtype"

	if [ -e /etc/.homecastpro-sat ];then boxtype="homecastpro-sat"; fi
	if [ -e /etc/.homecastpro-cable ];then boxtype="homecastpro-cable"; fi

	if [ -e /etc/.beta ] && [ "$imgtype" == "release" ]; then
		wget -T15 "$url/$urldir/$folder/list.auto.$boxtype" -O /tmp/list.$boxtype
	elif [ -e /etc/.beta ] || [ "$imgtype" == "dev" ]; then
		wget -T15 "$url/$urldir/$folder/list.$boxtype" -O /tmp/list.$boxtype
	else
# disable ab v1.78
#		wget -T15 "$url/$urldir/$folder/list.$boxtype.$tversion" -O /tmp/list.$boxtype
		wget -T15 "$url/$urldir/$folder/list.auto.$boxtype" -O /tmp/list.$boxtype
	fi

	sync
	if [ -e "/etc/.stable" ] && [ ! -e /etc/.beta ] && [ "$imgtype" == "release" ] && ([ "$boxtype" == "atemio6000" ] || [ "$boxtype" == "atemio6100" ] || [ "$boxtype" == "atemio6200" ]); then
		filelist=`cat /tmp/list."$boxtype" | grep "_stable"`
	elif [ ! -e /etc/.beta ] && [ "$imgtype" == "release" ] && ([ "$boxtype" == "atemio6000" ] || [ "$boxtype" == "atemio6100" ] || [ "$boxtype" == "atemio6200" ]); then
		filelist=`cat /tmp/list."$boxtype" | grep "_nightly"`
	else
		filelist=`cat /tmp/list."$boxtype"`
	fi

	imgversion=`cat /etc/version-svn | cut -d "_" -f2 | sed 's/M//'`
	if [ -z "$imgversion" ]; then
		echo "[update.sh] imgversion not found, use 0"
		imgversion=0;
	elif [ -e /etc/.beta ]; then
		imgversion=0
	elif [ "$imgtype" == "dev" ]; then
		imgversion=0
	else
#		imgversion=`expr $imgversion - 1`
#		imgversion=$imgversion
		imgversion=0
	fi
	if [ -z "$imgversion" ]; then
		echo "[update.sh] imgversion not found, use 0"
		imgversion=0;
	fi

	if [ -z "$updatelist" ]; then updatelist=10; fi
	showfromversion=`expr $imgversion - $updatelist`

	error="1"
	uptodate="1"

	for line in $filelist; do
		error="0"
#		version=`echo "$line" | cut -d "_" -f2 | sed 's/M//'`

		if [ `echo "$line" | grep ^M | wc -l` -eq 1 ];then
			version=`echo "$line" | cut -d "_" -f1 | sed 's/M//'`
		else
			version=${line#*_}
			version=${version%%_*}
			version=${version/M/}
		fi
#		if [ $showfromversion -lt $version ]; then
		if [ $version -gt $imgversion ]; then
			echo "[update.sh] newerfile: $line"
			uptodate="0"
			> "/tmp/online/$line";
		fi
	done
	sync
	### don't change this echo message, is used in panel ###
	if [ "$error" == "1" ];then
		rm -f online/*
		rm -f list."$boxtype"
		echo "[update.sh] error"
	elif [ "$uptodate" == "1" ];then
		echo "[update.sh] uptodate"
	fi

	exit 0
fi

### check input ###
if [ -z "$source" ] || [ -z "$target" ] || [ -z "$file" ]; then
	usage="1"
fi
if [ "$source" != "online" ] && [ "$source" != "tmp" ]; then
	usage="1"
fi

### print usage ###
if [ "$usage" == "1" ]; then
#	echo "GetOnlineFilelist		:  update.sh  getfilelist"
#	echo "FlashOnlineUpdate		:  update.sh  online kernel|var|root|full filename"
#	echo "FlashTmpUpdate			:  update.sh  tmp kernel|var|root|full filename"
#	echo "UsbOnlineUpdate			:  update.sh  online label|dev filename"
#	echo "UsbTmpUpdate				:  update.sh  tmp label|dev filename"
 	exit 1
fi

### got update to ###
if [ "$target" == "kernel" ] || [ "$target" == "var" ] || [ "$target" == "root" ] || [ "$target" = "updatenfi" ] || [ "$target" = "updateusb" ] || [ "$target" == "full" ] || [ "$target" == "fullbackup" ]; then
	update_to="flash"
	target=`echo "$target" | tr "a-z" "A-Z"`
else
	update_to="usb"
fi

### make filenames ###
tmp="/tmp"

### other update handling for some boxes if full image is to big for /tmp (flash) ###
if [ "$target" == "FULL" ] && [ "$boxtype" == "ufs912" ]; then target="KERNEL VAR ROOT"; fi
if [ "$target" == "FULL" ] && [ "$boxtype" == "ufs913" ]; then target="KERNEL VAR ROOT"; fi
if [ "$target" == "FULL" ] && [ "$boxtype" == "atemio510" ]; then target="KERNEL VAR ROOT"; fi
if [ "$target" == "FULL" ] && [ "$boxtype" == "atemio7600" ]; then target="KERNEL VAR ROOT"; fi
if [ "$target" == "FULL" ] && [ "$boxtype" == "atevio700" ]; then target="KERNEL VAR ROOT"; fi
if [ "$target" == "FULL" ] && [ "$boxtype" == "atevio7000" ]; then target="KERNEL VAR ROOT"; fi
if [ "$target" == "FULL" ] && [ "$boxtype" == "atemio520" ]; then target="KERNEL VAR ROOT"; fi
if [ "$target" == "FULL" ] && [ "$boxtype" == "atemio530" ]; then target="KERNEL VAR ROOT"; fi
if [ "$target" == "FULL" ] && [ "$boxtype" == "ufs910" ]; then target="KERNEL VAR ROOT"; fi
if [ "$target" == "FULL" ] && [ "$boxtype" == "ufs922" ]; then target="KERNEL VAR ROOT"; fi
if [ "$target" == "FULL" ] && [ "$boxtype" == "ipbox91" ]; then target="KERNEL VAR ROOT"; fi
if [ "$target" == "FULL" ] && [ "$boxtype" == "ipbox900" ]; then target="KERNEL VAR ROOT"; fi
if [ "$target" == "FULL" ] && [ "$boxtype" == "ipbox910" ]; then target="KERNEL VAR ROOT"; fi
if [ "$target" == "FULL" ] && [ "$boxtype" == "ipbox9000" ]; then target="KERNEL VAR ROOT"; fi
if [ "$target" == "FULL" ] && [ "$boxtype" == "spark" ]; then target="KERNEL ROOT"; fi
if [ "$target" == "FULL" ] && [ "$boxtype" == "spark7162" ]; then target="KERNEL ROOT"; fi
if [ "$target" == "FULL" ] && [ "$board" == "inihdp" ]; then target="KERNEL ROOT"; fi
if [ "$target" == "FULL" ] && [ "$board" == "inihde" ]; then target="KERNEL ROOT"; fi
if [ "$target" == "FULL" ] && [ "$board" == "inihde2" ]; then target="KERNEL ROOT"; fi
if [ "$target" == "FULL" ] && [ "$board" == "inihde2am" ]; then target="KERNEL ROOT"; fi
if [ "$target" == "FULL" ] && [ "$board" == "vusolo2" ]; then target="KERNEL ROOT"; fi
if [ "$target" == "UPDATENFI" ] && [ "$board" == "dm7020hd" ]; then target="UPDATENFI"; fi
if [ "$target" == "UPDATEUSB" ] && [ "$board" == "dm520" ]; then target="UPDATEUSB"; fi
if [ "$target" == "UPDATEUSB" ] && [ "$board" == "dm900" ]; then target="UPDATEUSB"; fi
if [ "$target" == "UPDATEUSB" ] && [ "$board" == "dm920" ]; then target="UPDATEUSB"; fi
if [ "$target" == "UPDATEUSB" ] && [ "$board" == "wetek" ]; then target="UPDATEUSB"; fi
if [ "$target" == "UPDATEUSB" ] && [ "$board" == "hd51" ]; then target="UPDATEUSB"; fi
if [ "$target" == "FULLBACKUP" ]; then target="FULL"; fi
if [ -e /etc/.oebuild ];then
	if [ "$target" == "UPDATEUSB" ]; then target="UPDATEUSB"; fi
fi

### other update handling for some boxes if full image is to big for /tmp (usb) ###
#if [ "USBFULL" == `basename "$file" | cut -d "_" -f9` ]; then bigimage=1; fi

### flash is nand ###
if [ "$boxtype" == "ufs912" ] || [ "$boxtype" == "spark" ] || [ "$boxtype" == "spark7162" ] || [ "$arch" = "mipsel" ] || [ "$arch" = "arm" ];then
	nandbox="1"
fi

### functions ###

killproc_oe()
{
	killall nmbd
	killall smbd
	killall rpc.mountd
	killall rpc.statd
	/etc/init.d/softcam stop
	killall CCcam
	pkill -9 -f '[Oo][Ss][Cc][Aa][Mm]'
	ps w | grep -i oscam | grep -v grep | awk '{print $1}' | xargs kill -9
	pkill -9 -f '[Ww][Ii][Cc][Aa][Rr][Dd][Dd]'
	ps w | grep -i wicardd | grep -v grep | awk '{print $1}' | xargs kill -9
	killall kodi.bin
	killall hddtemp
	killall transmission-daemon
	killall openvpn
	/etc/init.d/sabnzbd stop
	pkill -9 -f cihelper
	pkill -9 -f ciplus_helper
	pkill -9 -f ciplushelper
	# kill VMC
	pkill -f vmc.sh
	pkill -f DBServer.py
	# stop autofs
	/etc/init.d/autofs stop

	# sync filesystem
	echo "Syncing filesystem"
	sync 
	sleep 1

	init 2
}

umount_rootfs()
{
	multilib=0
	if [ -e /lib64 ];then multilib=1; fi
	echo multilib=$multilib

	echo "start umount_rootfs"
	# the start script creates /newroot dir and mount tmpfs on it
	# create directories

#	rm -rf /newroot
	mkdir /newroot
	cd /newroot
	mkdir -p /newroot/{bin,dev/pts,etc,lib,media,oldroot,oldroot_bind,proc,run,sbin,sys,usr/lib/autofs,var,var/volatile}

	if [ "$multilib" == 1 ];then
		mkdir -p /newroot/{lib64,usr/lib64/autofs}
	fi

	# we need init and libs to be able to exec init u later
	if [ "$multilib" == 1 ];then
		cp -arf /bin/busybox*     /newroot/bin
		cp -arf /bin/sh*          /newroot/bin
		cp -arf /bin/bash*        /newroot/bin
		cp -arf /sbin/init*       /newroot/sbin
		cp -arf /lib64/libc*        /newroot/lib64
		cp -arf /lib64/ld*          /newroot/lib64
		cp -arf /lib64/libtinfo*    /newroot/lib64
		cp -arf /lib64/libdl*       /newroot/lib64
	else
		cp -arf /bin/busybox*     /newroot/bin
		cp -arf /bin/sh*          /newroot/bin
		cp -arf /bin/bash*        /newroot/bin
		cp -arf /sbin/init*       /newroot/sbin
		cp -arf /lib/libc*        /newroot/lib
		cp -arf /lib/ld*          /newroot/lib
		cp -arf /lib/libtinfo*    /newroot/lib
		cp -arf /lib/libdl*       /newroot/lib
	fi

	# libcrypt is moved from /lib to /usr/libX in new OE versions
	if [ "$multilib" == 1 ];then
		cp -arf /lib64/libcrypt*    /newroot/lib64
		cp -arf /usr/lib64/libcrypt*    /newroot/usr/lib64
	else
		cp -arf /lib/libcrypt*    /newroot/lib
		cp -arf /usr/lib/libcrypt*    /newroot/usr/lib
	fi

	# copy for automount ignore errors as autofs is maybe not installed
	if [ "$multilib" == 1 ];then
		cp -arf /usr/sbin/autom*  /newroot/bin
		cp -arf /etc/auto*        /newroot/etc
		cp -arf /lib64/libpthread*  /newroot/lib64
		cp -arf /lib64/libnss*      /newroot/lib64
		cp -arf /lib64/libnsl*      /newroot/lib64
		cp -arf /lib64/libresolv*   /newroot/lib64
		cp -arf /usr/lib64/libtirp* /newroot/usr/lib64
		cp -arf /usr/lib64/autofs/* /newroot/usr/lib64/autofs
		cp -arf /etc/nsswitch*    /newroot/etc
		cp -arf /etc/resolv*      /newroot/etc
	else
		cp -arf /usr/sbin/autom*  /newroot/bin
		cp -arf /etc/auto*        /newroot/etc
		cp -arf /lib/libpthread*  /newroot/lib
		cp -arf /lib/libnss*      /newroot/lib
		cp -arf /lib/libnsl*      /newroot/lib
		cp -arf /lib/libresolv*   /newroot/lib
		cp -arf /usr/lib/libtirp* /newroot/usr/lib
		cp -arf /usr/lib/autofs/* /newroot/usr/lib/autofs
		cp -arf /etc/nsswitch*    /newroot/etc
		cp -arf /etc/resolv*      /newroot/etc
	fi

	echo "Switching to user mode 2"
	init 2
	killall -9 start.sh
	killall -9 titan
	sleep 2

	pivot_root /newroot oldroot

	cd /
	# move mounts to new root
	mount --move /oldroot/dev/ dev/
	mount --move /oldroot/proc/ proc/
	mount --move /oldroot/sys/ sys/
	mount --move /oldroot/var/volatile var/volatile/

	# create link for tmp
	ln -s /var/volatile/tmp /tmp

	mount --move /oldroot/media/ media/

	# /media is no tmpfs -> move every mount
	echo "/media is not tmpfs"

	# create link for mount/umount for autofs
	ln -s /bin/busybox /bin/mount
	ln -s /bin/busybox /bin/umount

	exec init u
	sleep 2

	// kill all remaining open processes which prevent umounting rootfs
	fuser -k -m /oldroot/
	sleep 3

	umount /oldroot/newroot
	umount /oldroot

	ret=$?		
	if [ $ret -eq 0 ]; then
		echo "umount successful"
	else
		echo "umount not successful"
	fi
}

flash_img()
{
	if [ "$targetfull" = "fullbackup" ];then
		file="$origfile"
	fi

	while true; do
		if [ "$arch" = "mipsel" ] || [ "$arch" = "arm" ];then
			echo skip flash_erase
#			echo /tmp/flash_erase /tmp/"$mtd" 0 0
#			/tmp/flash_erase /tmp/"$mtd" 0 0
		else
			/tmp/flash_eraseall -l "Erase: $part" -x "$boxtype" /tmp/"$mtd"
		fi
		sleep 1

		echo "[update.sh] boxtype=$boxtype part=$part mtd=$mtd file=$file"
		if [ "$arch" = "mipsel" ] || [ "$arch" = "arm" ];then
			if [ "$boxtype" = "atemio6000" ] || [ "$boxtype" = "atemio6100" ];then
				echo "EAas" > /tmp/$vfd
			else
				echo "Write: $part" > /tmp/$vfd
			fi

			if [ "$part" = "UPDATEUSB" ];then
				if [ -e /etc/.oebuild ];then
					imagefs=$(getboxbranding imagefs)
					imagedir=$(getboxbranding imagedir)
					mtdrootfs=$(getboxbranding mtdrootfs)
					mtdkernel=$(getboxbranding mtdkernel)
					tmp=/tmp/ramfs
					mkdir $tmp
					mount -t ramfs ramfs $tmp				

					FLASHTIME=`date "+%Y%m%d"`
					BACKUPFILE=mnt_${FLASHTIME}
					if [ -e /media/hdd ];then
						BACKUPDIR=/media/hdd/.update
						if [ ! -e $BACKUPDIR ];then mkdir $BACKUPDIR; fi
						if [ -e $BACKUPDIR ];then
							if [ $(ls -1 $BACKUPDIR | wc -l) -gt 4 ];then
								rm -rf $BACKUPDIR/*
							fi
							cp -a /mnt $BACKUPDIR/$BACKUPFILE
							echo "$BACKUPDIR/$BACKUPFILE" > $BACKUPDIR/.last
							rm $BACKUPDIR/.last.restored
							sync
						fi
					elif [ -e /media/usb ];then
						BACKUPDIR=/media/usb/.update
						if [ ! -e $BACKUPDIR ];then mkdir $BACKUPDIR; fi
						if [ -e $BACKUPDIR ];then
							if [ $(ls -1 $BACKUPDIR | wc -l) -gt 4 ];then
								rm -rf $BACKUPDIR/*
							fi
							cp -a /mnt $BACKUPDIR/$BACKUPFILE
							echo "$BACKUPDIR/$BACKUPFILE" > $BACKUPDIR/.last
							rm $BACKUPDIR/.last.restored
							sync
						fi
					fi
					if [ "$imagefs" = "ubinfi" ];then
						showtime=92
						if [ "$board" = "dm7020hd" ];then showtime=92 ;fi
						cp "$file" $tmp/flash.nfi
						rm "$file"
						echo "#!/bin/sh -x" > /tmp/dflash.sh
						echo "init 4" >> /tmp/dflash.sh
						echo "infobox -pos -1 75% 100$showtime 'UPDATENFI' '            Schreibe Daten            ' &" >> /tmp/dflash.sh
						echo "sleep 3" >> /tmp/dflash.sh
						echo "echo 50 > /proc/progress" >> /tmp/dflash.sh
						echo "/usr/sbin/nfiwrite -l -b -r -s -f $tmp/flash.nfi" >> /tmp/dflash.sh
						echo "exit 0" >> /tmp/dflash.sh
						chmod 755 /tmp/dflash.sh
						start-stop-daemon -S -b -n dflash.sh -x /tmp/dflash.sh
						exit
					else
						showtime=33
						if [ "$board" = "hd51" ];then showtime=40 ;fi
						infobox -pos -1 75% 100$showtime "UPDATEUSB" "            Entpacke Image            " &
						time unzip "$file" -x $imagedir/*.img -x $board/*.img -x usb_update.bin -d $tmp
						rm -f "$file"
						sync
						killall infobox
						ofgwrite -r$mtdrootfs -k$mtdkernel $tmp/$imagedir
						exit
					fi
				elif [ "$board" = "dm520" ];then
					infobox -pos -1 75% 10125 "UPDATEUSB" "            Schreibe Daten            " &
					unzip "$file" -d /tmp
					flash-kernel /tmp/kernel.bin
					helproot=/tmp/helproot
					if [ ! -d $helproot ]; then
						mkdir $helproot
					else
						umount -l $helproot
					fi
					mount -t ubifs ubi0:dreambox-rootfs $helproot
					erase_root $helproot
					tar -xvjf /tmp/rootfs.tar.bz2 --exclude=./mnt --exclude=./var/media -C $helproot
				elif [ "$board" = "dm900" ] || [ "$board" = "dm920" ] || [ "$board" = "wetek" ] || [ "$board" = "hd51" ];then
					showtime=33
					if [ "$board" = "hd51" ];then showtime=40 ;fi

					infobox -pos -1 75% 100$showtime "UPDATEUSB" "            Schreibe Daten            " &

					if [ -d /tmp/$board ]; then
						rm -r /tmp/$board
					fi
					time unzip "$file" -x $board/*.img -d /tmp
					rm -f "$file"
					time flash-kernel /tmp/$board/kernel.bin

					sync
# log to /var/swap/logs not working...
#					/etc/init.d/autofs stop
					umount -l /mnt

					helproot=/tmp/helproot
					if [ ! -d $helproot ]; then
						mkdir $helproot
					else
						umount -l $helproot
					fi
					mount /dev/root $helproot

					time erase_root $helproot
#					time tar -xvf /tmp/$board/rootfs.tar --exclude=./mnt --exclude=./var/media -C $helproot
					time tar -xvjf /tmp/$board/rootfs.tar.bz2 --exclude=./mnt --exclude=./var/media -C $helproot

				fi
			elif [ "$part" = "UPDATENFI" ];then
				infobox -pos -1 75% 10065 "UPDATENFI" "            Schreibe Daten            " &
				mkdir /tmp/ramfs
				mount -t ramfs ramfs /tmp/ramfs				
				cp "$file" /tmp/ramfs/flash.nfi
				rm "$file"
				nfiwrite -l -b -r -s -f -v /tmp/ramfs/flash.nfi
			elif [ "$part" = "KERNEL" ];then
				echo /tmp/mv -f "$file" /tmp/kernel.bin
				/tmp/mv -f "$file" /tmp/kernel.bin
				killall infobox
				#infobox -pos -1 75% 10010 "Kernel" "            Schreibe Daten            " &
				if [ "$boxtype" = "vusolo2" ];then
					#/sbin/fuser -k /dev/dvb/adapter0/frontend0
					echo /tmp/ofgwrite --kernel=mtd2 /tmp
					/tmp/ofgwrite --kernel=mtd2 /tmp
				elif [ "$boxtype" = "dm520" ] || [ "$boxtype" = "dm525" ];then
					echo /tmp/ofgwrite -nk /tmp
					/tmp/ofgwrite -nk /tmp
				else
					echo /tmp/ofgwrite -k /tmp
					/tmp/ofgwrite -k /tmp
				fi
			elif [ "$part" = "ROOT" ];then
				if [ "$arch" = "mipsel" ] || [ "$arch" = "arm" ];then
					touch /tmp/.init3
					touch /tmp/.update
				fi
				killall infobox
				echo /tmp/mv -f "$file" /tmp/rootfs.bin
				/tmp/mv -f "$file" /tmp/rootfs.bin
				if [ "$boxtype" = "vusolo2" ];then
					#/sbin/fuser -k /dev/dvb/adapter0/frontend0
					echo /tmp/ofgwrite --rootfs=mtd0 /tmp
					/tmp/ofgwrite --rootfs=mtd0 /tmp
				elif [ "$boxtype" = "dm520" ] || [ "$boxtype" = "dm525" ];then
					echo /tmp/ofgwrite_bin -r /tmp
#					mount -r -o remount /
#					/tmp/ubiformat /dev/mtd0 -f /tmp/rootfs.bin -D

					echo /tmp/ofgwrite -r /tmp
					/tmp/ofgwrite -r /tmp
				else
					echo /tmp/ofgwrite -r /tmp
					/tmp/ofgwrite -r /tmp
				fi
#				mount -r -o remount /
#				/tmp/ubiformat /dev/mtd0 -f /tmp/rootfs.bin -D
			fi
			ret=$?		
		elif [ "$boxtype" = "ufs913" ];then
			if [ "$part" = "KERNEL" ];then
				/tmp/flashcp -v "Write: $part" "$boxtype" "$file" /tmp/"$mtd"
			else
				echo /tmp/nandwrite -p /tmp/"$mtd" "$file"
				/tmp/nandwrite -p /tmp/"$mtd" "$file"
			fi
			ret=$?
		elif [ "$nandbox" = "1" ]; then
			echo /tmp/nandwrite -p /tmp/"$mtd" "$file"
			/tmp/nandwrite -p /tmp/"$mtd" "$file"
			ret=$?
		else
			/tmp/flashcp -v "Write: $part" "$boxtype" "$file" /tmp/"$mtd"
			ret=$?
		fi
		if [ $ret -eq 0 ]; then break; fi
		infobox -pos -1 70% 9999 "Aktualisierung" "ACHTUNG: Aktualisierung fehlgeschlagen" " " "OK = Weiter" "Blau = Aktualisierung wiederholen" &
		if [ "$boxtype" = "atemio520" ] || [ "$boxtype" = "atemio530" ] || [ "$boxtype" = "spark" ] || [ "$boxtype" = "ipbox91" ] || [ "$boxtype" = "atemio6000" ] || [ "$boxtype" = "atemio6100" ]; then
			echo "Err " > /tmp/$vfd
		else
			echo "Fehlgeschlagen ?" > /tmp/$vfd
		fi
		abfrage /tmp/$event; ret=$?
		echo " " > /tmp/$vfd
		killall infobox
		if [ "$part" = "UPDATEUSB" ];then
			echo "sync" > /tmp/$vfd
			sync
			echo " " > /tmp/$vfd
		fi
		if [ $ret -eq 0 ]; then break; fi
	done
}

killproc()
{
	count=0
	lastcount=`ps | tail -n1 | sed "s/ \+/ /g;s/^ //" | cut -d" " -f1`
	if [ "$boxtype" == "ufs910" ] || [ "$boxtype" == "ufs922" ]; then
		excludes="^ini inetd automount evremote2 telnetd update.sh infobox lircd"
	else
		excludes="^ini inetd automount evremote2 telnetd update.sh infobox lircd enigma2.sh dvb_server init_client oscam gbox cccam $swtype"
	fi
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
					echo "[update.sh] killing pid=$count cmd=$cmd"
					kill -9 "$count"
				fi
			fi
		fi
		if [ $count -ge $lastcount ]; then break; fi
	done
}

kill_and_show()
{
	if [ -z "$killandshow" ]; then
		killall infobox
		infobox -pos -1 75% 9999 "Info" "            Laufende Prozesse beenden            " "            Bitte warten            " &
	
		killandshow="1"
		emu.sh stop
		killall -9 rcS
		killproc
		echo "3" > /proc/sys/vm/drop_caches
# 	FIXME: kills update.sh why???
#		/sbin/fuser -k /dev/dvb/adapter0/frontend0
#		/sbin/fuser -k /dev/dvb/adapter0/frontend1
#		echo "0" > /proc/progress

		if [ "$boxtype" == "ufs910" ] || [ "$boxtype" == "ufs922" ]; then
			echo "[workaround] killall -9 $swtype"
			killall -9 $swtype
			infobox 1 "nobox#/var/etc/boot/start.jpg"
		else
			skip1=1
		fi
		killall infobox
	fi
}

error()
{
	killall infobox
	infobox -pos -1 70% 10010 "Aktualisierungsfehler!" "$1" "" "Neustart in 10 Sekunden" &
	echo "[update.sh] $1"
	for i in 9 8 7 6 5 4 3 2 1 0; do
		if [ "$boxtype" = "atemio520" ] || [ "$boxtype" = "atemio530" ] || [ "$boxtype" = "spark" ] || [ "$boxtype" = "ipbox91" ] || [ "$boxtype" = "atemio6000" ] || [ "$boxtype" = "atemio6100" ]; then
			echo "Rb $i" > /dev/$vfd
		else
			echo "$2 ($i)" > /dev/$vfd
		fi
		sleep 1
	done
	if [ "$boxtype" = "atemio520" ] || [ "$boxtype" = "atemio530" ] || [ "$boxtype" = "spark" ] || [ "$boxtype" = "ipbox91" ] || [ "$boxtype" = "atemio6000" ] || [ "$boxtype" = "atemio6100" ]; then
		echo "Ende" > /dev/$vfd
	else
		echo "Neustart" > /dev/$vfd
	fi
	echo "[update.sh] reboot"
#	init 6
	/tmp/reboot -f
	reboot -f
	exit
}

got_mtd_part()
{
	if [ "$part" == "UPDATENFI" ] || [ "$part" == "UPDATEUSB" ];then
		mtd=mtddisabled
	else
		mtd=`cat /proc/mtd | grep -i "$part" | cut -d":" -f1 | tail -n1`
		echo "[update.sh] Part $part: mtd=$mtd"
		### check mtd part ###
		if [ -z "$mtd" ] || [ `echo -e "$mtd" | wc -l` -ne 1 ] || [ "${mtd:0:3}" != "mtd" ] || ([ "$mtd" == "mtd0" ] && [ "$arch" = "sh4" ]); then
			error "Uncorrect MTD Partition: $mtd" "MTD err"
		fi
	fi
}

md5_check()
{
	if [ "$source" == "online" ];then
		md5=`md5sum "$file" | cut -d"/" -f1 | tr -d " "`
		echo "[update.sh] md5=$md5"
		md5file=`cat "$file_md5" | tr -d " "`
		echo "[update.sh] md5file=$md5file"
		if [ "$md5" != "$md5file" ]; then
			error "MD5 check failed" "MD5 err"
		fi
	fi
}

got_online_img()
{
	if [ "$source" == "online" ];then
		killall infobox
		infobox -pos -1 75% 3 Aktualisierung "  Lade Image und MD5  " &
		if [ "$boxtype" = "atemio520" ] || [ "$boxtype" = "atemio530" ] || [ "$boxtype" = "spark" ] || [ "$boxtype" = "ipbox91" ] || [ "$boxtype" = "atemio6000" ] || [ "$boxtype" = "atemio6100" ]; then
			echo "Lade" > /tmp/$vfd
		else
			echo "Lade Image" > /tmp/$vfd
		fi
		cd "$tmp"
		rm -f "$base"
		rm -f "$base_md5"
		sleep 3
		killall infobox

		wget.info -T15 "$url/$urldir/$folder/$base"
		wget.info -T15 "$url/$urldir/$folder/$base_md5"

		killall infobox
		infobox -pos -1 75% 20 Aktualisierung "    Laden abgeschlossen    " &
		if [ "$boxtype" = "atemio520" ] || [ "$boxtype" = "atemio530" ] || [ "$boxtype" = "spark" ] || [ "$boxtype" = "ipbox91" ]; then
			echo "Ok" > /tmp/$vfd
		elif [ "$boxtype" = "atemio6000" ] || [ "$boxtype" = "atemio6100" ];then
			echo "Done" > /tmp/$vfd
		else
			echo "Laden fertig" > /tmp/$vfd
		fi
	fi
}

copy_to_tmp()
{
	if [ ! -e "$tmp/$base" ]; then
		killall infobox
		infobox -pos -1 75% 100 Aktualisierung "  Kopiere Image nach $tmp  " &
		if [ "$boxtype" = "atemio520" ] || [ "$boxtype" = "atemio530" ] || [ "$boxtype" = "spark" ] || [ "$boxtype" = "ipbox91" ]; then
			echo "Kopi" > /tmp/$vfd
		elif [ "$boxtype" = "atemio6000" ] || [ "$boxtype" = "atemio6100" ];then
			echo "Copy" > /tmp/$vfd
		else
			echo "Kopiere" > /tmp/$vfd
		fi
		cp "$file" "$tmp/$base"
	fi
	if [ ! -e "$tmp/$base_md5" ]; then
		cp "$file_md5" "$tmp/$base_md5"
	fi
	file="$tmp/$base"
	file_md5="$tmp/$base_md5"
}

file_check()
{
	if [ ! -e "$file" ]; then
		error "Missing: File" "no file"
	fi
	if [ ! -e "$file_md5" ] && [ "$source" == "online" ]; then
		error "Missing: MD5 File" "no MD5"
	fi
}

got_basename()
{
	if [ "$targetfull" != "fullbackup" ];then
		file=`echo "$origfile" | sed "s/_FULL_/_${part}_/"`
	fi
#	file_md5="${file%.*}"; file_md5="${file_md5%.*}"; file_md5="$file_md5.md5"
	file_md5="${file%.*}".md5;
#	file_md5=`echo $file | sed 's/.img/.md5/' | sed 's/.tar.gz/.md5/'`
	base=`basename "$file"`
	base_md5=`basename "$file_md5"`
}

make_tmp()
{
	mkdir /tmp/mktmp
	mkdir /tmp/lib
	cp /bin/busybox /tmp
	cp /bin/infobox /tmp
	cp /bin/flashcp /tmp
	if [ "$arch" == "mipsel" ] || [ "$arch" = "arm" ];then
		cp /sbin/wget /tmp
	fi
	if [ -e /bin/cubefpctl ]; then cp /bin/cubefpctl /tmp; fi
	cp /sbin/halt /tmp
	cp /mnt/network/resolv.conf /tmp
	cp /bin/flash_eraseall /tmp
	cp /bin/grep /tmp

#	cp /lib/ld.so.1 /tmp/lib		#for busybox
#	cp -a /lib/ld-linux.so.* /tmp/lib		#for busybox
#	cp -a /lib/libc.so.* /tmp/lib				#for busybox
#	cp -a /lib/libcrypt.so.* /tmp/lib		#for busybox
#	cp -a /lib/libfreetype.so.* /tmp/lib	#for infobox
#	cp -a /lib/libz.so.* /tmp/lib				#for infobox
#	cp -a /lib/libm.so.* /tmp/lib				#for infobox
#	cp -a /lib/libgcc_s.so.* /tmp/lib		#for infobox
#	cp -a /lib/libjpeg.so.* /tmp/lib		#for infobox
#	cp -a /lib/libpng* /tmp/lib		#for infobox	
#	cp -a /lib/libbz2.so.* /tmp/lib		#for infobox

#p217 github duckbox
	cp -a /lib/ld.so.1 /tmp/lib
	cp -a /lib/ld-* /tmp/lib
	cp -a /lib/libc.* /tmp/lib
	cp -a /lib/libc-* /tmp/lib
	cp -a /lib/libcrypt.* /tmp/lib
	cp -a /lib/libcrypt-* /tmp/lib
	cp -a /lib/libfreetype.so.* /tmp/lib
	cp -a /lib/libz-* /tmp/lib
	cp -a /lib/libz.* /tmp/lib
	cp -a /lib/libm.* /tmp/lib
	cp -a /lib/libm-* /tmp/lib
	cp -a /lib/libgcc_s* /tmp/lib
	cp -a /lib/libjpeg.so.* /tmp/lib
	cp -a /lib/libpng* /tmp/lib
	cp -a /lib/libbz2.so.* /tmp/lib

	if [ "$arch" == "sh4" ];then
		cp /lib/libcrypto.so.* /tmp/lib		#for wget
		cp /lib/libssl.so.* /tmp/lib		#for wget
	fi

	if [ "$target" = "UPDATEUSB" ]; then
		cp /lib/ld* /tmp/lib		#for busybox
		cp /lib/libc.so.6 /tmp/lib				#for busybox
		cp /lib/libcrypt.so.1 /tmp/lib		#for busybox
		cp /usr/lib/libfreetype.so.6 /tmp/lib	#for infobox
		cp /lib/libz.so.1 /tmp/lib				#for infobox
		cp /lib/libm.so.6 /tmp/lib				#for infobox
		cp /lib/libgcc_s.so.1 /tmp/lib		#for infobox
		cp /usr/lib/libjpeg.so.62 /tmp/lib		#for infobox
		cp /usr/lib/libjpeg.so.8 /tmp/lib		#for infobox
		cp /usr/lib/libpng15.so.15 /tmp/lib		#for infobox
		cp /lib/libbz2* /tmp/lib		#for infobox
		cp /usr/lib/libcrypto* /tmp/lib		#for wget
		cp /lib/libssl* /tmp/lib		#for wget
		cp /usr/share/fonts/default.ttf /tmp	# for infobox

		cp /lib/libresolv.so* /tmp/lib		#for gzip
		cp /lib/librt* /tmp/lib		#for automount
		cp /usr/lib/libcrypt.* /tmp/lib		#for sleep

		cp /lib/libpam.so* /tmp/lib		#for login
		cp /lib/libpam_misc.so* /tmp/lib		#for login

		cp -a /lib/libtinfo.so.* /tmp/lib
		cp /usr/sbin/flash-kernel /tmp
		cp /usr/sbin/librecovery /tmp
		cp /usr/bin/unzip /tmp
		cp /bin/rm /tmp

		cp /lib/libcap.so.* /tmp/lib		#for avahi-daemon
	fi

	cp /usr/share/fonts/default.ttf /tmp	# for infobox
	cp /sbin/sleepms /tmp

	cat /sbin/abfrage | sed "s#/usr/bin/awk#/tmp/awk#" > /tmp/abfrage
	chmod 777 /tmp/abfrage
	cat /sbin/emu.sh | sed "s#/bin/sh#/tmp/sh#" > /tmp/emu.sh
	chmod 777 /tmp/emu.sh
	cat /sbin/wget.info | sed "s#/bin/sh#/tmp/sh#" > /tmp/wget.info
	chmod 777 /tmp/wget.info
	cat /sbin/start_sec.sh | sed "s#/bin/sh#/tmp/sh#" > /tmp/start_sec.sh
	chmod 777 /tmp/start_sec.sh

	if [ "$boxtype" = "ipbox91" ] || [ "$boxtype" = "ipbox910" ] || [ "$boxtype" = "ipbox900" ] || [ "$boxtype" = "ipbox9000" ]; then
		ln -s /tmp/halt /tmp/reboot
	elif [ "$arch" = "mipsel" ] || [ "$arch" = "arm" ];then
		cp /sbin/reboot.sysvinit /tmp/reboot
		cp /usr/bin/ofgwrite /tmp
		cp /usr/sbin/ubiformat /tmp
	else
		ln -s /tmp/busybox /tmp/reboot
	fi
	if [ "$arch" = "sh4" ];then
		ln -s /tmp/busybox /tmp/wget
	fi
	ln -s /tmp/busybox /tmp/killall
	ln -s /tmp/busybox /tmp/sleep
	ln -s /tmp/busybox /tmp/awk
	ln -s /tmp/busybox /tmp/dd
	ln -s /tmp/busybox /tmp/time
	ln -s /tmp/busybox /tmp/sync
	ln -s /tmp/busybox /tmp/mount
	ln -s /tmp/busybox /tmp/umount
	ln -s /tmp/busybox /tmp/mkdir
	ln -s /tmp/busybox /tmp/echo
	ln -s /tmp/busybox /tmp/sed
	ln -s /tmp/busybox /tmp/cut
	ln -s /tmp/busybox /tmp/tail
	ln -s /tmp/busybox /tmp/tr
	ln -s /tmp/busybox /tmp/date
	ln -s /tmp/busybox /tmp/basename
	ln -s /tmp/busybox /tmp/tar
	ln -s /tmp/busybox /tmp/cat
	ln -s /tmp/busybox /tmp/wc
	ln -s /tmp/busybox /tmp/rm
	ln -s /tmp/busybox /tmp/touch
	ln -s /tmp/busybox /tmp/ash
	ln -s /tmp/busybox /tmp/mv
	ln -s /tmp/ash /tmp/sh

	if [ "$arch" = "mipsel" ];then
		ln -s /proc/vfd /tmp/vfd
		cp -aL /dev/mtd0 /tmp/mtd0
		cp -aL /dev/dbox/oled0 /tmp
	elif [ "$arch" = "arm" ];then
		cp -aL /dev/mtd0 /tmp/mtd0
		cp -aL /dev/dbox/oled0 /tmp
		ln -s /tmp/oled0 /tmp/vfd
	else
		cp -aL /dev/vfd /tmp/vfd
	fi
	cp -aL /dev/mtd1 /tmp/mtd1
	cp -aL /dev/mtd2 /tmp/mtd2
	cp -aL /dev/mtd3 /tmp/mtd3
	cp -aL /dev/mtd4 /tmp/mtd4
	cp -aL /dev/mtd5 /tmp/mtd5
	cp -aL /dev/mtd6 /tmp/mtd6
	cp -aL /dev/mtd7 /tmp/mtd7
	cp -aL /dev/mtd8 /tmp/mtd8
	cp -aL /dev/mtd9 /tmp/mtd9
	cp -aL /dev/input/$event /tmp/$event
	cp -aL /dev/mtdblock2 /tmp/mtdblock2
	cp -aL /dev/mtdblock3 /tmp/mtdblock3
	cp -aL /dev/mtdblock4 /tmp/mtdblock4
	cp -aL /dev/mtdblock5 /tmp/mtdblock5
	cp -aL /dev/mtdblock6 /tmp/mtdblock6

	if [ "$nandbox" = "1" ] || [ "$boxtype" = "ufs913" ]; then
		cp /bin/nandwrite /tmp
		cp /lib/libpthread.so.0 /tmp/lib
		cp /lib/libdl.so.2 /tmp/lib
	fi

	if [ "$arch" = "mipsel" ] || [ "$arch" = "arm" ]; then
		echo copy nandwrite flash_erase
		cp /usr/sbin/nandwrite /tmp
		cp /usr/sbin/flash_erase  /tmp
		touch /tmp/.update
	fi

	if [ "$update_to" == "usb" ] || [ "$update_to" == "ba" ]; then
		cat /sbin/tar.info | sed "s#/bin/sh#/tmp/sh#" > /tmp/tar.info
		chmod 777 /tmp/tar.info
		cp /sbin/mke2fs /tmp/mktmp
		cp /sbin/fw_setenv /tmp
		cp /sbin/fw_printenv /tmp
		cp /mnt/network/resolv.conf /tmp
		ln -s /tmp/mktmp/mke2fs /tmp/mktmp/mkfs.ext2
	fi

	PATH="/tmp:$PATH"
	PATH="/tmp/mktmp:$PATH"
	export PATH
	LD_LIBRARY_PATH="/lib;/usr/lib;/tmp/lib"
	export LD_LIBRARY_PATH
}

erase_root()
{
	echo "erase root"
	rm -rf $1/var/bin
	rm -rf $1/var/lib
	rm -rf $1/var/usr
	rm -rf $1/var/etc
	rm -rf $1/usr/bin
	rm -rf $1/usr/lib
	rm -rf $1/usr/sbin
	#rm -rf $1/lib
	rm -rf $1/etc
	rm -rf $1/sbin
	rm -rf $1/boot
	#rm -f $1/ba
	rm -f $1/.swfinfo
	#rm -f $1/.smb
	#rm -f $1/.cache
	rm -rf $1/bin
}

### stop E2 (not TV) ###
killall -stop $swtype

### change tmp if image is to big for /tmp ###
if [ "$bigimage" == "1" ]; then
	tmp=`dirname "$file"`
fi

### show beginning of update ###
infobox -pos -1 75% 20 "Aktualisierung" "  Start: bitte warten  " &
if [ "$boxtype" = "atemio520" ] || [ "$boxtype" = "atemio530" ] || [ "$boxtype" = "spark" ] || [ "$boxtype" = "ipbox91" ] || [ "$boxtype" = "atemio6000" ] || [ "$boxtype" = "atemio6100" ]; then
	echo "Star" > /dev/$vfd
else
	echo "Aktualisierung gestartet" > /dev/$vfd
fi
sleep 3

### save orig filename ###
origfile="$file"
got_basename

### kill unneedet prozess and showiframe ###
if [ "$boxtype" = "ufs910" ] || [ "$boxtype" = "ufs922" ]; then
	kill_and_show
fi

if [ "$update_to" == "flash" ]; then swap.sh swapalloff; fi

### remove tmp, exclude img and md5 ###
targetlist="FULL $target"
for part in $targetlist; do
	got_basename
	skipliste="$skipliste $file" 
done

TEXT="| grep -v"
for ROUND in $skipliste; do
	SKIP="$SKIP $TEXT \"$ROUND\""
done

echo "find $tmp -xdev | "grep -v "^$tmp$" "$SKIP" > /tmp/tmp.cmd
chmod 755 /tmp/tmp.cmd
/tmp/tmp.cmd > /tmp/tmp.list

tlist=`cat /tmp/tmp.list`
rm -f /tmp/tmp.list
for t in $tlist; do
	mountpoint -q "$t"
	if [ $? -ne 0 ]; then
		echo "$t" >> /tmp/tmp.list
	fi
done

rm -rf `cat /tmp/tmp.list`
got_basename
# not working if image directly in /tmp
#rm -rf `find $tmp | grep -v "^$tmp$" | grep -v "$base" | grep -v "$base_md5"`


### copy all needet files to tmp ###
make_tmp

for part in $target; do
	### make filename ###
	got_basename

	### if source=online, got online image and .md5 file ###
	got_online_img

	### copy img and md5 to tmp if not in tmp ###
	if [ "$source" = "online" ];then
		copy_to_tmp
	fi

	### check ###
	killall infobox
	infobox -pos -1 75% 20 Aktualisierung "    Überprüfe Image    " &
	if [ "$boxtype" = "atemio520" ] || [ "$boxtype" = "atemio530" ] || [ "$boxtype" = "spark" ] || [ "$boxtype" = "ipbox91" ] || [ "$boxtype" = "atemio6000" ] || [ "$boxtype" = "atemio6100" ]; then
		echo "Uery" > /tmp/$vfd
	else
		echo "Verify" > /tmp/$vfd
	fi
	sleep 3

	### check if img and md5 is in tmp ###
	file_check

	### check md5 ###
	md5_check

	killall infobox
	infobox -pos -1 75% 20 Aktualisierung "    Überprüfung OK    " &

	if [ "$boxtype" = "atemio520" ] || [ "$boxtype" = "atemio530" ] || [ "$boxtype" = "spark" ] || [ "$boxtype" = "ipbox91" ] || [ "$boxtype" = "atemio6000" ] || [ "$boxtype" = "atemio6100" ]; then
		echo "Done" > /tmp/$vfd
	else
		echo "OK" > /tmp/$vfd
	fi

	sleep 3
done

for part in $target; do
	got_basename
	file="$tmp/$base"

	### kill unneedet prozess and showiframe ###
	echo "[update.sh] check box: $boxtype part: $part"
	if [ ! -e /etc/.oebuild ] && ([ "$arch" = "mipsel" ]  || [ "$arch" = "arm" ]) && ([ "$part" = "ROOT" ] || [ "$part" = "UPDATENFI" ] || [ "$part" = "UPDATEUSB" ]); then
		kill_and_show
	elif [ "$boxtype" = "ufs910" ]; then
		kill_and_show	
	else
		echo "[update.sh] skip kill_and_show"
	fi

	### update to flash ###
	if [ "$update_to" == "flash" ]; then
		killall infobox
		if [ "$boxtype" = "atemio520" ] || [ "$boxtype" = "atemio530" ] || [ "$boxtype" = "spark" ] || [ "$boxtype" = "ipbox91" ] || [ "$boxtype" = "atemio6000" ] || [ "$boxtype" = "atemio6100" ]; then
			echo "Upda" > /tmp/$vfd
		else
			echo "Update !!!" > /tmp/$vfd
		fi

		### got and check mtd part ###
		got_mtd_part

		### flash the image ###
		if [ "$target" == "FULL" ] || [ "$target" == "ROOT" ] || [ "$target" = "UPDATENFI" ] || [ "$target" = "UPDATEUSB" ]; then mount /tmp/lib /lib; fi
		flash_img

		if [ "$targetfull" != "fullbackup" ];then
			rm -f "$file"
		fi
	fi
done

### update to usb ###
if [ "$update_to" == "usb" ]; then
	### got label or device ###
	blkid=`blkid -w /dev/null -c /dev/null`
	if echo "$target" | grep -q sd[a-z][1-9]; then
		label=`echo -e "$blkid" | grep "^/dev/$target:" | grep LABEL= | sed 's/^.*LABEL="//;s/".*$//' | tr -d " " | tr -d "-"`
	else
		label="$target"
		target=`echo -e "$blkid" | grep "LABEL=\"$label\"" | head -n1 | cut -d":" -f1 | cut -d"/" -f3`
	fi

	### sync ###
	echo "[update.sh] sync"
	killall infobox
	infobox -pos -1 75% 20 Aktualisierung "     synchronisieren     " &
	if [ "$boxtype" = "atemio520" ] || [ "$boxtype" = "atemio530" ] || [ "$boxtype" = "spark" ] || [ "$boxtype" = "ipbox91" ]; then
		echo "Sync" > /dev/$vfd
	else
		echo "Sync" > /dev/$vfd
	fi
	sync
	sleep 2

	### umount ###
	echo "[update.sh] umount /dev/$target"
	killall infobox
	infobox -pos -1 75% 20 Aktualisierung "   umount /dev/$target   " &
	if [ "$boxtype" = "atemio520" ] || [ "$boxtype" = "atemio530" ] || [ "$boxtype" = "spark" ] || [ "$boxtype" = "ipbox91" ]; then
		echo "umnt" > /dev/$vfd
	else
		echo "umount" > /dev/$vfd
	fi
	umount -fl "/dev/$target"
	umount -fl "/dev/$target"
	umount -fl "/dev/$target"
	sleep 2

	### format ###
	echo "[update.sh] mkfs.ext2 -L $label -b 4096 /dev/$target"
	killall infobox
	infobox -pos -1 75% 20 Aktualisierung " mkfs.ext2 -L $label -b 4096 /dev/$target " &
	if [ "$boxtype" = "atemio520" ] || [ "$boxtype" = "atemio530" ] || [ "$boxtype" = "spark" ] || [ "$boxtype" = "ipbox91" ]; then
		echo "mkfs" > /dev/$vfd
	else
		echo "mkfs.ext2" > /dev/$vfd
	fi
	mkfs.ext2 -L "$label" -b 4096 -I128 "/dev/$target"
	ret=$?
	sleep 2

	### mount ###
	echo "[update.sh] mount /dev/$target /tmp/mnt"
	killall infobox
	infobox -pos -1 75% 20 Aktualisierung "  mount /dev/$target /tmp/mnt  " &
	if [ "$boxtype" = "atemio520" ] || [ "$boxtype" = "atemio530" ] || [ "$boxtype" = "spark" ] || [ "$boxtype" = "ipbox91" ]; then
		echo "mnt" > /dev/$vfd
	else
		echo "mount" > /dev/$vfd
	fi
	mkdir /tmp/mnt
	mount "/dev/$target" /tmp/mnt
	sleep 2

	### remove if mkfs fail ###
	if [ $ret -ne 0 ]; then
		echo "[update.sh] mkfs fail, use rm -rf /tmp/mnt"
		infobox -pos -1 75% 20 Aktualisierung "  mkfs Fehler, rm -rf /tmp/mnt  " &
		if [ "$boxtype" = "atemio520" ] || [ "$boxtype" = "atemio530" ] || [ "$boxtype" = "spark" ] || [ "$boxtype" = "ipbox91" ]; then
			echo "rem" > /dev/$vfd
		else
			echo "remove" > /dev/$vfd
		fi
		mount /tmp/lib /lib
		rm -rf /tmp/mnt
		sleep 2
	fi

	### copy image ###
	echo "[update.sh] tar -zxvf $file -C /tmp/mnt"
	killall infobox
	tar.info -zxvf "$file" -C /tmp/mnt
	sleep 2

	### sync ###
	echo "[update.sh] sync"
	killall infobox
	infobox -pos -1 75% 20 Aktualisierung "     synchronisieren     " &
	if [ "$boxtype" = "atemio520" ] || [ "$boxtype" = "atemio530" ] || [ "$boxtype" = "spark" ] || [ "$boxtype" = "ipbox91" ]; then
		echo "Sync" > /tmp/$vfd
	else
		echo "Sync" > /tmp/$vfd
	fi
	sync
	sleep 2

	### umount ###
	echo "[update.sh] umount /dev/$target"
	killall infobox
	infobox -pos -1 75% 20 Aktualisierung "   umount /dev/$target   " &
	if [ "$boxtype" = "atemio520" ] || [ "$boxtype" = "atemio530" ] || [ "$boxtype" = "spark" ] || [ "$boxtype" = "ipbox91" ]; then
		echo "umnt" > /tmp/$vfd
	else
		echo "umount" > /tmp/$vfd
	fi
	umount "/dev/$target"
fi

### make all clean and reboot ###
sleep 2
killall infobox
infobox -pos -1 70% 10010 Aktualisierung "Aktualisierung abgeschlossen!" "Neustart in 10 Sekunden" &
for i in 9 8 7 6 5 4 3 2 1 0; do
	if [ "$boxtype" = "atemio520" ] || [ "$boxtype" = "atemio530" ] || [ "$boxtype" = "spark" ] || [ "$boxtype" = "ipbox91" ] || [ "$boxtype" = "atemio6000" ] || [ "$boxtype" = "atemio6100" ]; then
		echo "Re $i" > /dev/$vfd
	else
		echo "Neustart ($i)" > /dev/$vfd
	fi
	sleep 1
done

echo "[update.sh] reboot"

start_sec.sh 2 /tmp/umount -ttmpfs -a -r
start_sec.sh 2 /tmp/umount -tnoproc,noprocfs,nodevfs,nosysfs,nousbfs,nousbdevfs,nodevpts -d -a -r
start_sec.sh 2 /tmp/mount -n -o remount,ro /
if [ "$boxtype" = "ipbox91" ] || [ "$boxtype" = "ipbox910" ] || [ "$boxtype" = "ipbox900" ] || [ "$boxtype" = "ipbox9000" ]; then
	/tmp/cubefpctl --reboot
fi

#echo "update.sh init 6"
#init 6 &
echo "update.sh sleep 10"
sleep 10
echo "update.sh reboot"
/tmp/reboot -f
reboot -f
