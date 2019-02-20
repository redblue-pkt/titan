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
FILENAME=`echo $FILENAME | tr '&' '.' | tr '/' '.' | tr '?' '.'  | tr '=' '.' | sed 's/ \+/./g' | sed 's/\.\+/./g'`

if [ -z "$FILENAME" ]; then
	FILENAME=none
fi

URL=`cat /mnt/config/titan.cfg | grep tithek_kinox_url | grep -v "#" | cut -d "=" -f2`
if [ -z "$URL" ];then
	URL=https://kinoxto.stream
fi
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME=KinoX

if [ `cat /mnt/config/titan.cfg | grep tithek_kinox_localhoster=1 | wc -l` -eq 1 ];then
	ACTIVEBIN="$curlbin" 
elif [ `cat /mnt/config/titan.cfg | grep tithek_kinox_localhoster=2 | wc -l` -eq 1 ];then
	ACTIVEBIN="$BIN /tmp/localhoster/cloudflare.py"
else
	ACTIVEBIN="$curlbin" 
	URL=`echo $URL | sed 's/https:/http:/'`
fi

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
	echo "Kino#$SRC $SRC kino 'Kino-filme.html'#http://atemio.dyndns.tv/mediathek/menu/Cine-Films.jpg#Cine-Films.jpg#$NAME#0" >$TMP/$FILENAME.list

	echo "Movie (Latest)#$SRC $SRC latest 'Latest-Movies.html'#http://atemio.dyndns.tv/mediathek/menu/Latest-Movies.jpg#Latest-Movies.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Movie (Popular)#$SRC $SRC latest 'Popular-Movies.html'#http://atemio.dyndns.tv/mediathek/menu/Popular-Movies.jpg#Popular-Movies.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Movie (A-Z)#$SRC $SRC sorted movie#http://atemio.dyndns.tv/mediathek/menu/a-z.jpg#a-z.jpg#$NAME#0" >>$TMP/$FILENAME.list

	echo "Tv (Latest)#$SRC $SRC latest 'Latest-TVSeries.html'#http://atemio.dyndns.tv/mediathek/menu/Latest-Series.jpg#Latest-Series.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Tv (Popular)#$SRC $SRC latest 'Popular-TVSeries.html'#http://atemio.dyndns.tv/mediathek/menu/Popular-Series.jpg#Popular-Series.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Tv (A-Z)#$SRC $SRC sorted series#http://atemio.dyndns.tv/mediathek/menu/a-z.jpg#a-z.jpg#$NAME#0" >>$TMP/$FILENAME.list

	echo "Doku (Latest)#$SRC $SRC latest 'Latest-Documentations.html'#http://atemio.dyndns.tv/mediathek/menu/Latest-Documentations.jpg#Latest-Documentations.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Doku (Popular)#$SRC $SRC latest 'Popular-Documentations.html'#http://atemio.dyndns.tv/mediathek/menu/Popular-Documentations.jpg#Popular-Documentations.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Doku (A-Z)#$SRC $SRC sorted documentation#http://atemio.dyndns.tv/mediathek/menu/a-z.jpg#a-z.jpg#$NAME#0" >>$TMP/$FILENAME.list

	echo "Actor (A-Z)#$SRC $SRC sorted actor#http://atemio.dyndns.tv/mediathek/menu/a-z.jpg#a-z.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Director (A-Z)#$SRC $SRC sorted director#http://atemio.dyndns.tv/mediathek/menu/a-z.jpg#a-z.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Search#http://atemio.dyndns.tv/mediathek/youtube/streams/kinox.all-sorted.list#http://atemio.dyndns.tv/mediathek/menu/search.jpg#search.jpg#KinoX#21" >>$TMP/$FILENAME.list
	echo "Search Cast#http://atemio.dyndns.tv/mediathek/youtube/streams/kinox.all-sorted.list#http://atemio.dyndns.tv/mediathek/menu/search.jpg#search10.jpg#KinoX#31" >>$TMP/$FILENAME.list
	echo "$TMP/$FILENAME.list"
}


#https://www.kinos.to/aGET/List/?sEcho=2&iColumns=7&sColumns=&iDisplayStart=@NEXT@&iDisplayLength=25&iSortingCols=1&iSortCol_0=2&sSortDir_0=asc&bSortable_0=true&bSortable_1=true&bSortable_2=true&bSortable_3=false&bSortable_4=false&bSortable_5=false&bSortable_6=true&additional=%7B%22fType%22%3A%22movie%22%2C%22Length%22%3A60%2C%22fLetter%22%3A1%7D
                                                                                                                                                                                                                                                                          additional=%7B%22fType%22%3A%22$@PAGE2@%22%2C%22fLetter%22%3A%22$@PAGE@%22%7D

