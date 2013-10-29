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


	if [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "german" | wc -l` -gt 0 ];then
		filename=German
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "music" | wc -l` -gt 0 ];then
		filename=Musik
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "xx" | wc -l` -gt 0 ];then
		filename=Xxx
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "adult" | wc -l` -gt 0 ];then
		filename=Xxx
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "sport" | wc -l` -gt 0 ];then
		filename=Sport
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "nba" | wc -l` -gt 0 ];then
		filename=Sport
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "nfl" | wc -l` -gt 0 ];then
		filename=Sport
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "russia" | wc -l` -gt 0 ];then
		filename=Russia
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "french" | wc -l` -gt 0 ];then
		filename=French
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "usa" | wc -l` -gt 0 ];then
		filename=Usa
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "uk" | wc -l` -gt 0 ];then
		filename=Uk
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "japan" | wc -l` -gt 0 ];then
		filename=Japan
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "webcam" | wc -l` -gt 0 ];then
		filename=Webcam
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "x Tv" | wc -l` -gt 0 ];then
		filename=Xxx
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "arabic" | wc -l` -gt 0 ];then
		filename=Arabic
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "muzica" | wc -l` -gt 0 ];then
		filename=Musik
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "israel" | wc -l` -gt 0 ];then
		filename=Israel
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "turkish" | wc -l` -gt 0 ];then
		filename=Turkish
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "czech" | wc -l` -gt 0 ];then
		filename=Czech
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "3d" | wc -l` -gt 0 ];then
		filename=3d
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "16+" | wc -l` -gt 0 ];then
		filename=Xxx
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "18+" | wc -l` -gt 0 ];then
		filename=Xxx
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "spania" | wc -l` -gt 0 ];then
		filename=Spania
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "holand" | wc -l` -gt 0 ];then
		filename=Holand
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "greek" | wc -l` -gt 0 ];then
		filename=Greek
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "hd" | wc -l` -gt 0 ];then
		filename=Hd
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "animal" | wc -l` -gt 0 ];then
		filename=Discovery
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "discovery" | wc -l` -gt 0 ];then
		filename=Discovery
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "geo" | wc -l` -gt 0 ];then
		filename=Discovery
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "outdoor" | wc -l` -gt 0 ];then
		filename=Discovery
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "planet" | wc -l` -gt 0 ];then
		filename=Discovery
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "scifi" | wc -l` -gt 0 ];then
		filename=Sci-fi
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "star trek" | wc -l` -gt 0 ];then
		filename=Sci-fi
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "sci" | wc -l` -gt 0 ];then
		filename=Sci-fi
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "korea" | wc -l` -gt 0 ];then
		filename=Korea
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "cartoon" | wc -l` -gt 0 ];then
		filename=Cartoon
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "simpson" | wc -l` -gt 0 ];then
		filename=Simpsons
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "asia" | wc -l` -gt 0 ];then
		filename=Asia
	elif [ `echo "$ROUND0" | tr 'A-Z' 'a-z' | grep "live" | wc -l` -gt 0 ];then
		filename=Live
	fi
		
	PIC=http://atemio.dyndns.tv/mediathek/menu/tv.`echo $TITLE | tr 'A-Z' 'a-z' | tr ' ' '.'`.jpg

	if [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
		piccount=`expr $piccount + 1`
		
		LINE="$TITLE#$URL#$PIC#internettv_$piccount.jpg#Internet TV#2"

		count=`expr $count + 1`
		echo "add: ($count/$MAX) $TITLE"
#		echo "add: ($count/$MAX) $TITLE" >> seclist 
#		echo LINE: $LINE
		if [ ! -z "$filename" ] && [ `cat cache.internettv."$filename".titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
			echo $LINE >> cache.internettv."$filename".titanlist
		fi
		if [ `cat cache.internettv.titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
			echo $LINE >> cache.internettv.titanlist
		fi
	fi

	if [ ! -z "$filename" ] && [ `cat cache.category.titanlist | grep ^"$filename#" | wc -l` -eq 0 ];then
		piccount=`expr $piccount + 1`
		URL="http://atemio.dyndns.tv/mediathek/internettv/streams/internettv."$filename".list"
		PIC="http://atemio.dyndns.tv/mediathek/menu/tv.`echo $filename | tr 'A-Z' 'a-z'`.jpg"
		if [ "$filename" == "Xxx" ];then
			ptype=1000
		else
			ptype=3
		fi
		LINE="$filename#$URL#$PIC#internettv_$piccount.jpg#Internet Tv#$ptype"
		echo $LINE >> cache.category.titanlist

	fi

	if [ ! -z "$filename" ];then
		cat cache.internettv."$filename".titanlist | sort -u > _full/internettv/streams/internettv."$filename".list
	fi
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

