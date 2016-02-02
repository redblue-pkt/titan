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
URL=http://livetv.sx
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME=NBA-On-Demand

curlbin='curl -k -s -v -L --cookie /mnt/network/cookies --cookie-jar /mnt/network/cookies'
#useragent="Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Maxthon/4.4.7.3000 Chrome/30.0.1599.101 Safari/537.36"

wgetbin="wget -q -T2"
TMP=/tmp/parser
#TMP=/mnt/parser/tmp
TMP=/var/usr/local/share/titan/plugins/tithek/parser/tmp

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
	rm -f /mnt/network/cookies > /dev/null 2>&1
	rm -rf $TMP > /dev/null 2>&1
	echo "$NAME ($TYPE)#$SRC $SRC mainmenu#http://atemio.dyndns.tv/mediathek/menu/$PARSER.jpg#$PARSER.jpg#TiThek#0"
}

mainmenu()
{
	echo "Category#$SRC $SRC category#http://atemio.dyndns.tv/mediathek/menu/categoty.jpg#categoty.jpg#$NAME#0" > $TMP/$PARSER.$INPUT.list
#	echo "Suche#$SRC $SRC suche#http://atemio.dyndns.tv/mediathek/menu/search.jpg#giga.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "$TMP/$PARSER.$INPUT.list"
}

category()
{
	echo "Live#$SRC $SRC live $INPUT de#http://atemio.dyndns.tv/mediathek/menu/categoty.jpg#categoty.jpg#$NAME#0" > $TMP/$PARSER.$INPUT.list
	echo "Latest#$SRC $SRC videos $INPUT en/videotourney/3#http://atemio.dyndns.tv/mediathek/menu/categoty.jpg#categoty.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "Teams#$SRC $SRC submenu $INPUT en/leagueresults/3/#http://atemio.dyndns.tv/mediathek/menu/categoty.jpg#categoty.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
  	echo "$TMP/$PARSER.$INPUT.list"
}

videos()
{
	if [ ! -e "$TMP/$PARSER.$INPUT.$FROM.$FILENAME.list" ]; then
		piccount=0
		$wgetbin $URL/$PAGE -O $TMP/cache.$PARSER.$FROM.$FILENAME.1

		cat $TMP/cache.$PARSER.$FROM.$FILENAME.1 | tr '\n' ' ' | sed 's!height=27!\nfound=!g' | grep '^found=' >$TMP/cache.$PARSER.$FROM.$FILENAME.2

		while read -u 3 ROUND; do
			URL=`echo $ROUND | sed 's!<a href="/en/eventinfo/!\nurl="/en/eventinfo/!g' | grep ^url= | cut -d'"' -f2 | head -n1`
			TITLE=`echo $ROUND | sed 's!width="30%"> <b>!\ntitle=<!g' | grep ^title= | cut -d'<' -f2 | sed 's!&ndash;!-!g'`
			SCORE=`echo $ROUND | sed 's!<font color="#949494"><b>!\nextra=<!g' | grep ^extra= | cut -d'<' -f2 | sed 's!&ndash;!-!g'`			

			if [ `echo $ROUND | sed 's!href=!\nurl=!g' | grep ^url= | grep "Full match record" | cut -d'"' -f2 | head -n1 | wc -l` -eq 0 ];then
				FULLVIDEO=""
			else
				FULLVIDEO="Full"
			fi

			PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
			if [ "$FROM" = "latest" ];then
				EXTRA=`echo $ROUND | sed 's!data-pop="ddr_!\ndate=_!g' | grep ^date= | cut -d'_' -f2`
			else
				EXTRA=`echo $ROUND | sed 's!<span class="date">!\ndate=<!g' | grep ^date= | cut -d'<' -f2`
			fi

			if [ ! -z "$TITLE" ] && [ ! -z "$SCORE" ];then
				TITLE="$TITLE ($SCORE)"
			fi

			if [ ! -z "$TITLE" ] && [ ! -z "$EXTRA" ];then
				TITLE="$TITLE ($EXTRA)"
			fi

			if [ ! -z "$TITLE" ] && [ ! -z "$FULLVIDEO" ];then
				TITLE="$TITLE ($FULLVIDEO)"
			fi

			if [ ! -z "$TITLE" ] && [ ! -z "$URL" ] && [ "$URL" != "http://xlivetv.sx" ] && [ `cat $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list | grep "#$URL#" | wc -l` -eq 0 ];then
				if [ ! -e $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list ];then
					touch $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list
				fi
				piccount=`expr $piccount + 1`

				URL="$SRC $SRC play $INPUT $URL"

				LINE="$TITLE#$URL#$PIC#$PARSER_$piccount.jpg#$NAME#0"
				echo "$LINE" >> $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list
			fi
	
		done 3<$TMP/cache.$PARSER.$FROM.$FILENAME.2
		rm $TMP/cache.* > /dev/null 2>&1
	fi
	echo "$TMP/$PARSER.$INPUT.$FROM.$FILENAME.list"
}

