#!/bin/bash
#

buildtype=$1
wgetbin="wget -T2 -t2 --waitretry=2"

rm cache.*
rm _liste
rm -rf _full/xvideos
mkdir -p _full/xvideos/streams
rm cache.*
touch cache.xvideos.titanlist

BEGINTIME=`date +%s`
DATENAME=`date +"%Y.%m.%d_%H.%M.%S"`
echo "[xvideos.sh] START (buildtype: $buildtype): $DATENAME" > _full/xvideos/build.log
	
testcount=0
piccount=0
count=0
	
if [ "$buildtype" = "full" ];then	

	
	$wgetbin http://www.xvideos.com/tags/ -O cache.main.tags
	TAGLIST=`cat cache.main.tags  | grep /c/ | sed 's!">!"<!g' | tr ' ' '~'`
	
	echo TAGLIST "$TAGLIST"
	for ROUND3 in $TAGLIST; do
		piccount=`expr $piccount + 1`
		count=`expr $count + 1`	
		tagname=`echo $ROUND3 | tr '~' ' ' | cut -d'<' -f4`
	#	echo tagname $tagname
		filename3=`echo $tagname | tr ' ' '.' | tr [A-Z] [a-z]`
	#	echo filename3 $filename3
		tagurl=`echo $ROUND3 | cut -d'"' -f2 | sed 's!/c/!!'`
	#	echo tagurl $tagurl
	
		tags="http://www.xvideos.com/c/$tagurl"
		tagcount=100
		i=1
		until [ "$i" -gt "$tagcount" ]
		do
	#	echo $i
		tags="$tags http://www.xvideos.com/c/$i/$tagurl"
		i=$[$i+1]
		
		done
	
		for ROUND4 in $tags; do
			piccount=`expr $piccount + 1`
			count=`expr $count + 1`	
			filename4=tags-$filename3
			$wgetbin "$ROUND4" -O cache."$filename4"."$count"
			if [ `cat cache."$filename4"."$count" | grep "<h1>Sorry but the page you requested was not found.</h1>" | wc -l` -eq 1 ];then
				break
			fi
	
			foundlist=`cat cache."$filename4"."$count" | grep '<a href="/video' | grep '><img src=' | tr ' ' '~'`
			for ROUND5 in $foundlist; do
	###########
				piccount=`expr $piccount + 1`
				URL=`echo $ROUND5 | sed 's!<a~href=!\nlink=!g' | grep link= | cut -d'"' -f2 | head -n1`
				PIC=`echo $ROUND5 | sed 's!<img~src=!\nimgpic=!g' | grep imgpic= | cut -d'"' -f2 | head -n1`
				TITLE=`cat cache."$filename4"."$count" | grep "$URL" | sed 's!<a href=!\npic=!g' | grep ^pic= | tail -n1 | cut -d">" -f2 | cut -d"<" -f1`
				if [ -z "$TITLE" ];then
					TITLE=`echo $URL | tr '/' '\n' | tail -n1 | tr '_' ' ' | tr '-' ' ' | sed 's/^ //'`
				fi
	
				TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' # ' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
			
				if [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
					LINE="$TITLE#http://www.xvideos.com$URL#$PIC#xvideos_$piccount.jpg#XVIDEOS#42"			
	
					if [ ! -e cache.xvideos."$filename4".titanlist ];then
						echo $LINE >> cache.xvideos."$filename4".titanlist
					elif [ `cat cache.xvideos."$filename4".titanlist | grep "#$URL#" | tail -n1 | wc -l ` -eq 0 ];then
						echo $LINE >> cache.xvideos."$filename4".titanlist					
					fi
	
					if [ `cat cache.xvideos.titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
						echo $LINE >> cache.xvideos.titanlist
					fi
				fi
	###########
	
			done	
		done
		if [ -e cache.xvideos."$filename4".titanlist ];then
			piccount=`expr $piccount + 1`
			URL="http://atemio.dyndns.tv/mediathek/xvideos/streams/xvideos."$filename4".list"
			PIC="http://atemio.dyndns.tv/mediathek/menu/"$filename3".jpg"
			LINE="$tagname#$URL#$PIC#xvideos_$piccount.jpg#XVIDEOS#3"
			echo $LINE >> cache.xvideos.category.titanlist
			cat cache.xvideos."$filename4".titanlist >> _full/xvideos/streams/xvideos."$filename4".list
		fi
	done
	
	cat cache.xvideos.titanlist | sort -u > _full/xvideos/streams/xvideos.all-sorted.list
	cat cache.xvideos.category.titanlist | sort -u > _full/xvideos/xvideos.category.list
	
	for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
		filename=`echo "$ROUND" | tr 'A-Z' 'a-z'`
		if [ `cat cache.xvideos.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
			cat cache.xvideos.titanlist | grep ^"$ROUND" > cache.xvideos.titanlist."$ROUND"
			cat cache.xvideos.titanlist."$ROUND" | sort -u > _full/xvideos/streams/xvideos.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
			echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/xvideos/streams/xvideos."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#XVIDEOS#3 >> _full/xvideos/xvideos.a-z.list
		elif [ `cat cache.xvideos.titanlist | grep ^"$filename" | wc -l` -gt 0 ];then
			cat cache.xvideos.titanlist | grep ^"$filename" > cache.xvideos.titanlist."$ROUND"
			cat cache.xvideos.titanlist."$ROUND" | sort -u > _full/xvideos/streams/xvideos.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
			echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/xvideos/streams/xvideos."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#XVIDEOS#3 >> _full/xvideos/xvideos.a-z.list
		fi
	done
fi

DONETIME=`date +%s`
TIME=`expr $DONETIME - $BEGINTIME`
echo "[xvideos.sh] build time: ($TIME s) done" >> _full/xvideos/build.log	
echo "[xvideos.sh] xvideos: "`ls -1 _full/xvideos` >> _full/xvideos/build.log
echo "[xvideos.sh] xvideos/streams: "`ls -1 _full/xvideos/streams` >> _full/xvideos/build.log

if [ "$buildtype" != "full" ];then
	cp -a _full/xvideos/* /var/www/atemio/web/mediathek/xvideos
fi

rm cache.*

exit
