#!/bin/bash
# box parser for titannit

SRC=$1
INPUT=$2
PAGE=$3
NEXT=$4

URL="http://kkiste.to/"
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME=`echo -n ${PARSER:0:2} | tr '[a-z]' '[A-Z]'`${PARSER:2}

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

if [ `echo $SRC | grep ^"/mnt/parser" |wc -l` -gt 0 ];then
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
	echo "Neue Filme#$SRC $SRC videos neue-filme/?page= 1#http://openaaf.dyndns.tv/mediathek/menu/all-newfirst.jpg#all-newfirst.jpg#$NAME#0" >$TMP/$PARSER.$INPUT.list
	echo "Kinofilme#$SRC $SRC videos aktuelle-kinofilme/?page= 1#http://openaaf.dyndns.tv/mediathek/menu/all-newfirst.jpg#all-newfirst.jpg#$NAME#0" >>$TMP/$PARSER.$INPUT.list
	echo "Serien#$SRC $SRC videos serien/?page= 1#http://openaaf.dyndns.tv/mediathek/menu/series.jpg#series.jpg#$NAME#0" >>$TMP/$PARSER.$INPUT.list
	echo "Genre#$SRC $SRC genre#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#gebre.jpg#$NAME#0" >>$TMP/$PARSER.$INPUT.list
	echo "A-Z#$SRC $SRC sorted#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#0" >>$TMP/$PARSER.$INPUT.list
	echo "Search#$SRC $SRC search 'search/?q=%search%'#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >>$TMP/$PARSER.$INPUT.list

#	echo "d 6#$SRC $SRC search 'film-index/d/?page=' 6#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#0" >>$TMP/$PARSER.$INPUT.list

	echo "$TMP/$PARSER.$INPUT.list"
}

sorted()
{
	watchlist="0 1 2 3 4 5 6 7 8 9 a b c d e f g h i j k l m n o p q r s t u v w x y z"

	rm $TMP/$PARSER.$INPUT.list > /dev/null 2>&1

	for ROUND0 in $watchlist; do
		TITLE=`echo $ROUND0`
		filename=`echo $TITLE`
		echo "$TITLE#$SRC $SRC search film-index/$ROUND0/?page= 1#http://openaaf.dyndns.tv/mediathek/menu/$filename.jpg#$filename.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	done
  	echo "$TMP/$PARSER.$INPUT.list"
}

genre()
{
	watchlist="
		abenteuer
		action
		animation
		biographie
		bollywood
		dokumentation
		drama
		familie
		fantasy
		geschichte
		horror
		klassiker
		komoedie
		kriegsfilm
		krimi
		musik
		mystery
		romantik
		sci-fi
		sport
		thriller
		western
		"
	rm $TMP/$PARSER.$INPUT.list > /dev/null 2>&1

	for ROUND0 in $watchlist; do
		TITLE=`echo $ROUND0 | tr '/' '-' | sed 's/free-//g'`
		filename=`echo $TITLE`	
		echo "$TITLE#$SRC $SRC videos $ROUND0/?page= 1#http://openaaf.dyndns.tv/mediathek/menu/$filename.jpg#$filename.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	done
  	echo "$TMP/$PARSER.$INPUT.list"
}