play()
{
	if [ ! -e "$TMP/$PARSER.$INPUT.$FROM.$FILENAME.list" ]; then
		piccount=0
		$wgetbin $URL/$PAGE -O $TMP/cache.$PARSER.$FROM.$FILENAME.1

		cat $TMP/cache.$PARSER.$FROM.$FILENAME.1 | tr '\n' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!<td width="33%">!\nfound=!g' | sed 's!<br> </td>!\n<br> </td>!g' | grep '^found=' >$TMP/cache.$PARSER.$FROM.$FILENAME.2
		while read -u 3 ROUND; do

			URL=http://xlivetv.sx`echo $ROUND | sed 's!href=!\nurl=!g' | grep ^url= | cut -d'"' -f2 | head -n1`
			TITLE=`echo $ROUND | sed 's!<a alt="!\ntitle="!g' | grep ^title= | cut -d'"' -f2 | sed 's!&ndash;!-!g'`

			PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
			if [ ! -z "$TITLE" ] && [ ! -z "$URL" ] && [ "$URL" != "http://xlivetv.sx" ] && [ `cat $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list | grep "#$URL#" | wc -l` -eq 0 ];then
				if [ ! -e $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list ];then
					touch $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list
				fi

				piccount=`expr $piccount + 1`
				LINE="$TITLE#$URL#$PIC#$PARSER_$piccount.jpg#$NAME#98"
				echo "$LINE" >> $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list
			fi
	
		done 3<$TMP/cache.$PARSER.$input.$FILENAME.2
		rm $TMP/cache.* > /dev/null 2>&1
	fi
	echo "$TMP/$PARSER.$INPUT.$FROM.$FILENAME.list"
}

