#!/bin/sh
# first sh box parser for titannit mfg obi

. /tmp/localhoster/hoster.sh

SRC=$1
INPUT=$2
FROM=$3
PAGE=$4
FILENAME=`echo $PAGE | tr '/' '.'`
if [ -z "$PAGE" ]; then
	FILENAME=none
fi

URL=http://livetv.sx
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME=Sports-On-Demand

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
	if [ -e /etc/.beta ];then
		echo "Live Sports#$SRC $SRC livelist live de#http://atemio.dyndns.tv/mediathek/menu/livesports.jpg#livesports.jpg#$NAME#0" > $TMP/$PARSER.$INPUT.list
		echo "Basketball#$SRC $SRC basketball#http://atemio.dyndns.tv/mediathek/menu/basketball.jpg#basketball.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	else
		echo "Basketball#$SRC $SRC basketball#http://atemio.dyndns.tv/mediathek/menu/basketball.jpg#basketball.jpg#$NAME#0" > $TMP/$PARSER.$INPUT.list
	fi
	echo "Ice Hockey#$SRC $SRC icehockey#http://atemio.dyndns.tv/mediathek/menu/icehockey.jpg#icehockey.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "Fussball#$SRC $SRC fussball#http://atemio.dyndns.tv/mediathek/menu/fussball.jpg#fussball.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "Handball#$SRC $SRC handball#http://atemio.dyndns.tv/mediathek/menu/handball.jpg#handball.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "Football#$SRC $SRC football#http://atemio.dyndns.tv/mediathek/menu/football.jpg#football.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list

	echo "$TMP/$PARSER.$INPUT.list"
} 

basketball()
{
	echo "NBA (Latest)#$SRC $SRC videos latest enx/videotourney/3#http://atemio.dyndns.tv/mediathek/menu/nba.latest.jpg#nba.latest.jpg#$NAME#0" > $TMP/$PARSER.$INPUT.list
	echo "NBA (Teams)#$SRC $SRC submenu teams enx/leagueresults/3/#http://atemio.dyndns.tv/mediathek/menu/nba.teams.jpg#nba.teams.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "NBA (Sorted Date)#$SRC $SRC year teams enx/leagueresults/3/#http://atemio.dyndns.tv/mediathek/menu/nba.date.jpg#nba.date.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list

	echo "German Bundesliga (Latest)#$SRC $SRC videos latest enx/videotourney/421#http://atemio.dyndns.tv/mediathek/menu/german.bundesliga.latest.jpg#german.bundesliga.latest.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "German Bundesliga (Teams)#$SRC $SRC submenu teams enx/leagueresults/421/#http://atemio.dyndns.tv/mediathek/menu/german.bundesliga.teams.jpg#german.bundesliga.teams.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "German Bundesliga (Sorted Date)#$SRC $SRC year teams enx/leagueresults/421/#http://atemio.dyndns.tv/mediathek/menu/german.bundesliga.date.jpg#german.bundesliga.date.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list

	echo "Italian Lega Basket (Latest)#$SRC $SRC videos latest enx/videotourney/668#http://atemio.dyndns.tv/mediathek/menu/italian.lega.basket.latest.jpg#italian.lega.basket.latest.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "Italian Lega Basket (Teams)#$SRC $SRC submenu teams enx/leagueresults/668/#http://atemio.dyndns.tv/mediathek/menu/italian.lega.basket.teams.jpg#italian.lega.basket.teams.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "Italian Lega Basket (Sorted Date)#$SRC $SRC year teams enx/leagueresults/668/#http://atemio.dyndns.tv/mediathek/menu/italian.lega.basket.date.jpg#italian.lega.basket.date.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list

	echo "European Championship (Latest)#$SRC $SRC videos latest enx/videotourney/282#http://atemio.dyndns.tv/mediathek/menu/european.championship.latest.jpg#european.championship.latest.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "European Championship (Teams)#$SRC $SRC submenu teams enx/leagueresults/282/#http://atemio.dyndns.tv/mediathek/menu/european.championship.teams.jpg#european.championship.teams.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "European Championship (Sorted Date)#$SRC $SRC year teams enx/leagueresults/282/#http://atemio.dyndns.tv/mediathek/menu/european.championship.date.jpg#european.championship.date.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list

	echo "Euroleague (Latest)#$SRC $SRC videos latest enx/videotourney/41#http://atemio.dyndns.tv/mediathek/menu/euroleague.latest.jpg#euroleague.latest.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "Euroleague (Teams)#$SRC $SRC submenu teams enx/leagueresults/41/#http://atemio.dyndns.tv/mediathek/menu/euroleague.teams.jpg#euroleague.teams.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "Euroleague (Sorted Date)#$SRC $SRC year teams enx/leagueresults/41/#http://atemio.dyndns.tv/mediathek/menu/euroleague.date.jpg#euroleague.date.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list

  	echo "$TMP/$PARSER.$INPUT.list"
}

