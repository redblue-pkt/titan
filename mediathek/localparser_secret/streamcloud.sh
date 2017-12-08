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
#	echo "Movies (A-Z)#$SRC $SRC sorted#http://atemio.dyndns.tv/mediathek/menu/Movies.jpg#Movies.jpg#$NAME#0" >>$TMP/$FILENAME.list
#	echo "Movies (Genre)#$SRC $SRC genre#http://atemio.dyndns.tv/mediathek/menu/Movies.genre.jpg#Movies.genre.jpg#$NAME#0" >>$TMP/$FILENAME.list
#	echo "Tv-Shows (A-Z)#$SRC $SRC sortedtv#http://atemio.dyndns.tv/mediathek/menu/tv-shows.jpg#tv-shows.jpg#$NAME#0" >>$TMP/$FILENAME.list
#	echo "Tv-Shows (Genre)#$SRC $SRC genretv#http://atemio.dyndns.tv/mediathek/menu/movie.genre.jpg#movie.genre.jpg#$NAME#0" >>$TMP/$FILENAME.list
#	echo "Search#$SRC $SRC search 'movies.php?list=search&search=%search%'#http://atemio.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >>$TMP/$FILENAME.list
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
			echo "$TITLE#$SRC $SRC search 'movies-genre-$NUMMER-' 1 '.html'#http://atemio.dyndns.tv/mediathek/menu/$filename.jpg#$filename.jpg#$NAME#0" >> $TMP/$FILENAME.list
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
			echo "$TITLE#$SRC $SRC searchtv 'tvshows-genre-$NUMMER-' 1 '.html'#http://atemio.dyndns.tv/mediathek/menu/$filename.jpg#$filename.jpg#$NAME#0" >> $TMP/$FILENAME.list
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

sortedtv()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		watchlist="0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z"

		rm $TMP/$FILENAME.list > /dev/null 2>&1

		for ROUND0 in $watchlist; do
			TITLE=`echo $ROUND0`
			filename=`echo $TITLE | tr [A-Z] [a-z]`
			echo "$TITLE#$SRC $SRC searchtv 'tvshows-all-$ROUND0-' 1 '.html'#http://atemio.dyndns.tv/mediathek/menu/$filename.jpg#$filename.jpg#$NAME#0" >> $TMP/$FILENAME.list
		done
	fi
  	echo "$TMP/$FILENAME.list"
}

