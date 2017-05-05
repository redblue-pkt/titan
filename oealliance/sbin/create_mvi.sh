#!/bin/sh

DIR="$1"

if [ -z "$1" ];then
	echo "[create_mvi] use: create_mvi /path"
	exit
fi

LIST=`ls -1 $DIR | grep .jpg`
LIST="$LIST `ls -1 $DIR | grep .jpeg`"

count=0
for ROUND in $LIST; do
	count=`expr $count + 1`
	TMPFILE=".tmpfile_$count.jpg"
	MPG=`echo $ROUND | sed 's/.jpg/.mpg/' | sed 's/.jpeg/.mpg/'`
	MVI=`echo $ROUND | sed 's/.jpg/.mvi/' | sed 's/.jpeg/.mvi/'`

	echo "[create_mvi] ($count) jpegtran -outfile $DIR/$TMPFILE -copy none $DIR/$ROUND"
	jpegtran -outfile "$DIR/$TMPFILE" -copy none "$DIR/$ROUND"

	echo "[create_mvi] ($count) ffmpeg -y -f image2 -i $DIR/$TMPFILE $DIR/$MPG"
	ffmpeg -y -f image2 -i "$DIR/$TMPFILE" "$DIR/$MPG"

	echo "[create_mvi] ($count) mv -f $DIR/$MPG $DIR/$MVI"
	mv -f "$DIR/$MPG" "$DIR/$MVI"
	rm -f "$DIR/$TMPFILE"
done

echo "[create_mvi] create ($count) mvi files, done !"
exit

