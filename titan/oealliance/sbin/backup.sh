#!/bin/sh
# backup by obi

rm /tmp/.backup_ok
touch /tmp/.backup_start

boxtype=`cat /etc/model`
source=$1
noinfo=$2

echo "[backup] source: $source"
name=`echo $source | tr 'a-z' 'A-Z'`
echo "[backup] name: $name"

echo "[backup] noinfo=$noinfo"

if [ -e /usr/local/bin/neutrino ]; then
	swtype="neutrino"
elif [ -e /usr/local/bin/enigma2 ]; then
	swtype="enigma2"
elif [ -e /usr/local/bin/vdr ]; then
	swtype="vdr"
elif [ -e /usr/local/bin/titan ]; then
	swtype="titan"
fi

if [ "$boxtype" = "dm900" ] && [ "$source" != "full" ]; then
	echo "Use backup.sh: <full>"
	rm /tmp/.backup_start
	exit 1
fi

if [ "$source" != "kernel" ] && [ "$source" != "var" ] && [ "$source" != "root" ] && [ "$source" != "full" ]; then
	echo "Use backup.sh: <kernel|var|root|full>"
	rm /tmp/.backup_start
	exit 1
fi

error()
{
	killall infobox
	infobox 10010 "Sicherungsfehler" "$1" "" "Neustart in 10 sekunden" & 
	echo "[backup.sh] $1"
	for i in 9 8 7 6 5 4 3 2 1 0; do
		echo "$2 ($i)" > /dev/vfd
		sleep 1
	done
	#do not start restore if update fail
	echo "Neustart" > /dev/vfd
	echo "[backup.sh] reboot"
#	init 6
	echo "backup.sh reboot"
	rm /tmp/.backup_start
	reboot -f
	exit
}

got_mtd_part()
{	
	mtd=`cat /proc/mtd | grep -i "$source" | cut -d":" -f1`
	echo "[backup.sh] Part $source: mtd=$mtd"
	### check mtd part ###
	if [ -z "$mtd" ] || [ `echo -e "$mtd" | wc -l` -ne 1 ] || [ "${mtd:0:3}" != "mtd" ] || [ "$mtd" == "mtd0" ]; then
		error "Uncorrect MTD Partition: $mtd" "MTD err"
	else
		mtdok=`cat /proc/mtd | grep -i "$source" | cut -d":" -f1 | sed s/mtd//g`
	fi
}
### functions ###

if [ "$boxtype" = "dm900" ]; then

	backupdir="/tmp"
	
	if [ $noinfo != "1" ]; then
		echo "infobox1"
		infobox 60 "SICHERUNG" "Sicherung gestartet... bitte warten" & 
	fi
	rm /tmp/"$boxtype"_full_backup_no_settings.zip
	if [ -e "/tmp/$boxtype" ]; then
		rm -r $boxtype
	fi
	mkdir /tmp/$boxtype
	
	if [ ! -e "/tmp/backuphelp" ]; then
		mkdir /tmp/backuphelp
	else
		umount -l /tmp/backuphelp
	fi
	mount /dev/root /tmp/backuphelp
	
	cd /tmp/backuphelp
	tar -cjf /tmp/$boxtype/rootfs.tar.bz2 ./
	cd /tmp
	umount -l /tmp/backuphelp
	zip -r /tmp/"$boxtype"_full_backup_no_settings.zip ./$boxtype
	rm -r $boxtype
	rm /tmp/.backup_start
	if [ $noinfo != "1" ]; then
		echo "infobox2"
		killall infobox
		infobox GUI#5 "SICHERUNG" "Ihre Sicherung wurde erfolgreich angelegt !" " " "Speicherort: $backupdir/"$boxtype"_full_backup_no_settings.zip" " " "EXIT = Fertig"
		killall infobox
	fi
	exit 0
