#!/bin/sh
# first sh box parser for titannit mfg obi

. /tmp/localhoster/hoster.sh

SRC=$1
INPUT=$2
PAGE=$3
NEXT=$4

FILENAME=`echo $PAGE | tr '/' '.'`
FILENAME=`echo $FILENAME | tr '&' '.'`

if [ -z "$PAGE" ]; then
	FILENAME=none
fi

URL=http://meinkino.to
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME=MeinKino

mkdir $TMP > /dev/null 2>&1

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
	echo "$NAME ($TYPE)#$SRC $SRC mainmenu#http://atemio.dyndns.tv/mediathek/menu/$PARSER.jpg#$PARSER.jpg#TiThek#0"
}

mainmenu()
{
	echo "Kino#$SRC $SRC kino '/filme?order=veroeffentlichung'#http://atemio.dyndns.tv/mediathek/menu/kino.ger.jpg#kino.ger.jpg#$NAME#0" >$TMP/$PARSER.$INPUT.list
	echo "Letze Uploads#$SRC $SRC kino '/filme?order=neu'#http://atemio.dyndns.tv/mediathek/menu/last.updates.ger.jpg#last.updates.ger.jpg#$NAME#0" >>$TMP/$PARSER.$INPUT.list
#	echo "Search#$SRC $SRC search 'movies.php?list=search&search=%search%'#http://atemio.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >>$TMP/$PARSER.$INPUT.list
	echo "$TMP/$PARSER.$INPUT.list"
}

kino()
{
	if [ -e "$TMP/$PARSER.$INPUT.$NEXT.$FILENAME.list" ] ; then
		rm $TMP/$PARSER.$INPUT.$NEXT.$FILENAME.list
	fi

	piccount=0
	$curlbin "$URL/$PAGE" -o "$TMP/cache.$PARSER.$INPUT.$NEXT.$FILENAME.1"
	cat $TMP/cache.$PARSER.$INPUT.$NEXT.$FILENAME.1 | tr '\n' ' ' | sed 's/<div class="ml-description-top">/\nfound=/g' | grep ^found= >$TMP/cache.$PARSER.$INPUT.$NEXT.$FILENAME.2

	while read -u 3 ROUND; do
		PIC=`echo $ROUND | sed 's!<img src=!\nurl=!g' | grep ^url= | cut -d'"' -f2 | tail -n1`
		TITLE=`echo $ROUND | sed 's!class="ml-name">!\ntitle=<!g' | grep ^title= | cut -d'<' -f2`
		NEWPAGE=`echo $ROUND | sed 's!<a href=!\nnewpage=!g' | grep ^newpage= | cut -d'"' -f2 | head -n1 | sed "s#$URL##"`

		if [ -z "$PIC" ]; then
			PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
		fi

		TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`

		if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
			if [ ! -e $TMP/$PARSER.$INPUT.$FILENAME.list ];then
				touch $TMP/$PARSER.$INPUT.$FILENAME.list
			fi
			piccount=`expr $piccount + 1`
			LINE="$TITLE#$SRC $SRC hosterlist $NEWPAGE#$PIC#$PARSER_$piccount.jpg#$NAME#0"

			echo "$LINE" >> $TMP/$PARSER.$INPUT.$NEXT.$FILENAME.list
		fi

	done 3<$TMP/cache.$PARSER.$INPUT.$NEXT.$FILENAME.2
	rm $TMP/cache.* > /dev/null 2>&1

	echo "$TMP/$PARSER.$INPUT.$NEXT.$FILENAME.list"
}

hosterlist()
{
	if [ -e "$TMP/$PARSER.$INPUT.list" ] ; then
		rm $TMP/$PARSER.$INPUT.list
	fi
	$curlbin $URL/$PAGE -o $TMP/cache.$PARSER.$INPUT.1
	TMPURL=`cat $TMP/cache.$PARSER.$INPUT.1 | sed 's/data-url-id=/\nfound=/g' | grep ^found= | cut -d'"' -f2`

	TRAILER=`cat $TMP/cache.$PARSER.$INPUT.1 | grep youtube | cut -d'"' -f2`

#	$curlbin2 -H "X-Requested-With: XMLHttpRequest" -X POST  --referer http://meinkino.to/film/the-zero-theorem-stream-id8795 http://meinkino.to/geturl/8795
	$curlbin2 -H "X-Requested-With: XMLHttpRequest" -X POST  --referer $URL/$PAGE $TMPURL -o $TMP/cache.$PARSER.$INPUT.2

	cat $TMP/cache.$PARSER.$INPUT.2 | sed 's/link_/\nlink_/g' | grep ^link_ | sed 's/"alternative":{"/\nlink_/g' >$TMP/cache.$PARSER.$INPUT.3

	if [ ! -z "$TRAILER" ];then
		ID=`echo $TRAILER | tr '=' '\n' | tail -n1`
		TRAILERURL="https://www.youtube.com/get_video_info?el=leanback&cplayer=UNIPLAYER&cos=Windows&height=1080&cbr=Chrome&hl=en_US&cver=4&ps=leanback&c=TVHTML5&video_id=$ID&cbrver=40.0.2214.115&width=1920&cosver=6.1&ssl_stream=1"

		LINE="Youtube TRAILER#$SRC $SRC play '$TRAILERURL'#http://atemio.dyndns.tv/mediathek/menu/youtube.jpg#$youtube.jpg#$NAME#111"
		echo "$LINE" >> $TMP/$PARSER.$INPUT.list
	fi

	while read -u 3 ROUND; do
		NEWPAGE=`echo $ROUND | cut -d'"' -f3`
		TITLE=`echo $ROUND | cut -d'"' -f1` 		
		EXTRA=`echo $ROUND | sed 's/"quality"/\nextra=/g' | grep ^extra= | cut -d'"' -f2` 		
		#"quality":"480"
		if [ ! -z "$EXTRA" ];then
			TITLE="$TITLE ($EXTRA)"
		fi

		if [ ! -z "$TITLE" ] && [ "$TITLE" != " " ] && [ ! -z "$NEWPAGE" ];then
			PIC=`echo $TITLE | tr [A-Z] [a-z] | cut -d "_" -f2 | cut " " -f1`
			LINE="$TITLE#$SRC $SRC play '$NEWPAGE'#http://atemio.dyndns.tv/mediathek/menu/$PIC.jpg#$PIC.jpg#$NAME#111"
			echo "$LINE" >> $TMP/$PARSER.$INPUT.list
		fi
	done 3<$TMP/cache.$PARSER.$INPUT.3
#	rm $TMP/cache.$PARSER.$INPUT.* > /dev/null 2>&1
	echo "$TMP/$PARSER.$INPUT.list"
}

play()
{
	URL="gethoster2 $PAGE"
	echo "$URL"
}

search()
{
	echo "$URL"
}

case $INPUT in
	init) $INPUT;;
	mainmenu) $INPUT;;
	hosterlist) $INPUT;;
	play) $INPUT;;
	search) $INPUT;;
	kino) $INPUT;;
esac
