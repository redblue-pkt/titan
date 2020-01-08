#!/bin/sh
# first sh box parser for titannit mfg obi

case $2 in
	init) echo skip load hoster.sh;;
	*) . /tmp/localhoster/hoster.sh;;
esac

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
PICNAME=`echo $FILENAME`

if [ -z "$FILENAME" ]; then
	FILENAME=none
fi

URL=https://s.to
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME=SerienStream

case $2 in
	init) echo skip mkdir $TMP;;
	*) if [ ! -e $TMP ];then mkdir $TMP;fi > /dev/null 2>&1;;
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
	echo "$NAME ($TYPE)#$SRC $SRC mainmenu#http://atemio.dyndns.tv/mediathek/menu/$PARSER.jpg#$PARSER.jpg#TiThek#0"
}

mainmenu()
{
	echo "Tv-Shows (A-Z)#$SRC $SRC sorted 'katalog'#http://atemio.dyndns.tv/mediathek/menu/tv-shows.jpg#tv-shows.jpg#$NAME#0" >$TMP/$FILENAME.list
	echo "All Series#$SRC $SRC all 'serien'#http://atemio.dyndns.tv/mediathek/menu/all-sorted.jpg#all-sorted.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "$TMP/$FILENAME.list"
}


sorted()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		watchlist="0-9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z"

		rm $TMP/$FILENAME.list > /dev/null 2>&1

		for ROUND in $watchlist; do
			TITLE=`echo $ROUND`
			filename=`echo $TITLE | tr [A-Z] [a-z]`
			echo "$TITLE#$SRC $SRC list '$PAGE/$ROUND'#http://atemio.dyndns.tv/mediathek/menu/$filename.jpg#$filename.jpg#$NAME#0" >> $TMP/$FILENAME.list
		done
	fi
  	echo "$TMP/$FILENAME.list"
}

