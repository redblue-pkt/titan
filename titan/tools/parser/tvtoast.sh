#! /bin/sh

buildtype=$1
wgetbin="wget -T2 -t2 --waitretry=2"

skipall=1
count=0
count0=0
count1=0
count2=0
piccount=0

rm cache.tvtoast.*
rm tvtoast.*.m3u
rm -rf _single
rm -rf _full/tvtoast
mkdir -p _full/tvtoast/streams
mkdir _single

BEGINTIME=`date +%s`
DATENAME=`date +"%Y.%m.%d_%H.%M.%S"`
echo "[tvtoast.sh] START (buildtype: $buildtype): $DATENAME" > _full/tvtoast/build.log

if [ "$buildtype" = "full" ];then
	$wgetbin http://tvtoast.com -O cache.tvtoast.main.html

	list=`cat cache.tvtoast.main.html | tr '\n' '\r' |  tr '\r' ' ' |  tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!<a href="sub-category/!\n\n<a href="sub-category/!g' | grep ^'<a href="sub-category/' | sed 's/ /~/g'`

	for ROUND in $list; do
		count=`expr $count + 1`
		ROUND=`echo $ROUND | tr '~' ' '` 
		url=http://tvtoast.com/`echo $ROUND | cut -d'"' -f2`
		tagname=`echo $ROUND | sed 's/<h3>/\ntitle</g' | grep ^"title<" | cut -d"<" -f2`
		filename1=`echo $tagname | tr ' ' '.' | tr 'A-Z' 'a-z'`
		$wgetbin "$url" -O cache.$filename1.$count.list
		list1=`cat cache.$filename1.$count.list | tr '\n' '\r' |  tr '\r' ' ' |  tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed 's!<a href="http://tvtoast.com/watch/!\n\n<a href="http://tvtoast.com/watch/!g' | grep ^'<a href="http://tvtoast.com/watch/' | sed 's/ /~/g'`

		for ROUND1 in $list1; do
			count=`expr $count + 1`
			piccount=`expr $piccount + 1`
			ROUND1=`echo $ROUND1 | tr '~' ' '` 
			URL=`echo $ROUND1 | cut -d'"' -f2`
			TITLE=`echo $ROUND1 | sed 's/title=/\ntitle=/g' | grep ^"title=" | cut -d'"' -f2`
			picname=`echo $ROUND1 | sed 's/channel-logo color-white/\nchannel-logo-color-white"/g' | grep ^"channel-logo" | cut -d'"' -f2 | sed 's/ //g'`
			filename=`echo $TITLE | tr ' ' '.' | tr 'A-Z' 'a-z'`

			id=`echo $url | cut -d"=" -f2`

			$wgetbin http://tvtoast.com/css/channel-logo-css.css?v=$id -O cache.$filename.$count.list
			list2=`cat cache.$filename.$count.list | tr '\n' '\r' |  tr '\r' ' ' |  tr '\n' ' ' | tr '\t' ' ' | sed 's/ \+/ /g' | sed "s!$picname!\n\n$picname!g" | grep ^"$picname" | sed 's/ /~/g'`
			PIC=`echo $list2 | sed 's!../uploads/!\n\npiclink=)http://tvtoast.com/uploads/!g' | grep ^"piclink=)http://tvtoast.com/uploads" | head -n1 | cut -d")" -f2`

			if [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
				LINE="$TITLE#$URL#$PIC#tvToast_$piccount.jpg#TvToast#64"
				if [ `cat cache.tvtoast.$filename1.titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
					echo $LINE >> cache.tvtoast.$filename1.titanlist
				fi
				if [ `cat cache.tvtoast.titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
					echo $LINE >> cache.tvtoast.titanlist
				fi
			fi
			
		done
		if [ -e cache.tvtoast."$filename1".titanlist ];then
			piccount=`expr $piccount + 1`
			URL="http://atemio.dyndns.tv/mediathek/tvtoast/streams/tvtoast."$filename1".list"
			PIC="http://atemio.dyndns.tv/mediathek/menu/"$filename1".jpg"
			LINE="$tagname#$URL#$PIC#tvtast_$piccount.jpg#TvToast#0"
			echo $LINE >> cache.tvtoast.category.titanlist
			cat cache.tvtoast."$filename1".titanlist >> _full/tvtoast/streams/tvtoast."$filename1".list
		fi
	done

	cat cache.tvtoast.titanlist | sort -u > _full/tvtoast/streams/tvtoast.all-sorted.list
	cat cache.tvtoast.category.titanlist | sort -u > _full/tvtoast/tvtoast.category.list
	
	for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
		echo ROUND $ROUND
		filename=`echo "$ROUND" | tr 'A-Z' 'a-z'`
		echo filename $filename	
		if [ `cat cache.tvtoast.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
			cat cache.tvtoast.titanlist | grep ^"$ROUND" > cache.tvtoast.titanlist."$ROUND"
			cat cache.tvtoast.titanlist."$ROUND" | sort -u > _full/tvtoast/streams/tvtoast.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
			echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/tvtoast/streams/tvtoast."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#TvToast#0 >> _full/tvtoast/tvtoast.a-z.list
		elif [ `cat cache.tvtoast.titanlist | grep ^"$filename" | wc -l` -gt 0 ];then
			cat cache.tvtoast.titanlist | grep ^"$filename" > cache.tvtoast.titanlist."$ROUND"
			cat cache.tvtoast.titanlist."$ROUND" | sort -u > _full/tvtoast/streams/tvtoast.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
			echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/tvtoast/streams/tvtoast."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#TvToast#0 >> _full/tvtoast/tvtoast.a-z.list
		fi
	done
	
	
	echo tvtoast closed
fi

DONETIME=`date +%s`
TIME=`expr $DONETIME - $BEGINTIME`
echo "[tvtoast.sh] build time: ($TIME s) done" >> _full/tvtoast/build.log	
echo "[tvtoast.sh] tvtoast: "`ls -1 _full/tvtoast` >> _full/tvtoast/build.log
echo "[tvtoast.sh] tvtoast/streams: "`ls -1 _full/tvtoast/streams` >> _full/tvtoast/build.log

if [ "$buildtype" != "full" ];then
	cp -a _full/tvtoast/* /var/www/atemio/web/mediathek/tvtoast
fi

rm cache.*

exit
