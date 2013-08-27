#!/bin/bash
#

buildtype=$1
wgetbin="wget -T2 -t2 --waitretry=2 --no-check-certificate"

rm cache.*
rm _liste
rm -rf _full/internettv
mkdir -p _full/internettv/streams
rm cache.*
touch cache.internettv.titanlist
echo "NBA TV#rtmp://31.7.58.122:1935/stream/I8s9S4H7.stream playpath=N1V3R0h8.stream swfUrl=http://thecdn.04stream.com/p/bbp.swf live=true  pageUrl=http://www.04stream.com/NBA TV#http://atemio.dyndns.tv/mediathek/menu/nbatv.jpg#internettv.nba.tv.jpg#Internet Tv#2" >> cache.internettv.titanlist
echo "TV Halle#rtmp://62.113.210.250/medienasa-live/tvhalle_high#http://atemio.dyndns.tv/mediathek/menu/tvhalle.jpg#internettv.tvhalle.jpg#Internet Tv#2" >> cache.internettv.titanlist
echo "TV Magdeburg#rtmp://62.113.210.250/medienasa-live/ok-magdeburg_high#http://atemio.dyndns.tv/mediathek/menu/tvmagdeburg.jpg#internettv.tvmagdeburg.jpg#Internet Tv#2" >> cache.internettv.titanlist
echo "Virgin 1#rtmp://fms.105.net:1935/live/virgin1 playpath=virgin1 swfurl=http://www.highermedia.com/assets/flash/jw_player.swf live=1 pageurl=http://www.google.com/Virgin 1#http://atemio.dyndns.tv/mediathek/menu/virgin.1.jpg#internettv.virgin.1.jpg#Internet Tv#2" >> cache.internettv.titanlist

BEGINTIME=`date +%s`
DATENAME=`date +"%Y.%m.%d_%H.%M.%S"`
echo "[internettv.sh] START (buildtype: $buildtype): $DATENAME" > _full/internettv/build.log

testcount=0
piccount=0
count=0
icount=0

count=`expr $count + 1`	
#$wgetbin "https://gitorious.org/~max10/neutrino-mp/max10s-neutrino-mp/blobs/raw/master/data/webtv/webtv.xml" -O cache.mainfile
$wgetbin "http://gitorious.org/neutrino-mp/max10s-neutrino-mp/raw/787e39155fc72e6df89dc01d502f457fc10f92a8:data/webtv/webtv.xml" -O cache.mainfile

id_list="`cat cache.mainfile | tr '\n' ' ' | sed 's/ \+/ /g' | sed 's/<webtv/\n\n<webtv/g' | grep ^"<webtv" | tr ' ' '~'`"

#exit

count1=0
for ROUND1 in $id_list; do
	piccount=`expr $piccount + 1`
	count1=`expr $count1 + 1`

	URL=`echo $ROUND1 | sed 's/url="/\nurl="/g' | grep ^"url=" | tr '~' ' ' | cut -d '"' -f2`
	TITLE=`echo $ROUND1 | sed 's/title="/\ntitle="/g' | grep ^"title=" | tr '~' ' ' | cut -d '"' -f2`
	TITLE=`echo $TITLE | sed 's/&amp;/und/g'`
	TITLE=`echo $TITLE | sed 's/&quot;/"/g'`
	TITLE=`echo $TITLE | sed 's/&uuml;/\ü/g'`
	TITLE=`echo $TITLE | sed 's/&auml;/\ä/g'`
	TITLE=`echo $TITLE | sed 's/&ouml;/\ö/g'`
	TITLE=`echo $TITLE | sed 's/&szlig;/\ß/g'`
	TITLE=`echo $TITLE | sed "s/&#039;/'/g"`
	TITLE=`echo $TITLE | sed 's/&#034;/"/g'`
	TITLE=`echo $TITLE | sed 's/&#038;/&/g'`
	
	PIC=http://atemio.dyndns.tv/mediathek/menu/`echo $TITLE | tr 'A-Z' 'a-z' | tr ' ' '_'`.jpg
	
	if [ -z "$PIC" ]; then
		PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
	fi

	if [ ! -z "$TITLE" ] && [ ! -z "$URL" ] && [ `echo "$TITLE" | grep -e ^[a-z] -e ^[A-Z] -e ^[0-9] | wc -l` -eq 1 ];then
		LINE="$TITLE#$URL#$PIC#internettv_$piccount.jpg#Internet TV#2"
		if [ `cat cache.internettv.titanlist | grep "^$TITLE" | wc -l` -eq 0 ];then
			echo $LINE >> cache.internettv.titanlist
		fi
	fi
#		echo URL $URL
#		echo PIC $PIC
#		echo TITLE $TITLE
done

cat cache.internettv.titanlist | sort -u > _full/internettv/streams/internettv.all-sorted.list

for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
	filename=`echo "$ROUND" | tr 'A-Z' 'a-z'`
	if [ `cat cache.internettv.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
		cat cache.internettv.titanlist | grep ^"$ROUND" >> cache.internettv.titanlist."$ROUND"
		cat cache.internettv.titanlist."$ROUND" | sort -um > _full/internettv/streams/internettv.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
		echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/internettv/streams/internettv."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#Internet TV#3 >> _full/internettv/internettv.a-z.list
	elif [ `cat cache.internettv.titanlist | grep ^"$filename" | wc -l` -gt 0 ];then
		cat cache.internettv.titanlist | grep ^"$filename" >> cache.internettv.titanlist."$ROUND"
		cat cache.internettv.titanlist."$ROUND" | sort -um > _full/internettv/streams/internettv.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
		echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/internettv/streams/internettv."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#Internet TV#3 >> _full/internettv/internettv.a-z.list

	fi
done

DONETIME=`date +%s`
TIME=`expr $DONETIME - $BEGINTIME`
echo "[internettv.sh] build time: ($TIME s) done" >> _full/internettv/build.log	
echo "[internettv.sh] internettv: "`ls -1 _full/internettv` >> _full/internettv/build.log
echo "[internettv.sh] internettv/streams: "`ls -1 _full/internettv/streams` >> _full/internettv/build.log

if [ "$buildtype" != "full" ];then
	cp -a _full/internettv/* /var/www/atemio/web/mediathek/internettv
fi

rm cache.*

exit
