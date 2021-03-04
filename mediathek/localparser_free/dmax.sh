#!/bin/sh
# first sh box parser for titannit mfg obi

SRC=$1
INPUT=$2
CURPAGE=$3
MAXPAGE=$4
PAGE=$5

URL=https://www.dmax.de
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME="DMAX"

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
#	if [ "$ARCH" == "arm" ] || [ "$ARCH" == "mipsel" ] || ([ "$ARCH" == "sh4" ] && [ -e "/etc/.beta" ]);then
		rm -rf $TMP > /dev/null 2>&1
		echo "$NAME ($TYPE)#$SRC $SRC mainmenu#http://openaaf.dyndns.tv/mediathek/menu/$PARSER.jpg#$PARSER.jpg#TiThek#0"
#	fi
}

mainmenu()
{
	echo "Genres#$SRC $SRC genre 1 0 'themen'#http://openaaf.dyndns.tv/mediathek/menu/all-newfirst.jpg#all-newfirst.jpg#$NAME#0" > $TMP/$PARSER.$INPUT.list
	echo "Search#$SRC $SRC series 1 0 '/api/search?query='#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >> $TMP/$PARSER.$INPUT.list

	if [ -e "$TMP/$PARSER.new.list" ] ; then
		rm $TMP/$PARSER.new.list
	fi
	if [ "`echo $TMP/$PARSER.search.*.list`" != "$TMP/$PARSER.search.*.list" ] ; then
		rm $TMP/$PARSER.search.*.list
	fi
	if [ -e "$TMP/$PARSER.page.list" ] ; then
		rm $TMP/$PARSER.page.list
	fi

	echo "$TMP/$PARSER.$INPUT.list"
}

genre()
{
	if [ ! -e "$TMP/$PARSER.$INPUT.list" ] ; then
		$curlbin $URL/$PAGE -o $TMP/cache.$PARSER.$INPUT.1
		cat $TMP/cache.$PARSER.$INPUT.1 | sed 's!sendungen"},{"submenu!sendungen"},\n{"submenu!g' | sed -n '/sendungen"},/{n;p;}' | cut -d "]" -f1 | sed -e 's!"title"!\n"title"!g' | sed "/submenu/d" > /$TMP/cache.$PARSER.$INPUT.2
		while read -u 3 ROUND; do
			TITLE=`echo $ROUND | cut -d '"' -f4 | sed 's!"title":!!g' | cut -d '"' -f2`
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/&%8211;/-/g' -e "s/&%8217;/'/g"`
			NEWPAGE=`echo $ROUND | sed 's/"url/\n"url/g' | grep ^'"url"' | sed 's!"url"!!g'| cut -d '"' -f2 | sed 's!themen!api/genres!g'`

			if [ -z  "$PIC" ]; then  
				PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
				TMPPIC="default.jpg"
			else
				TMPPIC=$PARSER'_'`echo $PIC | tr '/' '\n' | tail -n1`
			fi

			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
				if [ `cat $TMP/$PARSER.$INPUT.list | grep ^"$NEWPAGE" | wc -l` -eq 0 ];then
					if [ ! -e $TMP/$PARSER.$INPUT.list ];then
						touch $TMP/$PARSER.$INPUT.list
					fi
					LINE="$TITLE#$SRC $SRC series 1 0 $NEWPAGE#$PIC#$TMPPIC#$NAME#0"
					echo "$LINE" >> $TMP/$PARSER.$INPUT.list
				fi
			fi
		done 3<$TMP/cache.$PARSER.$INPUT.2
		rm $TMP/cache.$PARSER.$INPUT.* > /dev/null 2>&1
	fi

	if [ "`echo $TMP/$PARSER.series.*.list`" != "$TMP/$PARSER.series.*.list" ] ; then
		rm $TMP/$PARSER.series.*.list
	fi

	
	echo "$TMP/$PARSER.$INPUT.list"
}

