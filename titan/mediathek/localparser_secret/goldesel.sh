#!/bin/sh
# first sh box parser for titannit mfg obi

SRC=$1
INPUT=$2
PAGE=$3

URL=http://goldesel.to
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME=Goldesel

case $2 in
	init)	;;
	*) 	. /tmp/localhoster/hoster.sh
	   	mkdir $TMP > /dev/null 2>&1
		FILENAME="$PARSER $INPUT $PAGE"
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
	echo "Kinofilme#$SRC $SRC new '/'#http://openaaf.dyndns.tv/mediathek/menu/all-newfirst.jpg#all-newfirst.jpg#$NAME#0" > $TMP/$PARSER.$INPUT.list
	echo "Filme#$SRC $SRC page 'filme'#http://openaaf.dyndns.tv/mediathek/menu/Movies.jpg#Movies.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list

	if [ -e "$TMP/$PARSER.new.list" ] ; then
		rm $TMP/$PARSER.new.list
	fi
	if [ -e "$TMP/$PARSER.page.list" ] ; then
		rm $TMP/$PARSER.page.list
	fi

	echo "$TMP/$PARSER.$INPUT.list"
}

new()
{
	if [ ! -e "$TMP/$PARSER.$INPUT.list" ] ; then
#		$curlbin $URL/$PAGE -o $TMP/cache.$PARSER.$INPUT.1

		/tmp/localhoster/hoster.sh get $URL/$PAGE > $TMP/cache.$PARSER.$INPUT.1

		cat $TMP/cache.$PARSER.$INPUT.1 | sed 's/<div class/\n<div class/g' | sed 's/<a href="/\n<a href="/g' | grep ^'<a href="film' | grep title= | sed 's/ /~/g' >$TMP/cache.$PARSER.$INPUT.2

		while read -u 3 ROUND; do
			TITLE=`echo $ROUND | sed 's/title=/\ntitle=/' | grep ^"title=" | cut -d '"' -f2 | tr '~' ' ' | sed 's/#/%/'`
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' # ' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/\C3x/g' -e 's/&ndash;/-/g' -e 's/&Auml;/\C3/g' -e 's/&Uuml;/\C3S/g' -e 's/&Ouml;/\C3/g' -e 's/&auml;/ä/g' -e 's/&uuml;/ü/g' -e 's/&ouml;/ö/g' -e 's/&eacute;/é/g' -e 's/&egrave;/è/g' -e 's/%F6/ö/g' -e 's/%FC/ü/g' -e 's/%E4/ä/g' -e 's/%26/&/g' -e 's/%C4/\C3/g' -e 's/%D6/\C3/g' -e 's/%DC/\C3S/g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
			PIC=$URL/`echo $ROUND | sed s'!<img~src=!\nsrc=!' | grep ^"src=" | cut -d '"' -f2 | tr '~' ' '`
			NEWPAGE=`echo $ROUND | sed 's/<a~href=/\nhref=/' | grep ^"href=" | cut -d '"' -f2`

			if [ "$PIC" == "$URL/" ] ; then  
				PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
				TMPPIC="default.jpg"
			else
				TMPPIC=goldesel_`echo $PIC | cut -d '/' -f6`
			fi

			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
				if [ `cat $TMP/$PARSER.$INPUT.list | grep ^"$NEWPAGE" | wc -l` -eq 0 ];then
					if [ ! -e $TMP/$PARSER.$INPUT.list ];then
						touch $TMP/$PARSER.$INPUT.list
					fi
					LINE="$TITLE#$SRC $SRC hosterlist $NEWPAGE#$PIC#$TMPPIC#$NAME#0"
					echo "$LINE" >> $TMP/$PARSER.$INPUT.list
				fi
			fi
		done 3<$TMP/cache.$PARSER.$INPUT.2
#		rm $TMP/cache.$PARSER.$INPUT.* > /dev/null 2>&1
	fi

	if [ -e "$TMP/$PARSER.hosterlist.list" ] ; then
		rm $TMP/$PARSER.hosterlist.list
	fi

	echo "$TMP/$PARSER.$INPUT.list"
}