#https://www.kinos.to/aGET/Actor/?sEcho=1&iColumns=2&sColumns=&iDisplayStart=0&iDisplayLength=25&iSortingCols=1&iSortCol_0=1&sSortDir_0=asc&bSortable_0=true&bSortable_1=false&additional=%7B%22fLetter%22%3A%22B%22%2C%22fSex%22%3A%22%22%7D
#https://www.kinos.to/aGET/Actor/?sEcho=1&iColumns=2&sColumns=&iDisplayStart=0&iDisplayLength=25&iSortingCols=1&iSortCol_0=1&sSortDir_0=asc&bSortable_0=true&bSortable_1=false&additional=%7B%22fLetter%22%3A%22A%22%2C%22fSex%22%3A%22%22%7D
#https://www.kinos.to/aGET/Director/?sEcho=1&iColumns=2&sColumns=&iDisplayStart=0&iDisplayLength=25&iSortingCols=1&iSortCol_0=1&sSortDir_0=asc&bSortable_0=true&bSortable_1=false&additional=%7B%22fLetter%22%3A%22A%22%2C%22fSex%22%3A%22%22%7D
#https://www.kinos.to/aGET/List/?sEcho=2&iColumns=7&sColumns=&iDisplayStart=0&iDisplayLength=25&iSortingCols=1&iSortCol_0=2&sSortDir_0=asc&bSortable_0=true&bSortable_1=true&bSortable_2=true&bSortable_3=false&bSortable_4=false&bSortable_5=false&bSortable_6=true&additional=%7B%22Length%22%3A60%2C%22fLetter%22%3A1%2C%22fGenre%22%3A%226%22%7D
#https://www.kinos.to/aGET/List/?sEcho=2&iColumns=7&sColumns=&iDisplayStart=0&iDisplayLength=25&iSortingCols=1&iSortCol_0=2&sSortDir_0=asc&bSortable_0=true&bSortable_1=true&bSortable_2=true&bSortable_3=false&bSortable_4=false&bSortable_5=false&bSortable_6=true&additional=%7B%22fType%22%3A%22movie%22%2C%22Length%22%3A60%2C%22fLetter%22%3A1%7D
#https://www.kinos.to/aGET/List/?sEcho=2&iColumns=7&sColumns=&iDisplayStart=0&iDisplayLength=25&iSortingCols=1&iSortCol_0=2&sSortDir_0=asc&bSortable_0=true&bSortable_1=true&bSortable_2=true&bSortable_3=false&bSortable_4=false&bSortable_5=false&bSortable_6=true&additional=%7B%22fType%22%3A%22documentation%22%2C%22Length%22%3A60%2C%22fLetter%22%3A1%7D
#https://www.kinos.to/aGET/List/?sEcho=2&iColumns=7&sColumns=&iDisplayStart=0&iDisplayLength=25&iSortingCols=1&iSortCol_0=2&sSortDir_0=asc&bSortable_0=true&bSortable_1=true&bSortable_2=true&bSortable_3=false&bSortable_4=false&bSortable_5=false&bSortable_6=true&additional=%7B%22fType%22%3A%22series%22%2C%22Length%22%3A60%2C%22fLetter%22%3A1%7D
#https://www.kinos.to/aGET/List/?sEcho=3&iColumns=7&sColumns=&iDisplayStart=0&iDisplayLength=25&iSortingCols=1&iSortCol_0=2&sSortDir_0=asc&bSortable_0=true&bSortable_1=true&bSortable_2=true&bSortable_3=false&bSortable_4=false&bSortable_5=false&bSortable_6=true&additional=%7B%22fType%22%3A%22series%22%2C%22Length%22%3A60%2C%22fLetter%22%3A%22B%22%7D

sorted()
{

	if [ ! -e "$TMP/$FILENAME.list" ]; then
		watchlist="1 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z"

		rm $TMP/$FILENAME.list > /dev/null 2>&1

		for ROUND0 in $watchlist; do
			TITLE=`echo $ROUND0`
			if [ "$ROUND0" == "1" ];then
				TITLE="0-9"
			fi

			filename=`echo $TITLE | tr [A-Z] [a-z]`
#			echo "$TITLE#$SRC $SRC search '$ROUND0' 1 $PAGE#http://atemio.dyndns.tv/mediathek/menu/$filename.jpg#$filename.jpg#$NAME#0" >> $TMP/$FILENAME.list
			echo "$TITLE#$SRC $SRC search '$PAGE' 1 '$ROUND0'#http://atemio.dyndns.tv/mediathek/menu/$filename.jpg#$filename.jpg#$NAME#0" >> $TMP/$FILENAME.list

		done
	fi
  	echo "$TMP/$FILENAME.list"
}

