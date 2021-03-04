#!/bin/sh
# first sh box parser for titannit mfg obi

SRC=$1
INPUT=$2
PAGE=$3
NEXT=$4
PAGE2=$5
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`

URL=http://streamcloud.me
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME=Streamcloud.me

case $2 in
	init)	;;
	*) 	. /tmp/localhoster/hoster.sh
	   	mkdir $TMP > /dev/null 2>&1
		FILENAME="$PARSER $INPUT $PAGE $NEXT $PAGE2"
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
	echo "Kino#$SRC $SRC kino '/'#http://openaaf.dyndns.tv/mediathek/menu/kino.jpg#kino.jpg#$NAME#0" >$TMP/$FILENAME.list
	echo "Neu#$SRC $SRC search '?c=movie&m=filter&keyword=&res=&genre=&cast=&year=&order_by=releases&series=&date=year&country=&per_page=' 0#http://openaaf.dyndns.tv/mediathek/menu/all-newfirst.jpg#all-newfirst.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Updates#$SRC $SRC search '?c=movie&m=filter&keyword=&res=&genre=&cast=&year=&order_by=updates&series=&date=year&country=&per_page=' 0#http://openaaf.dyndns.tv/mediathek/menu/Movies.updates.jpg#Movies.updates.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Beliebt#$SRC $SRC search '?c=movie&m=filter&keyword=&res=&genre=&cast=&year=&order_by=views&series=&date=year&country=&per_page=' 0#http://openaaf.dyndns.tv/mediathek/menu/most.viewed.jpg#most.viewed.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Top Imdb#$SRC $SRC search '?c=movie&m=filter&keyword=&res=&genre=&cast=&year=&order_by=rating&series=&date=year&country=&per_page=' 0#http://openaaf.dyndns.tv/mediathek/menu/top.100.filme.jpg#top.100.filme.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Genre#$SRC $SRC genre#http://openaaf.dyndns.tv/mediathek/menu/Movies.genre.jpg#Movies.genre.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Year#$SRC $SRC year#http://openaaf.dyndns.tv/mediathek/menu/movie.year.jpg#movie.year.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Search#$SRC $SRC searchtv '?c=movie&m=quickSearch&key=%datakey%&keyword=%search%' 0#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >>$TMP/$FILENAME.list
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
			echo "$TITLE#$SRC $SRC search '?c=movie&m=filter&keyword=&res=&genre=&cast=&year=$ROUND0&order_by=views&series=&date=&country=&per_page=' 0#http://openaaf.dyndns.tv/mediathek/menu/$filename.jpg#$filename.jpg#$NAME#0" >> $TMP/$FILENAME.list
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
			echo "$TITLE#$SRC $SRC search '$ROUND0&cast=&year=&order_by=views&series=&date=&country=&per_page=' 0#http://openaaf.dyndns.tv/mediathek/menu/$filename.jpg#$filename.jpg#$NAME#0" >> $TMP/$FILENAME.list
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
			echo "$TITLE#$SRC $SRC search 'movies-all-$ROUND0-' 1 '.html'#http://openaaf.dyndns.tv/mediathek/menu/$filename.jpg#$filename.jpg#$NAME#0" >> $TMP/$FILENAME.list
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
				PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
			fi
	
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/\C3x/g' -e 's/&ndash;/-/g' -e 's/&Auml;/\C3/g' -e 's/&Uuml;/\C3S/g' -e 's/&Ouml;/\C3/g' -e 's/&auml;/\E4/g' -e 's/&uuml;/\FC/g' -e 's/&ouml;/\F6/g' -e 's/&eacute;/\E9/g' -e 's/&egrave;/\E8/g' -e 's/%F6/\F6/g' -e 's/%FC/\FC/g' -e 's/%E4/\E4/g' -e 's/%26/&/g' -e 's/%C4/\C3/g' -e 's/%D6/\C3/g' -e 's/%DC/\C3S/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
#	echo TITLE $TITLE
			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
				if [ ! -e $TMP/$FILENAME.list ];then
					touch $TMP/$FILENAME.list
				fi
				piccount=`expr $piccount + 1`
				LINE="$TITLE#$SRC $SRC hosterlist '$NEWPAGE'#$PIC#$PARSER.$INPUT.$NEXT.$PAGE2.$FILENAME.$piccount.jpg#$NAME#0"
	
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
			if [ "$NEXT" -gt 1 ];then
				NEXTTEXT=$(expr $NEXT / 10)
			fi
			NEXTTEXT=`expr $NEXTTEXT + 1`

#			LINE="Page ($NEXTPAGE/$pages)#$SRC $SRC search '$PAGE' $NEXTPAGE '$PAGE2'#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#$NAME#0"
			LINE="Page ($NEXTTEXT/$pages)#$SRC $SRC search '$PAGE' '$NEXTPAGE'#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#$NAME#0"

			echo "$LINE" >> $TMP/$FILENAME.list
		fi

		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi
	echo "$TMP/$FILENAME.list"
}


searchtv()
{
	rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	rm $TMP/$FILENAME.list
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		piccount=0
#		FILENAME=`echo $FILENAME | sed "s/%datakey%//"`
		if [ -z "$NEXT" ]; then NEXT=0; fi

		datakey=`$curlbin $URL | sed -nr 's/.*data-key="([^"]+)".*/\1/p'`
		if [ -z "$datakey" ]; then datakey=4164OPTZ98adf546874s4; fi
		PAGE=`echo $PAGE | sed "s/%datakey%/$datakey/"`

		$curlbin -H "X-Requested-With: XMLHttpRequest" -X POST "$URL/$PAGE" -o $TMP/cache.$FILENAME.1
		cat $TMP/cache.$FILENAME.1 | sed 's!},{!\n!g' >$TMP/cache.$FILENAME.2

		while read -u 3 ROUND; do
			TITLE=$(echo $ROUND | sed -nr 's/.*"title":"([^"]+)".*/\1/p')
			ID=$(echo $ROUND | sed -nr 's/.*"id":"([^"]+)".*/\1/p')
			if [ `echo $TITLE | grep "Staffel " | wc -l` -eq 0 ];then
				NEWPAGE=http://www.vodlocker.to/embed/movieStreams/?id=$ID
				TYPE=hosterlist
			else
				season=`echo $TITLE | tr ' ' '\n' | tail -n1`
				TMPTITLE=`echo $TITLE | tr ' ' '+'`
				NEWPAGE="http://www.vodlocker.to/embed?id=$ID&t=$TMPTITLE&season=$season&episode=1&referrer=link&server=1"
				TYPE=episode
			fi
			PIC=$(echo $ROUND | sed -nr 's/.*"img_link":"([^"]+)".*/\1/p' | sed 's/\\//g')
	
			if [ -z "$PIC" ]; then
				PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
			fi
	
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/\C3x/g' -e 's/&ndash;/-/g' -e 's/&Auml;/\C3/g' -e 's/&Uuml;/\C3S/g' -e 's/&Ouml;/\C3/g' -e 's/&auml;/\E4/g' -e 's/&uuml;/\FC/g' -e 's/&ouml;/\F6/g' -e 's/&eacute;/\E9/g' -e 's/&egrave;/\E8/g' -e 's/%F6/\F6/g' -e 's/%FC/\FC/g' -e 's/%E4/\E4/g' -e 's/%26/&/g' -e 's/%C4/\C3/g' -e 's/%D6/\C3/g' -e 's/%DC/\C3S/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`

			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
				if [ ! -e $TMP/$FILENAME.list ];then
					touch $TMP/$FILENAME.list
				fi
				piccount=`expr $piccount + 1`
				LINE="$TITLE#$SRC $SRC $TYPE '$NEWPAGE'#$PIC#$PARSER.$INPUT.$NEXT.$PAGE2.$FILENAME.$piccount.jpg#$NAME#0"
	
				echo "$LINE" >> $TMP/$FILENAME.list
			fi
	
		done 3<$TMP/cache.$FILENAME.2

