#!/bin/bash
#

rm cache.*
rm _liste
rm -rf _full/drdish
mkdir -p _full/drdish/streams
rm cache.*
touch cache.drdish.titanlist

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

#echo 1111111

for ROUND1 in $WATCHLIST; do
	count=`expr $count + 1`
	filename=`echo $ROUND1 | tr '-' '.' | tr '/' '.'`
	tagname=`echo $ROUND1 | tr '/' '\n' | tail -n1`

	wget http://www.drdish-tv.com/$ROUND1/ -O cache.drdish."$filename"."$count"
	LIST=`cat cache.drdish."$filename"."$count" | tr '\n' ' ' | sed 's/ \+/~/g' | sed 's!<div~class="videowrap">~<div~class="videocontent">~<div~class="videopic">~<a~href="videoplayer/?tx_kaltura_pi1%5Bclipid%5D=!\nlink=\&!g' | grep ^'link=&'`
	if [ $ROUND1 = "tv-programm" ];then
		LIST=`cat cache.drdish."$filename"."$count" | tr '\n' ' ' | sed 's/ \+/~/g' | sed 's!<div~class="sendebild"><a~href="videoplayer/?tx_kaltura_pi1%5Bclipid%5D=!\nlink="!g' | grep ^'link="'`
	fi

	for ROUND2 in $LIST; do
		count=`expr $count + 1`
		echo $ROUND2 > echo
		#	echo ROUND2 $ROUND2
		URL="http://medianac.nacamar.de/p/435/sp/43500/playManifest/entryId/"`echo $ROUND2 | cut -d"&" -f2`"/"
		if [ $ROUND1 = "tv-programm" ];then
			URL="http://medianac.nacamar.de/p/435/sp/43500/playManifest/entryId/"`echo $ROUND2 | cut -d'"' -f2`"/"
		fi	

		PIC=`echo $ROUND2 | sed 's!<img~src="!\npic="!g' | grep ^pic= | cut -d '"' -f2 | tail -n1`	
		TITLE=`echo $ROUND2 | sed 's!<div~class="videotitel"><h1>!\ntitle=<!g' | grep ^title= | cut -d '<' -f2 | tail -n1 | tr '~' ' '`

		if [ $ROUND1 = "tv-programm" ];then
			TITLE=`echo $ROUND2 | sed 's!<div~class="sendeinhalt">!\ntitle=<!g' | grep ^title= | cut -d '<' -f2 | tail -n1 | tr '~' ' '`
		fi
		TITLE=`echo $TITLE | sed 's/&amp;/und/g'`
		TITLE=`echo $TITLE | sed 's/&quot;/"/g'`
		TITLE=`echo $TITLE | sed 's/&uuml;/\�/g'`
		TITLE=`echo $TITLE | sed 's/&auml;/\�/g'`
		TITLE=`echo $TITLE | sed 's/&ouml;/\�/g'`
		TITLE=`echo $TITLE | sed 's/&szlig;/\�/g'`

		if [ ! -z "$TITLE" ];then
#			echo TITLE $TITLE
#			echo URL $URL
#			echo PIC $PIC
			wget $URL -O cache.drdish."$filename".streamurl."$count"
			URL=`cat cache.drdish."$filename".streamurl."$count" | grep "<media url="| cut -d'"' -f2`	
#			echo URL $URL
			if [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
				piccount=`expr $piccount + 1`
				LINE="$TITLE#$URL#$PIC#drdish_$piccount.jpg#DrDish#2"
				if [ `cat cache.drdish."$filename".titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
					echo $LINE >> cache.drdish."$filename".titanlist
				fi
				if [ `cat cache.drdish.titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
					echo $LINE >> cache.drdish.titanlist
				fi
			fi
		fi		
		echo "###################################"
	done
	if [ -e cache.drdish."$filename".titanlist ];then
		piccount=`expr $piccount + 1`
		URL="http://atemio.dyndns.tv/mediathek/drdish/streams/drdish."$filename".list"
		PIC="http://atemio.dyndns.tv/mediathek/menu/"$tagname".jpg"
		LINE="$tagname#$URL#$PIC#drdish$piccount.jpg#DrDish#3"
		echo $LINE >> cache.drdish.category.titanlist
		cat cache.drdish."$filename".titanlist >> _full/drdish/streams/drdish."$filename".list
	fi
done

cat cache.drdish.titanlist | sort -u > _full/drdish/streams/drdish.all-sorted.list
cat cache.drdish.category.titanlist | sort -u > _full/drdish/drdish.category.list

for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
	filename=`echo "$ROUND" | tr 'A-Z' 'a-z'`
	if [ `cat cache.drdish.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
		cat cache.drdish.titanlist | grep ^"$ROUND" > cache.drdish.titanlist."$ROUND"
		cat cache.drdish.titanlist."$ROUND" | sort -um > _full/drdish/streams/drdish.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
		echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/drdish/streams/drdish."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#DrDish#3 >> _full/drdish/drdish.a-z.list
	elif [ `cat cache.drdish.titanlist | grep ^"$filename" | wc -l` -gt 0 ];then
		cat cache.drdish.titanlist | grep ^"$filename" > cache.drdish.titanlist."$ROUND"
		cat cache.drdish.titanlist."$ROUND" | sort -um > _full/drdish/streams/drdish.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
		echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/drdish/streams/drdish."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#DrDish#3 >> _full/drdish/drdish.a-z.list
	fi
done
rm cache.*
#cp -a _full/drdish/* /var/www/atemio/web/mediathek/drdish
#cp -a _full/mainmenu.list /var/www/atemio/web/mediathek
#cp -a _full/mainmenu-drdish.list /var/www/atemio/web/mediathek

exit
