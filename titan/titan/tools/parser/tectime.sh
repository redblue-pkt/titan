#!/bin/bash
#

buildtype=$1
wgetbin="wget -T2 -t2 --waitretry=2"

rm -rf _full/tectime
mkdir -p _full/tectime/streams
rm cache.tectime.*
touch cache.tectime.titanlist

BEGINTIME=`date +%s`
DATENAME=`date +"%Y.%m.%d_%H.%M.%S"`
echo "[tectime.sh] START (buildtype: $buildtype): $DATENAME" > _full/tectime/build.log

WATCHLIST=`$wgetbin http://www.tectime.tv -O - | sed 's/<li><a href=/\n<li><a href=/g' | grep '<li><a href=' | grep -v '<ul class="nav2">' | grep -E "(aktuelle-videos|sendungen)" | cut -d'"' -f2 | sed s'/.$//'`
if [ "$buildtype" != "full" ];then
	echo Buildtyp: $buildtype
	WATCHLIST=`echo $WATCHLIST | cut -d" " -f1`
fi

for ROUND1 in $WATCHLIST; do
	count=`expr $count + 1`
	filename=`echo $ROUND1 | tr '-' '.' | tr '/' '.'`
	touch cache.tectime."$filename".titanlist
	tagname=`echo $ROUND1 | tr '/' '\n' | tail -n1`

	tectimeurl=`$wgetbin http://www.tectime.tv/$ROUND1/ -O - | sed 's/<iframe src=/\n<iframe src=/g' | grep '<iframe src=' | cut -d'"' -f2`

	$wgetbin http://www.tectime.tv/$tectimeurl -O cache.tectime."$filename"."$count"
	LIST=`cat cache.tectime."$filename"."$count" | sed 's/<li class="yt_preview"><a href=/\n<li class="yt_preview"><a href=/g' | sed 's/<\/p><\/a><\/li>/\n<\/p><\/a><\/li>/g' | grep '<li class="yt_preview"><a href=' | tr "\'" '"' | sed 's/class="yt_preview_text">/"/g' | cut -d'"' -f4`

	for ROUND2 in `echo $LIST | cut -d' ' -f1`; do
		count=`expr $count + 1`
		$wgetbin $ROUND2 -O cache.tectime."$filename"."$count"

		cat cache.tectime."$filename"."$count" | tr '\n' ' ' | sed 's/<li class="yt-uix/\n<li class="yt-uix/g' | sed 's/<\/ol>/\n<\/ol>/g' |grep '<li class="yt-uix' >cache.tectime.temp

		while read -u 3 ROUND3; do
			TITLE=`echo $ROUND3 | sed 's/data-video-title=/\ndata-video-title=/g' | grep 'data-video-title=' | cut -d'"' -f2`
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' # ' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
			URL="http://www.youtube.com/get_video_info?&video_id="`echo $ROUND3 | sed 's/data-video-id=/\ndata-video-id=/g' | grep 'data-video-id=' | cut -d'"' -f2`
			PIC=`echo $ROUND3 | sed 's/data-thumb=/\ndata-thumb=/g' | grep 'data-thumb=' | cut -d'"' -f2`
			if [ -z "$PIC" ];then
				PIC=`echo $ROUND3 | sed 's/ src=/\n src=/g' | tail -n 1 | cut -d'"' -f2`
			fi
			PIC="http:"`echo $PIC | sed 's/default.jpg/hqdefault.jpg/g'`
	
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
		done 3<cache.tectime.temp
	done


	if [ -e cache.tectime."$filename".titanlist ];then
		piccount=`expr $piccount + 1`
		URL="http://atemio.dyndns.tv/mediathek/tectime/streams/tectime."$filename".list"
		PIC="http://atemio.dyndns.tv/mediathek/menu/"$tagname".jpg"
		LINE="$tagname#$URL#$PIC#tectime$piccount.jpg#tectime#0"
		echo $LINE >> cache.tectime.category.titanlist
		cat cache.tectime."$filename".titanlist >> _full/tectime/streams/tectime."$filename".list
	fi