football()
{
	echo "NFL (Latest)#$SRC $SRC videos latest enx/videotourney/142#http://atemio.dyndns.tv/mediathek/menu/nfl.latest.jpg#nfl.latest.jpg#$NAME#0" > $TMP/$PARSER.$INPUT.list
	echo "NFL (Teams)#$SRC $SRC submenu teams enx/leagueresults/142/#http://atemio.dyndns.tv/mediathek/menu/nfl.teams.jpg#nfl.teams.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "NFL (Sorted Date)#$SRC $SRC year teams enx/leagueresults/142/#http://atemio.dyndns.tv/mediathek/menu/nfl.date.jpg#nfl.date.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
  	echo "$TMP/$PARSER.$INPUT.list"
}


handball()
{
	echo "Women. Champions League (Latest)#$SRC $SRC videos latest enx/videotourney/1249#http://atemio.dyndns.tv/mediathek/menu/women.champions.league.latest.jpg#women.champions.league.latest.jpg#$NAME#0" > $TMP/$PARSER.$INPUT.list
	echo "Women. Champions League (Teams)#$SRC $SRC submenu teams enx/leagueresults/1249/#http://atemio.dyndns.tv/mediathek/menu/women.champions.league.teams.jpg#women.champions.league.teams.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "Women. Champions League (Sorted Date)#$SRC $SRC year teams enx/leagueresults/1249/#http://atemio.dyndns.tv/mediathek/menu/women.champions.league.date.jpg#women.champions.league.date.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
  	echo "$TMP/$PARSER.$INPUT.list"
}

icehockey()
{
	echo "NHL (Latest)#$SRC $SRC videos latest enx/videotourney/2#http://atemio.dyndns.tv/mediathek/menu/nhl.latest.jpg#nhl.latest.jpg#$NAME#0" > $TMP/$PARSER.$INPUT.list
	echo "NHL (Teams)#$SRC $SRC submenu teams enx/leagueresults/2/#http://atemio.dyndns.tv/mediathek/menu/nhl.teams.jpg#nhl.teams.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "NHL (Sorted Date)#$SRC $SRC year teams enx/leagueresults/2/#http://atemio.dyndns.tv/mediathek/menu/nhl.date.jpg#nhl.date.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list

	echo "Champions League (Latest)#$SRC $SRC videos latest enx/videotourney/191#http://atemio.dyndns.tv/mediathek/menu/champions.league.latest.jpg#champions.league.latest.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "Champions League (Teams)#$SRC $SRC submenu teams enx/leagueresults/191/#http://atemio.dyndns.tv/mediathek/menu/champions.league.teams.jpg#champions.league.teams.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "Champions League (Sorted Date)#$SRC $SRC year teams enx/leagueresults/191/#http://atemio.dyndns.tv/mediathek/menu/champions.league.date.jpg#champions.league.date.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list

  	echo "$TMP/$PARSER.$INPUT.list"
}

