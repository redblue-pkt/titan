#!/bin/sh
# first sh box parser for titannit mfg obi

case $2 in
	init) echo skip load hoster.sh;;
	*) . /tmp/localhoster/hoster.sh;;
esac

SRC=$1
INPUT=$2
PAGE=$3

URL=http://www.7tv.de/
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME=7tv
FILENAME="$PARSER $INPUT $PAGE"
FILENAME=`echo $FILENAME | tr '&' '.' | tr '/' '.' | tr '?' '.'  | tr '=' '.' | sed 's/ \+/./g' | sed 's/\.\+/./g'`

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
#	rm -f /mnt/network/cookies > /dev/null 2>&1
	rm -rf $TMP > /dev/null 2>&1
	echo "$NAME ($TYPE)#$SRC $SRC mainmenu#http://atemio.dyndns.tv/mediathek/menu/$PARSER.jpg#$PARSER.jpg#TiThek#0"
}

mainmenu()
{
#	echo "Tv Shows#$SRC $SRC serien tv/#http://atemio.dyndns.tv/mediathek/menu/tv-shows.jpg#tv-shows.jpg#$NAME#0" > $TMP/$FILENAME.list
	echo "Search Serien#$SRC $SRC searchtv '7tvsearch/search/query/%search%/type/format/offset/0/limit/5'#http://atemio.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >$TMP/$FILENAME.list
	echo "Search Clips#$SRC $SRC searchtv '7tvsearch/search/query/%search%/type/clip/offset/0/limit/5'#http://atemio.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >>$TMP/$FILENAME.list
	echo "Search Ganue Folgen#$SRC $SRC searchtv '7tvsearch/search/query/%search%/type/episode/offset/0/limit/5'#http://atemio.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >>$TMP/$FILENAME.list
	echo "$TMP/$FILENAME.list"
}

#13:22:35.345 T:140602909902592   DEBUG: [plugin.video.7tvneu-1.0.1](searchtext) URL-x : https://www.7tv.de/7tvsearch/search/query/big+brother/type/format/offset/0/limit/5
searchtv()
{
rm "$TMP/$FILENAME.list"
rm $TMP/cache.$FILENAME.*
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		piccount=0

		$curlbin $URL/$PAGE$NEXT$PAGE2 -o $TMP/cache.$FILENAME.1
#		$BIN /tmp/localhoster/cloudflare.py "$URL/$PAGE$NEXT$PAGE2" > $TMP/cache.$FILENAME.1


		if [ -z "$NEXT" ];then
			NEXT=1
		fi
		pages=`cat $TMP/cache.$FILENAME.1 | grep $PAGE | sed "s/$PAGE/\n$PAGE/g" | cut -d ">" -f2 | cut -d "<" -f1 | tail -n1`
		if [ -z "$pages" ];then
			pages=0
		fi
		cat $TMP/cache.$FILENAME.1 | tr '\n' ' ' | sed 's!<a href=!\n<a href=!g' | grep ^"<a href=" >$TMP/cache.$FILENAME.2

#echo $TMP/cache.$FILENAME.1 
#exit

		while read -u 3 ROUND; do
			ID=`echo $ROUND | cut -d'"' -f1 | tail -n1`
			NEWPAGE=`echo $ROUND | sed -nr 's/.*<a href="([^"]+)".*/\1/p'`
			TITLE=`echo $NEWPAGE | tr '-' ' ' | sed 's!/!!'`
	
#			if [ ! -z "$NEWPAGE" ]; then
#				if [ -z "$TITLE" ];then
#					TITLE=`echo $TMPURL`
#				fi
#				TMPURL="$URL"/$TMPURL
#			fi

			PIC=`echo $ROUND | sed -nr 's/.*data-src="([^"]+)".*/\1/p'`

#suche big broter
#
#serien
#14:07:13.118 T:139913047566080   DEBUG: 'GET /7tvsearch/search/query/big+brother/type/format/offset/0/limit/5 HTTP/1.1\r\nHost: www.7tv.de\r\nConnection: close\r\nAccept-Encoding: gzip, deflate\r\n\r\n'
#
#clips
#14:08:13.114 T:139913047566080   DEBUG: 'GET /7tvsearch/search/query/big+brother/type/clip/offset/0/limit/5 HTTP/1.1\r\nHost: www.7tv.de\r\nConnection: close\r\nAccept-Encoding: gzip, deflate\r\n\r\n'
#
#ganue folgen
#14:09:21.003 T:139914294122240   DEBUG: 'GET /7tvsearch/search/query/big+brother/type/episode/offset/0/limit/5 HTTP/1.1\r\nHost: www.7tv.de\r\nConnection: close\r\nAccept-Encoding: gzip, deflate\r\n\r\n'

#14:14:26.546 T:139914311513856   DEBUG: [plugin.video.7tvneu-1.0.1](searchtext) URL-x : https://www.7tv.de/7tvsearch/search/query/big+brother/type/episode/offset/7/limit/5

			if [ `echo $ROUND | grep us_ger_small.png | wc -l` -eq 1 ]; then
				LANG=" (de)"
			elif [ `echo $ROUND | grep us_flag_small.png | wc -l` -eq 1 ]; then
				LANG=" (en)"
			else
				LANG=" (??)"
			fi
	
			if [ -z "$PIC" ]; then
				PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
			fi
#echo TITLE $TITLE
#echo NEWPAGE $NEWPAGE
#echo PIC $PIC
#exit
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/\C3x/g' -e 's/&ndash;/-/g' -e 's/&Auml;/\C3/g' -e 's/&Uuml;/\C3S/g' -e 's/&Ouml;/\C3/g' -e 's/&auml;/ä/g' -e 's/&uuml;/ü/g' -e 's/&ouml;/ö/g' -e 's/&eacute;/é/g' -e 's/&egrave;/è/g' -e 's/%F6/ö/g' -e 's/%FC/ü/g' -e 's/%E4/ä/g' -e 's/%26/&/g' -e 's/%C4/\C3/g' -e 's/%D6/\C3/g' -e 's/%DC/\C3S/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
	
			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
				if [ ! -e $TMP/$FILENAME.list ];then
					touch $TMP/$FILENAME.list
				fi
				piccount=`expr $piccount + 1`
#14:17:18.074 T:139914121307904   DEBUG: 'GET /big-brother/ganze-folgen HTTP/1.1\r\nHost: www.7tv.de\r\nConnection: close\r\nAccept-Encoding: gzip, deflate\r\n\r\n'

				LINE="$TITLE (ganze-folgen)#$SRC $SRC episodetv $NEWPAGE/ganze-folgen#$PIC#$PARSER.$INPUT.$NEXT.$PAGE2.$FILENAME.$piccount.jpg#$NAME#0"
				echo "$LINE" >> $TMP/$FILENAME.list

				LINE="$TITLE (alle-clips)#$SRC $SRC episodetv $NEWPAGE/alle-clips#$PIC#$PARSER.$INPUT.$NEXT.$PAGE2.$FILENAME.$piccount.jpg#$NAME#0"
				echo "$LINE" >> $TMP/$FILENAME.list

#	LINE="Big Brother#episode '/tv/big-brother'#http://atemio.dyndns.tv/mediathek/menu/default.jpg#sixx.serien.tv..41.jpg#Sixx#0"

			fi
	
		done 3<$TMP/cache.$FILENAME.2

		if [ "$NEXT" -lt "$pages" ]; then
			NEXTPAGE=`expr $NEXT + 1`
			LINE="Page ($NEXTPAGE/$pages)#$SRC $SRC searchtv '$PAGE' $NEXTPAGE '$PAGE2'#http://atemio.dyndns.tv/mediathek/menu/next.jpg#next.jpg#$NAME#0"
			echo "$LINE" >> $TMP/$FILENAME.list
		fi
#		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi
	echo "$TMP/$FILENAME.list"
}

