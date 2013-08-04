#!/bin/bash
#

wgetbin="wget -T2 -t2 --waitretry=2"
buildtype=$1

rm cache.*
rm _liste
rm -rf _full/giga
mkdir -p _full/giga/streams
rm cache.*
touch cache.giga.titanlist

BEGINTIME=`date +%s`
DATENAME=`date +"%Y.%m.%d_%H.%M.%S"`
echo "[giga.sh] START (buildtype: $buildtype): $DATENAME" > _full/giga/build.log

WATCHLIST="
tv/alle-videos
"	

for ROUND0 in $WATCHLIST; do
	tags=""
	tagcount=23
	i=1
	until [ "$i" -gt "$tagcount" ]
	do
#	echo $i
	tags="$tags "$ROUND0"/page/$i"
	i=$[$i+1]
	
	done
	filename=`echo $ROUND0 | tr '-' '.' | tr '/' '.'`
#	tagname=`echo $ROUND0 | tr '/' '\n' | tail -n1`
	tagname=`echo $ROUND0 | tr '/' '-'`
			
	for ROUND1 in $tags; do
		count=`expr $count + 1`
	#	$wgetbin http://www.giga.de/$ROUND1/ -O cache.giga."$filename"."$count"
		$wgetbin http://www.giga.de/$ROUND1 -O cache.giga."$filename"."$count"
	
	#	LIST=`cat cache.giga."$filename"."$count" | sed -e 's/$/\r/' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's/ \+/~/g' | sed 's!<span~class="timestamp">!\n\n\n<span~class="timestamp">!g' | grep ^'<span~class="timestamp">' > tests`
		LIST=`cat cache.giga."$filename"."$count" | grep "<h1><a href=" | cut -d'"' -f2`
		for ROUND2 in $LIST; do
			count=`expr $count + 1`
#			echo ROUND2 $ROUND2
			$wgetbin $ROUND2 -O cache.giga."$filename"."$count"
#			ls cache.giga."$filename"."$count"
			URL=`cat cache.giga."$filename"."$count" | grep 'rel="media:video" resource=' | sed 's!rel="media:video" resource=!\nlink=!g' | grep ^link= | cut -d'"' -f2 | tail -n1`
#			echo URL $URL
			PIC=`cat cache.giga."$filename"."$count" | grep 'rel="media:video" resource=' | sed 's!poster=!\npic=!g' | grep ^pic= | cut -d'"' -f2 | tail -n1`
#			echo PIC $PIC
	
			TITLE=`cat cache.giga."$filename"."$count" | grep 'rel="media:video" resource=' | sed 's!"POST_TITLE":!\ntitle=!g' | grep ^title= | cut -d'"' -f2`
	
			TITLE=`echo $TITLE | sed 's/&amp;/und/g'`
			TITLE=`echo $TITLE | sed 's/&quot;/"/g'`
			TITLE=`echo $TITLE | sed 's/&uuml;/\�/g'`
			TITLE=`echo $TITLE | sed 's/&auml;/\�/g'`
			TITLE=`echo $TITLE | sed 's/&ouml;/\�/g'`
			TITLE=`echo $TITLE | sed 's/&szlig;/\�/g'`
#			echo TITLE $TITLE
#			echo "################################################"
	
			if [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
				piccount=`expr $piccount + 1`
				LINE="$TITLE#$URL#$PIC#giga_$piccount.jpg#Giga#2"
				if [ `cat cache.giga."$filename".titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
					echo $LINE >> cache.giga."$filename".titanlist
				fi
				if [ `cat cache.giga.titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
					echo $LINE >> cache.giga.titanlist
				fi
			fi
		done
	done
	if [ -e cache.giga."$filename".titanlist ];then
		piccount=`expr $piccount + 1`
		URL="http://atemio.dyndns.tv/mediathek/giga/streams/giga."$filename".list"
		PIC="http://atemio.dyndns.tv/mediathek/menu/"$tagname".jpg"
		LINE="$tagname#$URL#$PIC#giga$piccount.jpg#Giga#3"
		echo $LINE >> cache.giga.category.titanlist
		cat cache.giga."$filename".titanlist >> _full/giga/streams/giga."$filename".list
	fi
done

cat cache.giga.titanlist | sort -u > _full/giga/streams/giga.all-sorted.list
cat cache.giga.category.titanlist | sort -u > _full/giga/giga.category.list

for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
	filename=`echo "$ROUND" | tr 'A-Z' 'a-z'`
	if [ `cat cache.giga.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
		cat cache.giga.titanlist | grep ^"$ROUND" > cache.giga.titanlist."$ROUND"
		cat cache.giga.titanlist."$ROUND" | sort -um > _full/giga/streams/giga.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
		echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/giga/streams/giga."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#Giga#3 >> _full/giga/giga.a-z.list
	elif [ `cat cache.giga.titanlist | grep ^"$filename" | wc -l` -gt 0 ];then
		cat cache.giga.titanlist | grep ^"$filename" > cache.giga.titanlist."$ROUND"
		cat cache.giga.titanlist."$ROUND" | sort -um > _full/giga/streams/giga.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
		echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/giga/streams/giga."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#Giga#3 >> _full/giga/giga.a-z.list
	fi
done

DONETIME=`date +%s`
TIME=`expr $DONETIME - $BEGINTIME`
echo "[giga.sh] build time: ($TIME s) done" >> _full/giga/build.log	
echo "[giga.sh] giga: "`ls -1 _full/giga` >> _full/giga/build.log
echo "[giga.sh] giga/streams: "`ls -1 _full/giga/streams` >> _full/giga/build.log

if [ "$buildtype" != "full" ];then
	cp -a _full/giga/* /var/www/atemio/web/mediathek/giga
fi

rm cache.*

exit