all()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - $URL/$PAGE | sed 's/<li><a/\n<li><a/g' | grep "/serie/" | grep 'data-alternative-title=""'| sort -u | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT \
		'
			# BEGIN variable setzen
			BEGIN \
				{
#					print "start"
				}
				# next page init
				# <li><a data-alternative-title="" href="/serie/stream/zwei-himmlische-toechter" title="Zwei himmlische Töchter Stream anschauen">Zwei himmlische Töchter</a></li>
				/href/ \
				{
#					print "$0" $0

					# extrahiere den newpage pfad
					i = index($0, "href=\"") + 6
		            j = index(substr($0, i), "\"") - 1
					# newpage = /view_video.php?viewkey=127170590
		            newpage = substr($0, i, j)

					# extrahiere den newpage pfad
					i = index($0, "\">") + 2
		            j = index(substr($0, i), "<") - 1
					# newpage = /view_video.php?viewkey=127170590
		            title = substr($0, i, j)

					if (title != "")
					{
						if ( pic == "" )
						{
			      			pic = "http://atemio.dyndns.tv/mediathek/menu/default.jpg"
						}

						piccount += 1
						# 25. in naechste zeile springen
						# 26. \x27 = single quotes
						print title "#" SRC " " SRC " season \x27" newpage "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#0"
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

#<ul>
#        <li><span><strong>Staffeln:</strong></span></li> 
#                    <li>  <a class="active" href="/serie/stream/lucifer/staffel-1" title="Staffel 1">1</a>  </li> 
#                    <li>  <a href="/serie/stream/lucifer/staffel-2" title="Staffel 2">2</a>  </li> 
#                    <li>  <a href="/serie/stream/lucifer/staffel-3" title="Staffel 3">3</a>  </li>  </ul>

season()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - $URL/$PAGE | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT \
		'
			# BEGIN variable setzen
			BEGIN \
				{
					suche = 0
				}
				# next page init
				/<strong>Staffeln:<\/strong>/ \
				{
					suche = 1
#					next
				}
				/href=/ \
				{
					if ( suche == 1 )
					{
						# extrahiere den newpage pfad
						i = index($0, "href=\"") + 6
				        j = index(substr($0, i), "\"") - 1
				        newpage = substr($0, i, j)

						# extrahiere den title pfad
						i = index($0, "title=\"") + 7
				        j = index(substr($0, i), "\"") - 1
				        title = substr($0, i, j)

						if (title != "")
						{
							if ( pic == "" )
							{
					  			pic = "http://atemio.dyndns.tv/mediathek/menu/default.jpg"
							}

							piccount += 1
							# 25. in naechste zeile springen
							# 26. \x27 = single quotes

							split(title, a, " ")
							season = a[2]
							print title "#" SRC " " SRC " episode \x27" newpage "\x27#http://atemio.dyndns.tv/mediathek/menu/s" season ".jpg#s" season ".jpg#" NAME "#0"
#							print title "#" SRC " " SRC " episode \x27" newpage "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#0"
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

episode()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - $URL/$PAGE | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT \
		'
			# BEGIN variable setzen
			BEGIN \
				{
					suche = 0
				}
				# next page init
				/<strong>Episoden:<\/strong>/ \
				{
					suche = 1
#					next
				}
				#<li>  <a class="active" href="/serie/stream/lucifer/staffel-1" title="Staffel 1">1</a>  </li>
				/href=/ \
				{
					if ( suche == 1 )
					{
						# extrahiere den newpage pfad
						i = index($0, "href=\"") + 6
				        j = index(substr($0, i), "\"") - 1
				        newpage = substr($0, i, j)

						# extrahiere den title pfad
						i = index($0, "title=\"") + 7
				        j = index(substr($0, i), "\"") - 1
				        title = substr($0, i, j)

						if (title != "")
						{
							if ( pic == "" )
							{
					  			pic = "http://atemio.dyndns.tv/mediathek/menu/default.jpg"
							}

							piccount += 1
							# 25. in naechste zeile springen
							# 26. \x27 = single quotes

							#Staffel 3 Episode 26
							split(title, a, " ")
							season = a[2]
							episode = a[4]
							print title "#" SRC " " SRC " hosterlist \x27" newpage "\x27#http://atemio.dyndns.tv/mediathek/menu/s" season "e" episode ".jpg#s" season "e" episode ".jpg#" NAME "#0"

#							print title "#" SRC " " SRC " hosterlist \x27" newpage "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#0"
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

list()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - $URL/$PAGE | tr -d '\n' | sed 's/<div/\n<div/g' | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v URL=$URL -v PAGE=$PAGE -v NEXT=$NEXT \
		'
			# BEGIN variable setzen
			BEGIN \
				{
					suche = 0
				}
				# next page init
				/<div class="seriesListContainer row">/ \
				{
					suche = 1
				}
				/<div class="cf"><\/div>/ \
				{
					if ( suche == 1 )
					{
						suche = 0
					}
					next
				}
				/<div class="col-md-15 col-sm-3 col-xs-6">/ \
				{
					if ( suche == 1 )
					{
						# extrahiere den newpage pfad
						i = index($0, "href=\"") + 6
				        	j = index(substr($0, i), "\"") - 1
				        	newpage = substr($0, i, j)

						# extrahiere den title pfad
#						i = index($0, "title=\"") + 7
##				        	j = index(substr($0, i), "\"") - 1
#				        	j = index(substr($0, i), "stream online") - 2
#				       		title = substr($0, i, j)


						# extrahiere den title pfad
						i = index($0, "<h3>") + 4
				        	j = index(substr($0, i), "<") - 1
				        	title = substr($0, i, j)


#						i = index($0, "img src=\"") + 9
						i = index($0, "src=\"") + 5
				        	j = index(substr($0, i), "\"") - 1
				        	pic = substr($0, i, j)

						if (title != "")
						{
							if ( pic == "" )
							{
				  				pic = "http://atemio.dyndns.tv/mediathek/menu/default.jpg"
							}

							piccount += 1
							# 25. in naechste zeile springen
							# 26. \x27 = single quotes
							print title "#" SRC " " SRC " season \x27" newpage "\x27#" URL pic "#" PICNAME "." piccount ".jpg#" NAME "#0"
						}
#						next
					}
				}
		# 29. schreibe alles in die list datei
		' >$TMP/$FILENAME.list
	fi
	# 30. gebe titan den list namen mit pfad zurueck
	echo "$TMP/$FILENAME.list"
#	cat "$TMP/$FILENAME.list"
}

hosterlist()
{
#rm $TMP/$FILENAME.list
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - $URL/$PAGE | tr -d '\n' | tr -d '\r' | sed 's/<li class="col-md-4 col-xs-12 col-sm-6/\n<li class="col-md-4 col-xs-12 col-sm-6/g' |  grep ^'<li class="col-md-4 col-xs-12 col-sm-6' | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v URL=$URL -v PAGE=$PAGE -v NEXT=$NEXT \
		'
			# BEGIN variable setzen
			BEGIN \
				{
					suche = 1
				}
				# next page init
#				/<div><a href="\/redirect/ \
				/<li class="col-md-4 col-xs-12 col-sm-6 episodeLink/ \
				{
					if ( suche == 1 )
					{
						# extrahiere den newpage pfad
						i = index($0, "href=\"") + 6
				        j = index(substr($0, i), "\"") - 1
				        newpage = substr($0, i, j)

						i = index($0, "data-link-target=\"") + 18
				        j = index(substr($0, i), "\"") - 1
				        newpage = substr($0, i, j)

						# extrahiere den title pfad
						i = index($0, "<h4>") + 4
				        j = index(substr($0, i), "<") - 1
				        title = substr($0, i, j)

				        pic = tolower(title)

						if (title != "")
						{
							if ( pic == "" )
							{
					  			pic = "http://atemio.dyndns.tv/mediathek/menu/default.jpg"
							}

							piccount += 1
							# 25. in naechste zeile springen
							# 26. \x27 = single quotes
							print title "#" SRC " " SRC " hoster \x27" newpage "\x27#http://atemio.dyndns.tv/mediathek/menu/" pic ".jpg#" pic ".jpg#" NAME "#111"
						}
#						next
					}
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
	rm -f $TMP/cache.hoster.$hoster.* > /dev/null 2>&1

	$curlbin -H "X-Requested-With: XMLHttpRequest" -X POST --data "password=abc123456&email=Ither1981%40dayrep.com" "$URL/login"
#	$curlbin2 -v https://s.to/redirect/1131416
	$curlbin2 -v "$URL/$PAGE" --dump-header $TMP/cache.hoster.$hoster.1
	cat $TMP/cache.hoster.$hoster.1 | grep ^Location: | cut -d" " -f2 >$TMP/cache.hoster.$hoster.1.url1
	URL=`cat $TMP/cache.hoster.$hoster.1.url1`

	echo $URL
}

case $INPUT in
	init) $INPUT;;
	mainmenu) $INPUT;;
	all) $INPUT;;
	sorted) $INPUT;;
	season) $INPUT;;
	episode) $INPUT;;
	list) $INPUT;;
	hosterlist) $INPUT;;
	hoster) $INPUT;;
esac
