#!/bin/bash
#

buildtype=$1
wgetbin="wget -T2 -t2 --waitretry=2"

rm cache.*
rm _liste
rm -rf _full/internetradio
mkdir -p _full/internetradio/streams
rm cache.*
touch cache.internetradio.titanlist

BEGINTIME=`date +%s`
DATENAME=`date +"%Y.%m.%d_%H.%M.%S"`
echo "[internetradio.sh] START (buildtype: $buildtype): $DATENAME" > _full/internetradio/build.log

if [ "$buildtype" == "full" ];then
	testcount=0
	piccount=0
	count=0
	icount=0
	
	count=`expr $count + 1`	
	$wgetbin "http://dir.xiph.org/yp.xml" -O cache.mainfile
	
	id_list="`cat cache.mainfile | tr '\n' ' ' | sed 's/ \+/ /g' | sed 's/<entry>/\n\n<entry>/g' | grep ^"<entry>" | tr ' ' '~'`"
	id_list_count="`cat cache.mainfile | tr '\n' ' ' | sed 's/ \+/ /g' | sed 's/<entry>/\n\n<entry>/g' | grep ^"<entry>" | tr ' ' '~' | wc -l`"
	
	count1=0
	for ROUND1 in $id_list; do
		piccount=`expr $piccount + 1`
		count1=`expr $count1 + 1`
		echo "add($count1/$id_list_count)"
	