search()
{
#rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
#rm $TMP/$FILENAME.list
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		piccount=0
#		$ACTIVEBIN "$URL/aGET/List/?sEcho=1&iColumns=10&sColumns=&iDisplayStart=$NEXT&iDisplayLength=50&iSortingCols=1&iSortCol_0=5&sSortDir_0=asc&bSortable_0=true&bSortable_1=true&bSortable_2=true&bSortable_3=false&bSortable_4=false&bSortable_5=false&bSortable_6=true&additional=%7B%22fType%22%3A%22$PAGE2%22%2C%22fLetter%22%3A%22$PAGE%22%7D" > $TMP/cache.$FILENAME.1
		if [ "$PAGE" == "movie" ] || [ "$PAGE" == "series" ] || [ "$PAGE" == "documentation" ];then
#			$ACTIVEBIN "$URL/aGET/List/?sEcho=1&iColumns=10&sColumns=&iDisplayStart=$NEXT&iDisplayLength=50&iSortingCols=1&iSortCol_0=5&sSortDir_0=asc&bSortable_0=true&bSortable_1=true&bSortable_2=true&bSortable_3=false&bSortable_4=false&bSortable_5=false&bSortable_6=true&additional=%7B%22fType%22%3A%22$PAGE%22%2C%22fLetter%22%3A%22$PAGE2%22%7D" > $TMP/cache.$FILENAME.1
			TMPPFAD=List
			FTYPE="%22fType%22%3A%22$PAGE%22"	
			FLETTER="%22fLetter%22%3A%22$PAGE2%22"
			additional="%7B$FTYPE%2C$FLETTER%7D"
		elif [ "$PAGE" == "actor" ];then
#			$ACTIVEBIN "$URL/aGET/Actor/?sEcho=1&iColumns=10&sColumns=&iDisplayStart=$NEXT&iDisplayLength=50&iSortingCols=1&iSortCol_0=5&sSortDir_0=asc&bSortable_0=true&bSortable_1=true&bSortable_2=true&bSortable_3=false&bSortable_4=false&bSortable_5=false&bSortable_6=true&additional=%7B%22fLetter%22%3A%22$PAGE2%22%2C%22fSex%22%3A%22%22%7D" > $TMP/cache.$FILENAME.1
			TMPPFAD=Actor
			FLETTER="%22fLetter%22%3A%22$PAGE2%22"
			FSEX="%22fSex%22%3A%22%22"
			additional="%7B$FLETTER%2C$FSEX%7D"
		elif [ "$PAGE" == "director" ];then
#			$ACTIVEBIN "$URL/aGET/Director/?sEcho=1&iColumns=10&sColumns=&iDisplayStart=$NEXT&iDisplayLength=50&iSortingCols=1&iSortCol_0=5&sSortDir_0=asc&bSortable_0=true&bSortable_1=true&bSortable_2=true&bSortable_3=false&bSortable_4=false&bSortable_5=false&bSortable_6=true&additional=%7B%22fLetter%22%3A%22$PAGE2%22%2C%22fSex%22%3A%22%22%7D" > $TMP/cache.$FILENAME.1
			TMPPFAD=Director
			FLETTER="%22fLetter%22%3A%22$PAGE2%22"
			FSEX="%22fSex%22%3A%22%22"
			additional="%7B$FLETTER%2C$FSEX%7D"
		fi

		$ACTIVEBIN "$URL/aGET/$TMPPFAD/?sEcho=1&iColumns=10&sColumns=&iDisplayStart=$NEXT&iDisplayLength=50&iSortingCols=1&iSortCol_0=5&sSortDir_0=asc&bSortable_0=true&bSortable_1=true&bSortable_2=true&bSortable_3=false&bSortable_4=false&bSortable_5=false&bSortable_6=true&additional=$additional" > $TMP/cache.$FILENAME.1

#https://www.kinos.to/aGET/Actor/?sEcho=1&iColumns=2&sColumns=&iDisplayStart=0&iDisplayLength=25&iSortingCols=1&iSortCol_0=1&sSortDir_0=asc&bSortable_0=true&bSortable_1=false&additional=%7B%22fLetter%22%3A%22B%22%2C%22fSex%22%3A%22%22%7D
#https://www.kinos.to/aGET/Actor/?sEcho=1&iColumns=2&sColumns=&iDisplayStart=0&iDisplayLength=25&iSortingCols=1&iSortCol_0=1&sSortDir_0=asc&bSortable_0=true&bSortable_1=false&additional=%7B%22fLetter%22%3A%22A%22%2C%22fSex%22%3A%22%22%7D
#https://www.kinos.to/aGET/Director/?sEcho=1&iColumns=2&sColumns=&iDisplayStart=0&iDisplayLength=25&iSortingCols=1&iSortCol_0=1&sSortDir_0=asc&bSortable_0=true&bSortable_1=false&additional=%7B%22fLetter%22%3A%22A%22%2C%22fSex%22%3A%22%22%7D
#https://www.kinos.to/aGET/List/?sEcho=2&iColumns=7&sColumns=&iDisplayStart=0&iDisplayLength=25&iSortingCols=1&iSortCol_0=2&sSortDir_0=asc&bSortable_0=true&bSortable_1=true&bSortable_2=true&bSortable_3=false&bSortable_4=false&bSortable_5=false&bSortable_6=true&additional=%7B%22Length%22%3A60%2C%22fLetter%22%3A1%2C%22fGenre%22%3A%226%22%7D
#https://www.kinos.to/aGET/List/?sEcho=2&iColumns=7&sColumns=&iDisplayStart=0&iDisplayLength=25&iSortingCols=1&iSortCol_0=2&sSortDir_0=asc&bSortable_0=true&bSortable_1=true&bSortable_2=true&bSortable_3=false&bSortable_4=false&bSortable_5=false&bSortable_6=true&additional=%7B%22fType%22%3A%22movie%22%2C%22Length%22%3A60%2C%22fLetter%22%3A1%7D
#https://www.kinos.to/aGET/List/?sEcho=2&iColumns=7&sColumns=&iDisplayStart=0&iDisplayLength=25&iSortingCols=1&iSortCol_0=2&sSortDir_0=asc&bSortable_0=true&bSortable_1=true&bSortable_2=true&bSortable_3=false&bSortable_4=false&bSortable_5=false&bSortable_6=true&additional=%7B%22fType%22%3A%22documentation%22%2C%22Length%22%3A60%2C%22fLetter%22%3A1%7D
#https://www.kinos.to/aGET/List/?sEcho=2&iColumns=7&sColumns=&iDisplayStart=0&iDisplayLength=25&iSortingCols=1&iSortCol_0=2&sSortDir_0=asc&bSortable_0=true&bSortable_1=true&bSortable_2=true&bSortable_3=false&bSortable_4=false&bSortable_5=false&bSortable_6=true&additional=%7B%22fType%22%3A%22series%22%2C%22Length%22%3A60%2C%22fLetter%22%3A1%7D
#https://www.kinos.to/aGET/List/?sEcho=3&iColumns=7&sColumns=&iDisplayStart=0&iDisplayLength=25&iSortingCols=1&iSortCol_0=2&sSortDir_0=asc&bSortable_0=true&bSortable_1=true&bSortable_2=true&bSortable_3=false&bSortable_4=false&bSortable_5=false&bSortable_6=true&additional=%7B%22fType%22%3A%22series%22%2C%22Length%22%3A60%2C%22fLetter%22%3A%22B%22%7D

		if [ `cat $TMP/cache.$FILENAME.1 | grep "KinoX 404 Not Found" | wc -l` -eq 1 ];then
			ERRORMSG="KinoX 404 Not Found"
#			echo "errormsg=$ERRORMSG"
			LINE="$ERRORMSG#$NEWPAGE#$PIC#kinox_$piccount.jpg#KinoX#22"
			echo "$LINE" > $TMP/$FILENAME.list
			exit
		fi

		if [ `cat $TMP/cache.$FILENAME.1 | grep "KinoX 503 Service Temporarily Unavailable" | wc -l` -eq 1 ];then
			ERRORMSG="KinoX 503 Service Temporarily Unavailable"
#			echo "errormsg=$ERRORMSG"
			LINE="$ERRORMSG#$NEWPAGE#$PIC#kinox_$piccount.jpg#KinoX#22"
			echo "$LINE" > $TMP/$FILENAME.list
			exit
		fi

		pages=`cat $TMP/cache.$FILENAME.1 | tr ',' '\n' | grep iTotalDisplayRecords | cut -d '"' -f4`
		if [ -z "$pages" ];then
			pages=0
		fi
		cat $TMP/cache.$FILENAME.1 | tr '\n' ' ' | sed 's!<TR>!\nfound=!g'| grep ^"found="  >$TMP/cache.$FILENAME.2

		if [ "$pages" == " " ] || [ -z "$pages" ] || [ "$pages" == 0 ]; then
			rm $TMP/cache.$FILENAME.1
			break
			ls $TMP/cache.$FILENAME.1
			exit
		fi
	
#		if [ $pages -gt 1000 ]; then
#			pages=1000
#		fi
	
		max=`expr $pages - 4`
#		echo max $max
		
		cat $TMP/cache.$FILENAME.1 | tr '],[' '\n' | grep 'Stream' | cut -d '/' -f3 | cut -d"\\" -f1 > $TMP/cache.$FILENAME.3

		if [ "$PAGE" == "actor" ] || [ "$PAGE" == "director" ];then
			cat $TMP/cache.$FILENAME.1 | sed "s!\[!\n\[!g" | grep '"nm' > $TMP/cache.$FILENAME.3
		fi

		count=0

		while read -u 3 ROUND; do
			piccount=`expr $piccount + 1`
			filename=`echo $ROUND | sed 's!/Stream/!!'`
			picname=`echo $filename | sed 's!.html!.jpg!'`
			searchname=`echo $filename | sed 's!.html!!'`	
		
			TITLE=`echo $picname | sed 's!.jpg!!' | tr "_" " " | cut -d'"' -f2`

			if [ `cat /mnt/config/titan.cfg | grep tithek_kinox_pic=1 | wc -l` -eq 1 ];then
		#		$wgetbin --no-check-certificate "$MAINURL/$ROUND1" -O cache."$filename".list
#				echo $BIN /tmp/localhoster/cloudflare.py "$URL/Stream/$ROUND"
				$ACTIVEBIN "$URL/Stream/$ROUND" > $TMP/cache."$filename".list
				if [ $(cat $TMP/cache."$filename".list | wc -l) -eq 0 ];then
#					echo $BIN2 /tmp/localhoster/cloudflare.py "$URL/Stream/$ROUND"
					$ACTIVEBIN "$URL/Stream/$ROUND" > $TMP/cache."$filename".list
				fi
				if [ $(cat $TMP/cache."$filename".list | wc -l) -eq 0 ];then
#					echo $BIN3 /tmp/localhoster/cloudflare.py "$URL/Stream/$ROUND"
					$ACTIVEBIN "$URL/Stream/$ROUND" > $TMP/cache."$filename".list
				fi
				if [ $(cat $TMP/cache."$filename".list | wc -l) -eq 0 ];then
#					echo $BIN4 /tmp/localhoster/cloudflare.py "$URL/Stream/$ROUND"
					$ACTIVEBIN "$URL/Stream/$ROUND" > $TMP/cache."$filename".list
				fi
				if [ $(cat $TMP/cache."$filename".list | wc -l) -eq 0 ];then
#					echo $BIN5 /tmp/localhoster/cloudflare.py "$URL/Stream/$ROUND"
					$ACTIVEBIN "$URL/Stream/$ROUND" > $TMP/cache."$filename".list
				fi
				if [ $(cat $TMP/cache."$filename".list | wc -l) -eq 0 ];then
#					echo $BIN6 /tmp/localhoster/cloudflare.py "$URL/Stream/$ROUND"
					$ACTIVEBIN "$URL/Stream/$ROUND" > $TMP/cache."$filename".list
				fi
				PIC=$URL/`cat $TMP/cache."$filename".list | tr '><' '>\n<' | grep $picname | cut -d '"' -f2 | sort -um`
				LANG=`cat $TMP/cache."$filename".list | grep 'alt="language" src="/gr/sys/lng' | sed 's!alt="language" src="/gr/sys/lng/!\n!' | tail -n1 |cut -d"." -f1`
			else
				PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
				LANG=1
			fi

		 	LANGTXT=" (de)"

			if [ ! -z $LANG ];then
				if [ $LANG = 1 ];then
				 	LANGTXT=" (de)"
				elif [ $LANG = 2 ];then
				 	LANGTXT=" (en)"
				else
				 	LANGTXT=" (??)"
				fi
			fi

			NEWPAGE="$URL/Stream/$ROUND"
			if [ "$PAGE" == "actor" ] || [ "$PAGE" == "director" ];then
				TMPPFAD=`echo $ROUND | sed -nr 's/.*","([^"]+)".*/\1/p'`
				NEWPAGE="/People/$TMPPFAD"
			fi
			if [ -z "$PIC" ]; then
				PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
			fi

			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/\C3x/g' -e 's/&ndash;/-/g' -e 's/&Auml;/\C3/g' -e 's/&Uuml;/\C3S/g' -e 's/&Ouml;/\C3/g' -e 's/&auml;/\E4/g' -e 's/&uuml;/\FC/g' -e 's/&ouml;/\F6/g' -e 's/&eacute;/\E9/g' -e 's/&egrave;/\E8/g' -e 's/%F6/\F6/g' -e 's/%FC/\FC/g' -e 's/%E4/\E4/g' -e 's/%26/&/g' -e 's/%C4/\C3/g' -e 's/%D6/\C3/g' -e 's/%DC/\C3S/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`

			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
				if [ ! -e $TMP/$FILENAME.list ];then
					touch $TMP/$FILENAME.list
				fi
				piccount=`expr $piccount + 1`
				LINE="$TITLE$LANGTXT#$NEWPAGE#$PIC#kinox_$piccount.jpg#KinoX#22"
				if [ "$PAGE" == "actor" ] || [ "$PAGE" == "director" ];then
#					LINE="$TITLE#$SRC $SRC search 'movie' 1 '$NEWPAGE'#$PIC#kinox_$piccount.jpg#KinoX#0"
					LINE="$TITLE#$NEWPAGE#$PIC#kinox_$piccount.jpg#KinoX#32"

				fi
				echo "$LINE" >> $TMP/$FILENAME.list
			fi

		done 3<$TMP/cache.$FILENAME.3

		if [ "$NEXT" -lt "$pages" ]; then
			NEXTPAGE=`expr $NEXT + 30`
			LINE="Page ($NEXTPAGE/$pages)#$SRC $SRC search '$PAGE' $NEXTPAGE $PAGE2#http://atemio.dyndns.tv/mediathek/menu/next.jpg#next.jpg#$NAME#0"
			echo "$LINE" >> $TMP/$FILENAME.list
		fi

#		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi
	echo "$TMP/$FILENAME.list"
}

kino()
{
#	rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
#	rm $TMP/$FILENAME.list
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		piccount=0
#		$curlbin "$URL/$PAGE" -o "$TMP/cache.$FILENAME.1"
		$ACTIVEBIN "$URL/$PAGE" > $TMP/cache.$FILENAME.1

		if [ `cat $TMP/cache.$FILENAME.1 | grep "KinoX 404 Not Found" | wc -l` -eq 1 ];then
			ERRORMSG="KinoX 404 Not Found"
#			echo "errormsg=$ERRORMSG"
			LINE="$ERRORMSG#$NEWPAGE#$PIC#kinox_$piccount.jpg#KinoX#22"
			echo "$LINE" > $TMP/$FILENAME.list
			exit
		fi
		if [ `cat $TMP/cache.$FILENAME.1 | grep "KinoX 503 Service Temporarily Unavailable" | wc -l` -eq 1 ];then
			ERRORMSG="KinoX 503 Service Temporarily Unavailable"
#			echo "errormsg=$ERRORMSG"
			LINE="$ERRORMSG#$NEWPAGE#$PIC#kinox_$piccount.jpg#KinoX#22"
			echo "$LINE" > $TMP/$FILENAME.list
			exit
		fi

#		cat $TMP/cache.$FILENAME.1 | grep /Stream/ | sed 's!/Stream/!\n/Stream/!' | grep ^/Stream/ | cut -d '"' -f1  | cut -d "'" -f1 | sort -um >$TMP/cache.$FILENAME.2
		cat $TMP/cache.$FILENAME.1 | grep /Stream/ | sed 's!/Stream/!\n/Stream/!' | grep ^/Stream/ | cut -d '"' -f1  | cut -d "'" -f1 >$TMP/cache.$FILENAME.2

		while read -u 3 ROUND; do
			piccount=`expr $piccount + 1`
			filename=`echo $ROUND | sed 's!/Stream/!!'`
			picname=`echo $filename | sed 's!.html!.jpg!'`
			searchname=`echo $filename | sed 's!.html!!'`	
		
			TITLE=`echo $picname | sed 's!.jpg!!' | tr "_" " "`

			if [ `cat /mnt/config/titan.cfg | grep tithek_kinox_pic=1 | wc -l` -eq 1 ];then
		#		$wgetbin --no-check-certificate "$MAINURL/$ROUND1" -O cache."$filename".list
#				echo $BIN /tmp/localhoster/cloudflare.py "$URL/$ROUND"
				$ACTIVEBIN "$URL/$ROUND" > $TMP/cache."$filename".list
				if [ $(cat $TMP/cache."$filename".list | wc -l) -eq 0 ];then
#					echo $BIN2 /tmp/localhoster/cloudflare.py "$URL/$ROUND"
					$ACTIVEBIN "$URL/$ROUND" > $TMP/cache."$filename".list
				fi
				if [ $(cat $TMP/cache."$filename".list | wc -l) -eq 0 ];then
#					echo $BIN3 /tmp/localhoster/cloudflare.py "$URL/$ROUND"
					$ACTIVEBIN "$URL/$ROUND" > $TMP/cache."$filename".list
				fi
				if [ $(cat $TMP/cache."$filename".list | wc -l) -eq 0 ];then
#					echo $BIN4 /tmp/localhoster/cloudflare.py "$URL/$ROUND"
					$ACTIVEBIN "$URL/$ROUND" > $TMP/cache."$filename".list
				fi
				if [ $(cat $TMP/cache."$filename".list | wc -l) -eq 0 ];then
#					echo $BIN5 /tmp/localhoster/cloudflare.py "$URL/$ROUND"
					$ACTIVEBIN "$URL/$ROUND" > $TMP/cache."$filename".list
				fi
				if [ $(cat $TMP/cache."$filename".list | wc -l) -eq 0 ];then
#					echo $BIN6 /tmp/localhoster/cloudflare.py "$URL/$ROUND"
					$ACTIVEBIN "$URL/$ROUND" > $TMP/cache."$filename".list
				fi
				PIC=$URL/`cat $TMP/cache."$filename".list | tr '><' '>\n<' | grep $picname | cut -d '"' -f2 | sort -um`
				LANG=`cat $TMP/cache."$filename".list | grep 'alt="language" src="/gr/sys/lng' | sed 's!alt="language" src="/gr/sys/lng/!\n!' | tail -n1 |cut -d"." -f1`
			else
				PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
				LANG=1
			fi

		 	LANGTXT=" (de)"

			if [ ! -z $LANG ];then
				if [ $LANG = 1 ];then
				 	LANGTXT=" (de)"
				elif [ $LANG = 2 ];then
				 	LANGTXT=" (en)"
				else
				 	LANGTXT=" (??)"
				fi
			fi

			NEWPAGE="$URL/$ROUND"

			if [ -z "$PIC" ]; then
				PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
			fi

			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/\C3x/g' -e 's/&ndash;/-/g' -e 's/&Auml;/\C3/g' -e 's/&Uuml;/\C3S/g' -e 's/&Ouml;/\C3/g' -e 's/&auml;/\E4/g' -e 's/&uuml;/\FC/g' -e 's/&ouml;/\F6/g' -e 's/&eacute;/\E9/g' -e 's/&egrave;/\E8/g' -e 's/%F6/\F6/g' -e 's/%FC/\FC/g' -e 's/%E4/\E4/g' -e 's/%26/&/g' -e 's/%C4/\C3/g' -e 's/%D6/\C3/g' -e 's/%DC/\C3S/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`

			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
				if [ ! -e $TMP/$FILENAME.list ];then
					touch $TMP/$FILENAME.list
				fi
				piccount=`expr $piccount + 1`
				LINE="$TITLE$LANGTXT#$NEWPAGE#$PIC#kinox_$piccount.jpg#KinoX#22"

				if [ `cat $TMP/$FILENAME.list | grep "$TITLE" | wc -l` -eq 0 ];then
					echo "$LINE" >> $TMP/$FILENAME.list
				fi
			fi

		done 3<$TMP/cache.$FILENAME.2
		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1

	fi
	echo "$TMP/$FILENAME.list"
}

