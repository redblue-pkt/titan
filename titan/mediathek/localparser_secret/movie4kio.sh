#!/bin/sh
# first sh box parser for titannit mfg obi

SRC=$1
INPUT=$2
PAGE=$3
NEXT=$4
PAGE2=$5
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`

#URL=http://movie4k.to
URL=https://movie4k.io
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME=Movie4kio

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
	echo "Search#$SRC $SRC search 'movies.php?list=search&search=%search%'#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >>$TMP/$FILENAME.list
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
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		piccount=0
	#	/tmp/localhoster/hoster.sh get $URL/$PAGE > $TMP/cache.$PARSER.$INPUT.1
#		$curlbin "$URL/$PAGE" -o "$TMP/cache.$FILENAME.1"
		$BIN /tmp/localhoster/cloudflare.py "$URL/$PAGE" > $TMP/cache.$FILENAME.1

		cat $TMP/cache.$FILENAME.1 | grep ^"<a href=" | grep "<img src=" >$TMP/cache.$FILENAME.2

		while read -u 3 ROUND; do
			ID=`echo $ROUND | cut -d'"' -f1 | tail -n1`
			PIC=`echo $ROUND | sed 's!<img src=!\nurl=!g' | grep ^url= | cut -d'"' -f2 | tail -n1`
			TITLE=`echo $ROUND | sed 's!title=!\nfound=>!g' | grep ^found= | cut -d'"' -f2 | tail -n1 | sed 's/ kostenlos//'`
			NEWPAGE=`echo $ROUND | sed 's!<a href=!\nfound=!g' | grep ^found= | cut -d'"' -f2 | tail -n1`

			if [ -z "$PIC" ]; then
				PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
			fi

			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/\C3x/g' -e 's/&ndash;/-/g' -e 's/&Auml;/\C3/g' -e 's/&Uuml;/\C3S/g' -e 's/&Ouml;/\C3/g' -e 's/&auml;/ä/g' -e 's/&uuml;/ü/g' -e 's/&ouml;/ö/g' -e 's/&eacute;/é/g' -e 's/&egrave;/è/g' -e 's/%F6/ö/g' -e 's/%FC/ü/g' -e 's/%E4/ä/g' -e 's/%26/&/g' -e 's/%C4/\C3/g' -e 's/%D6/\C3/g' -e 's/%DC/\C3S/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`

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
#		/tmp/localhoster/hoster.sh get $URL/$PAGE > $TMP/cache.$FILENAME.1
		$BIN /tmp/localhoster/cloudflare.py "$URL/$PAGE" > $TMP/cache.$FILENAME.1

		cat $TMP/cache.$FILENAME.1 | grep ^"links\[" >$TMP/cache.$FILENAME.2

		while read -u 3 ROUND; do

			NEWPAGE="`echo $ROUND | sed 's!<a href=!\nfound=!g' | grep found= | cut -d '"' -f2 | sed 's/;/%3B/g' | head -n1`"
			TITLE=`echo $ROUND | sed 's!&nbsp;!\nfound=<!g' | grep ^"found=<" | cut -d"<" -f2` 		
	
			if [ ! -z "$TITLE" ] && [ "$TITLE" != " " ] && [ ! -z "$NEWPAGE" ];then
				PIC=`echo $TITLE | tr [A-Z] [a-z] | cut -d"." -f1 | sed 's/streamclou/streamcloud/'`
	#			LINE="$TITLE#$SRC $SRC hoster $NEWPAGE '--referer $URL/$PAGE'#http://openaaf.dyndns.tv/mediathek/menu/$PIC.jpg#$PIC.jpg#$NAME#111"
				LINE="$TITLE#$SRC $SRC hoster $NEWPAGE#http://openaaf.dyndns.tv/mediathek/menu/$PIC.jpg#$PIC.jpg#$NAME#111"

				echo "$LINE" >> $TMP/$FILENAME.list
			fi
		done 3<$TMP/cache.$FILENAME.2
#		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi
	echo "$TMP/$FILENAME.list"
}


hoster()
{
	rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
#	$curlbin $URL/$PAGE -o $TMP/cache.$PARSER.$INPUT.1 -A 'Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Maxthon/4.4.7.3000 Chrome/30.0.1599.101 Safari/537.36'
#	/tmp/localhoster/hoster.sh get $URL/$PAGE > $TMP/cache.$FILENAME.1
	$BIN /tmp/localhoster/cloudflare.py "$URL/$PAGE$NEXT$PAGE2" > $TMP/cache.$FILENAME.1

	STREAMURL=`cat $TMP/cache.$FILENAME.1 | sed 's!<a target="_blank" href="!\nstreamurl="!' | grep ^streamurl= | cut -d'"' -f2`

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