fi	
		
	
umount -fl /tmp/mnt
rm -rf /tmp/*

if [ $noinfo != "1" ]; then
	echo "infobox1"
	infobox 30 "SICHERUNG" "$name Sicherung gestartet" & 
fi
echo "Sicherung gestartet" > /dev/vfd
sleep 1

backupdir="/tmp"

got_mtd_part

if [ -e "/tmp/.backupdev" ] && [ -e "/var/backup" ];then
	backupdir="/var/backup"
else
	echo "[backup.sh] kein backup device gefunden"
	if [ $noinfo != "1" ]; then
		echo "infobox2"
		infobox 20 "SICHERUNG" "$name Sicherung abgebrochen !" " " "Schliessen Sie Bitte ein Flash/Usb Datenträger mit einem backup ordner an." "Und versuchen Sie es erneut"
	fi
	echo "[backup.sh] done"
	rm /tmp/.backup_start
	exit
fi

checkrw=`readlink /var/backup | sed 's!/backup!!'`
if [ `mount | grep "$checkrw" | grep "(rw," | tail -n1 | wc -l` -eq 1 ];then
	echo "[backup.sh] rw check ok"
else
	echo "[backup.sh] zuwenig platz: filesize ($filesize) freespace ($freespace)"
	if [ $noinfo != "1" ]; then
		echo "infobox3"
		infobox 20 "SICHERUNG" "$name Sicherung abgebrochen !" " " "Flash/Usb Datenträger ist nicht beschreibbar !"
	fi
	echo "[backup.sh] done"
	rm /tmp/.backup_start
	exit
fi

freespace=`getfreespace $backupdir`
mtdblock=`echo $mtd | sed 's!mtd!mtdblock!'`
filesize=`cat /proc/partitions | grep $mtdblock | awk {'print $3'}`

echo freespace $freespace
echo filesize $filesize

if [ "$freespace" -lt "$filesize" ]; then
	echo "[backup.sh] zuwenig platz: filesize ($filesize) freespace ($freespace)"
	if [ $noinfo != "1" ]; then
		echo "infobox4"
		infobox 20 "SICHERUNG" "$name Sicherung abgebrochen !" " " "Zu wenig Freierspeicher auf dem Flash/Usb Datenträger !"
	fi
	echo "[backup.sh] done"
	rm /tmp/.backup_start
	exit
fi

echo "[backup.sh] freespace ok: filesize ($filesize) freespace ($freespace)"

time=`date +"%Y.%m.%d_%H.%M.%S"`
version=`cat /etc/version-svn`

killall infobox
fullname="$time"_"$version"_"$name"BACKUP
if [ $noinfo != "1" ]; then
	echo "infobox5"
	infobox 10020 "SICHERUNG" " " "Von: /dev/mtd$mtdok" "Nach: $backupdir/$fullname.img" " " &
fi
dd if="/dev/mtd$mtdok" of="$backupdir/$fullname.img"

sleep 1

echo "[backup.sh] Create MD5 File $fullname"
/sbin/md5sum "$backupdir/$fullname.img" | cut -d "/" -f1 > "$backupdir/$fullname.md5"

sleep 2
if [ $noinfo != "1" ]; then
	killall infobox
	echo "infobox6"
	infobox 20 "SICHERUNG" "$name Sicherung abgeschlossen" &
fi
echo "Sicherung abgeschlossen" > /dev/vfd
sleep 3

if [ $noinfo != "1" ]; then
	echo "infobox7"
	killall infobox
	infobox 9999 "SICHERUNG" "Ihre $name Sicherung wurde erfolgreich angelegt !" " " "Speicherort: $backupdir/$fullname.img" " " "EXIT = Fertig" &
	echo "Fertig ?" > /dev/vfd
	abfrage /dev/input/event0; ret=$?
	killall infobox
fi

echo " " > /dev/vfd

echo "[backup.sh] sync"
sync

touch /tmp/.backup_ok
rm /tmp/.backup_start

echo "[backup.sh] done"
