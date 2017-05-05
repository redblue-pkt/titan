#!/bin/sh

config=/mnt/config/start-config
if [ ! -e "$config" ]; then cp /etc/titan.restore/mnt/config/start-config /mnt/config/start-config; fi
configorg="/etc/titan.restore/var/etc/autostart/start-config.org"

echo "[sync-start-config.sh] start"

if [ ! -e "$configorg" ]; then 
	echo "[sync-start-config.sh] $configorg not exists"
	exit
fi

last=`tail -c1 "$config"`
if [ ! "$last" == "" ]; then
	#schreibe \n
	echo >> "$config"
fi

oldifs="$IFS"
IFS=$'\n'

#schreibe neue eintraege in start-config
list=`cat $configorg`
for i in $list
do
  search=`echo "$i" | cut -d"=" -f1`
  grep "^$search=" "$config" >/dev/null 2>&1
  if [ $? -ne 0 ]; then
    echo "[sync-start-config.sh] write new start-config entry: $i"
    echo "$i" >> "$config"
  fi
done

#loesche alte eintraege aus start-config
list=`cat $config`
for i in $list
do
  search=`echo "$i" | cut -d"=" -f1`
  grep "^$search=" "$configorg" >/dev/null 2>&1
  if [ $? -ne 0 ]; then
    echo "[sync-start-config.sh] delete old start-config entry: $i"
    sed /^$i$/d -i "$config"
  fi
done

IFS="$oldifs"

#loesche leerzeilen
sed /^$/d -i "$config"
