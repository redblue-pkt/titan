#!/bin/sh

MOUNTPOINT=/media/smbfs

case "$1" in
	start)
		test -d "${MOUNTPOINT}" || mkdir "${MOUNTPOINT}"
		if ! /usr/bin/smbnetfs -o allow_root config=/etc/smbnetfs.common.conf "${MOUNTPOINT}"
		then
			rmdir "${MOUNTPOINT}"
			exit 1
		fi
		;;
	stop)
		umount "${MOUNTPOINT}"
		rmdir "${MOUNTPOINT}"
		;;
	*)
		exit 1
		;;
esac
