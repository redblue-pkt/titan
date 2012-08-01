#!/bin/bash
#

rm cache.*
rm -rf _full/myvideo
mkdir -p _full/myvideo/streams
piccount=0

wget --no-check-certificate "http://myvideo.de" -O cache.main.list

LIST=`cat cache.main.list | tr '><' '>\n<' | grep "a onclick=" | grep -v "Themen/Alle_Channels" | grep -v meinoutback | grep -v News | grep -v apps | grep -v Forum | grep -v Community | grep -v "href='http:" | grep class | sed "s/a onclick=\"gaet({'u':this.href,'c':'new_home','a':'top_sub_nav','o':'//" | sed "s!','v':''});return false;\" href='/!#!" | cut -d"'" -f1 | tr ' ' '~'`

for ROUND in $LIST; do
	echo round $ROUND
	TITLE=`echo "$ROUND" | cut -d"#" -f1 | tr '~' ' '`
	WEBPATH=`echo "$ROUND" | cut -d"#" -f2`	
	echo TITLE $TITLE
	echo WEBPATH $WEBPATH
	filename=`echo $WEBPATH | tr '/' '.' | tr '_' '.' | tr '%' '.' | tr 'A-Z' 'a-z'`
	echo filename $filename
	
	wget --no-check-certificate "http://myvideo.de/$WEBPATH" -O cache.$filename.list
	LIST1=`cat cache.$filename.list | tr '><' '>\n<' | grep "^img id='i" | tr ' ' '~'`
	echo LIST1 $LIST1
	PAGES=`cat cache.$filename.list | tr '><' '>\n<' | grep "page" | grep 'pView pnNumbers' | cut -d ">" -f2 | tr '\n' ' '`
	echo PAGES $PAGES
	echo $PAGES >cache.myvideo.$filename.pages.titanlist

	echo "$TITLE""#http://atemio.dyndns.tv/mediathek/myvideo/streams/myvideo."$filename".list#http://atemio.dyndns.tv/mediathek/menu/"$filename".jpg#"$filename".jpg"#MyVideo#1 >> cache.myvideo.category.titanlist

	if [ -z "$PAGES" ];then
		for ROUND1 in $LIST1; do
			piccount=`expr $piccount + 1`
			echo round1 $ROUND1

			ROUND1=`echo $ROUND1 | tr '~' '\n'`
			ID=`echo $ROUND1 | tr '~' '\n' | grep "id='" | cut -d"'" -f2 | sed 's/i//'`
			echo ID $ID

			PIC=`cat "cache.$filename.list" | tr ' ' '\n' | grep "$ID"| grep ".jp" | cut -d "'" -f2 | tr ' ' '\n' | head -n1`
			echo PIC $PIC

			TITLE=`cat cache.$filename.list | tr ' ' '\n' | grep "/watch/$ID" | head -n1 | cut -d"'" -f2 | cut -d"/" -f4 | tr '_' ' '`			
			echo TITLE $TITLE
	
			URL=http://www.myvideo.de/watch/$ID/`echo $TITLE | tr ' ' '_'`
			echo URL $URL
			LINE="$TITLE#$URL#$PIC#myvideo_$piccount.jpg#MyVideo#12"
			if [ ! -z "$TITLE" ]; then
				echo $LINE >> cache.myvideo."$filename".titanlist
				echo $LINE >> cache.myvideo.all.titanlist
			fi
		done
	else
		for PAGE in $PAGES; do
			echo PAGE $PAGE
			wget --no-check-certificate "http://myvideo.de/$WEBPATH?lpage=$PAGE" -O "cache.$filename.$PAGE.list"
			LIST2=`cat "cache.$filename.$PAGE.list" | tr '><' '>\n<' | grep "^img id='i" | tr ' ' '~'`
			for ROUND1 in $LIST2; do
				piccount=`expr $piccount + 1`
				echo round1 $ROUND1
				
				ROUND1=`echo $ROUND1 | tr '~' '\n'`
				ID=`echo $ROUND1 | tr '~' '\n' | grep "id='" | cut -d"'" -f2 | sed 's/i//'`
				echo ID $ID

				PIC=`cat "cache.$filename.$PAGE.list" | tr ' ' '\n' | grep "$ID"| grep ".jp" | cut -d "'" -f2 | tr ' ' '\n' | head -n1`
				echo PIC $PIC
			
#				if [ -z "$PIC" ];then
#					PIC=`echo $ROUND1 | cut -d"'" -f4`
#					echo PIC3 $PIC
#					exit
#				fi

				TITLE=`cat "cache.$filename.$PAGE.list" | tr ' ' '\n' | grep "/watch/$ID" | head -n1 | cut -d"'" -f2 | cut -d"/" -f4 | tr '_' ' '`
				echo TITLE $TITLE
		
				#http://www.myvideo.de/watch/5296613/Unsere_Hochzeit
				URL=http://www.myvideo.de/watch/$ID/`echo $TITLE | tr ' ' '_'`
				echo URL $URL
				LINE="$TITLE#$URL#$PIC#myvideo_$piccount.jpg#MyVideo#12"
				if [ ! -z "$TITLE" ]; then
					echo $LINE >> cache.myvideo.$filename.titanlist
					echo $LINE >> cache.myvideo.titanlist
				fi
			done
		done
	fi

	cat cache.myvideo.$filename.titanlist | sort -u > _full/myvideo/streams/myvideo.$filename.list
	echo piccount $piccount

done

cat cache.myvideo.titanlist | sort -u > _full/myvideo/streams/myvideo.all-sorted.list
cat cache.myvideo.category.titanlist | sort -u > _full/myvideo/myvideo.category.list

for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
	if [ `cat cache.myvideo.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
		cat cache.myvideo.titanlist | grep ^"$ROUND" > cache.myvideo.titanlist."$ROUND"
		cat cache.myvideo.titanlist."$ROUND" | sort -u > _full/myvideo/streams/myvideo.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
		echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/myvideo/streams/myvideo."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#MyVideo#1 >> _full/myvideo/myvideo.a-z.list
	fi
done