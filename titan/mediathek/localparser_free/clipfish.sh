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

URL=http://www.clipfish.de
NAME="Clipfish"

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
	echo "Suchen#$SRC $SRC search 'devmobileapp/searchvideos/%search%/mostrecent/1/16' 1#http://atemio.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >$TMP/$FILENAME.list
	echo "$TMP/$FILENAME.list"
}

search()
{
#	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - $URL/$PAGE | sed 's/{"video_id":/\n{"video_id":/g' | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT \
		'
			# BEGIN variable setzen
			BEGIN
			{
				# setzt suchvariable auf 0 vor dem start
				piccount = 0
				pages = "1"
			}
			/"video_id"/ \
			{
#			print "1111111" $0
				i = index($0, "\"title\":\"") + 9
	            j = index(substr($0, i), "\",\"") - 1
				title = substr($0, i, j)
#			print "title: " title


				i = index($0, "\"media_length\":\"") + 16
	            j = index(substr($0, i), "\",\"") - 1
				duration = substr($0, i, j)
#			print "duration: " duration


				i = index($0, "\"pubDate\":\"") + 11
	            j = index(substr($0, i), "\",\"") - 1
				extra = substr($0, i, j)
#			print "extra: " extra

				i = index($0, "\"media_videourl_hls\":\"") + 22
	            j = index(substr($0, i), "\"") - 1
				newpage = substr($0, i, j)
				gsub(/\\/, "", newpage, newpage)

#			print "newpage: " newpage

				i = index($0, "\"media_content_thumbnail_large\":\"") + 33
	            j = index(substr($0, i), "\"") - 1
				pic = substr($0, i, j)
				gsub(/\\/, "", pic, pic)
#			print "pic: " pic

				piccount += 1
				if ( pic == "" )
				{
	            	pic = "http://atemio.dyndns.tv/mediathek/menu/default.jpg"
				}
#				print title " (" extra ")#" SRC " " SRC " hoster \x27" newpage "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#111"
				print title " (" duration "s) (" extra ")#" newpage "#" pic "#" PICNAME "." piccount ".jpg#" NAME "#2"

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

case $INPUT in
	init) $INPUT;;
	mainmenu) $INPUT;;
	search) $INPUT;;
esac