page()
{
	if [ ! -e "$TMP/$PARSER.$INPUT.list" ] ; then
#		$curlbin $URL/$PAGE -o $TMP/cache.$PARSER.$INPUT.1
		/tmp/localhoster/hoster.sh get $URL/$PAGE > $TMP/cache.$PARSER.$INPUT.1

		cat $TMP/cache.$PARSER.$INPUT.1 | sed 's/<p><ul class="rls_table/\n<p><ul class=rls_table/g' | sed 's/<\/ul><\/u>/<\/ul><\/u>\n/g' | grep ^'<p><ul class=rls_table' | sed 's/<a href="/\n<a href="/g' | sed 's/<li step=/\n<li step=/g' | grep ^'<a href=' | grep 'tba">VID' | sed 's/ /~/g' >$TMP/cache.$PARSER.$INPUT.2

		while read -u 3 ROUND; do
			TITLE=`echo $ROUND |  sed -e 's/"tle~bold"/"tle"/g' -e 's/<div~class="tle">/\n<div~class="tle""/' -e 's/<b/"b/g' | grep ^'<div~class="tle""' | cut -d '"' -f4 | tr '~' ' ' | sed 's/#/%/'`
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' # ' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/\C3x/g' -e 's/&ndash;/-/g' -e 's/&Auml;/\C3/g' -e 's/&Uuml;/\C3S/g' -e 's/&Ouml;/\C3/g' -e 's/&auml;/ä/g' -e 's/&uuml;/ü/g' -e 's/&ouml;/ö/g' -e 's/&eacute;/é/g' -e 's/&egrave;/è/g' -e 's/%F6/ö/g' -e 's/%FC/ü/g' -e 's/%E4/ä/g' -e 's/%26/&/g' -e 's/%C4/\C3/g' -e 's/%D6/\C3/g' -e 's/%DC/\C3S/g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
			PIC=$URL/`echo $ROUND | sed s'/data-original=/\ndata-original=/' | grep ^'data-original=' | cut -d '"' -f2 | sed 's/\/mini\//\/poster\//g'`
			NEWPAGE=`echo $ROUND | cut -d '"' -f2`
	
			if [ "$PIC" == "$URL/" ] ; then
				PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
				TMPPIC="default.jpg"
			else
				TMPPIC=goldesel_`echo $PIC | cut -d '/' -f6`
			fi

			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ] ; then
				LINE="$TITLE#$SRC $SRC hosterlist $NEWPAGE#$PIC#$TMPPIC#$NAME#0"
				echo "$LINE" >> $TMP/$PARSER.$INPUT.list
			fi
		done 3<$TMP/cache.$PARSER.$INPUT.2
		rm $TMP/cache.$PARSER.$INPUT.* > /dev/null 2>&1
	fi

	if [ -e "$TMP/$PARSER.hosterlist.list" ] ; then
		rm $TMP/$PARSER.hosterlist.list
	fi
		
	echo "$TMP/$PARSER.$INPUT.list"
}

hosterlist()
{
	if [ ! -e "$TMP/$PARSER.$INPUT.list" ] ; then
		$curlbin $URL/$PAGE -o $TMP/cache.$PARSER.$INPUT.1
		cat $TMP/cache.$PARSER.$INPUT.1 | sed "s/<li data=/\n<li data=/g" | grep ^"<li data=" >$TMP/cache.$PARSER.$INPUT.2

		while read -u 3 ROUND; do
			NEWPAGE="data=`echo $ROUND | cut -d '"' -f2 | sed 's/;/%3B/g'`"
			TITLE=`echo $ROUND | cut -d ';' -f5 | cut -d '.' -f1`
			PIC=`echo $ROUND | sed "s/<img src=/\n<img src=/g" | grep ^"<img src=" | cut -d\" -f2 | cut -d "/" -f3 | cut -d"." -f1`
			if [ -z "$TITLE" ];then TITLE=$PIC; fi
			if [ ! -z "$TITLE" ] && [ "$TITLE" != " " ] && [ ! -z "$NEWPAGE" ];then
				PIC=`echo $TITLE | tr [A-Z] [a-z]`
				LINE="$TITLE#$SRC $SRC hoster $NEWPAGE '--referer $URL/$PAGE'#http://openaaf.dyndns.tv/mediathek/menu/$PIC.jpg#$PIC.jpg#$NAME#111"
				echo "$LINE" >> $TMP/$PARSER.$INPUT.list
			fi
		done 3<$TMP/cache.$PARSER.$INPUT.2
		rm $TMP/cache.$PARSER.$INPUT.* > /dev/null 2>&1
	fi

	if [ -e "$TMP/$PARSER.hoster.list" ] ; then
		rm $TMP/$PARSER.hoster.list
	fi

	echo "$TMP/$PARSER.$INPUT.list"
}

hoster()
{
	if [ ! -e "$TMP/$PARSER.$INPUT.list" ] ; then
#		$curlbin $URL/res/links -X POST --data "$PAGE" -o $TMP/cache.$PARSER.$INPUT.1 -A 'Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Maxthon/4.4.7.3000 Chrome/30.0.1599.101 Safari/537.36'
		$curlbin $URL/res/links -X POST --data "$PAGE" -o $TMP/cache.$PARSER.$INPUT.1
		cat $TMP/cache.$PARSER.$INPUT.1 | sed 's!url="http://dref.pw/?!\nstreamurl="!' | grep ^streamurl= | cut -d'"' -f2 | sed 's!%3A!:!g' | sed 's!%2F!/!g' > $TMP/$PARSER.$INPUT.list
#		rm $TMP/cache.$PARSER.$INPUT.* > /dev/null 2>&1
	fi
	cat $TMP/$PARSER.$INPUT.list
}

case $INPUT in
	init) $INPUT;;
	mainmenu) $INPUT;;
	new) $INPUT;;
	page) $INPUT;;
	hosterlist) $INPUT;;
	hoster) $INPUT;;
esac
