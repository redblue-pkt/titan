#!/bin/sh
# first sh box parser for titannit mfg obi

case $2 in
	init) echo skip load hoster.sh;;
	*) . /tmp/localhoster/hoster.sh;;
esac

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
	echo "Ganze Folgen#$SRC $SRC season 'api/index.php?command=formats' 1#http://atemio.dyndns.tv/mediathek/menu/shows.jpg#shows.jpg#$NAME#0" >$TMP/$FILENAME.list
	echo "Suchen#$SRC $SRC search 'frontend/json/search-videos.php?QUERY_STRING=%search%' 1#http://atemio.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >>$TMP/$FILENAME.list
	echo "$TMP/$FILENAME.list"
}

season()
{
rm "$TMP/$FILENAME.list"

	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - $URL/$PAGE | sed 's/{"format_id":/\n{"format_id":/g' | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT \
		'
			# BEGIN variable setzen
			BEGIN \
			{
				# setzt suchvariable auf 0 vor dem start
				piccount = 0
				pages = "1"
			}
			/"format_id"/ \
			{
				i = index($0, "\"format_id\":\"") + 13
	            j = index(substr($0, i), "\",\"") - 1
				id = substr($0, i, j)

				i = index($0, "\"format\":\"") + 10
	            j = index(substr($0, i), "\",\"") - 1
				title = substr($0, i, j)

				i = index($0, "\"number_of_seasons\":\"") + 21
	            j = index(substr($0, i), "\",\"") - 1
				extra = substr($0, i, j)

				i = index($0, "\"node_id\":\"") + 11
	            j = index(substr($0, i), "\"") - 1
				newpage = substr($0, i, j)
				gsub(/\\/, "", newpage)

				i = index($0, "\"original_image\":\"") + 18
	            j = index(substr($0, i), "\"") - 1
				pic = substr($0, i, j)
				gsub(/\\/, "", pic)

				piccount += 1
				if ( pic == "" )
				{
	            	pic = "http://atemio.dyndns.tv/mediathek/menu/default.jpg"
				}
				else
					pic = "http:" pic

				print title " (" extra " Staffeln)#" SRC " " SRC " year \x27api/index.php?command=seasonslist&id=" id "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#0"

				next
			}
#			END
#			{
#				if (curpage != pages)
#					print "Page (" NEXT + 1 "/" pages ")#" SRC " " SRC " " INPUT " \x27" PAGE "\x27 " NEXT + 1 "#http://atemio.dyndns.tv/mediathek/menu/next.jpg#next.jpg#" NAME "#0"
#			}
		# schreibe alles in die list datei
		' >$TMP/$FILENAME.list
	fi
	# gebe titan den list namen mit pfad zurueck
	echo "$TMP/$FILENAME.list"
}

year()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - $URL/$PAGE | sed 's/{"season_id":/\n{"season_id":/g' | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT \
		'
			# BEGIN variable setzen
			BEGIN \
			{
				# setzt suchvariable auf 0 vor dem start
				piccount = 0
				pages = "1"
			}
			/"season_id"/ \
			{
				i = index($0, "\"season_id\":\"") + 13
	            j = index(substr($0, i), "\",\"") - 1
				id = substr($0, i, j)

				i = index($0, "\"season_number\":\"") + 17
	            j = index(substr($0, i), "\",\"") - 1
				title = substr($0, i, j)

				i = index($0, "\"season_name\":\"") + 15
	            j = index(substr($0, i), "\",\"") - 1
				extra = substr($0, i, j)

				i = index($0, "\"node_id\":\"") + 11
	            j = index(substr($0, i), "\"") - 1
				newpage = substr($0, i, j)
				gsub(/\\/, "", newpage)

				i = index($0, "\"original_image\":\"") + 18
	            j = index(substr($0, i), "\"") - 1
				pic = substr($0, i, j)
				gsub(/\\/, "", pic)

				piccount += 1
				if ( pic == "" )
				{
	            	pic = "http://atemio.dyndns.tv/mediathek/menu/default.jpg"
				}
				else
					pic = "http:" pic

				print "Staffel " title " (" extra ")#" SRC " " SRC " episodes \x27api/index.php?command=seasonepisodes&id=" id "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#0"

				next
			}
#			END
#			{
#				if (curpage != pages)
#					print "Page (" NEXT + 1 "/" pages ")#" SRC " " SRC " " INPUT " \x27" PAGE "\x27 " NEXT + 1 "#http://atemio.dyndns.tv/mediathek/menu/next.jpg#next.jpg#" NAME "#0"
#			}
		# schreibe alles in die list datei
		' >$TMP/$FILENAME.list
	fi
	# gebe titan den list namen mit pfad zurueck
	echo "$TMP/$FILENAME.list"
}

