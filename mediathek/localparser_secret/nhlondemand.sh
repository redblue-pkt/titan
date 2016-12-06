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
NAME=NHL-On-Demand

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
	echo "Category#$SRC $SRC category#http://atemio.dyndns.tv/mediathek/menu/category.jpg#category.jpg#$NAME#0" > $TMP/$PARSER.$INPUT.list
#	echo "Suche#$SRC $SRC suche#http://atemio.dyndns.tv/mediathek/menu/search.jpg#giga.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "$TMP/$PARSER.$INPUT.list"
}

category()
{
	echo "Ice Hockey - NHL (Latest)#$SRC $SRC videos latest enx/videotourney/2#http://atemio.dyndns.tv/mediathek/menu/nhlondemand.latest.jpg#nhlondemand.latest.jpg#$NAME#0" > $TMP/$PARSER.$INPUT.list
	echo "Ice Hockey - NHL (Teams)#$SRC $SRC submenu teams enx/leagueresults/2/#http://atemio.dyndns.tv/mediathek/menu/nhlondemand.teams.jpg#nhlondemand.teams.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
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
	if [ ! -e "$TMP/$PARSER.$INPUT.$FROM.$FILENAME.list" ]; then
		piccount=0

		$curlbin $URL/$PAGE -o $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.1

		cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.1 | tr '\n' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!<iframe src=!\nfound=!g' | sed 's!<br> </td>!\n<br> </td>!g' | grep '^found=' | grep 'video/embed' | cut -d"'" -f2 >$TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.2
		if [ `cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.2 | wc -l` -gt 0 ];then
			URLTMP=`cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.2`
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
#	        <iframe allowFullScreen allowFullScreen frameborder=0 marginheight=0 marginwidth=0 scrolling='no'src="http://emb.aliez.me/player/video.php?id=47383&s=t67axfnq&w=590&h=384" width="590" height="384">Your browser does not support inline frames or is currently configured not to display inline frames.</iframe>
#		    <iframe allowFullScreen src="//livetv141.net/export/vk.reframe.php?ur4=http://vk.com/video_ext.php?oid=-55574239&id=456242297&hash=f78096b994400693&hd=1" width="600" height="338" frameborder="0" allowfullscreen></iframe>
#			<iframe allowFullScreen src="http://livetv141.net/export/vk.reframe.php?ur4=http://vk.com/video_ext.php?oid=-30408&id=456242896&hash=1bafa57efd8d7c50" width="600" height="338" frameborder="0" allowfullscreen></iframe>
#			<iframe allowFullScreen src='https://my.mail.ru/video/embed/4219658639352267889' width='626' height='367' frameborder='0' webkitallowfullscreen mozallowfullscreen allowfullscreen></iframe>
			cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.1 | tr '\n' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!<iframe!\nfound=!g' | grep ^found | cut -d'"' -f2 | grep -v facebook >$TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.2

			if [ `cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.2 | grep "#f7f7f7" | wc -l` -eq 1 ];then
				cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.1 | tr '\n' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!<iframe!\nfound=!g' | grep ^found | cut -d"'" -f2 | grep -v facebook | grep -v Search >$TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.2
			fi

			URLTMP=`cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.2`
			if [ `echo $URLTMP | grep ^"//" | wc -l` -eq 1 ];then
				URLTMP=http:$URLTMP
			fi
			rm /mnt/network/cookies

			if [ `echo $URLTMP | grep "export/vk.reframe.php" | wc -l` -eq 1 ];then
				referer=$URLTMP
				$curlbin $URLTMP --referer $URL$PAGE -o $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.3
				# <iframe src="http://vk.com/video_ext.php?oid=-55574239&id=456242333&hash=8f52dbd56d595751&hd=1" frameborder="0" height="100%" width="100%"></iframe></body>
				cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.3 | tr '\n' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!<iframe src=!\nfound=!g' | grep '^found=' | cut -d'"' -f2 | head -n1 >$TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.4
				URLTMP=`cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.4`
				$curlbin $URLTMP --referer $URL$PAGE -o $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.5
				cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.5 | grep vk.com | sed 's!href=!\nfound=!' | grep ^found | cut -d '"' -f2 | head -n1 >$TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.6
				URLTMP=`cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.6`
				if [ `echo $URLTMP | grep ^"//" | wc -l` -eq 1 ];then
					URLTMP=https:$URLTMP
				fi

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
				URLTMP=`cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.11`

				$curlbin $URLTMP --referer "$referer" -o $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.12
				cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.12 | sed 's!<source src=!\nfound=!g' | grep ^found | grep .720. | cut -d'"' -f2 > $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.13.720
				URL=`cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.13.720`
				if [ -z "$URL" ];then
					cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.12 | sed 's!<source src=!\nfound=!g' | grep ^found | grep .360. | cut -d'"' -f2 > $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.13.360
					URL=`cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.13.360`
				fi
			elif [ `echo $URLTMP | grep "/embed/" | wc -l` -eq 1 ];then
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
			else
				$curlbin $URLTMP --referer $URL$PAGE -o $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.3
				cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.3 | tr '\n' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!file:!\nfound=!g' | grep '^found=' | cut -d"'" -f2 | head -n1 >$TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.4
				URL=`cat $TMP/cache.$PARSER.$INPUT.$FROM.$FILENAME.4`
			fi
		fi

		echo $URL
		rm $TMP/cache.* > /dev/null 2>&1
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
				piccount=$[$piccount+1]
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
	category) $INPUT;;
	livelist) $INPUT;;
	hosterlist) $INPUT;;
	hoster) $INPUT;;
	videos) $INPUT;;
	play) $INPUT;;
	playsrc) $INPUT;;
	submenu) $INPUT;;
esac
