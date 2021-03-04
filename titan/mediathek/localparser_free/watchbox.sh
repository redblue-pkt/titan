#!/bin/sh
# first sh box parser for titannit mfg obi

SRC=$1
INPUT=$2
CURPAGE=$3
MAXPAGE=$4
PAGE=$5

URL=https://www.watchbox.de
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME="Watchbox"

case $2 in
	init)	;;
	*) 	. /tmp/localhoster/hoster.sh
	   	mkdir $TMP > /dev/null 2>&1
		FILENAME="$PARSER $INPUT $PAGE $NEXT"
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
	echo "Neue Filme#$SRC $SRC new 1 0 'filme/neu/'#http://openaaf.dyndns.tv/mediathek/menu/Movies.jpg#Movies.jpg#$NAME#0" > $TMP/$PARSER.$INPUT.list
	echo "Neue Serien#$SRC $SRC series 1 0 'serien/neu/'#http://openaaf.dyndns.tv/mediathek/menu/serien.jpg#serien.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "Filme suchen#$SRC $SRC search 1 0 'https://api.watchbox.de/v1/search/?active=true&maxPerPage=30&page=1&types=%5B%22film%22%5D&term='#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >> $TMP/$PARSER.$INPUT.list
	echo "Serien suchen#$SRC $SRC search2 1 0 'https://api.watchbox.de/v1/search/?active=true&maxPerPage=30&page=1&types=%5B%22serie%22%5D&term='#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >> $TMP/$PARSER.$INPUT.list

	if [ "`echo $TMP/$PARSER.new.*.list`" != "$TMP/$PARSER.new.*.list" ] ; then
		rm $TMP/$PARSER.new.*.list
	fi
	if [ "`echo $TMP/$PARSER.search.*.list`" != "$TMP/$PARSER.search.*.list" ] ; then
		rm $TMP/$PARSER.search.*.list
	fi
	if [ "`echo $TMP/$PARSER.series.*.list`" != "$TMP/$PARSER.series.*.list" ] ; then
		rm $TMP/$PARSER.series.*.list
	fi
	
	echo "$TMP/$PARSER.$INPUT.list"
}

new()
{
	if [ ! -e "$TMP/$PARSER.$INPUT.$CURPAGE.list" ] ; then
		
		if [ "$CURPAGE" -eq "1" ] ; then
			NEWPAGE=$PAGE
		else
			NEWPAGE=$PAGE'?page='`expr $CURPAGE - 1`
		fi
		
		$curlbin $URL/$NEWPAGE -o $TMP/cache.$PARSER.$INPUT.$CURPAGE.1

		cat $TMP/cache.$PARSER.$INPUT.$CURPAGE.1 | grep -A 14 'data-asset-title' > /$TMP/cache.$PARSER.$INPUT.$CURPAGE.1a
		cat $TMP/cache.$PARSER.$INPUT.$CURPAGE.1a | tr "\n" " " | sed -e 's/--/--\n/g' > /$TMP/cache.$PARSER.$INPUT.$CURPAGE.2
		echo >> $TMP/cache.$PARSER.$INPUT.$CURPAGE.2
		while read -u 3 ROUND; do
			TITLE=`echo $ROUND | sed 's/"data-asset-title="/\n/g' | grep "data-asset-title=" | cut -d '"' -f2`
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/&%8211;/-/g' -e "s/&%8217;/'/g"`
			PIC=`echo $ROUND | sed 's/src/\nsrc/g' | sed 's!src="!src="https:!g' | grep "src=" | cut -d '"' -f2`
			NEWPAGE=`echo $ROUND | sed 's/href=/\nhref=/' | sed 's!href=!href=https://www.watchbox.de!g' | grep ^"href=" | cut -d '"' -f2`

			if [ -z  "$PIC" ]; then  
				PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
				TMPPIC="default.jpg"
			else
				TMPPIC=watchbox_`echo $PIC | tr '/' '\n' | tail -n1`
			fi

			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
					LINE="$TITLE#$SRC $SRC play 0 0 $NEWPAGE#$PIC#$TMPPIC#$NAME#111"
					echo "$LINE" >> $TMP/$PARSER.$INPUT.$CURPAGE.list
				
			fi
		done 3<$TMP/cache.$PARSER.$INPUT.$CURPAGE.2
		
		if grep -Fq "<span>Zeig mir mehr</span>" $TMP/cache.$PARSER.$INPUT.$CURPAGE.1
		then
		
		NEWPAGE=`expr $CURPAGE + 1`
		
		echo "Page $NEWPAGE#$SRC $SRC new $NEWPAGE $MAXPAGE '$PAGE'#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.$CURPAGE.list
		fi
		

		rm $TMP/cache.$PARSER.$INPUT.$CURPAGE.* > /dev/null 2>&1
	fi

	echo "$TMP/$PARSER.$INPUT.$CURPAGE.list"
}

