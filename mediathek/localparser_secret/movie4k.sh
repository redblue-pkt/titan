#!/bin/sh
# first sh box parser for titannit mfg obi

. /tmp/localhoster/hoster.sh

SRC=$1
INPUT=$2
PAGE=$3

FILENAME=`echo $PAGE | tr '/' '.'`
FILENAME=`echo $FILENAME | tr '&' '.'`

if [ -z "$PAGE" ]; then
	FILENAME=none
fi

URL=http://movie4k.to
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME=Movie4k

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
#	echo "Category#$SRC $SRC category#http://atemio.dyndns.tv/mediathek/menu/category.jpg#category.jpg#$NAME#0" > $TMP/$PARSER.$INPUT.list
	echo "Search#$SRC $SRC search 'movies.php?list=search&search=%search%'#http://atemio.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >$TMP/$PARSER.$INPUT.list
	echo "$TMP/$PARSER.$INPUT.list"
}

search()
{
	rm $TMP/$PARSER.$INPUT.list > /dev/null 2>&1
	if [ -e "$TMP/$PARSER.$INPUT.list" ] ; then
		rm $TMP/$PARSER.$INPUT.list
	fi

	piccount=0
	/tmp/localhoster/hoster.sh get $URL/$PAGE > $TMP/cache.$PARSER.$INPUT.1
#	$curlbin "$URL/$PAGE" -o "$TMP/cache.$PARSER.$INPUT.1" -A 'Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Maxthon/4.4.7.3000 Chrome/30.0.1599.101 Safari/537.36'
	cat $TMP/cache.$PARSER.$INPUT.1 | tr '\n' '\r' |  tr '\r' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!tablemoviesindex!\ntablemoviesindex!g' | grep ^"tablemoviesindex" | sed 's!coverPreview!\ncoverPreview!g' | grep ^"coverPreview" | grep 'id="tdmovies"' >$TMP/cache.$PARSER.$INPUT.2

	while read -u 3 ROUND; do
		ID=`echo $ROUND | cut -d'"' -f1 | tail -n1`
		PIC=`echo $ROUND | sed 's!"url": !\nurl=!g' | grep ^url= | cut -d'"' -f2 | tail -n1`
		TITLE=`echo $ROUND | sed 's!<a href=!\nfound=>!g' | grep ^found= | cut -d">" -f3 | cut -d"<" -f1`
		NEWPAGE=`echo $ROUND | sed 's!<a href=!\nfound=!g' | grep ^found= | cut -d'"' -f2 | tail -n1`

		if [ ! -z "$NEWPAGE" ]; then
			if [ -z "$TITLE" ];then
				TITLE=`echo $TMPURL`
			fi
			TMPURL="$URL"/$TMPURL
		fi

		PIC=`cat $TMP/cache.$PARSER.$INPUT.1 | tr '\n' '\r' |  tr '\r' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed "s/$ID/\n$ID/g" | grep ^"$ID"'").hover' | sed "s#img src=#\nfound=#g" | grep ^"found=" | cut -d"'" -f2 | head -n1`

		if [ `echo $ROUND | grep us_ger_small.png | wc -l` -eq 1 ]; then
			LANG=" (de)"
		elif [ `echo $ROUND | grep us_flag_small.png | wc -l` -eq 1 ]; then
			LANG=" (en)"
		else
			LANG=" (??)"
		fi

		if [ `echo $TMPURL | grep "online-serie-" | wc -l` -eq 1 ]; then
			STREAMTYPE="40"
		else
			STREAMTYPE="34"
		fi

		if [ -z "$PIC" ]; then
			PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
		fi

		TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`

		if [ ! -z "$TITLE" ] && [ ! -z "$TMPURL" ];then
			if [ ! -e $TMP/$PARSER.$INPUT.$FILENAME.list ];then
				touch $TMP/$PARSER.$INPUT.$FILENAME.list
			fi
			piccount=$[$piccount+1]
#				LINE="$TITLE#$SRC $SRC list '$TMPURL'#$PIC#$PARSER_$piccount.jpg#$NAME#0"
#				LINE="$TITLE $LANG#$TMPURL#$PIC#$PARSER_$piccount.jpg#$NAME#14"
#				LINE="$TITLE $LANG#$TMPURL#$PIC#$PARSER_$piccount.jpg#$NAME#$STREAMTYPE"
#				LINE="$TITLE $LANG#$TMPURL#$PIC#$PARSER_$piccount.jpg#$NAME#34"
			LINE="$TITLE $LANG#$SRC $SRC hosterlist $NEWPAGE#$PIC#$PARSER_$piccount.jpg#$NAME#0"

			echo "$LINE" >> $TMP/$PARSER.$INPUT.list
		fi

	done 3<$TMP/cache.$PARSER.$INPUT.2
	rm $TMP/cache.* > /dev/null 2>&1

	echo "$TMP/$PARSER.$INPUT.list"
}

hosterlist()
{
	if [ -e "$TMP/$PARSER.$INPUT.list" ] ; then
		rm $TMP/$PARSER.$INPUT.list
	fi
#	$curlbin $URL/$PAGE -o $TMP/cache.$PARSER.$INPUT.1 -A 'Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Maxthon/4.4.7.3000 Chrome/30.0.1599.101 Safari/537.36'
	/tmp/localhoster/hoster.sh get $URL/$PAGE > $TMP/cache.$PARSER.$INPUT.1

	cat $TMP/cache.$PARSER.$INPUT.1 | grep ^"links\[" >$TMP/cache.$PARSER.$INPUT.2

	while read -u 3 ROUND; do
		NEWPAGE="`echo $ROUND | cut -d '"' -f9 | sed 's/;/%3B/g'`"
		TITLE=`echo $ROUND | sed 's!&nbsp;!\nfound=<!g' | grep ^"found=<" | cut -d"<" -f2` 		
	
		if [ ! -z "$TITLE" ] && [ "$TITLE" != " " ] && [ ! -z "$NEWPAGE" ];then
			PIC=`echo $TITLE | tr [A-Z] [a-z]`
#			LINE="$TITLE#$SRC $SRC hoster $NEWPAGE '--referer $URL/$PAGE'#http://atemio.dyndns.tv/mediathek/menu/$PIC.jpg#$PIC.jpg#$NAME#111"
			LINE="$TITLE#$SRC $SRC hoster $NEWPAGE#http://atemio.dyndns.tv/mediathek/menu/$PIC.jpg#$PIC.jpg#$NAME#111"

#			LINE="$TITLE#$URL/$NEWPAGE#http://atemio.dyndns.tv/mediathek/menu/$PIC.jpg#$PIC.jpg#$NAME#41"

			echo "$LINE" >> $TMP/$PARSER.$INPUT.list
		fi
	done 3<$TMP/cache.$PARSER.$INPUT.2
#	rm $TMP/cache.$PARSER.$INPUT.* > /dev/null 2>&1
	echo "$TMP/$PARSER.$INPUT.list"
}

hoster()
{
	rm $TMP/cache.$PARSER.$INPUT.* > /dev/null 2>&1
#	$curlbin $URL/$PAGE -o $TMP/cache.$PARSER.$INPUT.1 -A 'Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Maxthon/4.4.7.3000 Chrome/30.0.1599.101 Safari/537.36'
	/tmp/localhoster/hoster.sh get $URL/$PAGE > $TMP/cache.$PARSER.$INPUT.1
	
	STREAMURL=`cat $TMP/cache.$PARSER.$INPUT.1 | sed 's!<a target="_blank" href="!\nstreamurl="!' | grep ^streamurl= | cut -d'"' -f2`
#	rm $TMP/cache.$PARSER.$INPUT.* > /dev/null 2>&1
	echo $STREAMURL
}

case $INPUT in
	init) $INPUT;;
	mainmenu) $INPUT;;
	hosterlist) $INPUT;;
	hoster) $INPUT;;
	search) $INPUT;;
esac