#		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi
	echo "$TMP/$FILENAME.list"
}

hosterlisttv()
{
#	rm $TMP/$FILENAME.list
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		piccount=0
		$curlbin "$PAGE" -o "$TMP/cache.$FILENAME.1"	
		cat $TMP/cache.$FILENAME.1 | tr '\n' ' ' | sed "s!</li></a><a!\nfound=!g" | grep -v javascript | grep ^found= >$TMP/cache.$FILENAME.2

		while read -u 3 ROUND; do

			ID=$(echo $ROUND | sed -nr "s/.*-stream-([^-]+).html.*/\1/p")
			PIC=$(echo $ROUND | grep $ID.jpg | sed -nr "s/.*<img src='([^']+)'.*/\1/p")
			NEWPAGE=$(echo $ROUND | sed -nr "s/.*href='([^']+)'.*/\1/p")
			TITLE=$(echo $NEWPAGE | sed -nr 's/.*[http|https]:\/\/([^\/]+)\/.*/\1/p' | sed 's/www.//' | tr [A-Z] [a-z])
			PIC="http://openaaf.dyndns.tv/mediathek/menu/"$TITLE".jpg"
			EXTRA=$(echo $ROUND | sed -nr "s/.*title='([^']+)'.*/\1/p")

			if [ ! -z "$TITLE" ] && [ ! -z "$EXTRA" ];then
				TITLE="$TITLE ($EXTRA)"
			fi

			if [ -z "$PIC" ]; then
				PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
			fi

			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/\C3x/g' -e 's/&ndash;/-/g' -e 's/&Auml;/\C3/g' -e 's/&Uuml;/\C3S/g' -e 's/&Ouml;/\C3/g' -e 's/&auml;/\E4/g' -e 's/&uuml;/\FC/g' -e 's/&ouml;/\F6/g' -e 's/&eacute;/\E9/g' -e 's/&egrave;/\E8/g' -e 's/%F6/\F6/g' -e 's/%FC/\FC/g' -e 's/%E4/\E4/g' -e 's/%26/&/g' -e 's/%C4/\C3/g' -e 's/%D6/\C3/g' -e 's/%DC/\C3S/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`

			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
				if [ ! -e $TMP/$FILENAME.list ];then
					touch $TMP/$FILENAME.list
				fi
				piccount=`expr $piccount + 1`
				LINE="$TITLE#$SRC $SRC hoster '$NEWPAGE'#$PIC#$PARSER.$INPUT.$NEXT.$PAGE2.$FILENAME.$piccount.jpg#$NAME#111"

				echo "$LINE" >> $TMP/$FILENAME.list
			fi

		done 3<$TMP/cache.$FILENAME.2
		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi
	echo "$TMP/$FILENAME.list"
}


kino()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		piccount=0
		$curlbin -v "$URL/$PAGE" -o "$TMP/cache.$FILENAME.1"	
		cat $TMP/cache.$FILENAME.1 | tr '\n' ' ' | sed "s!</li></a><a!\nfound=!g" | grep ^found= >$TMP/cache.$FILENAME.2

		while read -u 3 ROUND; do
			ID=$(echo $ROUND | sed -nr "s/.*-stream-([^-]+).html.*/\1/p")
			PIC=$(echo $ROUND | grep $ID.jpg | sed -nr "s/.*<img src='([^']+)'.*/\1/p")
			NEWPAGE=$(echo $ROUND | sed -nr "s/.*href='([^']+)'.*/\1/p")
#			TITLE=$(echo $ROUND | sed -nr "s/.*> <span title='([^']+)style=.*/\1/p")
			TITLE=$(echo $ROUND | sed "s!<span title='!\nfound=!g" | sed "s!' style=!\n!g" | grep ^found= | grep -v href | cut -d"=" -f2 | head -n1)

			if [ -z "$PIC" ]; then
				PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
			fi

			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/\C3x/g' -e 's/&ndash;/-/g' -e 's/&Auml;/\C3/g' -e 's/&Uuml;/\C3S/g' -e 's/&Ouml;/\C3/g' -e 's/&auml;/\E4/g' -e 's/&uuml;/\FC/g' -e 's/&ouml;/\F6/g' -e 's/&eacute;/\E9/g' -e 's/&egrave;/\E8/g' -e 's/%F6/\F6/g' -e 's/%FC/\FC/g' -e 's/%E4/\E4/g' -e 's/%26/&/g' -e 's/%C4/\C3/g' -e 's/%D6/\C3/g' -e 's/%DC/\C3S/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`

			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
				if [ ! -e $TMP/$FILENAME.list ];then
					touch $TMP/$FILENAME.list
				fi
				piccount=`expr $piccount + 1`
				LINE="$TITLE#$SRC $SRC hosterlist '$NEWPAGE'#$PIC#$PARSER.$INPUT.$NEXT.$PAGE2.$FILENAME.$piccount.jpg#$NAME#0"

				echo "$LINE" >> $TMP/$FILENAME.list
			fi

		done 3<$TMP/cache.$FILENAME.2
		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi
	echo "$TMP/$FILENAME.list"
}

