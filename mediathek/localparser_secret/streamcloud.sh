#!/bin/sh
# first sh box parser for titannit mfg obi

. /tmp/localhoster/hoster.sh

#SRC=$1
#INPUT=$2
#PAGE=$3
#NEXT=$4
#
#FILENAME=`echo $PAGE | tr '/' '.'`
#FILENAME=`echo $FILENAME | tr '&' '.'`
#
#if [ -z "$PAGE" ]; then
#	FILENAME=none
#fi

SRC=$1
INPUT=$2
PAGE=$3
NEXT=$4
PAGE2=$5
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`

FILENAME="$PARSER $INPUT $PAGE $NEXT $PAGE2"
FILENAME=`echo $FILENAME  | sed -e 's/\-\+/./g' | sed -e 's/\+\+/./g' | tr '&' '.' | tr '/' '.' | tr '?' '.'  | tr '=' '.' | sed 's/ \+/./g' | sed 's/\.\+/./g'`

if [ -z "$FILENAME" ]; then
	FILENAME=none
fi

URL=http://streamcloud.me
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME=Streamcloud.me

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
	echo "Kino#$SRC $SRC kino '/'#http://atemio.dyndns.tv/mediathek/menu/kino.jpg#kino.jpg#$NAME#0" >$TMP/$FILENAME.list
	echo "New#$SRC $SRC search '?c=movie&m=filter&keyword=&res=&genre=&cast=&year=&order_by=releases&series=&date=year&country=&per_page=' 0#http://atemio.dyndns.tv/mediathek/menu/all-newfirst.jpg#all-newfirst.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Updates#$SRC $SRC search '?c=movie&m=filter&keyword=&res=&genre=&cast=&year=&order_by=updates&series=&date=year&country=&per_page=' 0#http://atemio.dyndns.tv/mediathek/menu/Movies.updates.jpg#Movies.updates.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Beliebt#$SRC $SRC search '?c=movie&m=filter&keyword=&res=&genre=&cast=&year=&order_by=views&series=&date=year&country=&per_page=' 0#http://atemio.dyndns.tv/mediathek/menu/most.viewed.jpg#most.viewed.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Top Imdb#$SRC $SRC search '?c=movie&m=filter&keyword=&res=&genre=&cast=&year=&order_by=rating&series=&date=year&country=&per_page=' 0#http://atemio.dyndns.tv/mediathek/menu/top.100.filme.jpg#top.100.filme.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Genre#$SRC $SRC genre#http://atemio.dyndns.tv/mediathek/menu/Movies.genre.jpg#Movies.genre.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Year#$SRC $SRC year#http://atemio.dyndns.tv/mediathek/menu/movie.year.jpg#movie.year.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "$TMP/$FILENAME.list"
}

year()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin $URL -o $TMP/cache.$FILENAME.1
		watchlist=`cat $TMP/cache.$FILENAME.1 | grep year | sed "s#/year/#\nyear='#g" | grep ^year | cut -d"'" -f2 | sort -ur`

		rm $TMP/$FILENAME.list > /dev/null 2>&1

		for ROUND0 in $watchlist; do
			TITLE=`echo $ROUND0`
			filename=`echo $TITLE | tr [A-Z] [a-z]`
			echo "$TITLE#$SRC $SRC search '?c=movie&m=filter&keyword=&res=&genre=&cast=&year=$ROUND0&order_by=views&series=&date=&country=&per_page=' 0#http://atemio.dyndns.tv/mediathek/menu/$filename.jpg#$filename.jpg#$NAME#0" >> $TMP/$FILENAME.list
		done
	fi
  	echo "$TMP/$FILENAME.list"
}


genre()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		watchlist="
			?c=movie&m=filter&order_by=views&genre=Action
			?c=movie&m=filter&order_by=views&genre=Abenteuer
			?c=movie&m=filter&order_by=views&genre=Animation
			?c=movie&m=filter&order_by=views&genre=Biographie
			?c=movie&m=filter&order_by=views&genre=Komödie
			?c=movie&m=filter&order_by=views&genre=Krimi
			?c=movie&m=filter&order_by=views&genre=Drama
			?c=movie&m=filter&order_by=views&genre=Familie
			?c=movie&m=filter&order_by=views&genre=Fantasy
			?c=movie&m=filter&order_by=views&genre=Historie
			?c=movie&m=filter&order_by=views&genre=Horror
			?c=movie&m=filter&order_by=views&genre=Mystery
			?c=movie&m=filter&order_by=views&genre=Romantik
			?c=movie&m=filter&order_by=views&genre=Science%20Fiction
			?c=movie&m=filter&order_by=views&genre=Sport
			?c=movie&m=filter&order_by=views&genre=Thriller
			?c=movie&m=filter&order_by=views&genre=Krieg
			?c=movie&m=filter&order_by=views&genre=Western
			"
		rm $TMP/$FILENAME.list > /dev/null 2>&1

		for ROUND0 in $watchlist; do
			TITLE=`echo $ROUND0 | cut -d"=" -f5 | sed 's/.html//'`
			NUMMER=`echo $ROUND0 | cut -d"-" -f3`

			filename=`echo $TITLE | tr [A-Z] [a-z]`	
			echo "$TITLE#$SRC $SRC search '$ROUND0&cast=&year=&order_by=views&series=&date=&country=&per_page=' 0#http://atemio.dyndns.tv/mediathek/menu/$filename.jpg#$filename.jpg#$NAME#0" >> $TMP/$FILENAME.list
		done
	fi
  	echo "$TMP/$FILENAME.list"
}

sorted()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		watchlist="0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z"

		rm $TMP/$FILENAME.list > /dev/null 2>&1

		for ROUND0 in $watchlist; do
			TITLE=`echo $ROUND0`
			filename=`echo $TITLE | tr [A-Z] [a-z]`
			echo "$TITLE#$SRC $SRC search 'movies-all-$ROUND0-' 1 '.html'#http://atemio.dyndns.tv/mediathek/menu/$filename.jpg#$filename.jpg#$NAME#0" >> $TMP/$FILENAME.list
		done
	fi
  	echo "$TMP/$FILENAME.list"
}

search()
{
#	rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
#	rm $TMP/$FILENAME.list
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		piccount=0

		$curlbin $URL/$PAGE$NEXT$PAGE2 -o $TMP/cache.$FILENAME.1
		pages=`cat $TMP/cache.$FILENAME.1 | sed 's/href/\nfound=/g'| grep ^found | grep "Last &rsaquo" | sed -nr 's/.*per_page=([^=]+)">.*/\1/p'`

		if [ -z "$pages" ];then
			pages=0
		else
			pages=`expr $pages / 10` 
		fi

		cat $TMP/cache.$FILENAME.1 | tr '\n' ' ' | sed 's!<a class="clip-link"!\nfound=!g' | grep ^"found=" >$TMP/cache.$FILENAME.2

		while read -u 3 ROUND; do
			TITLE=$(echo $ROUND | sed -nr 's/.*data-id="5010" title="([^"]+)".*/\1/p')
			if [ -z "$TITLE" ];then
				TITLE=$(echo $ROUND | sed -nr 's/.*alt="([^"]+)".*/\1/p')
			fi
#			NEWPAGE=$(echo $ROUND | sed -nr 's/.*href="([^"]+)".*/\1/p')
			NEWPAGE=$(echo $ROUND | sed 's/href=/\nfoundlink=/' | grep ^foundlink= | cut -d'"' -f2 | tail -n1)
			PIC=$(echo $ROUND | sed -nr 's/.*<img src="([^"]+)".*/\1/p')
	
	
			if [ -z "$PIC" ]; then
				PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
			fi
	
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/\C3x/g' -e 's/&ndash;/-/g' -e 's/&Auml;/\C3/g' -e 's/&Uuml;/\C3S/g' -e 's/&Ouml;/\C3/g' -e 's/&auml;/\E4/g' -e 's/&uuml;/\FC/g' -e 's/&ouml;/\F6/g' -e 's/&eacute;/\E9/g' -e 's/&egrave;/\E8/g' -e 's/%F6/\F6/g' -e 's/%FC/\FC/g' -e 's/%E4/\E4/g' -e 's/%26/&/g' -e 's/%C4/\C3/g' -e 's/%D6/\C3/g' -e 's/%DC/\C3S/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
#	echo TITLE $TITLE
			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
				if [ ! -e $TMP/$FILENAME.list ];then
					touch $TMP/$FILENAME.list
				fi
				piccount=`expr $piccount + 1`
				LINE="$TITLE $LANG#$SRC $SRC hosterlist $NEWPAGE#$PIC#$PARSER.$INPUT.$NEXT.$PAGE2.$FILENAME.$piccount.jpg#$NAME#0"
	
				echo "$LINE" >> $TMP/$FILENAME.list
			fi
	
		done 3<$TMP/cache.$FILENAME.2

		#work
		if [ "$NEXT" == 0 ];then
			NEXT=1
		fi

		if [ "$NEXT" -lt "$pages" ]; then
			NEXTPAGE=`expr $NEXT + 10`
			NEXT=`expr $NEXT + 1`

#			LINE="Page ($NEXTPAGE/$pages)#$SRC $SRC search '$PAGE' $NEXTPAGE '$PAGE2'#http://atemio.dyndns.tv/mediathek/menu/next.jpg#next.jpg#$NAME#0"
			LINE="Page ($NEXT/$pages)#$SRC $SRC search '$PAGE' $NEXTPAGE#http://atemio.dyndns.tv/mediathek/menu/next.jpg#next.jpg#$NAME#0"

			echo "$LINE" >> $TMP/$FILENAME.list
		fi
		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi
	echo "$TMP/$FILENAME.list"
}

kino()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		piccount=0
		$curlbin "$URL/$PAGE" -o "$TMP/cache.$FILENAME.1"
		cat $TMP/cache.$FILENAME.1 | tr '\n' ' ' | sed "s!</li></a><a!\nfound=!g" | grep ^found= >$TMP/cache.$FILENAME.2

		while read -u 3 ROUND; do
			ID=$(echo $ROUND | sed -nr "s/.*-stream-([^-]+).html.*/\1/p")
			PIC=$(echo $ROUND | grep $ID.jpg | sed -nr "s/.*<img src='([^']+)'.*/\1/p")
			NEWPAGE=$(echo $ROUND | sed -nr "s/.*href='([^']+)'.*/\1/p")
#			TITLE=$(echo $ROUND | sed -nr "s/.*> <span title='([^']+)style=.*/\1/p")
			TITLE=$(echo $ROUND | sed "s!<span title='!\nfound=!g" | sed "s!' style=!\n!g" | grep ^found= | grep -v href | cut -d"=" -f2 | head -n1)

			if [ -z "$PIC" ]; then
				PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
			fi

			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/\C3x/g' -e 's/&ndash;/-/g' -e 's/&Auml;/\C3/g' -e 's/&Uuml;/\C3S/g' -e 's/&Ouml;/\C3/g' -e 's/&auml;/\E4/g' -e 's/&uuml;/\FC/g' -e 's/&ouml;/\F6/g' -e 's/&eacute;/\E9/g' -e 's/&egrave;/\E8/g' -e 's/%F6/\F6/g' -e 's/%FC/\FC/g' -e 's/%E4/\E4/g' -e 's/%26/&/g' -e 's/%C4/\C3/g' -e 's/%D6/\C3/g' -e 's/%DC/\C3S/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`

			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
				if [ ! -e $TMP/$FILENAME.list ];then
					touch $TMP/$FILENAME.list
				fi
				piccount=`expr $piccount + 1`
				LINE="$TITLE#$SRC $SRC hosterlist $NEWPAGE#$PIC#$PARSER.$INPUT.$NEXT.$PAGE2.$FILENAME.$piccount.jpg#$NAME#0"

				echo "$LINE" >> $TMP/$FILENAME.list
			fi

		done 3<$TMP/cache.$FILENAME.2
		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi
	echo "$TMP/$FILENAME.list"
}

