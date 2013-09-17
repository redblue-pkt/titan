#!/bin/bash
#

buildtype=$1
wgetbin="wget -T2 -t2 --waitretry=2"

rm cache.*
rm _liste
rm -rf _full/ard

mkdir -p _full/ard/streams
piccount=0

BEGINTIME=`date +%s`
DATENAME=`date +"%Y.%m.%d_%H.%M.%S"`
echo "[ard.sh] START (buildtype: $buildtype): $DATENAME" > _full/ard/build.log

watchlist="
New;/ard/servlet/ajax-cache/3516220/view=switch/index.html
Most~Viewed;/ard/servlet/ajax-cache/3516210/view=list/show=recent/index.html
Best~Rated;/ard/servlet/ajax-cache/3516188/view=switch/index.html
Documentaries;/ard/servlet/ajax-cache/3474718/view=switch/index.html
Movie~Highlights;/ard/servlet/ajax-cache/4585472/view=switch/index.html
"
watchlist1="
All Shows A-Z
Favorites (shows)
Categorys
Dossiers
Search
Das Erste - Live
"

for ROUND1 in $watchlist; do
	count=`expr $count + 1`
	filename=`echo $ROUND1 | cut -d ";" -f1 | tr 'A-Z' 'a-z' | tr '~' '.'`
	section=`echo $ROUND1 | cut -d ";" -f1 | tr '~' '.'`
	geturl=`echo $ROUND1 | cut -d ";" -f2`

echo ROUND1 $ROUND1
echo filename $filename
echo section $section
echo geturl $geturl
#exit
	wget http://www.ardmediathek.de$geturl -O cache.$filename.$count.html
	searchlist=`cat cache.$filename.$count.html | tr '\r' '\n' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's/ \+/~/g' | sed 's!<div~class="mt-media_item">!\n\n<div~class="mt-media_item">!g' | grep ^'<div~class="mt-media_item">'`

	for ROUND2 in $searchlist; do
		piccount=`expr $piccount + 1`
		count=`expr $count + 1`

		URL=http://www.ardmediathek.de`echo $ROUND2 | sed 's!<a~href=!\nurl=!' | grep url= | cut -d'"' -f2`
		PIC=http://www.ardmediathek.de`echo $ROUND2 | sed 's!<img~src=!\npic=!' | grep pic= | cut -d'"' -f2`		
		TITLE=`echo $ROUND2 | sed 's!alt=!\nalt=!' | grep alt= | cut -d'"' -f2 | tr '~' ' '`
		if [ -z "$TITLE" ];then
			TITLE=`echo $ROUND2 | sed 's!data-xtclib=!\ndata-xtclib=!' | grep data-xtclib= | cut -d'"' -f2 | tr '~' ' '`
		fi				
		TITLE=`echo $TITLE | sed 's/&amp;/und/g'`
		TITLE=`echo $TITLE | sed 's/&quot;/"/g'`
		TITLE=`echo $TITLE | sed 's/&uuml;/\ü/g'`
		TITLE=`echo $TITLE | sed 's/&auml;/\ä/g'`
		TITLE=`echo $TITLE | sed 's/&ouml;/\ö/g'`
		TITLE=`echo $TITLE | sed 's/&szlig;/\ß/g'`
	
		wget $URL -O cache.$filename.$count.list
	
		URL=`cat cache.$filename.$count.list | grep .mp4 | grep http | sed 's!http://!\nhttp://!g' | grep ^"http://" | cut -d'"' -f1 | tail -n1`
		
		echo TITLE $TITLE
		echo URL $URL
		echo PIC $PIC
		LINE="$TITLE#$URL#$PIC#ard_$piccount.jpg#ARD#2"
		if [ ! -z "$TITLE" ] && [ ! -z "$URL" ] && [ `cat cache.ard."$filename".titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
			echo $LINE >> cache.ard."$filename".titanlist
		fi

		if [ ! -z "$TITLE" ] && [ ! -z "$URL" ] && [ `cat cache.ard.titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
			echo $LINE >> cache.ard.titanlist
		else
			echo $LINE >> cache.ard.error.titanlist
		fi
	done
	if [ -e cache.ard."$filename".titanlist ];then
		piccount=`expr $piccount + 1`
		URL="http://atemio.dyndns.tv/mediathek/ard/streams/ard."$filename".list"
		PIC="http://atemio.dyndns.tv/mediathek/menu/"$filename".jpg"
		LINE="$section#$URL#$PIC#ard_$piccount.jpg#ARD#3"
		echo $LINE >> cache.ard.category.titanlist
		cat cache.ard."$filename".titanlist >> _full/ard/streams/ard."$filename".list
	fi
	
done

if [ "$buildtype" = "full" ];then
	cat cache.ard.titanlist | sort -u > _full/ard/streams/ard.all-sorted.list
	cat cache.ard.category.titanlist | sort -u > _full/ard/ard.category.list
	
	for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
		filename=`echo "$ROUND" | tr 'A-Z' 'a-z'`
		if [ `cat cache.ard.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
			cat cache.ard.titanlist | grep ^"$ROUND" > cache.ard.titanlist."$ROUND"
			cat cache.ard.titanlist."$ROUND" | sort -um > _full/ard/streams/ard.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
			echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/ard/streams/ard."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#ARD#3 >> _full/ard/ard.a-z.list
		elif [ `cat cache.ard.titanlist | grep ^"$filename" | wc -l` -gt 0 ];then
			cat cache.ard.titanlist | grep ^"$filename" > cache.ard.titanlist."$ROUND"
			cat cache.ard.titanlist."$ROUND" | sort -um > _full/ard/streams/ard.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
			echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/ard/streams/ard."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#ARD#3 >> _full/ard/ard.a-z.list
		fi
	done
fi

#cp cache.ard.all.titanlist _full/ard/ard.category.list
cp _full/mainmenu.list /var/www/atemio/web/mediathek
cp _full/mainmenu-ard.list /var/www/atemio/web/mediathek

cp -a _full/ard/* /var/www/atemio/web/mediathek/ard

exit