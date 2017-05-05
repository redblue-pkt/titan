#!/bin/sh

#----------------------- USAGE -------------------------------------------------
# /sbin/settings restore        -> restore settings on startup
# /sbin/settings backup         -> settings backup without touching .firstrun file (only backup your settings)
# GREETZ Civer
#-------------------------------------------------------------------------------

TYPE=$1
ret=0

boxtype=`cat /etc/model`

backpath="/var/backup"


ocp()
{
	if [ -e "$1" ]; then
		mkdir -p "$2"
		if [ `ls -l "$1" | wc -l` == "0" ]; then return; fi
		cp -a "$1"/* "$2"
		if [ $? -ne 0 ]; then ret=1; fi
	fi
}

### backup ###
if [ "$TYPE" == "backup" ]; then
	if [ ! -e /tmp/.backupdev ]; then exit 1; fi

	echo "[settings.sh] Backup your settings to $backpath!"

	if [ ! -d "$backpath/settings" ]; then rm -f "$backpath/settings"; mkdir -p "$backpath/settings"; fi
	if [ ! -d "$backpath/config" ]; then rm -f "$backpath/config"; mkdir -p "$backpath/config"; fi
	if [ ! -d "$backpath/network" ]; then rm -f "$backpath/network"; mkdir -p "$backpath/network"; fi
	if [ ! -d "$backpath/script" ]; then rm -f "$backpath/script"; mkdir -p "$backpath/script"; fi
	if [ ! -d "$backpath/tpk" ]; then rm -f "$backpath/tpk"; mkdir -p "$backpath/tpk"; fi
	if [ ! -d "$backpath/keys" ]; then rm -f "$backpath/keys"; mkdir -p "$backpath/keys"; fi
	if [ ! -d "$backpath/player" ]; then rm -f "$backpath/player"; mkdir -p "$backpath/player"; fi

	rm -rf "$backpath"/settings/*
	rm -rf "$backpath"/config/*
	rm -rf "$backpath"/network/*
	rm -rf "$backpath"/script/*
	rm -rf "$backpath"/tpk/*
	rm -rf "$backpath"/keys/*
	rm -rf "$backpath"/player/*

	ocp /mnt/settings "$backpath/settings"
	ocp /mnt/config "$backpath/config"
	ocp /mnt/network "$backpath/network"
	ocp /mnt/script "$backpath/script"
	ocp /mnt/tpk "$backpath/tpk"
	ocp /mnt/swapextensions/keys "$backpath/keys"
	ocp /mnt/swapextensions/player "$backpath/player"

	#set version of tpk to 0, so we can update all tpk
	sed "s/Version:.*/Version: 0/" -i "$backpath"/tpk/*/control

	sync
	exit $ret
fi

### restore ###
if [ "$TYPE" == "restore" ]; then
	if [ ! -e /tmp/.backupdev ]; then exit 1; fi

	echo "[settings.sh] Restoring your settings from $backpath!"

	ocp "$backpath/settings" /mnt/settings/
#	ocp "$backpath/config" /mnt/config/
	ocp "$backpath/network" /mnt/network/
	ocp "$backpath/script" /mnt/script/
	ocp "$backpath/tpk" /mnt/tpk/
	ocp "$backpath/keys" /mnt/swapextensions/keys/
	ocp "$backpath/player" /mnt/swapextensions/player/
	chmod 664 /mnt/network/*

	sync-start-config.sh
	sync-titan-config.sh

	sync
	exit $ret
fi

echo "[settings.sh] not used...."