series()
{
	if [ ! -e "$TMP/$PARSER.$INPUT.$CURPAGE.list" ] ; then
		
		if [ "$CURPAGE" -eq "1" ] ; then
			NEWPAGE=$PAGE
		else
			NEWPAGE=$PAGE'?page='$CURPAGE
		fi
		
		$curlbin $URL$NEWPAGE -o $TMP/cache.$PARSER.$INPUT.$CURPAGE.1

		cat $TMP/cache.$PARSER.$INPUT.$CURPAGE.1 | sed 's!"id"!\n"id"!g' | sed '/alias/d' > /$TMP/cache.$PARSER.$INPUT.$CURPAGE.2
		echo >> $TMP/cache.$PARSER.$INPUT.$CURPAGE.2
		while read -u 3 ROUND; do
			TITLE=`echo $ROUND | sed 's/"title"/\n"title"/g' | grep '"title"' | cut -d ',' -f1 | sed 's!"title":!!g'`
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/&%8211;/-/g' -e "s/&%8217;/'/g"`
			PIC=`echo $ROUND | sed 's/"src"/\n"src"/g' | sed 's!"src":!!g' | sed 's!id!!g' | grep "http[^ ]*"| cut -d '"' -f2`
			NEWPAGE=api/show-detail/`echo $ROUND | sed 's/"id"/\n"id"/' | grep ^'"id":' | sed 's!"id":!!g'| cut -d '"' -f2`

			if [ -z  "$PIC" ]; then  
				PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
				TMPPIC="default.jpg"
			else
				TMPPIC=$PARSER_`echo $PIC | tr '/' '\n' | tail -n1`
			fi

			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
					LINE="$TITLE#$SRC $SRC season 1 0 '$NEWPAGE'#$PIC#$TMPPIC#$NAME#0"
					echo "$LINE" >> $TMP/$PARSER.$INPUT.$CURPAGE.list
				
			fi
		done 3<$TMP/cache.$PARSER.$INPUT.$CURPAGE.2

		MAXPAGE=$(grep '"totalPages"' $TMP/cache.$PARSER.$INPUT.$CURPAGE.2 |sed 's!"totalPages!\n"totalPages!g' | sed -n '/meta/{n;p;}' | sed 's!"totalPages":!!g' | cut -d '}' -f1)

		if [ $MAXPAGE != $CURPAGE ];then
		
		NEWPAGE=`expr $CURPAGE + 1`
		echo "Page $NEWPAGE#$SRC $SRC series $NEWPAGE $MAXPAGE '$PAGE'#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.$CURPAGE.list
		fi
		

		rm $TMP/cache.$PARSER.$INPUT.$CURPAGE.* > /dev/null 2>&1
	fi
	
	if [ "`echo $TMP/$PARSER.season.*.list`" != "$TMP/$PARSER.season.*.list" ] ; then
		rm $TMP/$PARSER.season.*.list
	fi
	
	echo "$TMP/$PARSER.$INPUT.$CURPAGE.list"
}

season()
{
	if [ ! -e "$TMP/$PARSER.$INPUT.$CURPAGE.list" ] ; then
		
		if [ "$CURPAGE" -eq "1" ] ; then
			NEWPAGE=$PAGE
		else
			NEWPAGE=$PAGE'?page='`expr $CURPAGE - 1`
		fi
		
		$curlbin $URL/$NEWPAGE -o $TMP/cache.$PARSER.$INPUT.$CURPAGE.1

		cat $TMP/cache.$PARSER.$INPUT.$CURPAGE.1 | sed 's!"videos"!\n"videos"!g' | sed '/"show"/d' | sed 's!"id"!\n"id"!g' | sed '/"videos"/d' > /$TMP/cache.$PARSER.$INPUT.$CURPAGE.2
		echo >> $TMP/cache.$PARSER.$INPUT.$CURPAGE.2
		while read -u 3 ROUND; do
			TITLE=`echo $ROUND | sed 's!"name"!\n"name"!1' | grep ^'"name":' | cut -d '"' -f4`
			TITLE=`echo $TITLE | sed -e 's/-/ /g' -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/&%8211;/-/g' -e "s/&%8217;/'/g"`
			PIC=`echo $ROUND | sed 's!"src"!\n"src"!1' | grep '"src":' | cut -d '"' -f4`
			NEWPAGE=https://sonic-eu1-prod.disco-api.com/playback/videoPlaybackInfo/`echo $ROUND | sed 's!"id"!\n"id"!g' | grep ^'"id":' | cut -d '"' -f4`

			if [ -z  "$PIC" ]; then  
				PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
				TMPPIC="default.jpg"
			else
				TMPPIC=$PARSER'_'`echo $PIC | tr '/' '\n' | tail -n1`
			fi

			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
				
					LINE="$TITLE#$SRC $SRC play 0 0 '$NEWPAGE'#$PIC#$TMPPIC#$NAME#111"
					echo "$LINE" >> $TMP/$PARSER.$INPUT.$CURPAGE.list
				
			fi
		done 3<$TMP/cache.$PARSER.$INPUT.$CURPAGE.2

		rm $TMP/cache.$PARSER.$INPUT.$CURPAGE.* > /dev/null 2>&1
	fi
	
	if [ "`echo $TMP/$PARSER.episode.*.list`" != "$TMP/$PARSER.episode.*.list" ] ; then
		rm $TMP/$PARSER.episode.*.list
		rm /tmp/tithek/$PARSER.episode.* > /dev/null 2>&1
	
	fi
	
	echo "$TMP/$PARSER.$INPUT.$CURPAGE.list"
}

play()
{
	if [ -e "$TMP/$PARSER.$INPUT.list" ] ; then
		rm $TMP/$PARSER.$INPUT.list
	fi
	
	TOKEN=$($curlbin $URL | sed 's!"sonicToken"!\n"sonicToken"!g' | grep ^'"sonicToken":' | cut -d '"' -f4)
	curl -H "Authorization: Bearer $TOKEN" $PAGE -o $TMP/cache.$PARSER.$INPUT.1a

	cat $TMP/cache.$PARSER.$INPUT.1a | grep -A 2 'hls' | grep '"url"' | cut -d '"' -f4 > $TMP/cache.$PARSER.$INPUT.2
#	cat $TMP/cache.$PARSER.$INPUT.1a | grep -A 2 'dash' | grep '"url"' | cut -d '"' -f4 >> $TMP/cache.$PARSER.$INPUT.2

	cat $TMP/cache.$PARSER.$INPUT.2 > $TMP/$PARSER.playlist.list

	echo "$TMP/$PARSER.playlist.list"
}

case $INPUT in
#	init) $INPUT;;
	mainmenu) $INPUT;;
	genre) $INPUT;;
	series) $INPUT;;
	season) $INPUT;;
	play) $INPUT;;
esac