episodetv()
{
rm "$TMP/$FILENAME.list"
rm $TMP/cache.$FILENAME.*
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		piccount=0

		$curlbin $URL/$PAGE$NEXT$PAGE2 -o $TMP/cache.$FILENAME.1
#		$BIN /tmp/localhoster/cloudflare.py "$URL/$PAGE$NEXT$PAGE2" > $TMP/cache.$FILENAME.1


		if [ -z "$NEXT" ];then
			NEXT=1
		fi
		pages=`cat $TMP/cache.$FILENAME.1 | grep $PAGE | sed "s/$PAGE/\n$PAGE/g" | cut -d ">" -f2 | cut -d "<" -f1 | tail -n1`
		if [ -z "$pages" ];then
			pages=0
		fi
		cat $TMP/cache.$FILENAME.1 | tr '\n' ' ' | sed 's!<a href=!\n<a href=!g' | grep ^"<a href=" | grep "teaser-img" | grep "data-track" >$TMP/cache.$FILENAME.2

#next
#<a href="/big-brother/alle-clips" itemprop="url" ><span itemprop="name">Alle Clips</span></a>              </li>             </ul> </nav><!-- / #format_nav-->                     </div>                   <div class="format-sponsor">                             <div class="format-sponsor-brand">                     <a class="brand-logo-sixx" href="https://www.sixx.de" target="_blank" title="sixx"></a>                 </div>                                  </div>     </div>                                                <!--googleoff: index-->  <div class="main-zone">                                                                         <div class="row ">     <div class="row-wrapper " data-track="zone:main,row:N/A,col:N/A,width:3,cont:">                                                         <section class="bentobox-default teaser-row" data-ajax-more="/psdflow/ajaxblock/(block)/1600530d298d8cd1de1e560337538f82/(offset)/24/(limit)/24/(node_id)/117716">                 <article class="teaser teaser_300x160 class-clip" data-track="teasNr:1,BentoBox default,Staffel 1 Episode 92: Big Brother Live - Das Finale - Teil 1 - N/A">                  

#echo $TMP/cache.$FILENAME.1 
#exit

		while read -u 3 ROUND; do
			ID=`echo $ROUND | cut -d'"' -f1 | tail -n1`
			NEWPAGE=`echo $ROUND | sed -nr 's/.*<a href="([^"]+)".*/\1/p'`
			TITLE=`echo $ROUND | sed -nr 's/.*title="([^"]+)".*/\1/p'`
if [ -z "$TITLE" ]; then
			TITLE=`echo $NEWPAGE | tr '-' ' ' | sed 's!/!!'`
fi

TITLE=`echo $TITLE | tr '-' '\n' | tail -n3 | tr '\n' '-'`
#TITLE2=`echo $TITLE | tr '-' '\n' | tail -n2`

			PIC=`echo $ROUND | sed -nr 's/.*data-src="([^"]+)".*/\1/p'`
	
			if [ -z "$PIC" ]; then
				PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
			fi
#echo TITLE $TITLE
#echo NEWPAGE $NEWPAGE
#echo PIC $PIC
#exit
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/\C3x/g' -e 's/&ndash;/-/g' -e 's/&Auml;/\C3/g' -e 's/&Uuml;/\C3S/g' -e 's/&Ouml;/\C3/g' -e 's/&auml;/ä/g' -e 's/&uuml;/ü/g' -e 's/&ouml;/ö/g' -e 's/&eacute;/é/g' -e 's/&egrave;/è/g' -e 's/%F6/ö/g' -e 's/%FC/ü/g' -e 's/%E4/ä/g' -e 's/%26/&/g' -e 's/%C4/\C3/g' -e 's/%D6/\C3/g' -e 's/%DC/\C3S/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
	
			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
				if [ ! -e $TMP/$FILENAME.list ];then
					touch $TMP/$FILENAME.list
				fi
				piccount=`expr $piccount + 1`

#				LINE="$TITLE#$SRC $SRC episodetvlist $NEWPAGE#$PIC#$PARSER.$INPUT.$NEXT.$PAGE2.$FILENAME.$piccount.jpg#$NAME#0"
				LINE="$TITLE#$SRC $SRC play '$NEWPAGE'#$PIC#$FILENAME.$piccount.jpg#$NAME#111"

				echo "$LINE" >> $TMP/$FILENAME.list

			fi
	
		done 3<$TMP/cache.$FILENAME.2

		PAGE=`cat $TMP/cache.$FILENAME.1 | sed -nr 's/.*data-ajax-more="([^"]+)".*/\1/p'`

		if [ ! -z "$PAGE" ];then
#			if [ "$NEXT" -lt "$pages" ]; then
				NEXTPAGE=`expr $NEXT + 1`
				LINE="Page ($NEXTPAGE/$pages)#$SRC $SRC episodetv '$PAGE' $NEXTPAGE '$PAGE2'#http://atemio.dyndns.tv/mediathek/menu/next.jpg#next.jpg#$NAME#0"
				echo "$LINE" >> $TMP/$FILENAME.list
			fi
#		fi
#		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi
	echo "$TMP/$FILENAME.list"
}

