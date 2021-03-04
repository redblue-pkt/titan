#!/bin/sh
# first sh box parser for titannit mfg obi

SRC=$1
INPUT=$2
PAGE=$3
NEXT=$4
PAGE2=$5
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`

if [ -z "$FILENAME" ]; then
	FILENAME=none
fi

URL=http://meinkino.to
NAME=MeinKino

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
	echo "Kino#$SRC $SRC search 'filme/' 1 '/?order=veroeffentlichung'#http://openaaf.dyndns.tv/mediathek/menu/kino.ger.jpg#kino.ger.jpg#$NAME#0" >$TMP/$FILENAME.list
	echo "Letze Uploads#$SRC $SRC search 'filme/' 1 '/?order=neu'#http://openaaf.dyndns.tv/mediathek/menu/last.updates.ger.jpg#last.updates.ger.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Alle Filme#$SRC $SRC search 'filme/' 1 '/'#http://openaaf.dyndns.tv/mediathek/menu/Movies.jpg#Movies.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Top IMDb Filme#$SRC $SRC search '/filter/' 1 '/?type=filme&order=imdb'#http://openaaf.dyndns.tv/mediathek/menu/top.100.filme.jpg#top.100.filme.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Alle Serien#$SRC $SRC search 'tv/' 1 '/'#http://openaaf.dyndns.tv/mediathek/menu/Movies.jpg#Movies.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Genre#$SRC $SRC genre#http://openaaf.dyndns.tv/mediathek/menu/Movies.genre.jpg#Movies.genre.jpg#$NAME#0" >>$TMP/$FILENAME.list
#	echo "A-Z#$SRC $SRC sorted#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Suchen#$SRC $SRC search 'alle/' 1 '/?suche=%search%'#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >>$TMP/$FILENAME.list
	echo "$TMP/$FILENAME.list"
}

genre()
{
	watchlist="
		abenteuer-filme
		action-filme
		animation-filme
		dokumentarfilm-filme
		drama-filme
		familie-filme
		fantasy-filme
		foreign-filme
		historie-filme
		horror-filme
		komoedie-filme
		kriegsfilm-filme
		krimi-filme
		lovestory-filme
		musik-filme
		mystery-filme
		science-fiction-filme
		tv-film-filme
		thriller-filme
		western-filme
		"
	rm $TMP/$FILENAME.list > /dev/null 2>&1

	for ROUND0 in $watchlist; do
		TITLE=`echo $ROUND0 | tr '/' '-' | sed 's/-filme//g'`
		filename=`echo $TITLE`	
		echo "$TITLE#$SRC $SRC search 'genre/$ROUND0/' 1#http://openaaf.dyndns.tv/mediathek/menu/$filename.jpg#$filename.jpg#$NAME#0" >> $TMP/$FILENAME.list
	done
  	echo "$TMP/$FILENAME.list"
}

search()
{
	if [ -z "$NEXT" ]; then NEXT="search"; fi

#	if [ -e "$TMP/$FILENAME.list" ] ; then
#		rm $TMP/$FILENAME.list
#	fi

	if [ ! -e "$TMP/$FILENAME.list" ]; then
		piccount=0

		$curlbin $URL/$PAGESTART/$PAGE$NEXT$PAGE2 -o $TMP/cache.$FILENAME.1
		cat $TMP/cache.$FILENAME.1 | tr '\n' ' ' | sed 's/<div class="ml-description-top">/\nfound=/g' | grep ^found= >$TMP/cache.$FILENAME.2
	
#		<li class="active">1</li><li><a href="http://meinkino.to/filme/2?order=veroeffentlichung&type=filme" data-ci-pagination-page="2">2</a></li><li><a href="http://meinkino.to/filme/3?order=veroeffentlichung&type=filme" data-ci-pagination-page="3">3</a></li><li><a href="http://meinkino.to/filme/2?order=veroeffentlichung&type=filme" data-ci-pagination-page="2" rel="next">&gt;</a></li><li><a href="http://meinkino.to/filme/87?order=veroeffentlichung&type=filme" data-ci-pagination-page="87">Letzte &rsaquo;</a></li> </ul>
		pages=`cat $TMP/cache.$FILENAME.1 | grep data-ci-pagination-page | sed 's/data-ci-pagination-page=/\nfound=/g' |grep ^found= | cut -d'"' -f2 | tail -n1`
	
		while read -u 3 ROUND; do
			PIC=`echo $ROUND | sed 's!<img src=!\nurl=!g' | grep ^url= | cut -d'"' -f2 | tail -n1`
			TITLE=`echo $ROUND | sed 's!class="ml-name">!\ntitle=<!g' | grep ^title= | sed 's/<span class="mark-wc">//g' | sed 's!</span>!!g' | cut -d'<' -f2`
			NEWPAGE=`echo $ROUND | sed 's!<a href=!\nnewpage=!g' | grep ^newpage= | cut -d'"' -f2 | head -n1 | sed "s#$URL##"`
	
			if [ -z "$PIC" ]; then
				PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
			fi
	
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
	
			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
				if [ ! -e $TMP/$FILENAME.list ];then
					touch $TMP/$FILENAME.list
				fi
				piccount=`expr $piccount + 1`

				if [ `cat $TMP/$FILENAME.list | grep "#$NEWPAGE#" | wc -l` -eq 0 ];then
					if [ `echo $NEWPAGE | grep staffel | grep folge | wc -l` -eq 1 ];then
						LINE="$TITLE#$SRC $SRC serielist $NEWPAGE#$PIC#$FILENAME.$FILENAME.$NEXT.$piccount.jpg#$NAME#0"
					else
						LINE="$TITLE#$SRC $SRC hosterlist $NEWPAGE#$PIC#$FILENAME.$FILENAME.$NEXT.$piccount.jpg#$NAME#0"
					fi
				fi
				echo "$LINE" >> $TMP/$FILENAME.list
			fi
	
		done 3<$TMP/cache.$FILENAME.2
	
		if [ "$NEXT" -lt "$pages" ]; then
			NEXTPAGE=`expr $NEXT + 1`
			LINE="Page ($NEXTPAGE/$pages)#$SRC $SRC search '$PAGE' $NEXTPAGE '$PAGE2'#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#$NAME#0"
			echo "$LINE" >> $TMP/$FILENAME.list
		fi
		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi
	echo "$TMP/$FILENAME.list"
}

serielist()
{
	rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	if [ -e "$TMP/$FILENAME.list" ] ; then
		rm $TMP/$FILENAME.list
	fi
	$curlbin $URL/$PAGE -o $TMP/cache.$FILENAME.1
	TMPURL=`cat $TMP/cache.$FILENAME.1 | sed 's/data-url-id=/\nfound=/g' | grep ^found= | cut -d'"' -f2`

	cat $TMP/cache.$FILENAME.1 | sed 's/<li class="active">/<li>/g' | grep ^"<li><a href=" | grep staffel >$TMP/cache.$FILENAME.3
	while read -u 3 ROUND; do
		NEWPAGE=`echo $ROUND | cut -d'"' -f2 | sed "s#$URL##"`
		TITLE=`echo $ROUND | sed 's/staffel-/\nStaffel /g' | sed 's/-stream-/\nstream/g' | sed 's/-/ /g' | grep ^Staffel | sed 's/folge/Folge/g'` 

		if [ ! -z "$TITLE" ] && [ "$TITLE" != " " ] && [ ! -z "$NEWPAGE" ];then
			PIC=`echo $TITLE | sed 's/Staffel /s/g' | sed 's/ Folge /e/g'`
			LINE="$TITLE#$SRC $SRC hosterlist $NEWPAGE#http://openaaf.dyndns.tv/mediathek/menu/$PIC.jpg#$PIC.jpg#$NAME#0"
			echo "$LINE" >> $TMP/$FILENAME.list
		fi
	done 3<$TMP/cache.$FILENAME.3
#	rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	echo "$TMP/$FILENAME.list"
}


hosterlist()
{
	rm $TMP/cache.$FILENAME.* > /dev/null 2>&1

	if [ -e "$TMP/$FILENAME.list" ] ; then
		rm $TMP/$FILENAME.list
	fi
	$curlbin $URL/$PAGE -o $TMP/cache.$FILENAME.1
	TMPURL=`cat $TMP/cache.$FILENAME.1 | sed 's/data-url-id=/\nfound=/g' | grep ^found= | cut -d'"' -f2`

	TRAILER=`cat $TMP/cache.$FILENAME.1 | grep youtube | cut -d'"' -f2`

#	$curlbin2 -H "X-Requested-With: XMLHttpRequest" -X POST  --referer http://meinkino.to/film/the-zero-theorem-stream-id8795 http://meinkino.to/geturl/8795
	$curlbin -H "X-Requested-With: XMLHttpRequest" -X POST  --referer $URL/$PAGE $TMPURL -o $TMP/cache.$FILENAME.2

	cat $TMP/cache.$FILENAME.2 | sed 's/{"url":"/\nlink_url":"/g' | sed 's/link_/\nlink_/g' | grep ^link_ | sed 's/"alternative":{"/\nlink_/g' >$TMP/cache.$FILENAME.3

	if [ ! -z "$TRAILER" ];then
		ID=`echo $TRAILER | tr '=' '\n' | tail -n1`
#		TRAILERURL="https://www.youtube.com/get_video_info?el=leanback&cplayer=UNIPLAYER&cos=Windows&height=1080&cbr=Chrome&hl=en_US&cver=4&ps=leanback&c=TVHTML5&video_id=$ID&cbrver=40.0.2214.115&width=1920&cosver=6.1&ssl_stream=1"
		TRAILERURL="https://www.youtube.com/watch?v=$ID"

		LINE="Youtube TRAILER#$SRC $SRC play '$TRAILERURL'#http://openaaf.dyndns.tv/mediathek/menu/youtube.jpg#$youtube.jpg#$NAME#111"
		echo "$LINE" >> $TMP/$FILENAME.list
	fi

	while read -u 3 ROUND; do
		NEWPAGE=`echo $ROUND | cut -d'"' -f3`
		TITLE=`echo $ROUND | cut -d'"' -f1 | cut -d'_' -f2` 		
		EXTRA=`echo $ROUND | sed 's/"quality"/\nextra=/g' | grep ^extra= | cut -d'"' -f2` 		
		#"quality":"480"

		if [ ! -z "$TITLE" ] && [ "$TITLE" != " " ] && [ ! -z "$NEWPAGE" ];then
			PIC=`echo $TITLE | tr [A-Z] [a-z] | cut -d "_" -f2 | cut -d" " -f1`
			hostercheck=`echo $NEWPAGE | tr 'A-Z' 'a-z' | sed 's!://!\n!' | cut -d'/' -f1 | tail -n1 | tr '.' '\n' | wc -l`
			hosterline=`expr $hostercheck - 1`
			hoster=`echo $NEWPAGE | tr 'A-Z' 'a-z' | sed 's!://!\n!' | cut -d'/' -f1 | tail -n1 | cut -d"." -f$hosterline`
			#echo $hoster
			PIC=$hoster
			if [ ! -z "$hoster" ];then
				if [ ! -z "$EXTRA" ];then
					TITLE="$hoster ($TITLE $EXTRA)"
				else
					TITLE="$hoster ($TITLE)"
				fi
			fi
			LINE="$TITLE#$SRC $SRC play '$NEWPAGE|User-Agent=$USERAGENT&Referer=$URL/$PAGE'#http://openaaf.dyndns.tv/mediathek/menu/$PIC.jpg#$PIC.jpg#$NAME#111"
			echo "$LINE" >> $TMP/$FILENAME.list
		fi
	done 3<$TMP/cache.$FILENAME.3
#	rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	echo "$TMP/$FILENAME.list"
}

play()
{
	URL="gethoster2 $PAGE"
	echo "$URL"
}

case $INPUT in
#	init) $INPUT;;SRC=$1
	mainmenu) $INPUT;;
	hosterlist) $INPUT;;
	serielist) $INPUT;;
	play) $INPUT;;
	search) $INPUT;;
	genre) $INPUT;;
esac
