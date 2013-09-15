#!/bin/bash
#

buildtype=$1
wgetbin="wget -T2 -t2 --waitretry=2"

rm cache.*
rm _liste
rm -rf _full/myvideo

mkdir -p _full/myvideo/streams
piccount=0

BEGINTIME=`date +%s`
DATENAME=`date +"%Y.%m.%d_%H.%M.%S"`
echo "[myvideo.sh] START (buildtype: $buildtype): $DATENAME" > _full/myvideo/build.log

mainurl=http://www.myvideo.de
if [ "$buildtype" = "full" ];then
	main_list="Top_100 Videos_A-Z Serien Filme Musik"
else
	main_list="Top_100"
fi
	
black_list="Themen webstars tv Playlisten Flight Videos_in_Playlisten Videos_in_Playlisten/Beliebte_Playlisten Alle_Serien_A-Z Videos_in_Kategorien Video_Flight channel/unforgettable Serien/Serien_Suche Serien/WWE Top_100/Top_100_Playlisten Themen/Sexy echo channel/Clipgenerator musik-tv Videos_A-Z/Videos_in_Playlisten Videos_A-Z/Video_Flight"
skip=0

for ROUND1 in $main_list; do
	for black in $black_list; do	
		if [ $ROUND1 == $black ]; then
			skip=1
		fi
	done

	if [ $skip == 0 ];then
		filename=`echo $ROUND1 | tr '/' '.' | tr '-' '.' | tr '_' '.' | tr '%' '.' | tr 'A-Z' 'a-z'`
		filename_submenu1=$filename
		$wgetbin --no-check-certificate "http://myvideo.de/$ROUND1" -O cache.$filename.list
		TITLE=`echo "$ROUND1" | tr '_' ' ' | tr '-' ' '`			
		echo "$TITLE""#http://atemio.dyndns.tv/mediathek/myvideo/myvideo."$filename".list#http://atemio.dyndns.tv/mediathek/menu/"$filename".jpg#"$filename".jpg"#MyVideo#3 >> cache.myvideo.category.titanlist
		submenu_list=`cat cache.$filename.list | tr ' ' '\n' | grep "/$ROUND1/" | grep -v page | cut -d "'" -f2 | cut -d '"' -f2 | grep -v searchChannelID | grep ^/ | sed "s!/$ROUND1/!!" | tr ' ' '\n' | grep -v href= | sort -um`
	fi
	echo submenu_list1 $submenu_list

	for ROUND2 in $submenu_list; do
		for black in $black_list; do
			if [ $black == $ROUND2 ]; then
				skip=1
			fi
		done

		if [ $skip == 0 ];then
			filename=`echo $ROUND2 | tr '/' '.' | tr '-' '.' | tr '_' '.' | tr '%' '.' | tr 'A-Z' 'a-z'`	
			filename_submenu2=$filename
			$wgetbin --no-check-certificate "http://myvideo.de/$ROUND1/$ROUND2" -O cache.$filename.list
			echo ROUND2 $ROUND2
			echo filename $filename

			TITLE=`echo "$ROUND2" | tr '_' ' ' | tr '-' ' '`
				
			if [ -e cache.myvideo.$filename_submenu1.titanlist ] && [ ! `cat cache.myvideo.$filename_submenu1.titanlist | grep menu/$filename.jpg | wc -l` -eq 0 ] && [ ! `cat cache.myvideo.$filename_submenu1.titanlist | grep "$TITLE" | wc -l` -eq 0 ]; then
				skip=1
			fi	
	
			if [ $skip == 0 ];then
				if [ $filename_submenu1 == "serien" ] && [ $ROUND2 != Alle_Serien_A-Z ] ;then
					echo "$TITLE""#http://atemio.dyndns.tv/mediathek/myvideo/myvideo."$filename_submenu1"."$filename".list#http://atemio.dyndns.tv/mediathek/menu/"$filename".jpg#"$filename".jpg"#MyVideo#3 >> cache.myvideo.$filename_submenu1.titanlist
					submenu2_list=`cat cache.$filename.list | tr ";" "\n" | grep /channel | grep .jpg | sed "s/.*longdesc=/pic=/" | grep ^pic  | tr ' ' '\n'| grep href | cut -d"'" -f2`
				else
					echo "$TITLE""#http://atemio.dyndns.tv/mediathek/myvideo/streams/myvideo."$filename".list#http://atemio.dyndns.tv/mediathek/menu/"$filename".jpg#"$filename".jpg"#MyVideo#3 >> cache.myvideo.$filename_submenu1.titanlist
					submenu2_list=`cat cache.$filename.list | tr ' ' '\n' |grep "href='http://www.myvideo.de/channel" | cut -d"'" -f2 | sed 's!http://www.myvideo.de!!'`
				fi
	
				echo submenu2_list $submenu2_list
				echo "$filename#$ROUND1/$ROUND2" >> _liste
			fi			
		fi
		skip=0
		echo submenu2_list $submenu2_list
		if [ ! -z "$submenu2_list" ];then
			for ROUND3 in $submenu2_list; do
				ROUND3PATH=`echo $ROUND3 | cut -d"/" -f2`
				ROUND3=`echo $ROUND3 | cut -d"/" -f3`

				for black in $black_list; do
					if [ $black == $ROUND3 ]; then
						skip=1
					fi
				done

				if [ $skip == 0 ];then
					PIC=`cat cache.$filename_submenu2.list | tr ";" "\n" | grep /channel | grep .jpg | sed "s/.*longdesc=/pic=/" | grep ^pic | grep "/$ROUND3PATH/$ROUND3" | cut -d"'" -f2`
					echo PIC $PIC

					filename=`echo $ROUND3PATH | tr '/' '.' | tr '-' '.' | tr '_' '.' | tr '%' '.' | tr 'A-Z' 'a-z'`.`echo $ROUND3 | tr '/' '.' | tr '-' '.' | tr '_' '.' | tr '%' '.' | tr 'A-Z' 'a-z'`	
					$wgetbin --no-check-certificate "http://myvideo.de/$ROUND3PATH/$ROUND3" -O cache.$filename.list
					TITLE=`echo "$ROUND3" | tr '_' ' ' | tr '-' ' '`			

					if [ -e cache.myvideo.$filename_submenu1.$filename_submenu2.titanlist ] &&  [ ! `cat cache.myvideo.$filename_submenu1.$filename_submenu2.titanlist | grep menu/$filename.jpg | wc -l` -eq 0 ] && [ ! `cat cache.myvideo.$filename_submenu1.$filename_submenu2.titanlist | grep "$TITLE" | wc -l` -eq 0 ]; then
						skip=1
					fi	
		
					if [ $skip == 0 ];then			
						if [ -z $PIC ];then
							echo "$TITLE""#http://atemio.dyndns.tv/mediathek/myvideo/streams/myvideo."$filename".list#http://atemio.dyndns.tv/mediathek/menu/"$filename".jpg#"$filename".jpg"#MyVideo#3 >> cache.myvideo.$filename_submenu1.$filename_submenu2.titanlist
						else
							echo "$TITLE""#http://atemio.dyndns.tv/mediathek/myvideo/streams/myvideo."$filename".list#$PIC#"$filename".jpg"#MyVideo#3 >> cache.myvideo.$filename_submenu1.$filename_submenu2.titanlist
						fi
						echo "$filename#$ROUND3PATH/$ROUND3" >> _liste			
					fi
				fi
				skip=0
			done
		fi

		if [ ! `cat cache.myvideo."$filename_submenu1"."$filename_submenu2".titanlist | wc -l` -eq 0 ];then
			cat cache.myvideo."$filename_submenu1"."$filename_submenu2".titanlist | sort -um > _full/myvideo/myvideo."$filename_submenu1"."$filename_submenu2".list
		fi
	done
	skip=0
	cat cache.myvideo."$filename_submenu1".titanlist | sort -um > _full/myvideo/myvideo."$filename_submenu1".list

