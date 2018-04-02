#!/bin/sh
# first sh box parser for titannit mfg obi

. /tmp/localhoster/hoster.sh

SRC=$1
INPUT=$2
CURPAGE=$3
MAXPAGE=$4
PAGE=$5

FILENAME=`echo $PAGE | tr '/' '.'`
FILENAME=`echo $FILENAME | tr '&' '.'`

if [ -z "$PAGE" ]; then
	FILENAME=none
fi

URL=http://foxx.to
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME="Foxx HD"

mkdir $TMP > /dev/null 2>&1
#TMP=/tmp/localcache
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
	echo "Kinofilme#$SRC $SRC new 0 0 '?get=movies'#http://atemio.dyndns.tv/mediathek/menu/all-newfirst.jpg#all-newfirst.jpg#$NAME#0" > $TMP/$PARSER.$INPUT.list
	#echo "Serien#$SRC $SRC new 0 0 '?get=tv'#http://atemio.dyndns.tv/mediathek/menu/Movies.jpg#Movies.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
        echo "Search#$SRC $SRC search 1 0 '?s='#http://atemio.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >> $TMP/$PARSER.$INPUT.list

	if [ -e "$TMP/$PARSER.new.list" ] ; then
		rm $TMP/$PARSER.new.list
	fi
	if [ "`echo $TMP/$PARSER.search.*.list`" != "$TMP/$PARSER.search.*.list" ] ; then
		rm $TMP/$PARSER.search.*.list
	fi
	if [ -e "$TMP/$PARSER.page.list" ] ; then
		rm $TMP/$PARSER.page.list
	fi

	echo "$TMP/$PARSER.$INPUT.list"
}

new()
{
	if [ ! -e "$TMP/$PARSER.$INPUT.list" ] ; then
		$curlbin $URL/$PAGE -o $TMP/cache.$PARSER.$INPUT.1

#		/tmp/localhoster/hoster.sh get $URL/$PAGE > $TMP/cache.$PARSER.$INPUT.1

		cat $TMP/cache.$PARSER.$INPUT.1 | sed 's/<div class/\n<div class/g' | sed 's/<a href="/\n<a href="/g' | grep ^'<a href="' | grep alt= | grep -v .gif > /$TMP/cache.$PARSER.$INPUT.2
		while read -u 3 ROUND; do
			TITLE=`echo $ROUND | sed 's/alt=/\nalt=/' | grep ^"alt=" | cut -d '"' -f2 | sed 's/#/%/'`
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/&%8211;/-/g' -e "s/&%8217;/'/g"`
			PIC=`echo $ROUND | sed 's!data-original=!\nsrc=!' | grep ^"src=" | cut -d '"' -f2`
			NEWPAGE=`echo $ROUND | sed 's/<a href=/\nhref=/' | grep ^"href=" | cut -d '"' -f2`

			if [ -z  "$PIC" ]; then  
				PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
				TMPPIC="default.jpg"
			else
				TMPPIC=goldesel_`echo $PIC | tr '/' '\n' | tail -n1`
			fi

#			echo "ROUND " $ROUND
#			echo "Bildlink " $PIC
#			echo "TMPPIC " $TMPPIC
#			echo "TITLE " $TITLE
#			echo "NEWPAGE " $NEWPAGE
#
#
#exit
			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
				if [ `cat $TMP/$PARSER.$INPUT.list | grep ^"$NEWPAGE" | wc -l` -eq 0 ];then
					if [ ! -e $TMP/$PARSER.$INPUT.list ];then
						touch $TMP/$PARSER.$INPUT.list
					fi
# obi
					LINE="$TITLE#$SRC $SRC hosterlist 0 0 $NEWPAGE#$PIC#$TMPPIC#$NAME#0"
#					LINE="$TITLE#$SRC $SRC play $NEWPAGE#$PIC#$TMPPIC#$NAME#111"
					echo "$LINE" >> $TMP/$PARSER.$INPUT.list
				fi
			fi
		done 3<$TMP/cache.$PARSER.$INPUT.2
		rm $TMP/cache.$PARSER.$INPUT.* > /dev/null 2>&1
	fi

	echo "$TMP/$PARSER.$INPUT.list"
}