fussball()
{
	echo "1. Bundesliga (Latest)#$SRC $SRC videos latest enx/videotourney/36#http://atemio.dyndns.tv/mediathek/menu/dfb1.latest.jpg#dfb1.latest.jpg#$NAME#0" > $TMP/$PARSER.$INPUT.list
	echo "1. Bundesliga (Teams)#$SRC $SRC submenu teams enx/leagueresults/36/#http://atemio.dyndns.tv/mediathek/menu/dfb1.teams.jpg#dfb1.teams.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "1. Bundesliga (Sorted Date)#$SRC $SRC year teams enx/leagueresults/36/#http://atemio.dyndns.tv/mediathek/menu/dfb1.date.jpg#dfb1.date.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list

	echo "2. Bundesliga (Latest)#$SRC $SRC videos latest enx/videotourney/31#http://atemio.dyndns.tv/mediathek/menu/dfb2.latest.jpg#dfb2.latest.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "2. Bundesliga (Teams)#$SRC $SRC submenu teams enx/leagueresults/31/#http://atemio.dyndns.tv/mediathek/menu/dfb2.teams.jpg#dfb2.teams.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "2. Bundesliga (Sorted Date)#$SRC $SRC year teams enx/leagueresults/31/#http://atemio.dyndns.tv/mediathek/menu/dfb2.date.jpg#dfb2.date.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list

	echo "3. Bundesliga (Latest)#$SRC $SRC videos latest enx/videotourney/135#http://atemio.dyndns.tv/mediathek/menu/dfb3.latest.jpg#dfb3.latest.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "3. Bundesliga (Teams)#$SRC $SRC submenu teams enx/leagueresults/135/#http://atemio.dyndns.tv/mediathek/menu/dfb3.teams.jpg#dfb3.teams.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "3. Bundesliga (Sorted Date)#$SRC $SRC year teams enx/leagueresults/135/#http://atemio.dyndns.tv/mediathek/menu/dfb3.date.jpg#dfb3.date.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list

	echo "Italian Serie A (Latest)#$SRC $SRC videos latest enx/videotourney/11#http://atemio.dyndns.tv/mediathek/menu/isa.latest.jpg#isa.latest.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "Italian Serie A (Teams)#$SRC $SRC submenu teams enx/leagueresults/11/#http://atemio.dyndns.tv/mediathek/menu/isa.teams.jpg#isa.teams.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "Italian Serie A (Sorted Date)#$SRC $SRC year teams enx/leagueresults/11/#http://atemio.dyndns.tv/mediathek/menu/isa.date.jpg#isa.date.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list

	echo "English Premier League (Latest)#$SRC $SRC videos latest enx/videotourney/1#http://atemio.dyndns.tv/mediathek/menu/epl.latest.jpg#epl.latest.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "English Premier League (Teams)#$SRC $SRC submenu teams enx/leagueresults/1/#http://atemio.dyndns.tv/mediathek/menu/epl.teams.jpg#epl.teams.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "English Premier League (Sorted Date)#$SRC $SRC year teams enx/leagueresults/1/#http://atemio.dyndns.tv/mediathek/menu/epl.date.jpg#epl.date.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list

	echo "Europa League (Latest)#$SRC $SRC videos latest enx/videotourney/265#http://atemio.dyndns.tv/mediathek/menu/el.latest.jpg#el.latest.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "Europa League (Teams)#$SRC $SRC submenu teams enx/leagueresults/265/#http://atemio.dyndns.tv/mediathek/menu/el.teams.jpg#el.teams.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "Europa League (Sorted Date)#$SRC $SRC year teams enx/leagueresults/265/#http://atemio.dyndns.tv/mediathek/menu/el.date.jpg#el.date.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list

	echo "Champions League (Latest)#$SRC $SRC videos latest enx/videotourney/7#http://atemio.dyndns.tv/mediathek/menu/champions.league.latest.jpg#champions.league.latest.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "Champions League (Teams)#$SRC $SRC submenu teams enx/leagueresults/7/#http://atemio.dyndns.tv/mediathek/menu/champions.league.teams.jpg#champions.league.teams.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "Champions League (Sorted Date)#$SRC $SRC year teams enx/leagueresults/7/#http://atemio.dyndns.tv/mediathek/menu/champions.league.date.jpg#champions.league.date.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list

  	echo "$TMP/$PARSER.$INPUT.list"
}

year()
{
	rm $TMP/$PARSER.$INPUT.list > /dev/null 2>&1

	if [ ! -e "/tmp/tithek/$PARSER.$INPUT.list" ]; then
		tags=""
		tagcount=`date +"%Y"`
		i=2008
		until [ "$i" -gt "$tagcount" ]
		do
		echo "$i#$SRC $SRC month year enx/videotourney/3/$i#http://atemio.dyndns.tv/mediathek/menu/$i.jpg#$i.jpg#$NAME#0" | sort -r >> $TMP/$PARSER.$INPUT.list
		i=`expr $i + 1`
		done
	fi

  	echo "$TMP/$PARSER.$INPUT.list"
}

