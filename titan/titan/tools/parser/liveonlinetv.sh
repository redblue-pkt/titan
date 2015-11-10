#!/bin/bash
#

buildtype=$1
wgetbin="wget -T2 -t2 --waitretry=2"
rm cache.*
rm _liste
rm -rf _full/liveonlinetv
mkdir -p _full/liveonlinetv/streams
rm cache.*
touch cache.liveonlinetv.titanlist

BEGINTIME=`date +%s`
DATENAME=`date +"%Y.%m.%d_%H.%M.%S"`
echo "[liveonlinetv.sh] START (buildtype: $buildtype): $DATENAME" > _full/liveonlinetv/build.log

piccount=0
count=0

$wgetbin "http://www.liveonlinetv247.info/tvchannels.php" -O cache.liveonlinetv.cache


#cat cache.liveonlinetv.cache | tr '\n' '\r' |  tr '\r' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!<td><a href=!\n<td><a href=!g' | grep ^"<td><a href=" | tr ' ' '~' > cache.liveonlinetv1.cache
#http://www.liveonlinetv247.com/embed/tensports.php?width=650&amp;height=480

LIST=`cat cache.liveonlinetv.cache | tr '\n' '\r' |  tr '\r' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!<td><a href=!\n<td><a href=!g' | grep ^"<td><a href=" | tr ' ' '~'`
echo LIST $LIST

count1=0
#for ROUND1 in $LIST; do
#	count1=`expr $count + 1`
#
#	echo PAGELIST $PAGELIST	
#	$wgetbin "$ROUND1" -O cache.liveonlinetv.$count1.cache
#	LIST=`cat cache.liveonlinetv.$count1.cache | tr '\n' '\r' |  tr '\r' ' ' | tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!<a href="http://www.liveonlinetv.to/view/!\nhttp://www.liveonlinetv.to/view/!g' | grep ^"http://www.liveonlinetv.to/view/" | grep -v "<strong>" | grep -v "premium_only" | tr ' ' '~'`
#	
#	echo LIST $LIST
	for ROUND2 in $LIST; do
		count=`expr $count + 1`
		ROUND2=`echo $ROUND2 | tr '~' ' '`

		URL="http://www.liveonlinetv247.com/embed/"`echo $ROUND2 | cut -d'"' -f2`"?width=650&amp;height=480"
		TITLE=`echo $ROUND2 | cut -d'"' -f2 | sed -e 's!/!!g' -e 's/.php//g'`
		PIC=`echo $ROUND2 | cut -d'"' -f4`
		if [ -z "$PIC" ];then
			PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
		fi
		TITLE=`echo $TITLE | sed 's/-/ /g' | sed 's/_/ /g'`
		TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
	
		echo PIC $PIC
		echo URL $URL
		echo TITLE "$TITLE"
	
		echo "################################################"
	
		if [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
			piccount=`expr $piccount + 1`
			LINE="$TITLE#$URL#$PIC#liveonlinetv_$piccount.png#LiveOnlineTv#93"
	#				if [ `cat cache.liveonlinetv."$filename".titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
	#					echo $LINE >> cache.liveonlinetv."$filename".titanlist
	#				fi
			if [ `cat cache.liveonlinetv.titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
				echo $LINE >> cache.liveonlinetv.titanlist
			fi
		fi
	done
#done

if [ "$buildtype" = "full" ];then
	cat cache.liveonlinetv.titanlist | sort -u > _full/liveonlinetv/streams/liveonlinetv.all-sorted.list
	#cat cache.liveonlinetv.category.titanlist | sort -u > _full/liveonlinetv/liveonlinetv.category.list
#	cat cache.liveonlinetv.category.titanlist > _full/liveonlinetv/liveonlinetv.category.list
	
	for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
		filename=`echo "$ROUND" | tr 'A-Z' 'a-z'`
		if [ `cat cache.liveonlinetv.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
			cat cache.liveonlinetv.titanlist | grep ^"$ROUND" > cache.liveonlinetv.titanlist."$ROUND"
			cat cache.liveonlinetv.titanlist."$ROUND" | sort -u > _full/liveonlinetv/streams/liveonlinetv.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
			echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/liveonlinetv/streams/liveonlinetv."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#LiveOnlineTv#0 >> _full/liveonlinetv/liveonlinetv.a-z.list
		elif [ `cat cache.liveonlinetv.titanlist | grep ^"$filename" | wc -l` -gt 0 ];then
			cat cache.liveonlinetv.titanlist | grep ^"$filename" > cache.liveonlinetv.titanlist."$ROUND"
			cat cache.liveonlinetv.titanlist."$ROUND" | sort -u > _full/liveonlinetv/streams/liveonlinetv.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
			echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/liveonlinetv/streams/liveonlinetv."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#LiveOnlineTv#0 >> _full/liveonlinetv/liveonlinetv.a-z.list
		fi
	done
fi

DONETIME=`date +%s`
TIME=`expr $DONETIME - $BEGINTIME`
echo "[liveonlinetv.sh] build time: ($TIME s) done" >> _full/liveonlinetv/build.log	
echo "[liveonlinetv.sh] liveonlinetv: "`ls -1 _full/liveonlinetv` >> _full/liveonlinetv/build.log
echo "[liveonlinetv.sh] liveonlinetv/streams: "`ls -1 _full/liveonlinetv/streams` >> _full/liveonlinetv/build.log

if [ "$buildtype" != "full" ];then
	cp -a _full/liveonlinetv/* /var/www/atemio/web/mediathek/liveonlinetv
fi

rm cache.*

exit