search()
{
	if [ ! -e "$TMP/$PARSER.$INPUT.$CURPAGE.list" ] ; then
		if [ "$CURPAGE" -eq "1" ] ; then
			NEWPAGE=$PAGE
		else
			NEWPAGE=`echo $PAGE | sed "s/@PAGE@/$CURPAGE/g"`
		fi
		$curlbin $URL/$NEWPAGE -o $TMP/cache.$PARSER.$INPUT.1

#		/tmp/localhoster/hoster.sh get $URL/$PAGE > $TMP/cache.$PARSER.$INPUT.1

		cat $TMP/cache.$PARSER.$INPUT.1 | tr '\n' ' ' | sed -e 's/<a href=/\n<a href=/g' -e 's/Film/\nFilm/g' | grep '^<a href=' | grep '<img src=' > $TMP/cache.$PARSER.$INPUT.1a
		cat $TMP/cache.$PARSER.$INPUT.1a | sed '/Stream in HD/d' > $TMP/cache.$PARSER.$INPUT.2
		while read -u 3 ROUND; do
			TITLE=`echo $ROUND | sed 's/alt=/\nalt=/' | grep ^"alt=" | cut -d '"' -f2 | sed 's/#/%/'`
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/&%8211;/-/g' -e "s/&%8217;/'/g"`
			PIC=`echo $ROUND | sed 's/img src/\nsrc=/' | grep ^"src=" | cut -d '"' -f2`
			NEWPAGE=`echo $ROUND | sed 's/<a href=/\nhref=/' | grep ^"href=" | cut -d '"' -f2`

			if [ -z  "$PIC" ]; then  
				PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
				TMPPIC="default.jpg"
			else
				TMPPIC=foxx_`echo $PIC | tr '/' '\n' | tail -n1`
			fi

#			echo "ROUND " $ROUND
#			echo "Bildlink " $PIC
#			echo "TMPPIC " $TMPPIC
#			echo "TITLE " $TITLE
#			echo "NEWPAGE " $NEWPAGE
#
#
#exit
			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
				if [ `cat $TMP/$PARSER.$INPUT.list | grep ^"$NEWPAGE" | wc -l` -eq 0 ];then
					if [ ! -e $TMP/$PARSER.$INPUT.$CURPAGE.list ];then
						touch $TMP/$PARSER.$INPUT.$CURPAGE.list
					fi
# obi
					LINE="$TITLE#$SRC $SRC hosterlist 0 0 $NEWPAGE#$PIC#$TMPPIC#$NAME#0"
#					LINE="$TITLE#$SRC $SRC play 0 0 $NEWPAGE#$PIC#$TMPPIC#$NAME#111"
					echo "$LINE" >> $TMP/$PARSER.$INPUT.$CURPAGE.list
				fi
			fi
		done 3<$TMP/cache.$PARSER.$INPUT.2
		if [ "$CURPAGE" -eq "1" ] ; then
			PAGE="page/@PAGE@$PAGE"
			MAXPAGE=`cat $TMP/cache.$PARSER.$INPUT.1 | sed '/<div class="pagination/!d;s/^.*Seite 1 von //;s/<\/span>.*$//'`
		fi
		if [ "$CURPAGE" -lt "$MAXPAGE" ] ; then
			NEWPAGE=`expr $CURPAGE + 1`
			echo "Page ($NEWPAGE/$MAXPAGE)#$SRC $SRC search $NEWPAGE $MAXPAGE '$PAGE'#http://atemio.dyndns.tv/mediathek/menu/next.jpg#next.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.$CURPAGE.list
                fi

		rm $TMP/cache.$PARSER.$INPUT.* > /dev/null 2>&1
	fi
	if [ -e "$TMP/$PARSER.hosterlist.list" ] ; then
		rm $TMP/$PARSER.hosterlist.list
	fi

	echo "$TMP/$PARSER.$INPUT.$CURPAGE.list"
}

