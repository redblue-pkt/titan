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
	echo "All (A-Z)#$SRC $SRC sorted#http://atemio.dyndns.tv/mediathek/menu/a-z.jpg#a-z.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Search#http://atemio.dyndns.tv/mediathek/youtube/streams/kinox.all-sorted.list#http://atemio.dyndns.tv/mediathek/menu/search.jpg#search.jpg#KinoX#21" >>$TMP/$FILENAME.list
	echo "Search Cast#http://atemio.dyndns.tv/mediathek/youtube/streams/kinox.all-sorted.list#http://atemio.dyndns.tv/mediathek/menu/search.jpg#search10.jpg#KinoX#31" >>$TMP/$FILENAME.list
	echo "$TMP/$FILENAME.list"
}

sorted()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		watchlist="A B C D E F G H I J K L M N O P Q R S T U V W X Y Z"

		rm $TMP/$FILENAME.list > /dev/null 2>&1

		for ROUND0 in $watchlist; do
			TITLE=`echo $ROUND0`
			filename=`echo $TITLE | tr [A-Z] [a-z]`
			echo "$TITLE#$SRC $SRC search '$ROUND0' 1#http://atemio.dyndns.tv/mediathek/menu/$filename.jpg#$filename.jpg#$NAME#0" >> $TMP/$FILENAME.list
		done
	fi
  	echo "$TMP/$FILENAME.list"
}

search()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		piccount=0

		$BIN /tmp/localhoster/cloudflare.py "$URL/aGET/List/?sEcho=1&iColumns=10&sColumns=&iDisplayStart=$NEXT&iDisplayLength=50&iSortingCols=1&iSortCol_0=5&sSortDir_0=asc&bSortable_0=true&bSortable_1=true&bSortable_2=true&bSortable_3=false&bSortable_4=false&bSortable_5=false&bSortable_6=true&additional=%7B%22fType%22%3A%22movie%22%2C%22fLetter%22%3A%22$PAGE%22%7D" > $TMP/cache.$FILENAME.1

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

		count=0

		while read -u 3 ROUND; do
			piccount=`expr $piccount + 1`
			filename=`echo $ROUND | sed 's!/Stream/!!'`
			picname=`echo $filename | sed 's!.html!.jpg!'`
			searchname=`echo $filename | sed 's!.html!!'`	
		
			TITLE=`echo $picname | sed 's!.jpg!!' | tr "_" " "`

			if [ `cat /mnt/config/titan.cfg | grep tithek_kinox_pic=1 | wc -l` -eq 1 ];then
		#		$wgetbin --no-check-certificate "$MAINURL/$ROUND1" -O cache."$filename".list
#				echo $BIN /tmp/localhoster/cloudflare.py "$URL/Stream/$ROUND"
				$BIN /tmp/localhoster/cloudflare.py "$URL/Stream/$ROUND" > $TMP/cache."$filename".list
				if [ $(cat $TMP/cache."$filename".list | wc -l) -eq 0 ];then
#					echo $BIN2 /tmp/localhoster/cloudflare.py "$URL/Stream/$ROUND"
					$BIN /tmp/localhoster/cloudflare.py "$URL/Stream/$ROUND" > $TMP/cache."$filename".list
				fi
				if [ $(cat $TMP/cache."$filename".list | wc -l) -eq 0 ];then
#					echo $BIN3 /tmp/localhoster/cloudflare.py "$URL/Stream/$ROUND"
					$BIN /tmp/localhoster/cloudflare.py "$URL/Stream/$ROUND" > $TMP/cache."$filename".list
				fi
				if [ $(cat $TMP/cache."$filename".list | wc -l) -eq 0 ];then
#					echo $BIN4 /tmp/localhoster/cloudflare.py "$URL/Stream/$ROUND"
					$BIN /tmp/localhoster/cloudflare.py "$URL/Stream/$ROUND" > $TMP/cache."$filename".list
				fi
				if [ $(cat $TMP/cache."$filename".list | wc -l) -eq 0 ];then
