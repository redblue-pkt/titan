#!/bin/sh
# first sh box parser for titannit mfg obi

SRC=$1
INPUT=$2
CURPAGE=$3
MAXPAGE=$4
PAGE=$5
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`

URL=http://foxx.to
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME="Foxx HD"

case $2 in
	init)	;;
	*) 	. /tmp/localhoster/hoster.sh
	   	mkdir $TMP > /dev/null 2>&1
		FILENAME="$PARSER $INPUT $CURPAGE $MAXPAGE $PAGE"
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
	echo "Random Movie#$SRC $SRC new 0 0 '?get=movies'#http://openaaf.dyndns.tv/mediathek/menu/all-newfirst.jpg#all-newfirst.jpg#$NAME#0" > $TMP/$FILENAME.list
	#echo "Serien#$SRC $SRC new 0 0 '?get=tv'#http://openaaf.dyndns.tv/mediathek/menu/Movies.jpg#Movies.jpg#$NAME#0" >> $TMP/$FILENAME.list
    echo "Search#$SRC $SRC search 1 0 '?s=%search%'#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >> $TMP/$FILENAME.list

	if [ -e "$TMP/$PARSER.new.list" ] ; then
		rm $TMP/$PARSER.new.list
	fi
	if [ "`echo $TMP/$PARSER.search.*.list`" != "$TMP/$PARSER.search.*.list" ] ; then
		rm $TMP/$PARSER.search.*.list
	fi
	if [ -e "$TMP/$PARSER.page.list" ] ; then
		rm $TMP/$PARSER.page.list
	fi

	echo "$TMP/$FILENAME.list"
}

new()
{
	if [ ! -e "$TMP/$FILENAME.list" ] ; then
#		$curlbin $URL/$PAGE -o $TMP/cache.$FILENAME.1

#		/tmp/localhoster/hoster.sh get $URL/$PAGE > $TMP/cache.$FILENAME.1
		$BIN /tmp/localhoster/cloudflare.py "$URL/$PAGE" > $TMP/cache.$FILENAME.1

		cat $TMP/cache.$FILENAME.1 | sed 's/<div class/\n<div class/g' | sed 's/<a href="/\n<a href="/g' | grep ^'<a href="' | grep alt= | grep -v .gif > /$TMP/cache.$FILENAME.2
		while read -u 3 ROUND; do
			TITLE=`echo $ROUND | sed 's/alt=/\nalt=/' | grep ^"alt=" | cut -d '"' -f2 | sed 's/#/%/'`
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/&%8211;/-/g' -e "s/&%8217;/'/g"`
			PIC=`echo $ROUND | sed 's!data-original=!\nsrc=!' | grep ^"src=" | cut -d '"' -f2`
			NEWPAGE=`echo $ROUND | sed 's/<a href=/\nhref=/' | grep ^"href=" | cut -d '"' -f2`

			if [ `echo $PIC | grep ^// | wc -l` -eq 1 ];then
				PIC=https:$PIC
			fi
			if [ `echo $NEWPAGE | grep ^// | wc -l` -eq 1 ];then
				NEWPAGE=https:$NEWPAGE
			fi
			if [ -z  "$PIC" ]; then  
				PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
				TMPPIC="default.jpg"
			else
				TMPPIC=goldesel_`echo $PIC | tr '/' '\n' | tail -n1`
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
				if [ `cat $TMP/$FILENAME.list | grep ^"$NEWPAGE" | wc -l` -eq 0 ];then
					if [ ! -e $TMP/$FILENAME.list ];then
						touch $TMP/$FILENAME.list
					fi
# obi
					LINE="$TITLE#$SRC $SRC hosterlist 0 0 $NEWPAGE#$PIC#$TMPPIC#$NAME#0"
#					LINE="$TITLE#$SRC $SRC play $NEWPAGE#$PIC#$TMPPIC#$NAME#111"
					echo "$LINE" >> $TMP/$FILENAME.list
				fi
			fi
		done 3<$TMP/cache.$FILENAME.2
		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi

	echo "$TMP/$FILENAME.list"
}