hosterlist()
{
	if [ -e "$TMP/$PARSER.$INPUT.list" ] ; then
		rm $TMP/$PARSER.$INPUT.list
	fi
	$curlbin $PAGE -o $TMP/cache.$PARSER.$INPUT.1

	cat $TMP/cache.$PARSER.$INPUT.1 | grep -E ^"<iframe src=" | sed -e 's/<iframe src\=//g' | cut -d '"' -f2 >$TMP/cache.$PARSER.$INPUT.2
	TEMP=$(cat $TMP/cache.$PARSER.$INPUT.2)
	$curlbin $TEMP -o $TMP/cache.$PARSER.$INPUT.3
	cat $TMP/cache.$PARSER.$INPUT.3 | grep -A 10 "var decodeABC" >$TMP/cache.$PARSER.$INPUT.4
	echo "print(decodeABC(jbdaskgs));" >> $TMP/cache.$PARSER.$INPUT.4
	$DUKBIN $TMP/cache.$PARSER.$INPUT.4 > $TMP/cache.$PARSER.$INPUT.5
	cat $TMP/cache.$PARSER.$INPUT.5 | grep -o "http[^ ]*"| sed -e 's!{! !g' -e 's/}//g' -e 's/{//g' -e 's/\"//g' -e 's/file/\nfile/g' -e 's/file://g' -e 's/type:mp4//g' -e 's/default:true//g' -e 's/\]//g' >$TMP/cache.$PARSER.$INPUT.6
	echo >> $TMP/cache.$PARSER.$INPUT.6
	while read -u 3 ROUND; do
		NEWPAGE=$(echo $ROUND | grep -o "http[^ ]*" | cut -d "," -f1)
		TITLE=$(echo $ROUND | grep -o "label[^ ]*" | cut -d "," -f1 | sed -e 's!label:!!g' -e 's!"!!g')
		if [ ! -z "$TITLE" ] && [ "$TITLE" != " " ] && [ ! -z "$NEWPAGE" ];then
			PIC=`echo $TITLE | tr [A-Z] [a-z]`
			LINE="Http Stream ($TITLE)#$NEWPAGE#http://atemio.dyndns.tv/mediathek/menu/foxx.jpg#foxx.jpg#$NAME#2"
			echo "$LINE" >> $TMP/$PARSER.$INPUT.list
		fi
	done 3<$TMP/cache.$PARSER.$INPUT.6
	rm $TMP/cache.$PARSER.$INPUT.* > /dev/null 2>&1

	echo $TMP/$PARSER.$INPUT.list
}

hoster()
{
	if [ ! -e "$TMP/$PARSER.$INPUT.list" ] ; then
#		$curlbin $URL/res/links -X POST --data "$PAGE" -o $TMP/cache.$PARSER.$INPUT.1 -A 'Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Maxthon/4.4.7.3000 Chrome/30.0.1599.101 Safari/537.36'
		$curlbin $URL/res/links -X POST --data "$PAGE" -o $TMP/cache.$PARSER.$INPUT.1
		echo "curl hoster " $curlbin $URL/res/links -X POST --data "$PAGE" -o $TMP/cache.$PARSER.$INPUT.1
		cat $TMP/cache.$PARSER.$INPUT.1 | sed 's!url="http://dref.pw/?!\nstreamurl="!' | grep ^streamurl= | cut -d'"' -f2 | sed 's!%3A!:!g' | sed 's!%2F!/!g' > $TMP/$PARSER.$INPUT.list
		rm $TMP/cache.$PARSER.$INPUT.* > /dev/null 2>&1
	fi
	cat $TMP/$PARSER.$INPUT.list
}

#obi
play()
{
	if [ -e "$TMP/$PARSER.$INPUT.list" ] ; then
		rm $TMP/$PARSER.$INPUT.list
	fi

	$curlbin $PAGE -o $TMP/cache.$PARSER.$INPUT.1

	cat $TMP/cache.$PARSER.$INPUT.1 | grep -E ^"<iframe src=" | sed -e 's/<iframe src\=//g' | cut -d '"' -f2 >$TMP/cache.$PARSER.$INPUT.2
	TEMP=$(cat $TMP/cache.$PARSER.$INPUT.2)
	$curlbin $TEMP -o $TMP/cache.$PARSER.$INPUT.3
	cat $TMP/cache.$PARSER.$INPUT.3 | grep -E "sources:*" >$TMP/cache.$PARSER.$INPUT.4
	cat $TMP/cache.$PARSER.$INPUT.4 | grep -o "http[^ ]*"| sed -e 's/,/ /g' -e 's/}//g' -e 's/{//g' -e 's/\"//g' -e 's/file/\nfile/g' -e 's/file://g' -e 's/type:mp4//g' -e 's/default:true//g' -e 's/\]//g' >$TMP/cache.$PARSER.$INPUT.5

	cat $TMP/cache.$PARSER.$INPUT.5 > $TMP/$PARSER.playlist.list

	echo "$TMP/$PARSER.playlist.list"
}

case $INPUT in
	init) $INPUT;;
	mainmenu) $INPUT;;
	new) $INPUT;;
	search) $INPUT;;
	page) $INPUT;;
	hosterlist) $INPUT;;
	hoster) $INPUT;;
	play) $INPUT;;
esac
