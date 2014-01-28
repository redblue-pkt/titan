#!/bin/bash
#

buildtype=$1
wgetbin="wget -T2 -t2 --waitretry=2"

rm cache.*
rm _liste
rm -rf _full/tectime
mkdir -p _full/tectime/streams
rm cache.*
touch cache.tectime.titanlist

BEGINTIME=`date +%s`
DATENAME=`date +"%Y.%m.%d_%H.%M.%S"`
echo "[tectime.sh] START (buildtype: $buildtype): $DATENAME" > _full/tectime/build.log

if [ "$buildtype" = "full" ];then
WATCHLIST="
neueste-videos
sendungen/multimedia/dr-dish-magazin
sendungen/multimedia/hard-soft
sendungen/wissen
sendungen/app-der-woche
sendungen/check
sendungen/pixel
sendungen/standpunkt
sendungen/tectime
sendungen/hands-on
sendungen/etv
sendungen/esa-tv
sendungen/portalzine-tv
tv-programm
"
else
WATCHLIST="
neueste-videos
"
fi

for ROUND1 in $WATCHLIST; do
	count=`expr $count + 1`
	filename=`echo $ROUND1 | tr '-' '.' | tr '/' '.'`
	tagname=`echo $ROUND1 | tr '/' '\n' | tail -n1`

	$wgetbin http://www.tectime.tv/$ROUND1/ -O cache.tectime."$filename"."$count"
	LIST=`cat cache.tectime."$filename"."$count" | grep youtube | sed 's!src="//www.youtube.com!http://www.youtube.com!g' | tr ' ' '\n' | sed 's!?&amp;!\n?&amp;!g' | grep ^http://`

	for ROUND2 in $LIST; do
		count=`expr $count + 1`
		$wgetbin $ROUND2 -O cache.tectime."$filename"."$count"
		ls cache.tectime."$filename"."$count"	

		videoid=`cat cache.tectime."$filename"."$count" | sed 's!"video_id": "!\nvideoid="!g' | grep ^videoid= | cut -d'"' -f2`
		echo videoid $videoid

		playlist=`cat cache.tectime."$filename"."$count" | sed 's!"list": "!\nlist="!g' | grep ^list= | cut -d'"' -f2`
		echo playlist $playlist

		URL="http://www.youtube.com/watch?v=$videoid&list=$playlist"
		count=`expr $count + 1`
		echo URL $URL
		$wgetbin $URL -O cache.tectime."$filename"."$count"

		VLIST=`cat cache.tectime."$filename"."$count" | grep /watch? | sed 's!/watch?v=!\nwatch="!g' | grep ^watch | cut -d'"' -f2 | sort -u`

		for ROUND3 in $VLIST; do
			count=`expr $count + 1`
			$wgetbin http://www.youtube.com/watch?v=$ROUND3 -O cache.tectime."$filename"."$count"
	
			ROUND3=`echo $ROUND3 | cut -d'&' -f1`
	
			TITLE=`cat cache.tectime."$filename"."$count" | grep '<meta property="og:title"' | cut -d'"' -f4`
	
			URL="http://www.youtube.com/get_video_info?&video_id=$ROUND3"
			PIC=http://i.ytimg.com/vi/"$ROUND3"/0.jpg
				
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' # ' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
	
			if [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
				piccount=`expr $piccount + 1`
				LINE="$TITLE#$URL#$PIC#tectime_$piccount.jpg#TecTime#4"
				if [ `cat cache.tectime."$filename".titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
					echo $LINE >> cache.tectime."$filename".titanlist
				fi
				if [ `cat cache.tectime.titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
					echo $LINE >> cache.tectime.titanlist
				fi
			fi
		done
	done


	if [ -e cache.tectime."$filename".titanlist ];then
		piccount=`expr $piccount + 1`
		URL="http://atemio.dyndns.tv/mediathek/tectime/streams/tectime."$filename".list"
		PIC="http://atemio.dyndns.tv/mediathek/menu/"$tagname".jpg"
		LINE="$tagname#$URL#$PIC#tectime$piccount.jpg#tectime#3"
		echo $LINE >> cache.tectime.category.titanlist
		cat cache.tectime."$filename".titanlist >> _full/tectime/streams/tectime."$filename".list
	fi
done

if [ "$buildtype" = "full" ];then
	cat cache.tectime.titanlist | sort -u > _full/tectime/streams/tectime.all-sorted.list
	cat cache.tectime.category.titanlist | sort -u > _full/tectime/tectime.category.list
	
	for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
		filename=`echo "$ROUND" | tr 'A-Z' 'a-z'`
		if [ `cat cache.tectime.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
			cat cache.tectime.titanlist | grep ^"$ROUND" > cache.tectime.titanlist."$ROUND"
			cat cache.tectime.titanlist."$ROUND" | sort -u > _full/tectime/streams/tectime.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
			echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/tectime/streams/tectime."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#TecTime#3 >> _full/tectime/tectime.a-z.list
		elif [ `cat cache.tectime.titanlist | grep ^"$filename" | wc -l` -gt 0 ];then
			cat cache.tectime.titanlist | grep ^"$filename" > cache.tectime.titanlist."$ROUND"
			cat cache.tectime.titanlist."$ROUND" | sort -u > _full/tectime/streams/tectime.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
			echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/tectime/streams/tectime."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#TecTime#3 >> _full/tectime/tectime.a-z.list
		fi
	done
fi

DONETIME=`date +%s`
TIME=`expr $DONETIME - $BEGINTIME`
echo "[tectime.sh] build time: ($TIME s) done" >> _full/tectime/build.log	
echo "[tectime.sh] tectime: "`ls -1 _full/tectime` >> _full/tectime/build.log
echo "[tectime.sh] tectime/streams: "`ls -1 _full/tectime/streams` >> _full/tectime/build.log

if [ "$buildtype" != "full" ];then
	cp -a _full/tectime/* /var/www/atemio/web/mediathek/tectime
fi

rm cache.*

exit
