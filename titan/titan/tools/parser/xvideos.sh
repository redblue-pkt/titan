#!/bin/bash
#


rm cache.*
rm _liste
rm -rf _full/xvideos
mkdir -p _full/xvideos/streams
rm cache.*
touch cache.xvideos.titanlist

testcount=0
piccount=0
count=0

wget http://www.xvideos.com/tags/ -O cache.main.tags
TAGLIST=`cat cache.main.tags  | grep /c/ | sed 's!">!"<!g' | tr ' ' '~'`

echo TAGLIST "$TAGLIST"
for ROUND3 in $TAGLIST; do
	piccount=`expr $piccount + 1`
	count=`expr $count + 1`	
	tagname=`echo $ROUND3 | tr '~' ' ' | cut -d'<' -f4`
	echo tagname $tagname
	filename3=`echo $tagname | tr ' ' '.' | tr [A-Z] [a-z]`
	echo filename3 $filename3
	tagurl=`echo $ROUND3 | cut -d'"' -f2 | sed 's!/c/!!'`
	echo tagurl $tagurl

	tags="http://www.xvideos.com/c/$tagurl"
	tagcount=1000
	i=1
	until [ "$i" -gt "$tagcount" ]
	do
	echo $i
	tags="$tags http://www.xvideos.com/c/$i/$tagurl"
	i=$[$i+1]
	
	done

	for ROUND4 in $tags; do
		piccount=`expr $piccount + 1`
		count=`expr $count + 1`	
		filename4=tags-$filename3
		wget "$ROUND4" -O cache."$filename4"."$count"
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
			if [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
				LINE="$TITLE#http://www.xvideos.com$URL#$PIC#xvideos_$piccount.jpg#XVIDEOS#42"			

				if [ ! -e cache.xvideos."$filename4".titanlist ];then
					echo $LINE >> cache.xvideos."$filename4".titanlist
				elif [ `cat cache.xvideos."$filename4".titanlist | grep "$ROUND5" | tail -n1 | wc -l ` -eq 0 ];then
					echo $LINE >> cache.xvideos."$filename4".titanlist					
				fi

				if [ `cat cache.xvideos.titanlist | grep "^$TITLE#" | wc -l` -eq 0 ];then
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
		cat cache.xvideos.titanlist."$ROUND" | sort -um > _full/xvideos/streams/xvideos.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
		echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/xvideos/streams/xvideos."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#XVIDEOS#3 >> _full/xvideos/xvideos.a-z.list
	elif [ `cat cache.xvideos.titanlist | grep ^"$filename" | wc -l` -gt 0 ];then
		cat cache.xvideos.titanlist | grep ^"$filename" > cache.xvideos.titanlist."$ROUND"
		cat cache.xvideos.titanlist."$ROUND" | sort -um > _full/xvideos/streams/xvideos.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
		echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/xvideos/streams/xvideos."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#XVIDEOS#3 >> _full/xvideos/xvideos.a-z.list
	fi
done
rm cache.*
#cp -a _full/xvideos/* /var/www/atemio/web/mediathek/xvideos
exit
#done