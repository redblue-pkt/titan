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
#	TAGLIST=`cat cache.main.tags | grep /tags/ | cut -d'"' -f4 | tr '\n' ' ' | sort -um`
	TAGLIST=`cat cache.main.tags  | grep /c/ | sed 's!">!"<!g' | tr ' ' '~'`

echo 11111111111
ls cache.main.tags

echo TAGLIST "$TAGLIST"
#exit
#echo 2222222222222

#exit
for ROUND3 in $TAGLIST; do
	if [ "$ROUND3" = "/tags/" ] || [ "$ROUND3" = "/tags/-" ] || [ "$ROUND3" = "/tags/--" ] || [ "$ROUND3" = "/tags/---" ] || [ "$ROUND3" = "/tags/----" ];then
		continue
	fi

	piccount=`expr $piccount + 1`
	count=`expr $count + 1`	
#	filename3=`echo $ROUND3 | sed 's!http://!!' | tr '/' '.'`
	tagname=`echo $ROUND3 | tr '~' ' ' | cut -d'<' -f4`
	echo tagname $tagname
	filename3=`echo $tagname | tr ' ' '.' | tr [A-Z] [a-z]`
	echo filename3 $filename3
	tagurl=`echo $ROUND3 | cut -d'"' -f2 | sed 's!/c/!!'`
	echo tagurl $tagurl

#exit
#	wget http://www.xvideos.com$tagurl/10000 -O cache."$count"."$filename3"

#	tagcount=`cat cache."$count"."$filename3" | grep '<a class="sel">' | tail -n1 | cut -d">" -f2 | cut -d"<" -f1`
#echo tagcount $tagcount
#ls cache."$count"."$filename3"
#exit

#	if [ -z $tagcount ];then
	tagcount=10
#	fi 
	i=1
	until [ "$i" -gt "$tagcount" ]
	do
	echo $i
	tags="$tags http://www.xvideos.com/c/$i/$tagurl"
	i=$[$i+1]
	
#	if [ $i = 2 ];then
#		break
#	fi
	done
	echo tags $tags
#exit
	for ROUND4 in $tags; do
		piccount=`expr $piccount + 1`
		count=`expr $count + 1`	
		filename4=tags-$filename3
		wget "$ROUND4" -O cache."$filename4"."$count"
#		ls cache."$filename4"."$count"
#exit
		foundlist=`cat cache."$filename4"."$count" | grep '<a href="/video' | grep '><img src=' | tr ' ' '~'`
		for ROUND5 in $foundlist; do
###########

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
		URL="http://atemio.dyndns.tv/mediathek/xvideos/streams/xvideos."$filename3".list"
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