month()
{
	rm $TMP/$PARSER.$INPUT.list > /dev/null 2>&1

	if [ ! -e "/tmp/tithek/$PARSER.$INPUT.list" ]; then
		tags=""
		usedyear=`echo $PAGE | tr '/' '\n' | tail -n1`
		curyear=`date +"%Y"`
		if [ "$usedyear" == "$curyear" ];then
			tagcount=`date +"%m"`
		else
			tagcount=12
		fi
		i=1
		until [ "$i" -gt "$tagcount" ]
		do
		echo "$i / $usedyear#$SRC $SRC videos latest $PAGE$i#http://atemio.dyndns.tv/mediathek/menu/$i.jpg#$i.jpg#$NAME#0" | sort -r >> $TMP/$PARSER.$INPUT.list
		i=`expr $i + 1`
		done
	fi

  	echo "$TMP/$PARSER.$INPUT.list"
}

videos()
{
	if [ ! -e "$TMP/$PARSER.$INPUT.$FROM.$FILENAME.list" ]; then
		piccount=0
		$wgetbin $URL/$PAGE -O $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.1

		cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.1 | tr '\n' ' ' | sed 's!height=27!\nfound=!g' | grep '^found=' >$TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.2

		while read -u 3 ROUND; do
			URL=`echo $ROUND | sed 's!<a href="/enx/eventinfo/!\nurl="/enx/eventinfo/!g' | grep ^url= | cut -d'"' -f2 | head -n1`
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

				URL="$SRC $SRC playsrc $INPUT $URL"

				LINE="$TITLE#$URL#$PIC#$PARSER_$piccount.jpg#$NAME#0"
				echo "$LINE" >> $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list
			fi

		done 3<$TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.2
		rm $TMP/cache.* > /dev/null 2>&1
	fi
	echo "$TMP/$PARSER.$INPUT.$FROM.$FILENAME.list"
}

playsrc()
{
	if [ ! -e "$TMP/$PARSER.$INPUT.$FROM.$FILENAME.list" ]; then
		piccount=0
#		$wgetbin $URL/$PAGE -O $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.1
		$curlbin $URL/$PAGE -o $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.1

		cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.1 | tr '\n' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!<td width="33%">!\nfound=!g' | sed 's!<br> </td>!\n<br> </td>!g' | grep '^found=' >$TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.2

		while read -u 3 ROUND; do
#			URL=http://xlivetv.sx`echo $ROUND | sed 's!href=!\nurl=!g' | grep ^url= | cut -d'"' -f2 | head -n1`
			URL=`echo $ROUND | sed 's!href=!\nurl=!g' | grep ^url= | cut -d'"' -f2 | head -n1`
			TITLE=`echo $ROUND | sed 's!<a alt="!\ntitle="!g' | grep ^title= | cut -d'"' -f2 | sed 's!&ndash;!-!g'`
			PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
			if [ ! -z "$TITLE" ] && [ ! -z "$URL" ] && [ "$URL" != "http://xlivetv.sx" ] && [ `cat $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list | grep "#$URL#" | wc -l` -eq 0 ];then
				if [ ! -e $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list ];then
					touch $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list
				fi
				piccount=`expr $piccount + 1`
#				LINE="$TITLE#$URL#$PIC#$PARSER_$piccount.jpg#$NAME#98"
				URL="$SRC $SRC play $INPUT $URL"
				LINE="$TITLE#$URL#$PIC#$PARSER_$piccount.jpg#$NAME#111"

				echo "$LINE" >> $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list
			fi

		done 3<$TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.2
		rm $TMP/cache.* > /dev/null 2>&1
	fi
	echo "$TMP/$PARSER.$INPUT.$FROM.$FILENAME.list"
}

