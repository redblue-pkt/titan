#!/bin/sh
# first sh box parser for titannit mfg obi

SRC=$1
INPUT=$2
PAGE=$3
NEXT=$4
PAGE2=$5
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`

#URL=https://s.to
URL=https://api.netzkino.de.simplecache.net/
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME=Netzkino

#http://api.netzkino.de.simplecache.net/capi-2.0a/categories/scifikino.json?d=www
#http://api.netzkino.de.simplecache.net/capi-2.0a/categories/kinoab18.json?d=www


#URL_MAIN = 'https://api.netzkino.de.simplecache.net/capi-2.0a/categories/%s.json?d=www&l=de-DE'
#URL_SEARCH = 'https://api.netzkino.de.simplecache.net/capi-2.0a/search?q=%s&d=www&l=de-DE'

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
	echo "Kategorien#$SRC $SRC category '/devmobileapp/metachannels' 1#http://openaaf.dyndns.tv/mediathek/menu/category.jpg#category.jpg#$NAME#0" >$TMP/$FILENAME.list
	echo "Suche#$SRC $SRC list 'capi-2.0a/search?q=%search%&d=www' 2#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#all-sorted.jpg#$NAME#112" >>$TMP/$FILENAME.list
	echo "$TMP/$FILENAME.list"
}

category()
{
#curl -o - https://www.netzkino.de/  > $TMP/$FILENAME.list1
#curl -o - https://www.netzkino.de/ | sed 's/{/\n\n{/g' | grep '"id"' | grep '"title"' | grep CmsCategory > $TMP/$FILENAME.list2
#exit
#rm $TMP/$FILENAME.list		
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		curl -o - https://www.netzkino.de/ | sed 's/{/\n\n{/g' | grep '"id"' | grep '"title"' | grep CmsCategory | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT \
		'
			# BEGIN variable setzen
			BEGIN \
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

				i = index($0, "\"title\":\"") + 9
	            j = index(substr($0, i), "\",\"") - 1
				title = substr($0, i, j)

				i = index($0, "\"slug\":\"") + 8
	            j = index(substr($0, i), "\"") - 1
				newpage = substr($0, i, j)

				picname = title

                gsub(" ", "_", picname)
                gsub("-", "_", picname)

	           	pic = "http://openaaf.dyndns.tv/mediathek/menu/"

				piccount += 1

                if (newpage == "hdkino" || newpage == "western-frontpage" || newpage == "frontpage" || newpage == "netzkino-genre")
                    newpage = ""

                if (newpage != "" &&  title != "")
    				print title "#" SRC " " SRC " list \x27http://api.netzkino.de.simplecache.net/capi-2.0a/categories/" newpage ".json?d=www\x27#" pic tolower(picname) ".jpg#" PICNAME "2." piccount ".jpg#" NAME "#0"

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

list()
{	
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
                suche = 0
			}
			/"id"/ \
			{
                suche = 1

				i = index($0, "\"id\":\"") + 6
	            j = index(substr($0, i), "\",\"") - 1
				id = substr($0, i, j)

				i = index($0, "\"title\":\"") + 9
	            j = index(substr($0, i), "\",\"") - 1
				title = substr($0, i, j)

				i = index($0, "\"content\":\"") + 11
	            j = index(substr($0, i), "\",\"") - 1
				desc = substr($0, i, j)

                next
			}
			/"Adaptives_Streaming"/ \
			{
                if (suche == 1)
                {
				    i = index($0, "\"Streaming\":[\"") + 14
	                j = index(substr($0, i), "\"") - 1
				    newpage = substr($0, i, j)

                    if (newpage == "")
                    {
    				    i = index($0, "\"Youtube_Delivery_Id\":[\"") + 24
    	                j = index(substr($0, i), "\"") - 1
    				    youtube = substr($0, i, j)
                    }

				    i = index($0, "\"Artikelbild\":[\"") + 16
	                j = index(substr($0, i), "\"") - 1
				    pic = substr($0, i, j)

				    i = index($0, "\"Jahr\":[\"") + 9
	                j = index(substr($0, i), "\"") - 1
				    year = substr($0, i, j)

				    i = index($0, "\"IMDb-Bewertung\":[\"") + 19
	                j = index(substr($0, i), "\"") - 1
				    imdb = substr($0, i, j)

                    if (year != "" && imdb != "")
                        extra = " ("year") ("imdb")"
                    else if (year != "")
                        extra = " ("year")"
                    else
                        extra = ""

				    piccount += 1
#    			    print title extra "#http://netzkino_and-vh.akamaihd.net/i/" newpage ".mp4/master.m3u8#" pic "#" PICNAME "." piccount ".jpg#" NAME "#2#" desc
    			    print title extra "#https://pmd.netzkino-seite.netzkino.de/" newpage ".mp4#" pic "#" PICNAME "." piccount ".jpg#" NAME "#2#" desc
                   
                    if (youtube != "")
                    {
    				    print title extra " (YT1)#https://www.youtube.com/watch?v=" youtube "#" pic "#" PICNAME "." piccount ".jpg#" NAME "#2#" desc
    				    print title extra " (YT2)#" SRC " " SRC " play \x27https://www.youtube.com/watch?v=" youtube "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#111#" desc
                    }

                    year = ""
                    pic = ""
                    newpage = ""
                    imdb = ""
                    title = ""
                    youtube = ""
                    suche = "0"
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
	search) $INPUT;;
esac
