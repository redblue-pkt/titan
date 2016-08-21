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
	watchlist="
		tag/movies-2016
		tag/movies-2015
		tag/movies-2014
		tag/movies-2013
		tag/movies-2012
		tag/movies-2011
		tag/movies-2010
		tag/movies-2009
		tag/movies-2008
		tag/movies-2007
		tag/movies-2006
		tag/movies-2005
		tag/movies-2004
		tag/movies-2003
		tag/movies-2002
		tag/movies-2001
		tag/movies-2000
		tag/movies-1999
		tag/movies-1998
		tag/movies-1997
		tag/movies-1996
		tag/movies-1995
		tag/movies-1994
		tag/movies-1993
		tag/movies-1992
		tag/movies-1991
		tag/movies-1990
		tag/movies-1989
		tag/movies-1988
		tag/movies-1987
		tag/movies-1986
		tag/movies-1985
		tag/movies-1984
		tag/movies-1983
		tag/movies-1982
		tag/movies-1981
		tag/movies-1980
		tag/movies-1979
		tag/movies-1978
		tag/movies-1977
		tag/movies-1976
		tag/movies-1975
		tag/movies-1974
		tag/movies-1973
		tag/movies-1972
		tag/movies-1971
		tag/movies-1970
		"
	rm $TMP/$PARSER.$INPUT.list > /dev/null 2>&1

	for ROUND0 in $watchlist; do
		TITLE=`echo $ROUND0 | tr '/' '-' | sed 's/tag-//g'`
		filename=`echo $TITLE | sed 's/movies-//g'`	
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
				TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' # ' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
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
			LINE="Page $NEXTPAGE#$SRC $SRC page category '$PAGE' $NEXTPAGE#http://atemio.dyndns.tv/mediathek/menu/next.jpg#next.jpg#$NAME#0"
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
	page) $INPUT;;
	hosterlist) $INPUT;;
	hoster) $INPUT;;
esac