hosterlist()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin "$PAGE" -o $TMP/cache.$FILENAME.1
		cat $TMP/cache.$FILENAME.1 | grep iframe | sed -nr 's/.*src="([^"]+)".*/\1/p' >$TMP/cache.$FILENAME.2

		while read -u 3 ROUND; do
			NEWPAGE="$ROUND"
			TITLE=`echo $ROUND | sed -nr 's/.*:\/\/([^\/]+)\/.*/\1/p'` 		
			$curlbin "$NEWPAGE" -o $TMP/cache.$FILENAME.4

#			<a class='play_container' href='http://www.vodlocker.to/embed?t=Die+Eisk%C3%B6nigin%3A+Olaf+taut+auf&y=2017&lang=de&referrer=link'>
			cat $TMP/cache.$FILENAME.4 | grep play_container | sed -nr "s/.*href='([^']+)'.*/\1/p" >$TMP/cache.$FILENAME.4.url1
			URL=`cat $TMP/cache.$FILENAME.4.url1`
			if [ ! -z "$URL" ];then
				TITLE=`echo $URL | sed -nr 's/.*:\/\/([^\/]+)\/.*/\1/p'` 		
				NEWPAGE=$URL
			fi

			if [ ! -z "$URL" ];then
				$curlbin "$URL" -o $TMP/cache.$FILENAME.5

