#!/bin/bash
#

buildtype=$1
wgetbin="wget -T2 -t2 --waitretry=2"

rm cache.*
rm -rf _full/zdf
mkdir -p _full/zdf/streams

BEGINTIME=`date +%s`
DATENAME=`date +"%Y.%m.%d_%H.%M.%S"`
echo "[zdf.sh] START (buildtype: $buildtype): $DATENAME" > _full/zdf/build.log

if [ "$buildtype" = "full" ];then
	SITEURL="http://www.zdf.de/ZDFmediathek/hauptnavigation/startseite?flash=off"
	
	$wgetbin --no-check-certificate $SITEURL -O cache.zdf.main.list
	
	SEARCHLIST=`cat cache.main.list | grep /ZDFmediathek/ | grep "</li><li><a href=" | sed 's!</ul>!!' | cut -d'"' -f2`
	echo SEARCHLIST1=$SEARCHLIST
	
	SEARCHLIST=`echo $SEARCHLIST | sed "s!/ZDFmediathek/hauptnavigation/sendung-a-bis-z?flash=off!!"`
	echo SEARCHLIST2=$SEARCHLIST
	
	SEARCHLIST="/ZDFmediathek/hauptnavigation/sendung-a-bis-z?flash=off $SEARCHLIST"
	echo SEARCHLIST3=$SEARCHLIST

###################
	SEARCHLIST="/ZDFmediathek/hauptnavigation/sendung-a-bis-z?flash=off"
