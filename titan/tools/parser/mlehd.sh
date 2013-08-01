#!/bin/bash
#

wgetbin="wget -T2 -t2 --waitretry=2"

rm cache.*
rm _liste
rm -rf _full/mlehd
mkdir -p _full/mlehd/streams
rm cache.*
touch cache.mlehd.titanlist

# filme a-z
watchlist="
www.mle-hd.se
"

testcount=0
piccount=0
count=0
icount=0
for ROUND2 in $watchlist; do
	count=`expr $count + 1`	
	filename2="$ROUND2"
	$wgetbin "http://$ROUND2" -O cache."$count"."$filename2"

	id_list="`cat cache."$count"."$filename2" | grep "opacity" | cut -d'"' -f2`"
	for ROUND0 in $id_list; do
		piccount=`expr $piccount + 1`
		PIC=`cat cache."$count"."$filename2" | grep "$ROUND0" | grep "src=" | cut -d'"' -f12`
		TITLE=`echo $ROUND0 | cut -d'/' -f4 | tr '-' ' '`
		URL="$ROUND0"

		if [ -z "$PIC" ]; then
			PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
		fi

###
#		icount=`expr $icount + 1`
#		$wgetbin "$ROUND0" -O cache.urlsrc."$icount"
#		playlist=`cat cache.urlsrc."$icount" | tr '\015' '\012' | grep playlis | cut -d"'" -f2`
#		$wgetbin "$playlist" -O cache.urlstream."$icount"
#		STREAMURL=`cat cache.urlstream."$icount" | sed 's!location>!\nlocation<!' | grep location | cut -d'<' -f2`
		type=38