done

if [ "$buildtype" = "full" ];then
	echo main_list $main_list
	cat cache.myvideo.category.titanlist | sort -um > _full/myvideo/myvideo.category.list
fi
	
WEBLIST=`cat _liste`
for LIST in $WEBLIST; do
	filename=`echo $LIST | cut -d "#" -f1`
	WEBPATH=`echo $LIST | cut -d "#" -f2`
	echo filename $filename
	echo WEBPATH $WEBPATH
	LIST1=`cat cache.$filename.list | tr '><' '>\n<' | grep "^img id='i" | tr ' ' '~'`
	echo LIST1 $LIST1
	PAGES=`cat cache.$filename.list | tr '><' '>\n<' | grep "page" | grep 'pView pnNumbers' | cut -d ">" -f2 | tr '\n' ' '`
	echo PAGES $PAGES
	echo $PAGES >cache.myvideo.$filename.pages.titanlist
	if [ -z "$PAGES" ];then
		for ROUND1 in $LIST1; do
			piccount=`expr $piccount + 1`
			echo round1 $ROUND1

			ROUND1=`echo $ROUND1 | tr '~' '\n'`
			ID=`echo $ROUND1 | tr '~' '\n' | grep "id='" | cut -d"'" -f2 | sed 's/i//'`
			PIC=`cat "cache.$filename.list" | tr ' ' '\n' | grep "$ID" | grep ".jp" | grep thumbs | cut -d "'" -f2 | tr ' ' '\n' | head -n1`
			TITLE=`cat cache.$filename.list | tr ' ' '\n' | grep "/watch/$ID" | head -n1 | cut -d"'" -f2 | cut -d"/" -f4 | tr '_' ' '`			
			URL="http://www.myvideo.de/dynamic/get_player_video_xml.php?domain=www.myvideo.de&flash_playertype=SER&ds=1&autorun=yes&ID=$ID&_countlimit=4;pageUrl=http://www.myvideo.de/watch/$ID/;playpath=flv:`echo $PIC | tr '/' '\n' | tail -n 4 | head -n1`/`echo $PIC | tr '/' '\n' | tail -n 3 | head -n1`/$ID;$ID"

			if [ -z $PIC ];then
				PIC=`echo $ROUND1 | cut -d"'" -f4`
				if [ -z $PIC ];then
					LINE="$TITLE#$URL#http://atemio.dyndns.tv/mediathek/menu/comedy.jpg#myvideo_$piccount.jpg#MyVideo#12"
				else
					LINE="$TITLE#$URL#$PIC#myvideo_$piccount.jpg#MyVideo#12"
				fi				
			else
				LINE="$TITLE#$URL#$PIC#myvideo_$piccount.jpg#MyVideo#12"
			fi

			if [ ! -z "$TITLE" ]; then
				echo $LINE >> cache.myvideo."$filename".titanlist
				echo $LINE >> cache.myvideo.all.titanlist
			fi
		done
	else
		for PAGE in $PAGES; do
			$wgetbin --no-check-certificate "http://myvideo.de/$WEBPATH?lpage=$PAGE" -O "cache.$filename.$PAGE.list"
			LIST2=`cat "cache.$filename.$PAGE.list" | tr '><' '>\n<' | grep "^img id='i" | tr ' ' '~'`
			for ROUND1 in $LIST2; do
				piccount=`expr $piccount + 1`
				
				ROUND1=`echo $ROUND1 | tr '~' '\n'`
				ID=`echo $ROUND1 | tr '~' '\n' | grep "id='" | cut -d"'" -f2 | sed 's/i//'`
				PIC=`cat "cache.$filename.$PAGE.list" | tr ' ' '\n' | grep "$ID" | grep ".jp" | grep thumbs | cut -d "'" -f2 | tr ' ' '\n' | head -n1`
				TITLE=`cat "cache.$filename.$PAGE.list" | tr ' ' '\n' | grep "/watch/$ID" | head -n1 | cut -d"'" -f2 | cut -d"/" -f4 | tr '_' ' '`
				URL="http://www.myvideo.de/dynamic/get_player_video_xml.php?domain=www.myvideo.de&flash_playertype=SER&ds=1&autorun=yes&ID=$ID&_countlimit=4;pageUrl=http://www.myvideo.de/watch/$ID/;playpath=flv:`echo $PIC | tr '/' '\n' | tail -n 4 | head -n1`/`echo $PIC | tr '/' '\n' | tail -n 3 | head -n1`/$ID;$ID"

				if [ -z $PIC ];then
					PIC=`echo $ROUND1 | cut -d"'" -f4`
					if [ -z $PIC ];then
						LINE="$TITLE#$URL#http://atemio.dyndns.tv/mediathek/menu/comedy.jpg#myvideo_$piccount.jpg#MyVideo#12"
					else
						LINE="$TITLE#$URL#$PIC#myvideo_$piccount.jpg#MyVideo#12"
					fi
				else
					LINE="$TITLE#$URL#$PIC#myvideo_$piccount.jpg#MyVideo#12"
				fi
			
				if [ ! -z "$TITLE" ]; then
					echo $LINE >> cache.myvideo.$filename.titanlist
					echo $LINE >> cache.myvideo.titanlist
				fi
			done
		done
	fi

	cat cache.myvideo.$filename.titanlist | sort -um > _full/myvideo/streams/myvideo.$filename.list
	echo piccount $piccount
