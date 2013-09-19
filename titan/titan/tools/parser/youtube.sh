#!/bin/bash
#

buildtype=$1
wgetbin="wget -T2 -t2 --waitretry=2"

rm cache.*
rm -rf _full/youtube
mkdir -p _full/youtube/streams

BEGINTIME=`date +%s`
DATENAME=`date +"%Y.%m.%d_%H.%M.%S"`
echo "[youtube.sh] START (buildtype: $buildtype): $DATENAME" > _full/youtube/build.log

piccount=0

if [ "$buildtype" = "full" ];then
	SEARCHLIST="charts/trailers/most_popular?region=US&v=2 standardfeeds/DE/most_viewed_Music?v=2 standardfeeds/DE/most_viewed?v=2 standardfeedsDE/top_rated?v=2 videos/-/HD videos?q=bodyrock+tv videos?q=Zuzana+Light+ZWOW videos?q=titannit videos?q=trailer+2013+deutsch" 
else
	SEARCHLIST="videos?q=titannit videos?q=trailer+2013+deutsch" 
fi

for SEARCH in $SEARCHLIST; do
	echo SEARCH=$SEARCH 
	if [ $SEARCH == "charts/trailers/most_popular?region=US&v=2" ]; then
		filename=trailers_most_popular_us
	elif [ $SEARCH == "standardfeeds/DE/most_viewed_Music?v=2" ]; then
		filename=most_viewed_Music
	elif [ $SEARCH == "standardfeeds/DE/most_viewed?v=2" ]; then
		filename=most_viewed
	elif [ $SEARCH == "standardfeeds/DE/top_rated?v=2" ]; then
		filename=top_rated
	elif [ $SEARCH == "videos?q=bodyrock+tv" ]; then
		filename=bodyrock_tv
	elif [ $SEARCH == "videos?q=Zuzana+Light+ZWOW" ]; then
		filename=zuzana_light
	elif [ $SEARCH == "videos?q=titannit" ]; then
		filename=titannit
	elif [ $SEARCH == "videos?q=trailer+2013+deutsch" ]; then
		filename=trailer_2013_deutsch
	else
		filename=not_found	
	fi
																					
	rm cache.top_rated.list
	$wgetbin --no-check-certificate "http://gdata.youtube.com/feeds/api/$SEARCH&max-results=50" -O cache.$filename.list
	LIST=`cat cache.$filename.list | tr '><' '>\n<'| grep url | grep "http://i.ytimg.com/vi/" | grep "width='480'" | cut -d "/" -f5`
	
	for ROUND in $LIST; do
		echo round=$ROUND
#		URL=http://www.youtube.com/watch?v="$ROUND"
		URL="http://www.youtube.com/get_video_info?&video_id=$ROUND"

		PIC=http://i.ytimg.com/vi/"$ROUND"/1.jpg
		piccount=`expr $piccount + 1`
	
		$wgetbin --no-check-certificate http://www.youtube.com/watch?v=$ROUND -O cache.$filename.title.list
