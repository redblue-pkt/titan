#!/bin/bash
# first sh box parser for titannit mfg obi

SRC=$1
INPUT=$2
PAGE=$3

URL=http://www.giga.de
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME=Giga

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

if [ "$SRC" = "/mnt/parser" ];then
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
	echo "$NAME ($TYPE)#$SRC $SRC mainmenu#http://openaaf.dyndns.tv/mediathek/menu/$PARSER.jpg#$PARSER.jpg#TiThek#0"
}

mainmenu()
{
	echo "Category#$SRC $SRC category#http://openaaf.dyndns.tv/mediathek/menu/category.jpg#category.jpg#$NAME#0" > /tmp/tithek/$PARSER.$INPUT.list
#	echo "Suche#$SRC $SRC suche#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#giga.jpg#$NAME#0" >> /tmp/tithek/$PARSER.$INPUT.list
	echo "/tmp/tithek/$PARSER.$INPUT.list"
}

category()
{
	WATCHLIST="tv/alle-videos"

	if [ ! -e "/tmp/tithek/$PARSER.$INPUT.list" ]; then
		for ROUND0 in $WATCHLIST; do
			tags=""
			tagcount=60
			i=1
			until [ "$i" -gt "$tagcount" ]
			do
			tags="$tags~"$ROUND0"/page/$i"
			TITLE="`echo $ROUND0 | sed 's!/! !g' | sed 's!-! !g'` Page $i"
			echo "$TITLE#$SRC $SRC page $ROUND0/page/$i#http://openaaf.dyndns.tv/mediathek/menu/page.jpg#page.jpg#$NAME#0" >> /tmp/tithek/$PARSER.$INPUT.list
			i=`expr $i + 1`
			done
		done
	fi
	echo "/tmp/tithek/$PARSER.$INPUT.list"
}

page()
{
	if [ ! -e "/tmp/tithek/$PARSER.$INPUT.list" ]; then
		$wgetbin $URL/$PAGE -O $TMP/cache.$PARSER."$filename"."1"
		cat $TMP/cache.$PARSER."$filename"."1" | tr '\r' ' ' | tr '\n' '\r' | tr '\t' ' ' | sed 's/<section/\n<section/g' | grep '^<section id="content">' | sed 's/<li id="/\n<li id="/g' | grep '^<li id="' | sed 's/&quot;/"/g' >$TMP/cache.$PARSER."$filename"."2"

		piccount=0

		while read -u 3 ROUND2; do
			PIC=`echo $ROUND2 | sed 's/<img /\n<img /g' | grep '^<img ' | cut -d'"' -f2 | cut -d'"' -f1`
			URL=`echo $ROUND2 | sed 's/iframe data-tracking-origin-url="/\niframe data-tracking-origin-url="/g' | grep '^iframe data-tracking-origin-url=' | cut -d'"' -f2 | cut -d'?' -f1`
			TITLE=`echo $ROUND2 | sed 's/ alt="/\n alt="/g' | grep '^ alt=' | cut -d'"' -f2 | cut -d'"' -f1`

			if [ -z "$PIC" ]; then
				PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
			fi

			if [ `echo $PIC | grep ".png" | wc -l` -eq 0 ];then
				PICEXT=jpg
			else
				PICEXT=png
			fi

			TITLE=`echo $TITLE | sed -e 's/ &amp; / und /g' -e 's/&amp;/ und /g' -e "s/&#8217;/'/g" -e 's/&#8211;/-/g' -e 's/&#8230;/.../g' -e 's/&#8220;/"/g' -e 's/&#8221;/"/g' -e "s/&#8216;/'/g" -e 's/#//g'`

			if [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
				piccount=`expr $piccount + 1`

				LINE="$TITLE#$URL#$PIC#$PARSER_$piccount.$PICEXT#$NAME#14"
				echo "$LINE" >> /tmp/tithek/$PARSER.$INPUT.list
			fi
		done 3<$TMP/cache.$PARSER."$filename"."2"
		rm $TMP/cache.* > /dev/null 2>&1
	fi
	echo "/tmp/tithek/$PARSER.$INPUT.list"
}

case $INPUT in
#	init) $INPUT;;
	mainmenu) $INPUT;;
	category) $INPUT;;
	page) $INPUT;;
esac