latest()
{
#	rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	rm $TMP/$FILENAME.list
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		piccount=0
#		$curlbin "$URL/$PAGE" -o "$TMP/cache.$FILENAME.1"
		$ACTIVEBIN "$URL/$PAGE" > $TMP/cache.$FILENAME.1

		if [ `cat $TMP/cache.$FILENAME.1 | grep "KinoX 404 Not Found" | wc -l` -eq 1 ];then
			ERRORMSG="KinoX 404 Not Found"
#			echo "errormsg=$ERRORMSG"
			LINE="$ERRORMSG#$NEWPAGE#$PIC#kinox_$piccount.jpg#KinoX#22"
			echo "$LINE" > $TMP/$FILENAME.list
			exit
		fi

		if [ `cat $TMP/cache.$FILENAME.1 | grep "KinoX 503 Service Temporarily Unavailable" | wc -l` -eq 1 ];then
			ERRORMSG="KinoX 503 Service Temporarily Unavailable"
#			echo "errormsg=$ERRORMSG"
			LINE="$ERRORMSG#$NEWPAGE#$PIC#kinox_$piccount.jpg#KinoX#22"
			echo "$LINE" > $TMP/$FILENAME.list
			exit
		fi

		cat $TMP/cache.$FILENAME.1 | tr '\n' ' ' | sed 's/<tr>/\n<tr>/g' | grep ^"<tr>" | grep "/Stream/" >$TMP/cache.$FILENAME.2

		while read -u 3 ROUND; do
			piccount=`expr $piccount + 1`
			filename=`echo $ROUND | sed -nr 's/.*<a href="\/Stream\/([^\/]+)" .*/\1/p'`
			picname=`echo $filename | sed 's!.html!.jpg!'`
			searchname=`echo $filename | sed 's!.html!!'`	

			PAGE=`echo $ROUND | sed -nr 's/.*<a href="([^"]+)" .*/\1/p'`
			TITLE=`echo $picname | sed 's!.jpg!!' | tr "_" " "`
			LANG=`echo $ROUND | grep 'src="/gr/sys/lng' | sed 's!src="/gr/sys/lng/!\n!' | tail -n1 |cut -d"." -f1`
			IMDB=`echo $ROUND | sed -nr 's/.*"Rating">([^>]+)<\/td>.*/\1/p'`
			YEAR=`echo $ROUND | sed -nr 's/.*"Year">([^>]+)<\/span>.*/\1/p'`

			if [ `cat /mnt/config/titan.cfg | grep tithek_kinox_pic=1 | wc -l` -eq 1 ];then
		#		$wgetbin --no-check-certificate "$MAINURL/$PAGE" -O cache."$filename".list
#				echo $BIN /tmp/localhoster/cloudflare.py "$URL/$PAGE"
				$ACTIVEBIN "$URL/$PAGE" > $TMP/cache."$filename".list
				if [ $(cat $TMP/cache."$filename".list | wc -l) -eq 0 ];then
#					echo $BIN2 /tmp/localhoster/cloudflare.py "$URL/$PAGE"
					$ACTIVEBIN "$URL/$PAGE" > $TMP/cache."$filename".list
				fi
				if [ $(cat $TMP/cache."$filename".list | wc -l) -eq 0 ];then
#					echo $BIN3 /tmp/localhoster/cloudflare.py "$URL/$PAGE"
					$ACTIVEBIN "$URL/$PAGE" > $TMP/cache."$filename".list
				fi
				if [ $(cat $TMP/cache."$filename".list | wc -l) -eq 0 ];then
#					echo $BIN4 /tmp/localhoster/cloudflare.py "$URL/$PAGE"
					$ACTIVEBIN "$URL/$PAGE" > $TMP/cache."$filename".list
				fi
				if [ $(cat $TMP/cache."$filename".list | wc -l) -eq 0 ];then
#					echo $BIN5 /tmp/localhoster/cloudflare.py "$URL/$ROUND"
					$ACTIVEBIN "$URL/$filepath" > $TMP/cache."$filename".list
				fi
				if [ $(cat $TMP/cache."$filename".list | wc -l) -eq 0 ];then
#					echo $BIN6 /tmp/localhoster/cloudflare.py "$URL/$PAGE"
					$ACTIVEBIN "$URL/$PAGE" > $TMP/cache."$filename".list
				fi
				PIC=$URL/`cat $TMP/cache."$filename".list | tr '><' '>\n<' | grep $picname | cut -d '"' -f2 | sort -um`
				LANG=`cat $TMP/cache."$filename".list | grep 'alt="language" src="/gr/sys/lng' | sed 's!alt="language" src="/gr/sys/lng/!\n!' | tail -n1 |cut -d"." -f1`
			else
				PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
#				LANG=1
			fi

		 	LANGTXT=" (de)"

			if [ ! -z $LANG ];then
				if [ $LANG = "1" ];then
				 	LANGTXT=" (de)"
				elif [ $LANG = "2" ];then
				 	LANGTXT=" (en)"
				else
				 	LANGTXT=" (??)"
				fi
			fi

			if [ ! -z "$IMDB" ];then
				IMDBTXT=" ($IMDB)"
			fi
			if [ ! -z "$YEAR" ];then
				YEARTXT=" ($YEAR)"
			fi

			NEWPAGE="$URL$PAGE"

			if [ -z "$PIC" ]; then
				PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
			fi

			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/\C3x/g' -e 's/&ndash;/-/g' -e 's/&Auml;/\C3/g' -e 's/&Uuml;/\C3S/g' -e 's/&Ouml;/\C3/g' -e 's/&auml;/\E4/g' -e 's/&uuml;/\FC/g' -e 's/&ouml;/\F6/g' -e 's/&eacute;/\E9/g' -e 's/&egrave;/\E8/g' -e 's/%F6/\F6/g' -e 's/%FC/\FC/g' -e 's/%E4/\E4/g' -e 's/%26/&/g' -e 's/%C4/\C3/g' -e 's/%D6/\C3/g' -e 's/%DC/\C3S/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`

			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
				if [ ! -e $TMP/$FILENAME.list ];then
					touch $TMP/$FILENAME.list
				fi
				piccount=`expr $piccount + 1`
				LINE="$TITLE$LANGTXT$YEARTXT$IMDBTXT#$NEWPAGE#$PIC#kinox_$piccount.jpg#KinoX#22aaaa"

				if [ `cat $TMP/$FILENAME.list | grep "$TITLE" | wc -l` -eq 0 ];then
					echo "$LINE" >> $TMP/$FILENAME.list
				fi
			fi
		done 3<$TMP/cache.$FILENAME.2
		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1

	fi
	echo "$TMP/$FILENAME.list"
}


case $INPUT in
	init) $INPUT;;
	mainmenu) $INPUT;;
	search) $INPUT;;
	kino) $INPUT;;
	latest) $INPUT;;
	sorted) $INPUT;;
esac