play()
{
	debug=0

	if [ "$debug" = "1" ]; then rm $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list; fi
	if [ "$debug" = "1" ]; then echo $INPUT 1111111; fi

	if [ ! -e "$TMP/$PARSER.$INPUT.$FROM.$FILENAME.list" ]; then
		piccount=0

		$curlbin $URL/$PAGE -o $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.1
		if [ "$debug" = "1" ]; then echo $INPUT 222222; fi

		cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.1 | tr '\n' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!<iframe src=!\nfound=!g' | sed 's!<br> </td>!\n<br> </td>!g' | grep '^found=' | grep 'video/embed' | cut -d"'" -f2 >$TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.2
		if [ `cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.2 | wc -l` -gt 0 ];then
			URLTMP=`cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.2 | sed 's#//#\nhttp://#' | grep ^"http://"`
			if [ "$debug" = "1" ]; then echo $INPUT 333333 $URLTMP; fi

			$curlbin $URLTMP --referer $URL$PAGE -o $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.3
	#		"flashVars": {"autoplay":0,"movieSrc":"mail/arsen.bulyaev/_myvideo/738","metadataUrl":"//my.mail.ru/+/video/meta/4219658639352267490","showPauseRoll":"0","enable_search":"2","swfVersion":"29","static_version":"75","flash_enabled":"1"},
			cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.3 | tr '\n' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!"movieSrc":!\nfound=!g' | grep '^found=' | cut -d'"' -f2 | head -n1 >$TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.4
			TYPE=`cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.4 | cut -d "/" -f2`
			ID=`cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.4 | tr '/' '\n' | tail -n1`
			$curlbin http://videoapi.my.mail.ru/videos/mail/$TYPE/_myvideo/$ID.json?ver=0.2.60 --referer $URLTMP -o $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.5
	#		$curlbin http://videoapi.my.mail.ru/videos/mail/arsen.bulyaev/_myvideo/738.json?ver=0.2.60 --referer $URLTMP -o $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.5

			cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.5 | tr '\n' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!"url":!\nfound=!g' | grep '^found=' | cut -d'"' -f2 | tail -n1 >$TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.6
			URL=`cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.6`
			#eplayer3 "http://cdn41.my.mail.ru/v/60128796.mp4?sign=43ff6ef971dd635bbff0a57a061a3548cfaceef8&slave[]=s%3Ahttp%3A%2F%2F127.0.0.1%3A5010%2F60128796-v.mp4&p=f&expire_at=1477882800&touch=1477731621&reg=202&region=202"
		else
			if [ "$debug" = "1" ]; then echo $INPUT 444444 $URLTMP; fi

#	        <iframe allowFullScreen allowFullScreen frameborder=0 marginheight=0 marginwidth=0 scrolling='no'src="http://emb.aliez.me/player/video.php?id=47383&s=t67axfnq&w=590&h=384" width="590" height="384">Your browser does not support inline frames or is currently configured not to display inline frames.</iframe>
#		    <iframe allowFullScreen src="//livetv141.net/export/vk.reframe.php?ur4=http://vk.com/video_ext.php?oid=-55574239&id=456242297&hash=f78096b994400693&hd=1" width="600" height="338" frameborder="0" allowfullscreen></iframe>
#			<iframe allowFullScreen src="http://livetv141.net/export/vk.reframe.php?ur4=http://vk.com/video_ext.php?oid=-30408&id=456242896&hash=1bafa57efd8d7c50" width="600" height="338" frameborder="0" allowfullscreen></iframe>
#			<iframe allowFullScreen src='https://my.mail.ru/video/embed/4219658639352267889' width='626' height='367' frameborder='0' webkitallowfullscreen mozallowfullscreen allowfullscreen></iframe>
#			cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.1 | tr '\n' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!<iframe!\nfound=!g' | grep ^found | sed 's!src=!\nfound2=!g' | grep ^found2 | cut -d'"' -f2 | grep -v facebook | grep -v getbanner.php | grep -v userapi | grep http >$TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.2
			cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.1 | sed 's!<iframe!\nfound=!g' | grep ^found | sed 's!src=!\nfound2=!g' | grep ^found2 | cut -d'"' -f2 | grep -v facebook | grep -v getbanner.php | grep -v userapi | grep http >$TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.2

			URLTMP=`cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.2 | sed 's#//#\nhttp://#' | grep ^"http://" | cut -d"'" -f1 | cut -d'"' -f1`
			if [ "$debug" = "1" ]; then echo $INPUT 555555 $URLTMP; fi


#<a target="_blank" href="http://tinyurl.com/jg7d7px"><img src="//cdn.livetvcdn.net/img/extplay.gif"></a>
#<link rel="manifest" href="/manifest.json"><link rel="shortlink" href="https://youtu.be/016LXFHpFCk"><link rel="search" type="application/opensearchdescription+xml" href="https://www.youtube.com/opensearch?locale=de_DE" title="YouTube-Videosuche"><link rel="shortcut icon" href="https:/

			if [ -z "$URLTMP" ];then
				cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.1 | grep '<a target="_blank" href=' >$TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.2
				URLTMP=`cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.2 | sed 's#//#\nhttp://#' | grep ^"http://" | cut -d"'" -f1 | cut -d'"' -f1`
				if [ "$debug" = "1" ]; then echo $INPUT 666666 $URLTMP; fi
			fi

			rm /mnt/network/cookies

			if [ `echo $URLTMP | grep "export/vk.reframe.php" | wc -l` -eq 1 ];then
				if [ "$debug" = "1" ]; then echo $INPUT 777777 $URLTMP; fi

				referer=$URLTMP
				$curlbin $URLTMP --referer $URL$PAGE -o $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.3
				# <iframe src="http://vk.com/video_ext.php?oid=-55574239&id=456242333&hash=8f52dbd56d595751&hd=1" frameborder="0" height="100%" width="100%"></iframe></body>
				cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.3 | tr '\n' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!<iframe src=!\nfound=!g' | grep '^found=' | cut -d'"' -f2 | head -n1 >$TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.4
				URLTMP=`cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.4 | sed 's#//#\nhttps://#' | grep ^"https://"`
				if [ "$debug" = "1" ]; then echo $INPUT 888888 $URLTMP; fi

				$curlbin $URLTMP --referer $URL$PAGE -o $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.5
				cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.5 | grep vk.com | sed 's!href=!\nfound=!' | grep ^found | cut -d '"' -f2 | head -n1 >$TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.6
				URLTMP=`cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.6 | sed 's#//#\nhttps://#' | grep ^"https://"`
				if [ "$debug" = "1" ]; then echo $INPUT 999999 $URLTMP; fi

				email=`cat /mnt/config/titan.cfg | grep vk_user | cut -d"=" -f2`
				pass=`cat /mnt/config/titan.cfg | grep vk_pass | cut -d"=" -f2`

				$curlbin2 -v $URLTMP --referer $URL$PAGE -o $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.7
				loginurl="https://login.vk.com/?act=login"
				act=`cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.7 | sed 's!name="act"!\nfound=!' | grep ^found | cut -d'"' -f2| head -n1`
				role=`cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.7 | sed 's!name="role"!\nfound=!' | grep ^found | cut -d'"' -f2| head -n1`
				expire=`cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.7 | sed 's!name="expire"!\nfound=!' | grep ^found | cut -d'"' -f2| head -n1`
				captcha_sid=`cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.7 | sed 's!name="captcha_sid"!\nfound=!' | grep ^found | cut -d'"' -f2| head -n1`
				captcha_key=`cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.7 | sed 's!name="captcha_key"!\nfound=!' | grep ^found | cut -d'"' -f2| head -n1`
				_origin=`cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.7 | sed 's!name="_origin"!\nfound=!' | grep ^found | cut -d'"' -f2| head -n1`
				ip_h=`cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.7 | sed 's!name="ip_h"!\nfound=!' | grep ^found | cut -d'"' -f2| head -n1`
				lg_h=`cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.7 | sed 's!name="lg_h"!\nfound=!' | grep ^found | cut -d'"' -f2| head -n1`
				post="act=$act&role=$role&expire=$expire&captcha_sid=$captcha_sid&captcha_key=$captcha_key&_origin=$origin&ip_h=$ip_h&lg_h=$lg_h&email=$email&pass=$pass"

				$curlbin2 -v -X POST --data "$post" --referer "$URL$PAGE" "$loginurl" -D $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.8
				tmpurl="`cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.8 | grep Location: | sed 's/Location: //' | tr '\0' ' ' | tr '\r' ' ' | tr '\n' ' ' | sed 's/\s//g'`"
				$curlbin2 "$tmpurl" > $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.9

				$curlbin2 -v $URL$PAGE -o $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.10
				cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.3 | tr '\n' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!<iframe src=!\nfound=!g' | grep '^found=' | cut -d'"' -f2 | head -n1 >$TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.11
				URLTMP=`cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.11 | sed 's#//#\nhttps://#' | grep ^"https://"`
				if [ "$debug" = "1" ]; then echo $INPUT aaaaaa $URLTMP; fi

				$curlbin $URLTMP --referer "$referer" -o $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.12

				if [ `cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.12 | grep "Authorization required" | wc -l` -eq 1 ];then
					URL="errormsg=You need access to https://vk.com to use this full stream, add VK User/Pass on Tithek Settings"
				else
					cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.12 | sed 's!<source src=!\nfound=!g' | grep ^found | grep .720. | cut -d'"' -f2 > $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.13.720
					URL=`cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.13.720`
					if [ -z "$URL" ];then
						cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.12 | sed 's!<source src=!\nfound=!g' | grep ^found | grep .360. | cut -d'"' -f2 > $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.13.360
						URL=`cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.13.360`
					fi
				fi
			elif [ `echo $URLTMP | grep "youtube" | wc -l` -eq 1 ];then
				if [ "$debug" = "1" ]; then echo $INPUT bbbbbb $URLTMP; fi

				ID=`echo $URLTMP | tr '/' '\n' | tail -n1`
				URL="https://www.youtube.com/get_video_info?el=leanback&cplayer=UNIPLAYER&cos=Windows&height=1080&cbr=Chrome&hl=en_US&cver=4&ps=leanback&c=TVHTML5&video_id=$ID&cbrver=40.0.2214.115&width=1920&cosver=6.1&ssl_stream=1"
				
#				$curlbin "https://www.youtube.com/get_video_info?el=leanback&cplayer=UNIPLAYER&cos=Windows&height=1080&cbr=Chrome&hl=en_US&cver=4&ps=leanback&c=TVHTML5&video_id=17NnIkv2C3k&cbrver=40.0.2214.115&width=1920&cosver=6.1&ssl_stream=1"
				
			#	https://www.youtube.com/embed/17NnIkv2C3k

#				URL="$SRC $SRC hoster $INPUT $URL"
				URL="gethoster2 $URL"

#				URL="gethoster2 $URLTMP"

#				URL="$TITLE#$URL#$PIC#$PIC.jpg#$NAME#14"

			elif [ `echo $URLTMP | grep "/embed/" | wc -l` -eq 1 ];then
				if [ "$debug" = "1" ]; then echo $INPUT cccccc $URLTMP; fi

				$curlbin $URLTMP --referer $URL$PAGE -o $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.3
		#		"flashVars": {"autoplay":0,"movieSrc":"mail/arsen.bulyaev/_myvideo/738","metadataUrl":"//my.mail.ru/+/video/meta/4219658639352267490","showPauseRoll":"0","enable_search":"2","swfVersion":"29","static_version":"75","flash_enabled":"1"},
		#		"flashVars": {"autoplay":0,"movieSrc":"mail/arsen.bulyaev/_myvideo/1137","metadataUrl":"//my.mail.ru/+/video/meta/4219658639352267889","showPauseRoll":"0","enable_search":"2","swfVersion":"29","static_version":"77","flash_enabled":"1"},

				cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.3 | tr '\n' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!"movieSrc":!\nfound=!g' | grep '^found=' | cut -d'"' -f2 | head -n1 >$TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.4
				TYPE=`cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.4 | cut -d "/" -f2`
				ID=`cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.4 | tr '/' '\n' | tail -n1`
				$curlbin http://videoapi.my.mail.ru/videos/mail/$TYPE/_myvideo/$ID.json?ver=0.2.60 --referer $URLTMP -o $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.5
		#		$curlbin http://videoapi.my.mail.ru/videos/mail/arsen.bulyaev/_myvideo/738.json?ver=0.2.60 --referer $URLTMP -o $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.5

				cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.5 | tr '\n' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!"url":!\nfound=!g' | grep '^found=' | cut -d'"' -f2 | tail -n1 >$TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.6
				URL=`cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.6`
			elif [ `echo $URLTMP | grep "tinyurl" | wc -l` -eq 1 ];then
				if [ "$debug" = "1" ]; then echo $INPUT eeeeee $URLTMP; fi
#<link rel="manifest" href="/manifest.json"><link rel="shortlink" href="https://youtu.be/016LXFHpFCk"><link rel="search" type="application/opensearchdescription+xml" href="https://www.youtube.com/opensearch?locale=de_DE" title="YouTube-Videosuche"><link rel="shortcut icon" href="https:/

				$curlbin $URLTMP --referer $URL$PAGE -o $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.3
				cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.3 | sed 's!<link rel!\n<link rel!g' | grep shortlink | sed 's#//#\nhttp://#' | grep ^"http://" | cut -d"'" -f1 | cut -d'"' -f1 >$TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.4
				URLTMP=`cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.4`
				if [ "$debug" = "1" ]; then echo $INPUT ffffff $URLTMP; fi

				ID=`echo $URLTMP | tr '/' '\n' | tail -n1`
				URL="https://www.youtube.com/get_video_info?el=leanback&cplayer=UNIPLAYER&cos=Windows&height=1080&cbr=Chrome&hl=en_US&cver=4&ps=leanback&c=TVHTML5&video_id=$ID&cbrver=40.0.2214.115&width=1920&cosver=6.1&ssl_stream=1"
				URL="gethoster2 $URL"
			else
				if [ "$debug" = "1" ]; then echo $INPUT gggggg $URLTMP; fi
				URLTMP=`echo $URLTMP  | sed 's#//#\nhttp://#' | grep ^"http://"`
				if [ "$debug" = "1" ]; then echo $INPUT hhhhhh $URLTMP; fi

				$curlbin $URLTMP --referer $URL$PAGE -o $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.3
				cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.3 | tr '\n' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!file:!\nfound=!g' | grep '^found=' | cut -d"'" -f2 | head -n1 >$TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.4
				URL=`cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.4`
			fi
		fi

		echo $URL
		if [ "$debug" = "0" ]; then rm $TMP/cache.* > /dev/null 2>&1; fi
	fi
#	echo "$TMP/$PARSER.$INPUT.$FROM.$FILENAME.list"
}

