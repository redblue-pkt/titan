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
	tagname=`echo $ROUND0 | tr '/' '-'`
			
	for ROUND1 in $tags; do
		count=`expr $count + 1`
		$wgetbin http://www.giga.de/$ROUND1 -O cache.giga."$filename"."$count"

		LIST=`cat cache.giga."$filename"."$count" | tr '\n' '\r' |  tr '\r' ' ' |  tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!<noscript>!\n!g' | sed 's!<figure>!\n!g' | sed 's!</i>!\n!g' | grep ^" <img alt="  | sed 's/ \+/~/g'`
		LIST=`cat cache.giga."$filename"."$count" | grep "<h1><a href=" | cut -d'"' -f2`

echo LIST $LIST
ls cache.giga."$filename"."$count"
#exit
		for ROUND2 in $LIST; do
			count=`expr $count + 1`
			URL=$ROUND2

			$wgetbin $URL -O cache.giga."$filename"."$count"
			ls cache.giga."$filename"."$count"
			PIC=`cat cache.giga."$filename"."$count" | grep image_src | cut -d'"' -f4`
			URL=`echo $PIC | sed 's!.jpg!-normal.mp4!'`
			TITLE=`cat cache.giga."$filename"."$count" | grep "<title>" | sed 's!<title>!title<!' | grep ^"title<" | cut -d'<' -f2`

			if [ -z "$PIC" ]; then  
				PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
			fi
	
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' # ' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`

			echo PIC $PIC
			echo URL $URL
			echo TITLE "$TITLE"

			echo "################################################"

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
		cat cache.giga.titanlist."$ROUND" | sort -u > _full/giga/streams/giga.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
		echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/giga/streams/giga."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#Giga#3 >> _full/giga/giga.a-z.list
	elif [ `cat cache.giga.titanlist | grep ^"$filename" | wc -l` -gt 0 ];then
		cat cache.giga.titanlist | grep ^"$filename" > cache.giga.titanlist."$ROUND"
		cat cache.giga.titanlist."$ROUND" | sort -u > _full/giga/streams/giga.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
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