#					echo $BIN5 /tmp/localhoster/cloudflare.py "$URL/Stream/$ROUND"
					$BIN /tmp/localhoster/cloudflare.py "$URL/Stream/$ROUND" > $TMP/cache."$filename".list
				fi
				if [ $(cat $TMP/cache."$filename".list | wc -l) -eq 0 ];then
#					echo $BIN6 /tmp/localhoster/cloudflare.py "$URL/Stream/$ROUND"
					$BIN /tmp/localhoster/cloudflare.py "$URL/Stream/$ROUND" > $TMP/cache."$filename".list
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

				echo "$LINE" >> $TMP/$FILENAME.list
			fi

		done 3<$TMP/cache.$FILENAME.3

		if [ "$NEXT" -lt "$pages" ]; then
			NEXTPAGE=`expr $NEXT + 30`
			LINE="Page ($NEXTPAGE/$pages)#$SRC $SRC search '$PAGE' $NEXTPAGE#http://atemio.dyndns.tv/mediathek/menu/next.jpg#next.jpg#$NAME#0"
			echo "$LINE" >> $TMP/$FILENAME.list
		fi

		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi
	echo "$TMP/$FILENAME.list"
}

kino()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		piccount=0
#		$curlbin "$URL/$PAGE" -o "$TMP/cache.$FILENAME.1"
		$BIN /tmp/localhoster/cloudflare.py "$URL/$PAGE" > $TMP/cache.$FILENAME.1

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

		cat $TMP/cache.$FILENAME.1 | grep /Stream/ | sed 's!/Stream/!\n/Stream/!' | grep ^/Stream/ | cut -d '"' -f1  | cut -d "'" -f1 | sort -um >$TMP/cache.$FILENAME.2
		while read -u 3 ROUND; do
			piccount=`expr $piccount + 1`
			filename=`echo $ROUND | sed 's!/Stream/!!'`
			picname=`echo $filename | sed 's!.html!.jpg!'`
			searchname=`echo $filename | sed 's!.html!!'`	
		
			TITLE=`echo $picname | sed 's!.jpg!!' | tr "_" " "`

			if [ `cat /mnt/config/titan.cfg | grep tithek_kinox_pic=1 | wc -l` -eq 1 ];then
		#		$wgetbin --no-check-certificate "$MAINURL/$ROUND1" -O cache."$filename".list
#				echo $BIN /tmp/localhoster/cloudflare.py "$URL/$ROUND"
				$BIN /tmp/localhoster/cloudflare.py "$URL/$ROUND" > $TMP/cache."$filename".list
				if [ $(cat $TMP/cache."$filename".list | wc -l) -eq 0 ];then
					echo $BIN2 /tmp/localhoster/cloudflare.py "$URL/$ROUND"
					$BIN /tmp/localhoster/cloudflare.py "$URL/$ROUND" > $TMP/cache."$filename".list
				fi
				if [ $(cat $TMP/cache."$filename".list | wc -l) -eq 0 ];then
#					echo $BIN3 /tmp/localhoster/cloudflare.py "$URL/$ROUND"
					$BIN /tmp/localhoster/cloudflare.py "$URL/$ROUND" > $TMP/cache."$filename".list
				fi
				if [ $(cat $TMP/cache."$filename".list | wc -l) -eq 0 ];then
#					echo $BIN4 /tmp/localhoster/cloudflare.py "$URL/$ROUND"
					$BIN /tmp/localhoster/cloudflare.py "$URL/$ROUND" > $TMP/cache."$filename".list
				fi
				if [ $(cat $TMP/cache."$filename".list | wc -l) -eq 0 ];then
#					echo $BIN5 /tmp/localhoster/cloudflare.py "$URL/$ROUND"
					$BIN /tmp/localhoster/cloudflare.py "$URL/$ROUND" > $TMP/cache."$filename".list
				fi
				if [ $(cat $TMP/cache."$filename".list | wc -l) -eq 0 ];then
#					echo $BIN6 /tmp/localhoster/cloudflare.py "$URL/$ROUND"
					$BIN /tmp/localhoster/cloudflare.py "$URL/$ROUND" > $TMP/cache."$filename".list
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

				echo "$LINE" >> $TMP/$FILENAME.list
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
	sorted) $INPUT;;
esac
