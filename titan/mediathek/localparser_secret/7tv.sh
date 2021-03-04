#!/bin/sh
# first sh box parser for titannit mfg obi

SRC=$1
INPUT=$2
PAGE=$3

URL=http://www.7tv.de/
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME=7tv

case $2 in
	init)	;;
	*) 	. /tmp/localhoster/hoster.sh
	   	mkdir $TMP > /dev/null 2>&1
		FILENAME="$PARSER $INPUT $PAGE"
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
#	rm -f /mnt/network/cookies > /dev/null 2>&1
	rm -rf $TMP > /dev/null 2>&1
	echo "$NAME ($TYPE)#$SRC $SRC mainmenu#http://openaaf.dyndns.tv/mediathek/menu/$PARSER.jpg#$PARSER.jpg#TiThek#0"
}

mainmenu()
{
	echo "Search Serien#$SRC $SRC search '7tvsearch/search/query/%search%/type/format/offset/0/limit/5'#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >$TMP/$FILENAME.list
	echo "Search Clips#$SRC $SRC search '7tvsearch/search/query/%search%/type/clip/offset/0/limit/5'#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >>$TMP/$FILENAME.list
	echo "Search Ganue Folgen#$SRC $SRC search '7tvsearch/search/query/%search%/type/episode/offset/0/limit/5'#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >>$TMP/$FILENAME.list
	echo "$TMP/$FILENAME.list"
}

#13:22:35.345 T:140602909902592   DEBUG: [plugin.video.7tvneu-1.0.1](searchtext) URL-x : https://www.7tv.de/7tvsearch/search/query/big+brother/type/format/offset/0/limit/5
#suche big broter
#
#serien
#14:07:13.118 T:139913047566080   DEBUG: 'GET /7tvsearch/search/query/big+brother/type/format/offset/0/limit/5 HTTP/1.1\r\nHost: www.7tv.de\r\nConnection: close\r\nAccept-Encoding: gzip, deflate\r\n\r\n'
#
#clips
#14:08:13.114 T:139913047566080   DEBUG: 'GET /7tvsearch/search/query/big+brother/type/clip/offset/0/limit/5 HTTP/1.1\r\nHost: www.7tv.de\r\nConnection: close\r\nAccept-Encoding: gzip, deflate\r\n\r\n'
#
#ganue folgen
#14:09:21.003 T:139914294122240   DEBUG: 'GET /7tvsearch/search/query/big+brother/type/episode/offset/0/limit/5 HTTP/1.1\r\nHost: www.7tv.de\r\nConnection: close\r\nAccept-Encoding: gzip, deflate\r\n\r\n'

search()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		piccount=0

		$curlbin $URL/$PAGE$NEXT$PAGE2 -o $TMP/cache.$FILENAME.1
#		$BIN /tmp/localhoster/cloudflare.py "$URL/$PAGE$NEXT$PAGE2" > $TMP/cache.$FILENAME.1

		pages=`cat $TMP/cache.$FILENAME.1 | grep $PAGE | sed "s/$PAGE/\n$PAGE/g" | cut -d ">" -f2 | cut -d "<" -f1 | tail -n1`
		if [ -z "$pages" ];then
			pages=0
		fi
		cat $TMP/cache.$FILENAME.1 | tr '\n' ' ' | sed 's!<a href=!\n<a href=!g' | grep ^"<a href=" >$TMP/cache.$FILENAME.2

		while read -u 3 ROUND; do
			ID=`echo $ROUND | cut -d'"' -f1 | tail -n1`
			NEWPAGE=`echo $ROUND | sed -nr 's/.*<a href="([^"]+)".*/\1/p'`
			TITLE=`echo $NEWPAGE | tr '-' ' ' | sed 's!/!!'`
			PIC=`echo $ROUND | sed -nr 's/.*data-src="([^"]+)".*/\1/p'`
	
			if [ -z "$PIC" ]; then
				PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
			fi
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/\C3x/g' -e 's/&ndash;/-/g' -e 's/&Auml;/\C3/g' -e 's/&Uuml;/\C3S/g' -e 's/&Ouml;/\C3/g' -e 's/&auml;/ä/g' -e 's/&uuml;/ü/g' -e 's/&ouml;/ö/g' -e 's/&eacute;/é/g' -e 's/&egrave;/è/g' -e 's/%F6/ö/g' -e 's/%FC/ü/g' -e 's/%E4/ä/g' -e 's/%26/&/g' -e 's/%C4/\C3/g' -e 's/%D6/\C3/g' -e 's/%DC/\C3S/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
	
			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
				if [ ! -e $TMP/$FILENAME.list ];then
					touch $TMP/$FILENAME.list
				fi
				piccount=`expr $piccount + 1`

				LINE="$TITLE (ganze-folgen)#$SRC $SRC episode $NEWPAGE/ganze-folgen#$PIC#$PARSER.$INPUT.$NEXT.$PAGE2.$FILENAME.$piccount.jpg#$NAME#0"
				echo "$LINE" >> $TMP/$FILENAME.list

				LINE="$TITLE (alle-clips)#$SRC $SRC episode $NEWPAGE/alle-clips#$PIC#$PARSER.$INPUT.$NEXT.$PAGE2.$FILENAME.$piccount.jpg#$NAME#0"
				echo "$LINE" >> $TMP/$FILENAME.list
			fi
	
		done 3<$TMP/cache.$FILENAME.2
		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi
	echo "$TMP/$FILENAME.list"
}

