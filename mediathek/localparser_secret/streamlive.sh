#!/bin/sh
# first sh box parser for titannit mfg obi

SRC=$1
INPUT=$2
FROM=$3
PAGE=$4

URL=www.streamlive.to
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME=StreamLive

case $2 in
	init)	;;
	*) 	. /tmp/localhoster/hoster.sh
	   	mkdir $TMP > /dev/null 2>&1
		FILENAME="$PARSER $INPUT $FROM $PAGE"
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
#	if [ "$ARCH" != "sh4" ] || [ -e /etc/.beta ];then
#		rm -f /mnt/network/cookies > /dev/null 2>&1
		rm -rf $TMP > /dev/null 2>&1
		echo "$NAME ($TYPE)#$SRC $SRC mainmenu#http://openaaf.dyndns.tv/mediathek/menu/$PARSER.jpg#$PARSER.jpg#TiThek#0"
#	fi
}

mainmenu()
{
	echo "Category#$SRC $SRC category#http://openaaf.dyndns.tv/mediathek/menu/category.jpg#category.jpg#$NAME#0" > $TMP/$PARSER.$INPUT.list
	echo "$TMP/$PARSER.$INPUT.list"
}

category()
{
	if [ ! -e "/tmp/tithek/$PARSER.$INPUT.list" ]; then
		tagcount=60
		i=1
		until [ "$i" -gt "$tagcount" ]
		do
		TITLE="Page $i"
		echo "$TITLE#$SRC $SRC page category '?p=$i&q=&sort=1'#http://openaaf.dyndns.tv/mediathek/menu/page.jpg#page.jpg#$NAME#0" >> /tmp/tithek/$PARSER.$INPUT.list
		i=`expr $i + 1`
		done
	fi
	echo "/tmp/tithek/$PARSER.$INPUT.list"
}

page()
{
	if [ ! -e "$TMP/$PARSER.$INPUT.$FROM.$FILENAME.list" ]; then
		piccount=0

		$curlbin $URL/$PAGE -o $TMP/cache.$PARSER.$FROM.$FILENAME.1

		cat $TMP/cache.$PARSER.$FROM.$FILENAME.1 | tr '\n' '\r' |  tr '\r' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!<a href="http://www.streamlive.to/view/!\nhttp://www.streamlive.to/view/!g' | grep ^"http://www.streamlive.to/view/" | grep -v "<strong>" | grep -v "premium_only" >$TMP/cache.$PARSER.$FROM.$FILENAME.2

		while read -u 3 ROUND; do

			URL=`echo $ROUND | cut -d'"' -f1`
			PIC=`echo $ROUND | sed 's!src="!\npic="!g' | grep ^pic= | cut -d'"' -f2 | tail -n1`
			TITLE=`echo $ROUND | sed 's!title="!\ntitle="!g' | grep ^title= | cut -d'"' -f2 | tail -n1`
			if [ -z "$TITLE" ];then
				TITLE=`echo $ROUND | cut -d"<" -f2 | cut -d">" -f2`
			fi

			if [ -z "$PIC" ] || [ "$PIC" = "http:" ]; then
				PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
			fi

			TITLE=`echo $URL | tr '/' '\n' | tail -n1 | sed 's/_/ /g'`
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`

			if [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
				if [ ! -e $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list ];then
					touch $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list
				fi
				piccount=`expr $piccount + 1`
				LINE="$TITLE#$URL#$PIC#$PARSER_$piccount.jpg#$NAME#91"
				echo "$LINE" >> $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list
			fi

		done 3<$TMP/cache.$PARSER.$FROM.$FILENAME.2
		rm $TMP/cache.* > /dev/null 2>&1
	fi
	echo "$TMP/$PARSER.$INPUT.$FROM.$FILENAME.list"
}

case $INPUT in
#	init) $INPUT;;
	mainmenu) $INPUT;;
	category) $INPUT;;
	page) $INPUT;;
esac
