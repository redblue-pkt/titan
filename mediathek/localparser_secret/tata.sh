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

URL=https://www.tata.to
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME="Tata"

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
	echo "Channels#$SRC $SRC channels 'channels'#http://atemio.dyndns.tv/mediathek/menu/channels.jpg#all-newfirst.jpg#$NAME#0" > $TMP/$PARSER.$INPUT.list
	echo "$TMP/$PARSER.$INPUT.list"
}

channels()
{

	if [ ! -e "$TMP/$PARSER.$INPUT.list" ] ; then
		$curlbin $URL/$PAGE -o $TMP/cache.$PARSER.$INPUT.1

#		/tmp/localhoster/hoster.sh get $URL/$PAGE > $TMP/cache.$PARSER.$INPUT.1

		cat $TMP/cache.$PARSER.$INPUT.1 | tr '\n' ' ' | tr '\r' ' ' | sed 's/<div class="ml-item chanel-tem">/\n<div class="ml-item chanel-tem">/g' | grep ^'<div class="ml-item chanel-tem">' | grep alt= | grep -v .gif > /$TMP/cache.$PARSER.$INPUT.2
		while read -u 3 ROUND; do
			PIC=`echo $ROUND | sed -nr 's/.*<img src="([^"]+)".*/\1/p'`
			NEWPAGE=`echo $ROUND | sed -nr 's/.*<a href="([^"]+)".*/\1/p'`
			TITLE=`echo $NEWPAGE | tr '/' '\n' | tail -n1`

			if [ -z  "$PIC" ]; then  
				PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
				TMPPIC="default.jpg"
			else
				TMPPIC=tata_`echo $PIC | tr '/' '\n' | tail -n1`
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
					LINE="$TITLE#$SRC $SRC hoster $NEWPAGE#$PIC#$TMPPIC#$NAME#111"
					echo "$LINE" >> $TMP/$PARSER.$INPUT.list
				fi
			fi
		done 3<$TMP/cache.$PARSER.$INPUT.2
		rm $TMP/cache.$PARSER.$INPUT.* > /dev/null 2>&1
	fi

	if [ -e "$TMP/$PARSER.hosterlist.list" ] ; then
		rm $TMP/$PARSER.hosterlist.list
	fi

	echo "$TMP/$PARSER.$INPUT.list"
}

hoster()
{
	rm $TMP/cache.$PARSER.$INPUT.* > /dev/null 2>&1

#	$curlbin $URL/$PAGE -o $TMP/cache.$PARSER.$INPUT.1 -A 'Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Maxthon/4.4.7.3000 Chrome/30.0.1599.101 Safari/537.36'
	$curlbin $PAGE -o $TMP/cache.$PARSER.$INPUT.1

#   <div class="tv-play" data-src="https://s2.skyfall.to/13thStreet/embed.html?dvr=false&token=16f2413148195ed791797dbdf33e9389a2cd4b3d-5f0b55b631af6ce373fb25a15674d074-1518361638-1518350838
	URL=$(cat $TMP/cache.$PARSER.$INPUT.1 | grep tv-play | sed -nr 's/.*data-src="([^"]+).*/\1/p' | sed 's/embed.html/playlist.m3u8/')
	echo $URL"|User-Agent=$USERAGENT"
}

case $INPUT in
	init) $INPUT;;
	mainmenu) $INPUT;;
	channels) $INPUT;;
	hoster) $INPUT;;
esac