search()
{
	if [ ! -e "$TMP/$FILENAME.list" ] ; then
		if [ "$CURPAGE" -eq "1" ] ; then
			NEWPAGE=$PAGE
		else
			NEWPAGE=`echo $PAGE | sed "s/@PAGE@/$CURPAGE/g"`
		fi
#		$curlbin $URL/$NEWPAGE -o $TMP/cache.$FILENAME.1

#		/tmp/localhoster/hoster.sh get $URL/$PAGE > $TMP/cache.$FILENAME.1
		$BIN /tmp/localhoster/cloudflare.py "$URL/$PAGE" > $TMP/cache.$FILENAME.1

		cat $TMP/cache.$FILENAME.1 | tr '\n' ' ' | sed -e 's/<a href=/\n<a href=/g' -e 's/Film/\nFilm/g' | grep '^<a href=' | grep '<img src=' > $TMP/cache.$FILENAME.1a
		cat $TMP/cache.$FILENAME.1a | sed '/Stream in HD/d' > $TMP/cache.$FILENAME.2
		while read -u 3 ROUND; do
			TITLE=`echo $ROUND | sed 's/alt=/\nalt=/' | grep ^"alt=" | cut -d '"' -f2 | sed 's/#/%/'`
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/&%8211;/-/g' -e "s/&%8217;/'/g"`
			PIC=`echo $ROUND | sed 's/img src/\nsrc=/' | grep ^"src=" | cut -d '"' -f2`
			NEWPAGE=`echo $ROUND | sed 's/<a href=/\nhref=/' | grep ^"href=" | cut -d '"' -f2`
			if [ `echo $PIC | grep ^// | wc -l` -eq 1 ];then
				PIC=https:$PIC
			fi
			if [ `echo $NEWPAGE | grep ^// | wc -l` -eq 1 ];then
				NEWPAGE=https:$NEWPAGE
			fi

			if [ `echo $NEWPAGE | grep "/serie/" | wc -l` -eq 1 ];then
				TYPE=season
			else
				TYPE=hosterlist
			fi

			if [ -z  "$PIC" ]; then  
				PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
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
				if [ `cat $TMP/$FILENAME.list | grep ^"$NEWPAGE" | wc -l` -eq 0 ];then
					if [ ! -e $TMP/$FILENAME.list ];then
						touch $TMP/$FILENAME.list
					fi
# obi
					LINE="$TITLE#$SRC $SRC $TYPE 0 0 $NEWPAGE#$PIC#$TMPPIC#$NAME#0"
#					LINE="$TITLE#$SRC $SRC play 0 0 $NEWPAGE#$PIC#$TMPPIC#$NAME#111"
					echo "$LINE" >> $TMP/$FILENAME.list
				fi
			fi
		done 3<$TMP/cache.$FILENAME.2
		if [ "$CURPAGE" -eq "1" ] ; then
			PAGE="page/@PAGE@$PAGE"
			MAXPAGE=`cat $TMP/cache.$FILENAME.1 | sed '/<div class="pagination/!d;s/^.*Seite 1 von //;s/<\/span>.*$//'`
		fi
		if [ "$CURPAGE" -lt "$MAXPAGE" ] ; then
			NEWPAGE=`expr $CURPAGE + 1`
			echo "Page ($NEWPAGE/$MAXPAGE)#$SRC $SRC search $NEWPAGE $MAXPAGE '$PAGE'#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#$NAME#0" >> $TMP/$FILENAME.list
        fi

		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi
#	if [ -e "$TMP/$PARSER.hosterlist.list" ] ; then
#		rm $TMP/$PARSER.hosterlist.list
#	fi
	echo "$TMP/$FILENAME.list"
}

#/tmp/localparser/foxx.sh /tmp/localparser/foxx.sh hosterlist 0 0 https://foxx.to/film/72-stunden-next-three-days-2010-23914-Stream

