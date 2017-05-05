#!/bin/sh
# backup by gost

echo "[backup.sh] startet"
source=$1
noinfo=$2

rm /tmp/.backup_ok
touch /tmp/.backup_start

echo "SICHERUNG nicht implementiert"
if [ $noinfo != "1" ]; then
	echo "infobox1"
	#infobox 30 "SICHERUNG" "$name Sicherung gestartet" & 
	infobox 5 "SICHERUNG" "nicht implementiert" & 
fi
sleep 5

#mkdir /tmp/data
#mount /dev/mmcblk0p3 /tmp/data

#umount /tmp/data

echo "[backup.sh] sync"
sync

touch /tmp/.backup_ok
rm /tmp/.backup_start

echo "[backup.sh] done"


