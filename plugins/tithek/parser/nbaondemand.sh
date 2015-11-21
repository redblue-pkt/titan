#!/bin/sh
# first sh box parser for titannit mfg obi

SRC=$1
INPUT=$2
PAGE=$3

ARCH=`cat /etc/.arch`
URL=http://livetv.sx
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`

if [ "$ARCH" = "mipsel" ];then
	NAME=`echo ${PARSER^}`
else
	NAME=`echo $PARSER | sed 's/.*/\u&/'`
fi

wgetbin="wget -q -T2"
TMP=/tmp/parser
TMP=/var/usr/local/share/titan/plugins/tithek/parser/tmp

rm -rf $TMP > /dev/null 2>&1
mkdir $TMP > /dev/null 2>&1

if [ "$SRC" = "/mnt/parser" ];then
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
	echo "$NAME ($TYPE)#$SRC $SRC mainmenu#http://atemio.dyndns.tv/mediathek/menu/$PARSER.jpg#$PARSER.jpg#TiThek#0"
}

mainmenu()
{
	echo "Category#$SRC $SRC category#http://atemio.dyndns.tv/mediathek/menu/categoty.jpg#categoty.jpg#$NAME#0" > /tmp/tithek/$PARSER.$INPUT.list
#	echo "Suche#$SRC $SRC suche#http://atemio.dyndns.tv/mediathek/menu/search.jpg#giga.jpg#$NAME#0" >> /tmp/tithek/$PARSER.$INPUT.list
	echo "/tmp/tithek/$PARSER.$INPUT.list"
}

category()
{
	echo "Latest#$SRC $SRC listvideos en/videotourney/3#http://atemio.dyndns.tv/mediathek/menu/categoty.jpg#categoty.jpg#$NAME#0" > /tmp/tithek/$PARSER.$INPUT.list
	echo "Games#$SRC $SRC submenu http://xlivetv.sx/en/videotourney/3/#http://atemio.dyndns.tv/mediathek/menu/categoty.jpg#categoty.jpg#$NAME#0" >> /tmp/tithek/$PARSER.$INPUT.list
	echo "Highlights#$SRC submenu http://livetv.sx/en/videotourney/3$SRC category#http://atemio.dyndns.tv/mediathek/menu/categoty.jpg#categoty.jpg#$NAME#0" >> /tmp/tithek/$PARSER.$INPUT.list
	echo "Teams#$SRC $SRC submenu http://livetv.sx/en/leagueresults/3/#http://atemio.dyndns.tv/mediathek/menu/categoty.jpg#categoty.jpg#$NAME#0" >> /tmp/tithek/$PARSER.$INPUT.list
  	echo "/tmp/tithek/$PARSER.$INPUT.list"
}

listvideos()
{
	if [ ! -e "/tmp/tithek/$PARSER.$INPUT.list" ]; then
		piccount=0
	
		$wgetbin $URL/$PAGE -O $TMP/cache.$PARSER."$filename"."1"
#		cat $TMP/cache.$PARSER."$filename"."1" | tr '\n' '\r' |  tr '\r' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!<tr> <td> <table width=\"100%\" height=27!\nfound=!g' | grep '^found=' | grep -v LiveTV | tr ' ' '~' >$TMP/cache.$PARSER."$filename"."2"
		LIST=`cat $TMP/cache.$PARSER."$filename"."1" | tr '\n' '\r' |  tr '\r' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!<tr> <td> <table width=\"100%\" height=27!\nfound=!g' | grep '^found=' | grep -v LiveTV | tr ' ' '~'`
	
#		while read -u 3 ROUND; do
		for ROUND in $LIST; do
			ROUND=`echo $ROUND | tr '~' ' '`
			URL=http://xlivetv.sx`echo $ROUND | sed 's!href=!\nurl=!g' | grep ^url= | grep "Full match record" | cut -d'"' -f2`
			TITLE=`echo $ROUND | sed 's!width="30%"> <b>!\ntitle=<!g' | grep ^title= | cut -d'<' -f2 | sed 's!&ndash;!-!g'`
			PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
			EXTRA=`echo $ROUND | sed 's!data-pop="ddr_!\ndate=_!g' | grep ^date= | cut -d'_' -f2`
	
			if [ ! -z "$TITLE" ] && [ ! -z "$EXTRA" ];then
				TITLE="$TITLE ($EXTRA)"
			fi
	
			if [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
				if [ "$ARCH" = "mipsel" ];then
					piccount=$[$piccount+1] 
				else
					piccount=`expr $piccount + 1`
				fi

				LINE="$TITLE#$URL#$PIC#$PARSER_$piccount.jpg#$NAME#98"
				echo "$LINE" >> /tmp/tithek/$PARSER.$INPUT.list
			fi
	
		done
#		done 3<$TMP/cache.$PARSER."$filename"."2"
#		rm $TMP/cache.* > /dev/null 2>&1
	fi
	echo "/tmp/tithek/$PARSER.$INPUT.list"
}

case $INPUT in
	init) $INPUT;;
	mainmenu) $INPUT;;
	category) $INPUT;;
	listvideos) $INPUT;;
esac