hosterlist()
{
	if [ -e "$TMP/$FILENAME.list" ] ; then
		rm $TMP/$FILENAME.list
	fi
#	$curlbin $PAGE -o $TMP/cache.$FILENAME.1
	$BIN /tmp/localhoster/cloudflare.py "$PAGE" > $TMP/cache.$FILENAME.1

	cat $TMP/cache.$FILENAME.1 | sed 's/<iframe src=/\n<iframe src=/g' | grep -E ^"<iframe src=" | sed -e 's/<iframe src\=//g' | cut -d '"' -f2 >$TMP/cache.$FILENAME.2
	TEMP=$(cat $TMP/cache.$FILENAME.2)

	if [ `echo $TEMP | grep ^// | wc -l` -eq 1 ];then
		TEMP=https:$TEMP
	fi

#	$curlbin $TEMP -o $TMP/cache.$FILENAME.3
	$BIN /tmp/localhoster/cloudflare.py "$TEMP" > $TMP/cache.$FILENAME.3
	cat $TMP/cache.$FILENAME.3 | grep -A 10 "var decodeABC" >$TMP/cache.$FILENAME.4
	echo "print(decodeABC(jbdaskgs));" >> $TMP/cache.$FILENAME.4
	$DUKBIN $TMP/cache.$FILENAME.4 > $TMP/cache.$FILENAME.5

#exit

#[{"file":"https://lh3.googleusercontent.com/_KqlWWPoaC4gfQ0z_z9bK1y99QsnvN2eTSBz32Kd78Y7xLO_KoXveMe7TkimM0wpJl56HNTY=m18","label":"360p","type":"mp4"},{"file":"https://lh3.googleusercontent.com/_KqlWWPoaC4gfQ0z_z9bK1y99QsnvN2eTSBz32Kd78Y7xLO_KoXveMe7TkimM0wpJl56HNTY=m22","label":"720p","type":"mp4"}]
#{"error":"status=fail&hl=uk&allow_embed=0&ps=docs&partnerid=30&errorcode=100&reason=Ð¦Ðµ+Ð²ÑÐ´ÐµÐ¾+Ð½Ðµ+ÑÑÐ½ÑÑ.&timestamp=1532793394463"}
	errorcode=`cat /tmp/localcache/cache.foxx.hosterlist.5 | sed -nr 's/.*errorcode=([^=]+)&.*/\1/p'`

	cat $TMP/cache.$FILENAME.5 | grep -o "http[^ ]*"| sed -e 's!{! !g' -e 's/}//g' -e 's/{//g' -e 's/\"//g' -e 's/file/\nfile/g' -e 's/file://g' -e 's/type:mp4//g' -e 's/default:true//g' -e 's/\]//g' >$TMP/cache.$FILENAME.6
	echo >> $TMP/cache.$FILENAME.6
	while read -u 3 ROUND; do
		NEWPAGE=$(echo $ROUND | grep -o "http[^ ]*" | cut -d "," -f1)
		TITLE=$(echo $ROUND | grep -o "label[^ ]*" | cut -d "," -f1 | sed -e 's!label:!!g' -e 's!"!!g')
		if [ ! -z "$TITLE" ] && [ "$TITLE" != " " ] && [ ! -z "$NEWPAGE" ];then
			PIC=`echo $TITLE | tr [A-Z] [a-z]`
#ffplay -debug 1 "https://master.foxx.to/videoplayback?hash=df8cbe41-1fcd-468e-be40-90b2aeef6a9c&expireAt=1533097023189" -user-agent "Mozilla/5.0 (Windows NT 6.3; rv:36.0) Gecko/20100101 Firefox/36.0" -headers "cookie: cf_clearance=2ace7e2a04dc45592043701ae70fc07b06a4244d-1533078966-14400;"
#			REFERER=`echo "$TEMP" | sed -e 's/=/%3D/g' -e 's/&/%26/g'` 
			LINE="Http Stream ($TITLE)#$NEWPAGE|User-Agent=$USERAGENT#http://openaaf.dyndns.tv/mediathek/menu/foxx.jpg#foxx.jpg#$NAME#2"
			echo "$LINE" >> $TMP/$FILENAME.list
		fi
	done 3<$TMP/cache.$FILENAME.6
	rm $TMP/cache.$FILENAME.* > /dev/null 2>&1

	if [ ! -z "$errorcode" ];then
		ERRORMSG="Foxx Website error, this video file cannot be played. (Error Code: $errorcode)"
		LINE="$ERRORMSG#$NEWPAGE#$PIC#kinox_$piccount.jpg#Foxx#0"
		echo "$LINE" > $TMP/$FILENAME.list
	fi
	echo $TMP/$FILENAME.list

}