done

if [ "$buildtype" = "full" ];then	
	cat cache.myvideo.titanlist | sort -um > _full/myvideo/streams/myvideo.all-sorted.list
	cat cache.myvideo.category.titanlist | sort -um > _full/myvideo/myvideo.category.list
	
	for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
		filename=`echo "$ROUND" | tr 'A-Z' 'a-z'`
		if [ `cat cache.myvideo.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
			cat cache.myvideo.titanlist | grep ^"$ROUND" > cache.myvideo.titanlist."$ROUND"
			cat cache.myvideo.titanlist."$ROUND" | sort -um > _full/myvideo/streams/myvideo.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
			echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/myvideo/streams/myvideo."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#MyVideo#3 >> _full/myvideo/myvideo.a-z.list
		elif [ `cat cache.myvideo.titanlist | grep ^"$filename" | wc -l` -gt 0 ];then
			cat cache.myvideo.titanlist | grep ^"$filename" > cache.myvideo.titanlist."$ROUND"
			cat cache.myvideo.titanlist."$ROUND" | sort -um > _full/myvideo/streams/myvideo.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
			echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/myvideo/streams/myvideo."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#MyVideo#3 >> _full/myvideo/myvideo.a-z.list
		fi
	done
fi

# check category
input_list=`ls -1 _full/myvideo | grep -v streams | grep -v myvideo.a-z.list | grep -v myvideo.category.list`

for ROUND0 in $input_list; do
echo round $ROUND0
	CHECK=`cat _full/myvideo/$ROUND0 | grep "/streams/" | wc -l`
	TYPE=myvideo
	if [ "$CHECK" -gt 0 ];then
		TYPE="myvideo/streams"
	fi

	category_file=`cat _full/myvideo/$ROUND0 | cut -d "#" -f2 | sed "s!http://atemio.dyndns.tv/mediathek/$TYPE/!!"`
	category_files=`ls -1 _full/$TYPE`
	
	count=0
	for ROUND1 in $category_file; do
		for ROUND2 in $category_files; do
			if [ "$ROUND1" == "$ROUND2" ]; then
				LINE=`cat _full/myvideo/$ROUND0 | grep "$ROUND1"`
				if [ ! -z "$LINE" ];then
					count=`expr $count + 1`
					echo "$LINE" >> _full/myvideo/$ROUND0.filter
					echo "add ($count) $ROUND1 $ROUND2"
				fi
			fi
		done
	done

	in=`cat _full/myvideo/$ROUND0 | wc -l`
	out=`cat _full/myvideo/$ROUND0.filter | wc -l`
	
	echo "$ROUND0             $in"
	echo "$ROUND0.filter $out"
		
	if [ "$in" -gt "$out" ];then
		cat _full/myvideo/$ROUND0.filter | sort -um > _full/myvideo/$ROUND0.sorted.list
		mv -f _full/myvideo/$ROUND0 _full/myvideo/$ROUND0.searchlist
		mv -f _full/myvideo/$ROUND0.sorted.list _full/myvideo/$ROUND0
		echo ---------------------- change ----------------------------- $ROUND0
	fi
done

DONETIME=`date +%s`
TIME=`expr $DONETIME - $BEGINTIME`
echo "[myvideo.sh] build time: ($TIME s) done" >> _full/myvideo/build.log	
echo "[myvideo.sh] myvideo: "`ls -1 _full/myvideo` >> _full/myvideo/build.log
echo "[myvideo.sh] myvideo/streams: "`ls -1 _full/myvideo/streams` >> _full/myvideo/build.log

if [ "$buildtype" != "full" ];then
	cp -a _full/myvideo/* /var/www/atemio/web/mediathek/myvideo
fi

rm cache.*

exit