hosterlist()
{
#	rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin "$PAGE" -o $TMP/cache.$FILENAME.1
		cat $TMP/cache.$FILENAME.1 | grep iframe | sed -nr 's/.*src="([^"]+)".*/\1/p' >$TMP/cache.$FILENAME.2

		while read -u 3 ROUND; do
			NEWPAGE="$ROUND"
			TITLE=`echo $ROUND | sed -nr 's/.*:\/\/([^\/]+)\/.*/\1/p'` 		
			$curlbin "$NEWPAGE" -o $TMP/cache.$FILENAME.4

#			<a class='play_container' href='http://www.vodlocker.to/embed?t=Die+Eisk%C3%B6nigin%3A+Olaf+taut+auf&y=2017&lang=de&referrer=link'>
			cat $TMP/cache.$FILENAME.4 | grep play_container | sed -nr "s/.*href='([^']+)'.*/\1/p" >$TMP/cache.$FILENAME.4.url1
			TMPURL=`cat $TMP/cache.$FILENAME.4.url1`
			if [ ! -z "$TMPURL" ];then
				TITLE=`echo $TMPURL | sed -nr 's/.*:\/\/([^\/]+)\/.*/\1/p'` 		
				NEWPAGE=$TMPURL
				if [ ! -z "$TITLE" ] && [ "$TITLE" != " " ] && [ ! -z "$NEWPAGE" ];then
					PIC=`echo $TITLE | tr [A-Z] [a-z] | sed 's/www.//' | cut -d"." -f1 | sed 's/streamclou/streamcloud/'`
					LINE="$TITLE#$SRC $SRC hoster '$NEWPAGE'#http://openaaf.dyndns.tv/mediathek/menu/$PIC.jpg#$PIC.jpg#$NAME#111"

					echo "$LINE" >> $TMP/$FILENAME.list
				fi
			fi

			$curlbin "$TMPURL" -o $TMP/cache.$FILENAME.5
#			$("#hostname").html("<a href='http://openload.co/embed/vq1HFMJ5vAo/detroit.SD-spectre.mkv' target='_blank' style='color:gold; text-decoration:underline;'>openload.co</a>");

			cat $TMP/cache.$FILENAME.5 | grep "#hostname" | sed -nr "s/.*a href='([^']+)'.*/\1/p" | grep -v "+link+" >$TMP/cache.$FILENAME.5.url1
			TMPURL=`cat $TMP/cache.$FILENAME.5.url1`
			if [ ! -z "$TMPURL" ];then
				TITLE=`echo $TMPURL | sed -nr 's/.*:\/\/([^\/]+)\/.*/\1/p'`
				NEWPAGE=$TMPURL

				if [ ! -z "$TITLE" ] && [ "$TITLE" != " " ] && [ ! -z "$NEWPAGE" ];then
					PIC=`echo $TITLE | tr [A-Z] [a-z] | sed 's/www.//' | cut -d"." -f1 | sed 's/streamclou/streamcloud/'`
					LINE="$TITLE#$SRC $SRC hoster '$NEWPAGE'#http://openaaf.dyndns.tv/mediathek/menu/$PIC.jpg#$PIC.jpg#$NAME#111"

					echo "$LINE" >> $TMP/$FILENAME.list
				fi

			fi 		

#			var id = "131803";
#			var e = ""; 				var lang = "2";
#			var cat = "movie";
#			var links = "3";
#
#			$.post( "/embed/movieStreams/?id="+id+"&e="+e+"&lang="+lang+"&cat="+cat+"&links="+links, function( data ) {
#				$("#loadStreams").html( data );
#			});

			cat $TMP/cache.$FILENAME.5 | sed -nr 's/.*var id = "([^"]+)".*/\1/p' >$TMP/cache.$FILENAME.5.id
			id=$(cat $TMP/cache.$FILENAME.5.id)
			cat $TMP/cache.$FILENAME.5 | sed -nr 's/.*var e = "([^"]+)".*/\1/p' >$TMP/cache.$FILENAME.5.e
			e=$(cat $TMP/cache.$FILENAME.5.e)
			cat $TMP/cache.$FILENAME.5 | sed -nr 's/.*var lang = "([^"]+)".*/\1/p' >$TMP/cache.$FILENAME.5.lang
			lang=$(cat $TMP/cache.$FILENAME.5.lang)
			cat $TMP/cache.$FILENAME.5 | sed -nr 's/.*var cat = "([^"]+)".*/\1/p' >$TMP/cache.$FILENAME.5.cat
			lang=$(cat $TMP/cache.$FILENAME.5.cat)
			cat $TMP/cache.$FILENAME.5 | sed -nr 's/.*var links = "([^"]+)".*/\1/p' >$TMP/cache.$FILENAME.5.links
			links=$(cat $TMP/cache.$FILENAME.5.links)

			if [ -z "$id" ];then
#				<source src='http://www.vodlocker.to/embed/getfile?id=1472367&res=360p' type='video/mp4' data-res='360'>		</video>
				cat $TMP/cache.$FILENAME.5 | sed -nr "s/.*<source src='([^']+)'.*/\1/p" | sed -nr "s/.*getfile\?id=(.*)\&res=.*/\1/p" >$TMP/cache.$FILENAME.5.id2
#				cat /tmp/localcache/cache.streamcloud.hosterlist.http\:.www.streamcloud.me.novitiate.stream.1472367.html.5 | sed -nr "s/.*<source src='([^']+)'.*/\1/p" | sed -nr "s/.*getfile\?id=(.*)\&res=.*/\1/p"
				id=$(cat $TMP/cache.$FILENAME.5.id2)
			fi 

#			http://www.vodlocker.to/embed/movieStreams/?id=131803&e=&lang=2&cat=movie&links=3
			HOSTERURL="www.vodlocker.to/embed/movieStreams/?id=$id&e=$e&lang=$lang&cat=$cat&links=$links"
			$curlbin "$HOSTERURL" -o $TMP/cache.$FILENAME.6

			HOSTERLIST=$(cat $TMP/cache.$FILENAME.6 | grep "</li></a><a" | sed 's!http!\nfound="http!g' | grep ^found= | cut -d '"' -f2 | cut -d"'" -f1)
			for ROUND0 in $HOSTERLIST; do
				TITLE=`echo $ROUND0 | sed -nr 's/.*:\/\/([^\/]+)\/.*/\1/p'`
				if [ ! -z "$TITLE" ] && [ "$TITLE" != " " ] && [ ! -z "$ROUND0" ];then
					TITLE=`echo $ROUND0 | sed -nr 's/.*:\/\/([^\/]+)\/.*/\1/p'`
					PIC=`echo $TITLE | tr [A-Z] [a-z] | sed 's/www.//' | cut -d"." -f1 | sed 's/streamclou/streamcloud/'`
					LINE="$TITLE#$SRC $SRC hoster '$ROUND0'#http://openaaf.dyndns.tv/mediathek/menu/$PIC.jpg#$PIC.jpg#$NAME#111"

					echo "$LINE" >> $TMP/$FILENAME.list
				fi
			done
		done 3<$TMP/cache.$FILENAME.2
		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi
	echo "$TMP/$FILENAME.list"
}

