#!/bin/bash
#

buildtype=$1
wgetbin="wget -T2 -t2 --waitretry=2"

rm cache.*
rm _liste
rm -rf _full/streamlive
mkdir -p _full/streamlive/streams
rm cache.*
touch cache.streamlive.titanlist

BEGINTIME=`date +%s`
DATENAME=`date +"%Y.%m.%d_%H.%M.%S"`
echo "[streamlive.sh] START (buildtype: $buildtype): $DATENAME" > _full/streamlive/build.log

piccount=0
count=0

$wgetbin "http://www.streamlive.to/channels/?q=&sort=1" -O cache.streamlive.cache

PAGECOUNT=`cat cache.streamlive.cache | tr '\n' '\r' |  tr '\r' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!http://www.streamlive.to/channels/?p=!\nhttp://www.streamlive.to/channels/?p=!g' | grep ^"http://www.streamlive.to/channels/?p=" | wc -l`
PAGECOUNT=`expr $PAGECOUNT + 1`
echo PAGECOUNT $PAGECOUNT	

i=1
until [ "$i" -gt "$PAGECOUNT" ]
do
	PAGELIST="$PAGELIST http://www.streamlive.to/channels/?p=$i&q=&sort=1"
	i=$[$i+1]
done
count1=0
for ROUND1 in $PAGELIST; do
	count1=`expr $count + 1`
	
	echo PAGELIST $PAGELIST	
	$wgetbin "$ROUND1" -O cache.streamlive.$count1.cache
	LIST=`cat cache.streamlive.$count1.cache | tr '\n' '\r' |  tr '\r' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!<a href="http://www.streamlive.to/view/!\nhttp://www.streamlive.to/view/!g' | grep ^"http://www.streamlive.to/view/" | grep -v "<strong>" | grep -v "premium_only" | tr ' ' '~'`
	
	echo LIST $LIST
	for ROUND2 in $LIST; do
		count=`expr $count + 1`
		ROUND2=`echo $ROUND2 | tr '~' ' '`

		URL=`echo $ROUND2 | cut -d'"' -f1`
		PIC=http:`echo $ROUND2 | sed 's!src="!\npic="!g' | grep ^pic= | cut -d'"' -f2 | tail -n1`
		TITLE=`echo $ROUND2 | sed 's!title="!\ntitle="!g' | grep ^title= | cut -d'"' -f2 | tail -n1`
		if [ -z "$TITLE" ];then 
			TITLE=`echo $ROUND2 | cut -d"<" -f2 | cut -d">" -f2`
		fi
	
		if [ -z "$PIC" ] || [ "$PIC" = "http:" ]; then  
			PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
		fi
	
		TITLE=`echo $TITLE | sed 's/-live-stream-channel//' | sed 's/_/ /g'`
		TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
	
		echo PIC $PIC
		echo URL $URL
		echo TITLE "$TITLE"
	
		echo "################################################"
	
		if [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
			piccount=`expr $piccount + 1`
			LINE="$TITLE#$URL#$PIC#streamlive_$piccount.jpg#StreamLive#91"
	#				if [ `cat cache.streamlive."$filename".titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
	#					echo $LINE >> cache.streamlive."$filename".titanlist
	#				fi
			if [ `cat cache.streamlive.titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
				echo $LINE >> cache.streamlive.titanlist
			fi
		fi
	done
done

if [ "$buildtype" = "full" ];then
	cat cache.streamlive.titanlist | sort -u > _full/streamlive/streams/streamlive.all-sorted.list
	#cat cache.streamlive.category.titanlist | sort -u > _full/streamlive/streamlive.category.list
#	cat cache.streamlive.category.titanlist > _full/streamlive/streamlive.category.list
	
	for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
		filename=`echo "$ROUND" | tr 'A-Z' 'a-z'`
		if [ `cat cache.streamlive.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
			cat cache.streamlive.titanlist | grep ^"$ROUND" > cache.streamlive.titanlist."$ROUND"
			cat cache.streamlive.titanlist."$ROUND" | sort -u > _full/streamlive/streams/streamlive.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
			echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/streamlive/streams/streamlive."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#streamlive#0 >> _full/streamlive/streamlive.a-z.list
		elif [ `cat cache.streamlive.titanlist | grep ^"$filename" | wc -l` -gt 0 ];then
			cat cache.streamlive.titanlist | grep ^"$filename" > cache.streamlive.titanlist."$ROUND"
			cat cache.streamlive.titanlist."$ROUND" | sort -u > _full/streamlive/streams/streamlive.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
			echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/streamlive/streams/streamlive."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#streamlive#0 >> _full/streamlive/streamlive.a-z.list
		fi
	done
fi

DONETIME=`date +%s`
TIME=`expr $DONETIME - $BEGINTIME`
echo "[streamlive.sh] build time: ($TIME s) done" >> _full/streamlive/build.log	
echo "[streamlive.sh] streamlive: "`ls -1 _full/streamlive` >> _full/streamlive/build.log
echo "[streamlive.sh] streamlive/streams: "`ls -1 _full/streamlive/streams` >> _full/streamlive/build.log

if [ "$buildtype" != "full" ];then
	cp -a _full/streamlive/* /var/www/atemio/web/mediathek/streamlive
fi

rm cache.*

exit
