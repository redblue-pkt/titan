#!/bin/bash
#

buildtype=$1
wgetbin="wget -T2 -t2 --waitretry=2"

rm cache.*
rm _liste
rm -rf _full/youtube
mkdir -p _full/youtube/streams
rm cache.*
touch cache.youtube.titanlist

BEGINTIME=`date +%s`
DATENAME=`date +"%Y.%m.%d_%H.%M.%S"`
echo "[youtube.sh] START (buildtype: $buildtype): $DATENAME" > _full/youtube/build.log
old=0

if [ $old = 1 ];then
	rm -rf _full/youtubetmp
	mkdir _full/youtubetmp

	/opt/gs/youtube/create.sh _full/youtubetmp
	cat _full/youtubetmp/mainmenu-youtube.list | sed 's!/youtube/group!/youtube/streams/group!' > cache.youtube.category.titanlist
	cat _full/youtubetmp/youtube/* > cache.youtube.titanlist
	cp _full/youtubetmp/youtube/* _full/youtube/streams
	
	#if [ "$buildtype" = "full" ];then
		cat cache.youtube.titanlist | sort -u > _full/youtube/streams/youtube.all-sorted.list
		cat cache.youtube.category.titanlist | sort -u > _full/youtube/youtube.category.list
		
		for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
			filename=`echo "$ROUND" | tr 'A-Z' 'a-z'`
			if [ `cat cache.youtube.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
				cat cache.youtube.titanlist | grep ^"$ROUND" > cache.youtube.titanlist."$ROUND"
				cat cache.youtube.titanlist."$ROUND" | sort -u > _full/youtube/streams/youtube.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
				echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/youtube/streams/youtube."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#youtube#0 >> _full/youtube/youtube.a-z.list
			elif [ `cat cache.youtube.titanlist | grep ^"$filename" | wc -l` -gt 0 ];then
				cat cache.youtube.titanlist | grep ^"$filename" > cache.youtube.titanlist."$ROUND"
				cat cache.youtube.titanlist."$ROUND" | sort -u > _full/youtube/streams/youtube.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
				echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/youtube/streams/youtube."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#youtube#0 >> _full/youtube/youtube.a-z.list
			fi
		done
	#fi
   

	DONETIME=`date +%s`
	TIME=`expr $DONETIME - $BEGINTIME`
	echo "[youtube.sh] build time: ($TIME s) done" >> _full/youtube/build.log	
	echo "[youtube.sh] youtube: "`ls -1 _full/youtube` >> _full/youtube/build.log
	echo "[youtube.sh] youtube/streams: "`ls -1 _full/youtube/streams` >> _full/youtube/build.log
	
	rm -rf _full/youtubetmp
else
	/opt/gs/youtube/create.sh _full/youtube
	cp -a _full/youtube/* /var/www/atemio/web/mediathek/youtube
	DONETIME=`date +%s`
	TIME=`expr $DONETIME - $BEGINTIME`
	echo "[youtube.sh] build time: ($TIME s) done" >> _full/youtube/build.log	
	echo "[youtube.sh] youtube: "`ls -1 _full/youtube` >> _full/youtube/build.log
	echo "[youtube.sh] youtube/streams: "`ls -1 _full/youtube/streams` >> _full/youtube/build.log

fi

if [ "$buildtype" != "full" ];then
	cp -a _full/youtube/* /var/www/atemio/web/mediathek/youtube
fi

rm cache.*

exit