submenu()
{
	if [ ! -e "$TMP/$PARSER.$INPUT.$FROM.$FILENAME.list" ]; then
		piccount=0

		$wgetbin $URL/$PAGE -O $TMP/cache.$PARSER.$FROM.$FILENAME.1
#		cat $TMP/cache.$PARSER.$FROM.$FILENAME.1 | sed 's!<a href="/en/team/!\n<a href="/en/team/!g' | grep '^<a href="/en/team/' | tr ' ' '~' >$TMP/cache.$PARSER.$FROM.$FILENAME.2
		cat $TMP/cache.$PARSER.$FROM.$FILENAME.1 | sed 's!<a href="/en/team/!\n<a href="/en/team/!g' | grep '^<a href="/en/team/' >$TMP/cache.$PARSER.$FROM.$FILENAME.2

		while read -u 3 ROUND; do
			URL="$SRC $SRC videos $INPUT `echo $ROUND | cut -d'"' -f2 | sed 's!/calendar/!/video/!g'`"
			TITLE=`echo $ROUND | sed 's!<b>!\ntitle=<!g' | grep ^title= | cut -d'<' -f2`
			PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
	
			if [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
				if [ ! -e $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list ];then
					touch $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list
				fi
				piccount=$[$piccount+1]
				LINE="$TITLE#$URL#$PIC#$PARSER_$piccount.jpg#$NAME#0"
				echo "$LINE" >> $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list
			fi
	
		done 3<$TMP/cache.$PARSER.$FROM.$FILENAME.2
		rm $TMP/cache.* > /dev/null 2>&1
	fi
	echo "$TMP/$PARSER.$INPUT.$FROM.$FILENAME.list"
}

live()
{
	rm $TMP/cache.* > /dev/null 2>&1
	rm $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list > /dev/null 2>&1

	if [ ! -e "$TMP/$PARSER.$INPUT.$FROM.$FILENAME.list" ]; then
		piccount=0
		$wgetbin $URL/$PAGE -O $TMP/cache.$PARSER.$FROM.$FILENAME.1

#		zcat $TMP/cache.$PARSER.$FROM.$FILENAME.1 | tr '\n' ' ' | sed 's!<img width=27!\nfound=!g' | grep '^found=' | grep nba.gif | grep -v nhl >$TMP/cache.$PARSER.$FROM.$FILENAME.2
		zcat $TMP/cache.$PARSER.$FROM.$FILENAME.1 | tr '\n' ' ' | sed 's!<img width=27!\nfound=!g' | grep '^found=' >$TMP/cache.$PARSER.$FROM.$FILENAME.2

		while read -u 3 ROUND; do
			URL=`echo $ROUND | sed 's!href=!\nurl=!g' | grep ^url= | cut -d'"' -f2 | head -n1`
			TITLE=`echo $ROUND | sed "s!$URL\">!\ntitle=<!g" | grep ^title= | cut -d'<' -f2 | sed 's!&ndash;!-!g'`
			SCORE=`echo $ROUND | sed 's!&nbsp; !\nscore=<!g' | grep ^score= | cut -d'<' -f2 | sed 's!&ndash;!-!g'`			
			EXTRA=`echo $ROUND | sed 's!<span class="evdesc">!\nextra=<!g' | grep ^extra= | cut -d'<' -f2 | sed 's!&ndash;!-!g'`			

			PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"

			if [ ! -z "$TITLE" ] && [ ! -z "$SCORE" ];then
				TITLE="$TITLE ($SCORE)"
			fi

			if [ ! -z "$TITLE" ] && [ ! -z "$EXTRA" ];then
				TITLE="$TITLE $EXTRA"
			fi

			TITLE=`echo $TITLE | sed 's/&nbsp;//g'`
			TITLE=`echo $TITLE | sed 's/&ndash;/-/g'`
			TITLE=`echo $TITLE | sed 's/\n//g'`

			if [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
				if [ ! -e $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list ];then
					touch $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list
				fi

				piccount=`expr $piccount + 1`

				URL="$SRC $SRC hosterlist $INPUT '$URL'"

				LINE="$TITLE#$URL#$PIC#$PARSER_$piccount.jpg#$NAME#0"
				echo "$LINE" >> $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list
			fi
	
		done 3<$TMP/cache.$PARSER.$FROM.$FILENAME.2
#		rm $TMP/cache.* > /dev/null 2>&1
	fi
	echo "$TMP/$PARSER.$INPUT.$FROM.$FILENAME.list"
}

hosterlist()
{
	rm $TMP/cache.* > /dev/null 2>&1
	rm $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list > /dev/null 2>&1

	if [ ! -e "$TMP/$PARSER.$INPUT.$FROM.$FILENAME.list" ]; then
		piccount=0
		$curlbin "$URL/$PAGE" -o $TMP/cache.$PARSER.$FROM.$FILENAME.1
		
		cat $TMP/cache.$PARSER.$FROM.$FILENAME.1 | tr '\n' ' ' | sed "s!show_webplayer('!\nfound=('!g" | sed 's!src="!\nsrc="!g' | grep '^found=' >$TMP/cache.$PARSER.$FROM.$FILENAME.2

		while read -u 3 ROUND; do
			URL=`echo $ROUND | sed 's!href=!\nurl=!g' | grep ^url= | cut -d'"' -f2 | head -n1 | sed 's!/webplayer.php!/export/webplayer.iframe.php!'`
			TITLE=`echo $ROUND | cut -d"'" -f2`
			EXTRA=`echo $ROUND | cut -d"'" -f4`			

			PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"

			if [ ! -z "$TITLE" ] && [ ! -z "$EXTRA" ];then
				TITLE="$TITLE ($EXTRA)"
			fi

			if [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
				if [ ! -e $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list ];then
					touch $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list
				fi
				piccount=`expr $piccount + 1`

				URL="$SRC $SRC hoster $INPUT '$URL'"
#				URL="$SRC $SRC findhoster $FROM '$URL'"

				LINE="$TITLE#$URL#$PIC#$PARSER_$piccount.jpg#$NAME#111"
				echo "$LINE" >> $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list
			fi
	
		done 3<$TMP/cache.$PARSER.$FROM.$FILENAME.2
#		rm $TMP/cache.* > /dev/null 2>&1
	fi
	echo "$TMP/$PARSER.$INPUT.$FROM.$FILENAME.list"
}
################
#			<script type="text/javascript" language="JavaScript" src="http://hqstream.tv/pl?streampage=dsgdfhdffd26&width=700&height=480"></script>
#
#			get http://hqstream.tv/pl?streampage=dsgdfhdffd26&width=700&height=480
#			document.write("<iframe FRAMEBORDER=0 MARGINWIDTH=0 MARGINHEIGHT=0 SCROLLING=NO src=\"http://hqstream.tv/player.php?streampage=dsgdfhdffd26&height=480&width=700\" height=\"480\" width=\"700\"\></iframe>");
#
################
#			<iframe width="700" height="480" src="http://www.youtube.com/embed/PjF7_8_R9q8?autoplay=1" frameborder="0" allowfullscreen></iframe>

#			<script type="text/javascript" language="JavaScript" src="http://hqstream.tv/pl?streampage=dsgdfhdffd26&width=700&height=480"></script>
#				document.write("<iframe style='margin-top: 5px;' width=700 height=16 marginwidth=0 marginheight=0 hspace=0 vspace=0 frameborder=0 scrolling=no src='http://cdn.livetvcdn.net/cache/links/de.381782.html?" + Math.floor(new Date().getTime() / 100000) + "'></iframe>");

#		<iframe scrolling="no" frameborder=0 width=700 height=480 src="https://fsport.in/broadcasts/la_ua_2.php"></iframe>

#		<iframe scrolling="no" frameborder=0 width=700 height=480 src="http://www.laola1.tv/titanplayer.php?videoid=462888&type=S"></iframe>

#		<iframe scrolling="no" frameborder=0 width=700 height=480 src="http://tuttosportweb.com/update/ch3.html"></iframe>

#			<IFRAME FRAMEBORDER=0 MARGINWIDTH=0 MARGINHEIGHT=0 SCROLLING=NO align="center" WIDTH=700 HEIGHT=480 SRC="http://leton.tv/player.php?streampage=sdfewfwe26&width=700&height=480"></IFRAME>

#		<iframe scrolling="no" frameborder=0 width=700 height=480 src="http://www.ehftv.com//player.php?play=467026&type=S&status=4&geo=int"></iframe>

#		<iframe scrolling="no" frameborder=0 width=700 height=480 src="http://fsport.in/broadcasts/stream_19.php"></iframe>
#		<iframe scrolling="no" frameborder=0 width=700 height=480 src="http://shstream.co/stream5/live5.php"></iframe>


hoster()
{
	rm $TMP/cache.* > /dev/null 2>&1

	$curlbin -o $TMP/cache.$PARSER.$INPUT.$FROM.1 ${PAGE}
	URL=`zcat $TMP/cache.$PARSER.$INPUT.$FROM.1 | grep iframe | sed -nr 's/.*src="([^"]+)".*/\1/p'`
	if [ -z "$URL" ];then
		URL=`cat $TMP/cache.$PARSER.$INPUT.$FROM.1 | grep iframe | sed -nr 's/.*src="([^"]+)".*/\1/p'`
	fi
	if [ -z "$URL" ];then
		URL=`cat $TMP/cache.$PARSER.$INPUT.$FROM.1 | grep "text/javascript" | grep -v jQuery | sed -nr 's/.*src="([^"]+)".*/\1/p'`
	fi
	
	echo $URL
}

hoster1()
{
rm $TMP/cache.* > /dev/null 2>&1
rm $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list > /dev/null 2>&1

	if [ ! -e "$TMP/$PARSER.$INPUT.$FROM.$FILENAME.list" ]; then
		piccount=0
		FILENAME=GET
		$wgetbin "$PAGE" -O $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.1

#		zcat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.1 > $TMP/cache.$PARSER.$FROM.$FILENAME.2
		cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.1 | tr '\n' ' ' | sed "s!<iframe scrolling!\nfound=!g" | grep '^found=' >$TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.2
		zcat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.1 | tr '\n' ' ' | sed "s!<iframe scrolling!\nfound=!g" | grep '^found=' >$TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.3

		URL=`cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.2 | sed "s!src=!\nsrc=!g" | grep '^src=' | cut -d'"' -f2`
		URL2=`cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.3 | sed "s!src=!\nsrc=!g" | grep '^src=' | cut -d'"' -f2`

#		URL2=`cat $TMP/cache.$PARSER.$FROM.$FILENAME.1 | tr '\n' ' ' | sed "s!<iframe scrolling=!\nfound=!g" | grep '^found=' | cut -d'"' -f4`

echo URL$URL
exit
echo URL2 $URL2
exit
		$wgetbin "$URL2" -O $TMP/cache.$PARSER.$FROM.$FILENAME.2
#		echo URL2 $URL2
#domain=www.streamhd.eu

		found=`cat $TMP/cache.$PARSER.$FROM.$FILENAME.2 | sed 's!fid=!\nfid=!g' | grep '^fid='`
		fid=`echo $found | cut -d'"' -f2`
		width=640
		height=400
		URL3=`echo $found | cut -d'"' -f6`
		$wgetbin "$URL3" -O $TMP/cache.$PARSER.$FROM.$FILENAME.3
		found=`cat $TMP/cache.$PARSER.$FROM.$FILENAME.3`
		URL4=`echo $found | cut -d'"' -f2`
		URL4=`echo $URL4 | sed "s/'+ fid +'/$fid/"`

		URL4=`echo $URL4 | sed "s/'+v_width+'/$width/"`
		URL4=`echo $URL4 | sed "s/'+v_height+'/$height/"`
		URL4=`echo $URL4 | sed "s/'+document.domain+'/$domain/"`
		$curlbin --referer $URL2 "$URL4" -o $TMP/cache.$PARSER.$FROM.$FILENAME.4

		streamer=`cat $TMP/cache.$PARSER.$FROM.$FILENAME.4 | sed 's!var video =!\nfound=!g' | grep '^found=' | cut -d'"' -f2`
		swfurl=`cat $TMP/cache.$PARSER.$FROM.$FILENAME.4 | sed 's!flashplayer:!\nfound=!g' | grep '^found=' | cut -d'"' -f2`
		app=`echo $streamer | sed 's!/app/!\napp=!g' | grep '^app='`

		LINE="RTMP STREAM#$streamer $app swfUrl=$swfurl live=1 timeout=15 swfVfy=1 pageUrl=$URL2#pic#$PARSER_$piccount.jpg#$NAME#2"
		echo "$LINE" >> $TMP/$PARSER.RTMPSTREAM.list
	
	fi
	echo "$TMP/$PARSER.RTMPSTREAM.list"
}


case $INPUT in
	init) $INPUT;;
	mainmenu) $INPUT;;
	category) $INPUT;;
	live) $INPUT;;
	hosterlist) $INPUT;;
	hoster) $INPUT;;
	videos) $INPUT;;
	play) $INPUT;;
	submenu) $INPUT;;
esac