search()
{
	if [ -z "$NEXT" ]; then NEXT="search"; fi

	if [ ! -e "$TMP/$PARSER.$INPUT.$NEXT.$FILENAME.list" ]; then
		piccount=0
	
		if [ "$NEXT" == "search" ]; then
			$curlbin $URL/$PAGE -o $TMP/cache.$PARSER.$INPUT.$NEXT.$FILENAME.1
		else
			$curlbin $URL/$PAGE$NEXT -o $TMP/cache.$PARSER.$INPUT.$NEXT.$FILENAME.1
		fi
	
		cat $TMP/cache.$PARSER.$INPUT.$NEXT.$FILENAME.1 | tr '\n' '\r' |  tr '\r' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!<li class="mbox list"!\nfound=!g' | sed 's!<div class="clear">!\n<div class="clear">!g' | grep ^"found=" >$TMP/cache.$PARSER.$INPUT.$NEXT.$FILENAME.2
	
		pages=`cat $TMP/cache.$PARSER.$INPUT.$NEXT.$FILENAME.1 | grep 'div class="paginated"' | sed 's!<li><a href=!\nfound=!g' | grep ^"found=" | cut -d'"' -f2 | cut -d'=' -f2 | tail -n1`
	
		while read -u 3 ROUND; do
			ID=`echo $ROUND | cut -d'"' -f1 | tail -n1`
			PIC=`echo $ROUND | sed 's!"url": !\nurl=!g' | grep ^url= | cut -d'"' -f2 | tail -n1`
			TITLE=`echo $ROUND | sed 's!class="title">!\ntitle=<!g' | grep ^title= | cut -d'<' -f2`
			NEWPAGE=`echo $ROUND | sed 's!<a href=!\nfound=!g' | grep ^found= | cut -d'"' -f2 | tail -n1`
	
			PIC=`$curlbin "$URL/$NEWPAGE" | tr '\n' '\r' |  tr '\r' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!<img src=!\nfound=!g' | grep ^found= | cut -d'"' -f2 | tail -n1`
	
			if [ -z "$PIC" ]; then
				PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
			fi
	
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
	
			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
				if [ ! -e $TMP/$PARSER.$INPUT.$INPUT.$FILENAME.list ];then
					touch $TMP/$PARSER.$INPUT.$INPUT.$FILENAME.list
				fi
				piccount=`expr $piccount + 1`
				LINE="$TITLE $LANG#$SRC $SRC parts $NEWPAGE#$PIC#$PARSER.$INPUT.$FILENAME.$piccount.jpg#$NAME#0"
	
				echo "$LINE" >> $TMP/$PARSER.$INPUT.$NEXT.$FILENAME.list
			fi
	
		done 3<$TMP/cache.$PARSER.$INPUT.$NEXT.$FILENAME.2
	
		if [ "$NEXT" -lt "$pages" ]; then
			NEXTPAGE=`expr $NEXT + 1`
			LINE="Page ($NEXTPAGE/$pages)#$SRC $SRC search '$PAGE' $NEXTPAGE#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#$NAME#0"
			echo "$LINE" >> $TMP/$PARSER.$INPUT.$NEXT.$FILENAME.list
		fi
	
		rm $TMP/cache.* > /dev/null 2>&1
	fi
	echo "$TMP/$PARSER.$INPUT.$NEXT.$FILENAME.list"
}

videos()
{
	$curlbin -o - $URL$PAGE$NEXT | awk -v SRC=$SRC -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT -v PARSER=$PARSER -v NAME=$NAME '

BEGIN { title=""
        url=""
        pic=""
      }

/<a href="/ { if(match($0, /class="title">/) != 0) {
                 i = match($0, /<a href="/)
                 split(substr($0, i), a, "\"")
                 url = a[2]
                 i = match($0, / title="Jetzt /)
                 split(substr($0, i), a, "\"")
                 title = substr(a[2], 7, length(a[2]) - 21)
              }
            }

/<img src="/ { i = match($0, /<img src="/)
               split(substr($0, i), a, "\"")
               pic = a[2]
             }

/<div class="title"><strong>/ { n = split(pic, a, "/")
                                pic2 = a[n]
                                print title "#" SRC " " SRC " parts " url "#" pic "#" pic2 "#" NAME "#0"
                                title=""
                                url=""
                                pic=""
                                pic2=""
                              }

END   { print "Page " NEXT + 1 "#" SRC " " SRC " " INPUT " " PAGE " " NEXT + 1 "#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#" NAME "#0"
      }
' >$TMP/$PARSER.$INPUT.list
	echo "$TMP/$PARSER.$INPUT.list"
}

parts()
{
	$curlbin -o - $URL$PAGE >$TMP/cache.$PARSER.$INPUT.1

	cat $TMP/cache.$PARSER.$INPUT.1 | awk -v SRC=$SRC -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT -v PARSER=$PARSER -v NAME=$NAME '
BEGIN { kz_parts=0
        count=1
        extra=test
      }

/<div class="mediabar">/ { kz_parts=1
                           next
                 }
/<select class="seasonselect"/ { extra=1
                           next
                 }
/<option value="/ { if(extra=1) {
                 i = match($0, /<option value="/)
                 split(substr($0, i), a, "\"")
                 #print url
                 title = a[2]
				 if(title!="") {
	                 count = title
    	             print "Staffel: " title "#" SRC " " SRC " episodelist " PAGE " " "season=" count "#http://openaaf.dyndns.tv/mediathek/menu/s" title ".jpg#s" title ".jpg#" NAME "#0"
        		 }
                 next
              }
            }

