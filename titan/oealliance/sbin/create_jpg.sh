#!/bin/sh

DIR="$1"

if [ -z "$1" ];then
	echo "[create_jpg] use: create_mvi /path"
	exit
fi

LIST=`ls -1 $DIR | grep .mvi`

count=0
for ROUND in $LIST; do
	count=`expr $count + 1`
	TMPFILE=".tmpfile_$count.jpg"
	JPG=`echo $ROUND | sed 's/.mvi/.jpg/'`
	echo "[create_mvi] ($count) ffmpeg -i $DIR/$ROUND -vframes 1 -s 1280x720 $DIR/$JPG"
	ffmpeg -i $DIR/$ROUND -vframes 1 -s 1280x720 $DIR/$JPG
done

echo "[create_jpg] create ($count) mvi files, done !"
exit

