#!/bin/sh
# first sh box parser for titannit mfg obi

SRC=$1
INPUT=$2
PAGE=$3
NEXT=$4
PAGE2=$5
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`

URL=http://www.clipfish.de
NAME="ClipFish"

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
	rm -rf $TMP > /dev/null 2>&1
	echo "$NAME ($TYPE)#$SRC $SRC mainmenu#http://openaaf.dyndns.tv/mediathek/menu/$PARSER.jpg#$PARSER.jpg#TiThek#0"
}

mainmenu()
{
	echo "Kategorien#$SRC $SRC category '/devmobileapp/metachannels' 1#http://openaaf.dyndns.tv/mediathek/menu/category.jpg#category.jpg#$NAME#0" >$TMP/$FILENAME.list
	echo "Suchen#$SRC $SRC search '/devmobileapp/searchvideos/%search%/mostrecent/' 1 '/16'#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >>$TMP/$FILENAME.list
	echo "$TMP/$FILENAME.list"
}

category()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - $URL$PAGE | sed -e 's!</html>\[{"id"!</html>\[{"id"\nfound={"id"!g' -e 's/}],/\nfound=/g' | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT \
		'
			# BEGIN variable setzen
			BEGIN
			{
				# setzt suchvariable auf 0 vor dem start
				piccount = 0
				pages = "1"
			}
			/"id"/ \
			{
				i = index($0, "\"id\":\"") + 6
	            j = index(substr($0, i), "\",\"") - 1
				id = substr($0, i, j)

				i = index($0, "\"specials\":\"") + 9
	            j = index(substr($0, i), "\",\"") - 1
				title1 = substr($0, i, j)

				i = index($0, "\"title\":\"") + 9
	            j = index(substr($0, i), "\",\"") - 1
				title = substr($0, i, j)

				i = index($0, "\"media_length\":\"") + 16
	            j = index(substr($0, i), "\",\"") - 1
				duration = substr($0, i, j)

				i = index($0, "\"id\":\"") + 6
	            j = index(substr($0, i), "\"") - 1
				newpage = substr($0, i, j)

				gsub(/\\/, "", newpage, newpage)

	           	pic = "http://openaaf.dyndns.tv/mediathek/menu/"

				piccount += 1
				print title "#" SRC " " SRC " submenu \x27/devmobileapp/metachannels\x27 " newpage "#" pic tolower(title) ".jpg#" PICNAME "." piccount ".jpg#" NAME "#0"

				next
			}
			END
			{
#				if (curpage != pages)
#					print "Page (" NEXT + 1 "/" pages ")#" SRC " " SRC " " INPUT " \x27" PAGE "\x27 " NEXT + 1 "#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#" NAME "#0"
			}
		# schreibe alles in die list datei
		' >$TMP/$FILENAME.list
	fi
	# gebe titan den list namen mit pfad zurueck
	echo "$TMP/$FILENAME.list"
}

submenu()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - $URL$PAGE | sed -e 's!</html>\[{"id"!</html>\[{"id"\nfound1={"id"!g' -e 's/}],/\nfound1=/g' | grep \"id\":\"$NEXT\" | sed -e 's/:\[{/\nfound=}/g' -e 's/},{/\nfound=},{/g' | grep found= | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT \
		'
			# BEGIN variable setzen
			BEGIN
			{
				# setzt suchvariable auf 0 vor dem start
				piccount = 0
				pages = "1"
			}
			/"id"/ \
			{
				i = index($0, "\"id\":\"") + 6
	            j = index(substr($0, i), "\",\"") - 1
				id = substr($0, i, j)

				if(id == "=},{")
				{
					i = index($0, "\"id\":") + 5
		            j = index(substr($0, i), ",\"") - 1
					id = substr($0, i, j)
				}

				i = index($0, "\"title\":\"") + 9
	            j = index(substr($0, i), "\",\"") - 1
				title = substr($0, i, j)

				i = index($0, "\"id\":\"") + 6
	            j = index(substr($0, i), "\"") - 1
				newpage = substr($0, i, j)

				if(newpage == "=},{")
				{
					i = index($0, "\"id\":") + 5
		            j = index(substr($0, i), ",\"") - 1
					newpage = substr($0, i, j)
				}

				gsub(/\\/, "", newpage, newpage)

				i = index($0, "\"img_thumbnail\":\"") + 17
	            j = index(substr($0, i), "\"") - 1
				pic = substr($0, i, j)
				gsub(/\\/, "", pic, pic)

				piccount += 1
				if ( pic != "1" )
				if ( pic ~ /http/ )
					print title "#" SRC " " SRC " search \x27/devmobileapp/specialvideos/" newpage "/mostrecent/\x27 1 \x27/16\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#0"

				next
			}
			END
			{
#				if (curpage != pages)
#					print "Page (" NEXT + 1 "/" pages ")#" SRC " " SRC " " INPUT " \x27" PAGE "\x27 " NEXT + 1 "#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#" NAME "#0"
			}
		# schreibe alles in die list datei
		' >$TMP/$FILENAME.list
	fi
	# gebe titan den list namen mit pfad zurueck
	echo "$TMP/$FILENAME.list"
}

search()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - $URL$PAGE$NEXT$PAGE2 | sed 's/{"video_id":/\n{"video_id":/g' | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT \
		'
			# BEGIN variable setzen
			BEGIN
			{
				# setzt suchvariable auf 0 vor dem start
				piccount = 0
				pages = "1"
			}
			/"list_page_limit"/ \
			{
					i = index($0, "\"list_page_limit\":\"") + 19
		            j = index(substr($0, i), "\",\"") - 1
					pages = substr($0, i, j)
			}
			/"video_id"/ \
			{
				i = index($0, "\"title\":\"") + 9
	            j = index(substr($0, i), "\",\"") - 1
				title = substr($0, i, j)

				i = index($0, "\"media_length\":\"") + 16
	            j = index(substr($0, i), "\",\"") - 1
				duration = substr($0, i, j)

				i = index($0, "\"pubDate\":\"") + 11
	            j = index(substr($0, i), "\",\"") - 1
				extra = substr($0, i, j)

				i = index($0, "\"video_url_edge_quality\":\"") + 26
	            j = index(substr($0, i), "\"") - 1
				newpage = substr($0, i, j)

				if (newpage == "")
				{
					i = index($0, "\"video_url_wifi_quality\":\"") + 26
		            j = index(substr($0, i), "\"") - 1
					newpage = substr($0, i, j)
				}
				if (newpage == "")
				{
					i = index($0, "\"media_videourl_hls\":\"") + 22
		            j = index(substr($0, i), "\"") - 1
					newpage = substr($0, i, j)
				}
				gsub(/\\/, "", newpage, newpage)

				i = index($0, "\"media_content_thumbnail_large\":\"") + 33
	            j = index(substr($0, i), "\"") - 1
				pic = substr($0, i, j)
				gsub(/\\/, "", pic, pic)

				piccount += 1
				if ( pic == "" )
				{
	            	pic = "http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
				}
				print title " (" duration "s) (" extra ")#" newpage "#" pic "#" PICNAME "." piccount ".jpg#" NAME "#2"

				next
			}
			END
			{
#				if (curpage != pages)
					print "Page (" NEXT + 1 "/" pages ")#" SRC " " SRC " " INPUT " \x27" PAGE "\x27 " NEXT + 1 " \x27/16\x27#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#" NAME "#0"


			}
		# schreibe alles in die list datei
		' >$TMP/$FILENAME.list
	fi
	# gebe titan den list namen mit pfad zurueck
	echo "$TMP/$FILENAME.list"
}

case $INPUT in
#	init) $INPUT;;
	mainmenu) $INPUT;;
	search) $INPUT;;
	category) $INPUT;;
	submenu) $INPUT;;	
esac
