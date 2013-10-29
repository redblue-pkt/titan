#!/bin/bash
#

buildtype=$1
wgetbin="wget -T2 -t2 --waitretry=2 --no-check-certificate"

rm cache.*
rm _liste
rm -rf _full/internettv
mkdir -p _full/internettv/streams
rm cache.*

BEGINTIME=`date +%s`
DATENAME=`date +"%Y.%m.%d_%H.%M.%S"`
echo "[internettv.sh] START (buildtype: $buildtype): $DATENAME" > _full/internettv/build.log


LIST=`cat /var/www/atemio/web/mediathek/mainmenu-internettv-manuel.list | tr ' ' '~'`
MAX=`cat /var/www/atemio/web/mediathek/mainmenu-internettv-manuel.list | wc -l`

GENRES="
german:German
music:Musik
muzica:Musik
xx:Xxx
adult:Xxx
sport:Sport
nba:Sport
nfl:Sport
russia:Russia
french:French
usa:Usa
uk:Uk
japan:Japan
webcam:Webcam
arabic:Arabic
israel:Israel
turkish:Turkish
czech:Czech
3d:3d
hd:Hd
16+:Xxx
18+:Xxx
spania:Spania
holand:Holand
greek:Greek
animal:Discovery
geo:Discovery
outdoor:Discovery
planet:Discovery
sci:Sci-Fi
trek:Sci-Fi
korea:Korea
cartoon:Cartoon
simpson:Cartoon
south:Cartoon
asia:Asia
live:Live"

MAXGENRES=`echo $GENRES | tr ' ' '\n' | wc -l`

count=0
piccount=0
for ROUND0 in $LIST; do
	LIST2=`echo $ROUND0 | sed 's!#!\n!g'`
	icount=0

	filename=""
	for ROUND in $LIST2; do
		icount=`expr $icount + 1`

		if [ "$icount" = "1" ];then
			TITLE=`echo $ROUND | tr '~' ' '`
		fi

		if [ "$icount" = "2" ];then
			URL=`echo $ROUND | tr '~' ' '`
		fi
	done
count2=0
gcount=0
foundgenre=0
	for ROUND1 in $GENRES; do
	filename=""
		search=`echo $ROUND1 | cut -d ":" -f1`
		setsearch=`echo $ROUND1 | cut -d ":" -f2`
gcount=`expr $gcount + 1`
		if [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep $search | wc -l` -gt 0 ];then
			filename=$setsearch
			foundgenre=1
		elif [ "$MAXGENRES" = "$gcount" ] && [ "$foundgenre" = "0" ];then
			filename=Other		
		fi

		
		PIC=http://atemio.dyndns.tv/mediathek/menu/tv.`echo $TITLE | tr 'A-Z' 'a-z' | tr ' ' '.'`.jpg
	
		if [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
			piccount=`expr $piccount + 1`
			
			LINE="$TITLE#$URL#$PIC#internettv_$piccount.jpg#Internet TV#2"
	
	#		echo "add: ($count/$MAX) $TITLE" >> seclist 
	#		echo LINE: $LINE
			add=0
			if [ ! -z "$filename" ] && [ ! -e cat cache.internettv."$filename".titanlist ];then
				echo $LINE >> cache.internettv."$filename".titanlist
				count2=`expr $count2 + 1`
				echo "add: ($count/$MAX) ($count2) $TITLE Genre: ($filename)"
				add=1
			elif [ ! -z "$filename" ] && [ `cat cache.internettv."$filename".titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
				echo $LINE >> cache.internettv."$filename".titanlist
				count2=`expr $count2 + 1`
				echo "add: ($count/$MAX) ($count2) $TITLE Genre: ($filename)"
				add=1
			fi
			if [ `cat cache.internettv.titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
				count=`expr $count + 1`
				if [ "$add" = "0" ] && [ ! -z "$filename" ];then
					count2=`expr $count2 + 1`
					echo "2add: ($count/$MAX) ($count2) $TITLE Genre: ($filename)"
				fi
				echo $LINE >> cache.internettv.titanlist
			fi
		fi
	
		if [ ! -z "$filename" ] && [ `cat cache.category.titanlist | grep ^"$filename#" | wc -l` -eq 0 ];then
			piccount=`expr $piccount + 1`
			URL2="http://atemio.dyndns.tv/mediathek/internettv/streams/internettv."$filename".list"
			PIC="http://atemio.dyndns.tv/mediathek/menu/tv.`echo $filename | tr 'A-Z' 'a-z'`.jpg"
			if [ "$filename" == "Xxx" ];then
				ptype=1000
			else
				ptype=3
			fi
			LINE="$filename#$URL2#$PIC#internettv_$piccount.jpg#Internet Tv#$ptype"
			echo $LINE >> cache.category.titanlist
	
		fi
	
		if [ ! -z "$filename" ];then
			cat cache.internettv."$filename".titanlist | sort -u > _full/internettv/streams/internettv."$filename".list
		fi
	done
	echo "----------------------------------------------------"
done

cat cache.category.titanlist | sort -u > _full/internettv/internettv.category.list
cat cache.internettv.titanlist | sort -u > _full/internettv/streams/internettv.all-sorted.list

for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
	filename=`echo "$ROUND" | tr 'A-Z' 'a-z'`
	if [ `cat cache.internettv.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
		cat cache.internettv.titanlist | grep ^"$ROUND" >> cache.internettv.titanlist."$ROUND"
		cat cache.internettv.titanlist."$ROUND" | sort -u > _full/internettv/streams/internettv.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
		echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/internettv/streams/internettv."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#Internet TV#3 >> _full/internettv/internettv.a-z.list
	elif [ `cat cache.internettv.titanlist | grep ^"$filename" | wc -l` -gt 0 ];then
		cat cache.internettv.titanlist | grep ^"$filename" >> cache.internettv.titanlist."$ROUND"
		cat cache.internettv.titanlist."$ROUND" | sort -u > _full/internettv/streams/internettv.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
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

