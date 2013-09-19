#! /bin/sh

buildtype=$1
wgetbin="wget -T2 -t2 --waitretry=2"

skipall=1
count=0
count0=0
count1=0
count2=0
piccount=0

rm cache.beeg.*
rm Beeg.*.m3u
rm -rf _single
rm -rf _full/beeg
mkdir -p _full/beeg/streams
mkdir _single

BEGINTIME=`date +%s`
DATENAME=`date +"%Y.%m.%d_%H.%M.%S"`
echo "[beeg.sh] START (buildtype: $buildtype): $DATENAME" > _full/beeg/build.log

if [ "$buildtype" = "full" ];then
	$wgetbin http://beeg.com -O cache.beeg.main.html
	
	sectionstags=`cat cache.beeg.main.html | grep 'href="/tag/' | cut -d '"' -f4`
	count=`cat cache.beeg.main.html | grep ^'<a href="/section/home/' | grep 'target="_self">' | cut -d ">" -f2 | cut -d"<" -f1 | tail -n1`
	echo "[beeg.sh] count: $count"
	i=1
	until [ "$i" -gt "$count" ]
	do
	#echo $i
	sections="$sections http://beeg.com/section/home/$i/"
	i=$[$i+1]
	done
	echo sections $sections
	
	for ROUND0 in $sections; do
		count0=`expr $count0 + 1`
		echo "sections($count0)=$ROUND0"
	
	#	echo $wgetbin $ROUND0
		$wgetbin $ROUND0 -O cache.beeg.section."$count0".html
	
		tumbid=`cat cache.beeg.section."$count0".html | grep "var tumbid" | cut -d"[" -f2 | cut -d"]" -f1 | tr ',' ' '`
		tumbalt=`cat cache.beeg.section."$count0".html | grep "var tumbalt" | cut -d"[" -f2 | cut -d"]" -f1 | sed "s/','\+/|/g" | tr ' ' '_' | tr '|' ' '`
		tumburl=`cat cache.beeg.section."$count0".html | grep "var IMGthumb" | cut -d"'" -f2`
	
	#	echo tumbid: $tumbid
	#	echo tumbalt: $tumbalt
	#	echo tumburl: $tumburl
	#sleep 2
		count1=0
		for ROUND1 in $tumbid; do
	#if [ $count1 = 3 ];then
	#	break
	#fi
			count1=`expr $count1 + 1`
			
			count=`expr $count + 1`
			echo "section($count0/$i) add($count) id($ROUND1)"
	
			STREAMURL=http://beeg.com/$ROUND1
			piccount=`expr $piccount + 1`
			PIC=`echo $tumburl`$ROUND1.jpg
		
			count2=0
			for ROUND2 in $tumbalt; do
				count2=`expr $count2 + 1`
				if [ $count1 = $count2 ];then
					TITLE=`echo $ROUND2 | tr "'" ' ' | tr '_' ' '`
					TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' # ' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'` 
					break
				fi
			done
			LINE="$TITLE#$STREAMURL#$PIC#beeg_$piccount.jpg#Beeg#50"
			echo $LINE >> cache.beeg.titanlist
		done
	done
	#fi
	
	for ROUND3 in $sectionstags; do
		count3=`expr $count3 + 1`
		$wgetbin http://beeg.com$ROUND3 -O cache.beeg.tags.$count3.html
		tagcount=`cat cache.beeg.tags.$count3.html | grep ^'<a href="/tag/' | sort -u | cut -d"/" -f4 | tail -n1`
	
	#if [ $count3 = 3 ];then
	#	break
	#fi
	
		if [ ! -z $tagcount ];then
			tags=""
			i=1
	#		echo tagcount = $tagcount
			until [ "$i" -gt "$tagcount" ]
			do
	#		echo $i
			tags="$tags http://beeg.com$ROUND3/$i/"
			i=$[$i+1]
			done
	#		echo tags $tags
			
			count4=0
			for ROUND4 in $tags; do
				count4=`expr $count4 + 1`
				echo "tags($count4)=$ROUND4"
				
				piccount=`expr $piccount + 1`
				tagname=`echo $ROUND4 | cut -d"/" -f5`
				STREAMURL="http://atemio.dyndns.tv/mediathek/beeg/streams/beeg.$tagname.list"
				PIC="http://atemio.dyndns.tv/mediathek/menu/$tagname.jpg"
				LINE="$tagname#$STREAMURL#$PIC#beeg_$piccount.jpg#Beeg#3"
				
				if [ `cat cache.beeg.category.titanlist | grep "$tagname#$STREAMURL#$PIC#beeg_" | wc -l` -eq 0 ];then
					echo $LINE >> cache.beeg.category.titanlist
					cat cache.beeg.category.titanlist | sort -u > _full/beeg/beeg.category.list
				fi
							
	#			echo $wgetbin $ROUND4
				$wgetbin $ROUND4 -O cache.beeg.tags."$count4".html
			
				tumbid=`cat cache.beeg.tags."$count4".html | grep "var tumbid" | cut -d"[" -f2 | cut -d"]" -f1 | tr ',' ' '`
			
	#			echo tumbid: $tumbid
		#	sleep 2
			
				count5=0
				for ROUND5 in $tumbid; do
					LINE=""
					LINE=`cat cache.beeg.titanlist | grep $ROUND5`
		
					if [ ! -z "$LINE" ] && [ `cat cache.beeg.$tagname.titanlist | grep "$LINE" | wc -l` -eq 0 ];then
						count=`expr $count + 1`
						echo "tags($tagname $count4/$i) add($count) id($ROUND5)"
						echo "$LINE" >> cache.beeg.$tagname.titanlist
						cat cache.beeg.$tagname.titanlist | sort -u > _full/beeg/streams/beeg.$tagname.list
	echo "$LINE"
					fi		
				done
			done
		fi
	done
	
	cp cache.beeg.titanlist _full/beeg/streams/beeg.all-newfirst.list
	cat cache.beeg.titanlist | sort -u > _full/beeg/streams/beeg.all-sorted.list
	
	for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
		echo ROUND $ROUND
		filename=`echo "$ROUND" | tr 'A-Z' 'a-z'`
		echo filename $filename	
		if [ `cat cache.beeg.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
			cat cache.beeg.titanlist | grep ^"$ROUND" > cache.beeg.titanlist."$ROUND"
			cat cache.beeg.titanlist."$ROUND" | sort -u > _full/beeg/streams/beeg.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
			echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/beeg/streams/beeg."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#Beeg#3 >> _full/beeg/beeg.a-z.list
		elif [ `cat cache.beeg.titanlist | grep ^"$filename" | wc -l` -gt 0 ];then
			cat cache.beeg.titanlist | grep ^"$filename" > cache.beeg.titanlist."$ROUND"
			cat cache.beeg.titanlist."$ROUND" | sort -u > _full/beeg/streams/beeg.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
			echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/beeg/streams/beeg."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#Beeg#3 >> _full/beeg/beeg.a-z.list
		fi
	done
	
	
	echo beeg closed
fi

DONETIME=`date +%s`
TIME=`expr $DONETIME - $BEGINTIME`
echo "[beeg.sh] build time: ($TIME s) done" >> _full/beeg/build.log	
echo "[beeg.sh] beeg: "`ls -1 _full/beeg` >> _full/beeg/build.log
echo "[beeg.sh] beeg/streams: "`ls -1 _full/beeg/streams` >> _full/beeg/build.log

if [ "$buildtype" != "full" ];then
	cp -a _full/beeg/* /var/www/atemio/web/mediathek/beeg
fi

rm cache.*

exit
