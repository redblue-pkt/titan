#!/bin/sh
# first sh box parser for titannit mfg obi

. /tmp/localhoster/hoster.sh

SRC=$1
INPUT=$2
PAGE=$3
NEXT=$4
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`

FILENAME="$PARSER $INPUT $PAGE $NEXT"
FILENAME=`echo $FILENAME | tr '&' '.' | tr '/' '.' | tr '?' '.' | tr ';' '.' | tr '=' '.' | sed 's/ \+/./g' | sed 's/\.\+/./g'`

if [ -z "$FILENAME" ]; then
	FILENAME=none
fi

URL=http://www.pornhub.com
NAME="Pornhub"

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
	rm -rf $TMP > /dev/null 2>&1
	echo "$NAME ($TYPE)#$SRC $SRC mainmenu#http://atemio.dyndns.tv/mediathek/menu/$PARSER.jpg#$PARSER.jpg#TiThek#1000"
}

mainmenu()
{
	echo "Genre#$SRC $SRC genre 'categories'#http://atemio.dyndns.tv/mediathek/menu/Movies.genre.jpg#Movies.genre.jpg#$NAME#0" >$TMP/$FILENAME.list
	echo "$TMP/$FILENAME.list"
}

genre()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		piccount=0
		$curlbin $URL/$PAGE -o $TMP/cache.$FILENAME.1
		cat $TMP/cache.$FILENAME.1 | tr '\n' ' ' | sed 's/<a href=/\n\n\nfound=/g' | grep ^found= | grep "/video?c=" | tr '\t' ' ' | sed 's/ \+/ /g' | grep "img src" | grep ".jpg" >$TMP/cache.$FILENAME.2
		
		while read -u 3 ROUND; do
			PIC=`echo $ROUND | sed 's!<img src=!\npic=!g' | grep ^pic= | cut -d'"' -f2 | tail -n1`
			TITLE=`echo $ROUND | sed 's!data-mxptext=!\ntitle=!g' | grep ^title= | cut -d'"' -f2`
			NEWPAGE=`echo $ROUND | cut -d'"' -f2 | head -n1`
	
			if [ -z "$PIC" ]; then
				PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
			fi
	
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
	
			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
				if [ ! -e $TMP/$FILENAME.list ];then
					touch $TMP/$FILENAME.list
				fi
				piccount=`expr $piccount + 1`
	
				if [ `cat $TMP/$FILENAME.list | grep "#$NEWPAGE#" | wc -l` -eq 0 ];then
					LINE="$TITLE#$SRC $SRC search '$NEWPAGE&page=' 1#$PIC#$FILENAME.$FILENAME.$NEXT.$piccount.jpg#$NAME#0"
				fi
				echo "$LINE" >> $TMP/$FILENAME.list
			fi
	
		done 3<$TMP/cache.$FILENAME.2
		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi
	echo "$TMP/$FILENAME.list"
}

search()
{
	if [ -z "$NEXT" ]; then NEXT="search"; fi

	if [ -e "$TMP/$FILENAME.list" ] ; then
		rm $TMP/$FILENAME.list
	fi

	if [ ! -e "$TMP/$FILENAME.list" ]; then
		piccount=0

		$curlbin $URL/$PAGE$NEXT -o $TMP/cache.$FILENAME.1
		cat $TMP/cache.$FILENAME.1 | tr '\n' ' ' | sed 's/<a href=/\n\n\nfound=/g' | grep ^found= | grep "/view_video.php?viewkey=p" | tr '\t' ' ' | sed 's/ \+/ /g' | grep "img src" >$TMP/cache.$FILENAME.2
	
#		<li class="page_next_set"><a class="greyButton" href="/video?c=95&amp;page=10">10</a></li>
		pages=`cat $TMP/cache.$FILENAME.1 | grep "&amp;page" | grep "page_next_set" | sed 's/&amp;page=/\nfound="/g' | grep ^found= | cut -d'"' -f2 | tail -n1`

		while read -u 3 ROUND; do
			PIC=`echo $ROUND | sed 's!data-mediumthumb=!\npic=!g' | grep ^pic= | cut -d'"' -f2 | tail -n1`
			TITLE=`echo $ROUND | sed 's!title=!\ntitle=!g' | grep ^title= | cut -d'"' -f2`
			NEWPAGE=`echo $ROUND | cut -d'"' -f2`	
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
	
			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ] && [ ! -z "$PIC" ];then
				if [ ! -e $TMP/$FILENAME.list ];then
					touch $TMP/$FILENAME.list
				fi
				piccount=`expr $piccount + 1`

				if [ `cat $TMP/$FILENAME.list | grep "#$NEWPAGE#" | wc -l` -eq 0 ];then
					LINE="$TITLE#$SRC $SRC hoster '$NEWPAGE'#$PIC#$FILENAME.$FILENAME.$NEXT.$piccount.jpg#$NAME#111"
				fi
				echo "$LINE" >> $TMP/$FILENAME.list
			fi
	
		done 3<$TMP/cache.$FILENAME.2

		if [ "$NEXT" -lt "$pages" ]; then
			NEXTPAGE=`expr $NEXT + 1`
			LINE="Page ($NEXTPAGE/$pages)#$SRC $SRC search '$PAGE' $NEXTPAGE#http://atemio.dyndns.tv/mediathek/menu/next.jpg#next.jpg#$NAME#0"
			echo "$LINE" >> $TMP/$FILENAME.list
		fi
		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi
	echo "$TMP/$FILENAME.list"
}

hoster()
{
	rm $TMP/cache.$PARSER.$INPUT.* > /dev/null 2>&1
	/tmp/localhoster/hoster.sh youtube_dl $URL$PAGE > $TMP/cache.$PARSER.$INPUT.1
	STREAMURL=`cat $TMP/cache.$PARSER.$INPUT.1`
	echo $STREAMURL
}

case $INPUT in
	init) $INPUT;;
	mainmenu) $INPUT;;
	hoster) $INPUT;;
	search) $INPUT;;
	genre) $INPUT;;
esac
