#!/bin/sh
# first sh box parser for titannit mfg obi

. /tmp/localhoster/hoster.sh

SRC=$1
INPUT=$2
PAGE=$3
NEXT=$4
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`

FILENAME="`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'` $INPUT $PAGE $NEXT"
FILENAME="`echo $FILENAME | sed -e 's/\&\+/./g' -e 's#\/\+#.#g' -e 's/\?\+/./g' -e 's/;\+/./g' -e 's/=\+/./g' -e 's/ \+/./g' -e 's/\.\+/./g'`"
PICNAME=`echo $FILENAME`

if [ -z "$FILENAME" ]; then
	FILENAME=none
fi

#URL=http://www.myspass.de
URL=http://m.myspass.de
NAME="MySpass"

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
	echo "Ganze Folgen#$SRC $SRC season 'api/index.php?command=formats' 1#http://atemio.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#0" >$TMP/$FILENAME.list
	echo "Suchen#$SRC $SRC search 'frontend/json/search-videos.php?QUERY_STRING=%search%' 1#http://atemio.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >>$TMP/$FILENAME.list
	echo "$TMP/$FILENAME.list"
}

season()
{
#	if [ ! -e "$TMP/$FILENAME.list" ]; then
#echo $URL/$PAGE
		$curlbin -o - $URL/$PAGE | sed 's/{"format_id":/\n{"format_id":/g' | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT \
		'
			# BEGIN variable setzen
			BEGIN
			{
				# setzt suchvariable auf 0 vor dem start
				piccount = 0
				pages = "1"
			}
			/"format_id"/ \
			{
#			print "1111111" $0

				i = index($0, "\"format_id\":\"") + 13
	            j = index(substr($0, i), "\",\"") - 1
				id = substr($0, i, j)
#			print "id: " id

				i = index($0, "\"format\":\"") + 10
	            j = index(substr($0, i), "\",\"") - 1
				title = substr($0, i, j)
#			print "title: " title

				i = index($0, "\"number_of_seasons\":\"") + 21
	            j = index(substr($0, i), "\",\"") - 1
				extra = substr($0, i, j)
#			print "extra: " extra

				i = index($0, "\"node_id\":\"") + 11
	            j = index(substr($0, i), "\"") - 1
				newpage = substr($0, i, j)
				gsub(/\\/, "", newpage, newpage)

#			print "newpage: " newpage

				i = index($0, "\"96x54\":\"") + 9
	            j = index(substr($0, i), "\"") - 1
				pic = substr($0, i, j)
				gsub(/\\/, "", pic, pic)
#			print "pic: " pic

				piccount += 1
				if ( pic == "" )
				{
	            	pic = "http://atemio.dyndns.tv/mediathek/menu/default.jpg"
				}
#				http://m.myspass.de/api/index.php?command=seasonslist&id=
#				print title " (" extra " Staffeln)#" SRC " " SRC " hoster \x27" newpage "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#111"
				print title " (" extra " Staffeln)#" SRC " " SRC " year \x27api/index.php?command=seasonslist&id=" id "\x27#" pic "#http:" PICNAME "." piccount ".jpg#" NAME "#0"

				next
			}
			END
			{
#				if (curpage != pages)
#					print "Page (" NEXT + 1 "/" pages ")#" SRC " " SRC " " INPUT " \x27" PAGE "\x27 " NEXT + 1 "#http://atemio.dyndns.tv/mediathek/menu/next.jpg#next.jpg#" NAME "#0"
			}
		# schreibe alles in die list datei
		' >$TMP/$FILENAME.list
#	fi
	# gebe titan den list namen mit pfad zurueck
	echo "$TMP/$FILENAME.list"
}

year()
{
#	if [ ! -e "$TMP/$FILENAME.list" ]; then
#echo $URL/$PAGE
		$curlbin -o - $URL/$PAGE | sed 's/{"season_id":/\n{"season_id":/g' | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT \
		'
			# BEGIN variable setzen
			BEGIN
			{
				# setzt suchvariable auf 0 vor dem start
				piccount = 0
				pages = "1"
			}
			/"season_id"/ \
			{
#			print "1111111" $0

				i = index($0, "\"season_id\":\"") + 13
	            j = index(substr($0, i), "\",\"") - 1
				id = substr($0, i, j)
#			print "id: " id

				i = index($0, "\"season_number\":\"") + 17
	            j = index(substr($0, i), "\",\"") - 1
				title = substr($0, i, j)
#			print "title: " title

				i = index($0, "\"season_name\":\"") + 15
	            j = index(substr($0, i), "\",\"") - 1
				extra = substr($0, i, j)
#			print "extra: " extra

				i = index($0, "\"node_id\":\"") + 11
	            j = index(substr($0, i), "\"") - 1
				newpage = substr($0, i, j)
				gsub(/\\/, "", newpage, newpage)

#			print "newpage: " newpage

				i = index($0, "\"original_image\":\"") + 18
	            j = index(substr($0, i), "\"") - 1
				pic = substr($0, i, j)
				gsub(/\\/, "", pic, pic)
#			print "pic: " pic

				piccount += 1
				if ( pic == "" )
				{
	            	pic = "http://atemio.dyndns.tv/mediathek/menu/default.jpg"
				}
#				http://m.myspass.de/api/index.php?command=seasonepisodes&id=285
#				print title " (" extra " Staffeln)#" SRC " " SRC " hoster \x27" newpage "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#111"
				print "Staffel " title " (" extra ")#" SRC " " SRC " episodes \x27api/index.php?command=seasonepisodes&id=" id "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#0"

				next
			}
			END
			{
#				if (curpage != pages)
#					print "Page (" NEXT + 1 "/" pages ")#" SRC " " SRC " " INPUT " \x27" PAGE "\x27 " NEXT + 1 "#http://atemio.dyndns.tv/mediathek/menu/next.jpg#next.jpg#" NAME "#0"
			}
		# schreibe alles in die list datei
		' >$TMP/$FILENAME.list
#	fi
	# gebe titan den list namen mit pfad zurueck
	echo "$TMP/$FILENAME.list"
}

