#!/bin/bash
#

rm cache.*
rm _liste
rm -rf _full/mlhhd
mkdir -p _full/mlhhd/streams
rm cache.*
touch cache.mlhhd.titanlist

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
	wget "http://$ROUND2" -O cache."$count"."$filename2"

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
#		wget "$ROUND0" -O cache.urlsrc."$icount"
#		playlist=`cat cache.urlsrc."$icount" | tr '\015' '\012' | grep playlis | cut -d"'" -f2`
#		wget "$playlist" -O cache.urlstream."$icount"
#		STREAMURL=`cat cache.urlstream."$icount" | sed 's!location>!\nlocation<!' | grep location | cut -d'<' -f2`
		type=37
#		if [ ! -z $STREAMURL ] && [ `echo $STREAMURL | grep "http://" | wc -l` -eq 1 ];then
#			URL=$STREAMURL
#			type=2
#		fi
####		
		if [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
			LINE="$TITLE#$URL#$PIC#mlhhd_$piccount.jpg#Mle-HD#$type"
			if [ `cat cache.mlhhd.newest.titanlist | grep "^$TITLE" | wc -l` -eq 0 ];then
				echo $LINE >> cache.mlhhd.newest.titanlist
			fi
			if [ `cat cache.mlhhd.titanlist | grep "^$TITLE" | wc -l` -eq 0 ];then
				echo $LINE >> cache.mlhhd.titanlist
			fi
		fi
	done

	piccount=`expr $piccount + 1`
	URL="http://atemio.dyndns.tv/mediathek/mlhhd/streams/mlhhd.newest.list"
	PIC="http://atemio.dyndns.tv/mediathek/menu/Movies.update.jpg"
	TITLE="Letzte Uploads"
	LINE="$TITLE#$URL#$PIC#mlhhd_$piccount.jpg#Mle-HD#3"
	echo $LINE >> cache.mlhhd.category.titanlist

###

	catlist="`cat cache.$count.$filename2 | grep "/category/" | cut -d'/' -f5`"	
#echo catlist $catlist
#echo sleep 20
#sleep 20
#exit
	for ROUND3 in $catlist; do
		count=`expr $count + 1`
		filename3="$ROUND3"
		wget "http://www.mle-hd.se/category/$ROUND3/" -O cache."$count"."$filename3"
		pagelist="http://www.mle-hd.se/category/$ROUND3/ "`cat cache."$count"."$filename3" | grep "Page 1 of" | tr "'" '\n' | grep ^http`
		for ROUND4 in $pagelist; do
			count=`expr $count + 1`
			filename4=`echo $ROUND4 | sed 's!http://www.mle-hd.se/!!' | tr '/' '.'`
			piccount=`expr $piccount + 1`
			wget "$ROUND4" -O cache."$count"."$filename4"		
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
#				wget "$ROUND5" -O cache.urlsrc."$icount"
#				playlist=`cat cache.urlsrc."$icount" | tr '\015' '\012' | grep playlis | cut -d"'" -f2`
#				wget "$playlist" -O cache.urlstream."$icount"
#				STREAMURL=`cat cache.urlstream."$icount" | sed 's!location>!\nlocation<!' | grep location | cut -d'<' -f2`
				type=37
#				if [ ! -z $STREAMURL ] && [ `echo $STREAMURL | grep "http://" | wc -l` -eq 1 ];then
#					URL=$STREAMURL
#					type=2
#				fi
				
				if [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
					LINE="$TITLE#$URL#$PIC#mlhhd_$piccount.jpg#Mle-HD#$type"
					if [ `cat cache.mlhhd.category.$ROUND3.titanlist | grep "^$TITLE" | wc -l` -eq 0 ];then
						echo $LINE >> cache.mlhhd.category.$ROUND3.titanlist
					fi
					if [ `cat cache.mlhhd.titanlist | grep "^$TITLE" | wc -l` -eq 0 ];then
						echo $LINE >> cache.mlhhd.titanlist
					fi
				fi
			done
		done

		cat cache.mlhhd.category.$ROUND3.titanlist > _full/mlhhd/streams/mlhhd.category.$ROUND3.list
			
		piccount=`expr $piccount + 1`
		URL="http://atemio.dyndns.tv/mediathek/mlhhd/streams/mlhhd.category.$ROUND3.list"
		PIC="http://atemio.dyndns.tv/mediathek/menu/$ROUND3.jpg"
		TITLE="$ROUND3"
		LINE="$TITLE#$URL#$PIC#mlhhd_$piccount.jpg#Mle-HD#3"
		echo $LINE >> cache.mlhhd.category.titanlist
	done
done

cat cache.mlhhd.newest.titanlist > _full/mlhhd/streams/mlhhd.newest.list
cat cache.mlhhd.titanlist | sort -um > _full/mlhhd/streams/mlhhd.all-sorted.list
#cat cache.mlhhd.category.titanlist | sort -um > _full/mlhhd/mlhhd.category.list
cat cache.mlhhd.category.titanlist > _full/mlhhd/mlhhd.category.list

rm -rf _full
cp /home/obiwan/test/_ba/mlhhd/streams/mlhhd.all-sorted.list cache.mlhhd.titanlist
mkdir -p _full/mlhhd/streams/

for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
	filename=`echo "$ROUND" | tr 'A-Z' 'a-z'`
	if [ `cat cache.mlhhd.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
		cat cache.mlhhd.titanlist | grep ^"$ROUND" >> cache.mlhhd.titanlist."$ROUND"
		cat cache.mlhhd.titanlist."$ROUND" | sort -um > _full/mlhhd/streams/mlhhd.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
		echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/mlhhd/streams/mlhhd."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#Mle-HD#3 >> _full/mlhhd/mlhhd.a-z.list
	elif [ `cat cache.mlhhd.titanlist | grep ^"$filename" | wc -l` -gt 0 ];then
		cat cache.mlhhd.titanlist | grep ^"$filename" >> cache.mlhhd.titanlist."$ROUND"
		cat cache.mlhhd.titanlist."$ROUND" | sort -um > _full/mlhhd/streams/mlhhd.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
		echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/mlhhd/streams/mlhhd."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#Mle-HD#3 >> _full/mlhhd/mlhhd.a-z.list

	fi
done

#exit
#echo lostcount $lostcount

cp -a _full/mlhhd/* /var/www/atemio/web/mediathek/mlhhd
#cp -a mainmenu.list /var/www/atemio/web/mediathek
#cp -a mainmenu-mlhhd.list /var/www/atemio/web/mediathek

rm file.*
rm cache.*