/<a href="/ { if(kz_parts!=0) {
                 i = match($0, /<a href="/)
                 split(substr($0, i), a, "\"")
                 url = a[2]
                 i = match($0, /target="/)
                 split(substr($0, i), a, "\"")
                 title = a[3]
                 sub(/>/, "", title) 
                 sub(/<small>/, "", title) 
                 sub(/<\/small><\/a><\/li>/, "", title) 
                 split(title, a, "\[")
                 split(a[2], b, "\]")
                 split(b[1], a, " ")
                 part = a[2]
                 print title "#" SRC " " SRC " hoster " PAGE " " count "#http://openaaf.dyndns.tv/mediathek/menu/" part ".jpg#" part ".jpg#" NAME "#111"
                 count=count+1
                 next
              }
            }

/<div class="ad leaderboard">/ { kz_parts=0
                                 next
         }
' >$TMP/$PARSER.$INPUT.$FILENAME.list
	echo "$TMP/$PARSER.$INPUT.$FILENAME.list"
}

hoster()
{
	i=`expr $NEXT \* 2`
	data=`$curlbin "$URL$PAGE" | grep "data-det=" | cut -d'"' -f4`

	if [ ! -z "$data" ];then
		id=`$curlbin --header "Content-Type: application/json" -H "X-Requested-With: XMLHttpRequest" -X POST --data "$data" --referer $URL$PAGE http://kkiste.to/xhr/link/ | cut -d'"' -f$i`
	fi

	if [ -z "$id" ];then
		id=`$curlbin "$URL$PAGE" | grep "http://www.ecostream.tv/stream" | sed 's#http://www.ecostream.tv/stream/#\nlink=.#g' | cut -d"." -f2`
	fi
	echo "http://www.ecostream.tv/stream/$id.html"
}

episodelist()
{
	if [ -e "$TMP/$PARSER.$INPUT.list" ] ; then
		rm $TMP/$PARSER.$INPUT.list
	fi

	SEASON=`echo $NEXT | cut -d"=" -f2`
	PARAM1=`echo $PAGE | sed 's!.html!/!g'`

	data=`$curlbin "$URL$PAGE" | grep "data-movie=" | cut -d'"' -f4`
	$curlbin2 -H "X-Requested-With: XMLHttpRequest" -X POST --data "$NEXT" --referer $URL$PAGE http://kkiste.to/xhr/movies/episodes/$data/ -o "$TMP/cache.$PARSER.$INPUT.1"
### curl -k -s -v -H "X-Requested-With: XMLHttpRequest" -X POST --data "season=1" --referer http://kkiste.to/24-twenty-four-stream.html http://kkiste.to/xhr/movies/episodes/24-twenty-four-stream/

	cat $TMP/cache.$PARSER.$INPUT.1 | sed 's!},{!\n{!g' | cut -d"[" -f2 >$TMP/cache.$PARSER.$INPUT.2

	count=0

	while read -u 3 ROUND; do
		count=`expr $count + 1`
	
		TITLE=`echo $ROUND | cut -d'"' -f10 | tail -n1`
		ID=`echo $ROUND | cut -d'"' -f6 | tail -n1`
		NEWPAGE="http://www.ecostream.tv/stream/$ID.html"
		EPISODE=`echo $TITLE | tr ' ' '\n' | tail -n1`
		PICNAME=s"$SEASON"e"$EPISODE".jpg
		PIC="http://openaaf.dyndns.tv/mediathek/menu/$PICNAME"

		TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`

		if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
			if [ ! -e $TMP/$PARSER.$INPUT.$FILENAME.list ];then
				touch $TMP/$PARSER.$INPUT.$FILENAME.list
			fi
#gehen beide varianten weiss nur nicht ob die folgen dann stimmen
#			LINE="$TITLE#$SRC $SRC hoster $PARAM $count#$PIC#$PICNAME#$NAME#111"
			LINE="$TITLE#$NEWPAGE#$PIC#$PICNAME#$NAME#14"

			echo "$LINE" >> $TMP/$PARSER.$INPUT.list
		fi

	done 3<$TMP/cache.$PARSER.$INPUT.2
	rm $TMP/cache.* > /dev/null 2>&1

	echo "$TMP/$PARSER.$INPUT.list"
}


case $INPUT in
#	init) $INPUT;;
	mainmenu) $INPUT;;
	videos) $INPUT;;
	parts) $INPUT;;
	serien) $INPUT;;
	hoster) $INPUT;;
	search) $INPUT;;
	page) $INPUT;;
	episodelist) $INPUT;;
	sorted) $INPUT;;
	genre) $INPUT;;
esac