searchtv()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		piccount=0

		$curlbin $URL/$PAGE$NEXT$PAGE2 -o $TMP/cache.$FILENAME.1

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
				PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
			fi
	
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/\C3x/g' -e 's/&ndash;/-/g' -e 's/&Auml;/\C3/g' -e 's/&Uuml;/\C3S/g' -e 's/&Ouml;/\C3/g' -e 's/&auml;/\E4/g' -e 's/&uuml;/\FC/g' -e 's/&ouml;/\F6/g' -e 's/&eacute;/\E9/g' -e 's/&egrave;/\E8/g' -e 's/%F6/\F6/g' -e 's/%FC/\FC/g' -e 's/%E4/\E4/g' -e 's/%26/&/g' -e 's/%C4/\C3/g' -e 's/%D6/\C3/g' -e 's/%DC/\C3S/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
	
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
			LINE="Page ($NEXTPAGE/$pages)#$SRC $SRC searchtv '$PAGE' $NEXTPAGE '$PAGE2'#http://atemio.dyndns.tv/mediathek/menu/next.jpg#next.jpg#$NAME#0"
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

		$curlbin $URL/$PAGE$NEXT$PAGE2 -o $TMP/cache.$FILENAME.1

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
				PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
			fi
	
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/\C3x/g' -e 's/&ndash;/-/g' -e 's/&Auml;/\C3/g' -e 's/&Uuml;/\C3S/g' -e 's/&Ouml;/\C3/g' -e 's/&auml;/\E4/g' -e 's/&uuml;/\FC/g' -e 's/&ouml;/\F6/g' -e 's/&eacute;/\E9/g' -e 's/&egrave;/\E8/g' -e 's/%F6/\F6/g' -e 's/%FC/\FC/g' -e 's/%E4/\E4/g' -e 's/%26/&/g' -e 's/%C4/\C3/g' -e 's/%D6/\C3/g' -e 's/%DC/\C3S/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
	
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
			LINE="Page ($NEXTPAGE/$pages)#$SRC $SRC season '$PAGE' $NEXTPAGE '$PAGE2'#http://atemio.dyndns.tv/mediathek/menu/next.jpg#next.jpg#$NAME#0"
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

		$curlbin $URL/$PAGE$NEXT$PAGE2 -o $TMP/cache.$FILENAME.1

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
				PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
			fi
	
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/\C3x/g' -e 's/&ndash;/-/g' -e 's/&Auml;/\C3/g' -e 's/&Uuml;/\C3S/g' -e 's/&Ouml;/\C3/g' -e 's/&auml;/\E4/g' -e 's/&uuml;/\FC/g' -e 's/&ouml;/\F6/g' -e 's/&eacute;/\E9/g' -e 's/&egrave;/\E8/g' -e 's/%F6/\F6/g' -e 's/%FC/\FC/g' -e 's/%E4/\E4/g' -e 's/%26/&/g' -e 's/%C4/\C3/g' -e 's/%D6/\C3/g' -e 's/%DC/\C3S/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
	
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
			LINE="Page ($NEXTPAGE/$pages)#$SRC $SRC search '$PAGE' $NEXTPAGE '$PAGE2'#http://atemio.dyndns.tv/mediathek/menu/next.jpg#next.jpg#$NAME#0"
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

		$curlbin $URL/$PAGE$NEXT$PAGE2 -o $TMP/cache.$FILENAME.1

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
				PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
			fi
	
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/\C3x/g' -e 's/&ndash;/-/g' -e 's/&Auml;/\C3/g' -e 's/&Uuml;/\C3S/g' -e 's/&Ouml;/\C3/g' -e 's/&auml;/\E4/g' -e 's/&uuml;/\FC/g' -e 's/&ouml;/\F6/g' -e 's/&eacute;/\E9/g' -e 's/&egrave;/\E8/g' -e 's/%F6/\F6/g' -e 's/%FC/\FC/g' -e 's/%E4/\E4/g' -e 's/%26/&/g' -e 's/%C4/\C3/g' -e 's/%D6/\C3/g' -e 's/%DC/\C3S/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
	
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
			LINE="Page ($NEXTPAGE/$pages)#$SRC $SRC search '$PAGE' $NEXTPAGE '$PAGE2'#http://atemio.dyndns.tv/mediathek/menu/next.jpg#next.jpg#$NAME#0"
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

#			TITLE=`echo $ROUND | sed 's!title=!\nfound=>!g' | grep ^found= | cut -d'"' -f2 | tail -n1 | sed 's/ kostenlos//'`
#			NEWPAGE=`echo $ROUND | sed 's!<a href=!\nfound=!g' | grep ^found= | cut -d'"' -f2 | tail -n1`

#echo $ROUND
#echo $ID
#echo $PIC
#echo $TITLE
#echo $NEWPAGE

#exit
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
rm $TMP/$FILENAME.list
#	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin "$PAGE" -o $TMP/cache.$FILENAME.1
		cat $TMP/cache.$FILENAME.1 | grep iframe | sed -nr 's/.*src="([^"]+)".*/\1/p' >$TMP/cache.$FILENAME.2

		while read -u 3 ROUND; do
#echo ROUND $ROUND
			NEWPAGE="$ROUND"
			TITLE=`echo $ROUND | sed -nr 's/.*:\/\/([^\/]+)\/.*/\1/p'` 		
#echo TITLE $TITLE
			$curlbin "$NEWPAGE" -o $TMP/cache.$FILENAME.4

#			<a class='play_container' href='http://www.vodlocker.to/embed?t=Die+Eisk%C3%B6nigin%3A+Olaf+taut+auf&y=2017&lang=de&referrer=link'>
			cat $TMP/cache.$FILENAME.4 | grep play_container | sed -nr "s/.*href='([^']+)'.*/\1/p" >$TMP/cache.$FILENAME.4.url1
			URL=`cat $TMP/cache.$FILENAME.4.url1`
			if [ ! -z "$URL" ];then
				TITLE=`echo $URL | sed -nr 's/.*:\/\/([^\/]+)\/.*/\1/p'` 		
				NEWPAGE=$URL
			fi