done

ROUND1="tv-programm"
count=`expr $count + 1`
filename=`echo $ROUND1 | tr '-' '.' | tr '/' '.'`
tagname=`echo $ROUND1 | tr '/' '\n' | tail -n1`

$wgetbin http://www.tectime.tv/$ROUND1/ -O cache.tectime."$filename"."$count"

cat cache.tectime."$filename"."$count" | tr '\r' ' ' | tr '\n' '\r' | sed 's/<div class="sendeplan-item/\n<div class="sendeplan-item/g' | sed 's/<div id="footer/\n<div id="footer/g' | grep '^<div class="sendeplan-item' >cache.tectime.temp

while read -u 3 ROUND3; do
	YT_ID=`echo $ROUND3 | sed 's/<a href=/\n<a href=/g' | grep '^<a href=' | cut -d'"' -f2 | cut -d'/' -f4`
	TV_TIME=`echo $ROUND3 | sed 's/class="sendestart/\nclass="sendestart/g' | grep '^class="sendestart' | cut -d'>' -f2 | cut -d'<' -f1`
	TT_TL=`cat cache.tectime.titanlist | grep $YT_ID`
	if [ ! -z "$TT_TL" ] ; then
		echo "$TV_TIME $TT_TL" >> cache.tectime."$filename".titanlist
	else
		piccount=`expr $piccount + 1`
		TV_SENDUNG=`echo $ROUND3 | sed 's/class="sendestart/\nclass="sendestart/g' | grep '^class="sendestart' | cut -d'>' -f3 | cut -d'<' -f1`
		TV_INHALT=`echo $ROUND3 | sed 's/class="sendeinhalt/\nclass="sendeinhalt/g' | grep '^class="sendeinhalt' | cut -d'>' -f2 | cut -d'
' -f1`
		PIC=`echo $ROUND3 | sed 's/<img src=/\n<img src=/g' | grep '^<img src=' | cut -d'"' -f2`
		LINE="$TV_SENDUNG: $TV_INHALT#http://www.youtube.com/get_video_info?&video_id=$YT_ID#$PIC#tectime_$piccount.jpg#TecTime#4"
		echo "$LINE" >> cache.tectime.titanlist
		echo "$TV_TIME $LINE" >> cache.tectime."$filename".titanlist
	fi
done 3<cache.tectime.temp

if [ -e cache.tectime."$filename".titanlist ];then
	piccount=`expr $piccount + 1`
	URL="http://atemio.dyndns.tv/mediathek/tectime/streams/tectime."$filename".list"
	PIC="http://atemio.dyndns.tv/mediathek/menu/"$tagname".jpg"
	LINE="$tagname#$URL#$PIC#tectime$piccount.jpg#tectime#0"
	echo $LINE >> cache.tectime.category.titanlist
	cat cache.tectime."$filename".titanlist >> _full/tectime/streams/tectime."$filename".list
fi

if [ "$buildtype" = "full" ];then
	cat cache.tectime.titanlist | sort -u > _full/tectime/streams/tectime.all-sorted.list
	cat cache.tectime.category.titanlist | sort -u > _full/tectime/tectime.category.list
	
	for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
		filename=`echo "$ROUND" | tr 'A-Z' 'a-z'`
		if [ `cat cache.tectime.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
			cat cache.tectime.titanlist | grep ^"$ROUND" > cache.tectime.titanlist."$ROUND"
			cat cache.tectime.titanlist."$ROUND" | sort -u > _full/tectime/streams/tectime.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
			echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/tectime/streams/tectime."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#TecTime#0 >> _full/tectime/tectime.a-z.list
		elif [ `cat cache.tectime.titanlist | grep ^"$filename" | wc -l` -gt 0 ];then
			cat cache.tectime.titanlist | grep ^"$filename" > cache.tectime.titanlist."$ROUND"
			cat cache.tectime.titanlist."$ROUND" | sort -u > _full/tectime/streams/tectime.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
			echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/tectime/streams/tectime."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#TecTime#0 >> _full/tectime/tectime.a-z.list
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
