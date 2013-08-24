#!/bin/bash
#

buildtype=$1
wgetbin="wget -T2 -t2 --waitretry=2"

rm cache.*
rm _liste
rm -rf _full/internetradio
mkdir -p _full/internetradio/streams
rm cache.*
touch cache.internetradio.titanlist

BEGINTIME=`date +%s`
DATENAME=`date +"%Y.%m.%d_%H.%M.%S"`
echo "[internetradio.sh] START (buildtype: $buildtype): $DATENAME" > _full/internetradio/build.log

testcount=0
piccount=0
count=0
icount=0

count=`expr $count + 1`	
$wgetbin "http://dir.xiph.org/yp.xml" -O cache.mainfile

id_list="`cat cache.mainfile | tr '\n' ' ' | sed 's/ \+/ /g' | sed 's/<entry>/\n\n<entry>/g' | grep ^"<entry>" | tr ' ' '~'`"

count1=0
for ROUND1 in $id_list; do
	piccount=`expr $piccount + 1`
	count1=`expr $count1 + 1`

	URL=`echo $ROUND1 | sed 's/<listen_url>/\nurl=</g' | grep ^"url=" | tr '~' ' ' | cut -d '<' -f2`
	TITLE=`echo $ROUND1 | sed 's/<server_name>/\ntitle=</g' | grep ^"title=" | tr '~' ' ' | cut -d '<' -f2`
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
		LINE="$TITLE#$URL#$PIC#internetradio_$piccount.jpg#Internet Radio#2"
		if [ `cat cache.internetradio.titanlist | grep "^$TITLE" | wc -l` -eq 0 ];then
			echo $LINE >> cache.internetradio.titanlist
		fi
	fi
#		echo URL $URL
#		echo PIC $PIC
#		echo TITLE $TITLE
done

cat cache.internetradio.titanlist | sort -u > _full/internetradio/streams/internetradio.all-sorted.list

for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
	filename=`echo "$ROUND" | tr 'A-Z' 'a-z'`
	if [ `cat cache.internetradio.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
		cat cache.internetradio.titanlist | grep ^"$ROUND" >> cache.internetradio.titanlist."$ROUND"
		cat cache.internetradio.titanlist."$ROUND" | sort -um > _full/internetradio/streams/internetradio.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
		echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/internetradio/streams/internetradio."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#Internet Radio#3 >> _full/internetradio/internetradio.a-z.list
	elif [ `cat cache.internetradio.titanlist | grep ^"$filename" | wc -l` -gt 0 ];then
		cat cache.internetradio.titanlist | grep ^"$filename" >> cache.internetradio.titanlist."$ROUND"
		cat cache.internetradio.titanlist."$ROUND" | sort -um > _full/internetradio/streams/internetradio.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
		echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/internetradio/streams/internetradio."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#Internet Radio#3 >> _full/internetradio/internetradio.a-z.list

	fi
done

DONETIME=`date +%s`
TIME=`expr $DONETIME - $BEGINTIME`
echo "[internetradio.sh] build time: ($TIME s) done" >> _full/internetradio/build.log	
echo "[internetradio.sh] internetradio: "`ls -1 _full/internetradio` >> _full/internetradio/build.log
echo "[internetradio.sh] internetradio/streams: "`ls -1 _full/internetradio/streams` >> _full/internetradio/build.log

if [ "$buildtype" != "full" ];then
	cp -a _full/internetradio/* /var/www/atemio/web/mediathek/internetradio
fi

rm cache.*

exit