series()
{
	if [ ! -e "$TMP/$PARSER.$INPUT.$CURPAGE.list" ] ; then
		
		if [ "$CURPAGE" -eq "1" ] ; then
			NEWPAGE=$PAGE
		else
			NEWPAGE=$PAGE'?page='`expr $CURPAGE - 1`
		fi
		
		$curlbin $URL/$NEWPAGE -o $TMP/cache.$PARSER.$INPUT.$CURPAGE.1

		cat $TMP/cache.$PARSER.$INPUT.$CURPAGE.1 | grep -A 14 'data-asset-title' | sed '/"Show"/,/alt/d' > /$TMP/cache.$PARSER.$INPUT.$CURPAGE.1a
		cat $TMP/cache.$PARSER.$INPUT.$CURPAGE.1a | tr "\n" " " | sed -e 's/--/<--\n/g' > /$TMP/cache.$PARSER.$INPUT.$CURPAGE.2
		echo >> $TMP/cache.$PARSER.$INPUT.$CURPAGE.2
		while read -u 3 ROUND; do
			TITLE=`echo $ROUND | sed 's/"data-asset-title="/\n/g' | grep "data-asset-title=" | cut -d '"' -f2`
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/&%8211;/-/g' -e "s/&%8217;/'/g"`
			PIC=`echo $ROUND | sed 's/src/\nsrc/g' | sed 's!src="!src="https:!g' | grep "src=" | cut -d '"' -f2`
			NEWPAGE=`echo $ROUND | sed 's/href=/\nhref=/' | sed 's!href=!href=https://www.watchbox.de!g' | grep ^"href=" | cut -d '"' -f2`

			if [ -z  "$PIC" ]; then  
				PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
				TMPPIC="default.jpg"
			else
				TMPPIC=watchbox_`echo $PIC | tr '/' '\n' | tail -n1`
			fi

			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
					LINE="$TITLE#$SRC $SRC season 0 0 '$NEWPAGE'#$PIC#$TMPPIC#$NAME#0"
					echo "$LINE" >> $TMP/$PARSER.$INPUT.$CURPAGE.list
				
			fi
		done 3<$TMP/cache.$PARSER.$INPUT.$CURPAGE.2
		
		if grep -Fq "<span>Zeig mir mehr</span>" $TMP/cache.$PARSER.$INPUT.$CURPAGE.1
		then
		
		NEWPAGE=`expr $CURPAGE + 1`
		
		echo "Page $NEWPAGE#$SRC $SRC series $NEWPAGE $MAXPAGE '$PAGE'#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.$CURPAGE.list
		fi
		

		rm $TMP/cache.$PARSER.$INPUT.$CURPAGE.* > /dev/null 2>&1
	fi
	
	if [ "`echo $TMP/$PARSER.season.*.list`" != "$TMP/$PARSER.season.*.list" ] ; then
		rm $TMP/$PARSER.season.*.list
	fi
	
	echo "$TMP/$PARSER.$INPUT.$CURPAGE.list"
}

