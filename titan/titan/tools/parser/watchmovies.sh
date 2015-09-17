#!/bin/bash
#

buildtype=$1
wgetbin="wget -T2 -t2 --waitretry=2"

rm cache.*
rm _liste
rm -rf _full/watchmovies
mkdir -p _full/watchmovies/streams
rm cache.*
touch cache.watchmovies.titanlist

BEGINTIME=`date +%s`
DATENAME=`date +"%Y.%m.%d_%H.%M.%S"`
echo "[watchmovies.sh] START (buildtype: $buildtype): $DATENAME" > _full/watchmovies/build.log

if [ "$buildtype" = "full" ];then
watchlist="
movies
tv-shows
cinema-movies
"
else
watchlist="
cinema-movies
"
fi

piccount=0
count=0
for ROUND0 in $watchlist; do
	count=`expr $count + 1`

	filename0=`echo $ROUND0 | tr '/' '-'`
	$wgetbin "http://watchmovies.to/$ROUND0" -O cache."$count"."$filename0"
	echo cache."$count"."$filename0"

############
	PAGES=""
	if [ "$ROUND0" = "tv-shows" ];then
		PAGES=`cat cache."$count"."$filename0" | grep "http://watchmovies.to/tv-shows/page-" | grep Last | sed 's/-/"/g' | cut -d'"' -f4`
	fi

	if [ "$ROUND0" = "movies" ];then
		PAGES=`cat cache."$count"."$filename0" | grep "http://watchmovies.to/movies/page-" | grep Last | sed 's/-/"/' | cut -d'"' -f3`
	fi
	echo PAGES $PAGES

	if [ ! -z "$PAGES" ];then
		pcount=0
		while [ "$pcount" -lt "$PAGES" ]
		do
			echo "pcount $pcount"
			pcount=`expr $pcount + 1`
			count=`expr $count + 1`
			if [ "$pcount" = "2" ]; then
				echo skipppppppppppppppppppppppppppppppppp
