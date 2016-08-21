#!/bin/sh
# first sh box parser for titannit mfg obi

. /tmp/localhoster/hoster.sh

SRC=$1
INPUT=$2
FROM=$3
PAGE=$4
NEXT=$5

FILENAME=`echo $PAGE | tr '/' '.'`
FILENAME=`echo $FILENAME | tr '&' '.'`

if [ -z "$PAGE" ]; then
	FILENAME=none
fi

URL=http://www.thesolarmovie.me
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME=TheSolarMovie

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
	echo "Category#$SRC $SRC category#http://atemio.dyndns.tv/mediathek/menu/category.jpg#category.jpg#$NAME#0" > $TMP/$PARSER.$INPUT.list
	echo "$TMP/$PARSER.$INPUT.list"
}

category()
{
	echo "Movies (Year)#$SRC $SRC movieyear#http://atemio.dyndns.tv/mediathek/menu/movie.year.jpg#movie.year.jpg#$NAME#0" > $TMP/$PARSER.$INPUT.list
	echo "Movies (Genre)#$SRC $SRC moviegenre#http://atemio.dyndns.tv/mediathek/menu/movie.genre.jpg#movie.genre.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "Series#$SRC $SRC page category 'free/tv-series/page/' 1#http://atemio.dyndns.tv/mediathek/menu/series.jpg#series.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "$TMP/$PARSER.$INPUT.list"
}

movieyear()
{
	rm $TMP/$PARSER.$INPUT.list > /dev/null 2>&1

	WATCHLIST="tag/movies"

	if [ ! -e "/tmp/tithek/$PARSER.$INPUT.list" ]; then
		for ROUND0 in $WATCHLIST; do
			tags=""
			tagcount=`date +"%Y"`
			i=1950
			until [ "$i" -gt "$tagcount" ]
			do
			echo "$i#$SRC $SRC page category '$ROUND0-$i/page/' 1#http://atemio.dyndns.tv/mediathek/menu/$i.jpg#$i.jpg#$NAME#0" | sort -r >> $TMP/$PARSER.$INPUT.list
			i=`expr $i + 1` 
			done
		done
	fi

  	echo "$TMP/$PARSER.$INPUT.list"
}

moviegenre()
{
	watchlist="
		free/action
		free/adventure
		free/animation
		free/biography
		free/comedy
		free/crime
		free/documentary
		free/drama
		free/family
		free/history
		free/horror
		free/music
		free/mystery
		free/romance
		free/sci-fi
		free/sport
		free/thriller
		free/war
		free/western
		"
	rm $TMP/$PARSER.$INPUT.list > /dev/null 2>&1

	for ROUND0 in $watchlist; do
		TITLE=`echo $ROUND0 | tr '/' '-' | sed 's/free-//g'`
		filename=`echo $TITLE`	
		echo "$TITLE#$SRC $SRC page category '$ROUND0/page/' 1#http://atemio.dyndns.tv/mediathek/menu/$filename.jpg#$filename.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	done
  	echo "$TMP/$PARSER.$INPUT.list"
}