season()
{
	if [ ! -e "$TMP/$PARSER.$INPUT.$CURPAGE.list" ] ; then
		
		if [ "$CURPAGE" -eq "1" ] ; then
			NEWPAGE=$PAGE
		else
			NEWPAGE=$PAGE'?page='`expr $CURPAGE - 1`
		fi
		
		$curlbin $URL$NEWPAGE -o $TMP/cache.$PARSER.$INPUT.$CURPAGE.1

		cat $TMP/cache.$PARSER.$INPUT.$CURPAGE.1 | grep -A 14 'data-title' > /$TMP/cache.$PARSER.$INPUT.$CURPAGE.1a
		cat $TMP/cache.$PARSER.$INPUT.$CURPAGE.1a | tr "\n" " " | sed -e 's/data-title/\ndata-title/g' > /$TMP/cache.$PARSER.$INPUT.$CURPAGE.2
		echo >> $TMP/cache.$PARSER.$INPUT.$CURPAGE.2
		while read -u 3 ROUND; do
			TITLE=`echo $ROUND | sed 's/"data-title="/\n/g' | grep "data-title=" | cut -d '"' -f2`
			TITLE=`echo $TITLE | sed -e 's/-/ /g' -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/&%8211;/-/g' -e "s/&%8217;/'/g"`
#			PIC=`echo $ROUND | sed 's/src/\nsrc/g' | sed 's!src="!src="https:!g' | grep "src=" | cut -d '"' -f2`
			NEWPAGE=`echo $ROUND | sed 's/href=/\nhref=/' | sed 's!href=!href=https://www.watchbox.de!g' | grep ^"href=" | cut -d '"' -f2`

			if [ -z  "$PIC" ]; then  
				PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
				TMPPIC="default.jpg"
			else
				TMPPIC=watchbox_`echo $PIC | tr '/' '\n' | tail -n1`
			fi

			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
				
					LINE="$TITLE#$SRC $SRC episode 0 0 '$NEWPAGE'#$PIC#$TMPPIC#$NAME#0"
					echo "$LINE" >> $TMP/$PARSER.$INPUT.$CURPAGE.list
				
			fi
		done 3<$TMP/cache.$PARSER.$INPUT.$CURPAGE.2

		rm $TMP/cache.$PARSER.$INPUT.$CURPAGE.* > /dev/null 2>&1
	fi
	
	if [ "`echo $TMP/$PARSER.episode.*.list`" != "$TMP/$PARSER.episode.*.list" ] ; then
		rm $TMP/$PARSER.episode.*.list
		rm /tmp/tithek/watchbox.episode.* > /dev/null 2>&1
	
	fi
	
	echo "$TMP/$PARSER.$INPUT.$CURPAGE.list"
}

episode()
{
	if [ ! -e "$TMP/$PARSER.$INPUT.$CURPAGE.list" ] ; then
		
		if [ "$CURPAGE" -eq "1" ] ; then
			NEWPAGE=$PAGE
		else
			NEWPAGE=$PAGE'?page='`expr $CURPAGE - 1`
		fi
		
		$curlbin $URL/$NEWPAGE -o $TMP/cache.$PARSER.$INPUT.$CURPAGE.1

		cat $TMP/cache.$PARSER.$INPUT.$CURPAGE.1 | grep -A 14 'data-asset-title' > /$TMP/cache.$PARSER.$INPUT.$CURPAGE.1a
		cat $TMP/cache.$PARSER.$INPUT.$CURPAGE.1a | tr "\n" " " | sed -e 's/--/--\n/g' | sed '/Platzhalter/,$ d' > /$TMP/cache.$PARSER.$INPUT.$CURPAGE.2
		echo >> $TMP/cache.$PARSER.$INPUT.$CURPAGE.2
		while read -u 3 ROUND; do
			TITLE=`echo $ROUND | sed 's/"data-asset-title="/\n/g' | grep "data-asset-title=" | cut -d '"' -f2`
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/&%8211;/-/g' -e "s/&%8217;/'/g"`
			PIC=`echo $ROUND | sed 's/data-src/\ndata-src/g' | sed 's!data-src="!data-src="https:!g' | grep "data-src=" | cut -d '"' -f2`
			NEWPAGE=`echo $ROUND | sed 's/href=/\nhref=/' | sed 's!href=!href=https://www.watchbox.de!g' | grep ^"href=" | cut -d '"' -f2`

			if [ -z  "$PIC" ]; then  
				PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
				TMPPIC="default.jpg"
			else
				TMPPIC=watchbox.$INPUT.`echo $PIC | tr '/' '\n' | tail -n1`
			fi

			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
				LINE="$TITLE#$SRC $SRC play 0 0 $NEWPAGE#$PIC#$TMPPIC#$NAME#111"
				echo "$LINE" >> $TMP/$PARSER.$INPUT.$CURPAGE.list
				
			fi
		done 3<$TMP/cache.$PARSER.$INPUT.$CURPAGE.2
		

		rm $TMP/cache.$PARSER.$INPUT.$CURPAGE.* > /dev/null 2>&1
	fi
	
	if [ "`echo $TMP/$PARSER.play.list`" != "$TMP/$PARSER.play.list" ] ; then
		rm $TMP/$PARSER.play.list
	fi
	
	
	echo "$TMP/$PARSER.$INPUT.$CURPAGE.list"
}


