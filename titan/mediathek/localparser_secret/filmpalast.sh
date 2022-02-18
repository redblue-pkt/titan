#!/bin/sh
# first sh box parser for titannit mfg obi

SRC=$1
INPUT=$2
PAGE=$3
NEXT=$4
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`

URL=https://filmpalast.to
NAME="FilmPalast"

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
	echo "New#$SRC $SRC search '/page/' 0#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#new.jpg#$NAME#0" >$TMP/$FILENAME.list
	echo "Filme#$SRC $SRC search '/movies/new/page/' 0#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#movies.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Serien#$SRC $SRC search '/serien/view/page/' 0#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#serien.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Top#$SRC $SRC search '/movies/top/page/' 1#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#top.jpg#$NAME#0" >>$TMP/$FILENAME.list
#	echo "Englisch#$SRC $SRC search '/search/genre/Englisch/' 1#http://openaaf.dyndns.tv/mediathek/menu/englisch.jpg#search.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Genre#$SRC $SRC genre#http://openaaf.dyndns.tv/mediathek/menu/genre.jpg#genre.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "A-Z#$SRC $SRC sorted#http://openaaf.dyndns.tv/mediathek/menu/a-z.jpg#a-z.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Suchen#$SRC $SRC search '/search/title/%search%/' 1#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >>$TMP/$FILENAME.list

	echo "$TMP/$FILENAME.list"
}

sorted()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
#		watchlist="0-9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z"
		watchlist=$($curlbin $URL$PAGE$NEXT | sed -nr 's!.*/alpha/([^/]+)".*!\1!p' | sort -u)

		rm $TMP/$FILENAME.list > /dev/null 2>&1

		for ROUND0 in $watchlist; do
			TITLE=`echo $ROUND0`
			filename=`echo $TITLE | tr [A-Z] [a-z]`
			echo "$TITLE#$SRC $SRC search '/search/alpha/$ROUND0/' 1#http://openaaf.dyndns.tv/mediathek/menu/$filename.jpg#$filename.jpg#$NAME#0" >> $TMP/$FILENAME.list
		done
	fi
  	echo "$TMP/$FILENAME.list"
}

genre()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		watchlist=$($curlbin $URL$PAGE$NEXT | sed -nr 's!.*/genre/([^/]+)".*!\1!p' | sort -u)

		rm $TMP/$FILENAME.list > /dev/null 2>&1

		for ROUND0 in $watchlist; do
			TITLE=`echo $ROUND0`
			filename=`echo $TITLE | tr [A-Z] [a-z]`
			echo "$TITLE#$SRC $SRC search '/search/genre/$ROUND0/' 1#http://openaaf.dyndns.tv/mediathek/menu/$filename.jpg#$filename.jpg#$NAME#0" >> $TMP/$FILENAME.list
		done
	fi
  	echo "$TMP/$FILENAME.list"
}

search()
{
	PAGE2=$($curlbin $URL$PAGE$NEXT | sed "s!$PAGE!\n$PAGE!g" | grep ^$PAGE | cut -d'>' -f2| cut -d'<' -f1 | sort | tail -n1)
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - $URL$PAGE$NEXT | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v PAGE2=$PAGE2 -v NEXT=$NEXT -v URL=$URL \
		'
			# BEGIN variable setzen
			BEGIN \
				{
					# setzt suchvariable auf 0 vor dem start
					suche = 0
					newpage = ""
					pages = "0"
					piccount = 0
                    foundpages = 0
				}
				# eindeutige zeile vor ersten treffer
                #<h3 style="padding:20px;">Suchergebnisse: 5</h3>
				/<h3 style="padding:20px;">Suchergebnisse:/  \
				{
#	    			    print "aaaaaa: " $0

					    # suche erlauben ab dieser zeile
					    suche = 1
					    # in naechste zeile springen

					    i = index($0, ">Suchergebnisse: ") + 17
		                j = index(substr($0, i), "<") - 1
		                results = substr($0, i, j)
#                       print "results: " results
                        if (int(results) > 32)
                        {
                            pages = int((results / 32) + 1)
                        }
                        else
                        {
                            pages = 0
                        }
                    foundpages = 1
					next
				}
                /<div id="content" role="main"/ \
                {
#                   print "bbbbbb: " $0
				    suche = 1
                    next
                }
                #</div>
                #    <a href="//filmpalast.to/stream/rambo-5-last-blood" title="Rambo 5 Last Blood"> <img  width="236px" height="338px" src="/files/movies/315/rambo-5-last-blood.jpg" class="cover-opacity" alt="stream Rambo 5 Last Blood"/></a>
                #</article>  
                /<\/div>/ \
                {
                    if (suche == 1)
				    {
                        found = 1
                        next
                    }
                }
                /<a href="/ \
                {
				    print "1cccccc: " $0

                    if (found == 1 && $0 ~ /title=\"/)
				    {
#        			    print "2cccccc: " $0

                        found = 0
					    i = index($0, "<a href=\"") + 9
		                j = index(substr($0, i), "\"") - 1
		                newpage = substr($0, i, j)

					    i = index($0, "title=\"") + 7
		                j = index(substr($0, i), "\"") - 1
		                title = substr($0, i, j)
    				    print "title: " title

					    i = index($0, "src=\"") + 5
		                j = index(substr($0, i), "\"") - 1
		                pic = substr($0, i, j)
					    
					    if ( pic == "" )
					    {
						    picname = tolower(title)
	    
						    # 23. tausche leehrzeichen in punkte
						    # 24. titel = reife.frauen
		                	gsub(" ", ".", picname)
						    pic = "http://openaaf.dyndns.tv/mediathek/menu/" picname ".jpg"
					    }
                        else
                        {
                            pic = URL pic
                        }
	    
					    if (title != "")
					    {
						    piccount += 1
						    # 25. in naechste zeile springen
						    # 26. \x27 = single quotes
							print title "#" SRC " " SRC " hosterlist \x27" newpage "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#0"
					    }
					    
					    # 27. reset variables
					    newpage = ""
					    title = ""
					    picname = ""
					    pic = ""
					    # 28. in naechste zeile springen
					    next
                    }
                }
                /<div id="paging">/ \
                {
                        suche = 0
                        next

                }
			END \
				{
					if (pages != "0")# && pages >= NEXT + 1)
						print "Page (" NEXT + 1 "/" pages ")#" SRC " " SRC " " INPUT " \x27" PAGE "\x27 " NEXT + 1 "#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#" NAME "#0"
					else if (NEXT != PAGE2 && PAGE2 != "")
						print "Page (" NEXT + 1 "/" PAGE2 ")#" SRC " " SRC " " INPUT " \x27" PAGE "\x27 " NEXT + 1 " " PAGE2 "#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#" NAME "#0"

				}
		# 29. schreibe alles in die list datei
		' >$TMP/$FILENAME.list
	fi
	# 30. gebe titan den list namen mit pfad zurueck
	echo "$TMP/$FILENAME.list"
}

hosterlist()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - https:$PAGE$NEXT | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT -v URL=$URL \
		'
			# BEGIN variable setzen
			BEGIN \
				{
					# setzt suchvariable auf 0 vor dem start
					suche = 0
					newpage = ""
					pages = "0"
					piccount = 0
				}
                /class="button rb iconPlay"/ \
                {
				    print "hosterlist: " $0

				    i = index($0, "href=\"") + 6
                    j = index(substr($0, i), "\"") - 1
                    newpage = substr($0, i, j)

                    if ($0 ~ /data-player-url=/)
                    {
    				    i = index($0, "data-player-url=\"") + 17
	                    j = index(substr($0, i), "\"") - 1
	                    newpage = substr($0, i, j)
                    }

				    i = index($0, "//") + 2
	                j = index(substr($0, i), "/") - 1
	                title = substr($0, i, j)
    					    
				    if ( pic == "" )
				    {
                        split(title, a, ".")
					    picname = tolower(a[1])
    
					    # 23. tausche leehrzeichen in punkte
					    # 24. titel = reife.frauen
	                	gsub(" ", ".", picname)
					    pic = "http://openaaf.dyndns.tv/mediathek/menu/" picname ".jpg"
				    }
                    else
                    {
                        pic = URL pic
                    }
    
				    if (title != "")
				    {
					    piccount += 1
					    # 25. in naechste zeile springen
					    # 26. \x27 = single quotes
						print title "#" SRC " " SRC " hoster \x27" newpage "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#111"
				    }
				    
				    # 27. reset variables
				    newpage = ""
				    title = ""
				    picname = ""
				    pic = ""
				    # 28. in naechste zeile springen
				    next
                }
				# next page init
			END \
				{
				}
		# 29. schreibe alles in die list datei
		' >$TMP/$FILENAME.list
	fi
	# 30. gebe titan den list namen mit pfad zurueck
	echo "$TMP/$FILENAME.list"
}

hoster()
{
	echo $PAGE
}

case $INPUT in
	init) $INPUT;;
	mainmenu) $INPUT;;
	genre) $INPUT;;
	search) $INPUT;;
	sorted) $INPUT;;
	hosterlist) $INPUT;;
	hoster) $INPUT;;
esac
