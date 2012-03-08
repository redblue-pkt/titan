rm cache.*
rm -rf _full
mkdir -p _full/rtl2now/streams
piccount=0
SITEURL=http://rtl2now.rtl2.de
wget --no-check-certificate $SITEURL -O cache.rtl2now.list

SEARCHLIST=`cat cache.rtl2now.list | grep '<a class="menu' | grep -v '&paytype=ppv' | sed 's/href="/\nlink=/' | sed 's/.php/\n/' | grep ^link=  | sed 's!link=/!!'`

for SEARCH in $SEARCHLIST; do
	echo SEARCH=$SEARCH 
	rm cache.$SEARCH.list
	piccount=`expr $piccount + 1`	
	URL="$SITEURL"/"$SEARCH".php
	wget --no-check-certificate $URL -O cache.$SEARCH.list
	PIC=`cat cache.$SEARCH.list | grep jpg | grep _logo_ | sed 's!src="!\n!' | sed 's/">//' | grep ^http://`
	TITLE=`echo $SEARCH | tr '-' ' '`
	
	URL="http://atemio.dyndns.tv/mediathek/rtl2now/streams/rtl2now."`echo "$SEARCH" | tr 'A-Z' 'a-z'`.list
	
	LINE="$TITLE#$URL#$PIC#rtl2now_$piccount.jpg#Rtl2Now#1"
	echo line: $LINE
	echo "$LINE" >> cache.rtl2now.category.titanlist

	LINKLIST=`cat cache.$SEARCH.list | tr '><' '\n' | grep "^a href=\"/$SEARCH/" | cut -d'"' -f2 | grep film_id=`

	count=0
	for ROUND in $LINKLIST; do
		echo ROUND=$ROUND
		piccount=`expr $piccount + 1`
		count=`expr $count + 1`		
		echo wget
		DURL=$SITEURL/`echo $ROUND | sed 's/amp;//'`
		echo DURL: $DURL
		echo $DURL | sed 's/amp;//'

		DURL=`echo $DURL | sed 's/amp;//'`
		echo DURL: $DURL
		
		echo wget --no-check-certificate "$DURL" -O cache.$SEARCH.$count.list
		wget --no-check-certificate "$DURL" -O cache.$SEARCH.$count.list

		PIC=`cat cache.$SEARCH.$count.list | grep jpg | grep '<meta property="og:image"' | cut -d'"' -f4`
#		TITLE=`./urldecode.sh cache.$SEARCH.$count.list | grep og:title  | cut -d'"' -f4 | sed 's/ - /#/' | cut -d"#" -f2`
		TITLE=`cat cache.$SEARCH.$count.list | grep og:title  | cut -d'"' -f4 | sed 's/ - /#/' | cut -d"#" -f2 | sed 's/ - /#/' | cut -d"#" -f2`
		URL=$SITEURL/$ROUND

		LINE="$TITLE#$DURL#$PIC#rtl2now_$piccount.jpg#Rtl2Now#5"
		echo line: $LINE
		echo "$LINE" >> cache.rtl2now.`echo "$SEARCH" | tr 'A-Z' 'a-z'`.titanlist
		echo $LINE >> cache.rtl2now.all.titanlist
	done
	cat cache.rtl2now.$SEARCH.titanlist > _full/rtl2now/streams/rtl2now.`echo "$SEARCH" | tr 'A-Z' 'a-z'`.list
done
cat cache.rtl2now.category.titanlist > _full/rtl2now/rtl2now.category.list

cat cache.rtl2now.all.titanlist | sort -u > _full/rtl2now/streams/rtl2now.all-sorted.list	

for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z a b c d e f g h i j k l m n o p q r s t u v w x y z; do
	if [ `cat cache.rtl2now.all.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
		cat cache.rtl2now.all.titanlist | grep ^"$ROUND" > cache.rtl2now.all.titanlist."$ROUND"
		cat cache.rtl2now.all.titanlist."$ROUND" | sort -u > _full/rtl2now/streams/rtl2now.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
		echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/rtl2now/streams/rtl2now."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#Rtl2Now#1 >> _full/rtl2now/rtl2now.a-z.list
	fi
done

rm cache.*
