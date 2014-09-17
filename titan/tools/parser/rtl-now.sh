#!/bin/bash
#

buildtype=$1
wgetbin="wget -T2 -t2 --waitretry=2"

SUBDOMAIN=rtl-now
DOMAIN=rtl.de
SHOWNAME=RTL-Now
MEDIAURL=atemio.dyndns.tv
MEDIAPATH=mediathek
STREAMTYPE=7

rm cache.*
rm -rf _full/$SUBDOMAIN
mkdir -p _full/$SUBDOMAIN/streams

BEGINTIME=`date +%s`
DATENAME=`date +"%Y.%m.%d_%H.%M.%S"`
echo "[rtl-now.sh] START (buildtype: $buildtype): $DATENAME" > _full/rtl-now/build.log

piccount=0

SITEURL=http://$SUBDOMAIN.$DOMAIN
$wgetbin --no-check-certificate $SITEURL -O cache.$SUBDOMAIN.list

SEARCHLIST=`cat cache.$SUBDOMAIN.list | grep '<a class="menu' | grep -v '&paytype=ppv' | sed 's/href="/\nlink=/' | sed 's/.php/\n/' | grep ^link=  | sed 's!link=/!!'`

for SEARCH in $SEARCHLIST; do
	echo SEARCH=$SEARCH 
	rm cache.$SEARCH.list
	piccount=`expr $piccount + 1`	
	URL="$SITEURL"/"$SEARCH".php
	$wgetbin --no-check-certificate $URL -O cache.$SEARCH.list
	PIC=`cat cache.$SEARCH.list | grep .jpg  | sed 's!src="!\n!' | tr '"' '\n' | grep ^http:// | tail -n1`
	if [ -z $PIC ]; then
		PIC=`cat cache.$SEARCH.list | grep jpg | grep _logo_ | sed 's!src="!\n!' | sed 's/">//' | grep ^http:// | tail -n1`
	fi
	if [ -z $PIC ]; then
		PIC=http://$MEDIAURL/$MEDIAPATH/menu/`echo "$SEARCH" | tr 'A-Z' 'a-z'`.jpg
	fi
	TITLE=`echo $SEARCH | tr '-' ' '`
	TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' # ' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/�x/g' -e 's/&ndash;/-/g' -e 's/&Auml;/�/g' -e 's/&Uuml;/�S/g' -e 's/&Ouml;/�/g' -e 's/&auml;/ä/g' -e 's/&uuml;/ü/g' -e 's/&ouml;/ö/g' -e 's/&eacute;/é/g' -e 's/&egrave;/è/g' -e 's/%F6/ö/g' -e 's/%FC/ü/g' -e 's/%E4/ä/g' -e 's/%26/&/g' -e 's/%C4/�/g' -e 's/%D6/�/g' -e 's/%DC/�S/g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`

	URL="http://$MEDIAURL/$MEDIAPATH/$SUBDOMAIN/streams/$SUBDOMAIN."`echo "$SEARCH" | tr 'A-Z' 'a-z'`.list

#	LINKLIST=`cat cache.$SEARCH.list | tr '><' '\n' | grep "^a href=\"/$SEARCH/" | cut -d'"' -f2 | grep film_id=`
	LINKLIST=`cat cache.$SEARCH.list | sed 's/></\n/g' | grep "^a href=\"/$SEARCH/" | cut -d'"' -f2 | grep film_id=`
	LINKTEXT=`cat cache.$SEARCH.list | sed 's/></\n/g' | grep "^a href=\"/$SEARCH/" | sed 's/class="minibutton">/\nclass="minibutton"></g' |grep ^'class="minibutton"><' | cut -d"<" -f2 | sed 's/&euro; /Euro/g' | tr ' ' '~' | sed 's/^/(/g' | sed 's/.$/&)/'`

	count=0
	TMPTYPE=""
	for ROUND in $LINKLIST; do
		echo ROUND=$ROUND
		piccount=`expr $piccount + 1`
		count=`expr $count + 1`		
		echo $wgetbin
		DURL=$SITEURL/`echo $ROUND | sed 's/amp;//'`
		echo DURL: $DURL
		echo $DURL | sed 's/amp;//'

		DURL=`echo $DURL | sed 's/amp;//' | sed 's/amp;//' | sed 's/amp;//' | sed 's/productdetail=/player=/'`
		echo DURL: $DURL

		echo $wgetbin --no-check-certificate "$DURL" -O cache.$SEARCH.$count.list
		$wgetbin --no-check-certificate "$DURL" -O cache.$SEARCH.$count.list

		DPIC=`cat cache.$SEARCH.$count.list | grep jpg | grep '<meta property="og:image"' | cut -d'"' -f4 | tail -n1`
#		TITLE=`./urldecode.sh cache.$SEARCH.$count.list | grep og:title  | cut -d'"' -f4 | sed 's/ - /#/' | cut -d"#" -f2`
		DTITLE=`cat cache.$SEARCH.$count.list | grep og:title  | cut -d'"' -f4 | sed 's/ - /#/' | cut -d"#" -f2 | sed 's/ - /#/' | cut -d"#" -f2 | tail -n1`
		DTITLE=`echo $DTITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' # ' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/�x/g' -e 's/&ndash;/-/g' -e 's/&Auml;/�/g' -e 's/&Uuml;/�S/g' -e 's/&Ouml;/�/g' -e 's/&auml;/ä/g' -e 's/&uuml;/ü/g' -e 's/&ouml;/ö/g' -e 's/&eacute;/é/g' -e 's/&egrave;/è/g' -e 's/%F6/ö/g' -e 's/%FC/ü/g' -e 's/%E4/ä/g' -e 's/%26/&/g' -e 's/%C4/�/g' -e 's/%D6/�/g' -e 's/%DC/�S/g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`

#		DURL=$SITEURL/$ROUND

		tcount=0
		for ROUNDT in $LINKTEXT; do
			echo ROUNDT $ROUNDT
		
			tcount=`expr $tcount + 1`
			if [ "$tcount" = "$count" ];then
				DTITLE="$DTITLE `echo $ROUNDT | tr '~' ' '`"
				if [ `echo "$ROUNDT" | grep "(kostenlos)" | wc -l` -eq 1 ];then
					STREAMTYPE=7
					TMPTYPE="$TMPTYPE 7"
				elif [ `echo "$ROUNDT" | grep "Euro)" | wc -l` -eq 1 ];then
					STREAMTYPE=17
					TMPTYPE="$TMPTYPE 17"
				else
					STREAMTYPE=7
					TMPTYPE="$TMPTYPE 7"
					DTITLE="$DTITLE (???)"
				fi
			fi
		done

		LINE="$DTITLE""#""$DURL""#""$DPIC""#""$SUBDOMAIN""_""$piccount"".""jpg""#""$SHOWNAME""#""$STREAMTYPE"
		echo line: $LINE
		echo "$LINE" >> cache.$SUBDOMAIN.`echo "$SEARCH" | tr 'A-Z' 'a-z'`.titanlist
		echo $LINE >> cache.$SUBDOMAIN.all.titanlist
		TMPFILE=cache.$SUBDOMAIN.`echo "$SEARCH" | tr 'A-Z' 'a-z'`.titanlist
	done

	if [ `echo $TMPTYPE | tr ' ' '\n' | grep -v "17" | grep "7" | wc -l` -gt 0 ];then
		MENU=0
	else
		MENU=1
	fi

	LINE="$TITLE""#""$URL""#""$PIC""#""$SUBDOMAIN""_""$piccount"".""jpg""#""$SHOWNAME""#""$MENU"
	echo line: $LINE
	echo "$LINE" >> cache."$SUBDOMAIN".category.titanlist

	cat cache.$SUBDOMAIN.$SEARCH.titanlist > _full/$SUBDOMAIN/streams/$SUBDOMAIN.`echo "$SEARCH" | tr 'A-Z' 'a-z'`.list
done
cat cache.$SUBDOMAIN.category.titanlist > _full/$SUBDOMAIN/$SUBDOMAIN.category.list

cat cache.$SUBDOMAIN.all.titanlist | sort -u > _full/$SUBDOMAIN/streams/$SUBDOMAIN.all-sorted.list	

for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z a b c d e f g h i j k l m n o p q r s t u v w x y z; do
	filename=`echo "$ROUND" | tr 'A-Z' 'a-z'`
	if [ `cat cache.$SUBDOMAIN.all.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
		cat cache.$SUBDOMAIN.all.titanlist | grep ^"$ROUND" > cache.$SUBDOMAIN.all.titanlist."$ROUND"
		cat cache.$SUBDOMAIN.all.titanlist."$ROUND" | sort -u > _full/$SUBDOMAIN/streams/$SUBDOMAIN.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
		echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://$MEDIAURL/$MEDIAPATH/$SUBDOMAIN/streams/$SUBDOMAIN."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://$MEDIAURL/$MEDIAPATH/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#$SHOWNAME#2 >> _full/$SUBDOMAIN/$SUBDOMAIN.a-z.list
	elif [ `cat cache.$SUBDOMAIN.all.titanlist | grep ^"$filename" | wc -l` -gt 0 ];then
		cat cache.$SUBDOMAIN.all.titanlist | grep ^"$filename" > cache.$SUBDOMAIN.all.titanlist."$ROUND"
		cat cache.$SUBDOMAIN.all.titanlist."$ROUND" | sort -u > _full/$SUBDOMAIN/streams/$SUBDOMAIN.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
		echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://$MEDIAURL/$MEDIAPATH/$SUBDOMAIN/streams/$SUBDOMAIN."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://$MEDIAURL/$MEDIAPATH/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#$SHOWNAME#2 >> _full/$SUBDOMAIN/$SUBDOMAIN.a-z.list
	fi
done

DONETIME=`date +%s`
TIME=`expr $DONETIME - $BEGINTIME`
echo "[rtl-now.sh] build time: ($TIME s) done" >> _full/rtl-now/build.log	
echo "[rtl-now.sh] rtl-now: "`ls -1 _full/rtl-now` >> _full/rtl-now/build.log
echo "[rtl-now.sh] rtl-now/streams: "`ls -1 _full/rtl-now/streams` >> _full/rtl-now/build.log

if [ "$buildtype" != "full" ];then
	cp -a _full/rtl-now/* /var/www/atemio/web/mediathek/rtl-now
fi

rm cache.*

exit
