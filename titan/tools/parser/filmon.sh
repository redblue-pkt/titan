#!/bin/bash
#

buildtype=$1
wgetbin="wget -T2 -t2 --waitretry=2"

rm cache.*
rm _liste
rm -rf _full/filmon
mkdir -p _full/filmon/streams
rm cache.*
touch cache.filmon.titanlist

BEGINTIME=`date +%s`
DATENAME=`date +"%Y.%m.%d_%H.%M.%S"`
echo "[filmon.sh] START (buildtype: $buildtype): $DATENAME" > _full/filmon/build.log
old=0

if [ $old = 1 ];then
	rm -rf _full/filmontmp
	mkdir _full/filmontmp

	/opt/gs/filmon/create.sh _full/filmontmp
	cat _full/filmontmp/mainmenu-filmon.list | sed 's!/filmon/group!/filmon/streams/group!' > cache.filmon.category.titanlist
	cat _full/filmontmp/filmon/* > cache.filmon.titanlist
	cp _full/filmontmp/filmon/* _full/filmon/streams
	
	#if [ "$buildtype" = "full" ];then
		cat cache.filmon.titanlist | sort -u > _full/filmon/streams/filmon.all-sorted.list
		cat cache.filmon.category.titanlist | sort -u > _full/filmon/filmon.category.list
		
		for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
			filename=`echo "$ROUND" | tr 'A-Z' 'a-z'`
			if [ `cat cache.filmon.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
				cat cache.filmon.titanlist | grep ^"$ROUND" > cache.filmon.titanlist."$ROUND"
				cat cache.filmon.titanlist."$ROUND" | sort -u > _full/filmon/streams/filmon.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
				echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/filmon/streams/filmon."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#filmon#0 >> _full/filmon/filmon.a-z.list
			elif [ `cat cache.filmon.titanlist | grep ^"$filename" | wc -l` -gt 0 ];then
				cat cache.filmon.titanlist | grep ^"$filename" > cache.filmon.titanlist."$ROUND"
				cat cache.filmon.titanlist."$ROUND" | sort -u > _full/filmon/streams/filmon.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
				echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/filmon/streams/filmon."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#filmon#0 >> _full/filmon/filmon.a-z.list
			fi
		done
	#fi
	
	DONETIME=`date +%s`
	TIME=`expr $DONETIME - $BEGINTIME`
	echo "[filmon.sh] build time: ($TIME s) done" >> _full/filmon/build.log	
	echo "[filmon.sh] filmon: "`ls -1 _full/filmon` >> _full/filmon/build.log
	echo "[filmon.sh] filmon/streams: "`ls -1 _full/filmon/streams` >> _full/filmon/build.log
	
	rm -rf _full/filmontmp
else
	/opt/gs/filmon/create.sh _full/filmon
	cp -a _full/filmon/* /var/www/atemio/web/mediathek/filmon
	DONETIME=`date +%s`
	TIME=`expr $DONETIME - $BEGINTIME`
	echo "[filmon.sh] build time: ($TIME s) done" >> _full/filmon/build.log	
	echo "[filmon.sh] filmon: "`ls -1 _full/filmon` >> _full/filmon/build.log
	echo "[filmon.sh] filmon/streams: "`ls -1 _full/filmon/streams` >> _full/filmon/build.log

fi

if [ "$buildtype" != "full" ];then
	cp -a _full/filmon/* /var/www/atemio/web/mediathek/filmon
fi

rm cache.*

exit
