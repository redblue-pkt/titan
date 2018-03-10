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

URL=https://www.watchbox.de
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME="Watchbox"

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
	echo "Neue Filme#$SRC $SRC new 1 0 'filme/neu/'#http://atemio.dyndns.tv/mediathek/menu/Movies.jpg#Movies.jpg#$NAME#0" > $TMP/$PARSER.$INPUT.list
#	echo "Neue Serien#$SRC $SRC new 1 0 'serien/neu/'#http://atemio.dyndns.tv/mediathek/menu/serien.jpg#serien.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "Filme suchen#$SRC $SRC search 1 0 'https://api.watchbox.de/v1/search/?active=true&maxPerPage=30&page=1&types=%5B%22film%22%5D&term='#http://atemio.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >> $TMP/$PARSER.$INPUT.list

	if [ "`echo $TMP/$PARSER.new.*.list`" != "$TMP/$PARSER.new.*.list" ] ; then
		rm $TMP/$PARSER.new.*.list
	fi
	if [ "`echo $TMP/$PARSER.search.*.list`" != "$TMP/$PARSER.search.*.list" ] ; then
		rm $TMP/$PARSER.search.*.list
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
				PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
				TMPPIC="default.jpg"
			else
				TMPPIC=watchbox_`echo $PIC | tr '/' '\n' | tail -n1`
			fi

			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
				if [ `cat $TMP/$PARSER.$INPUT.list | grep ^"$NEWPAGE" | wc -l` -eq 0 ];then
					if [ ! -e $TMP/$PARSER.$INPUT.$CURPAGE.list ];then
						touch $TMP/$PARSER.$INPUT.$CURPAGE.list
					fi
					LINE="$TITLE#$SRC $SRC play 0 0 $NEWPAGE#$PIC#$TMPPIC#$NAME#111"
					echo "$LINE" >> $TMP/$PARSER.$INPUT.$CURPAGE.list
				fi
			fi
		done 3<$TMP/cache.$PARSER.$INPUT.$CURPAGE.2
		
		if grep -Fq "<span>Zeig mir mehr</span>" $TMP/cache.$PARSER.$INPUT.$CURPAGE.1
		then
		
		NEWPAGE=`expr $CURPAGE + 1`
		
		echo "Page $NEWPAGE#$SRC $SRC new $NEWPAGE $MAXPAGE '$PAGE'#http://atemio.dyndns.tv/mediathek/menu/next.jpg#next.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.$CURPAGE.list
		fi
		

		rm $TMP/cache.$PARSER.$INPUT.CURPAGE.* > /dev/null 2>&1
	fi

	echo "$TMP/$PARSER.$INPUT.$CURPAGE.list"
}

search()
{
	if [ ! -e "$TMP/$PARSER.$INPUT.$CURPAGE.list" ] ; then
#		if [ "$CURPAGE" -eq "1" ] ; then
#			NEWPAGE=$PAGE
#		else
#			NEWPAGE=`echo $PAGE | sed "s/@PAGE@/$CURPAGE/g"`
#		fi
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
 					LINE="$TITLE#$SRC $SRC play 0 0 $NEWPAGE#$PIC#$TMPPIC#$NAME#111"
					echo "$LINE" >> $TMP/$PARSER.$INPUT.$CURPAGE.list
				fi
			fi
		done 3<$TMP/cache.$PARSER.$INPUT.2
		
		TOTALITEM=`cat $TMP/cache.$PARSER.$INPUT.2 | sed q | cut -d ',' -f1 | sed 's!:!\n!g' | sed '1,1d'`
		MAXPAGE=`echo $(($TOTALITEM/30+1))`
		
		if [ "$MAXPAGE" -gt "$CURPAGE" ] ; then
			NEWPAGE=`expr $CURPAGE + 1`
			PAGE=$(echo $PAGE | sed -e "s!page=$CURPAGE!page=$NEWPAGE!g")
			echo "Page ($NEWPAGE/$MAXPAGE)#$SRC $SRC search $NEWPAGE $MAXPAGE '$PAGE'#http://atemio.dyndns.tv/mediathek/menu/next.jpg#next.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.$CURPAGE.list
                fi

		rm $TMP/cache.$PARSER.$INPUT.* > /dev/null 2>&1
	fi

	echo "$TMP/$PARSER.$INPUT.$CURPAGE.list"
}

play()
{
	$curlbin $URL$PAGE -o $TMP/cache.$PARSER.$INPUT.1

	ID=`cat $TMP/cache.$PARSER.$INPUT.1 | sed '/class="big-teaser-video js-bigteaser"/,/class="big-teaser-video__videoplayer"/!d;/data-movie-id=/!d;s/^.*data-movie-id="//;s/".*$//'`
	$curlbin "http://api.watchbox.de/devapi/id/$ID?apikey=hbbtv&format=json" -o $TMP/cache.$PARSER.$INPUT.2
	cat $TMP/cache.$PARSER.$INPUT.2 | sed 's/^.*"media_videourl":"//;s/".*$//;s/\\\//\//g' >$TMP/cache.$PARSER.$INPUT.3
	cat $TMP/cache.$PARSER.$INPUT.3 > $TMP/$PARSER.play.list
	echo >> $TMP/$PARSER.play.list
#	rm $TMP/cache.$PARSER.$INPUT.* > /dev/null 2>&1

	cat "$TMP/$PARSER.play.list"
#	echo "$TMP/$PARSER.play.list"
}

case $INPUT in
	init) $INPUT;;
	mainmenu) $INPUT;;
	new) $INPUT;;
	search) $INPUT;;
	play) $INPUT;;
esac