#		if [ ! -z $STREAMURL ] && [ `echo $STREAMURL | grep "http://" | wc -l` -eq 1 ];then
#			URL=$STREAMURL
#			type=2
#		fi
####		
		if [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
			LINE="$TITLE#$URL#$PIC#mlehd_$piccount.jpg#Mle-HD#$type"
			if [ `cat cache.mlehd.newest.titanlist | grep "^$TITLE" | wc -l` -eq 0 ];then
				echo $LINE >> cache.mlehd.newest.titanlist
			fi
			if [ `cat cache.mlehd.titanlist | grep "^$TITLE" | wc -l` -eq 0 ];then
				echo $LINE >> cache.mlehd.titanlist
			fi
		fi
	done

	piccount=`expr $piccount + 1`
	URL="http://atemio.dyndns.tv/mediathek/mlehd/streams/mlehd.newest.list"
	PIC="http://atemio.dyndns.tv/mediathek/menu/Movies.update.jpg"
	TITLE="Letzte Uploads"
	LINE="$TITLE#$URL#$PIC#mlehd_$piccount.jpg#Mle-HD#3"
	echo $LINE >> cache.mlehd.category.titanlist

###

	catlist="`cat cache.$count.$filename2 | grep "/category/" | cut -d'/' -f5`"	
#echo catlist $catlist
#echo sleep 20
#sleep 20
#exit
	for ROUND3 in $catlist; do
		count=`expr $count + 1`
		filename3="$ROUND3"
		$wgetbin "http://www.mle-hd.se/category/$ROUND3/" -O cache."$count"."$filename3"
		pagelist="http://www.mle-hd.se/category/$ROUND3/ "`cat cache."$count"."$filename3" | grep "Page 1 of" | tr "'" '\n' | grep ^http`
		for ROUND4 in $pagelist; do
			count=`expr $count + 1`
			filename4=`echo $ROUND4 | sed 's!http://www.mle-hd.se/!!' | tr '/' '.'`
			piccount=`expr $piccount + 1`
			$wgetbin "$ROUND4" -O cache."$count"."$filename4"		
			id_list="`cat cache."$count"."$filename4" | grep "opacity" | cut -d'"' -f2`"
			for ROUND5 in $id_list; do
				piccount=`expr $piccount + 1`
				PIC=`cat cache."$count"."$filename4" | grep "$ROUND5" | grep "src=" | cut -d'"' -f12`
				TITLE=`echo $ROUND5 | cut -d'/' -f4 | tr '-' ' '`
				URL="$ROUND5"

				if [ -z "$PIC" ]; then
					PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
				fi
		
#				icount=`expr $icount + 1`
#				$wgetbin "$ROUND5" -O cache.urlsrc."$icount"
#				playlist=`cat cache.urlsrc."$icount" | tr '\015' '\012' | grep playlis | cut -d"'" -f2`
#				$wgetbin "$playlist" -O cache.urlstream."$icount"
#				STREAMURL=`cat cache.urlstream."$icount" | sed 's!location>!\nlocation<!' | grep location | cut -d'<' -f2`
				type=38
#				if [ ! -z $STREAMURL ] && [ `echo $STREAMURL | grep "http://" | wc -l` -eq 1 ];then
#					URL=$STREAMURL
#					type=2
#				fi
				
				if [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
					LINE="$TITLE#$URL#$PIC#mlehd_$piccount.jpg#Mle-HD#$type"
					if [ `cat cache.mlehd.category.$ROUND3.titanlist | grep "^$TITLE" | wc -l` -eq 0 ];then
						echo $LINE >> cache.mlehd.category.$ROUND3.titanlist
					fi
					if [ `cat cache.mlehd.titanlist | grep "^$TITLE" | wc -l` -eq 0 ];then
						echo $LINE >> cache.mlehd.titanlist
					fi
				fi
			done
		done

		cat cache.mlehd.category.$ROUND3.titanlist > _full/mlehd/streams/mlehd.category.$ROUND3.list
			
		piccount=`expr $piccount + 1`
		URL="http://atemio.dyndns.tv/mediathek/mlehd/streams/mlehd.category.$ROUND3.list"
		PIC="http://atemio.dyndns.tv/mediathek/menu/$ROUND3.jpg"
		TITLE="$ROUND3"
		LINE="$TITLE#$URL#$PIC#mlehd_$piccount.jpg#Mle-HD#3"
		echo $LINE >> cache.mlehd.category.titanlist
	done
done

cat cache.mlehd.newest.titanlist > _full/mlehd/streams/mlehd.newest.list
cat cache.mlehd.titanlist | sort -um > _full/mlehd/streams/mlehd.all-sorted.list
#cat cache.mlehd.category.titanlist | sort -um > _full/mlehd/mlehd.category.list
cat cache.mlehd.category.titanlist > _full/mlehd/mlehd.category.list

for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
	filename=`echo "$ROUND" | tr 'A-Z' 'a-z'`
	if [ `cat cache.mlehd.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
		cat cache.mlehd.titanlist | grep ^"$ROUND" >> cache.mlehd.titanlist."$ROUND"
		cat cache.mlehd.titanlist."$ROUND" | sort -um > _full/mlehd/streams/mlehd.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
		echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/mlehd/streams/mlehd."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#Mle-HD#3 >> _full/mlehd/mlehd.a-z.list
	elif [ `cat cache.mlehd.titanlist | grep ^"$filename" | wc -l` -gt 0 ];then
		cat cache.mlehd.titanlist | grep ^"$filename" >> cache.mlehd.titanlist."$ROUND"
		cat cache.mlehd.titanlist."$ROUND" | sort -um > _full/mlehd/streams/mlehd.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
		echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/mlehd/streams/mlehd."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#Mle-HD#3 >> _full/mlehd/mlehd.a-z.list

	fi
done

#exit
#echo lostcount $lostcount

#cp -a _full/mlehd/* /var/www/atemio/web/mediathek/mlehd
#cp -a mainmenu.list /var/www/atemio/web/mediathek
#cp -a mainmenu-mlehd.list /var/www/atemio/web/mediathek

rm file.*
rm cache.*