#				break
			fi

			if [ "$ROUND0" = "tv-shows" ];then
				page=tv-shows/page
				type=74
			else
				page=page
				type=73
			fi
			filename2=`echo $ROUND0 | tr '/' '-'`
			$wgetbin http://watchmovies.to/"$page"-"$pcount" -O cache."$count"."$filename2"
			input=`cat cache."$count"."$filename2" | tr '\r' ' ' | tr '\n' ' ' | sed 's/\t\+/ /g' | sed 's/ \+/ /g' | sed 's/<li id="entry_/\n\n<li id="entry_/g'| grep ^'<li id="entry_'  | tr ' ' '~'`
		
			for ROUND1 in $input; do
				count=`expr $count + 1`
				filename1=`echo $ROUND1 | tr '/' '-'`
				piccount=`expr $piccount + 1`	
				
				URL=`echo $ROUND1 | sed 's/<a~href=/\n<a~href=/' | grep "<a~href=" | head -n1 | cut -d'"' -f2 | tr '~' ' '`
				PIC=`echo $ROUND1 | sed 's/background-image:url(/\nbackground-image:url(/' | grep "background-image:url(" | head -n1 | sed 's/(/)/' | cut -d')' -f2 | tr '~' ' '`
		
				TITLE=`echo $ROUND1 | sed 's/title=/\ntitle=/' | grep title= | cut -d'"' -f2| tr '~' ' '| tr '-' ' ' | tr '/' ' ' | sed 's/ \+/ /g'`
				TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' # ' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/#/ /g' -e 's/\.\+/./g'`
		
				LANG=??
				if [ `echo $ROUND1 | grep "languages/13.png" | wc -l` -eq 1 ];then
					LANG=en
				fi
				
				TITLE=`echo $TITLE`" ($LANG)" 
				echo TITLE $TITLE
				echo URL $URL
				echo PIC $PIC
				echo LANG $LANG
		
				LINE="$TITLE#$URL#$PIC#watchmovies_$piccount.jpg#watchmovies#$type"
				if [ ! -z "$TITLE" ]; then
					echo $LINE >> cache.watchmovies.titanlist
				fi
				if [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
					echo $LINE >> cache.watchmovies."$filename0".titanlist	
				fi
			done
		done			
	else
		input=`cat cache."$count"."$filename0" | tr '\r' ' ' | tr '\n' ' ' | sed 's/\t\+/ /g' | sed 's/ \+/ /g' | sed 's/<div style="float:left/\n\n<div style="float:left/g'| grep ^'<div style="float:left'  | tr ' ' '~'`
	
###########################
		type=73
		for ROUND1 in $input; do
			count=`expr $count + 1`
			filename1=`echo $ROUND1 | tr '/' '-'`
			piccount=`expr $piccount + 1`	
			
			URL=`echo $ROUND1 | sed 's/<a~href=/\n<a~href=/' | grep "<a~href=" | head -n1 | cut -d'"' -f2 | tr '~' ' '`
			PIC=http://watchmovies.to/`echo $ROUND1 | sed 's/<img~src=/\n<img~src=/' | grep "<img~src=" | head -n1 | cut -d'"' -f2 | tr '~' ' '`
			
	
			TITLE=`echo $ROUND1 | sed 's/title=/\ntitle=/' | grep title= | cut -d'"' -f2| tr '~' ' '| tr '-' ' ' | tr '/' ' ' | sed 's/ \+/ /g'`
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' # ' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
	
			LANG=??
			if [ `echo $ROUND1 | grep "languages/13.png" | wc -l` -eq 1 ];then
				LANG=en
			fi
			
			TITLE=`echo $TITLE`" ($LANG)" 
			echo TITLE $TITLE
			echo URL $URL
			echo PIC $PIC
			echo LANG $LANG
	
			LINE="$TITLE#$URL#$PIC#watchmovies_$piccount.jpg#watchmovies#73"
			if [ ! -z "$TITLE" ]; then
				echo $LINE >> cache.watchmovies.titanlist
			fi
			if [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
				echo $LINE >> cache.watchmovies."$filename0".titanlist	
			fi
		done
	fi

	TITLE=`echo $ROUND0 | sed 's!-!!g'`
	piccount=`expr $piccount + 1`
	URL="http://atemio.dyndns.tv/mediathek/watchmovies/streams/watchmovies.$filename0.list"
	PIC="http://atemio.dyndns.tv/mediathek/menu/$filename0.jpg"
	
	LINE="$TITLE#$URL#$PIC#watchmovies_$piccount.jpg#watchmovies#0"
	if [ ! -z "$TITLE" ]; then
		echo $LINE >> cache.watchmovies.category.titanlist	
	fi
		
	if [ `cat cache.watchmovies.$filename0.titanlist | wc -l` -gt 0 ];then
		cat cache.watchmovies.$filename0.titanlist | sort -u > _full/watchmovies/streams/watchmovies.$filename0.list
	fi

done

if [ "$buildtype" = "full" ];then
	cat cache.watchmovies.titanlist | sort -u > _full/watchmovies/streams/watchmovies.all-sorted.list
	#cat cache.watchmovies.category.titanlist | sort -u > _full/watchmovies/watchmovies.category.list
	cat cache.watchmovies.category.titanlist > _full/watchmovies/watchmovies.category.list
	
	for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
		filename=`echo "$ROUND" | tr 'A-Z' 'a-z'`
		if [ `cat cache.watchmovies.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
			cat cache.watchmovies.titanlist | grep ^"$ROUND" > cache.watchmovies.titanlist."$ROUND"
			cat cache.watchmovies.titanlist."$ROUND" | sort -u > _full/watchmovies/streams/watchmovies.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
			echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/watchmovies/streams/watchmovies."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#watchmovies#0 >> _full/watchmovies/watchmovies.a-z.list
		elif [ `cat cache.watchmovies.titanlist | grep ^"$filename" | wc -l` -gt 0 ];then
			cat cache.watchmovies.titanlist | grep ^"$filename" > cache.watchmovies.titanlist."$ROUND"
			cat cache.watchmovies.titanlist."$ROUND" | sort -u > _full/watchmovies/streams/watchmovies.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
			echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/watchmovies/streams/watchmovies."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#watchmovies#0 >> _full/watchmovies/watchmovies.a-z.list
		fi
	done
fi

DONETIME=`date +%s`
TIME=`expr $DONETIME - $BEGINTIME`
echo "[watchmovies.sh] build time: ($TIME s) done" >> _full/watchmovies/build.log	
echo "[watchmovies.sh] watchmovies: "`ls -1 _full/watchmovies` >> _full/watchmovies/build.log
echo "[watchmovies.sh] watchmovies/streams: "`ls -1 _full/watchmovies/streams` >> _full/watchmovies/build.log

if [ "$buildtype" != "full" ];then
	cp -a _full/watchmovies/* /var/www/atemio/web/mediathek/watchmovies
fi

rm cache.*

exit
