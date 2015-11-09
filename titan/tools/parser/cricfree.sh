#!/bin/bash
#

buildtype=$1
wgetbin="wget -T2 -t2 --waitretry=2"

rm cache.*
rm _liste
rm -rf _full/cricfree
mkdir -p _full/cricfree/streams
rm cache.*
touch cache.cricfree.titanlist

BEGINTIME=`date +%s`
DATENAME=`date +"%Y.%m.%d_%H.%M.%S"`
echo "[cricfree.sh] START (buildtype: $buildtype): $DATENAME" > _full/cricfree/build.log

piccount=0
count=0

$wgetbin "http://cricfree.sx/" -O cache.cricfree.cache

#cat cache.cricfree.cache | tr '\n' '\r' |  tr '\r' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!<li class="has-sub">!\n<li class="has-sub">!g' | grep ^'<li class="has-sub">' | sed 's!href=!\n<a href=!g' | grep ^'<a href=' | grep "http://cricfree.sx" | grep -v "/watch/live/" | cut -d '"' -f2 > cache.cricfree1.cache

LIST=`cat cache.cricfree.cache | tr '\n' '\r' |  tr '\r' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!<li class="has-sub">!\n<li class="has-sub">!g' | grep ^'<li class="has-sub">' | sed 's!href=!\n<a href=!g' | grep ^'<a href=' | grep "http://cricfree.sx" | grep -v "/watch/live/" | cut -d '"' -f2`
echo LIST $LIST

count1=0
#for ROUND1 in $LIST; do
#	count1=`expr $count + 1`
#
#	echo PAGELIST $PAGELIST	
#	$wgetbin "$ROUND1" -O cache.cricfree.$count1.cache
#	LIST=`cat cache.cricfree.$count1.cache | tr '\n' '\r' |  tr '\r' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!<a href="http://www.cricfree.to/view/!\nhttp://www.cricfree.to/view/!g' | grep ^"http://www.cricfree.to/view/" | grep -v "<strong>" | grep -v "premium_only" | tr ' ' '~'`
#	
#	echo LIST $LIST
	for ROUND2 in $LIST; do
		count=`expr $count + 1`
		ROUND2=`echo $ROUND2 | tr '~' ' '`

		URL=`echo $ROUND2 | cut -d'"' -f1`
		TITLE=`echo $ROUND2 | sed 's!http://cricfree.sx/!!' | sed 's!watch/live/!!' | sed 's!-live-stream-2!!' | sed 's!-live-stream!!' | sed 's!-in-streaming!!' | sed 's!-live-streaming-2!!' | sed 's!-live-streaming!!'`
		PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
	
		TITLE=`echo $TITLE | sed 's/-/ /g' | sed 's/_/ /g'`
		TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
	
		echo PIC $PIC
		echo URL $URL
		echo TITLE "$TITLE"
	
		echo "################################################"
	
		if [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
			piccount=`expr $piccount + 1`
			LINE="$TITLE#$URL#$PIC#cricfree_$piccount.jpg#CricFree#92"
	#				if [ `cat cache.cricfree."$filename".titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
	#					echo $LINE >> cache.cricfree."$filename".titanlist
	#				fi
			if [ `cat cache.cricfree.titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
				echo $LINE >> cache.cricfree.titanlist
			fi
		fi
	done
#done

if [ "$buildtype" = "full" ];then
	cat cache.cricfree.titanlist | sort -u > _full/cricfree/streams/cricfree.all-sorted.list
	#cat cache.cricfree.category.titanlist | sort -u > _full/cricfree/cricfree.category.list
#	cat cache.cricfree.category.titanlist > _full/cricfree/cricfree.category.list
	
	for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
		filename=`echo "$ROUND" | tr 'A-Z' 'a-z'`
		if [ `cat cache.cricfree.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
			cat cache.cricfree.titanlist | grep ^"$ROUND" > cache.cricfree.titanlist."$ROUND"
			cat cache.cricfree.titanlist."$ROUND" | sort -u > _full/cricfree/streams/cricfree.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
			echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/cricfree/streams/cricfree."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#CricFree#0 >> _full/cricfree/cricfree.a-z.list
		elif [ `cat cache.cricfree.titanlist | grep ^"$filename" | wc -l` -gt 0 ];then
			cat cache.cricfree.titanlist | grep ^"$filename" > cache.cricfree.titanlist."$ROUND"
			cat cache.cricfree.titanlist."$ROUND" | sort -u > _full/cricfree/streams/cricfree.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
			echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/cricfree/streams/cricfree."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#CricFree#0 >> _full/cricfree/cricfree.a-z.list
		fi
	done
fi

DONETIME=`date +%s`
TIME=`expr $DONETIME - $BEGINTIME`
echo "[cricfree.sh] build time: ($TIME s) done" >> _full/cricfree/build.log	
echo "[cricfree.sh] cricfree: "`ls -1 _full/cricfree` >> _full/cricfree/build.log
echo "[cricfree.sh] cricfree/streams: "`ls -1 _full/cricfree/streams` >> _full/cricfree/build.log

if [ "$buildtype" != "full" ];then
	cp -a _full/cricfree/* /var/www/atemio/web/mediathek/cricfree
fi

rm cache.*

exit
