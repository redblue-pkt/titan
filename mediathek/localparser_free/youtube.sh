#!/bin/sh
# first sh box parser for titannit mfg obi

SRC=$1
INPUT=$2
PAGE=$3
NEXT=$4
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`

URL=https://www.googleapis.com
NAME=Youtube

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
	echo "Search 10#$SRC $SRC search '/youtube/v3/search?q=%search%&regionCode=US&part=snippet&hl=en_US&key=AIzaSyAL9jCDWvRD2G5nUgBrLEgEhZTQsRvzt80&type=video&maxResults=10'#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >$TMP/$FILENAME.list
	echo "Search 50#$SRC $SRC search '/youtube/v3/search?q=%search%&regionCode=US&part=snippet&hl=en_US&key=AIzaSyAL9jCDWvRD2G5nUgBrLEgEhZTQsRvzt80&type=video&maxResults=50'#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >>$TMP/$FILENAME.list
	echo "$TMP/$FILENAME.list"
}

search()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		piccount=0
#		$curlbin "$URL/$PAGE" -o "$TMP/cache.$FILENAME.1"
		curl "$URL/$PAGE" -o "$TMP/cache.$FILENAME.1"
		cat $TMP/cache.$FILENAME.1 | tr '\n' '\r' |  tr '\r' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!"kind":!\nkind":!g' | grep ^"kind" | grep videoId >$TMP/cache.$FILENAME.2

		while read -u 3 ROUND; do
			ID=`echo $ROUND | sed 's!"videoId": !\nvideoId=!g' | grep ^videoId= | cut -d'"' -f2 | tail -n1`
			PIC=`echo $ROUND | sed 's!"url": !\nurl=!g' | grep ^url= | cut -d'"' -f2 | tail -n1`
			TITLE=`echo $ROUND | sed 's!"title": !\ntitle=!g' | grep ^title= | cut -d'"' -f2 | tail -n1`
#			URL="/get_video_info?el=leanback&cplayer=UNIPLAYER&cos=Windows&height=1080&cbr=Chrome&hl=en_US&cver=4&ps=leanback&c=TVHTML5&video_id=$ID&cbrver=40.0.2214.115&width=1920&cosver=6.1&ssl_stream=1"
#			URL="https://www.youtube.com/get_video_info?el=leanback&cplayer=UNIPLAYER&cos=Windows&height=1080&cbr=Chrome&hl=en_US&cver=4&ps=leanback&c=TVHTML5&video_id=$ID&cbrver=40.0.2214.115&width=1920&cosver=6.1&ssl_stream=1"
			NEWPAGE="https://www.youtube.com/watch?v=$ID"

			if [ -z "$TITLE" ];then
				TITLE=`echo $ROUND | cut -d"<" -f2 | cut -d">" -f2`
			fi

			if [ -z "$PIC" ]; then
				PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
			fi

			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`

			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
				if [ ! -e $TMP/$FILENAME.list ];then
					touch $TMP/$FILENAME.list
				fi
				piccount=`expr $piccount + 1`
#				LINE="$TITLE#$URL#$PIC#$FILENAME_$piccount.jpg#$NAME#14"
#				LINE="$TITLE#$SRC $SRC hoster '$NEWPAGE'#$PIC#$FILENAME.$piccount.jpg#$NAME#111"
#				LINE="$TITLE#$SRC $SRC hosterlist '$NEWPAGE'#$PIC#$FILENAME.$piccount.jpg#$NAME#0"

                desc="curl --connect-timeout 5 $NEWPAGE | sed -nr 's/.*<meta name=\"description\" content=\"([^\"]+)\".*/\1/p'"
#    			desc=`echo $ROUND | sed 's!"description": !\ndescription=!g' | grep ^description= | cut -d'"' -f2 | tail -n1 | sed 's/#/~/g'`

#				LINE="$TITLE#$SRC $SRC play '$NEWPAGE'#$PIC#$FILENAME.$piccount.jpg#$NAME#111"
				LINE="$TITLE#$SRC $SRC play '$NEWPAGE'#$PIC#$FILENAME.$piccount.jpg#$NAME#111#$desc"

				echo "$LINE" >> $TMP/$FILENAME.list
			fi

		done 3<$TMP/cache.$FILENAME.2
		rm $TMP/cache.* > /dev/null 2>&1
	fi
	echo "$TMP/$FILENAME.list"
}

hosterlist()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		/tmp/localhoster/hoster.sh youtube_dl $PAGE > $TMP/cache.$FILENAME.1
		piccount=0

		while read -u 3 ROUND; do
			TITLE=`echo $ROUND | sed 's/mime=/\nfound=\&/g' | grep ^"found=&" | cut -d'&' -f2 | sed 's#%2F#/#g'`
			PIC="http://openaaf.dyndns.tv/mediathek/menu/`echo $TITLE | tr '/' '.'`.jpg"
			NEWPAGE="$ROUND"

			if [ -z "$PIC" ] || [ "$PIC" = ".jpg" ]; then
				PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
			fi

			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
				if [ ! -e $TMP/$FILENAME.list ];then
					touch $TMP/$FILENAME.list
				fi
				piccount=`expr $piccount + 1`
				LINE="$TITLE#$SRC $SRC play '$NEWPAGE'#$PIC#$FILENAME.$piccount.jpg#$NAME#111"
				echo "$LINE" >> $TMP/$FILENAME.list
			fi

		done 3<$TMP/cache.$FILENAME.1
		rm $TMP/cache.* > /dev/null 2>&1
	fi
	echo "$TMP/$FILENAME.list"
}

play()
{
#	rm $TMP/cache.$PARSER.$INPUT.* > /dev/null 2>&1
#	/tmp/localhoster/hoster.sh get $PAGE > $TMP/cache.$PARSER.$INPUT.1
#	STREAMURL=`cat $TMP/cache.$PARSER.$INPUT.1`
#	echo $STREAMURL
	/tmp/localhoster/hoster.sh youtube_dl $PAGE
}

hoster()
{
# not used anymore
#	rm $TMP/cache.$PARSER.$INPUT.* > /dev/null 2>&1
	/tmp/localhoster/hoster.sh youtube_dl $PAGE > $TMP/cache.$PARSER.$INPUT.1
	STREAMURL=`cat $TMP/cache.$PARSER.$INPUT.1`
	echo $STREAMURL
}

case $INPUT in
	init) $INPUT;;
	mainmenu) $INPUT;;
	hoster) $INPUT;;
	hosterlist) $INPUT;;
	play) $INPUT;;
	search) $INPUT;;
esac
