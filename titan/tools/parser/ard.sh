#!/bin/bash
#

buildtype=$1
wgetbin="wget -T2 -t2 --waitretry=2"

rm cache.*
rm _liste
rm -rf _full/ard

mkdir -p _full/ard/streams
piccount=0

BEGINTIME=`date +%s`
DATENAME=`date +"%Y.%m.%d_%H.%M.%S"`
echo "[ard.sh] START (buildtype: $buildtype): $DATENAME" > _full/ard/build.log
DOMAIN=ard
SHOWNAME=ARD

##################
# watchlist
##################

for ROUND in A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
	count=`expr $count + 1`
	filename=`echo $ROUND | tr 'A-Z' 'a-z'`
	$wgetbin http://www.ardmediathek.de/tv/sendungen-a-z?buchstabe=$ROUND -O cache.$filename.$count.html
	LIST=`cat cache.$filename.$count.html | tr '\r' ' ' | tr '\n' ' ' | sed 's/ \+/ /g' | sed 's!<a href="/tv/!\nfshow=/tv/!g' | grep ^fshow=  | grep "?documentId" | grep -v sendungsTyp | tr ' ' '~'`
	for ROUND1 in $LIST; do

		count=`expr $count + 1`
		SECTION=`echo $ROUND1 | cut -d "/" -f3`
		TITLE=`echo $ROUND1 | cut -d "/" -f3 | tr '-' ' '`
		filename1=`echo $TITLE | tr 'A-Z' 'a-z' | tr -d '\n' | tr -d '\r' | tr -d '\t' | tr -d ' ' | tr -d '%'`

		TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/&#043;/+/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/#043;/+/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/+/ /g' -e 's/,/ /g' -e 's/;/ /g' -e 's/\.\+/./g' -e 's/+/ /g' -e 's/#/ /g' -e 's/\.\+/ /g' -e 's/\ \+/ /g' -e 's/^ //'`
		TITLE=`echo $TITLE | sed 's/\(.*\)\./\1\t/g' | sed 's/^\.//g'`
		TITLE=`echo $(php -r "echo rawurldecode('$TITLE');")`
						
		URL="http://atemio.dyndns.tv/mediathek/$DOMAIN/streams/$DOMAIN.category.$filename1.list"
		LPATH=`echo $ROUND1 | sed 's/"~class=/\n/' | grep fshow=/ | sed 's/fshow=//'`
		PIC=http://www.ardmediathek.de"`echo $ROUND1 | sed 's!/image/!\n/image/!' | grep ^/image/ | sed 's/##width##.*/0/'`"
		STREAMTYPE=0

		LINE="$TITLE""#""$URL""#""$PIC""#""$DOMAIN""_""$filename1"".""jpg""#""$SHOWNAME""#""$STREAMTYPE"
		if [ ! -e cache.$DOMAIN.category.titanlist ] || [ `cat cache.$DOMAIN.category.titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
			echo "$LINE" >> cache.$DOMAIN.category.titanlist
		fi
		$wgetbin http://www.ardmediathek.de"$LPATH" -O cache.$filename.$filename1.$count.html
		LIST2=`cat cache.$filename.$filename1.$count.html | tr '\r' ' ' | tr '\n' ' ' | sed 's/ \+/ /g' | sed "s!<a href=\"/tv/$SECTION/!\nfshow=/tv/$SECTION/!g" | grep ^fshow=  | grep "?documentId" | grep -v "Sendung?documentId" | grep -v sendungsTyp | tr ' ' '~'`
			
		for ROUND2 in $LIST2; do
			piccount=`expr $piccount + 1`	
			TITLE=`echo $ROUND2 | cut -d "/" -f4 | tr '-' ' '`
			TITLE="$TITLE - `echo $ROUND2 | cut -d "/" -f5 | tr '-' ' '`"
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/&#043;/+/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/#043;/+/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/+/ /g' -e 's/,/ /g' -e 's/;/ /g' -e 's/\.\+/./g' -e 's/+/ /g' -e 's/#/ /g' -e 's/\.\+/ /g' -e 's/\ \+/ /g' -e 's/^ //'`
			TITLE=`echo $TITLE | sed 's/\(.*\)\./\1\t/g' | sed 's/^\.//g'`
			TITLE=`echo $(php -r "echo rawurldecode('$TITLE');")`
#			URL=http://www.ardmediathek.de"`echo $ROUND2 | sed 's/"~class=/\n/' | grep fshow=/ | sed 's/fshow=//'`"
			URL=http://www.ardmediathek.de/play/media/"`echo $ROUND2 | sed 's/"~class=/\n/' | grep fshow=/ | sed 's/fshow=//' | sed 's!?documentId=!\ndocumentId=&!' | grep ^documentId= | cut -d "&" -f1 | sed 's!?documentId=!!' | sed 's!documentId=!!'`?devicetype=pc&features=flash"

			PIC=http://www.ardmediathek.de"`echo $ROUND2 | sed 's!/image/!\n/image/!' | grep ^/image/ | sed 's/##width##.*/0/'`"
			STREAMTYPE=45
			LINE="$TITLE""#""$URL""#""$PIC""#""$DOMAIN""_""$filename""_""$filename1""_""$piccount"".""jpg""#""$SHOWNAME""#""$STREAMTYPE"
			if [ ! -e cache.$DOMAIN.category.$filename1.titanlist ] || [ `cat cache.$DOMAIN.category.$filename1.titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
				echo "$LINE" >> cache.$DOMAIN.category.$filename1.titanlist
			fi
			if [ ! -e  cache.ard.titanlist ] || [ `cat  cache.ard.titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
				echo "$LINE" >>  cache.ard.titanlist
			fi
		done
		if [ -e cache.$DOMAIN.category.$filename1.titanlist ];then
			cat cache.$DOMAIN.category.$filename1.titanlist | sort -u > _full/ard/streams/$DOMAIN.category.$filename1.list		
		fi
	done
done

##############
if [ "$buildtype" = "full" ];then
	cat cache.ard.titanlist | sort -u > _full/ard/streams/ard.all-sorted.list
#	cat cache.ard.category.titanlist | sort -u > _full/ard/ard.category.list
	cat cache.ard.category.titanlist > _full/ard/ard.category.list
	
	for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
		filename=`echo "$ROUND" | tr 'A-Z' 'a-z'`
		if [ `cat cache.ard.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
			cat cache.ard.titanlist | grep ^"$ROUND" > cache.ard.titanlist."$ROUND"
			cat cache.ard.titanlist."$ROUND" | sort -u > _full/ard/streams/ard.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
			echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/ard/streams/ard."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#ARD#3 >> _full/ard/ard.a-z.list
		elif [ `cat cache.ard.titanlist | grep ^"$filename" | wc -l` -gt 0 ];then
			cat cache.ard.titanlist | grep ^"$filename" > cache.ard.titanlist."$ROUND"
			cat cache.ard.titanlist."$ROUND" | sort -u > _full/ard/streams/ard.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
			echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/ard/streams/ard."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#ARD#3 >> _full/ard/ard.a-z.list
		fi
	done
fi

DONETIME=`date +%s`
TIME=`expr $DONETIME - $BEGINTIME`
echo "[ard.sh] build time: ($TIME s) done" >> _full/ard/build.log	
echo "[ard.sh] ard: "`ls -1 _full/beeg` >> _full/ard/build.log
echo "[ard.sh] ard/streams: "`ls -1 _full/ard/streams` >> _full/ard/build.log

if [ "$buildtype" != "full" ];then
	cp -a _full/ard/* /var/www/atemio/web/mediathek/ard
fi

rm cache.*
exit