page()
{
	if [ ! -e "$TMP/$PARSER.$INPUT.$FROM.$NEXT.$FILENAME.list" ]; then
		piccount=0
		$curlbin $URL/$PAGE/$NEXT -o $TMP/cache.$PARSER.$FROM.$NEXT.$FILENAME.1
		cat $TMP/cache.$PARSER.$FROM.$NEXT.$FILENAME.1 | tr '\n' '\r' |  tr '\r' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's/<a class="coverImage"/\nfound=/g' | grep ^found=  | sed 's/data-original/\r\n/g' | tr ' ' '~' >$TMP/cache.$PARSER.$FROM.$NEXT.$FILENAME.2

		pages=`cat $TMP/cache.$PARSER.$FROM.$NEXT.$FILENAME.1 | grep "<span class='pages'>" | sed 's! of !\npage<!' | grep ^"page<" | cut -d'<' -f2`
		while read -u 3 ROUND; do
			TITLE=`echo $ROUND | cut -d '"' -f2 | tr '~' ' ' | sed 's/#/%/'`
			if [ `echo $TITLE | grep ^"//" | wc -l` -eq 0 ];then
				TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' # ' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/�x/g' -e 's/&ndash;/-/g' -e 's/&Auml;/�/g' -e 's/&Uuml;/�S/g' -e 's/&Ouml;/�/g' -e 's/&auml;/ä/g' -e 's/&uuml;/ü/g' -e 's/&ouml;/ö/g' -e 's/&eacute;/é/g' -e 's/&egrave;/è/g' -e 's/%F6/ö/g' -e 's/%FC/ü/g' -e 's/%E4/ä/g' -e 's/%26/&/g' -e 's/%C4/�/g' -e 's/%D6/�/g' -e 's/%DC/�S/g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
				NEWPAGE=`echo $ROUND | sed s'!href=!\nhref=!' | grep ^"href=" | cut -d '"' -f2 | tr '~' ' '`
				PIC=http:`echo $ROUND | sed s'!src=!\nsrc=!' | grep ^"src=" | cut -d '"' -f2 | tr '~' ' '`
			
				if [ -z "$PIC" ] || [ "$PIC" = "http:" ]; then  
					PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
				fi
			
				if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
					if [ ! -e $TMP/$PARSER.$INPUT.$FROM.$NEXT.$FILENAME.list ];then
						touch $TMP/$PARSER.$INPUT.$FROM.$NEXT.$FILENAME.list
					fi
					piccount=`expr $piccount + 1`
					LINE="$TITLE#$SRC $SRC hosterlist page $NEWPAGE#$PIC#$PARSER.$FILENAME.$NEXT.$piccount.jpg#$NAME#0"

					echo "$LINE" >> $TMP/$PARSER.$INPUT.$FROM.$NEXT.$FILENAME.list
				fi
			fi
		done 3<$TMP/cache.$PARSER.$FROM.$NEXT.$FILENAME.2

		if [ "$NEXT" -lt "$pages" ]; then
			NEXTPAGE=`expr $NEXT + 1`
			LINE="Page ($NEXTPAGE/$pages)#$SRC $SRC page category '$PAGE' $NEXTPAGE#http://atemio.dyndns.tv/mediathek/menu/next.jpg#next.jpg#$NAME#0"
			echo "$LINE" >> $TMP/$PARSER.$INPUT.$FROM.$NEXT.$FILENAME.list
		fi
		rm $TMP/cache.* > /dev/null 2>&1
	fi
	echo "$TMP/$PARSER.$INPUT.$FROM.$NEXT.$FILENAME.list"
}

hosterlist()
{
	piccount=0
	$curlbin $PAGE -o $TMP/cache.$PARSER.$FROM.$NEXT.$FILENAME.1
	cat $TMP/cache.$PARSER.$FROM.$NEXT.$FILENAME.1 | tr '\n' '\r' |  tr '\r' ' ' | tr '\n' ' ' | sed 's/<tr id="link_/\nfound=/g' | sed 's!</tbody>!\n!g' | grep ^found= >$TMP/cache.$PARSER.$FROM.$NEXT.$FILENAME.2

	while read -u 3 ROUND; do
		NEWPAGE=`echo $ROUND | sed s'!go.php?url=!\nurl="!' | grep ^'url="' | cut -d '"' -f2`
		TITLE=`echo $ROUND | sed s'!target="_blank">!\nurl=<!' | grep ^'url=<' | cut -d '<' -f2`
		if [ ! -z "$TITLE" ] && [ "$TITLE" != " " ] && [ ! -z "$NEWPAGE" ];then
			PIC=`echo $TITLE | tr [A-Z] [a-z]`
			LINE="$TITLE#$SRC $SRC hoster page $NEWPAGE $NEWPAGE#http://atemio.dyndns.tv/mediathek/menu/$PIC#$PIC.jpg#$NAME#111"
			echo "$LINE" >> $TMP/$PARSER.$INPUT.$FROM.$NEXT.$FILENAME.list
		fi
	done 3<$TMP/cache.$PARSER.$FROM.$NEXT.$FILENAME.2
	rm $TMP/cache.* > /dev/null 2>&1
	echo "$TMP/$PARSER.$INPUT.$FROM.$NEXT.$FILENAME.list"
}

hoster()
{
	echo $PAGE
}

case $INPUT in
	init) $INPUT;;
	mainmenu) $INPUT;;
	category) $INPUT;;
	moviegenre) $INPUT;;
	movieyear) $INPUT;;
	page) $INPUT;;
	hosterlist) $INPUT;;
	hoster) $INPUT;;
esac
