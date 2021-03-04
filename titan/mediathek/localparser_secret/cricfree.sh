#!/bin/sh
# first sh box parser for titannit mfg obi

SRC=$1
INPUT=$2
PAGE=$3

URL=http://cricfree.sc/
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME=CricFree

case $2 in
	init)	;;
	*) 	. /tmp/localhoster/hoster.sh
	   	mkdir $TMP > /dev/null 2>&1
		FILENAME="$PARSER $INPUT $PAGE"
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
#	if [ "$ARCH" != "sh4" ] || [ -e /etc/.beta ];then
#		rm -f /mnt/network/cookies > /dev/null 2>&1
		rm -rf $TMP > /dev/null 2>&1
		echo "$NAME ($TYPE)#$SRC $SRC mainmenu#http://openaaf.dyndns.tv/mediathek/menu/$PARSER.jpg#$PARSER.jpg#TiThek#0"
#	fi
}

mainmenu()
{
	echo "Channels#$SRC $SRC search /#http://openaaf.dyndns.tv/mediathek/menu/category.jpg#category.jpg#$NAME#0" > $TMP/$FILENAME.list
	echo "$TMP/$FILENAME.list"
}

search()
{
	rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	rm $TMP/$FILENAME.* > /dev/null 2>&1

	if [ ! -e "$TMP/$FILENAME.list" ]; then
		piccount=0

		$curlbin $URL/$PAGE -o $TMP/cache.$FILENAME.1
		cat $TMP/cache.$FILENAME.1 | tr '\n' '\r' |  tr '\r' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!<li class="has-sub">!\n<li class="has-sub">!g' | grep ^'<li class="has-sub">' | sed 's!href=!\n<a href=!g' | grep ^'<a href=' | grep "http://cricfree.sc" | grep -v "/watch/live/" | cut -d '"' -f2 >$TMP/cache.$FILENAME.2

		while read -u 3 ROUND; do
			NEWPAGE=`echo $ROUND | cut -d'"' -f1 | sed "s#$URL##"`
#echo NEWPAGE $NEWPAGE
#echo URL $URL
#exit
			TITLE=`echo $ROUND | sed 's!http://cricfree.sc/!!' | sed 's!watch/live/!!' | sed 's!-live-stream-2!!' | sed 's!-live-stream!!' | sed 's!-in-streaming!!' | sed 's!-live-streaming-2!!' | sed 's!-live-streaming!!'`
			PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"

			TITLE=`echo $TITLE | sed 's/-/ /g' | sed 's/_/ /g'`
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`

			if [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
				if [ ! -e $TMP/$FILENAME.list ];then
					touch $TMP/$FILENAME.list
				fi
				piccount=$[$piccount+1]
#				LINE="$TITLE#$URL#$PIC#$PARSER_$piccount.jpg#$NAME#92"
#				LINE="$TITLE#$SRC $SRC play '$NEWPAGE'#$PIC#$FILENAME.$piccount.jpg#$NAME#111"
				LINE="$TITLE#$SRC $SRC play '$NEWPAGE'#$PIC#$FILENAME.$piccount.jpg#$NAME#111"

				echo "$LINE" >> $TMP/$FILENAME.list
			fi

		done 3<$TMP/cache.$FILENAME.2
#		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi
	echo "$TMP/$FILENAME.list"
}

play()
{
	/tmp/localhoster/hoster.sh get $URL/$PAGE
}

playold()
{
	rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	rm $TMP/$FILENAME.* > /dev/null 2>&1

	if [ ! -e "$TMP/$FILENAME.list" ]; then
		piccount=0

		$curlbin $URL/$PAGE -o $TMP/cache.$FILENAME.1
		# <iframe frameborder="0" marginheight="0" allowfullscreen="true" marginwidth="0" height="555" src="http://cricfree.sc/update/skys4.php" id="iframe" name="iframe_a" scrolling="no" width="620">Your Browser Do not Support Iframe</iframe>
		NEWPAGE=`cat $TMP/cache.$FILENAME.1 | grep iframe | grep http | grep allowfullscreen | sed 's/src=/\nfound=/' | grep ^found= | cut -d'"' -f2`

		$curlbin $NEWPAGE -o $TMP/cache.$FILENAME.2
#		<script type='text/javascript'>id='ss4new'; width='620'; height='490';</script><script type='text/javascript' src='http://soccerschedule.online/livegamecr.js'></script>
		NEWPAGE=`cat $TMP/cache.$FILENAME.2 | grep javascript | grep id= | sed 's/src=/\nfound=/' | grep ^found= | cut -d"'" -f2`
		ID=`cat $TMP/cache.$FILENAME.2 | grep javascript | grep id= | sed 's/id=/\nfound=/' | grep ^found= | cut -d"'" -f2`
		WIDTH=`cat $TMP/cache.$FILENAME.2 | grep javascript | grep id= | sed 's/width=/\nfound=/' | grep ^found= | cut -d"'" -f2`
		HEIGHT=`cat $TMP/cache.$FILENAME.2 | grep javascript | grep id= | sed 's/height=/\nfound=/' | grep ^found= | cut -d"'" -f2`

#echo NEWPAGE $NEWPAGE
#echo ID $ID
#echo WIDTH $WIDTH
#echo HEIGHT $HEIGHT

		$curlbin --referer $URL/$PAGE $NEWPAGE -o $TMP/cache.$FILENAME.3
		data="text1=`cat $TMP/cache.$FILENAME.3`"

#echo data $data
#		$curlbin http://ddecode.com/hexdecoder/ -o $TMP/cache.$FILENAME.3
		$curlbin -X POST --data "$data" http://ddecode.com/hexdecoder/ -o $TMP/cache.$FILENAME.4

#		"http://soccerschedule.online/cricyes.php?id=ss4new&width=620&height=490&stretching="
		NEWPAGE="http://soccerschedule.online/cricyes.php?id=$ID&width=$WIDTH&height=$HEIGHT&stretching="
		$curlbin --referer  $URL/$PAGE $NEWPAGE -o $TMP/cache.$FILENAME.5
		REFERER=$NEWPAGE


#		<script type='text/javascript'>id='241497'; width='100%'; height='100%'; stretching='';</script><script type='text/javascript' src='http://playstream.club/cricyesplayer.js'></script>
		NEWPAGE=`cat $TMP/cache.$FILENAME.5 | grep javascript | grep id= | sed 's/src=/\nfound=/' | grep ^found= | cut -d"'" -f2`
		ID=`cat $TMP/cache.$FILENAME.5 | grep javascript | grep id= | sed 's/id=/\nfound=/' | grep ^found= | cut -d"'" -f2`
		WIDTH=`cat $TMP/cache.$FILENAME.5 | grep javascript | grep id= | sed 's/width=/\nfound=/' | grep ^found= | cut -d"'" -f2`
		HEIGHT=`cat $TMP/cache.$FILENAME.5 | grep javascript | grep id= | sed 's/height=/\nfound=/' | grep ^found= | cut -d"'" -f2`
		$curlbin --referer $URL/$PAGE $NEWPAGE -o $TMP/cache.$FILENAME.6

#echo NEWPAGE $NEWPAGE
#echo ID $ID
#echo WIDTH $WIDTH
#echo HEIGHT $HEIGHT

		data="text1=`cat $TMP/cache.$FILENAME.6`"


#		echo data $data
		$curlbin -X POST --data "$data" http://ddecode.com/hexdecoder/ -o $TMP/cache.$FILENAME.7

		NEWPAGE="http://playstream.club/cricyesplayer.php?id=$ID&width=$WIDTH&height=$HEIGHT&stretching="
		$curlbin --referer  $REFERER $NEWPAGE -o $TMP/cache.$FILENAME.8


#       source: "http://178.159.36.7/live/241497.m3u8?wmsAuthSign=c2VydmVyX3RpbWU9Mi8yMi8yMDE3IDU6MDU6MTUgUE0maGFzaF92YWx1ZT1VdUhuZFlKc3N6ZU80czZmSUJ1NEd3PT0mdmFsaWRtaW51dGVzPTI4ODA=&wmsAuthSign2=NTgzZGIxMWJjMGMxOQ==", 
		STREAMURL=`cat $TMP/cache.$FILENAME.8 | grep "source:" | cut -d'"' -f2`

#		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi
	echo "$STREAMURL"
}


case $INPUT in
#	init) $INPUT;;
	mainmenu) $INPUT;;
	play) $INPUT;;
	search) $INPUT;;
esac
