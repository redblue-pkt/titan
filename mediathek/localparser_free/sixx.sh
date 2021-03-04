#!/bin/sh
# first sh box parser for titannit mfg obi

SRC=$1
INPUT=$2
PAGE=$3

URL=http://www.sixx.de/
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME=Sixx

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
#	rm -f /mnt/network/cookies > /dev/null 2>&1
	rm -rf $TMP > /dev/null 2>&1
	echo "$NAME ($TYPE)#$SRC $SRC mainmenu#http://openaaf.dyndns.tv/mediathek/menu/$PARSER.jpg#$PARSER.jpg#TiThek#0"
}

mainmenu()
{
	echo "Tv Shows#$SRC $SRC serien tv/#http://openaaf.dyndns.tv/mediathek/menu/tv-shows.jpg#tv-shows.jpg#$NAME#0" > $TMP/$FILENAME.list
	echo "$TMP/$FILENAME.list"
}

serien()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		piccount=0

		$curlbin $URL/$PAGE -o $TMP/cache.$FILENAME.1
#		cat $TMP/cache.$FILENAME.1 | tr '\n' '\r' |  tr '\r' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!<a href=!\nfound=!g' | grep ^'found=' | grep "/$PAGE" | grep -v "class=" | grep -v title= | grep '">' >$TMP/cache.$FILENAME.2

		while read -u 3 ROUND; do
			NEWPAGE=`echo $ROUND | cut -d'"' -f2`

			TITLE=$(echo $ROUND | sed -nr 's/.*">([^>]+)<\/a>.*/\1/p')
			PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"

			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`

			if [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
				if [ ! -e $TMP/$FILENAME.list ];then
					touch $TMP/$FILENAME.list
				fi
				piccount=$[$piccount+1]
				LINE="$TITLE#$SRC $SRC episode '$NEWPAGE'#$PIC#$FILENAME.$piccount.jpg#$NAME#0"
				echo "$LINE" >> $TMP/$FILENAME.list
			fi

		done 3<$TMP/cache.$FILENAME.2
		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi
#work start
	LINE="Big Brother#/tmp/localparser/sixx.sh /tmp/localparser/sixx.sh episode '/tv/big-brother'#http://openaaf.dyndns.tv/mediathek/menu/default.jpg#sixx.serien.tv..41.jpg#Sixx#0"
	echo "$LINE" > $TMP/$FILENAME.list
#end
	echo "$TMP/$FILENAME.list"
}

episode()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		piccount=0

		$curlbin $URL/$PAGE/alle-tage -o $TMP/cache.$FILENAME.1
		cat $TMP/cache.$FILENAME.1 | tr '\n' '\r' |  tr '\r' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!<a href=!\nfound=!g' | grep ^'found=' | grep "title=" | grep "$PAGE" >$TMP/cache.$FILENAME.2

		while read -u 3 ROUND; do
			NEWPAGE=`echo $ROUND | cut -d'"' -f2`
			TITLE=$(echo $ROUND | sed -nr 's/.*title="([^"]+)".*/\1/p')
			PIC=$(echo $ROUND | sed -nr 's/.*data-src="([^"]+)".*/\1/p')

			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`

			if [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
				if [ ! -e $TMP/$FILENAME.list ];then
					touch $TMP/$FILENAME.list
				fi
				piccount=$[$piccount+1]
				LINE="$TITLE#$SRC $SRC episodelist '$NEWPAGE'#$PIC#$FILENAME.$piccount.jpg#$NAME#0"
				echo "$LINE" >> $TMP/$FILENAME.list
			fi

		done 3<$TMP/cache.$FILENAME.2
		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi
	echo "$TMP/$FILENAME.list"
}


episodelist()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		piccount=0

		SEARCHSTR=$(echo $PAGE | tr '/' '\n' | tail -n1)

		$curlbin $URL/$PAGE -o $TMP/cache.$FILENAME.1
		cat $TMP/cache.$FILENAME.1 | tr '\n' '\r' |  tr '\r' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!<a href=!\nfound=!g' | grep ^'found=' | grep "title="  | grep "$SEARCHSTR" >$TMP/cache.$FILENAME.2

		while read -u 3 ROUND; do

			NEWPAGE=`echo $ROUND | cut -d'"' -f2`
			TITLE=$(echo $ROUND | sed -nr 's/.*title="([^"]+)".*/\1/p')
			PIC=$(echo $ROUND | sed -nr 's/.*data-src="([^"]+)".*/\1/p')

			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`

			if [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
				if [ ! -e $TMP/$FILENAME.list ];then
					touch $TMP/$FILENAME.list
				fi
				piccount=$[$piccount+1]
				LINE="$TITLE#$SRC $SRC play '$NEWPAGE'#$PIC#$FILENAME.$piccount.jpg#$NAME#111"
				echo "$LINE" >> $TMP/$FILENAME.list
			fi

		done 3<$TMP/cache.$FILENAME.2
		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi
	echo "$TMP/$FILENAME.list"
}

play()
{
	/tmp/localhoster/hoster.sh youtube_dl $URL/$PAGE
}


case $INPUT in
#	init) $INPUT;;
	mainmenu) $INPUT;;
	play) $INPUT;;
	search) $INPUT;;
	serien) $INPUT;;
	episode) $INPUT;;
	episodelist) $INPUT;;
esac