###################
	piccount=0
	count=0
	for ROUND1 in $SEARCHLIST; do
	
		TITLE=`cat cache.zdf.main.list | grep /ZDFmediathek/ | grep $ROUND1 | grep "</li><li><a href="  | sed 's!</ul>!!' | cut -d'>' -f4 | cut -d '<' -f1`
		filepath=`cat cache.zdf.main.list | grep /ZDFmediathek/ | grep $ROUND1 | grep "</li><li><a href="  | sed 's!</ul>!!' | cut -d'"' -f2 | cut -d "?" -f1 | tr '/' '\n' | tail -n1`	
		TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/&#043;/+/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/#043;/+/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/+/ /g' -e 's/,/ /g' -e 's/;/ /g' -e 's/\.\+/./g' -e 's/+/ /g' -e 's/#/ /g' -e 's/\.\+/ /g' -e 's/\ \+/ /g' -e 's/^ //'`
		TITLE=`echo $TITLE | sed 's/\(.*\)\./\1\t/g' | sed 's/^\.//g'`
		TITLE=`echo $(php -r "echo rawurldecode('$TITLE');")`

		filename=`echo $TITLE | tr 'A-Z' 'a-z' | tr '(' '.' | tr ')' '.' | tr '-' '.' | tr '_' '.' | tr '~' '.' | tr '|' '.' | tr ',' '.' | tr '"' '.' | tr ':' '.' | tr ' ' '.' | tr '+' '.' | tr '!' '.' | sed 's!%2!!g' | sed 's!&!.!g' | sed 's/\.\+/./g'`
		piccount=`expr $piccount + 1`
		URL="http://atemio.dyndns.tv/mediathek/zdf/zdf.$filename.list"
		PIC="http://atemio.dyndns.tv/mediathek/menu/$filename.jpg"
		LINE="$TITLE#$URL#$PIC#zdf_$piccount.jpg#ZDF#3"
		if [ ! -z "$TITLE" ] || [ ! -z "$URL" ] || [ ! -z "$PIC" ]; then
			echo $LINE >> cache.zdf.category.titanlist
		fi
	
		$wgetbin --no-check-certificate http://www.zdf.de/$ROUND1 -O cache.zdf.$filename.list
	
		SEARCHLIST2=`cat cache.zdf.$filename.list | grep "<img src" | grep title | cut -d '"' -f2 | grep -v flash=off`		
		SEARCHLIST3=`cat cache.zdf.$filename.list | grep "<img src" | grep title | cut -d '"' -f2 | grep -v contentblob`

		tcount=0

		if [ "$filename" == "sendungen.a.z" ];then
			for i in 0 1 2 3 4 5 6 7 8; do				
				tcount=`expr $tcount + 1`
	
				piccount=`expr $piccount + 1`
				$wgetbin --no-check-certificate "http://www.zdf.de/ZDFmediathek/hauptnavigation/$filepath/saz$i?flash=off" -O cache.zdf.$filename.saz$i.titanlist
				TITLE=`cat cache.zdf.$filename.saz$i.titanlist | grep "/ZDFmediathek/hauptnavigation/$filepath/saz$i?flash=off" | head -n1 | cut -d">" -f2 | cut -d"<" -f1`
				TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/&#043;/+/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/#043;/+/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/+/ /g' -e 's/,/ /g' -e 's/;/ /g' -e 's/\.\+/./g' -e 's/+/ /g' -e 's/#/ /g' -e 's/\.\+/ /g' -e 's/\ \+/ /g' -e 's/^ //'`
				TITLE=`echo $TITLE | sed 's/\(.*\)\./\1\t/g' | sed 's/^\.//g'`
				TITLE=`echo $(php -r "echo rawurldecode('$TITLE');")`

				filename2=`echo $TITLE | tr 'A-Z' 'a-z' | tr '(' '.' | tr ')' '.' | tr '-' '.' | tr '_' '.' | tr '~' '.' | tr '|' '.' | tr ',' '.' | tr '"' '.' | tr ':' '.' | tr ' ' '.' | tr '+' '.' | tr '!' '.' | sed 's!%2!!g' | sed 's!&!.!g' | sed 's/\.\+/./g'`

				URL="http://atemio.dyndns.tv/mediathek/zdf/zdf.$filename.$filename2.list"
				PIC="http://atemio.dyndns.tv/mediathek/menu/$filename2.jpg"

				LINE="$TITLE#$URL#$PIC#zdf_$piccount.jpg#ZDF#3"
				if [ ! -z "$TITLE" ] || [ ! -z "$URL" ] || [ ! -z "$PIC" ]; then
					echo "$LINE" >> cache.zdf.$filename.titanlist
				fi
	
				SEARCHLIST5=`cat cache.zdf.$filename.saz$i.titanlist | tr '\r' '\n' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | tr ' ' '~' | sed 's!<div~class="image">!\r\n<div~class="image">!g' | grep ^'<div~class="image">' | sed 's/<!--/\e\n<!--/g' | grep -v "<!--"`
	
				for ROUND5 in $SEARCHLIST5; do
					piccount=`expr $piccount + 1`
					TITLE=`echo $ROUND5 | cut -d ">" -f14 | cut -d "<" -f1 | sed 's/^ //g' | tr '~' ' '` 
					TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/&#043;/+/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/#043;/+/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/+/ /g' -e 's/,/ /g' -e 's/;/ /g' -e 's/\.\+/./g' -e 's/+/ /g' -e 's/#/ /g' -e 's/\.\+/ /g' -e 's/\ \+/ /g' -e 's/^ //'`
					TITLE=`echo $TITLE | sed 's/\(.*\)\./\1\t/g' | sed 's/^\.//g'`
					TITLE=`echo $(php -r "echo rawurldecode('$TITLE');")`
					filename3=`echo $TITLE | tr 'A-Z' 'a-z' | tr '(' '.' | tr ')' '.' | tr '-' '.' | tr '_' '.' | tr '~' '.' | tr '|' '.' | tr ',' '.' | tr '"' '.' | tr ':' '.' | tr ' ' '.' | tr '+' '.' | tr '!' '.' | sed 's!%2!!g' | sed 's!&!.!g' | sed 's/\.\+/./g'`
					PIC=http://www.zdf.de`echo $ROUND5 | sed 's!<img~src=!\n<img~src=!' | grep ^"<img~src=" | cut -d '"' -f2 | tr '~' ' '`
					URL="http://atemio.dyndns.tv/mediathek/zdf/streams/zdf.$filename.$filename2.$filename3.list"
					GETURL=http://www.zdf.de`echo $ROUND5 | sed 's!<a~href=!\n<a~href=!' | grep ^"<a~href=" | head -n1 | cut -d '"' -f2 | tr '~' ' '`

					if [ ! -z "$TITLE" ] && [ ! -z "$URL" ] && [ `cat cache.zdf.$filename.$filename2.titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
						LINE="$TITLE#$URL#$PIC#zdf_$piccount.jpg#ZDF#3"
						echo "$LINE" >> cache.zdf.$filename.$filename2.titanlist
					fi
					
					$wgetbin --no-check-certificate "$GETURL" -O cache.zdf.$filename.$filename2.$filename3.list
					SEARCHLIST6=`cat cache.zdf.$filename.$filename2.$filename3.list | grep "<img src" | grep title | cut -d '"' -f2 | grep -v flash=off`	
				
					for ROUND6 in $SEARCHLIST6; do
						piccount=`expr $piccount + 1`
						PIC=http://www.zdf.de$ROUND6
						ID=`echo $PIC | cut -d '/' -f6`

						TITLE=`cat cache.zdf.$filename.$filename2.$filename3.list | tr '\r' '\n' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!<div class="image">!\n\n<div class="image">!g' | grep ^'<div class="image">' | grep "<img src" | grep $ID | cut -d ">" -f14 | cut -d "<" -f1 | sed 's/^ //g'`
						TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/&#043;/+/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/#043;/+/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/+/ /g' -e 's/,/ /g' -e 's/;/ /g' -e 's/\.\+/./g' -e 's/+/ /g' -e 's/#/ /g' -e 's/\.\+/ /g' -e 's/\ \+/ /g' -e 's/^ //'`
						TITLE=`echo $TITLE | sed 's/\(.*\)\./\1\t/g' | sed 's/^\.//g'`
						TITLE=`echo $TITLE | sed 's/\(.*\)"/\1\t/g' | sed 's/^"//g'`
						if [ `echo $TITLE | grep '"' | wc -l` -eq 0 ] && [ `echo $TITLE | grep "'" | wc -l` -eq 0 ];then
							TITLE=`echo $(php -r "echo rawurldecode('$TITLE');")`
						fi
					
						URL="http://www.zdf.de/ZDFmediathek/xmlservice/web/beitragsDetails?id=$ID&ak=web"
						AIRTIME=`cat cache.zdf.$filename.$filename2.$filename3.list | tr '\r' '\n' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!<div class="image">!\n\n<div class="image">!g' | grep ^'<div class="image">' | grep "<img src" | grep $ID | cut -d"," -f2 | cut -d "<" -f1 | sed 's/^ //g'`
						TITLE="`echo $TITLE` (`echo $AIRTIME`)"
						
						LINE="$TITLE#$URL#$PIC#zdf_$piccount.jpg#ZDF#46"
						if [ ! -z "$TITLE" ] && [ ! -z "$URL" ] && [ -e cache.zdf.$filename.$filename2.$filename3.titanlist ] && [ `cat cache.zdf.$filename.$filename2.$filename3.titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
							echo $LINE >> cache.zdf.$filename.$filename2.$filename3.titanlist
						elif [ ! -e cache.zdf.$filename.$filename2.$filename3.titanlist ];then
							echo $LINE >> cache.zdf.$filename.$filename2.$filename3.titanlist
						fi
				
						if [ ! -z "$TITLE" ] && [ ! -z "$URL" ] && [ `cat cache.zdf.titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
							echo $LINE >> cache.zdf.titanlist
						elif [ ! -e cache.zdf.titanlist ];then
							echo $LINE >> cache.zdf.titanlist
						fi
				
					done

					if [ -e cache.zdf.$filename.$filename2.$filename3.titanlist ];then
						cat cache.zdf.$filename.$filename2.$filename3.titanlist | sort -um > _full/zdf/streams/zdf.$filename.$filename2.$filename3.list
					fi

					filename3=""
				done

				if [ -e cache.zdf.$filename.$filename2.titanlist ];then
					cat cache.zdf.$filename.$filename2.titanlist | sort -u > _full/zdf/zdf.$filename.$filename2.list
				fi
				filename2=""
			done
		fi
		
		if [ -e cache.zdf.$filename.titanlist ];then
			cat cache.zdf.$filename.titanlist | sort -um > _full/zdf/zdf.$filename.list	
		fi
	done
fi

if [ "$buildtype" = "full" ];then
	cat cache.zdf.titanlist | sort -u > _full/zdf/streams/zdf.all-sorted.list
	cat cache.zdf.category.titanlist > _full/zdf/zdf.category.list
	
	for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
		filename=`echo "$ROUND" | tr 'A-Z' 'a-z'`
		if [ `cat cache.zdf.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
			cat cache.zdf.titanlist | grep ^"$ROUND" > cache.zdf.titanlist."$ROUND"
			cat cache.zdf.titanlist."$ROUND" | sort -u > _full/zdf/streams/zdf.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
			echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/zdf/streams/zdf."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#ZDF#3 >> _full/zdf/zdf.a-z.list
		elif [ `cat cache.zdf.titanlist | grep ^"$filename" | wc -l` -gt 0 ];then
			cat cache.zdf.titanlist | grep ^"$filename" > cache.zdf.titanlist."$ROUND"
			cat cache.zdf.titanlist."$ROUND" | sort -u > _full/zdf/streams/zdf.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
			echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/zdf/streams/zdf."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#ZDF#3 >> _full/zdf/zdf.a-z.list
		fi
	done
fi

DONETIME=`date +%s`
TIME=`expr $DONETIME - $BEGINTIME`
echo "[zdf.sh] build time: ($TIME s) done" >> _full/zdf/build.log	
echo "[zdf.sh] zdf: "`ls -1 _full/zdf` >> _full/zdf/build.log
echo "[zdf.sh] zdf/streams: "`ls -1 _full/zdf/streams` >> _full/zdf/build.log

if [ "$buildtype" != "full" ];then
	cp -a _full/zdf/* /var/www/atemio/web/mediathek/zdf
fi

rm cache.*

exit