#!/bin/sh
# first sh box parser for titannit mfg obi

SRC=$1
INPUT=$2
PAGE=$3
NEXT=$4
PAGE2=$5
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`

#URL=https://s.to
URL=http://190.115.18.20
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME=SerienStream

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
	echo "Neue Episoden#$SRC $SRC latest 'neue-episoden'#http://openaaf.dyndns.tv/mediathek/menu/all-sorted.jpg#all-sorted.jpg#$NAME#0" >$TMP/$FILENAME.list
	echo "Neue Serien#$SRC $SRC list 'neu'#http://openaaf.dyndns.tv/mediathek/menu/all-sorted.jpg#all-sorted.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Populär#$SRC $SRC list 'beliebte-serien'#http://openaaf.dyndns.tv/mediathek/menu/all-sorted.jpg#all-sorted.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Alle#$SRC $SRC all 'serien'#http://openaaf.dyndns.tv/mediathek/menu/all-sorted.jpg#all-sorted.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "A-Z#$SRC $SRC sorted 'katalog'#http://openaaf.dyndns.tv/mediathek/menu/tv-shows.jpg#tv-shows.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Suche#$SRC $SRC search 'serien' 1 '%search%'#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#all-sorted.jpg#$NAME#112" >>$TMP/$FILENAME.list
	echo "$TMP/$FILENAME.list"
}
#https://s.to/search?q={search_string}

sorted()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		watchlist="0-9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z"

		rm $TMP/$FILENAME.list > /dev/null 2>&1

		for ROUND in $watchlist; do
			TITLE=`echo $ROUND`
			filename=`echo $TITLE | tr [A-Z] [a-z]`
			echo "$TITLE#$SRC $SRC list '$PAGE/$ROUND' 1#http://openaaf.dyndns.tv/mediathek/menu/$filename.jpg#$filename.jpg#$NAME#0" >> $TMP/$FILENAME.list
		done
	fi
  	echo "$TMP/$FILENAME.list"
}

all()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - $URL/$PAGE | sed 's/<li><a/\n<li><a/g' | grep "/serie/" | grep 'data-alternative-title=""'| sort -u | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v URL=$URL -v PAGE=$PAGE -v NEXT=$NEXT \
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
			      			pic = "http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
						}

                        desc = "curl --connect-timeout 5 " URL newpage " | tr -d \"\\n\" | sed -nr \"s/.*data-full-description=\\\"([^\\\"]+)\\\".*/\\1/p\""

#<div class="backdrop" style="background-image: url(https://zrt5351b7er9.static-webarchive.org/img/cover/reacher-stream-cover-dGoiANWASrQtOOxy58TI9aKYfqDqq4sk_800x300.png)"></div>            <div class="container row">
#<div class="seriesCoverBox"><img src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAAAAAA6fptVAAAACklEQVR4nGP6AgAA+gD3odZZSQAAAABJRU5ErkJggg==" data-src="https://zrt5351b7er9.static-webarchive.org/img/cover/reacher-stream-cover-GTyjOBAKAL3Ifah66vuKkBDC1Z2esgrm_220x330.jpg" alt="Reacher, Cover, HD, Serien Stream, ganze Folge" itemprop="image" title="Cover Reacher SerienStream"><noscript><img src="https://zrt5351b7er9.static-webarchive.org/img/cover/reacher-stream-cover-GTyjOBAKAL3Ifah66vuKkBDC1Z2esgrm_220x330.jpg" alt="Reacher, Cover, HD, Serien Stream, ganze Folge" itemprop="image" title="Cover Reacher SerienStream"></noscript></div>
                        pic = "curl --connect-timeout 5 " URL newpage " | tr -d \"\\n\" | sed -nr \"s/.*<noscript><img src=\\\"([^\\\"]+)\\\".*/\\1/p\""

						piccount += 1
						# 25. in naechste zeile springen
						# 26. \x27 = single quotes
						print title "#" SRC " " SRC " season \x27" newpage "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#0#" desc
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
					  			pic = "http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
							}

							piccount += 1
							# 25. in naechste zeile springen
							# 26. \x27 = single quotes

							split(title, a, " ")
							season = a[2]
							print title "#" SRC " " SRC " episode \x27" newpage "\x27#http://openaaf.dyndns.tv/mediathek/menu/s" season ".jpg#s" season ".jpg#" NAME "#0"
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

episode_old()
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
					  			pic = "http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
							}

							piccount += 1
							# 25. in naechste zeile springen
							# 26. \x27 = single quotes

							#Staffel 3 Episode 26
							split(title, a, " ")
							season = a[2]
							episode = a[4]
							print title "#" SRC " " SRC " hosterlist \x27" newpage "\x27#http://openaaf.dyndns.tv/mediathek/menu/s" season "e" episode ".jpg#s" season "e" episode ".jpg#" NAME "#0"

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

episode()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - $URL/$PAGE | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT \
		'
			# BEGIN variable setzen
			BEGIN \
				{
					suche = 1
				}
                /<td class="seasonEpisodeTitle"><a href=/ \
				{
					if ( suche == 1 )
					{
						# extrahiere den newpage pfad
						i = index($0, "href=\"") + 6
				        j = index(substr($0, i), "\"") - 1
				        newpage = substr($0, i, j)

						# extrahiere den title pfad
						i = index($0, "<strong>") + 8
				        j = index(substr($0, i), "<") - 1
				        title = substr($0, i, j)

						if (newpage != "")
						{
							if ( pic == "" )
							{
					  			pic = "http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
							}

							piccount += 1
							# 25. in naechste zeile springen
							# 26. \x27 = single quotes

						    # extrahiere den title pfad
						    i = index(newpage, "/staffel-") + 9
				            j = index(substr(newpage, i), "/") - 1
				            season = substr(newpage, i, j)

						    # extrahiere den title pfad
						    i = index(newpage, "/episode-") + 9
    #				        j = index(substr(newpage, i), "\n") - 1
                            j = length(newpage)
				            episode = substr(newpage, i, j)

#							#Staffel 3 Episode 26
#							split(extra, a, " ")
#							season = a[2]
#							episode = a[4]
#							print title "(" extra ")#" SRC " " SRC " hosterlist \x27" newpage "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#0"

    						if (title == "")
    							print "S" season "E" episode "#" SRC " " SRC " hosterlist \x27" newpage "\x27#http://openaaf.dyndns.tv/mediathek/menu/s" season "e" episode ".jpg#s" season "e" episode ".jpg#" NAME "#0"
                            else
       							print "S" season "E" episode " - " title "#" SRC " " SRC " hosterlist \x27" newpage "\x27#http://openaaf.dyndns.tv/mediathek/menu/s" season "e" episode ".jpg#s" season "e" episode ".jpg#" NAME "#0"
                        }
						next
					}
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
		if [ "$NEXT" == "1" ]; then PAGE2=$($curlbin $URL/$PAGE/$NEXT | grep '<ul><li class="active">' | sed 's/<a href=/\npages:<a href=/g' |  grep -v "&gt" | sed -nr 's/.*>([^>]+)<.*/\1/p' | tail -n1);fi
		$curlbin -o - $URL/$PAGE/$NEXT | tr -d '\n' | sed 's/<div/\n<div/g' | awk -v SRC=$SRC -v pages=$pages -v NAME=$NAME -v NEXT=$NEXT -v PICNAME=$PICNAME -v INPUT=$INPUT -v URL=$URL -v PAGE=$PAGE -v NEXT=$NEXT -v PAGE2=$PAGE2 \
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
##				        j = index(substr($0, i), "\"") - 1
#				        j = index(substr($0, i), "stream online") - 2
#				       	title = substr($0, i, j)


						# extrahiere den title pfad
						i = index($0, "<h3>") + 4
				        j = index(substr($0, i), "<") - 1
				        title = substr($0, i, j)


#						i = index($0, "img src=\"") + 9
#						i = index($0, "src=\"") + 5
						i = index($0, "data-src=\"") + 10

				        j = index(substr($0, i), "\"") - 1
				        pic = substr($0, i, j)
#print "a: " $a
#print "pic: " pic

						if (title != "")
						{
							if ( pic == "" )
							{
				  				pic = "http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
							}

                            if (pic ~ /.png/)
                                picext = ".png"
                            else
                                picext = ".jpg"

                            desc = "curl --connect-timeout 5 " URL newpage " | tr -d \"\\n\" | sed -nr \"s/.*data-full-description=\\\"([^\\\"]+)\\\".*/\\1/p\""

							piccount += 1
							# 25. in naechste zeile springen
							# 26. \x27 = single quotes

    						print title "#" SRC " " SRC " season \x27" newpage "\x27#" pic "#" PICNAME "." piccount picext "#" NAME "#0#" desc
						}
#						next
					}
				}
			END \
				{
					if (NEXT != PAGE2)
						print "Page (" NEXT + 1 "/" PAGE2 ")#" SRC " " SRC " " INPUT " \x27" PAGE "\x27 " NEXT + 1 " " PAGE2 "#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#" NAME "#0"
				}
		# 29. schreibe alles in die list datei
		' >$TMP/$FILENAME.list
	fi
	# 30. gebe titan den list namen mit pfad zurueck
	echo "$TMP/$FILENAME.list"
#	cat "$TMP/$FILENAME.list"
}

latest()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - $URL/$PAGE/$NEXT | tr -d '\n' | sed 's/<div/\n<div/g' | awk -v SRC=$SRC -v pages=$pages -v NAME=$NAME -v NEXT=$NEXT -v PICNAME=$PICNAME -v INPUT=$INPUT -v URL=$URL -v PAGE=$PAGE -v NEXT=$NEXT -v PAGE2=$PAGE2 \
		'
			# BEGIN variable setzen
			BEGIN \
				{
					suche = 0
				}
				# next page init
				/<div class="newEpisodeList">/ \
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
				/<a href=/ \
				{
					if ( suche == 1 )
					{
						# extrahiere den newpage pfad
						i = index($0, "href=\"") + 6
			        	j = index(substr($0, i), "\"") - 1
			        	newpage = substr($0, i, j)
#print "newpage: " newpage
						# extrahiere den newpage pfad
						i = index($0, "<strong>") + 8
			        	j = index(substr($0, i), "<") - 1
			        	name = substr($0, i, j)
#print "name: " name
						# extrahiere den newpage pfad
						i = index($0, "blue2\">") + 7
			        	j = index(substr($0, i), "<") - 1
			        	title = substr($0, i, j)
#print "title: " title
						# extrahiere den newpage pfad
						i = index($0, "elementFloatRight\">") + 19
			        	j = index(substr($0, i), "<") - 1
			        	date = substr($0, i, j)
#print "date: " date
						# extrahiere den newpage pfad
#						i = index($0, "src=\"") + 5
						i = index($0, "data-src=\"") + 10

			        	j = index(substr($0, i), "\"") - 1
			        	pic = substr($0, i, j)
#print "pic: " pic
						# extrahiere den newpage pfad
						i = index($0, "title=\"") + 7
			        	j = index(substr($0, i), "\"") - 1
			        	episodename = substr($0, i, j)
#print "episodename: " episodename

						# extrahiere den newpage pfad
						i = index($0, "/svg/") + 5
			        	j = index(substr($0, i), ".") - 1
			        	lang = substr($0, i, j)
#print "lang: " lang
						if (title != "")
						{
							if ( pic == "" || pic ~ /.svg/)
							{
				  				pic = "http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
							}

                            desc = "curl --connect-timeout 5 " URL newpage " | tr -d \"\\n\" | sed -nr \"s/.*data-full-description=\\\"([^\\\"]+)\\\".*/\\1/p\""

							piccount += 1
							# 25. in naechste zeile springen
							# 26. \x27 = single quotes
#							print name "#" SRC " " SRC " season \x27" newpage "\x27#" URL pic "#" PICNAME "." piccount ".jpg#" NAME "#0"
#							print name "#" SRC " " SRC " season \x27" newpage "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#0#" desc

							#Staffel 3 Episode 26
							split(title, a, " ")
							season = a[1]
							episode = a[2]

                            gsub("S0", "S", season)
                            gsub("E0", "E", episode)

                            gsub("S", "", season)
                            gsub("E", "", episode)

#print "season: " season
#print "episode: " episode

#							print name " - " title " - " episodename " - " lang "#" SRC " " SRC " hosterlist \x27" newpage "\x27#http://openaaf.dyndns.tv/mediathek/menu/s" season "e" episode ".jpg#s" season "e" episode ".jpg#" NAME "#0"
#							print name " - " title " - " episodename "#" SRC " " SRC " hosterlist \x27" newpage "\x27#http://openaaf.dyndns.tv/mediathek/menu/s" season "e" episode ".jpg#s" season "e" episode ".jpg#" NAME "#0#" desc
#							print title " - " episodename " - " lang " - " date "#" SRC " " SRC " hosterlist \x27" newpage "\x27#http://openaaf.dyndns.tv/mediathek/menu/s" season "e" episode ".jpg#s" season "e" episode ".jpg#" NAME "#0#" desc
#							print name " (" title ") (" episodename ") (" date ")#" SRC " " SRC " hosterlist \x27" newpage "\x27#http://openaaf.dyndns.tv/mediathek/menu/s" season "e" episode ".jpg#s" season "e" episode ".jpg#" NAME "#0#" desc
							print name " (" title ") (" episodename ") (" date ")#" SRC " " SRC " hosterlist \x27" newpage "\x27#http://openaaf.dyndns.tv/mediathek/menu/s" season "e" episode ".jpg#s" season "e" episode ".jpg#" NAME "#0#" desc
						}
                        next
					}
                }             
			END \
				{
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
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - $URL/$PAGE | tr -d '\n' | tr -d '\r' | sed 's/<li class="col-md-3 col-xs-12 col-sm-6/\n<li class="col-md-3 col-xs-12 col-sm-6/g' |  grep ^'<li class="col-md-3 col-xs-12 col-sm-6' | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v URL=$URL -v PAGE=$PAGE -v NEXT=$NEXT \
		'
			# BEGIN variable setzen
			BEGIN \
				{
					suche = 1
				}
				# next page init
#				/<div><a href="\/redirect/ \
				/<li class="col-md-3 col-xs-12 col-sm-6 episodeLink/ \
				{
#print "a " $a
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

						# extrahiere den title pfad
						i = index($0, "data-lang-key=\"") + 15
				        j = index(substr($0, i), "\"") - 1
				        langck = substr($0, i, j)

                        if ($0 ~ /data-lang-key="1"/)
                            lang = "de"
                        else if ($0 ~ /data-lang-key="2"/)
                            lang = "en"
                        else if ($0 ~ /data-lang-key="3"/)
                            lang = "de sub"
                        else
                            lang = "??"

				        pic = tolower(title)

						if (title != "")
						{
							if ( pic == "" )
							{
					  			pic = "http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
							}

							piccount += 1
							# 25. in naechste zeile springen
							# 26. \x27 = single quotes
							print title " (" lang ")#" SRC " " SRC " hoster \x27" newpage "\x27#http://openaaf.dyndns.tv/mediathek/menu/" pic ".jpg#" pic ".jpg#" NAME "#111"
						}
						next
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
	URL=`cat $TMP/cache.hoster.$hoster.1.url1 | tr -d '\n' | tr -d '\r'`

	echo $URL
}

search()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - $URL/$PAGE | sed 's/<li><a/\n<li><a/g' | grep "/serie/" | grep 'data-alternative-title=""' | grep $PAGE2 | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT \
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
			      			pic = "http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
						}

                        desc = "curl --connect-timeout 5 " URL newpage " | tr -d \"\\n\" | sed -nr \"s/.*data-full-description=\\\"([^\\\"]+)\\\".*/\\1/p\""

						piccount += 1
						# 25. in naechste zeile springen
						# 26. \x27 = single quotes
						print title "#" SRC " " SRC " season \x27" newpage "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#0#" desc
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

case $INPUT in
	init) $INPUT;;
	mainmenu) $INPUT;;
	all) $INPUT;;
	sorted) $INPUT;;
	season) $INPUT;;
	episode) $INPUT;;
	latest) $INPUT;;
	list) $INPUT;;
	hosterlist) $INPUT;;
	hoster) $INPUT;;
	search) $INPUT;;
esac
