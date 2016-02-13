#!/bin/sh
# first sh box parser for titannit mfg obi

SRC=$1
INPUT=$2
FROM=$3
PAGE=$4
FILENAME=`echo $PAGE | tr '/' '.'`
if [ -z "$PAGE" ]; then
	FILENAME=none
fi

ARCH=`cat /etc/.arch`
URL=http://cricfree.sx/
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME=CricFree

curlbin='curl -k -s -v -L --cookie /mnt/network/cookies --cookie-jar /mnt/network/cookies'
#useragent="Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Maxthon/4.4.7.3000 Chrome/30.0.1599.101 Safari/537.36"

wgetbin="wget -q -T2"
TMP=/tmp/parser
#TMP=/mnt/parser/tmp
#TMP=/var/usr/local/share/titan/plugins/tithek/parser/tmp

#rm -rf $TMP > /dev/null 2>&1
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
	if [ "$ARCH" != "sh4" ];then
		rm -f /mnt/network/cookies > /dev/null 2>&1
		rm -rf $TMP > /dev/null 2>&1
		echo "$NAME ($TYPE)#$SRC $SRC mainmenu#http://atemio.dyndns.tv/mediathek/menu/$PARSER.jpg#$PARSER.jpg#TiThek#0"
	fi
}

mainmenu()
{
	echo "Category#$SRC $SRC category#http://atemio.dyndns.tv/mediathek/menu/categoty.jpg#categoty.jpg#$NAME#0" > $TMP/$PARSER.$INPUT.list
	echo "$TMP/$PARSER.$INPUT.list"
}

category()
{
	echo "Channels#$SRC $SRC list channels /#http://atemio.dyndns.tv/mediathek/menu/categoty.jpg#categoty.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
  	echo "$TMP/$PARSER.$INPUT.list"
}

list()
{
	if [ ! -e "$TMP/$PARSER.$INPUT.$FROM.$FILENAME.list" ]; then
		piccount=0

		$wgetbin $URL/$PAGE -O $TMP/cache.$PARSER.$FROM.$FILENAME.1
		cat $TMP/cache.$PARSER.$FROM.$FILENAME.1 | tr '\n' '\r' |  tr '\r' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!<li class="has-sub">!\n<li class="has-sub">!g' | grep ^'<li class="has-sub">' | sed 's!href=!\n<a href=!g' | grep ^'<a href=' | grep "http://cricfree.sx" | grep -v "/watch/live/" | cut -d '"' -f2 >$TMP/cache.$PARSER.$FROM.$FILENAME.2

		while read -u 3 ROUND; do

			URL=`echo $ROUND | cut -d'"' -f1`
			TITLE=`echo $ROUND | sed 's!http://cricfree.sx/!!' | sed 's!watch/live/!!' | sed 's!-live-stream-2!!' | sed 's!-live-stream!!' | sed 's!-in-streaming!!' | sed 's!-live-streaming-2!!' | sed 's!-live-streaming!!'`
			PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
		
			TITLE=`echo $TITLE | sed 's/-/ /g' | sed 's/_/ /g'`
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`

			if [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
				if [ ! -e $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list ];then
					touch $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list
				fi
				piccount=$[$piccount+1]
				LINE="$TITLE#$URL#$PIC#$PARSER_$piccount.jpg#$NAME#92"
				echo "$LINE" >> $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list
			fi
	
		done 3<$TMP/cache.$PARSER.$FROM.$FILENAME.2
		rm $TMP/cache.* > /dev/null 2>&1
	fi
	echo "$TMP/$PARSER.$INPUT.$FROM.$FILENAME.list"
}

case $INPUT in
	init) $INPUT;;
	mainmenu) $INPUT;;
	category) $INPUT;;
	list) $INPUT;;
esac