#	if [ $count1 == 200 ]; then
#		break
#	fi
		URL=`echo $ROUND1 | sed 's/<listen_url>/\nurl=</g' | grep ^"url=" | tr '~' ' ' | cut -d '<' -f2`
		GENRE=`echo $ROUND1 | sed 's/<genre>/\ngenre=</g' | grep ^"genre=" | tr '~' ' ' | cut -d '<' -f2`
		GENRE=`echo $GENRE | sed 's/&#038;/&/g'`
		GENRE=`echo $GENRE | sed 's/&amp;/und/g'`
		GENRE=`echo $GENRE | sed 's/&quot;/"/g'`
		GENRE=`echo $GENRE | sed 's/&lt;/\</g'`
		GENRE=`echo $GENRE | sed 's/&#034;/\"/g'`
		GENRE=`echo $GENRE | sed 's/&#039;/\"/g'` # '
		GENRE=`echo $GENRE | sed 's/#034;/\"/g'`
		GENRE=`echo $GENRE | sed 's/#039;/\"/g'` # '
		GENRE=`echo $GENRE | sed 's/&szlig;/Ãx/g'`
		GENRE=`echo $GENRE | sed 's/&ndash;/-/g'`
		GENRE=`echo $GENRE | sed 's/&Auml;/Ã/g'`
		GENRE=`echo $GENRE | sed 's/&Uuml;/ÃS/g'`
		GENRE=`echo $GENRE | sed 's/&Ouml;/Ã/g'`
		GENRE=`echo $GENRE | sed 's/&auml;/Ã¤/g'`
		GENRE=`echo $GENRE | sed 's/&uuml;/Ã¼/g'`
		GENRE=`echo $GENRE | sed 's/&ouml;/Ã¶/g'`
		GENRE=`echo $GENRE | sed 's/&eacute;/Ã©/g'`
		GENRE=`echo $GENRE | sed 's/&egrave;/Ã¨/g'`
		GENRE=`echo $GENRE | sed 's/%F6/Ã¶/g'`
		GENRE=`echo $GENRE | sed 's/%FC/Ã¼/g'`
		GENRE=`echo $GENRE | sed 's/%E4/Ã¤/g'`
		GENRE=`echo $GENRE | sed 's/%26/&/g'`
		GENRE=`echo $GENRE | sed 's/%C4/Ã/g'`
		GENRE=`echo $GENRE | sed 's/%D6/Ã/g'`
		GENRE=`echo $GENRE | sed 's/%DC/ÃS/g'`
		GENRE=`echo $GENRE | sed 's/|/ /g'`
		GENRE=`echo $GENRE | sed 's/(/ /g'`
		GENRE=`echo $GENRE | sed 's/)/ /g'`
		GENRE=`echo $GENRE | sed 's/+/ /g'`
		GENRE=`echo $GENRE | sed 's/\//-/g'`
		GENRE=`echo $GENRE | sed 's/,/ /g'`
		GENRE=`echo $GENRE | sed 's/;/ /g'`
		GENRE=`echo $GENRE | sed 's/:/ /g'`
		GENRE=`echo $GENRE | sed 's/\.\+/./g'`
	#	GENRE=`echo $(php -r "echo rawurldecode('$GENRE');")`
	
		TITLE=`echo $ROUND1 | sed 's/<server_name>/\ntitle=</g' | grep ^"title=" | tr '~' ' ' | cut -d '<' -f2`
		TITLE=`echo $TITLE | sed 's/&#038;/&/g'`
		TITLE=`echo $TITLE | sed 's/&amp;/und/g'`
		TITLE=`echo $TITLE | sed 's/&quot;/"/g'`
		TITLE=`echo $TITLE | sed 's/&lt;/\</g'`
		TITLE=`echo $TITLE | sed 's/&#034;/\"/g'`
		TITLE=`echo $TITLE | sed 's/&#039;/\"/g'` # '
		TITLE=`echo $TITLE | sed 's/#034;/\"/g'`
		TITLE=`echo $TITLE | sed 's/#039;/\"/g'` # '
		TITLE=`echo $TITLE | sed 's/&szlig;/Ãx/g'`
		TITLE=`echo $TITLE | sed 's/&ndash;/-/g'`
		TITLE=`echo $TITLE | sed 's/&Auml;/Ã/g'`
		TITLE=`echo $TITLE | sed 's/&Uuml;/ÃS/g'`
		TITLE=`echo $TITLE | sed 's/&Ouml;/Ã/g'`
		TITLE=`echo $TITLE | sed 's/&auml;/Ã¤/g'`
		TITLE=`echo $TITLE | sed 's/&uuml;/Ã¼/g'`
		TITLE=`echo $TITLE | sed 's/&ouml;/Ã¶/g'`
		TITLE=`echo $TITLE | sed 's/&eacute;/Ã©/g'`
		TITLE=`echo $TITLE | sed 's/&egrave;/Ã¨/g'`
		TITLE=`echo $TITLE | sed 's/%F6/Ã¶/g'`
		TITLE=`echo $TITLE | sed 's/%FC/Ã¼/g'`
		TITLE=`echo $TITLE | sed 's/%E4/Ã¤/g'`
		TITLE=`echo $TITLE | sed 's/%26/&/g'`
		TITLE=`echo $TITLE | sed 's/%C4/Ã/g'`
		TITLE=`echo $TITLE | sed 's/%D6/Ã/g'`
		TITLE=`echo $TITLE | sed 's/%DC/ÃS/g'`
		TITLE=`echo $TITLE | sed 's/|/ /g'`
		TITLE=`echo $TITLE | sed 's/(/ /g'`
		TITLE=`echo $TITLE | sed 's/)/ /g'`
		TITLE=`echo $TITLE | sed 's/+/ /g'`
		TITLE=`echo $TITLE | sed 's/\//-/g'`
		TITLE=`echo $TITLE | sed 's/,/ /g'`
		TITLE=`echo $TITLE | sed 's/;/ /g'`
		TITLE=`echo $TITLE | sed 's/:/ /g'`
		TITLE=`echo $TITLE | sed 's/"/ /g'`
		TITLE=`echo $TITLE | sed 's/\.\+/./g'`
	#	TITLE=`echo $(php -r "echo rawurldecode('$TITLE');")`
				
		PIC=http://atemio.dyndns.tv/mediathek/menu/`echo $TITLE | tr 'A-Z' 'a-z' | tr ' ' '_'`.jpg
		
		if [ -z "$PIC" ]; then
			PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
		fi
	
		if [ -z "$GENRE" ]; then
			GENRE="unknown"
		fi
	
	
		filename=`echo $GENRE | tr 'A-Z' 'a-z'`
		for ROUND in 50 60 70 80 90; do
			filename=`echo $filename | sed s/"$ROUND"./"$ROUND"s./`
			filename=`echo $filename | sed s/"$ROUND"s./"$ROUND"s./`
			filename=`echo $filename | sed s/"$ROUND"er./"$ROUND"s./`
			filename=`echo $filename | sed s/19"$ROUND"./"$ROUND"s./`
			filename=`echo $filename | sed s/19"$ROUND"s./"$ROUND"s./`
			filename=`echo $filename | sed s/19"$ROUND"er./"$ROUND"s./`
		done
	
		for ROUND in 2000 2001 2002 2003 2004 2005 2006 2007 2008 2009 2010 2011 2012 2013 2014; do
			filename=`echo $filename | sed s/"$ROUND"./"$ROUND"s./`
			filename=`echo $filename | sed s/"$ROUND"s./"$ROUND"s./`
			filename=`echo $filename | sed s/"$ROUND"er./"$ROUND"s./`
		done
	
		filename=`echo $filename | sed 's/^00s./2000s./'`
		filename=`echo $filename | sed 's/^100./100%./'`
	
		filename=`echo $filename | sed 's/top 40/top40/'`
		filename=`echo $filename | sed 's/#/ /g'`
		filename=`echo $filename | tr -d '"' | tr '-' ' ' | tr '_' ' ' | tr '.' ' ' | sed 's/\ \+/ /g'`
		
		if [ ! -z "$TITLE" ] && [ ! -z "$URL" ] && [ -e cache.internetradio.titanlist ] && [ `cat cache.internetradio.titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
			LINE="$TITLE#$URL#$PIC#internetradio_$piccount.jpg#Internet Radio#2"
			echo $LINE >> cache.internetradio.titanlist
		elif [ ! -z "$TITLE" ] && [ ! -z "$URL" ]; then
			LINE="$TITLE#$URL#$PIC#internetradio_$piccount.jpg#Internet Radio#2"
			echo $LINE >> cache.internetradio.titanlist
		fi
	
		for ROUND2 in $filename; do
			if [ ! -z "$TITLE" ] && [ ! -z "$URL" ] && [ -e cache.internetradio.genre."$ROUND2".titanlist ] &&  [ `cat cache.internetradio.genre."$ROUND2".titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
				LINE="$TITLE#$URL#$PIC#internetradio_$piccount.jpg#Internet Radio#2"
				echo $LINE >> cache.internetradio.genre."$ROUND2".titanlist
	
				piccount=`expr $piccount + 1`
				URL="http://atemio.dyndns.tv/mediathek/internetradio/streams/internetradio.genre.$ROUND2.list"
				PIC="http://atemio.dyndns.tv/mediathek/menu/$ROUND2.jpg"
				LINE="$ROUND2#$URL#$PIC#internetradio_$piccount.jpg#Internet Radio#0"
				if [ ! -z "$URL" ] && [ `cat cache.internetradio.category.titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
					echo $LINE >> cache.internetradio.category.titanlist
				fi
				cat cache.internetradio.genre."$ROUND2".titanlist | sort -u > _full/internetradio/streams/internetradio.genre."$ROUND2".list
			elif [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
				LINE="$TITLE#$URL#$PIC#internetradio_$piccount.jpg#Internet Radio#2"
				echo $LINE >> cache.internetradio.genre."$ROUND2".titanlist
	
				piccount=`expr $piccount + 1`
				URL="http://atemio.dyndns.tv/mediathek/internetradio/streams/internetradio.genre.$ROUND2.list"
				PIC="http://atemio.dyndns.tv/mediathek/menu/$ROUND2.jpg"
				LINE="$ROUND2#$URL#$PIC#internetradio_$piccount.jpg#Internet Radio#0"
				if [ ! -z "$URL" ] && [ `cat cache.internetradio.category.titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
					echo $LINE >> cache.internetradio.category.titanlist
				fi
				cat cache.internetradio.genre."$ROUND2".titanlist | sort -u > _full/internetradio/streams/internetradio.genre."$ROUND2".list		
			fi
		done
				
	#		echo ROUND1 $ROUND1
	#		echo URL $URL
	#		echo PIC $PIC
	#		echo TITLE $TITLE
	#		echo GENRE $GENRE
	
	done
	
	if [ "$buildtype" = "full" ];then
		cat cache.internetradio.category.titanlist | sort -u > _full/internetradio/internetradio.category.list	
		cat cache.internetradio.titanlist | sort -u > _full/internetradio/streams/internetradio.all-sorted.list
		
		for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
			filename=`echo "$ROUND" | tr 'A-Z' 'a-z'`
			if [ `cat cache.internetradio.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
				cat cache.internetradio.titanlist | grep ^"$ROUND" >> cache.internetradio.titanlist."$ROUND"
				cat cache.internetradio.titanlist."$ROUND" | sort -um > _full/internetradio/streams/internetradio.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
				echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/internetradio/streams/internetradio."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#Internet Radio#3 >> _full/internetradio/internetradio.a-z.list
			elif [ `cat cache.internetradio.titanlist | grep ^"$filename" | wc -l` -gt 0 ];then
				cat cache.internetradio.titanlist | grep ^"$filename" >> cache.internetradio.titanlist."$ROUND"
				cat cache.internetradio.titanlist."$ROUND" | sort -um > _full/internetradio/streams/internetradio.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
				echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/internetradio/streams/internetradio."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#Internet Radio#3 >> _full/internetradio/internetradio.a-z.list
		
			fi
		done
	fi
fi

DONETIME=`date +%s`
TIME=`expr $DONETIME - $BEGINTIME`
echo "[internetradio.sh] build time: ($TIME s) done" >> _full/internetradio/build.log	
echo "[internetradio.sh] internetradio: "`ls -1 _full/internetradio` >> _full/internetradio/build.log
echo "[internetradio.sh] internetradio/streams: "`ls -1 _full/internetradio/streams` >> _full/internetradio/build.log

if [ "$buildtype" != "full" ];then
	cp -a _full/internetradio/* /var/www/atemio/web/mediathek/internetradio
fi

rm cache.*

exit