season()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then

#		$curlbin $PAGE -o $TMP/cache.$FILENAME.1
		$BIN /tmp/localhoster/cloudflare.py "$PAGE" > $TMP/cache.$FILENAME.1
		seasons=`cat $TMP/cache.$FILENAME.1 | sed -nr 's/.*class="se-t ">([^>]+)<.*/\1/p'`

		tags=""
		i=1
		if [ ! -z "$i" ] && [ ! -z "$seasons" ];then
			until [ "$i" -gt "$seasons" ]
			do
			TMPURL=`echo $TMPURL | sed -e "s/&season=.*//" -e "s/&episode=.*//" -e "s/&referrer=.*//"`
			echo "Season $i#$SRC $SRC episode $i 0 '$PAGE'#http://openaaf.dyndns.tv/mediathek/menu/s"$i".jpg#s"$i".jpg#$NAME#0" | sort -r >> $TMP/$FILENAME.list
			i=`expr $i + 1` 
			done
		fi

		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi
	echo $TMP/$FILENAME.list
}

episode()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin $PAGE -o $TMP/cache.$FILENAME.1
		$BIN /tmp/localhoster/cloudflare.py "$PAGE" > $TMP/cache.$FILENAME.1
		cat $TMP/cache.$FILENAME.1 | sed 's/<span class="se-t/\n<span class="se-t/g' | grep ^'<span class="se-t' | grep ">$CURPAGE</span><span class=" | sed 's/<li><div class/\nfound/g' | sed -e '/^ *$/d' > $TMP/cache.$FILENAME.2

		while read -u 3 ROUND; do
			#echo ROUND $ROUND
			#found="imagen"><a href="//foxx.to/episode/game-of-thrones-7x1"><img src="//foxx.to/wp-content/uploads/2017/09/3SB4PUzZAnY6HnZbVbktIZoopGs-300x170.jpg"></a></div><div class="numerando">7 - 1</div><div class="episodiotitle"><a href="//foxx.to/episode/game-of-thrones-7x1">Drachenstein</a><span class="date">Jul. 16, 2017</span></div></li>
			NEWPAGE=$(echo $ROUND | sed -nr 's/.*<a href="([^"]+)".*/\1/p')
			PIC=$(echo $ROUND | sed -nr 's/.*<img src="([^"]+)".*/\1/p')
			EXTRA=$(echo $ROUND | sed -nr 's/.*episodiotitle">.*">([^>.*].*)<\/span>.*/\1/p')
			TITLE=$(echo $ROUND | sed -nr 's/.*episodiotitle"><a href.*">([^>].*)<\/a><span.*/\1/p')
			NUM=$(echo $ROUND | sed -nr 's/.*numerando">([^>]+)<\/div>.*/\1/p')

			if [ `echo $PIC | grep ^// | wc -l` -eq 1 ];then
				PIC=https:$PIC
			fi
			if [ `echo $NEWPAGE | grep ^// | wc -l` -eq 1 ];then
				NEWPAGE=https:$NEWPAGE
			fi

			if [ ! -z "$TITLE" ] && [ ! -z "$NUM" ];then
				NUM=`echo $NUM | cut -d "-" -f2 | sed -e 's/ //g'`
				TITLE="$NUM - $TITLE"
			fi

			if [ ! -z "$TITLE" ] && [ ! -z "$EXTRA" ];then
				TITLE="$TITLE ($EXTRA)"
			fi
	#echo NEWPAGE $NEWPAGE
	#echo NUM $NUM
	#echo TITLE $TITLE
	#echo EXTRA $EXTRA
	#echo PIC $PIC
	#exit

			if [ ! -z "$TITLE" ] && [ "$TITLE" != " " ] && [ ! -z "$NEWPAGE" ];then
