#!/bin/bash
#

buildtype=$1
wgetbin="wget -T2 -t2 --waitretry=2"

rm cache.*
rm -rf _full/zdf
mkdir -p _full/zdf/streams

BEGINTIME=`date +%s`
DATENAME=`date +"%Y.%m.%d_%H.%m.%S"`
echo "[zdf.sh] START (buildtype: $buildtype): $DATENAME > _full/zdf/build.log

if [ "$buildtype" = "full" ];then
	SITEURL="http://www.zdf.de/ZDFmediathek/hauptnavigation/startseite?flash=off"
	
	$wgetbin --no-check-certificate $SITEURL -O cache.main.list
	
	SEARCHLIST=`cat cache.main.list | grep /ZDFmediathek/ | grep "</li><li><a href=" | sed 's!</ul>!!' | cut -d'"' -f2`
	echo SEARCHLIST1=$SEARCHLIST
	
	SEARCHLIST=`echo $SEARCHLIST | sed "s!/ZDFmediathek/hauptnavigation/sendung-a-bis-z?flash=off!!"`
	echo SEARCHLIST2=$SEARCHLIST
	
	SEARCHLIST="/ZDFmediathek/hauptnavigation/sendung-a-bis-z?flash=off $SEARCHLIST"
	echo SEARCHLIST3=$SEARCHLIST
	
	piccount=0
	count=0
	for ROUND1 in $SEARCHLIST; do
	
		TITLE=`cat cache.main.list | grep /ZDFmediathek/ | grep $ROUND1 | grep "</li><li><a href="  | sed 's!</ul>!!' | cut -d'>' -f4 | cut -d '<' -f1`
		filename=`cat cache.main.list | grep /ZDFmediathek/ | grep $ROUND1 | grep "</li><li><a href="  | sed 's!</ul>!!' | cut -d'"' -f2 | cut -d "?" -f1 | tr '/' '\n' | tail -n1`	
		piccount=`expr $piccount + 1`
		URL="http://atemio.dyndns.tv/mediathek/zdf/streams/zdf.$filename.list"
		PIC="http://atemio.dyndns.tv/mediathek/menu/$filename.jpg"
		LINE="$TITLE#$URL#$PIC#zdf_$piccount.jpg#ZDF#3"
		if [ ! -z "$TITLE" ] || [ ! -z "$URL" ] || [ ! -z "$PIC" ]; then
			echo $LINE >> cache.category.titanlist
		fi
	
		$wgetbin --no-check-certificate http://www.zdf.de/$ROUND1 -O cache.$filename.list
	
		SEARCHLIST2=`cat cache.$filename.list | grep "<img src" | grep title | cut -d '"' -f2 | grep -v flash=off`		
		SEARCHLIST3=`cat cache.$filename.list | grep "<img src" | grep title | cut -d '"' -f2 | grep -v contentblob`
		tcount=0

		if [ $filename == "sendung-verpasst" ] || [ $filename == "live" ];then
			for i in 1 2 3 4 5 6 7; do
				tcount=`expr $tcount + 1`
				piccount=`expr $piccount + 1`
				URL="http://atemio.dyndns.tv/mediathek/zdf/streams/zdf.$filename.day$i.list"
				TITLE=`cat cache.$filename.list | grep "</li><li><a href=\"/ZDFmediathek/hauptnavigation/$filename/day$i" | cut -d">" -f4 | cut -d"<" -f1 | sed 's/- //'`
				PIC="http://atemio.dyndns.tv/mediathek/menu/day$i.jpg"
				LINE="$TITLE#$URL#$PIC#zdf_$piccount.jpg#ZDF#3"
				if [ ! -z "$TITLE" ] || [ ! -z "$URL" ] || [ ! -z "$PIC" ]; then
					echo "$LINE" >> cache.$filename.titanlist
				fi
	
				$wgetbin --no-check-certificate "http://www.zdf.de/ZDFmediathek/hauptnavigation/$filename/day$i?flash=off" -O cache.$filename.day$i.list
	
				if [ $filename == "live" ];then
					SEARCHLIST7=`cat cache.$filename.day$i.list | grep "<img src" | grep title | cut -d '"' -f2 | grep flash=off`
				else
					SEARCHLIST7=`cat cache.$filename.day$i.list | grep "<img src" | grep title | cut -d '"' -f2 | grep -v flash=off`
				fi
				
				filename2=day$i

				for ROUND7 in $SEARCHLIST7; do
					piccount=`expr $piccount + 1`
					if [ $filename == "live" ];then
						PIC=`cat cache.$filename.day$i.list | grep "<img src" | grep title | grep $ROUND7 | cut -d '"' -f4 | tr '&#' '%'`
						TITLE=`cat cache.$filename.day$i.list | grep "<img src" | grep title | grep $ROUND7 | cut -d '"' -f6 | tr '&#' '%'`
						ID=`echo $PIC | cut -d '/' -f6`
					else
						PIC=http://www.zdf.de`cat cache.$filename.day$i.list | grep "<img src" | grep title | grep $ROUND7 | cut -d '"' -f2`
						TITLE=`cat cache.$filename.day$i.list | grep "<img src" | grep title | grep $ROUND7 | cut -d '"' -f4 | tr '&#' '%'`
						ID=`echo $PIC | cut -d '/' -f6`
					fi

					if [ `cat cache.search.titanlist | grep $ID | wc -l` -eq 0 ];then		 
						$wgetbin --no-check-certificate "http://www.zdf.de/ZDFmediathek/xmlservice/web/beitragsDetails?id=$ID&ak=web" -O cache.$ID.list
						if [ `cat cache.$ID.list | grep "<debuginfo>Kein Beitrag mit ID=7011824</debuginfo>" | wc -l` -eq 1 ]; then
							URL=http://www.zdf.de`cat cache.$filename.day$i.list | grep "<img src" | grep title | grep $ROUND7 | cut -d '"' -f2 | tr '&#' '%'`

							$wgetbin --no-check-certificate $URL -O cache.$ID.url.list
							URL2=`cat cache.$ID.url.list | grep Abspielen | grep play | grep asx | cut -d'"' -f1 | head -n1`

							$wgetbin --no-check-certificate $URL2 -O cache.$ID.url2.list
							URL3=`cat cache.$ID.url2.list | grep "Ref href" | cut -d'"' -f2 | head -n1`

							$wgetbin --no-check-certificate $URL3 -O cache.$ID.url3.list
							URL3=`cat cache.$ID.url3.list | grep "REF HREF" | cut -d'"' -f2 | head -n1`
							if [ ! -z $URL3 ];then
								URL=$URL3
							fi

							MODE=277
						else
							URL=`cat cache.$ID.list | grep mp4 | grep url | grep http | tail -n1 | cut -d'>' -f2 | cut -d'<' -f1`
							MODE=2
						fi
			
						if [ ! `echo $TITLE | grep "^/" | wc -l` -eq 1 ]; then					
							LINE="$TITLE#$URL#$PIC#zdf_$piccount.jpg#ZDF#2"
							if [ ! -z "$TITLE" ] || [ ! -z "$URL" ] || [ ! -z "$PIC" ]; then
								echo "$LINE" >> cache.$filename.$filename2.titanlist
								echo "$ID#$TITLE#$URL#$PIC#zdf_$piccount.jpg#ZDF#$MODE" >> cache.search.titanlist
								echo "$TITLE#$URL#$PIC#zdf_$piccount.jpg#ZDF#$MODE" >> cache.titanlist
							fi
						fi
					else
						OLDLINE=`cat cache.search.titanlist | grep $ID | sed "s/$ID#//"`
						echo OLDLINE $OLDLINE
						if [ ! -z "$OLDLINE" ]; then
							echo "$OLDLINE" >> cache.$filename.$filename2.titanlist
						fi
					fi
				done
				if [ `cat cache.$filename.$filename2.titanlist | wc -l` -gt 0 ];then
					cat cache.$filename.$filename2.titanlist | sort -um > _full/zdf/streams/zdf.$filename.$filename2.list
				fi
				filename2=""
			done
		fi
		tcount=0

		if [ $filename == "sendung-a-bis-z" ];then
			for i in 0 1 2 3 4 5 6 7 8; do				
				tcount=`expr $tcount + 1`
	
				piccount=`expr $piccount + 1`
				$wgetbin --no-check-certificate "http://www.zdf.de/ZDFmediathek/hauptnavigation/$filename/saz$i?flash=off" -O cache.$filename.saz$i.titanlist
	
				TITLE=`cat cache.$filename.saz$i.titanlist | grep "/ZDFmediathek/hauptnavigation/$filename/saz$i?flash=off" | head -n1 | cut -d">" -f2 | cut -d"<" -f1`
				filename2=`echo $TITLE`
				URL="http://atemio.dyndns.tv/mediathek/zdf/streams/zdf.$filename.$filename2.list"
				PIC="http://atemio.dyndns.tv/mediathek/menu/$filename2.jpg"
	
				LINE="$TITLE#$URL#$PIC#zdf_$piccount.jpg#ZDF#3"
				if [ ! -z "$TITLE" ] || [ ! -z "$URL" ] || [ ! -z "$PIC" ]; then
					echo "$LINE" >> cache.$filename.titanlist
				fi
	
				SEARCHLIST5=`cat cache.$filename.saz$i.titanlist | grep "<img src" | grep title | cut -d '"' -f2 | grep -v contentblob`
	
				for ROUND5 in $SEARCHLIST5; do
					piccount=`expr $piccount + 1`
			
					PIC=http://www.zdf.de`cat cache.$filename.saz$i.titanlist | grep "<img src" | grep title | grep $ROUND5 | cut -d '"' -f4 | tr '&#' '%'`
					TITLE=`cat cache.$filename.saz$i.titanlist | grep "<img src" | grep title | grep $ROUND5 | cut -d '"' -f6 | tr '&#' '%'`
					ID=`echo $PIC | cut -d '/' -f6`
					filename3=`echo $TITLE | tr ' ' '.' | tr '-' '.'`
					URL="http://atemio.dyndns.tv/mediathek/zdf/streams/zdf.$filename.$filename2.$filename3.list"
			
					if [ ! `echo $TITLE | grep "^/" | wc -l` -eq 1 ]; then
						LINE="$TITLE#$URL#$PIC#zdf_$piccount.jpg#ZDF#3"
						if [ ! -z "$TITLE" ] || [ ! -z "$URL" ] || [ ! -z "$PIC" ]; then
							echo "$LINE" >> cache.$filename.$filename2.titanlist
						fi
					fi
					
					$wgetbin --no-check-certificate "http://www.zdf.de$ROUND5" -O cache.$filename.$filename2.$filename3.list
					SEARCHLIST6=`cat cache.$filename.$filename2.$filename3.list | grep "<img src" | grep title | cut -d '"' -f2 | grep -v flash=off`
					
					for ROUND6 in $SEARCHLIST6; do
						echo ROUND6=$ROUND6
						piccount=`expr $piccount + 1`
						PIC=http://www.zdf.de`cat cache.$filename.$filename2.$filename3.list | grep "<img src" | grep title | grep $ROUND6 | cut -d '"' -f2`
						TITLE=`cat cache.$filename.$filename2.$filename3.list | grep "<img src" | grep title | grep $ROUND6 | cut -d '"' -f4 | tr '&#' '%'`
						ID=`echo $PIC | cut -d '/' -f6`
				 
						$wgetbin --no-check-certificate "http://www.zdf.de/ZDFmediathek/xmlservice/web/beitragsDetails?id=$ID&ak=web" -O cache.$ID.list
						URL=`cat cache.$ID.list | grep mp4 | grep url | grep http | tail -n1 | cut -d'>' -f2 | cut -d'<' -f1`
				
						if [ ! `echo $TITLE | grep "^/" | wc -l` -eq 1 ]; then
							LINE="$TITLE#$URL#$PIC#zdf_$piccount.jpg#ZDF#2"
							if [ ! -z "$TITLE" ] || [ ! -z "$URL" ] || [ ! -z "$PIC" ]; then
								echo "$LINE" >> cache.$filename.$filename2.$filename3.titanlist
								echo "$ID#$TITLE#$URL#$PIC#zdf_$piccount.jpg#ZDF#2" >> cache.search.titanlist
								echo "$TITLE#$URL#$PIC#zdf_$piccount.jpg#ZDF#2" >> cache.titanlist
							fi
						fi
				
					done
					if [ `cat cache.$filename.$filename2.$filename3.titanlist | wc -l` -gt 0 ];then
						cat cache.$filename.$filename2.$filename3.titanlist | sort -um > _full/zdf/streams/zdf.$filename.$filename2.$filename3.list
					fi
					filename3=""
				done
		
				if [ `cat cache.$filename.$filename2.titanlist | wc -l` -gt 0 ];then
					cat cache.$filename.$filename2.titanlist | sort -um > _full/zdf/streams/zdf.$filename.$filename2.list
				fi
				filename2=""
			done
		fi
	
		incount=0
		for ROUND3 in $SEARCHLIST3; do
			piccount=`expr $piccount + 1`
			
			PIC=http://www.zdf.de`cat cache.$filename.list | grep "<img src" | grep title | grep $ROUND3 | cut -d '"' -f4 | tr '&#' '%'`
			TITLE=`cat cache.$filename.list | grep "<img src" | grep title | grep $ROUND3 | cut -d '"' -f6 | tr '&#' '%'`
			ID=`echo $PIC | cut -d '/' -f6`
			filename2=`echo $TITLE | tr ' ' '.' | tr '-' '.'`
	
			URL="http://atemio.dyndns.tv/mediathek/zdf/streams/zdf.$filename.$filename2.list"
			
			if [ ! `echo $TITLE | grep "^/" | wc -l` -eq 1 ]; then
				if [ $filename == rubriken ];then
					LINE="$TITLE#$URL#$PIC#zdf_$piccount.png#ZDF#3"
				elif [ $filename == themen ];then
					LINE="$TITLE#$URL#$PIC#zdf_$piccount.png#ZDF#3"
				else
					LINE="$TITLE#$URL#$PIC#zdf_$piccount.jpg#ZDF#3"
				fi
				if [ ! -z "$TITLE" ] || [ ! -z "$URL" ] || [ ! -z "$PIC" ]; then			
					echo "$LINE" >> cache.$filename.titanlist
				fi
			fi
			count=`expr $count + 1`
			NEWURL=http://www.zdf.de`cat cache.$filename.list | grep "<img src" | grep title | grep $ROUND3 | cut -d '"' -f2`
			$wgetbin --no-check-certificate $NEWURL -O cache.$filename.submenu.$count.list
	#######
			SEARCHLIST4=`cat cache.$filename.submenu.$count.list | grep "<img src" | grep title | cut -d '"' -f2 | grep -v contentblob`
		
			for ROUND4 in $SEARCHLIST4; do
				piccount=`expr $piccount + 1`
		
				PIC=http://www.zdf.de`cat cache.$filename.submenu.$count.list | grep "<img src" | grep title | grep $ROUND4 | cut -d '"' -f4 | tr '&#' '%'`
				TITLE=`cat cache.$filename.submenu.$count.list | grep "<img src" | grep title | grep $ROUND4 | cut -d '"' -f6 | tr '&#' '%'`
				ID=`echo $PIC | cut -d '/' -f6`
				filename3=`echo $TITLE | tr ' ' '.' | tr '-' '.'`
				URL="http://atemio.dyndns.tv/mediathek/zdf/streams/zdf.$filename.$filename2.$filename3.list"
							
				if [ ! `echo $TITLE | grep "^/" | wc -l` -eq 1 ]; then	
					LINE="$TITLE#$URL#$PIC#zdf_$piccount.jpg#ZDF#3"
					if [ ! -z "$TITLE" ] || [ ! -z "$URL" ] || [ ! -z "$PIC" ]; then
						echo "$LINE" >> cache.$filename.$filename2.titanlist
					fi
				fi
			done
	
			if [ `cat cache.$filename.$filename2.titanlist | wc -l` -gt 0 ];then
				cat cache.$filename.$filename2.titanlist | sort -um > _full/zdf/streams/zdf.$filename.$filename2.list
			fi
			filename2=""
	########
			
		done
	
		cat cache.$filename.titanlist | sort -um > _full/zdf/streams/zdf.$filename.list	
	done
	
	cat cache.titanlist | sort -um > _full/zdf/streams/zdf.all-sorted.list
	cat cache.category.titanlist | sort -um > _full/zdf/zdf.category.list
fi

DONETIME=`date +%s`
TIME=`expr $DONETIME - $BEGINTIME`
echo "[zdf.sh] build time: ($TIME s) done" >> _full/zdf/build.log	

if [ "$buildtype" != "full" ];then
	cp -a _full/zdf/* /var/www/atemio/web/mediathek/zdf
fi

rm cache.*

exit