search()
{
	if [ ! -e "$TMP/$PARSER.$INPUT.$CURPAGE.list" ] ; then
		$curlbin $PAGE -o $TMP/cache.$PARSER.$INPUT.1

		cat $TMP/cache.$PARSER.$INPUT.1 | sed 's!"type"!\n"type"!g' > $TMP/cache.$PARSER.$INPUT.2
		echo >> $TMP/cache.$PARSER.$INPUT.2
		while read -u 3 ROUND; do
			TITLE=`echo $ROUND | grep 'headline":"' | sed -e 's!headline":"!\n"!g' | cut -d '"' -f2 | sed '/type/d'`
			ENTITYID=`echo $ROUND | grep 'entityId":' | sed -e 's!entityId":!\n!g' | cut -d ',' -f1 | sed '/type/d'`
			SEOPATH=`echo $ROUND | grep 'seoPath":"' | sed -e 's!seoPath":"!\n"!g' | cut -d '"' -f2 | sed '/type/d'`
			PIC="https://aiswatchbox-a.akamaihd.net/watchbox/format/"$ENTITYID"_dvdcover/484x677/"$SEOPATH".jpg"
			NEWPAGE="/filme/"$SEOPATH"-"$ENTITYID".html"

			if [ -z  "$PIC" ]; then  
				PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
				TMPPIC="default.jpg"
			else
				TMPPIC=watchbox_`echo $PIC | tr '/' '\n' | tail -n1`
			fi

			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
				LINE="$TITLE#$SRC $SRC play 0 0 $NEWPAGE#$PIC#$TMPPIC#$NAME#111"
				echo "$LINE" >> $TMP/$PARSER.$INPUT.$CURPAGE.list
			fi
		done 3<$TMP/cache.$PARSER.$INPUT.2
		
		TOTALITEM=`cat $TMP/cache.$PARSER.$INPUT.2 | sed q | cut -d ',' -f1 | sed 's!:!\n!g' | sed '1,1d'`
		MODULO=`echo $(($TOTALITEM%30))`
		case "$MODULO" in
			[0]) MAXPAGE=`echo $(($TOTALITEM/30))`;;
			*) MAXPAGE=`echo $(($TOTALITEM/30+1))`;;
		esac

		if [ "$MAXPAGE" -gt "$CURPAGE" ] ; then
				NEWPAGE=`expr $CURPAGE + 1`
				PAGE=$(echo $PAGE | sed -e "s!page=$CURPAGE!page=$NEWPAGE!g")
				echo "Page ($NEWPAGE/$MAXPAGE)#$SRC $SRC search $NEWPAGE $MAXPAGE '$PAGE'#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.$CURPAGE.list
		fi

		rm $TMP/cache.$PARSER.$INPUT.* > /dev/null 2>&1
	fi

	echo "$TMP/$PARSER.$INPUT.$CURPAGE.list"
}
search2()
{
	if [ ! -e "$TMP/$PARSER.$INPUT.$CURPAGE.list" ] ; then
		$curlbin $PAGE -o $TMP/cache.$PARSER.$INPUT.1

		cat $TMP/cache.$PARSER.$INPUT.1 | sed 's!"type"!\n"type"!g' > $TMP/cache.$PARSER.$INPUT.2
		echo >> $TMP/cache.$PARSER.$INPUT.2
		while read -u 3 ROUND; do
			TITLE=`echo $ROUND | grep 'headline":"' | sed -e 's!headline":"!\n"!g' | cut -d '"' -f2 | sed '/type/d'`
			ENTITYID=`echo $ROUND | grep 'entityId":' | sed -e 's!entityId":!\n!g' | cut -d ',' -f1 | sed '/type/d'`
			SEOPATH=`echo $ROUND | grep 'seoPath":"' | sed -e 's!seoPath":"!\n"!g' | cut -d '"' -f2 | sed '/type/d'`
			PIC="https://aiswatchbox-a.akamaihd.net/watchbox/format/"$ENTITYID"_dvdcover/484x677/"$SEOPATH".jpg"
			NEWPAGE="/filme/"$SEOPATH"-"$ENTITYID".html"

			if [ -z  "$PIC" ]; then  
				PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
				TMPPIC="default.jpg"
			else
				TMPPIC=watchbox_`echo $PIC | tr '/' '\n' | tail -n1`
			fi

			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
				LINE="$TITLE#$SRC $SRC season 1 0 $NEWPAGE#$PIC#$TMPPIC#$NAME#0"
				echo "$LINE" >> $TMP/$PARSER.$INPUT.$CURPAGE.list
			fi
		done 3<$TMP/cache.$PARSER.$INPUT.2
		
		TOTALITEM=`cat $TMP/cache.$PARSER.$INPUT.2 | sed q | cut -d ',' -f1 | sed 's!:!\n!g' | sed '1,1d'`
		MODULO=`echo $(($TOTALITEM%30))`
		case "$MODULO" in
			[0]) MAXPAGE=`echo $(($TOTALITEM/30))`;;
			*) MAXPAGE=`echo $(($TOTALITEM/30+1))`;;
		esac
		
		if [ "$MAXPAGE" -gt "$CURPAGE" ] ; then
			
			NEWPAGE=`expr $CURPAGE + 1`
			PAGE=$(echo $PAGE | sed -e "s!page=$CURPAGE!page=$NEWPAGE!g")
			echo "Page ($NEWPAGE/$MAXPAGE)#$SRC $SRC search $NEWPAGE $MAXPAGE '$PAGE'#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.$CURPAGE.list
		fi

		rm $TMP/cache.$PARSER.$INPUT.* > /dev/null 2>&1
	fi

	echo "$TMP/$PARSER.$INPUT.$CURPAGE.list"
}