hosterlist3()
{
#	rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
#	rm "$TMP/$FILENAME.list"
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin "$PAGE" -o $TMP/cache.$FILENAME.1
		cat $TMP/cache.$FILENAME.1 | sed 's/<source src/\n<source src/g' | sed -nr "s/.*src='([^']+)'.*/\1/p" | grep getfile >$TMP/cache.$FILENAME.2
#echo $TMP/cache.$FILENAME.1


#<source src='http://www.vodlocker.to/embed/getfile?id=1473892&res=720p&cat=episode&e=1' type='video/mp4' data-res='720'>
#<source src='http://www.vodlocker.to/embed/getfile?id=1473892&res=480p&cat=episode&e=1' type='video/mp4' data-res='480'>
#<source src='http://www.vodlocker.to/embed/getfile?id=1473892&res=360p&cat=episode&e=1' type='video/mp4' data-res='360'>                </video>


#exit
		while read -u 3 ROUND; do
			NEWPAGE="$ROUND"
			TITLE=`echo $ROUND | sed -nr 's/.*:\/\/([^\/]+)\/.*/\1/p'`
			EXTRA=`echo $ROUND | sed -nr "s/.*res=([^=]+)&.*/\1/p"`
			PIC=`echo $TITLE | tr [A-Z] [a-z] | sed 's/www.//' | cut -d"." -f1 | sed 's/streamclou/streamcloud/'`

			if [ ! -z "$TITLE" ] && [ ! -z "$EXTRA" ];then
				TITLE="$TITLE ($EXTRA)"
			fi

#			$curlbin2 -v --referer "$PAGE" "$NEWPAGE" -o $TMP/cache.$FILENAME.3
#			REFERER=`echo "$INPUT" | sed -e 's/=/%3D/g' -e 's/&/%26/g'` 

#echo TITLE $TITLE
#echo EXTRA $EXTRA
#echo NEWPAGE $NEWPAGE

#echo $TMP/cache.$FILENAME.3
#exit
			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
				LINE="$TITLE#$SRC $SRC hoster '$NEWPAGE'#http://openaaf.dyndns.tv/mediathek/menu/$PIC.jpg#$PIC.jpg#$NAME#111"
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
		$curlbin $PAGE -o $TMP/cache.$FILENAME.1

		MAXEPISODE=`cat $TMP/cache.$FILENAME.1 | grep Episode | sed -nr "s/.*'>([^>]+)<.*/\1/p"` 
		season=`echo $PAGE | sed -nr "s/.*season=([^=]+)&.*/\1/p"` 
		episode=`echo $PAGE | sed -nr "s/.*episode=([^=]+)&.*/\1/p"` 
		referrer=`echo $PAGE | sed -nr "s/.*referrer=([^=]+)&.*/\1/p"` 
		server=`echo $PAGE | sed -nr "s/.*server=([^=]+).*/\1/p"` 
#echo season $season
#echo episode $episode
#echo referrer $referrer
#echo server $server
#echo MAXEPISODE $MAXEPISODE
		TMPURL=$PAGE

		FOUNDEPISODE=`cat $TMP/cache.$FILENAME.1 | sed 's/episode/\nepisode/g' | grep ^episode= | sed -nr "s/.*episode=([^=]+)'.*/\1/p"` 

		tags=""
		i=1
		if [ ! -z "$i" ] && [ ! -z "$MAXEPISODE" ];then
			until [ "$i" -gt "$MAXEPISODE" ]
			do
			TMPURL=`echo $TMPURL | sed -e "s/&season=.*//" -e "s/&episode=.*//" -e "s/&referrer=.*//"`
			for ROUND in $FOUNDEPISODE; do
				if [ "$ROUND" == "$i" ];then
					echo "Season $season Episode $i#$SRC $SRC hosterlist3 '$TMPURL&season=$season&episode=$i&referrer=$referrer&server=$server' 1#http://openaaf.dyndns.tv/mediathek/menu/s"$season"e"$i".jpg#s"$season"e"$i".jpg#$NAME#0" | sort -r >> $TMP/$FILENAME.list
				fi
			done
			i=`expr $i + 1` 
			done
		fi
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
#	init) $INPUT;;
	mainmenu) $INPUT;;
	hoster) $INPUT;;
	hosterlist) $INPUT;;
	hosterlisttv) $INPUT;;
	hosterlist3) $INPUT;;
	search) $INPUT;;
	searchtv) $INPUT;;
	kino) $INPUT;;
	genre) $INPUT;;
	year) $INPUT;;
	episode) $INPUT;;
esac