episodes()
{
#	if [ ! -e "$TMP/$FILENAME.list" ]; then
#echo $URL/$PAGE
		$curlbin -o - $URL/$PAGE | sed 's/{"format":/\n{"format":/g' | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT \
		'
			# BEGIN variable setzen
			BEGIN
			{
				# setzt suchvariable auf 0 vor dem start
				piccount = 0
				pages = "1"
			}
			/"format"/ \
			{
#			print "1111111" $0

				i = index($0, "\"episode_id\":\"") + 14
	            j = index(substr($0, i), "\",\"") - 1
				id = substr($0, i, j)
#			print "id: " id

				i = index($0, "\"title\":\"") + 9
	            j = index(substr($0, i), "\",\"") - 1
				title = substr($0, i, j)
#			print "title: " title

				i = index($0, "\"season_name\":\"") + 15
	            j = index(substr($0, i), "\",\"") - 1
				extra = substr($0, i, j)
#			print "extra: " extra



				i = index($0, "\"season_number\":\"") + 17
	            j = index(substr($0, i), "\",\"") - 1
				season = substr($0, i, j)
#			print "season: " season

				i = index($0, "\"episode_nr\":\"") + 14
	            j = index(substr($0, i), "\",\"") - 1
				episode = substr($0, i, j)
#			print "episode: " episode



				i = index($0, "\"video_url\":\"") + 13
	            j = index(substr($0, i), "\"") - 1
				newpage = substr($0, i, j)
				gsub(/\\/, "", newpage, newpage)

#			print "newpage: " newpage

				i = index($0, "\"original_image\":\"") + 18
	            j = index(substr($0, i), "\"") - 1
				pic = substr($0, i, j)
				gsub(/\\/, "", pic, pic)
#			print "pic: " pic

				piccount += 1
				if ( pic == "" )
				{
	            	pic = "http://atemio.dyndns.tv/mediathek/menu/default.jpg"
				}
#				http://m.myspass.de/api/index.php?command=seasonepisodes&id=285
#				print title " (" extra " Staffeln)#" SRC " " SRC " hoster \x27" newpage "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#111"
#				print "S" season "E" episode " - " title "#" SRC " " SRC " play \x27" newpage "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#0"
				print "S" season "E" episode " - " title "#" newpage "#http:" pic "#" PICNAME "." piccount ".jpg#" NAME "#2"

#print newpage

				next
			}
			END
			{
#				if (curpage != pages)
#					print "Page (" NEXT + 1 "/" pages ")#" SRC " " SRC " " INPUT " \x27" PAGE "\x27 " NEXT + 1 "#http://atemio.dyndns.tv/mediathek/menu/next.jpg#next.jpg#" NAME "#0"
			}
		# schreibe alles in die list datei
		' >$TMP/$FILENAME.list
#	fi
	# gebe titan den list namen mit pfad zurueck
	echo "$TMP/$FILENAME.list"
}

search()
{
#	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - $URL/$PAGE | sed 's/{"uniqueID":/\n{"uniqueID":/g' | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT \
		'
			# BEGIN variable setzen
			BEGIN
			{
				# setzt suchvariable auf 0 vor dem start
				piccount = 0
				pages = "1"
			}
			/"uniqueID"/ \
			{
#			print "1111111" $0
				i = index($0, "\"title\":\"") + 9
	            j = index(substr($0, i), "\",\"") - 1
				title = substr($0, i, j)
#			print "title: " title

				i = index($0, "\"broadcast\":\"") + 13
	            j = index(substr($0, i), "\",\"") - 1
				extra = substr($0, i, j)
#			print "extra: " extra

				i = index($0, "\"url\":\"") + 7
	            j = index(substr($0, i), "\"") - 1
				newpage = substr($0, i, j)
				gsub(/\\/, "", newpage, newpage)

#			print "newpage: " newpage

				i = index($0, "\"teaser\":\"") + 10
	            j = index(substr($0, i), "\"") - 1
				pic = substr($0, i, j)
				gsub(/\\/, "", pic, pic)
#			print "pic: " pic

				piccount += 1
				if ( pic == "" )
				{
	            	pic = "http://atemio.dyndns.tv/mediathek/menu/default.jpg"
				}
				print title " (" extra ")#" SRC " " SRC " hoster \x27" newpage "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#111"
				next
			}
			END
			{
#				if (curpage != pages)
#					print "Page (" NEXT + 1 "/" pages ")#" SRC " " SRC " " INPUT " \x27" PAGE "\x27 " NEXT + 1 "#http://atemio.dyndns.tv/mediathek/menu/next.jpg#next.jpg#" NAME "#0"
			}
		# schreibe alles in die list datei
		' >$TMP/$FILENAME.list
#	fi
	# gebe titan den list namen mit pfad zurueck
	echo "$TMP/$FILENAME.list"
}

hoster()
{
	echo $URL$PAGE
}

play()
{
	echo $URL$PAGE
}

case $INPUT in
	init) $INPUT;;
	mainmenu) $INPUT;;
	search) $INPUT;;
	season) $INPUT;;
	year) $INPUT;;
	episodes) $INPUT;;
	play) $INPUT;;
esac