episodes()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - $URL/$PAGE | sed 's/{"format":/\n{"format":/g' | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT \
		'
			# BEGIN variable setzen
			BEGIN \
			{
				# setzt suchvariable auf 0 vor dem start
				piccount = 0
				pages = "1"
			}
			/"format"/ \
			{
				i = index($0, "\"episode_id\":\"") + 14
	            j = index(substr($0, i), "\",\"") - 1
				id = substr($0, i, j)

				i = index($0, "\"title\":\"") + 9
	            j = index(substr($0, i), "\",\"") - 1
				title = substr($0, i, j)
				gsub(" - Teil 1", "", title)

				i = index($0, "\"season_name\":\"") + 15
	            j = index(substr($0, i), "\",\"") - 1
				extra = substr($0, i, j)

				i = index($0, "\"season_number\":\"") + 17
	            j = index(substr($0, i), "\",\"") - 1
				season = substr($0, i, j)

				i = index($0, "\"episode_nr\":\"") + 14
	            j = index(substr($0, i), "\",\"") - 1
				episode = substr($0, i, j)

				i = index($0, "\"myspass_url\":\"") + 15
	            j = index(substr($0, i), "\"") - 1
				newpage = substr($0, i, j)
				gsub(/\\/, "", newpage)

				i = index($0, "\"original_image\":\"") + 18
	            j = index(substr($0, i), "\"") - 1
				pic = substr($0, i, j)
				gsub(/\\/, "", pic)

				piccount += 1
				if ( pic == "" )
				{
	            	pic = "http://atemio.dyndns.tv/mediathek/menu/default.jpg"
				}
				print "S" season "E" episode " - " title "#" SRC " " SRC " parts \x27" newpage "\x27#http:" pic "#" PICNAME "." piccount ".jpg#" NAME "#0"

				next
			}
#			END
#			{
#				if (curpage != pages)
#					print "Page (" NEXT + 1 "/" pages ")#" SRC " " SRC " " INPUT " \x27" PAGE "\x27 " NEXT + 1 "#http://atemio.dyndns.tv/mediathek/menu/next.jpg#next.jpg#" NAME "#0"
#			}
		# schreibe alles in die list datei
		' >$TMP/$FILENAME.list
	fi
	# gebe titan den list namen mit pfad zurueck
	echo "$TMP/$FILENAME.list"
}

search()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - $URL/$PAGE | sed 's/{"uniqueID":/\n{"uniqueID":/g' | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT \
		'
			# BEGIN variable setzen
			BEGIN \
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
				gsub(/\\/, "", newpage)

#			print "newpage: " newpage

				i = index($0, "\"teaser\":\"") + 10
	            j = index(substr($0, i), "\"") - 1
				pic = substr($0, i, j)
				gsub(/\\/, "", pic)
#			print "pic: " pic

				piccount += 1
				if ( pic == "" )
				{
	            	pic = "http://atemio.dyndns.tv/mediathek/menu/default.jpg"
				}
				print title " (" extra ")#" SRC " " SRC " hoster \x27" newpage "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#111"
				next
			}
#			END
#			{
#				if (curpage != pages)
#					print "Page (" NEXT + 1 "/" pages ")#" SRC " " SRC " " INPUT " \x27" PAGE "\x27 " NEXT + 1 "#http://atemio.dyndns.tv/mediathek/menu/next.jpg#next.jpg#" NAME "#0"
#			}
		# schreibe alles in die list datei
		' >$TMP/$FILENAME.list
	fi
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

parts()
{
	rm $TMP/$FILENAME.list
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		FOUND=`echo $PAGE | sed 's/Teil/\n/g' | grep ^http | sed 's!http://www.myspass.de/myspass!!g' | sed 's!http://www.myspass.de!!g'`
#		IDLIST=`$curlbin "$PAGE" | grep "$FOUND" | sed 's/<a href=/\nfound=/g' | grep ^found= | grep title= | grep -v myspassTeaserTextDesc | cut -d '"' -f2 | sort -u | tr '/' ' ' | awk '{ print $NF }'`
		IDLIST=`$curlbin "$PAGE" | grep "$FOUND"| sed 's/<a href=/\nfound=/g' | grep ^found= | tr -d ' ' | cut -d "/" -f6 | sort -u`

		for ROUND in $IDLIST; do
			$curlbin http://www.myspass.de/includes/apps/video/getvideometadataxml.php?id=$ROUND > "$TMP/cache.$FILENAME.$ROUND.1"
			
			#	<title><![CDATA[23. September 2006 - Teil 1]]></title>
			#	<url_flv><![CDATA[http://x3583brainc11021.s.o.l.lb.core-cdn.net/secdl/9b7b49482bf7ba65145bc28c6c3ac7b5/58ea7331/11021brainpool/ondemand/3583brainpool/163840/myspass2009/82/578/9231/12073/12073_61.mp4]]></url_flv>
			#	<imagePreview><![CDATA[//www.myspass.de/myspass/media/images/videos/73/12073_640x360.jpg]]></imagePreview>

			TITLE="`cat $TMP/cache.$FILENAME.$ROUND.1 | sed 's/<title><!\[CDATA\[/\nfound=\]/g' | grep ^found= | cut -d"]" -f2`"
			NEWPAGE="`cat $TMP/cache.$FILENAME.$ROUND.1 | sed 's/<url_flv><!\[CDATA\[/\nfound=\]/g' | grep ^found= | cut -d"]" -f2`"
			PIC="http:`cat $TMP/cache.$FILENAME.$ROUND.1 | sed 's/<imagePreview><!\[CDATA\[/\nfound=\]/g' | grep ^found= | cut -d"]" -f2`"

			if [ -z "$PIC" ] || "$PIC" = "http:" ] ; then
				PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
			fi

			echo "$TITLE#$NEWPAGE#$PIC#$PICNAME$piccount.jpg#$NAME#2" >> $TMP/$FILENAME.list

		done
	fi

	echo "$TMP/$FILENAME.list"
}

case $INPUT in
	init) $INPUT;;
	mainmenu) $INPUT;;
	search) $INPUT;;
	season) $INPUT;;
	year) $INPUT;;
	episodes) $INPUT;;
	play) $INPUT;;
	parts) $INPUT;;
esac