#echo URL2 $URL
#echo TITLE2 $TITLE

if [ ! -z "$URL" ];then
			$curlbin "$URL" -o $TMP/cache.$FILENAME.5

#			$("#hostname").html("<a href='http://openload.co/embed/vq1HFMJ5vAo/detroit.SD-spectre.mkv' target='_blank' style='color:gold; text-decoration:underline;'>openload.co</a>");
			cat $TMP/cache.$FILENAME.5 | grep "#hostname" | sed -nr "s/.*a href='([^']+)'.*/\1/p" | grep -v "+link+" >$TMP/cache.$FILENAME.5.url1
			URL=`cat $TMP/cache.$FILENAME.5.url1`
			if [ ! -z "$URL" ];then
				TITLE=`echo $URL | sed -nr 's/.*:\/\/([^\/]+)\/.*/\1/p'` 		
			fi 		

#echo URL3 $URL
#echo TITLE3 $TITLE

#if [ ! -z "$URL" ];then
#			if [ -z "$URL" ];then
#
#				cat $TMP/cache.$FILENAME.5 | sed 's/<source src=/\nfound=/g' | grep ^found= | cut -d"'" -f2 >$TMP/cache.$FILENAME.5.url2
#				URL=`cat $TMP/cache.$FILENAME.5.url2`
#				TITLE=`echo $URL | sed -nr 's/.*:\/\/([^\/]+)\/.*/\1/p'` 		
#			fi
#echo URL4 $URL
#echo TITLE4 $TITLE
			if [ ! -z "$URL" ];then
				TITLE=`echo $URL | sed -nr 's/.*:\/\/([^\/]+)\/.*/\1/p'` 		
				NEWPAGE=$URL
			fi
#fi

		#echo 111111111
#			if [ -z "$URL" ];then
		#echo 22222222222
#				cat $TMP/cache.$FILENAME.4 | grep "#hostname" | sed -nr "s/.*a href='([^']+)'.*/\1/p" >$TMP/cache.$FILENAME.5.url3
#				URL=`cat $TMP/cache.$FILENAME.5.url3`

#				TITLE=`echo $URL | sed -nr 's/.*:\/\/([^\/]+)\/.*/\1/p'` 		

#				if [ ! -z "$URL" ];then
#					/tmp/localhoster/hoster.sh get "$URL" > $TMP/cache.hoster.$hoster.url3
#					URL=`cat $TMP/cache.hoster.$hoster.url3`
#				fi
#			fi
fi

			if [ ! -z "$TITLE" ] && [ "$TITLE" != " " ] && [ ! -z "$NEWPAGE" ];then
				PIC=`echo $TITLE | tr [A-Z] [a-z] | sed 's/www.//' | cut -d"." -f1 | sed 's/streamclou/streamcloud/'`
				LINE="$TITLE#$SRC $SRC hoster '$NEWPAGE'#http://atemio.dyndns.tv/mediathek/menu/$PIC.jpg#$PIC.jpg#$NAME#111"

				echo "$LINE" >> $TMP/$FILENAME.list
			fi
#echo $PIC
#echo $TITLE
#echo $NEWPAGE
		done 3<$TMP/cache.$FILENAME.2
#		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
#	fi
	echo "$TMP/$FILENAME.list"
}


hoster()
{
	rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
#	$curlbin $URL/$PAGE -o $TMP/cache.$PARSER.$INPUT.1 -A 'Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Maxthon/4.4.7.3000 Chrome/30.0.1599.101 Safari/537.36'
#	/tmp/localhoster/hoster.sh get $URL/$PAGE > $TMP/cache.$FILENAME.1


#	/tmp/localhoster/hoster.sh get "$PAGE" > $TMP/cache.$FILENAME.1

#	STREAMURL=`cat $TMP/cache.$FILENAME.1`
	STREAMURL="$PAGE"
	echo $STREAMURL
}


case $INPUT in
	init) $INPUT;;
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
