#!/bin/sh

arch=$1

if [ "$arch" = "mipsel" ];then
	if ! grep -q "^rcfrontpower=" /mnt/config/rcconfig; then
		sed 's/rcpower=/\nrcfrontpower=416\nrcpower=/' -i /mnt/config/rcconfig
		echo "rcfrontpower=416" >> /mnt/config/titan.cfg
	fi
fi