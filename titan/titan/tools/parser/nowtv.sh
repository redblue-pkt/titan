#!/bin/bash
#

buildtype=$1
wgetbin="wget -T2 -t2 --waitretry=2"

DOMAIN=nowtv.de
SHOWNAME=NowTv
MEDIAURL=atemio.dyndns.tv
MEDIAPATH=mediathek
STREAMTYPE=7

rm cache.*
rm -rf _full/$DOMAIN
mkdir -p _full/$DOMAIN/streams

BEGINTIME=`date +%s`
DATENAME=`date +"%Y.%m.%d_%H.%M.%S"`
echo "[$DOMAIN.sh] START (buildtype: $buildtype): $DATENAME" > _full/$DOMAIN/build.log

count=0
piccount=0

WATCHLIST="rtl vox rtl2 superrtl nitro ntv"

for ROUND in $WATCHLIST; do
	count=`expr $count + 1`
	piccount=`expr $piccount + 1`	

	SHOWNAME=`echo $DOMAIN | tr 'a-z' 'A-Z'`
	TITLE=`echo $ROUND | tr 'a-z' 'A-Z'`
	PIC="http://atemio.dyndns.tv/mediathek/menu/$ROUND.jpg"
	URL="http://atemio.dyndns.tv/mediathek/$DOMAIN/$DOMAIN.$ROUND.category.list"
	STREAMTYPE=0

	LINE="$TITLE""#""$URL""#""$PIC""#""$DOMAIN""_""$ROUND"".""jpg""#""$SHOWNAME""#""$STREAMTYPE"
	echo "$LINE" >> cache.$DOMAIN.category.titanlist

	$wgetbin --no-check-certificate "https://api.nowtv.de/v3/formats?filter=%7B%22Disabled%22%3A+%220%22%2C+%22Station%22%3A+%22$ROUND%22%2C+%22CategoryId%22%3A+%7B%22containsIn%22%3A+%5B%22serie%22%2C+%22news%22%5D%7D%7D&fields=title%2Cstation%2Ctitle%2CtitleGroup%2CseoUrl%2CcategoryId%2C%2A&maxPerPage=1000" -O cache.$DOMAIN.$ROUND.list

	SEARCHLIST=`cat cache.$DOMAIN.$ROUND.list | sed 's/{/\n{/g' | grep ^'{"title"' | sed 's/ \+/~/g'`

	for SEARCH in $SEARCHLIST; do
		piccount=`expr $piccount + 1`	
	
		TITLE=`echo $SEARCH | sed 's/"title":/\n"title":/' | grep ^'"title":' | cut -d '"' -f4 | sed 's/~/ /g'`
	
		PIC=`echo $SEARCH | sed 's/"defaultDvdImage":/\n"defaultDvdImage":/' | grep ^'"defaultDvdImage":' | cut -d '"' -f4 | sed 's/~/ /g' | sed 's!\\\!!g'`

		echo PIC $PIC
		filename=`echo $SEARCH | sed 's/"seoUrl":/\n"seoUrl":/' | grep ^'"seoUrl":' | cut -d '"' -f4 | sed 's/~/ /g'`
		
		URL="https://api.nowtv.de/v3/formats/seo?fields=*,.*,formatTabs.*,formatTabs.formatTabPages.*&name=$filename.php"
		$wgetbin --no-check-certificate "$URL" -O cache.$DOMAIN.$ROUND.$filename.seo.list

		ID=`cat cache.$DOMAIN.$ROUND.$filename.seo.list | sed 's!"formatTabs":{"items":!\nid=!' | grep ^'id=' | cut -d ',' -f1 | cut -d ':' -f2 | sed 's/~/ /g' | sed 's!\\\!!g'`

		URL="https://api.nowtv.de/v3/formatlists/$ID?maxPerPage=100&fields=%2A%2CformatTabPages.%2A%2CformatTabPages.container.%2A%2CformatTabPages.container.movies.%2A%2CformatTabPages.container.movies.format.%2A%2CformatTabPages.container.movies.paymentPaytypes.%2A%2CformatTabPages.container.movies.pictures&page=1"

		$wgetbin --no-check-certificate "$URL" -O cache.$DOMAIN.$ROUND.$filename.list
		FINDLIST=`cat cache.$DOMAIN.$ROUND.$filename.list | sed 's/{"orderWeight"/\norderWeight="/g' | grep ^'orderWeight=' | sed 's/ \+/~/g'`
		
		for SEARCH in $FINDLIST; do
			piccount=`expr $piccount + 1`	
			STITLE=`echo $SEARCH | sed 's/"title":/\n"title":/' | grep ^'"title":' | cut -d '"' -f4 | sed 's/~/ /g'`
			SPIC=`echo $SEARCH | sed 's/"defaultHeaderSmallImage":/\n"defaultHeaderSmallImage":/' | grep ^'"defaultHeaderSmallImage":' | cut -d '"' -f4 | sed 's/~/ /g' | sed 's!\\\!!g'`
			filename2=`echo $SEARCH | sed 's/"seoUrl":/\n"seoUrl":/' | grep ^'"seoUrl":' | cut -d '"' -f4 | sed 's/~/ /g'`

			filename2=`echo $SEARCH | sed 's/"seoUrl":/\n"seoUrl":/' | grep ^'"seoUrl":' | cut -d '"' -f4 | sed 's/~/ /g'`
			STREAMURL="https://api.nowtv.de/v3/movies/$filename/$filename2?fields=fields%3D%2A%2Cformat%2Cfiles%2Cbreakpoints%2CpaymentPaytypes%2Ctrailers"

			STREAMTYPE=2222

			PAY=`echo $SEARCH | sed 's/"price":/\n"price":/' | grep ^'"price":' | cut -d '"' -f4 | sed 's/~/ /g' | sed 's!\\\!!g'`

			if [ ! -z "$PAY" ] && [ `echo $SEARCH | grep '"free":false' | wc -l` -gt 0 ];then
				STREAMTYPE=3333
				PAY=`echo $SEARCH | sed 's/"price":/\n"price":/' | grep ^'"price":' | cut -d '"' -f4 | sed 's/~/ /g' | sed 's!\\\!!g'`
				WARE=`echo $SEARCH | sed 's/"currency":/\n"currency":/' | grep ^'"currency":' | cut -d '"' -f4 | sed 's/~/ /g' | sed 's!\\\!!g'`
				STITLE="$STITLE ($PAY $WARE)"
			fi

			ID=`echo $SEARCH | sed 's!"id":!\nid=,!' | grep ^'id=' | cut -d ',' -f2 | cut -d ':' -f2 | sed 's/~/ /g' | sed 's!\\\!!g'`
			SPIC=http://autoimg.rtl.de/rtlnow/$ID/660x660/formatimage.jpg