#[titan] cmd: /mnt/parser/7tv.sh /mnt/parser/7tv.sh searchtv '7tvsearch/search/query/big+brother/type/format/offset/0/limit/5', file=tithek_global.h, func=localparser_search, line=1476
#[tithek] cmd: /mnt/parser/7tv.sh /mnt/parser/7tv.sh episodetv /big-brother/ganze-folgen
#[tithek] cmd: /mnt/parser/7tv.sh /mnt/parser/7tv.sh episodetvlist /big-brother/183-staffel-1-episode-83-big-brother-tag-82-teil-2-ganze-folge

episodetvlist()
{
rm "$TMP/$FILENAME.list"
rm $TMP/cache.$FILENAME.*

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
#		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi
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
			PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"

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
	LINE="Big Brother#/tmp/localparser/sixx.sh /tmp/localparser/sixx.sh episode '/tv/big-brother'#http://atemio.dyndns.tv/mediathek/menu/default.jpg#sixx.serien.tv..41.jpg#Sixx#0"
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
rm "$TMP/$FILENAME.list"
rm $TMP/cache.$FILENAME.*

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
#		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi
	echo "$TMP/$FILENAME.list"
}

play()
{
#	/tmp/localhoster/hoster.sh youtube_dl $URL/$PAGE
	/tmp/localhoster/hoster.sh youtube_dl $PAGE
}


case $INPUT in
	init) $INPUT;;
	mainmenu) $INPUT;;
	play) $INPUT;;
	search) $INPUT;;
	serien) $INPUT;;
	episode) $INPUT;;
	episodelist) $INPUT;;
	searchtv) $INPUT;;
	episodetv) $INPUT;;
	episodetvlist) $INPUT;;
esac