#		TITLE=`cat cache.$filename.title.list | grep '<meta name="title" content="' | sed 's/      <meta name="title" content="//' | sed 's/">//' | tr '&#' '%' | tr -d ';'`
		TITLE=`cat cache.$filename.title.list | grep '<meta name="title" content="' | sed 's/content=/\n/' | tail -n 1 | cut -d '"' -f2`
		TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' # ' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`

#ls cat cache.$filename.title.list 
#exit

		if [ -z "$TITLE" ]; then
			TITLE="not found"
		fi
		LINE="$TITLE#$URL#$PIC#youtube_$piccount.jpg#YouTube#4"
#		LINE="`echo $( php -r "$TITLE"; )`#$URL#$PIC#youtube_$piccount.jpg#YouTube#4"
		echo $LINE >> cache.youtube.$filename.titanlist
		echo $LINE >> cache.youtube.all.titanlist
	done
	LINE="`echo $filename | tr '_' ' '`#http://atemio.dyndns.tv/mediathek/youtube/streams/youtube.$filename.list#http://atemio.dyndns.tv/mediathek/menu/$filename.jpg#`echo "$filename" | tr 'A-Z' 'a-z'`.jpg#YouTube#3"

	if [ `cat cache.youtube.$filename.titanlist | wc -l` -gt 0 ];then
		cat cache.youtube.$filename.titanlist > _full/youtube/streams/youtube.$filename.list
		echo $LINE >> cache.youtube.category.titanlist
	fi

done


if [ "$buildtype" = "full" ];then
	#####################
	$wgetbin http://gdata.youtube.com/schemas/2007/categories.cat -O cache.categories.list 
	SEARCHLIST=`cat cache.categories.list | tr '><' '>\n<' | grep "atom:category term=" | tr ' ' '\n' | grep term=| cut -d"'" -f2`
	
	for SEARCH in $SEARCHLIST; do
		echo SEARCH=$SEARCH
		filename=`echo $SEARCH | tr 'A-Z' 'a-z'`																		
		$wgetbin --no-check-certificate "http://gdata.youtube.com/feeds/api/standardfeeds/DE/top_rated_$SEARCH?v=2&max-results=50" -O cache.$filename.list
		LIST=`cat cache.$filename.list | tr '><' '>\n<'| grep url | grep "http://i.ytimg.com/vi/" | grep "width='480'" | cut -d "/" -f5`
		
		for ROUND in $LIST; do
			echo round=$ROUND
	#		URL=http://www.youtube.com/watch?v="$ROUND"
			URL="http://www.youtube.com/get_video_info?&video_id=$ROUND"
			PIC=http://i.ytimg.com/vi/"$ROUND"/0.jpg
			piccount=`expr $piccount + 1`
		
			$wgetbin --no-check-certificate http://www.youtube.com/watch?v=$ROUND -O cache.$filename.title.list
	#		TITLE=`cat cache.$filename.title.list | grep '<meta name="title" content="' | sed 's/      <meta name="title" content="//' | sed 's/">//' | tr '&#' '%' | tr -d ';'`
			TITLE=`cat cache.$filename.title.list | grep '<meta name="title" content="' | sed 's/content=/\n/' | tail -n 1 | cut -d '"' -f2`
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' # ' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
	
	
	
			if [ -z "$TITLE" ]; then
				TITLE="not found"
			fi
			LINE="$TITLE#$URL#$PIC#youtube_$piccount.jpg#YouTube#4"
			echo $LINE >> cache.youtube.$filename.titanlist
			echo $LINE >> cache.youtube.all.titanlist
		done
		LINE="`echo $filename | tr '_' ' '`#http://atemio.dyndns.tv/mediathek/youtube/streams/youtube.$filename.list#http://atemio.dyndns.tv/mediathek/menu/$filename.jpg#`echo "$filename" | tr 'A-Z' 'a-z'`.jpg#YouTube#3"
	
		if [ `cat cache.youtube.$filename.titanlist | wc -l` -gt 0 ];then
			cat cache.youtube.$filename.titanlist > _full/youtube/streams/youtube.$filename.list
			echo $LINE >> cache.youtube.category.titanlist
		fi
	
	done
	
	cat cache.youtube.category.titanlist > _full/youtube/youtube.category.list
	cat cache.youtube.all.titanlist | sort -u > _full/youtube/streams/youtube.all-sorted.list	
	
	for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z a b c d e f g h i j k l m n o p q r s t u v w x y z; do
		filename=`echo "$ROUND" | tr 'A-Z' 'a-z'`
		if [ `cat cache.youtube.all.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
			cat cache.youtube.all.titanlist | grep ^"$ROUND" > cache.youtube.all.titanlist."$ROUND"
			cat cache.youtube.all.titanlist."$ROUND" | sort -u > _full/youtube/streams/youtube.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
			echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/youtube/streams/youtube."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#YouTube#3 >> _full/youtube/youtube.a-z.list
		elif [ `cat cache.youtube.all.titanlist | grep ^"$filename" | wc -l` -gt 0 ];then
			cat cache.youtube.all.titanlist | grep ^"$filename" > cache.youtube.all.titanlist."$ROUND"
			cat cache.youtube.all.titanlist."$ROUND" | sort -u > _full/youtube/streams/youtube.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
			echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/youtube/streams/youtube."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#YouTube#3 >> _full/youtube/youtube.a-z.list
		fi
	done
fi

DONETIME=`date +%s`
TIME=`expr $DONETIME - $BEGINTIME`
echo "[youtube.sh] build time: ($TIME s) done" >> _full/youtube/build.log	
echo "[youtube.sh] youtube: "`ls -1 _full/youtube` >> _full/youtube/build.log
echo "[youtube.sh] youtube/streams: "`ls -1 _full/youtube/streams` >> _full/youtube/build.log

if [ "$buildtype" != "full" ];then
	cp -a _full/youtube/* /var/www/atemio/web/mediathek/youtube
fi

rm cache.*

exit
