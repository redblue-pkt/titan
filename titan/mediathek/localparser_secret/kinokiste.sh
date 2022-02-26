#!/bin/sh
# first sh box parser for titannit mfg obi

SRC=$1
INPUT=$2
PAGE=$3
NEXT=$4
PAGE2=$5
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`

URL=https://api.tmdb.club/
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME=KinoKiste

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
#https://api.tmdb.club/data/browse/?lang=all&type=movies&order_by=featured&page=1

#	echo "en 1movies-featured#$SRC $SRC list '/data/browse/?lang=1&type=movies&order_by=featured&page=' 1#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#all-sorted.jpg#$NAME#112" >>$TMP/$FILENAME.list
#	echo "de 2movies-featured#$SRC $SRC list '/data/browse/?lang=2&type=movies&order_by=featured&page=' 1#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#all-sorted.jpg#$NAME#112" >>$TMP/$FILENAME.list
#	echo "en 3movies-featured#$SRC $SRC list '/data/browse/?lang=3&type=movies&order_by=featured&page=' 1#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#all-sorted.jpg#$NAME#112" >>$TMP/$FILENAME.list
#	echo "allmovies-featured#$SRC $SRC list '/data/browse/?lang=all&type=movies&order_by=featured&page=' 1#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#all-sorted.jpg#$NAME#112" >>$TMP/$FILENAME.list
	echo "Filme#$SRC $SRC category movies#http://openaaf.dyndns.tv/mediathek/menu/movies.jpg#movies.jpg#$NAME#0" >$TMP/$FILENAME.list
	echo "Serien#$SRC $SRC category tvseries#http://openaaf.dyndns.tv/mediathek/menu/series.jpg#series.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Suche#$SRC $SRC search '/data/browse/?lang=all&keyword=%search%&page=' 1#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#all-sorted.jpg#$NAME#112" >>$TMP/$FILENAME.list
	echo "$TMP/$FILENAME.list"
}

category()
{
#rm $TMP/$FILENAME.list
	if [ ! -e "$TMP/$FILENAME.list" ]; then
        if [ "$PAGE" == "movies" ];then
    		watchlist="featured releases trending updates requested rating votes views"
        elif [ "$PAGE" == "tvseries" ];then
		    watchlist="releases trending updates requested rating votes views"
        fi

		rm $TMP/$FILENAME.list > /dev/null 2>&1

		for ROUND0 in $watchlist; do
			TITLE=`echo $ROUND0`
			filename=`echo $TITLE | tr [A-Z] [a-z]`
            typename=`echo $PAGE | tr [A-Z] [a-z]`
            picname="$typename.$filename"
	        echo "$TITLE#$SRC $SRC search '/data/browse/?lang=all&type=$PAGE&order_by=$ROUND0&page=' 1#http://openaaf.dyndns.tv/mediathek/menu/$picname.jpg#$picname.jpg#$NAME#0" >>$TMP/$FILENAME.list
		done
	fi
  	echo "$TMP/$FILENAME.list"
}

search()
{
#rm $TMP/$FILENAME.list
	if [ ! -e "$TMP/$FILENAME.list" ]; then
#        if [ -z "$NEXT" ];then URL=""; fi
		curl -o - $URL$PAGE$NEXT | sed 's/{/\n\n{/g' | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v URL=$URL -v PAGE=$PAGE -v NEXT=$NEXT \
		'
			# BEGIN variable setzen
			BEGIN \
			{
				# setzt suchvariable auf 0 vor dem start
				piccount = 0
				pages = "1"
                suche = 0
			}
			/"pages"/ \
			{
				i = index($0, "\"totalPages\":") + 13
	            j = index(substr($0, i), ",") - 1
				pages = substr($0, i, j)

				i = index($0, "\"currentPage\":") + 14
	            j = index(substr($0, i), ",") - 1
				curpage = substr($0, i, j)
			}
			/"slug"/ \
			{
#print "a " $a
                suche = 1

				i = index($0, "\"_id\":\"") + 7
	            j = index(substr($0, i), "\",\"") - 1
				id = substr($0, i, j)

				i = index($0, "\"title\":\"") + 9
	            j = index(substr($0, i), "\",\"") - 1
				title = substr($0, i, j)

                if ($a ~ /\"storyline\"/)
                {
				    i = index($0, "\"storyline\":\"") + 13
	                j = index(substr($0, i), "\",\"") - 1
				    desc = substr($0, i, j)
                }

			    i = index($0, "\"year\":") + 7
                j = index(substr($0, i), ",") - 1
			    year = substr($0, i, j)

			    i = index($0, "\"lang\":") + 7
                j = index(substr($0, i), ",") - 1
			    lang = substr($0, i, j)

                next
			}
            /"poster_path"/ \
			{
#print "b " $a
                gsub(/\\"/, "\"", $a)
#print "c " $a
                gsub(/\\/, "", $a)
#print "d " $a
                if (suche == 1)
                {
				    i = index($0, "\"poster_path\":\"") + 15
	                j = index(substr($0, i), "\"") - 1
				    pic = substr($0, i, j)

                    if ( pic == "" )
                    {
                        print "pic emthy"
				        i = index($0, "\"poster\":\"") + 10
	                    j = index(substr($0, i), "\"") - 1
				        pic = substr($0, i, j)
                    }
                    else if ( pic == "" )
                    {
                        print "pic emthy2"
                        #"img_link":"http://i.imgur.com/jBGWN7t.jpg"
				        i = index($0, "\"img_link\":\"") + 12
	                    j = index(substr($0, i), "\"") - 1
				        pic = substr($0, i, j)
                    }
                    else
                        pic = "https://image.tmdb.org/t/p/w300"pic

#print "desc1 " desc

                    if ( desc == "\"" || desc == "" )
                    {
        				i = index($0, "\"overview\":\"") + 12
        	            j = index(substr($0, i), "\",\"") - 1

        				desc = substr($0, i, j)
#print "desc2 " desc

                        if ( desc == "\"" || desc == "" )
                        {

                            gsub(/\\"/, "\"", $0)
#print "d " $a


#print "desc emthy"
        	    			i = index($0, "\"storyline\":\"") + 13
        	                j = index(substr($0, i), "\",\"") - 1
        	    			desc = substr($0, i, j)
#print "desc3 " desc
                        }
                    }
                    if (lang == 2)
                        lang = "de"
                    else if (lang == 3)
                        lang = "en"
                    else
                        lang = "??"

                    if (year != "" && year != "0" && lang != "")
                        extra = " ("year") ("lang")"
                    else if (year != "" && year != "0")
                        extra = " ("year")"
                    else
                        extra = ""

				    piccount += 1
#    			    print title extra "#" SRC " " SRC " hosterlist \x27https://api.tmdb.club/data/watch/?_id=" id "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#0#" desc
#"storyline":false
#"overview":"",
                    if ($a ~ /\"storyline\":false/ && $a ~ /\"overview\":\"\"/)
        			    print title extra "#" SRC " " SRC " hosterlist \x27" URL "/data/watch/?_id=" id "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#0#"
                    else
        			    print title extra "#" SRC " " SRC " hosterlist \x27" URL "/data/watch/?_id=" id "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#0#" desc

                    id = ""
                    title = ""
                    pic = ""
                    year = ""
                    lang = ""
                    desc = ""
                    extra = ""
                    suche = "0"
                }
				next
			}
			END \
			{
				if (curpage != pages)
					print "Page (" NEXT + 1 "/" pages ")#" SRC " " SRC " " INPUT " \x27" PAGE "\x27 " NEXT + 1 "#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#" NAME "#0"
			}
		# schreibe alles in die list datei
		' >$TMP/$FILENAME.list
	fi
	# gebe titan den list namen mit pfad zurueck
	echo "$TMP/$FILENAME.list"
}

hosterlist()
{
#rm $TMP/$FILENAME.list
	if [ ! -e "$TMP/$FILENAME.list" ]; then
        if [ -z "$NEXT" ];then URL=""; fi
		curl -o - $URL$PAGE$NEXT | sed 's/{/\n\n{/g' | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT \
		'
			# BEGIN variable setzen
			BEGIN \
			{
				# setzt suchvariable auf 0 vor dem start
				piccount = 0
				pages = "1"
                suche = 1
			}
			/"stream"/ \
			{
                if (suche == 1)
                {
				    i = index($0, "\"stream\":\"") + 10
	                j = index(substr($0, i), "\"") - 1
				    newpage = substr($0, i, j)

				    i = index(newpage, "://") + 3
	                j = index(substr(newpage, i), "/") - 1
				    title = substr(newpage, i, j)

                    split(title, a, ".")
				    picname = tolower(a[1])

				    # 23. tausche leehrzeichen in punkte
				    # 24. titel = reife.frauen
                	gsub(" ", ".", picname)
				    pic = "http://openaaf.dyndns.tv/mediathek/menu/" picname ".jpg"

				    i = index($0, "\"added\":\"") + 9
	                j = index(substr($0, i), "T") - 1
				    date = substr($0, i, j)

				    i = index($0, "\"release\":\"") + 11
	                j = index(substr($0, i), "\"") - 1
				    release = substr($0, i, j)

                    if (release != "" && release != "0" && date != "")
                        extra = " ("release") ("date")"
                    else if (year != "" && year != "0")
                        extra = " ("release")"
                    else
                        extra = ""

				    piccount += 1
    			    print title extra "#" SRC " " SRC " hoster \x27" newpage "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#111"

                    newpage = ""
                    title = ""
                    pic = ""
                    extra = ""
                    release = ""
                    data = ""
                }
				next
			}
			END \
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

hoster()
{
	echo $PAGE
}

play()
{
	/tmp/localhoster/hoster.sh youtube_dl $PAGE
}

case $INPUT in
	init) $INPUT;;
	mainmenu) $INPUT;;
	category) $INPUT;;
	list) $INPUT;;
	play) $INPUT;;
	hosterlist) $INPUT;;
	hoster) $INPUT;;
	search) $INPUT;;
esac
