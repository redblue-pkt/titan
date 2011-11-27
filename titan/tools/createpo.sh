#!/bin/sh

if [ -z "$1" ]; then
	echo "usage: createpo.sh file [update]"
	exit
fi

if [ "$2" == "update" ]; then
	xgettext -j -k_ *.h -o "$1"
else
	xgettext -k_ *.h -o "$1"
fi