#				$("#hostname").html("<a href='http://openload.co/embed/vq1HFMJ5vAo/detroit.SD-spectre.mkv' target='_blank' style='color:gold; text-decoration:underline;'>openload.co</a>");
				cat $TMP/cache.$FILENAME.5 | grep "#hostname" | sed -nr "s/.*a href='([^']+)'.*/\1/p" | grep -v "+link+" >$TMP/cache.$FILENAME.5.url1
				URL=`cat $TMP/cache.$FILENAME.5.url1`
				if [ ! -z "$URL" ];then
					TITLE=`echo $URL | sed -nr 's/.*:\/\/([^\/]+)\/.*/\1/p'` 		
				fi 		

				if [ ! -z "$URL" ];then
					TITLE=`echo $URL | sed -nr 's/.*:\/\/([^\/]+)\/.*/\1/p'` 		
					NEWPAGE=$URL
				fi
			fi

			if [ ! -z "$TITLE" ] && [ "$TITLE" != " " ] && [ ! -z "$NEWPAGE" ];then
				PIC=`echo $TITLE | tr [A-Z] [a-z] | sed 's/www.//' | cut -d"." -f1 | sed 's/streamclou/streamcloud/'`
				LINE="$TITLE#$SRC $SRC hoster '$NEWPAGE'#http://atemio.dyndns.tv/mediathek/menu/$PIC.jpg#$PIC.jpg#$NAME#111"

				echo "$LINE" >> $TMP/$FILENAME.list
			fi
		done 3<$TMP/cache.$FILENAME.2
		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi
	echo "$TMP/$FILENAME.list"
}


hoster()
{
	STREAMURL="$PAGE"
	echo $STREAMURL
}


case $INPUT in
	init) $INPUT;;
	mainmenu) $INPUT;;
	hosterlist) $INPUT;;
	hoster) $INPUT;;
	search) $INPUT;;
	kino) $INPUT;;
	genre) $INPUT;;
	year) $INPUT;;
esac
