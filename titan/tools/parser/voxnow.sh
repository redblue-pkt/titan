SUBDOMAIN=voxnow
DOMAIN=de
SHOWNAME=VOX-Now
MEDIAURL=atemio.dyndns.tv
MEDIAPATH=mediathek
STREAMTYPE=8

rm cache.*
rm -rf _full/$SUBDOMAIN
mkdir -p _full/$SUBDOMAIN/streams
piccount=0

SITEURL=http://www.$SUBDOMAIN.$DOMAIN
wget --no-check-certificate $SITEURL -O cache.$SUBDOMAIN.list

SEARCHLIST=`cat cache.$SUBDOMAIN.list | grep '<a class="menu' | grep -v '&paytype=ppv' | sed 's/href="/\nlink=/' | sed 's/.php/\n/' | grep ^link=  | sed 's!link=/!!'`

for SEARCH in $SEARCHLIST; do
	echo SEARCH=$SEARCH 
	rm cache.$SEARCH.list
	piccount=`expr $piccount + 1`	
	URL="$SITEURL"/"$SEARCH".php
	wget --no-check-certificate $URL -O cache.$SEARCH.list
	PIC=`cat cache.$SEARCH.list | grep .jpg  | sed 's!src="!\n!' | tr '"' '\n' | grep ^http:// | tail -n1`
	if [ -z $PIC ]; then
		PIC=`cat cache.$SEARCH.list | grep jpg | grep _logo_ | sed 's!src="!\n!' | sed 's/">//' | grep ^http:// | tail -n1`
	fi
	if [ -z $PIC ]; then
		PIC=http://$MEDIAURL/$MEDIAPATH/menu/`echo "$SEARCH" | tr 'A-Z' 'a-z'`.jpg
	fi
	TITLE=`echo $SEARCH | tr '-' ' '`
	
	URL="http://$MEDIAURL/$MEDIAPATH/$SUBDOMAIN/streams/$SUBDOMAIN."`echo "$SEARCH" | tr 'A-Z' 'a-z'`.list
	
	LINE="$TITLE""#""$URL""#""$PIC""#""$SUBDOMAIN""_""$piccount"".""jpg""#""$SHOWNAME""#""1"
	echo line: $LINE
	echo "$LINE" >> cache."$SUBDOMAIN".category.titanlist

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

		DURL=`echo $DURL | sed 's/amp;//' | sed 's/productdetail=/player=/'`
		echo DURL: $DURL
		
		echo wget --no-check-certificate "$DURL" -O cache.$SEARCH.$count.list
		wget --no-check-certificate "$DURL" -O cache.$SEARCH.$count.list

		PIC=`cat cache.$SEARCH.$count.list | grep jpg | grep '<meta property="og:image"' | cut -d'"' -f4 | tail -n1`
#		TITLE=`./urldecode.sh cache.$SEARCH.$count.list | grep og:title  | cut -d'"' -f4 | sed 's/ - /#/' | cut -d"#" -f2`
		TITLE=`cat cache.$SEARCH.$count.list | grep og:title  | cut -d'"' -f4 | sed 's/ - /#/' | cut -d"#" -f2 | sed 's/ - /#/' | cut -d"#" -f2 | tail -n1`
		URL=$SITEURL/$ROUND

		LINE="$TITLE""#""$DURL""#""$PIC""#""$SUBDOMAIN""_""$piccount"".""jpg""#""$SHOWNAME""#""$STREAMTYPE"
		echo line: $LINE
		echo "$LINE" >> cache.$SUBDOMAIN.`echo "$SEARCH" | tr 'A-Z' 'a-z'`.titanlist
		echo $LINE >> cache.$SUBDOMAIN.all.titanlist
	done
	cat cache.$SUBDOMAIN.$SEARCH.titanlist > _full/$SUBDOMAIN/streams/$SUBDOMAIN.`echo "$SEARCH" | tr 'A-Z' 'a-z'`.list
done
cat cache.$SUBDOMAIN.category.titanlist > _full/$SUBDOMAIN/$SUBDOMAIN.category.list

cat cache.$SUBDOMAIN.all.titanlist | sort -u > _full/$SUBDOMAIN/streams/$SUBDOMAIN.all-sorted.list	

for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z a b c d e f g h i j k l m n o p q r s t u v w x y z; do
	if [ `cat cache.$SUBDOMAIN.all.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
		cat cache.$SUBDOMAIN.all.titanlist | grep ^"$ROUND" > cache.$SUBDOMAIN.all.titanlist."$ROUND"
		cat cache.$SUBDOMAIN.all.titanlist."$ROUND" | sort -u > _full/$SUBDOMAIN/streams/$SUBDOMAIN.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
		echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://$MEDIAURL/$MEDIAPATH/$SUBDOMAIN/streams/$SUBDOMAIN."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://$MEDIAURL/$MEDIAPATH/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#$SHOWNAME#1 >> _full/$SUBDOMAIN/$SUBDOMAIN.a-z.list
	fi
done

rm cache.*