#			$wgetbin --no-check-certificate "$STREAMURL" -O cache.$DOMAIN.$ROUND.$filename.$filename2.list
#			PIC2=`echo $SEARCH | sed 's/"defaultHeaderSmallImage":/\n"defaultHeaderSmallImage":/' | grep ^'"defaultHeaderSmallImage":' | cut -d '"' -f4 | sed 's/~/ /g' | sed 's!\\\!!g'`
#			echo PIC2 $PIC2
#exit
#			PATH=`cat cache.$DOMAIN.$ROUND.$filename.$filename2.list | sed 's/"path":/\npath=/' | grep ^'path=' | cut -d'"' -f2 | tr -d '\\\'`
			piccount=`expr $piccount + 1`	

			LINE="$STITLE""#""$STREAMURL""#""$SPIC""#""$DOMAIN""_""$piccount"".""jpg""#""$SHOWNAME""#""$STREAMTYPE"

			echo "$LINE" >> cache.$DOMAIN.$ROUND.$filename.titanlist
			echo "$LINE" >> cache.$DOMAIN.all.titanlist
		done

		STREAMTYPE=0
		SHOWNAME=NOWTV

		URL="http://atemio.dyndns.tv/mediathek/$DOMAIN/streams/$DOMAIN.$ROUND.$filename.list"

		LINE="$TITLE""#""$URL""#""$PIC""#""$DOMAIN""_""$piccount"".""jpg""#""$SHOWNAME""#""$STREAMTYPE"

		echo "$LINE" >> cache.$DOMAIN.$ROUND.category.titanlist
	
		cat cache.$DOMAIN.$ROUND.$filename.titanlist > _full/$DOMAIN/streams/$DOMAIN.$ROUND.`echo "$filename" | tr 'A-Z' 'a-z'`.list
			
	done
	cat cache.$DOMAIN.$ROUND.category.titanlist | sort -u > _full/$DOMAIN/$DOMAIN.$ROUND.category.list

done

cat cache.$DOMAIN.category.titanlist | sort -u > _full/$DOMAIN/$DOMAIN.category.list
cat cache.$DOMAIN.all.titanlist | sort -u > _full/$DOMAIN/streams/$DOMAIN.all-sorted.list	

for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z a b c d e f g h i j k l m n o p q r s t u v w x y z; do
	filename=`echo "$ROUND" | tr 'A-Z' 'a-z'`
	if [ `cat cache.$DOMAIN.all.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
		cat cache.$DOMAIN.all.titanlist | grep ^"$ROUND" > cache.$DOMAIN.all.titanlist."$ROUND"
		cat cache.$DOMAIN.all.titanlist."$ROUND" | sort -u > _full/$DOMAIN/streams/$DOMAIN.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
		echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://$MEDIAURL/$MEDIAPATH/$DOMAIN/streams/$DOMAIN."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://$MEDIAURL/$MEDIAPATH/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#$SHOWNAME#2 >> _full/$DOMAIN/$DOMAIN.a-z.list
	elif [ `cat cache.$DOMAIN.all.titanlist | grep ^"$filename" | wc -l` -gt 0 ];then
		cat cache.$DOMAIN.all.titanlist | grep ^"$filename" > cache.$DOMAIN.all.titanlist."$ROUND"
		cat cache.$DOMAIN.all.titanlist."$ROUND" | sort -u > _full/$DOMAIN/streams/$DOMAIN.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
		echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://$MEDIAURL/$MEDIAPATH/$DOMAIN/streams/$DOMAIN."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://$MEDIAURL/$MEDIAPATH/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#$SHOWNAME#2 >> _full/$DOMAIN/$DOMAIN.a-z.list
	fi
done

DONETIME=`date +%s`
TIME=`expr $DONETIME - $BEGINTIME`
echo "[$DOMAIN.sh] build time: ($TIME s) done" >> _full/$DOMAIN/build.log	
echo "[$DOMAIN.sh] $DOMAIN: "`ls -1 _full/$DOMAIN` >> _full/$DOMAIN/build.log
echo "[$DOMAIN.sh] $DOMAIN/streams: "`ls -1 _full/$DOMAIN/streams` >> _full/$DOMAIN/build.log

if [ "$buildtype" != "full" ];then
	cp -a _full/$DOMAIN/* /var/www/atemio/web/mediathek/$DOMAIN
fi

rm cache.*

exit
