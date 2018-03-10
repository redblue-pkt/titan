#!/bin/sh
# first sh box parser for titannit mfg obi

. /tmp/localhoster/hoster.sh

SRC=$1
INPUT=$2
PAGE=$3
NEXT=$4
PAGE2=$5
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`

#FILENAME="$PARSER $INPUT $PAGE $NEXT $PAGE2"
FILENAME="$PARSER $INPUT $NEXT $PAGE2"
FILENAME=`echo $FILENAME | tr '&' '.' | tr '/' '.' | tr '?' '.'  | tr '=' '.' | sed 's/ \+/./g' | sed 's/\.\+/./g'`

if [ -z "$FILENAME" ]; then
	FILENAME=none
fi

URL=https://api.tvnow.de
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME="TvNow"

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


#nitro page=1
#https://api.tvnow.de/v3/formats?fields=id,title,station,title,titleGroup,seoUrl,icon,hasFreeEpisodes,hasPayEpisodes,categoryId,searchAliasName,genres&filter=%7B%22Id%22:%7B%22containsNotIn%22:%5B%221896%22%5D%7D,%22Disabled%22:0%7D&maxPerPage=500&page=1&page=1

#nitro page=4
#https://api.tvnow.de/v3/formats?fields=id,title,station,title,titleGroup,seoUrl,icon,hasFreeEpisodes,hasPayEpisodes,categoryId,searchAliasName,genres&filter=%7B%22Id%22:%7B%22containsNotIn%22:%5B%221896%22%5D%7D,%22Disabled%22:0%7D&maxPerPage=500&page=1&page=4
#36:
#id	2500
#title	"Matlock"
#station	"nitro"
#titleGroup	"M"
#seoUrl	"matlock"

#matlock
#https://api.tvnow.de/v3/formats/seo?fields=*,.*,formatTabs.*,formatTabs.formatTabPages.*,formatTabs.formatTabPages.container.*,annualNavigation.*&name=matlock.php&station=nitro
#alle folgen
#https://api.tvnow.de/v3/containers/141431/movies?fields=*,format.*,paymentPaytypes.*,livestreamEvent.*,pictures,trailers,packages,annualNavigation&maxPerPage=500&order=OrderWeight+asc,+BroadcastStartDate+desc&page=1
#einzel folge
#https://api.tvnow.de/v3/movies/matlock/der-alptraum?fields=*,format,files,manifest,breakpoints,paymentPaytypes,trailers,packages&station=nitro

mainmenu()
{
	echo "Nitro#$SRC $SRC new 'v3/formats?fields=id,title,station,title,titleGroup,seoUrl,icon,hasFreeEpisodes,hasPayEpisodes,categoryId,searchAliasName,genres&filter=%7B%22Id%22:%7B%22containsNotIn%22:%5B%221896%22%5D%7D,%22Disabled%22:0%7D&maxPerPage=500&page=1' station nitro#http://atemio.dyndns.tv/mediathek/menu/nitro.jpg#nitro.jpg#$NAME#0" > $TMP/$PARSER.$INPUT.list
	echo "Vox#$SRC $SRC new 'v3/formats?fields=id,title,station,title,titleGroup,seoUrl,icon,hasFreeEpisodes,hasPayEpisodes,categoryId,searchAliasName,genres&filter=%7B%22Id%22:%7B%22containsNotIn%22:%5B%221896%22%5D%7D,%22Disabled%22:0%7D&maxPerPage=500&page=1' station  vox#http://atemio.dyndns.tv/mediathek/menu/vox.jpg#vox.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list

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
	if [ ! -e "$TMP/$PARSER.$INPUT.sorted.list" ] ; then
		$curlbin "$URL/$PAGE&page=1" > $TMP/cache.$PARSER.$INPUT.1
		$curlbin "$URL/$PAGE&page=2" >> $TMP/cache.$PARSER.$INPUT.1
		$curlbin "$URL/$PAGE&page=3" >> $TMP/cache.$PARSER.$INPUT.1
		$curlbin "$URL/$PAGE&page=4" >> $TMP/cache.$PARSER.$INPUT.1
		cat $TMP/cache.$PARSER.$INPUT.1  | sed 's/{"id":/\n{"id":/g' | grep ^'{"id":' | grep "\"$NEXT\":\"$PAGE2\"" > /$TMP/cache.$PARSER.$INPUT.2

		while read -u 3 ROUND; do
			TITLE=`echo $ROUND | sed -nr 's/.*"title":"([^"]+).*/\1/p'`
			seoUrl=`echo $ROUND | sed -nr 's/.*"seoUrl":"([^"]+).*/\1/p'`
			titleGroup=`echo $ROUND | sed -nr 's/.*"titleGroup":"([^"]+).*/\1/p'`
			PIC=`echo $ROUND | sed 's!data-original=!\nsrc=!' | grep ^"src=" | cut -d '"' -f2`
			NEWPAGE="v3/formats/seo?fields=*,.*,formatTabs.*,formatTabs.formatTabPages.*,formatTabs.formatTabPages.container.*,annualNavigation.*&name=$seoUrl.php&station=$PAGE2"
			if [ -z  "$PIC" ]; then  
				PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
				TMPPIC="default.jpg"
			else
				TMPPIC=nowtv_`echo $PIC | tr '/' '\n' | tail -n1`
			fi

			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
				if [ `cat $TMP/$PARSER.$INPUT.list | grep ^"$NEWPAGE" | wc -l` -eq 0 ];then
					if [ ! -e $TMP/$PARSER.$INPUT.list ];then
						touch $TMP/$PARSER.$INPUT.list
					fi
					LINE="$TITLE#$SRC $SRC videos '$NEWPAGE'#$PIC#$TMPPIC#$NAME#0"
					echo "$LINE" >> $TMP/$PARSER.$INPUT.list
				fi
			fi
		done 3<$TMP/cache.$PARSER.$INPUT.2
		rm $TMP/cache.$PARSER.$INPUT.* > /dev/null 2>&1
	fi

 	cat $TMP/$PARSER.$INPUT.list | sort -u > $TMP/$PARSER.$INPUT.sorted.list

	if [ -e "$TMP/$PARSER.hosterlist.list" ] ; then
		rm $TMP/$PARSER.hosterlist.list
	fi

#	echo "$TMP/$PARSER.$INPUT.list"
	echo "$TMP/$PARSER.$INPUT.sorted.list"
}

videos()
{
	if [ ! -e "$TMP/$PARSER.$INPUT.list" ] ; then
		$curlbin "$URL/$PAGE" > $TMP/cache.$PARSER.$INPUT.1
		cat $TMP/cache.$PARSER.$INPUT.1 | sed -nr 's/.*"id":([^,]+).*/\1/p' > /$TMP/cache.$PARSER.$INPUT.2
		ID=`cat $TMP/cache.$PARSER.$INPUT.2`
		NEWPAGE="v3/containers/$ID/movies?fields=*,format.*,paymentPaytypes.*,livestreamEvent.*,pictures,trailers,packages,annualNavigation&maxPerPage=500&order=OrderWeight+asc,+BroadcastStartDate+desc&page=1"

		$curlbin "$URL/$NEWPAGE" > $TMP/cache.$PARSER.$INPUT.3
		cat $TMP/cache.$PARSER.$INPUT.3  | sed 's/orderWeight/\norderWeight/g' | grep ^orderWeight > /$TMP/cache.$PARSER.$INPUT.4

		while read -u 3 ROUND; do
			TITLE=`echo $ROUND | sed 's/"title":/\ntitle=/g' | grep ^title= | head -n1 | cut -d'"' -f2`
			PIC=`echo $ROUND | sed -nr 's/.*"defaultDvdImage":"([^"]+).*/\1/p'`
#			NEWPAGE=`echo $ROUND | sed -nr 's/.*"hlsfairplay":"([^"]+).*/\1/p'`
			NEWPAGE=`echo $ROUND | sed -nr 's/.*"dash":"([^"]+).*/\1/p'`

			if [ -z  "$PIC" ]; then  
				PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
				TMPPIC="default.jpg"
			else
				TMPPIC=nowtv_`echo $PIC | tr '/' '\n' | tail -n1`
			fi

			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
				if [ `cat $TMP/$PARSER.$INPUT.list | grep ^"$NEWPAGE" | wc -l` -eq 0 ];then
					if [ ! -e $TMP/$PARSER.$INPUT.list ];then
						touch $TMP/$PARSER.$INPUT.list
					fi
					LINE="$TITLE#$NEWPAGE#$PIC#$TMPPIC#$NAME#2"
					echo "$LINE" >> $TMP/$PARSER.$INPUT.list
				fi
			fi
		done 3<$TMP/cache.$PARSER.$INPUT.4
		rm $TMP/cache.$PARSER.$INPUT.* > /dev/null 2>&1
	fi

	if [ -e "$TMP/$PARSER.hosterlist.list" ] ; then
		rm $TMP/$PARSER.hosterlist.list
	fi

	echo "$TMP/$PARSER.$INPUT.list"
}

case $INPUT in
	init) $INPUT;;
	mainmenu) $INPUT;;
	new) $INPUT;;
	videos) $INPUT;;
esac
