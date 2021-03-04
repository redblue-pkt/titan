#!/bin/sh
# first sh box parser for titannit mfg obi

SRC=$1
INPUT=$2
PAGE=$3
NEXT=$4
PAGE2=$5
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`

URL=https://api.tvnow.de
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME="TvNow"

case $2 in
	init)	;;
	*) 	. /tmp/localhoster/hoster.sh
	   	mkdir $TMP > /dev/null 2>&1
		## start filename work, filename to long
		LIST="5B 1896 500 page Id 7B 22 5D 7D filter containsNotIn Disabled maxPerPage v3 formats? fields= id, title, station, title, titleGroup, seoUrl, icon, hasFreeEpisodes, hasPayEpisodes, categoryId, searchAliasName, genres fields= formatTabs. formatTabPages. container. annualNavigation"
		TMPPAGE="$PAGE"
		for ROUND in $LIST; do
			TMPPAGE=$(echo $TMPPAGE | sed "s/$ROUND/./g")
		done
		TMPPAGE=`echo $TMPPAGE | tr ':' '.' | tr '*' '.' | tr ',' '.' | tr '-' '.' | tr '%' '.' | tr '&' '.' | tr '/' '.' | tr '?' '.'  | tr '=' '.' | sed 's/ \+/./g' | sed 's/\.\+/./g'`
		## done

		FILENAME="$PARSER $INPUT $TMPPAGE $NEXT $PAGE2"
		FILENAME=`echo $FILENAME | tr '&' '.' | tr '/' '.' | tr '?' '.'  | tr '=' '.' | sed 's/ \+/./g' | sed 's/\.\+/./g'`

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
	echo "Rtl#$SRC $SRC new 'v3/formats?fields=id,title,station,title,titleGroup,seoUrl,icon,hasFreeEpisodes,hasPayEpisodes,categoryId,searchAliasName,genres&filter=%7B%22Id%22:%7B%22containsNotIn%22:%5B%221896%22%5D%7D,%22Disabled%22:0%7D&maxPerPage=500&page=1' station rtl#http://openaaf.dyndns.tv/mediathek/menu/rtl.jpg#rtl.jpg#$NAME#0" > $TMP/$FILENAME.list
	echo "Vox#$SRC $SRC new 'v3/formats?fields=id,title,station,title,titleGroup,seoUrl,icon,hasFreeEpisodes,hasPayEpisodes,categoryId,searchAliasName,genres&filter=%7B%22Id%22:%7B%22containsNotIn%22:%5B%221896%22%5D%7D,%22Disabled%22:0%7D&maxPerPage=500&page=1' station vox#http://openaaf.dyndns.tv/mediathek/menu/vox.jpg#vox.jpg#$NAME#0" >> $TMP/$FILENAME.list
	echo "Rtl2#$SRC $SRC new 'v3/formats?fields=id,title,station,title,titleGroup,seoUrl,icon,hasFreeEpisodes,hasPayEpisodes,categoryId,searchAliasName,genres&filter=%7B%22Id%22:%7B%22containsNotIn%22:%5B%221896%22%5D%7D,%22Disabled%22:0%7D&maxPerPage=500&page=1' station rtl2#http://openaaf.dyndns.tv/mediathek/menu/rtl2.jpg#rtl2.jpg#$NAME#0" >> $TMP/$FILENAME.list
	echo "Nitro#$SRC $SRC new 'v3/formats?fields=id,title,station,title,titleGroup,seoUrl,icon,hasFreeEpisodes,hasPayEpisodes,categoryId,searchAliasName,genres&filter=%7B%22Id%22:%7B%22containsNotIn%22:%5B%221896%22%5D%7D,%22Disabled%22:0%7D&maxPerPage=500&page=1' station nitro#http://openaaf.dyndns.tv/mediathek/menu/nitro.jpg#nitro.jpg#$NAME#0" >> $TMP/$FILENAME.list
	echo "N-Tv#$SRC $SRC new 'v3/formats?fields=id,title,station,title,titleGroup,seoUrl,icon,hasFreeEpisodes,hasPayEpisodes,categoryId,searchAliasName,genres&filter=%7B%22Id%22:%7B%22containsNotIn%22:%5B%221896%22%5D%7D,%22Disabled%22:0%7D&maxPerPage=500&page=1' station ntv#http://openaaf.dyndns.tv/mediathek/menu/ntv.jpg#ntv.jpg#$NAME#0" >> $TMP/$FILENAME.list
	echo "RTLplus#$SRC $SRC new 'v3/formats?fields=id,title,station,title,titleGroup,seoUrl,icon,hasFreeEpisodes,hasPayEpisodes,categoryId,searchAliasName,genres&filter=%7B%22Id%22:%7B%22containsNotIn%22:%5B%221896%22%5D%7D,%22Disabled%22:0%7D&maxPerPage=500&page=1' station rtlplus#http://openaaf.dyndns.tv/mediathek/menu/rtlplus.jpg#rtlplus.jpg#$NAME#0" >> $TMP/$FILENAME.list
	echo "Super RTL#$SRC $SRC new 'v3/formats?fields=id,title,station,title,titleGroup,seoUrl,icon,hasFreeEpisodes,hasPayEpisodes,categoryId,searchAliasName,genres&filter=%7B%22Id%22:%7B%22containsNotIn%22:%5B%221896%22%5D%7D,%22Disabled%22:0%7D&maxPerPage=500&page=1' station superrtl#http://openaaf.dyndns.tv/mediathek/menu/superrtl.jpg#superrtl.jpg#$NAME#0" >> $TMP/$FILENAME.list
	echo "WatchBox#$SRC $SRC new 'v3/formats?fields=id,title,station,title,titleGroup,seoUrl,icon,hasFreeEpisodes,hasPayEpisodes,categoryId,searchAliasName,genres&filter=%7B%22Id%22:%7B%22containsNotIn%22:%5B%221896%22%5D%7D,%22Disabled%22:0%7D&maxPerPage=500&page=1' station watchbox#http://openaaf.dyndns.tv/mediathek/menu/watchbox.jpg#watchbox.jpg#$NAME#0" >> $TMP/$FILENAME.list
	echo "$TMP/$FILENAME.list"
}

new()
{
	if [ ! -e "$TMP/$FILENAME.sorted.list" ] ; then
		$curlbin "$URL/$PAGE&page=1" > $TMP/cache.$FILENAME.1
		$curlbin "$URL/$PAGE&page=2" >> $TMP/cache.$FILENAME.1
		$curlbin "$URL/$PAGE&page=3" >> $TMP/cache.$FILENAME.1
		$curlbin "$URL/$PAGE&page=4" >> $TMP/cache.$FILENAME.1
		cat $TMP/cache.$FILENAME.1  | sed 's/{"id":/\n{"id":/g' | grep ^'{"id":' | grep "\"$NEXT\":\"$PAGE2\"" > /$TMP/cache.$FILENAME.2

		while read -u 3 ROUND; do
			TITLE=`echo $ROUND | sed -nr 's/.*"title":"([^"]+).*/\1/p'`
			seoUrl=`echo $ROUND | sed -nr 's/.*"seoUrl":"([^"]+).*/\1/p'`
			titleGroup=`echo $ROUND | sed -nr 's/.*"titleGroup":"([^"]+).*/\1/p'`
			PIC=`echo $ROUND | sed 's!data-original=!\nsrc=!' | grep ^"src=" | cut -d '"' -f2`
			NEWPAGE="v3/formats/seo?fields=*,.*,formatTabs.*,formatTabs.formatTabPages.*,formatTabs.formatTabPages.container.*,annualNavigation.*&name=$seoUrl.php&station=$PAGE2"
			if [ -z  "$PIC" ]; then  
				PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
				TMPPIC="default.jpg"
			else
				TMPPIC=nowtv_`echo $PIC | tr '/' '\n' | tail -n1`
			fi

			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
				if [ `cat $TMP/$FILENAME.list | grep ^"$NEWPAGE" | wc -l` -eq 0 ];then
					if [ ! -e $TMP/$FILENAME.list ];then
						touch $TMP/$FILENAME.list
					fi
					LINE="$TITLE#$SRC $SRC videos '$NEWPAGE'#$PIC#$TMPPIC#$NAME#0"
					echo "$LINE" >> $TMP/$FILENAME.list
				fi
			fi
		done 3<$TMP/cache.$FILENAME.2
		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1

		cat $TMP/$FILENAME.list | sort -u > $TMP/$FILENAME.sorted.list
		if [ -e $TMP/$FILENAME.list ];then
			rm $TMP/$FILENAME.list
		fi
	fi

#	echo "$TMP/$FILENAME.list"
	echo "$TMP/$FILENAME.sorted.list"
}

videos()
{
	if [ ! -e "$TMP/$FILENAME.list" ] ; then
		$curlbin "$URL/$PAGE" > $TMP/cache.$FILENAME.1
		cat $TMP/cache.$FILENAME.1 | sed -nr 's/.*"id":([^,]+).*/\1/p' > /$TMP/cache.$FILENAME.2
		ID=`cat $TMP/cache.$FILENAME.2`
		NEWPAGE="v3/containers/$ID/movies?fields=*,format.*,paymentPaytypes.*,livestreamEvent.*,pictures,trailers,packages,annualNavigation&maxPerPage=500&order=OrderWeight+asc,+BroadcastStartDate+desc&page=1"

		$curlbin "$URL/$NEWPAGE" > $TMP/cache.$FILENAME.3
		cat $TMP/cache.$FILENAME.3  | sed 's/orderWeight/\norderWeight/g' | grep ^orderWeight > /$TMP/cache.$FILENAME.4

		while read -u 3 ROUND; do
			TITLE=`echo $ROUND | sed 's/"title":/\ntitle=/g' | grep ^title= | head -n1 | cut -d'"' -f2`
			PIC=`echo $ROUND | sed -nr 's/.*"defaultDvdImage":"([^"]+).*/\1/p'`
#  			dash working on sh4 libeplayer mipsel/arm need dash patch
#			NEWPAGE=`echo $ROUND | sed -nr 's/.*"dash":"([^"]+).*/\1/p'`
			NEWPAGE=`echo $ROUND | sed -nr 's/.*"hlsclear":"([^"]+).*/\1/p'`

			#NEWPAGE="https://vodnowusohls-a.akamaihd.net/proxy/clear/manifest/tvnow/417382-1-17915.ism/fairplay.m3u8?filter=(type%3d%3d%22audio%22)%7c%7c(type%3d%3d%22video%22%26%26systemBitrate%3c1550000)"
			PART1=$(echo "$NEWPAGE" | sed 's!fairplay.m3u8!\n!' | grep ^http)
			PART2=$(echo "$NEWPAGE" | sed -nr 's!.*/tvnow/([^.ism/]+).*!\1!p')
			#echo PART1 $PART1
			#echo PART2 $PART2

			#https://vodnowusohls.secure.footprint.net/proxy/clear/manifest/tvnow/417382-1-17915.ism/417382-1-17915-audio=128000-video=1400000.m3u8
			NEWPAGE=$PART1$PART2-audio=128000-video=1400000.m3u8

			if [ -z  "$PIC" ]; then  
				PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
				TMPPIC="default.jpg"
			else
				TMPPIC=nowtv_`echo $PIC | tr '/' '\n' | tail -n1`
			fi

			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
				if [ `cat $TMP/$FILENAME.list | grep ^"$NEWPAGE" | wc -l` -eq 0 ];then
					if [ ! -e $TMP/$FILENAME.list ];then
						touch $TMP/$FILENAME.list
					fi
					LINE="$TITLE#$NEWPAGE#$PIC#$TMPPIC#$NAME#2"
					echo "$LINE" >> $TMP/$FILENAME.list
				fi
			fi
		done 3<$TMP/cache.$FILENAME.4
		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi

	echo "$TMP/$FILENAME.list"
}

case $INPUT in
	init) $INPUT;;
	mainmenu) $INPUT;;
	new) $INPUT;;
	videos) $INPUT;;
esac