episode()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		piccount=0

		$curlbin $URL/$PAGE$NEXT$PAGE2 -o $TMP/cache.$FILENAME.1
#		$BIN /tmp/localhoster/cloudflare.py "$URL/$PAGE$NEXT$PAGE2" > $TMP/cache.$FILENAME.1

		if [ -z "$NEXT" ];then
			NEXT=1
		fi
		pages=`cat $TMP/cache.$FILENAME.1 | grep $PAGE | sed "s/$PAGE/\n$PAGE/g" | cut -d ">" -f2 | cut -d "<" -f1 | tail -n1`
		if [ -z "$pages" ];then
			pages=0
		fi
		cat $TMP/cache.$FILENAME.1 | tr '\n' ' ' | sed 's!<a href=!\n<a href=!g' | grep ^"<a href=" | grep "teaser-img" | grep "data-track" >$TMP/cache.$FILENAME.2

		while read -u 3 ROUND; do
			ID=`echo $ROUND | cut -d'"' -f1 | tail -n1`
			NEWPAGE=`echo $ROUND | sed -nr 's/.*<a href="([^"]+)".*/\1/p'`
			TITLE=`echo $ROUND | sed -nr 's/.*title="([^"]+)".*/\1/p'`
			if [ -z "$TITLE" ]; then
				TITLE=`echo $NEWPAGE | tr '-' ' ' | sed 's!/!!'`
			fi

			TITLE=`echo $TITLE | tr '-' '\n' | tail -n3 | tr '\n' '-'`

			PIC=`echo $ROUND | sed -nr 's/.*data-src="([^"]+)".*/\1/p'`
	
			if [ -z "$PIC" ]; then
				PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
			fi

			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/\C3x/g' -e 's/&ndash;/-/g' -e 's/&Auml;/\C3/g' -e 's/&Uuml;/\C3S/g' -e 's/&Ouml;/\C3/g' -e 's/&auml;/ä/g' -e 's/&uuml;/ü/g' -e 's/&ouml;/ö/g' -e 's/&eacute;/é/g' -e 's/&egrave;/è/g' -e 's/%F6/ö/g' -e 's/%FC/ü/g' -e 's/%E4/ä/g' -e 's/%26/&/g' -e 's/%C4/\C3/g' -e 's/%D6/\C3/g' -e 's/%DC/\C3S/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
	
			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
				if [ ! -e $TMP/$FILENAME.list ];then
					touch $TMP/$FILENAME.list
				fi
				piccount=`expr $piccount + 1`

#				LINE="$TITLE#$SRC $SRC episodetvlist $NEWPAGE#$PIC#$PARSER.$INPUT.$NEXT.$PAGE2.$FILENAME.$piccount.jpg#$NAME#0"
				LINE="$TITLE#$SRC $SRC play '$NEWPAGE'#$PIC#$FILENAME.$piccount.jpg#$NAME#111"

				echo "$LINE" >> $TMP/$FILENAME.list

			fi
	
		done 3<$TMP/cache.$FILENAME.2

		PAGE=`cat $TMP/cache.$FILENAME.1 | sed -nr 's/.*data-ajax-more="([^"]+)".*/\1/p'`

		if [ ! -z "$PAGE" ];then
#			if [ "$NEXT" -lt "$pages" ]; then
				NEXTPAGE=`expr $NEXT + 1`
#				LINE="Page ($NEXTPAGE/$pages)#$SRC $SRC episodetv '$PAGE' $NEXTPAGE '$PAGE2'#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#$NAME#0"
				LINE="Page (+1)#$SRC $SRC episode '$PAGE' $NEXTPAGE '$PAGE2'#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#$NAME#0"
				echo "$LINE" >> $TMP/$FILENAME.list
#			fi
		fi
		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi
	echo "$TMP/$FILENAME.list"
}

play()
{
	/tmp/localhoster/hoster.sh youtube_dl $PAGE
}


case $INPUT in
	init) $INPUT;;
	mainmenu) $INPUT;;
	play) $INPUT;;
	search) $INPUT;;
	episode) $INPUT;;
	episodelist) $INPUT;;
esac