submenu()
{
	if [ ! -e "$TMP/$PARSER.$INPUT.$FROM.$FILENAME.list" ]; then
		piccount=0

		$wgetbin $URL/$PAGE -O $TMP/cache.$PARSER.$FROM.$FILENAME.1
#		cat $TMP/cache.$PARSER.$FROM.$FILENAME.1 | sed 's!<a href="/enx/team/!\n<a href="/enx/team/!g' | grep '^<a href="/enx/team/' | tr ' ' '~' >$TMP/cache.$PARSER.$FROM.$FILENAME.2
		cat $TMP/cache.$PARSER.$FROM.$FILENAME.1 | sed 's!<a href="/enx/team/!\n<a href="/enx/team/!g' | grep '^<a href="/enx/team/' >$TMP/cache.$PARSER.$FROM.$FILENAME.2

		while read -u 3 ROUND; do
			URL="$SRC $SRC videos $INPUT `echo $ROUND | cut -d'"' -f2 | sed 's!/calendar/!/video/!g'`"
			TITLE=`echo $ROUND | sed 's!<b>!\ntitle=<!g' | grep ^title= | cut -d'<' -f2`
			PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"

			if [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
				if [ ! -e $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list ];then
					touch $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list
				fi
				piccount=`expr $piccount + 1`
				LINE="$TITLE#$URL#$PIC#$PARSER_$piccount.jpg#$NAME#0"
				echo "$LINE" >> $TMP/$PARSER.$INPUT.$FROM.$FILENAME.list
			fi

		done 3<$TMP/cache.$PARSER.$FROM.$FILENAME.2
		rm $TMP/cache.* > /dev/null 2>&1
	fi
	echo "$TMP/$PARSER.$INPUT.$FROM.$FILENAME.list"
}

livelist()
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

case $INPUT in
	init) $INPUT;;
	mainmenu) $INPUT;;
	basketball) $INPUT;;
	icehockey) $INPUT;;
	fussball) $INPUT;;
	handball) $INPUT;;
	football) $INPUT;;
	livelist) $INPUT;;
	hosterlist) $INPUT;;
	hoster) $INPUT;;
	videos) $INPUT;;
	play) $INPUT;;
	playsrc) $INPUT;;
	submenu) $INPUT;;
	month) $INPUT;;
	year) $INPUT;;
esac
