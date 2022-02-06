#!/bin/sh
# first sh box parser for titannit mfg obi

SRC=$1
INPUT=$2
PAGE=$3
NEXT=$4

PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`

URL=http://www.mediathekdirekt.de
#PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME=MediathekDirekt

case $2 in
	init)	;;
	*) 	. /tmp/localhoster/hoster.sh
	   	mkdir $TMP > /dev/null 2>&1
		FILENAME="$PARSER $INPUT $PAGE $NEXT"
	   	FILENAME=$(echo $FILENAME | tr '&' '.' | tr '/' '.' | tr '?' '.' | tr '=' '.' | sed -e 's/\&\+/./g' -e 's#\/\+#.#g' -e 's/\?\+/./g' -e 's/;\+/./g' -e 's/=\+/./g' -e 's/ \+/./g' -e 's/\.\+/./g')
		if [ -z "$FILENAME" ]; then FILENAME=none;fi
		PICNAME="$FILENAME"
		;;
esac

if [ `echo $SRC | grep ^"/mnt/parser" | wc -l` -gt 0 ];then
	TYPE="$SRC - Shell script"
elif [ `echo $SRC | grep ^"/var/swap" | wc -l` -gt 0 ];then
	TYPE="Swap - Shell script"
elif [ `echo $SRC | grep ^"/mnt/swapextensions" | wc -l` -gt 0 ];then
	TYPE="Mnt - Shell script"
else
	TYPE="Flash - Shell script"
fi

init()
{
	rm -rf $TMP > /dev/null 2>&1
	echo "$NAME ($TYPE)#$SRC $SRC mainmenu#http://openaaf.dyndns.tv/mediathek/menu/$PARSER.jpg#$PARSER.jpg#TiThek#0"
}

mainmenu()
{
	echo "Channels#$SRC $SRC channels 'good.json'#http://openaaf.dyndns.tv/mediathek/menu/channels.jpg#channels.jpg#$NAME#0" > $TMP/$FILENAME.list
	echo "Shows#$SRC $SRC shows 'good.json'#http://openaaf.dyndns.tv/mediathek/menu/shows.jpg#shows.jpg#$NAME#0" >> $TMP/$FILENAME.list
	echo "Date#$SRC $SRC date 'good.json'#http://openaaf.dyndns.tv/mediathek/menu/date.jpg#date.jpg#$NAME#0" >> $TMP/$FILENAME.list
	echo "ALL#$SRC $SRC videos 'good.json'#http://openaaf.dyndns.tv/mediathek/menu/all.jpg#all.jpg#$NAME#0" >> $TMP/$FILENAME.list
	echo "$TMP/$FILENAME.list"
} 

channels1()
{
	watchlist="
		3Sat
		ARD
		ARTE.DE
		ARTE.FR
		BR
		DW
		HR
		KiKA
		MDR
		NDR
		ORF
		PHOENIX
		RBB
		SR
		SRF
		SRF.Podcast
		SWR
		WDR
		ZDF
		"
	rm $TMP/$FILENAME.list > /dev/null 2>&1

	for ROUND0 in $watchlist; do
		TITLE=`echo $ROUND0 | tr '/' '-' | sed 's/-filme//g'`
		filename=`echo $TITLE | tr 'A-Z' 'a-z'`	
		echo "$TITLE#$SRC $SRC videosgrep 'good.json' '$ROUND0'#http://openaaf.dyndns.tv/mediathek/menu/$filename.jpg#$filename.jpg#$NAME#0" >> $TMP/$FILENAME.list
	done
  	echo "$TMP/$FILENAME.list"
}

date()
{
#curl http://www.mediathekdirekt.de/good.json | tr '[' '\n'|  cut -d"," -f4 | grep -v [a-z] | grep -v [A-Z] | grep "[0.9].[0.9].[0.9]" | sort -u | sort -r -k 1.10 -k 1.6 | sed '1,1d'
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin --compressed $URL/$PAGE | tr '[' '\n'|  cut -d"," -f4 | grep -v [a-z] | grep -v [A-Z] | grep "[0.9].[0.9].[0.9]" | sort -u | sort -r -k 1.10 -k 1.6 | sed '1,1d' > $TMP/cache.$FILENAME.1

		rm $TMP/$FILENAME.list > /dev/null 2>&1
		while read -u 3 ROUND; do
			TITLE=`echo $ROUND | cut -d'"' -f2`
			filename=`echo $TITLE | tr 'A-Z' 'a-z'`	
			echo "$TITLE#$SRC $SRC videosgrep 'good.json' $ROUND#http://openaaf.dyndns.tv/mediathek/menu/$filename.jpg#$filename.jpg#$NAME#0" >> $TMP/$FILENAME.list
		done 3<$TMP/cache.$FILENAME.1
	fi
  	echo "$TMP/$FILENAME.list"
}

channels()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin --compressed -o - $URL/$PAGE | tr '[' '\n' | awk '{ gsub(/\\"/, "\x27"); print }'  | grep ^'"' | awk '{ gsub(/\", \"/, "\" ; \""); print }' | cut -d ";" -f1 | sort -u > $TMP/cache.$FILENAME.1
		rm $TMP/$FILENAME.list > /dev/null 2>&1
	
		while read -u 3 ROUND; do
			TITLE=`echo $ROUND | cut -d'"' -f2`
			filename=`echo $TITLE | tr 'A-Z' 'a-z'`		
			echo "$TITLE#$SRC $SRC videosgrep 'good.json' $ROUND#http://openaaf.dyndns.tv/mediathek/menu/$filename.jpg#$filename.jpg#$NAME#0" >> $TMP/$FILENAME.list
		done 3<$TMP/cache.$FILENAME.1
	fi
  	echo "$TMP/$FILENAME.list"
}

shows()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin --compressed -o - $URL/$PAGE | tr '[' '\n' | awk '{ gsub(/\\"/, "\x27"); print }'  | grep ^'"' | awk '{ gsub(/\", \"/, "\" ; \""); print }' | cut -d ";" -f3 | sort -u | grep ^' "' > $TMP/cache.$FILENAME.1
		rm $TMP/$FILENAME.list > /dev/null 2>&1
	
		while read -u 3 ROUND; do
			TITLE=`echo $ROUND | cut -d'"' -f2`
			filename=`echo $TITLE | tr 'A-Z' 'a-z'`	
			echo "$TITLE#$SRC $SRC videosgrep 'good.json' $ROUND#http://openaaf.dyndns.tv/mediathek/menu/$filename.jpg#$filename.jpg#$NAME#0" >> $TMP/$FILENAME.list
		done 3<$TMP/cache.$FILENAME.1
	fi
  	echo "$TMP/$FILENAME.list"
}

videosgrep()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin --compressed -o - $URL/$PAGE | tr '[' '\n' | grep \""$NEXT"\" | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME \
		'
			# BEGIN variable setzen
			BEGIN
			{
				# setzt suchvariable auf 0 vor dem start
			}
			# eindeutige zeile vor ersten treffer
			/^\"/ \
			{
				# print "111111" $0
				gsub(/\\"/, "\x27", $0, $0)

				# print "222222" $0

				split($0, a, "\"")
            	pic = "http://openaaf.dyndns.tv/mediathek/menu/default.jpg"

				sender = a[2]
				title = a[4]
				date = a[8]
				duration = a[10]
				stream = a[14]
				print sender " - " title " (" date ") (" duration ")#" stream "#" pic "#" PICNAME ".jpg#" NAME "#2"

				next
			}
		# schreibe alles in die list datei
		' >$TMP/$FILENAME.list
	fi
	# gebe titan den list namen mit pfad zurueck
	echo "$TMP/$FILENAME.list"
}

videos()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin --compressed -o - $URL/$PAGE | tr '[' '\n' | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME \
		'
			# BEGIN variable setzen
			BEGIN
			{
				# setzt suchvariable auf 0 vor dem start
			}
			# eindeutige zeile vor ersten treffer
			/^\"/ \
			{
				# print "111111" $0
				gsub(/\\"/, "\x27", $0, $0)

				# print "222222" $0

				split($0, a, "\"")
				# print "a1" a[1]
				# print "a2" a[2]
				# print "a3" a[3]
				# print "a4" a[4]
				# print "a5" a[5]
				# print "a6" a[6]
				# print "a7" a[7]
				# print "a8" a[8]
				# print "a9" a[9]
				# print "a10" a[10]
				# print "a11" a[11]
				# print "a12" a[12]
				# print "a13" a[13]
				# print "a14" a[14]
				# print "a15" a[15]
				
            	pic = "http://openaaf.dyndns.tv/mediathek/menu/default.jpg"

				sender = a[2]
				title = a[4]
				date = a[8]
				duration = a[10]
				stream = a[14]
				print sender " - " title " (" date ") (" duration ")#" stream "#" pic "#" PICNAME ".jpg#" NAME "#2"

				next
			}
		# schreibe alles in die list datei
		' >$TMP/$FILENAME.list
	fi
	# gebe titan den list namen mit pfad zurueck
	echo "$TMP/$FILENAME.list"
}

case $INPUT in
#	init) $INPUT;;
	mainmenu) $INPUT;;
	channels) $INPUT;;
	date) $INPUT;;
	videos) $INPUT;;
	videosgrep) $INPUT;;
	shows) $INPUT;;
esac