#				LINE="$TITLE#$SRC $SRC hosterlist 0 0 $NEWPAGE#$PIC#foxx.jpg#$NAME#0"
				LINE="$TITLE#$SRC $SRC hosterlist 0 0 $NEWPAGE#http://openaaf.dyndns.tv/mediathek/menu/s"$CURPAGE"e"$NUM".jpg#s"$CURPAGE"e"$NUM".jpg#$NAME#0"

				echo "$LINE" >> $TMP/$FILENAME.list
			fi
		done 3<$TMP/cache.$FILENAME.2
		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi
	echo $TMP/$FILENAME.list
}


hoster()
{
	if [ ! -e "$TMP/$FILENAME.list" ] ; then
#		$curlbin $URL/res/links -X POST --data "$PAGE" -o $TMP/cache.$FILENAME.1 -A 'Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Maxthon/4.4.7.3000 Chrome/30.0.1599.101 Safari/537.36'
		$curlbin $URL/res/links -X POST --data "$PAGE" -o $TMP/cache.$FILENAME.1
		echo "curl hoster " $curlbin $URL/res/links -X POST --data "$PAGE" -o $TMP/cache.$FILENAME.1
#		cat $TMP/cache.$FILENAME.1 | sed 's!url="http://dref.pw/?!\nstreamurl="!' | grep ^streamurl= | cut -d'"' -f2 | sed 's!%3A!:!g' | sed 's!%2F!/!g' > $TMP/$FILENAME.list

#		REFERER=`echo "$PAGE" | sed -e 's/=/%3D/g' -e 's/&/%26/g'` 
#		echo "$URL|User-Agent=$USERAGENT&Referer=$REFERER" >> $STREAMLIST
		cat $TMP/cache.$FILENAME.1 | sed 's!url="http://dref.pw/?!\nstreamurl="!' | grep ^streamurl= | cut -d'"' -f2 | sed 's!%3A!:!g' | sed 's!%2F!/!g'"|User-Agent=$USERAGENT&Referer=$REFERER" > $TMP/$FILENAME.list

		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi
	cat $TMP/$FILENAME.list
}

#obi
play()
{
	if [ -e "$TMP/$FILENAME.list" ] ; then
		rm $TMP/$FILENAME.list
	fi

#	$curlbin $PAGE -o $TMP/cache.$FILENAME.1
	$BIN /tmp/localhoster/cloudflare.py "$PAGE" > $TMP/cache.$FILENAME.1

	cat $TMP/cache.$FILENAME.1 | grep -E ^"<iframe src=" | sed -e 's/<iframe src\=//g' | cut -d '"' -f2 >$TMP/cache.$FILENAME.2
	TEMP=$(cat $TMP/cache.$FILENAME.2)
	$curlbin $TEMP -o $TMP/cache.$FILENAME.3
	cat $TMP/cache.$FILENAME.3 | grep -E "sources:*" >$TMP/cache.$FILENAME.4
	cat $TMP/cache.$FILENAME.4 | grep -o "http[^ ]*"| sed -e 's/,/ /g' -e 's/}//g' -e 's/{//g' -e 's/\"//g' -e 's/file/\nfile/g' -e 's/file://g' -e 's/type:mp4//g' -e 's/default:true//g' -e 's/\]//g' >$TMP/cache.$FILENAME.5

	cat $TMP/cache.$FILENAME.5 > $TMP/$PARSER.playlist.list

	echo "$TMP/$PARSER.playlist.list"
}

case $INPUT in
#	init) $INPUT;;
	mainmenu) $INPUT;;
	new) $INPUT;;
	search) $INPUT;;
	page) $INPUT;;
	season) $INPUT;;
	episode) $INPUT;;
	hosterlist) $INPUT;;
	hoster) $INPUT;;
	play) $INPUT;;
esac