play()
{
		$curlbin $URL$PAGE -o $TMP/cache.$PARSER.$INPUT.1
		
        ID=`cat $TMP/cache.$PARSER.$INPUT.1 | grep 'videoId' | sed 's/,/\n/g' | grep videoId | cut -d ':' -f2`
		$curlbin "http://api.watchbox.de/devapi/id/$ID?apikey=hbbtv&format=json" -o $TMP/cache.$PARSER.$INPUT.2
		cat $TMP/cache.$PARSER.$INPUT.2 | sed 's/^.*"media_videourl":"//;s/".*$//;s/\\\//\//g' >$TMP/$PARSER.play.list
		echo >> $TMP/$PARSER.play.list
		cat $TMP/cache.$PARSER.$INPUT.2 | sed 's/^.*"media_videourl_wv":"//;s/".*$//;s/\\\//\//g' >>$TMP/$PARSER.play.list
		echo >> $TMP/$PARSER.play.list
		cat $TMP/cache.$PARSER.$INPUT.2 | sed 's/^.*"media_videourl_hls":"//;s/".*$//;s/\\\//\//g' >>$TMP/$PARSER.play.list
		echo >> $TMP/$PARSER.play.list

#{"command":"id","items":[{"vid":"422766","title":"JOHN CARPENTERS VAMPIRES: LOS MUERTOS","title_short":"john-carpenters-vampires-los-muertos","media_length":"5383","descr":"Mexiko. Eine einsame, verfallene Festung im Nirgendwo. Hier haben die Vampire, angeführt von der scheinbar unbesiegbaren Meisterin Una ihren Unterschlupf. Im weiten Umkreis erzittert das Land unter ihren Bluttaten. Gerade erst haben die untoten Killer ein ganzes Kloster ausgelöscht. Derek Bliss, Vampirjäger im Auftrag des Vatikans, und sein unerschrockenes Team stellen sich zwischen die Armee des Bösen und ihre Gier, Tag und Nacht gleichermaßen beherrschen zu wollen!","display_ascpect_ratio":"16:9","media_videourl_hls":"https:\/\/vodwbusohls.secure.footprint.net\/proxy\/manifest-wb-drm\/at-ch-de\/422766-1.ism\/fairplay.m3u8","media_videourl_wv":"https:\/\/vodwbusodash-a.akamaihd.net\/proxy\/manifest-wb-drm\/at-ch-de\/422766-1.ism\/.mpd","media_videourl":"","drm":true,"token":"eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpYXQiOjE1MjQwNjAwNzYsImlzcyI6Imh0dHBzOlwvXC9hcGkudHZub3cuZGVcL3dhdGNoYm94XC93ZWIiLCJhdWQiOiJvM2kydWhyb3doaWVmaXNlZmh1c2xpZWpmaGxrc2RoZmtqc2RmIiwidWlkIjo0MzU4NjE2OCwicm9sZXMiOlsidXNlciJdLCJwYWNrYWdlcyI6W10sImhhc0FjcXVpcmVkUGFja2FnZXMiOmZhbHNlLCJyaCI6IjczMzdlMDQzY2FiY2UyZmVmOGYyZDc2YTkyNjY2NjFjIiwiY2siOiI3NjVmZGE3YzViYzFhODg2NGQ5ZDZlYWI5MzdjNzViMyIsImNsaWVudElkIjoiYzY0YmQ1OTA3ODEzYTlmMzY2NmIxNGY0MGNhNzA1ODE2MGY1ODg0ZmUzNTk0YmU1NTE5NGIxM2MwNWU0YWZjNCIsInN1YnNjcmlwdGlvblN0YXRlIjozLCJyZXNpZGVuY2UiOm51bGwsInN1YnNjcmlwdGlvbiI6bnVsbH0.Th3X0vmQob_h0IUQfnQrXQZppAdDToYQyn-huXLxTbw","fsk":"16","poster":"http:\/\/aiswatchbox-a.akamaihd.net\/watchbox\/format\/15895_dvdcover\/john-carpenters-vampires-los-muertos.jpg","media_thumbnail":"http:\/\/aiswatchbox-a.akamaihd.net\/watchbox\/movie\/422766\/154x87\/image.jpg","media_content_thumbnail_large":"http:\/\/aiswatchbox-a.akamaihd.net\/watchbox\/movie\/422766\/john-carpenters-vampires-los-muertos.jpg","media_image_blur":"http:\/\/aiswatchbox-a.akamaihd.net\/watchbox\/movie\/422766\/20-k8\/3442\/john-carpenters-vampires-los-muertos.jpg","media_content_thumbnail_path":"http:\/\/aiswatchbox-a.akamaihd.net\/watchbox\/movie\/422766\/","special":{"id":"15895","title":"John Carpenters Vampires: Los Muertos","title_short":"John Carpenters Vampires: Los Muertos"},"special_ivw_tag":"watchbox_aio_hbbtv\/filme\/horror","videoplaza_share":"rtl_watchbox\/filme_action","videoplaza_cutlist":"900,1800,2700,3600","movie":{"vid":"422766","fsk":"16","rel_year":2002,"creationPlace":"USA","cast":"Arly Jover, Darius McCrary, Jon Bon Jovi, Natasha Gregson Wagner, Diego Luna, Cristián de la Fuente, Javier Grajeda","headline_movie":"JOHN CARPENTERS VAMPIRES: LOS MUERTOS","genre":[{"id":"32","genre":"Horror","image":"http:\/\/aiswatchbox-a.akamaihd.net\/watchbox\/cms\/cd1ded6546aca6c8a74241456947fdf7\/horror.jpg","head_genre":"32"},{"id":"4","genre":"Action","image":"http:\/\/aiswatchbox-a.akamaihd.net\/watchbox\/format\/15895\/john-carpenters-vampires-los-muertos.jpg","head_genre":"32"},{"id":"82","genre":"Trash","image":"http:\/\/aiswatchbox-a.akamaihd.net\/watchbox\/format\/15895\/john-carpenters-vampires-los-muertos.jpg","head_genre":"32"}],"image_url":"http:\/\/aiswatchbox-a.akamaihd.net\/watchbox\/movie\/422766\/john-carpenters-vampires-los-muertos.jpg","image_path":"http:\/\/aiswatchbox-a.akamaihd.net\/watchbox\/movie\/422766\/"},"agof_code":"dbrscaf_ten_wbfilmhorror","video_ivw_tag":"watchbox_aio_hbbtv\/videoabrufe\/filme\/horror","content_form":"long_form","geo_countries":"DE, AT, CH","geoblocking":"whitelist","link":"http:\/\/www.watchbox.de\/filme\/john-carpenters-vampires-los-muertos-15895.html","category":"film","currentTime":"21:04:56"}],"openSearch_totalResults":0,"openSearch_startIndex":1,"openSearch_itemsPerPage":10}
#		rm $TMP/cache.$PARSER.$INPUT.* > /dev/null 2>&1

#		cat "$TMP/$PARSER.play.list"
		echo "$TMP/$PARSER.play.list"

}

case $INPUT in
#	init) $INPUT;;
	mainmenu) $INPUT;;
	new) $INPUT;;
	series) $INPUT;;
	season) $INPUT;;
	episode) $INPUT;;
	play) $INPUT;;
	search) $INPUT;;
	search2) $INPUT;;
esac
