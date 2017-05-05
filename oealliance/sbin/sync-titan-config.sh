#!/bin/sh

arch=$1
read model < /etc/model

config=/mnt/config/titan.cfg
if [ ! -e "$config" ]; then cp /etc/titan.restore/mnt/config/titan.cfg /mnt/config/titan.cfg; fi
configorg="/etc/titan.restore/var/etc/titan/titan-merge.cfg"

echo "[sync-titan-config.sh] start"

if [ ! -e "$configorg" ]; then 
	echo "[sync-titan-config.sh] $configorg not exists"
	exit
fi

last=`tail -c1 "$config"`
if [ ! "$last" == "" ]; then
	#schreibe \n
	echo >> "$config"
fi

oldifs="$IFS"
IFS=$'\n'

#set default entrys to titan.cfg
list=`cat $configorg`
for i in $list
do
	search=`echo "$i" | cut -d"=" -f1`
	if [ ! -z "$search" ]; then
	    echo "[sync-titan-config.sh] write default titan.cfg entry: $i"
		sed "s!^$search=.*!$i!" -i /mnt/config/titan.cfg
  	fi
done

#schreibe neue eintraege in titan.cfg
list=`cat $configorg`
for i in $list
do
  search=`echo "$i" | cut -d"=" -f1`
  grep "^$search=" "$config" >/dev/null 2>&1
  if [ $? -ne 0 ]; then
    echo "[sync-titan-config.sh] write new titan.cfg entry: $i"
    echo "$i" >> "$config"
  fi
done

#loesche alte eintraege aus titan.cfg
#list=`cat $config`
#for i in $list
#do
#  search=`echo "$i" | cut -d"=" -f1`
#  grep "^$search=" "$configorg" >/dev/null 2>&1
#  if [ $? -ne 0 ]; then
#    echo "[sync-titan-config.sh] delete old titan.cfg entry: $i"
#    sed /^$i$/d -i "$config"
#  fi
#done

IFS="$oldifs"

#loesche leerzeilen
sed /^$/d -i "$config"

sed "s!/var/etc/titan/!/mnt/settings/!" -i /mnt/settings/bouquets.cfg

if [ ! -e /mnt/swapextensions/lib/modules ]; then mkdir -p /mnt/swapextensions/lib/modules; fi
if [ ! -e /mnt/swapextensions/usr/local/share/titan/saver ]; then mkdir -p /mnt/swapextensions/usr/local/share/titan/saver; fi
if [ ! -e /mnt/swapextensions/usr/local/share/titan/picons ]; then mkdir -p /mnt/swapextensions/usr/local/share/titan/picons; fi
if [ ! -e /mnt/swapextensions/usr/local/share/titan/skin ]; then mkdir -p /mnt/swapextensions/usr/local/share/titan/skin; fi
if [ ! -e /mnt/swapextensions/usr/local/share/titan/plugins ]; then mkdir -p /mnt/swapextensions/usr/local/share/titan/plugins; fi
if [ ! -e /mnt/swapextensions/usr/local/share/titan/imdbfolder ]; then mkdir -p /mnt/swapextensions/usr/local/share/titan/imdbfolder; fi

if [ -e /var/swap/player/playlist ]; then mv -f /var/swap/player/playlist /mnt/config/playlist; fi
if [ -e /var/swap/player/weather ]; then mv -f /var/swap/player/weather /mnt/config/weather; fi
if [ -e /var/swap/player/browserfav ]; then mv -f /var/swap/player/browserfav /mnt/config/browserfav; fi
if [ -e /var/swap/player/hbbtvfav ]; then mv -f /var/swap/player/hbbtvfav /mnt/config/hbbtvfav; fi
if [ -e /var/swap/player/searchhistory ]; then mv -f /var/swap/player/searchhistory /mnt/config/searchhistory; fi
if [ -e /var/swap/player/favorite ]; then mv -f /var/swap/player/favorite /mnt/config/favorite; fi
if [ -e /var/swap/player/playlist ]; then mv -f /var/swap/player/playlist /mnt/config/playlist; fi

if [ -e /mnt/swapextensions/player/playlist ]; then mv -f /mnt/swapextensions/player/playlist /mnt/config/playlist; fi
if [ -e /mnt/swapextensions/player/weather ]; then mv -f /mnt/swapextensions/player/weather /mnt/config/weather; fi
if [ -e /mnt/swapextensions/player/browserfav ]; then mv -f /mnt/swapextensions/player/browserfav /mnt/config/browserfav; fi
if [ -e /mnt/swapextensions/player/hbbtvfav ]; then mv -f /mnt/swapextensions/player/hbbtvfav /mnt/config/hbbtvfav; fi
if [ -e /mnt/swapextensions/player/searchhistory ]; then mv -f /mnt/swapextensions/player/searchhistory /mnt/config/searchhistory; fi
if [ -e /mnt/swapextensions/player/favorite ]; then mv -f /mnt/swapextensions/player/favorite /mnt/config/favorite; fi
if [ -e /mnt/swapextensions/player/playlist ]; then mv -f /mnt/swapextensions/player/playlist /mnt/config/playlist; fi

