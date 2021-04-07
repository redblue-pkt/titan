#!/bin/sh
# first sh box parser for titannit mfg obi

SRC=$1
INPUT=$2
PAGE=$3
NEXT=$4
PAGE2=$5
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`

#URL=http://movie4k.to
URL=https://www2.movie4k.org/

PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME=Movie4k

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
	echo "Kino#$SRC $SRC kino 'index.php?lang=de'#http://openaaf.dyndns.tv/mediathek/menu/kino.jpg#kino.jpg#$NAME#0" >$TMP/$FILENAME.list
	echo "Movies (A-Z)#$SRC $SRC sorted#http://openaaf.dyndns.tv/mediathek/menu/Movies.jpg#Movies.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Movies (Genre)#$SRC $SRC genre#http://openaaf.dyndns.tv/mediathek/menu/Movies.genre.jpg#Movies.genre.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Tv-Shows (A-Z)#$SRC $SRC sortedtv#http://openaaf.dyndns.tv/mediathek/menu/tv-shows.jpg#tv-shows.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Tv-Shows (Genre)#$SRC $SRC genretv#http://openaaf.dyndns.tv/mediathek/menu/movie.genre.jpg#movie.genre.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Search Movie#$SRC $SRC search 'movies.php?list=search&search=%search%'#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >>$TMP/$FILENAME.list
	echo "Search Tv#$SRC $SRC searchtv 'movies.php?list=search&search=%search%'#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >>$TMP/$FILENAME.list
	echo "$TMP/$FILENAME.list"
}

genre()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		watchlist="
			movies-genre-1-Action.html
			movies-genre-58-Adult.html
			movies-genre-4-Adventure.html
			movies-genre-5-Animation.html
			movies-genre-6-Biography.html
			movies-genre-27-Bollywood.html
			movies-genre-3-Comedy.html
			movies-genre-7-Crime.html
			movies-genre-8-Documentary.html
			movies-genre-2-Drama.html
			movies-genre-9-Family.html
			movies-genre-10-Fantasy.html
			movies-genre-13-History.html
			movies-genre-14-Horror.html
			movies-genre-15-Music.html
			movies-genre-56-Musical.html
			movies-genre-17-Mystery.html
			movies-genre-26-Other.html
			movies-genre-59-Reality-TV.html
			movies-genre-20-Romance.html
			movies-genre-21-Sci-Fi.html
			movies-genre-55-Short.html
			movies-genre-22-Sport.html
			movies-genre-23-Thriller.html
			movies-genre-24-War.html
			movies-genre-25-Western.html
			"
		rm $TMP/$FILENAME.list > /dev/null 2>&1

		for ROUND0 in $watchlist; do
			TITLE=`echo $ROUND0 | cut -d"-" -f4 | sed 's/.html//'`
			NUMMER=`echo $ROUND0 | cut -d"-" -f3`

			filename=`echo $TITLE | tr [A-Z] [a-z]`	
			echo "$TITLE#$SRC $SRC search 'movies-genre-$NUMMER-' 1 '.html'#http://openaaf.dyndns.tv/mediathek/menu/$filename.jpg#$filename.jpg#$NAME#0" >> $TMP/$FILENAME.list
		done
	fi
  	echo "$TMP/$FILENAME.list"
}

genretv()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		watchlist="
			tvshows-genre-1-Action.html
			tvshows-genre-58-Adult.html
			tvshows-genre-4-Adventure.html
			tvshows-genre-5-Animation.html
			tvshows-genre-6-Biography.html
			tvshows-genre-27-Bollywood.html
			tvshows-genre-3-Comedy.html
			tvshows-genre-7-Crime.html
			tvshows-genre-8-Documentary.html
			tvshows-genre-2-Drama.html
			tvshows-genre-9-Family.html
			tvshows-genre-10-Fantasy.html
			tvshows-genre-13-History.html
			tvshows-genre-14-Horror.html
			tvshows-genre-15-Music.html
			tvshows-genre-56-Musical.html
			tvshows-genre-17-Mystery.html
			tvshows-genre-26-Other.html
			tvshows-genre-59-Reality-TV.html
			tvshows-genre-20-Romance.html
			tvshows-genre-21-Sci-Fi.html
			tvshows-genre-55-Short.html
			tvshows-genre-22-Sport.html
			tvshows-genre-23-Thriller.html
			tvshows-genre-24-War.html
			tvshows-genre-25-Western.html
			"
		rm $TMP/$FILENAME.list > /dev/null 2>&1

		for ROUND0 in $watchlist; do
			TITLE=`echo $ROUND0 | cut -d"-" -f4 | sed 's/.html//'`
			NUMMER=`echo $ROUND0 | cut -d"-" -f3`

			filename=`echo $TITLE | tr [A-Z] [a-z]`	
			echo "$TITLE#$SRC $SRC searchtv 'tvshows-genre-$NUMMER-' 1 '.html'#http://openaaf.dyndns.tv/mediathek/menu/$filename.jpg#$filename.jpg#$NAME#0" >> $TMP/$FILENAME.list
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

sortedtv()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		watchlist="0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z"

		rm $TMP/$FILENAME.list > /dev/null 2>&1

		for ROUND0 in $watchlist; do
			TITLE=`echo $ROUND0`
			filename=`echo $TITLE | tr [A-Z] [a-z]`
			echo "$TITLE#$SRC $SRC searchtv 'tvshows-all-$ROUND0-' 1 '.html'#http://openaaf.dyndns.tv/mediathek/menu/$filename.jpg#$filename.jpg#$NAME#0" >> $TMP/$FILENAME.list
		done
	fi
  	echo "$TMP/$FILENAME.list"
}

searchtv()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		piccount=0

#		$curlbin $URL/$PAGE$NEXT$PAGE2 -o $TMP/cache.$FILENAME.1
		$BIN /tmp/localhoster/cloudflare.py "$URL/$PAGE$NEXT$PAGE2" > $TMP/cache.$FILENAME.1

		if [ -z "$NEXT" ];then
			NEXT=1
		fi
		pages=`cat $TMP/cache.$FILENAME.1 | grep $PAGE | sed "s/$PAGE/\n$PAGE/g" | cut -d ">" -f2 | cut -d "<" -f1 | tail -n1`
		if [ -z "$pages" ];then
			pages=0
		fi
		cat $TMP/cache.$FILENAME.1 | tr '\n' ' ' | sed 's!tablemoviesindex!\ntablemoviesindex!g' | grep ^"tablemoviesindex" | sed 's!coverPreview!\ncoverPreview!g' | grep ^"coverPreview" | grep 'id="tdmovies"' >$TMP/cache.$FILENAME.2

		while read -u 3 ROUND; do
			ID=`echo $ROUND | cut -d'"' -f1 | tail -n1`
			TITLE=`echo $ROUND | sed 's!<a href=!\nfound=>!g' | grep ^found= | cut -d">" -f3 | cut -d"<" -f1`
			NEWPAGE=`echo $ROUND | sed 's!<a href=!\nfound=!g' | grep ^found= | cut -d'"' -f2 | tail -n1`
	
			if [ ! -z "$NEWPAGE" ]; then
				if [ -z "$TITLE" ];then
					TITLE=`echo $TMPURL`
				fi
				TMPURL="$URL"/$TMPURL
			fi

			PIC=`cat $TMP/cache.$FILENAME.1 | tr '\n' ' ' | sed "s/$ID/\n$ID/g" | grep ^"$ID"'").hover' | sed "s#img src=#\nfound=#g" | grep ^"found=" | cut -d"'" -f2 | head -n1`

			if [ `echo $ROUND | grep us_ger_small.png | wc -l` -eq 1 ]; then
				LANG=" (de)"
			elif [ `echo $ROUND | grep us_flag_small.png | wc -l` -eq 1 ]; then
				LANG=" (en)"
			else
				LANG=" (??)"
			fi
	
			if [ -z "$PIC" ]; then
				PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
			fi
	
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/\C3x/g' -e 's/&ndash;/-/g' -e 's/&Auml;/\C3/g' -e 's/&Uuml;/\C3S/g' -e 's/&Ouml;/\C3/g' -e 's/&auml;/ä/g' -e 's/&uuml;/ü/g' -e 's/&ouml;/ö/g' -e 's/&eacute;/é/g' -e 's/&egrave;/è/g' -e 's/%F6/ö/g' -e 's/%FC/ü/g' -e 's/%E4/ä/g' -e 's/%26/&/g' -e 's/%C4/\C3/g' -e 's/%D6/\C3/g' -e 's/%DC/\C3S/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
	
			if [ ! -z "$TITLE" ] && [ ! -z "$TMPURL" ];then
				if [ ! -e $TMP/$FILENAME.list ];then
					touch $TMP/$FILENAME.list
				fi
				piccount=`expr $piccount + 1`
				LINE="$TITLE $LANG#$SRC $SRC season $NEWPAGE#$PIC#$PARSER.$INPUT.$NEXT.$PAGE2.$FILENAME.$piccount.jpg#$NAME#0"
	
				echo "$LINE" >> $TMP/$FILENAME.list
			fi
	
		done 3<$TMP/cache.$FILENAME.2

		if [ "$NEXT" -lt "$pages" ]; then
			NEXTPAGE=`expr $NEXT + 1`
			LINE="Page ($NEXTPAGE/$pages)#$SRC $SRC searchtv '$PAGE' $NEXTPAGE '$PAGE2'#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#$NAME#0"
			echo "$LINE" >> $TMP/$FILENAME.list
		fi
		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi
	echo "$TMP/$FILENAME.list"
}

searchtold()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		piccount=0

#		$curlbin $URL/$PAGE$NEXT$PAGE2 -o $TMP/cache.$FILENAME.1
		$BIN /tmp/localhoster/cloudflare.py "$URL/$PAGE$NEXT$PAGE2" > $TMP/cache.$FILENAME.1

		pages=`cat $TMP/cache.$FILENAME.1 | grep $PAGE | sed "s/$PAGE/\n$PAGE/g" | cut -d ">" -f2 | cut -d "<" -f1 | tail -n1`
		if [ -z "$pages" ];then
			pages=0
		fi
		cat $TMP/cache.$FILENAME.1 | tr '\n' ' ' | sed 's!<TR>!\nfound=!g'| grep ^"found="  >$TMP/cache.$FILENAME.2

		while read -u 3 ROUND; do
			ID=`echo $ROUND | cut -d'"' -f1 | tail -n1`
			TITLE=`echo $ROUND | sed 's!<a href=!\nfound=>!g' | grep ^found= | cut -d">" -f3 | cut -d"<" -f1`
			NEWPAGE=`echo $ROUND | sed 's!<a href=!\nfound=!g' | grep ^found= | cut -d'"' -f2 | tail -n1`
	
			if [ ! -z "$NEWPAGE" ]; then
				if [ -z "$TITLE" ];then
					TITLE=`echo $TMPURL`
				fi
				TMPURL="$URL"/$TMPURL
			fi

			PIC=`cat $TMP/cache.$FILENAME.1 | tr '\n' ' ' | sed "s/$ID/\n$ID/g" | grep ^"$ID"'").hover' | sed "s#img src=#\nfound=#g" | grep ^"found=" | cut -d"'" -f2 | head -n1`

			if [ `echo $ROUND | grep us_ger_small.png | wc -l` -eq 1 ]; then
				LANG=" (de)"
			elif [ `echo $ROUND | grep us_flag_small.png | wc -l` -eq 1 ]; then
				LANG=" (en)"
			else
				LANG=" (??)"
			fi
		
			if [ -z "$PIC" ]; then
				PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
			fi
	
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/\C3x/g' -e 's/&ndash;/-/g' -e 's/&Auml;/\C3/g' -e 's/&Uuml;/\C3S/g' -e 's/&Ouml;/\C3/g' -e 's/&auml;/ä/g' -e 's/&uuml;/ü/g' -e 's/&ouml;/ö/g' -e 's/&eacute;/é/g' -e 's/&egrave;/è/g' -e 's/%F6/ö/g' -e 's/%FC/ü/g' -e 's/%E4/ä/g' -e 's/%26/&/g' -e 's/%C4/\C3/g' -e 's/%D6/\C3/g' -e 's/%DC/\C3S/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
	
			if [ ! -z "$TITLE" ] && [ ! -z "$TMPURL" ];then
				if [ ! -e $TMP/$FILENAME.list ];then
					touch $TMP/$FILENAME.list
				fi
				piccount=`expr $piccount + 1`
				LINE="$TITLE $LANG#$SRC $SRC season '$NEWPAGE'#$PIC#$PARSER.$INPUT.$NEXT.$PAGE2.$FILENAME.$piccount.jpg#$NAME#0"
				echo "$LINE" >> $TMP/$FILENAME.list
			fi
	
		done 3<$TMP/cache.$FILENAME.2

		if [ "$NEXT" -lt "$pages" ]; then
			NEXTPAGE=`expr $NEXT + 1`
			LINE="Page ($NEXTPAGE/$pages)#$SRC $SRC searchtv '$PAGE' $NEXTPAGE '$PAGE2'#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#$NAME#0"
			echo "$LINE" >> $TMP/$FILENAME.list
		fi
		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi
	echo "$TMP/$FILENAME.list"
}

season()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		piccount=0

#		$curlbin $URL/$PAGE$NEXT$PAGE2 -o $TMP/cache.$FILENAME.1
		$BIN /tmp/localhoster/cloudflare.py "$URL/$PAGE$NEXT$PAGE2" > $TMP/cache.$FILENAME.1

		pages=`cat $TMP/cache.$FILENAME.1 | grep $PAGE | sed "s/$PAGE/\n$PAGE/g" | cut -d ">" -f2 | cut -d "<" -f1 | tail -n1`
		if [ -z "$pages" ];then
			pages=0
		fi
		if [ -z "$NEXT" ];then
			NEXT=0
		fi

		cat $TMP/cache.$FILENAME.1 | tr '\r' ' ' | tr '\n' ' ' | sed 's!<div id="episodediv!\nfound=!g'| sed 's!</td>!\nstop=!g' | grep ^"found="  >$TMP/cache.$FILENAME.2

		while read -u 3 ROUND; do
			echo $ROUND | sed 's!<OPTION value=!\nfound2=!g' | grep ^found2 >$TMP/cache.$FILENAME.3
			SEASON=`echo $ROUND | sed 's!<FORM name="episodeform!\nfound1="!g' | grep ^found1 | cut -d '"' -f2`
			while read -u 3 ROUND2; do
#				echo ROUND2 $ROUND2
				EPISODE=`echo $ROUND2 | cut -d'>' -f2 | cut -d'<' -f1 | sed 's/Episode //g'`
				NEWPAGE=`echo $ROUND2 | cut -d'"' -f2 | tail -n1`
				TITLE="Season $SEASON Episode $EPISODE"

				if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
					if [ ! -e $TMP/$FILENAME.list ];then
						touch $TMP/$FILENAME.list
					fi
					piccount=`expr $piccount + 1`
					LINE="$TITLE#$SRC $SRC hosterlist '$NEWPAGE'#http://openaaf.dyndns.tv/mediathek/menu/s"$SEASON"e"$EPISODE".jpg#s"$SEASON"e"$EPISODE".jpg#$NAME#0"

					echo "$LINE" >> $TMP/$FILENAME.list
				fi
			done 3<$TMP/cache.$FILENAME.3
		done 3<$TMP/cache.$FILENAME.2

		if [ "$NEXT" -lt "$pages" ]; then
			NEXTPAGE=`expr $NEXT + 1`
			LINE="Page ($NEXTPAGE/$pages)#$SRC $SRC season '$PAGE' $NEXTPAGE '$PAGE2'#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#$NAME#0"
			echo "$LINE" >> $TMP/$FILENAME.list
		fi
		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi
	echo "$TMP/$FILENAME.list"
}

seasonold()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		piccount=0

#		$curlbin $URL/$PAGE$NEXT$PAGE2 -o $TMP/cache.$FILENAME.1
		$BIN /tmp/localhoster/cloudflare.py "$URL/$PAGE$NEXT$PAGE2" > $TMP/cache.$FILENAME.1

		pages=`cat $TMP/cache.$FILENAME.1 | grep $PAGE | sed "s/$PAGE/\n$PAGE/g" | cut -d ">" -f2 | cut -d "<" -f1 | tail -n1`
		if [ -z "$pages" ];then
			pages=0
		fi
		if [ -z "$NEXT" ];then
			NEXT=0
		fi

		cat $TMP/cache.$FILENAME.1 | tr '\n' ' ' | sed 's!<TR>!\nfound=!g'| grep ^"found="  >$TMP/cache.$FILENAME.2

		while read -u 3 ROUND; do
			ID=`echo $ROUND | cut -d'"' -f1 | tail -n1`
			TITLE=`echo $ROUND | sed 's!<a href=!\nfound=>!g' | grep ^found= | cut -d">" -f3 | cut -d"<" -f1`
			NEWPAGE=`echo $ROUND | sed 's!<a href=!\nfound=!g' | grep ^found= | cut -d'"' -f2 | tail -n1`
	
			if [ ! -z "$NEWPAGE" ]; then
				if [ -z "$TITLE" ];then
					TITLE=`echo $TMPURL`
				fi
				TMPURL="$URL"/$TMPURL
			fi

			PIC=`cat $TMP/cache.$FILENAME.1 | tr '\n' ' ' | sed "s/$ID/\n$ID/g" | grep ^"$ID"'").hover' | sed "s#img src=#\nfound=#g" | grep ^"found=" | cut -d"'" -f2 | head -n1`

			if [ `echo $ROUND | grep us_ger_small.png | wc -l` -eq 1 ]; then
				LANG=" (de)"
			elif [ `echo $ROUND | grep us_flag_small.png | wc -l` -eq 1 ]; then
				LANG=" (en)"
			else
				LANG=" (??)"
			fi
		
			if [ -z "$PIC" ]; then
				PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
			fi
	
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/\C3x/g' -e 's/&ndash;/-/g' -e 's/&Auml;/\C3/g' -e 's/&Uuml;/\C3S/g' -e 's/&Ouml;/\C3/g' -e 's/&auml;/ä/g' -e 's/&uuml;/ü/g' -e 's/&ouml;/ö/g' -e 's/&eacute;/é/g' -e 's/&egrave;/è/g' -e 's/%F6/ö/g' -e 's/%FC/ü/g' -e 's/%E4/ä/g' -e 's/%26/&/g' -e 's/%C4/\C3/g' -e 's/%D6/\C3/g' -e 's/%DC/\C3S/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
	
			if [ ! -z "$TITLE" ] && [ ! -z "$TMPURL" ];then
				if [ ! -e $TMP/$FILENAME.list ];then
					touch $TMP/$FILENAME.list
				fi
				piccount=`expr $piccount + 1`
				LINE="$TITLE $LANG#$SRC $SRC episode '$NEWPAGE'#$PIC#$PARSER.$INPUT.$NEXT.$PAGE2.$FILENAME.$piccount.jpg#$NAME#0"

				echo "$LINE" >> $TMP/$FILENAME.list
			fi
	
		done 3<$TMP/cache.$FILENAME.2

		if [ "$NEXT" -lt "$pages" ]; then
			NEXTPAGE=`expr $NEXT + 1`
			LINE="Page ($NEXTPAGE/$pages)#$SRC $SRC season '$PAGE' $NEXTPAGE '$PAGE2'#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#$NAME#0"
			echo "$LINE" >> $TMP/$FILENAME.list
		fi
		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi
	echo "$TMP/$FILENAME.list"
}

episode()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		piccount=0

#		$curlbin $URL/$PAGE$NEXT$PAGE2 -o $TMP/cache.$FILENAME.1
		$BIN /tmp/localhoster/cloudflare.py "$URL/$PAGE$NEXT$PAGE2" > $TMP/cache.$FILENAME.1

		pages=`cat $TMP/cache.$FILENAME.1 | grep $PAGE | sed "s/$PAGE/\n$PAGE/g" | cut -d ">" -f2 | cut -d "<" -f1 | tail -n1`
		if [ -z "$pages" ];then
			pages=0
		fi
		if [ -z "$NEXT" ];then
			NEXT=0
		fi
		cat $TMP/cache.$FILENAME.1 | tr '\n' ' ' | sed 's!<TR>!\nfound=!g'| grep ^"found="  >$TMP/cache.$FILENAME.2

		while read -u 3 ROUND; do
			ID=`echo $ROUND | cut -d'"' -f1 | tail -n1`
			TITLE=`echo $ROUND | sed 's!<a href=!\nfound=>!g' | grep ^found= | cut -d">" -f3 | cut -d"<" -f1`
			NEWPAGE=`echo $ROUND | sed 's!<a href=!\nfound=!g' | grep ^found= | cut -d'"' -f2 | tail -n1`
	
			if [ ! -z "$NEWPAGE" ]; then
				if [ -z "$TITLE" ];then
					TITLE=`echo $TMPURL`
				fi
				TMPURL="$URL"/$TMPURL
			fi

			PIC=`cat $TMP/cache.$FILENAME.1 | tr '\n' ' ' | sed "s/$ID/\n$ID/g" | grep ^"$ID"'").hover' | sed "s#img src=#\nfound=#g" | grep ^"found=" | cut -d"'" -f2 | head -n1`

			if [ `echo $ROUND | grep us_ger_small.png | wc -l` -eq 1 ]; then
				LANG=" (de)"
			elif [ `echo $ROUND | grep us_flag_small.png | wc -l` -eq 1 ]; then
				LANG=" (en)"
			else
				LANG=" (??)"
			fi
	
			if [ -z "$PIC" ]; then
				PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
			fi
	
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/\C3x/g' -e 's/&ndash;/-/g' -e 's/&Auml;/\C3/g' -e 's/&Uuml;/\C3S/g' -e 's/&Ouml;/\C3/g' -e 's/&auml;/ä/g' -e 's/&uuml;/ü/g' -e 's/&ouml;/ö/g' -e 's/&eacute;/é/g' -e 's/&egrave;/è/g' -e 's/%F6/ö/g' -e 's/%FC/ü/g' -e 's/%E4/ä/g' -e 's/%26/&/g' -e 's/%C4/\C3/g' -e 's/%D6/\C3/g' -e 's/%DC/\C3S/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
	
			if [ ! -z "$TITLE" ] && [ ! -z "$TMPURL" ];then
				if [ ! -e $TMP/$FILENAME.list ];then
					touch $TMP/$FILENAME.list
				fi
				piccount=`expr $piccount + 1`
				LINE="$TITLE $LANG#$SRC $SRC hosterlist '$NEWPAGE'#$PIC#$PARSER.$INPUT.$NEXT.$PAGE2.$FILENAME.$piccount.jpg#$NAME#0"

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

search()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		piccount=0

#		$curlbin $URL/$PAGE$NEXT$PAGE2 -o $TMP/cache.$FILENAME.1
		$BIN /tmp/localhoster/cloudflare.py "$URL/$PAGE$NEXT$PAGE2" > $TMP/cache.$FILENAME.1

		pages=`cat $TMP/cache.$FILENAME.1 | grep $PAGE | sed "s/$PAGE/\n$PAGE/g" | cut -d ">" -f2 | cut -d "<" -f1 | tail -n1`
		if [ -z "$pages" ];then
			pages=0
		fi
		cat $TMP/cache.$FILENAME.1 | tr '\n' ' ' | sed 's!tablemoviesindex!\ntablemoviesindex!g' | grep ^"tablemoviesindex" | sed 's!coverPreview!\ncoverPreview!g' | grep ^"coverPreview" | grep 'id="tdmovies"' >$TMP/cache.$FILENAME.2

		while read -u 3 ROUND; do
			ID=`echo $ROUND | cut -d'"' -f1 | tail -n1`
			TITLE=`echo $ROUND | sed 's!<a href=!\nfound=>!g' | grep ^found= | cut -d">" -f3 | cut -d"<" -f1`
			NEWPAGE=`echo $ROUND | sed 's!<a href=!\nfound=!g' | grep ^found= | cut -d'"' -f2 | tail -n1`
	
			if [ ! -z "$NEWPAGE" ]; then
				if [ -z "$TITLE" ];then
					TITLE=`echo $TMPURL`
				fi
				TMPURL="$URL"/$TMPURL
			fi

			PIC=`cat $TMP/cache.$FILENAME.1 | tr '\n' ' ' | sed "s/$ID/\n$ID/g" | grep ^"$ID"'").hover' | sed "s#img src=#\nfound=#g" | grep ^"found=" | cut -d"'" -f2 | head -n1`

			if [ `echo $ROUND | grep us_ger_small.png | wc -l` -eq 1 ]; then
				LANG=" (de)"
			elif [ `echo $ROUND | grep us_flag_small.png | wc -l` -eq 1 ]; then
				LANG=" (en)"
			else
				LANG=" (??)"
			fi
	
			if [ -z "$PIC" ]; then
				PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
			fi
	
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/\C3x/g' -e 's/&ndash;/-/g' -e 's/&Auml;/\C3/g' -e 's/&Uuml;/\C3S/g' -e 's/&Ouml;/\C3/g' -e 's/&auml;/ä/g' -e 's/&uuml;/ü/g' -e 's/&ouml;/ö/g' -e 's/&eacute;/é/g' -e 's/&egrave;/è/g' -e 's/%F6/ö/g' -e 's/%FC/ü/g' -e 's/%E4/ä/g' -e 's/%26/&/g' -e 's/%C4/\C3/g' -e 's/%D6/\C3/g' -e 's/%DC/\C3S/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
	
			if [ ! -z "$TITLE" ] && [ ! -z "$TMPURL" ];then
				if [ ! -e $TMP/$FILENAME.list ];then
					touch $TMP/$FILENAME.list
				fi
				piccount=`expr $piccount + 1`
				LINE="$TITLE $LANG#$SRC $SRC hosterlist $NEWPAGE#$PIC#$PARSER.$INPUT.$NEXT.$PAGE2.$FILENAME.$piccount.jpg#$NAME#0"
	
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

kino()
{
#rm $TMP/$FILENAME.list
#<a href="Star-Wars-Der-Aufstieg-Skywalkers-online-film-16849319.html"><img src="https://www2.movie4k.org//thumbs/cover-16849576-Star-Wars-Der-Aufstieg-Skywalkers-movie4k-film.jpg" border=0 style="width:105px;max-width:105px;max-height:160px;min-height:140px;" alt="Star Wars: Der Aufstieg Skywalkers kostenlos" title="Star Wars: Der Aufstieg Skywalkers kostenlos"></a>

#$BIN /tmp/localhoster/cloudflare.py "$URL/$PAGE" > /tmp/localcache/movie4k.123
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$BIN /tmp/localhoster/cloudflare.py "$URL/$PAGE" | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT \
		'
			# BEGIN variable setzen
			BEGIN \
				{
					suche = 0
				}
				/\/public-announcement.php/ \
 				{
					suche = 1
					next
				}
				/^<a href=/ \
 				{
					if ( suche == 1 )
					{
						i = index($0, "<a href=\"") + 9
					        j = index(substr($0, i), "\"") - 1
					        newpage = substr($0, i, j)

						i = index($0, "<img src=\"") + 10
					        j = index(substr($0, i), "\"") - 1
					        pic = substr($0, i, j)

						i = index($0, "title=\"") + 7
					        j = index(substr($0, i), "\"") - 1
					        title = substr($0, i, j)

						gsub(/ kostenlos/,"",title)

						if (title != "")
						{

							piccount += 1
							# 25. in naechste zeile springen
							# 26. \x27 = single quotes
							if ( newpage != "" )
								print title "#" SRC " " SRC " hosterlist \x27" newpage "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#0"
							else
								print title "(ERROR)#" SRC " " SRC " hosterlist \x27" newpage "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#0"
						}
						next
					}
				}

		# 29. schreibe alles in die list datei
		' >$TMP/$FILENAME.list
	fi

	echo "$TMP/$FILENAME.list"
}

hosterlist()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$BIN /tmp/localhoster/cloudflare.py "$URL/$PAGE" | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT \
		'
			# BEGIN variable setzen
			BEGIN \
				{
					suche = 1
				}
#				/^links\[/ \
                		/tablemoviesindex2/ \
				{
					if ( suche == 1 )
					{
#						# extrahiere den newpage pfad
#						i = index($0, "href=\\\"") + 7
#				        	j = index(substr($0, i), "\\") - 1
#				        	newpage = substr($0, i, j)

						# extrahiere den newpage pfad
						i = index($0, "window.location.href = \x27") + 24
					    	j = index(substr($0, i), "\x27") - 1
					    	newpage = substr($0, i, j)
#print "0" newpage

						if (newpage == "")
						{
							i = index($0, "href=\\\"") + 7
					        j = index(substr($0, i), "\\") - 1
					        newpage = substr($0, i, j)
#print "1" newpage
						}

						if (newpage == "")
						{
							i = index($0, "href=\"") + 6
					        j = index(substr($0, i), "\">") - 1
					        newpage = substr($0, i, j)
#print "3" newpage
						}

						# extrahiere den title pfad
						i = index($0, "> &nbsp;") + 8
				        	j = index(substr($0, i), "</a>") - 1
				        	title = substr($0, i, j)

						i = index($0, "html\\\">") + 7
				        	j = index(substr($0, i), "<") - 1
				        	extra = substr($0, i, j)
#print "4" extra

						if (extra == "")
						{
							i = index($0, "html\">") + 5
					        j = index(substr($0, i), "<") - 1
					        extra = substr($0, i, j)
#print "5" extra

						}
						if (extra ~ /tablemoviesindex2/)
							extra = ""

						if (extra ~ /PT>/)
							extra = ""

						if (newpage ~ /movie-/)
						{
							i = index(newpage, "movie-") + 6
					        j = index(substr(newpage, i), ".html") - 1
					        extra2 = substr(newpage, i, j)
						}
						if (newpage ~ /tvshows-/)
						{
							i = index(newpage, "tvshows-") + 8
					        j = index(substr(newpage, i), "-") - 1
					        extra2 = substr(newpage, i, j)
						}

#print 123 $0
#print "newpage" newpage
#print "extra" extra
#print "extra2" extra2
#print "title" title

						if (title != "")
						{
							pic = tolower(title)
							split(pic, a, ".")


							if (extra != "" )
							{
								# trim left "  Hallo   tester   "
								gsub(/^[ \t]+/,"",extra)
								# trim right "  Hallo   tester   "
								gsub(/[ \t]+$/,"",extra)
								# trim middle "  Hallo   tester   "
								gsub(/\t+/," / ",extra)
							}
							if (extra2 != "" )
							{
								# trim left "  Hallo   tester   "
								gsub(/^[ \t]+/,"",extra2)
								# trim right "  Hallo   tester   "
								gsub(/[ \t]+$/,"",extra2)
								# trim middle "  Hallo   tester   "
								gsub(/\t+/," / ",extra2)
							}
							if (extra != "" && extra2 != "")				
								title = title " (" extra2 " - " extra ")"
							else if(extra != "")
								title = title " (" extra ")"
							else if(extra2 != "")
								title = title " (" extra2 ")"

							# trim left "  Hallo   tester   "
							gsub(/^[ \t]+/,"",title)
							# trim right "  Hallo   tester   "
							gsub(/[ \t]+$/,"",title)
							# trim middle "  Hallo   tester   "
							gsub(/\t+/," / ",title)

							if ( pic == "" )
							{
					  			pic = "default"
							}
							else
							{
								pic = a[1]
								if ( pic == "streamclou" )
									pic = pic "d"
							}
							piccount += 1
							# 25. in naechste zeile springen
							# 26. \x27 = single quotes
							if ( newpage != "" )
								print title "#" SRC " " SRC " hoster \x27" newpage "\x27#http://openaaf.dyndns.tv/mediathek/menu/" pic ".jpg#" pic ".jpg#" NAME "#111"
							else
								print title "(ERROR)#" SRC " " SRC " hoster \x27" newpage "\x27#http://openaaf.dyndns.tv/mediathek/menu/" pic ".jpg#" pic ".jpg#" NAME "#111"
						}
						next
					}
				}
				/<div class="cf"><\/div>/ \
				{
					if ( suche == 1 )
					{
						suche = 0
					}
					next
				}

		# 29. schreibe alles in die list datei
		' >$TMP/$FILENAME.list
	fi
	# 30. gebe titan den list namen mit pfad zurueck
	echo "$TMP/$FILENAME.list"
#	cat "$TMP/$FILENAME.list"
}

hoster()
{
	rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
#	$curlbin $URL/$PAGE -o $TMP/cache.$PARSER.$INPUT.1 -A 'Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Maxthon/4.4.7.3000 Chrome/30.0.1599.101 Safari/537.36'
#	/tmp/localhoster/hoster.sh get $URL/$PAGE > $TMP/cache.$FILENAME.1
	$BIN /tmp/localhoster/cloudflare.py "$URL/$PAGE$NEXT$PAGE2" > $TMP/cache.$FILENAME.1

	STREAMURL=`cat $TMP/cache.$FILENAME.1 | sed 's!<a target="_blank" href="!\nstreamurl="!' | grep ^streamurl= | cut -d'"' -f2`

        if [ ! -z "$STREAMURL" ] && [ `echo "$STREAMURL" | grep ^"https://" | wc -l` -eq 0 ]; then
		STREAMURL="https://$STREAMURL"
	fi

	if [ `echo $STREAMURL | grep ^http | wc -l` -eq 0 ]; then
#		<iframe src="//www.rapidvideo.com/e/FIW59O2DED" width="730" height="460" frameborder="0" scrolling="no"></iframe><BR> <div id="underplayer">
		STREAMURL=`cat $TMP/cache.$FILENAME.1 | sed 's!<iframe src="!\nstreamurl="!' | grep ^streamurl= | cut -d'"' -f2 | tr ' ' '\n' | head -n1`
	fi

#	STREAMURL=`cat $TMP/cache.$PARSER.$INPUT.1 | sed 's!<a target="_blank" href="!\nstreamurl="!' | grep ^streamurl= | cut -d'"' -f2`
#	rm $TMP/cache.$PARSER.$INPUT.* > /dev/null 2>&1
	echo $STREAMURL
}

case $INPUT in
#	init) $INPUT;;
	mainmenu) $INPUT;;
	hosterlist) $INPUT;;
	hoster) $INPUT;;
	search) $INPUT;;
	searchtv) $INPUT;;
    	season) $INPUT;;
    	episode) $INPUT;;
	kino) $INPUT;;
	sorted) $INPUT;;
	genre) $INPUT;;
	sortedtv) $INPUT;;
	genretv) $INPUT;;
esac
