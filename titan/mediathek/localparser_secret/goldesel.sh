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

URL=http://goldesel.to
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME=Goldesel

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
    echo "NEW#$SRC $SRC page category '/'#http://atemio.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#0" > $TMP/$PARSER.$INPUT.list
#	echo "Movies (Year)#$SRC $SRC movieyear#http://atemio.dyndns.tv/mediathek/menu/movie.year.jpg#movie.year.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
#	echo "Movies (Genre)#$SRC $SRC moviegenre#http://atemio.dyndns.tv/mediathek/menu/movie.genre.jpg#movie.genre.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
#	echo "Series#$SRC $SRC page category 'free/tv-series/page/' 1#http://atemio.dyndns.tv/mediathek/menu/series.jpg#series.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
#   echo "Search#$SRC $SRC page category '?s='#http://atemio.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >> $TMP/$PARSER.$INPUT.list
	echo "$TMP/$PARSER.$INPUT.list"
}

page()
{
	if [ ! -e "$TMP/$PARSER.$INPUT.$FROM.$NEXT.$FILENAME.list" ]; then
		piccount=0
		$curlbin $URL/$PAGE -o $TMP/cache.$PARSER.$FROM.$NEXT.$FILENAME.1

		cat $TMP/cache.$PARSER.$FROM.$NEXT.$FILENAME.1 | sed 's/<div class/\n<div class/g' | sed 's/<a href="/\n<a href="/g' | grep ^'<a href="film' | grep title= | sed 's/ /~/g' >$TMP/cache.$PARSER.$FROM.$NEXT.$FILENAME.2

		while read -u 3 ROUND; do
			TITLE=`echo $ROUND | sed 's/title=/\ntitle=/' | grep ^"title=" | cut -d '"' -f2 | tr '~' ' ' | sed 's/#/%/'`
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' # ' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
			PIC=$URL/`echo $ROUND | sed s'!<img~src=!\nsrc=!' | grep ^"src=" | cut -d '"' -f2 | tr '~' ' '`
			NEWPAGE=`echo $ROUND | sed 's/<a~href=/\nhref=/' | grep ^"href=" | cut -d '"' -f2`
		
			if [ -z "$PIC" ] || [ "$PIC" = "$URL/" ]; then  
				PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
			fi

			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
				if [ `cat $TMP/$PARSER.$INPUT.$FROM.$NEXT.$FILENAME.list | grep ^"$NEWPAGE" | wc -l` -eq 0 ];then
					if [ ! -e $TMP/$PARSER.$INPUT.$FROM.$NEXT.$FILENAME.list ];then
						touch $TMP/$PARSER.$INPUT.$FROM.$NEXT.$FILENAME.list
					fi
					piccount=`expr $piccount + 1`
					LINE="$TITLE#$SRC $SRC hosterlist page $NEWPAGE#$PIC#$PARSER.$FILENAME.$NEXT.$piccount.jpg#$NAME#0"
	
					echo "$LINE" >> $TMP/$PARSER.$INPUT.$FROM.$NEXT.$FILENAME.list
				fi
			fi
		done 3<$TMP/cache.$PARSER.$FROM.$NEXT.$FILENAME.2
		rm $TMP/cache.* > /dev/null 2>&1
	fi
	echo "$TMP/$PARSER.$INPUT.$FROM.$NEXT.$FILENAME.list"
}

hosterlist()
{
	rm -f "$TMP/$PARSER.$INPUT.$FROM.$NEXT.$FILENAME.list"
	piccount=0
	$curlbin $URL/$PAGE -o $TMP/cache.$PARSER.$FROM.$NEXT.$FILENAME.1
	cat $TMP/cache.$PARSER.$FROM.$NEXT.$FILENAME.1 | sed 's!<h2>Stream-Links</h2>!\nfound=!g' | sed 's!<h2>Sample-Links</h2>!\nerror=!g' | grep ^found= | sed 's/<li data=/\ndata=/g' | grep ^data= >$TMP/cache.$PARSER.$FROM.$NEXT.$FILENAME.2

	while read -u 3 ROUND; do
		NEWPAGE="data=`echo $ROUND | cut -d '"' -f2 | sed 's/;/%3B/g'`"
		TITLE=`echo $ROUND | cut -d ';' -f5 | cut -d '.' -f1`
		if [ ! -z "$TITLE" ] && [ "$TITLE" != " " ] && [ ! -z "$NEWPAGE" ];then
			PIC=`echo $TITLE | tr [A-Z] [a-z]`
			LINE="$TITLE#$SRC $SRC hoster hosterlist $NEWPAGE '--referer $URL/$PAGE'#http://atemio.dyndns.tv/mediathek/menu/$PIC.jpg#$PIC.jpg#$NAME#111"
			echo "$LINE" >> $TMP/$PARSER.$INPUT.$FROM.$NEXT.$FILENAME.list
		fi
	done 3<$TMP/cache.$PARSER.$FROM.$NEXT.$FILENAME.2
	rm $TMP/cache.* > /dev/null 2>&1
	echo "$TMP/$PARSER.$INPUT.$FROM.$NEXT.$FILENAME.list"
}

hoster()
{
	STREAMURL=`$curlbin $URL/res/links -X POST --data "$PAGE" | sed 's!url="http://dref.me/?!\nstreamurl="!' | grep ^streamurl= | cut -d'"' -f2 | sed 's!%3A!:!g' | sed 's!%2F!/!g'`
	echo $STREAMURL
}

case $INPUT in
	init) $INPUT;;
	mainmenu) $INPUT;;
	category) $INPUT;;
	page) $INPUT;;
	hosterlist) $INPUT;;
	hoster) $INPUT;;
esac
