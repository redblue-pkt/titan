#!/bin/bash
#

buildtype=$1
wgetbin="wget -T2 -t2 --waitretry=2"

rm cache.*
rm _liste
rm -rf _full/ard

mkdir -p _full/ard/streams
piccount=0

BEGINTIME=`date +%s`
DATENAME=`date +"%Y.%m.%d_%H.%M.%S"`
echo "[ard.sh] START (buildtype: $buildtype): $DATENAME" > _full/ard/build.log

##################
# watchlist
##################

watchlist="
New;/ard/servlet/ajax-cache/3516220/view=switch/index.html
Most~Viewed;/ard/servlet/ajax-cache/3516210/view=list/show=recent/index.html
Best~Rated;/ard/servlet/ajax-cache/3516188/view=switch/index.html
Documentaries;/ard/servlet/ajax-cache/3474718/view=switch/index.html
Movie~Highlights;/ard/servlet/ajax-cache/4585472/view=switch/index.html
"
watchlist1="
All Shows A-Z
Favorites (shows)
Categorys
Dossiers
Search
Das Erste - Live
"

for ROUND1 in $watchlist; do
	count=`expr $count + 1`
	filename=`echo $ROUND1 | cut -d ";" -f1 | tr 'A-Z' 'a-z' | tr '~' '.'`
	section=`echo $ROUND1 | cut -d ";" -f1 | tr '~' ' '`
	geturl=`echo $ROUND1 | cut -d ";" -f2`

	wget http://www.ardmediathek.de$geturl -O cache.$filename.$count.html
	searchlist=`cat cache.$filename.$count.html | tr '\r' '\n' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's/ \+/~/g' | sed 's!<div~class="mt-media_item">!\n\n<div~class="mt-media_item">!g' | grep ^'<div~class="mt-media_item">'`

	for ROUND2 in $searchlist; do
		piccount=`expr $piccount + 1`
		count=`expr $count + 1`

		URL=http://www.ardmediathek.de`echo $ROUND2 | sed 's!<a~href=!\nurl=!' | grep url= | cut -d'"' -f2`
		PIC=http://www.ardmediathek.de`echo $ROUND2 | sed 's!<img~src=!\npic=!' | grep pic= | cut -d'"' -f2`		
		TITLE=`echo $ROUND2 | sed 's!data-xtclib=!\ndata-xtclib=!' | grep "data-xtclib=" | cut -d'"' -f2 | tr '~' ' '`
		TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/&#043;/+/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/#043;/+/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/+/ /g' -e 's/,/ /g' -e 's/;/ /g' -e 's/\.\+/./g' -e 's/+/ /g' -e 's/#/ /g'`
		TITLE=`echo $(php -r "echo rawurldecode('$TITLE');")`

		AIRTIME=`echo $ROUND2 | sed 's!<span~class="mt-airtime">!\n<span~class="mt-airtime"<!' | grep '<span~class="mt-airtime"<' | cut -d'<' -f3 | tr '~' ' '`
		TITLE="`echo $TITLE` (`echo $AIRTIME`)"

		wget $URL -O cache.$filename.$count.list
	
		URL=`cat cache.$filename.$count.list | grep .mp4 | grep http | sed 's!http://!\nhttp://!g' | grep ^"http://" | cut -d'"' -f1 | tail -n1`
		
		LINE="$TITLE#$URL#$PIC#ard_$piccount.jpg#ARD#2"
		if [ ! -z "$TITLE" ] && [ ! -z "$URL" ] && [ `cat cache.ard."$filename".titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
			echo $LINE >> cache.ard."$filename".titanlist
		fi

		if [ ! -z "$TITLE" ] && [ ! -z "$URL" ] && [ `cat cache.ard.titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
			echo $LINE >> cache.ard.titanlist
		else
			echo $LINE >> cache.ard.error.titanlist
		fi
	done
	if [ -e cache.ard."$filename".titanlist ];then
		piccount=`expr $piccount + 1`
		URL="http://atemio.dyndns.tv/mediathek/ard/streams/ard."$filename".list"
		PIC="http://atemio.dyndns.tv/mediathek/menu/"$filename".jpg"
		LINE="$section#$URL#$PIC#ard_$piccount.jpg#ARD#3"
		echo $LINE >> cache.ard.category.titanlist
		cat cache.ard."$filename".titanlist > _full/ard/streams/ard."$filename".list
	fi
	
done

###############
# shows
###############

if [ "$buildtype" = "full" ];then
	for ROUND in A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
		watchlist="$watchlist $ROUND;/ard/servlet/ajax-cache/3474820/view=list/initial=$ROUND/index.html"
	done
			
	skipcount=0
	for ROUND0 in $watchlist; do
		count=`expr $count + 1`
		filename=`echo $ROUND0 | cut -d ";" -f1 | tr 'A-Z' 'a-z' | tr '~' '.'`
		section=`echo $ROUND0 | cut -d ";" -f1 | tr '~' ' '`
		geturl=`echo $ROUND0 | cut -d ";" -f2`
		wget http://www.ardmediathek.de$geturl -O cache.$filename.$count.html
		if [ `du cache.$filename.$count.html | cut -d"/" -f1 | tr '\t' '\n' | head -n1` = 0 ];then
			echo 000000000000000000
			exit
		fi
				
		searchlist1=`cat cache.$filename.$count.html | tr '\r' '\n' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's/ \+/~/g' | sed 's!<div~class="mt-media_item">!\n\n<div~class="mt-media_item">!g' | grep ^'<div~class="mt-media_item">'`

		for ROUND1 in $searchlist1; do
			piccount=`expr $piccount + 1`
			count=`expr $count + 1`
			URL=http://www.ardmediathek.de`echo $ROUND1 | sed 's!<a~href=!\nurl=!' | grep url= | cut -d'"' -f2`
			PIC=http://www.ardmediathek.de`echo $ROUND1 | sed 's!<img~src=!\npic=!' | grep pic= | cut -d'"' -f2`		
			TITLE=`echo $ROUND1 | sed 's!data-xtclib=!\ndata-xtclib=!' | grep "data-xtclib=" | cut -d'"' -f2 | tr '~' ' '`
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/&#043;/+/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/#043;/+/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/+/ /g' -e 's/,/ /g' -e 's/;/ /g' -e 's/\.\+/./g' -e 's/+/ /g' -e 's/#/ /g'`
			TITLE=`echo $TITLE | sed 's/\(.*\)\./\1\t/g' | sed 's/^\.//g'`
			TITLE=`echo $(php -r "echo rawurldecode('$TITLE');")`
			TAGTITLE=$TITLE
			filename2=`echo $TITLE | tr 'A-Z' 'a-z' | tr '(' '.' | tr ')' '.' | tr '-' '.' | tr '_' '.' | tr '~' '.' | tr '|' '.' | tr ',' '.' | tr '"' '.' | tr ':' '.' | tr ' ' '.' | tr '+' '.' | tr '!' '.' | sed 's!%2!!g' | sed 's!&!.!g' | sed 's/\.\+/./g'`

			ID=`echo $URL | cut -d"=" -f2`
			
			URL=http://www.ardmediathek.de/ard/servlet/ajax-cache/3516962/view=list/documentId="$ID"/goto=1/index.html
	
			wget $URL -O cache.$filename."$filename2".$count.list
				
			sectionlist2=`cat cache.$filename."$filename2".$count.list | grep 'value="/ard/servlet/ajax-cache' | sed 's!value="!\n\n\nvalue="!' | grep ^value= | cut -d'"' -f2`

			for ROUND2 in $sectionlist2; do
				count=`expr $count + 1`
				
				wget http://www.ardmediathek.de$ROUND2 -O cache.$filename."$filename2".$count.list

				searchlist3=`cat cache.$filename."$filename2".$count.list | tr '\r' '\n' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's/ \+/~/g' | sed 's!<div~class="mt-media_item">!\r\n<div~class="mt-media_item">!g' | grep ^'<div~class="mt-media_item">' | grep -v '<h3~class="mt-title">~<a>~<span~class' | grep -v '<div~class="mt-media_item">~<form~action='`
				for ROUND3 in $searchlist3; do
					piccount=`expr $piccount + 1`
					count=`expr $count + 1`
					skipcount=`expr $skipcount + 1`
	
					URL=http://www.ardmediathek.de`echo $ROUND3 | sed 's!<a~href=!\nurl=!' | grep url= | cut -d'"' -f2`
					PIC=http://www.ardmediathek.de`echo $ROUND3 | sed 's!<img~src=!\npic=!' | grep pic= | cut -d'"' -f2`		
					TITLE=`echo $ROUND3 | sed 's!data-xtclib=!\ndata-xtclib=!' | grep "data-xtclib=" | cut -d'"' -f2 | tr '~' ' '`
					TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/&#043;/+/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/#043;/+/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/+/ /g' -e 's/,/ /g' -e 's/;/ /g' -e 's/\.\+/./g' -e 's/+/ /g' -e 's/#/ /g'`
					TITLE=`echo $(php -r "echo rawurldecode('$TITLE');")`

					AIRTIME=`echo $ROUND3 | sed 's!<span~class="mt-airtime">!\n<span~class="mt-airtime"<!' | grep '<span~class="mt-airtime"<' | cut -d'<' -f3 | tr '~' ' '`

					TITLE="`echo $TITLE` (`echo $AIRTIME`)"

#echo ROUND3 $ROUND3
#echo TITLE $TITLE
#exit
					wget $URL -O cache.$filename.$count.list
#					if [ `du cache.$filename.$count.list | cut -d"/" -f1 | tr '\t' '\n' | head -n1` = 0 ];then
#						echo 333333333333333aaaaaaaaaaaaaaaaa
#						exit
#					fi
#				
					URL=`cat cache.$filename.$count.list | grep .mp4 | grep http | sed 's!http://!\nhttp://!g' | grep ^"http://" | cut -d'"' -f1 | tail -n1`
				
					LINE="$TITLE#$URL#$PIC#ard_$piccount.jpg#ARD#2"
					if [ ! -z "$TITLE" ] && [ ! -z "$URL" ] && [ `cat cache.ard.shows."$filename2".titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
						echo $LINE >> cache.ard.shows."$filename2".titanlist
					fi
			
					if [ ! -z "$TITLE" ] && [ ! -z "$URL" ] && [ `cat cache.ard.titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
						echo $LINE >> cache.ard.titanlist
					else
						echo $LINE >> cache.ard.error.titanlist
					fi
				done
			done
			if [ -e cache.ard.shows."$filename2".titanlist ];then
				piccount=`expr $piccount + 1`
				URL="http://atemio.dyndns.tv/mediathek/ard/streams/ard.shows."$filename2".list"
				PIC="http://atemio.dyndns.tv/mediathek/menu/"$filename2".jpg"
				LINE="$TAGTITLE#$URL#$PIC#ard_$piccount.jpg#ARD#0"
				if [ ! -z "$TITLE" ] && [ ! -z "$URL" ] && [ `cat cache.ard.shows.titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
					echo $LINE >> cache.ard.shows.titanlist
				fi
				cat cache.ard.shows."$filename2".titanlist > _full/ard/streams/ard.shows."$filename2".list
			fi
		done
	done
	
	if [ -e cache.ard.shows.titanlist ];then
		piccount=`expr $piccount + 1`
		URL="http://atemio.dyndns.tv/mediathek/ard/ard.shows.list"
		PIC="http://atemio.dyndns.tv/mediathek/menu/shows.jpg"
		LINE="All Shows#$URL#$PIC#ard_$piccount.jpg#ARD#0"
		echo $LINE >> cache.ard.category.titanlist
		cat cache.ard.shows.titanlist | sort -u > _full/ard/ard.shows.list
	fi
fi

##############
if [ "$buildtype" = "full" ];then
	cat cache.ard.titanlist | sort -u > _full/ard/streams/ard.all-sorted.list
#	cat cache.ard.category.titanlist | sort -u > _full/ard/ard.category.list
	cat cache.ard.category.titanlist > _full/ard/ard.category.list
	
	for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
		filename=`echo "$ROUND" | tr 'A-Z' 'a-z'`
		if [ `cat cache.ard.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
			cat cache.ard.titanlist | grep ^"$ROUND" > cache.ard.titanlist."$ROUND"
			cat cache.ard.titanlist."$ROUND" | sort -u > _full/ard/streams/ard.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
			echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/ard/streams/ard."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#ARD#3 >> _full/ard/ard.a-z.list
		elif [ `cat cache.ard.titanlist | grep ^"$filename" | wc -l` -gt 0 ];then
			cat cache.ard.titanlist | grep ^"$filename" > cache.ard.titanlist."$ROUND"
			cat cache.ard.titanlist."$ROUND" | sort -u > _full/ard/streams/ard.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
			echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/ard/streams/ard."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#ARD#3 >> _full/ard/ard.a-z.list
		fi
	done
fi

DONETIME=`date +%s`
TIME=`expr $DONETIME - $BEGINTIME`
echo "[ard.sh] build time: ($TIME s) done" >> _full/ard/build.log	
echo "[ard.sh] ard: "`ls -1 _full/beeg` >> _full/ard/build.log
echo "[ard.sh] ard/streams: "`ls -1 _full/ard/streams` >> _full/ard/build.log

if [ "$buildtype" != "full" ];then
	cp -a _full/ard/* /var/www/atemio/web/mediathek/ard
fi

rm cache.*
exit