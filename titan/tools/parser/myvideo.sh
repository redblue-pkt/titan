#!/bin/bash
#

rm cache.*
rm _liste
rm -rf _full/myvideo
mkdir -p _full/myvideo/streams
piccount=0

mainurl=http://www.myvideo.de
mainliste="Top_100 Videos_A-Z Serien Filme Musik"

black_list="Themen webstars tv Playlisten Flight"
wget --no-check-certificate "http://myvideo.de" -O cache.main.list
main_list=`cat cache.main.list | tr ':' '\n' | grep return | grep topgrey | tr ' ' '\n' | grep href | cut -d"'" -f2 | grep -v ^# | sed 's!/!!' | sort -u`
skip=0

echo main_list $main_list
for ROUND1 in $main_list; do
	for black in $black_list; do	
		if [ $ROUND1 == $black ]; then
			skip=1
		fi
	done

	if [ $skip == 0 ];then
		filename=`echo $ROUND1 | tr '/' '.' | tr '-' '.' | tr '_' '.' | tr '%' '.' | tr 'A-Z' 'a-z'`
		filename_submenu1=$filename
		wget --no-check-certificate "http://myvideo.de/$ROUND1" -O cache.$filename.list
		echo ROUND1 $ROUND1
		TITLE=`echo "$ROUND1" | tr '_' ' ' | tr '-' ' '`			
		echo "$TITLE""#http://atemio.dyndns.tv/mediathek/myvideo/myvideo."$filename".list#http://atemio.dyndns.tv/mediathek/menu/"$filename".jpg#"$filename".jpg"#MyVideo#1 >> cache.myvideo.category.titanlist
		submenu_list=`cat cache.$filename.list | tr ' ' '\n' | grep "/$ROUND1/" | grep -v page |cut -d "'" -f2 | sed "s!/$ROUND1/!!" | sort -u`
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
			wget --no-check-certificate "http://myvideo.de/$ROUND1/$ROUND2" -O cache.$filename.list
			echo ROUND2 $ROUND2
			echo filename $filename

			TITLE=`echo "$ROUND2" | tr '_' ' ' | tr '-' ' '`
			if [ $filename_submenu1 == "serien" ] && [ $ROUND2 != Alle_Serien_A-Z ] ;then
				echo "$TITLE""#http://atemio.dyndns.tv/mediathek/myvideo/myvideo."$filename_submenu1"."$filename".list#http://atemio.dyndns.tv/mediathek/menu/"$filename".jpg#"$filename".jpg"#MyVideo#1 >> cache.myvideo.$filename_submenu1.titanlist
				submenu2_list=`cat cache.$filename.list | tr ";" "\n" | grep /channel | grep .jpg | sed "s/.*longdesc=/pic=/" | grep ^pic  | tr ' ' '\n'| grep href | cut -d"'" -f2`
			else
#				echo "$TITLE""#http://atemio.dyndns.tv/mediathek/myvideo/streams/myvideo."$filename_submenu1"."$filename".list#http://atemio.dyndns.tv/mediathek/menu/"$filename".jpg#"$filename".jpg"#MyVideo#1 >> cache.myvideo.$filename_submenu1.titanlist
				echo "$TITLE""#http://atemio.dyndns.tv/mediathek/myvideo/streams/myvideo."$filename".list#http://atemio.dyndns.tv/mediathek/menu/"$filename".jpg#"$filename".jpg"#MyVideo#1 >> cache.myvideo.$filename_submenu1.titanlist
				submenu2_list=`cat cache.$filename.list | tr ' ' '\n' |grep "href='http://www.myvideo.de/channel" | cut -d"'" -f2 | sed 's!http://www.myvideo.de!!'`
			fi

			echo submenu2_list $submenu2_list
#			echo "cache.$filename.list#http://myvideo.de/$ROUND1/$ROUND2" >> _liste
			echo "$filename#$ROUND1/$ROUND2" >> _liste
			
		fi
		skip=0
		echo submenu2_list $submenu2_list
		if [ ! -z "$submenu2_list" ];then
			for ROUND3 in $submenu2_list; do
				echo ROUND3 $ROUND3
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
					wget --no-check-certificate "http://myvideo.de/$ROUND3PATH/$ROUND3" -O cache.$filename.list
					echo ROUND1 $ROUND1
					echo ROUND2 $ROUND2
					echo ROUND3 $ROUND3										
					echo ROUND3PATH $ROUND3PATH										

					echo filename $filename
					echo filename_submenu1 $filename_submenu1
					echo filename_submenu2 $filename_submenu2
										
					TITLE=`echo "$ROUND3" | tr '_' ' ' | tr '-' ' '`			
					echo TITLE $TITLE

					if [ -z $PIC ];then
						echo "$TITLE""#http://atemio.dyndns.tv/mediathek/myvideo/streams/myvideo."$filename".list#http://atemio.dyndns.tv/mediathek/menu/"$filename".jpg#"$filename".jpg"#MyVideo#1 >> cache.myvideo.$filename_submenu1.$filename_submenu2.titanlist
					else
						echo "$TITLE""#http://atemio.dyndns.tv/mediathek/myvideo/streams/myvideo."$filename".list#$PIC#"$filename".jpg"#MyVideo#1 >> cache.myvideo.$filename_submenu1.$filename_submenu2.titanlist
					fi

#					echo "cache.$filename.list#http://myvideo.de/$ROUND3PATH/$ROUND3" >> _liste
					echo "$filename#$ROUND3PATH/$ROUND3" >> _liste			
				fi
				skip=0
			done
		fi

		if [ ! `cat cache.myvideo."$filename_submenu1"."$filename_submenu2".titanlist | wc -l` -eq 0 ];then
			cat cache.myvideo."$filename_submenu1"."$filename_submenu2".titanlist | sort -u > _full/myvideo/myvideo."$filename_submenu1"."$filename_submenu2".list
		fi
	done
	skip=0
	cat cache.myvideo."$filename_submenu1".titanlist | sort -u > _full/myvideo/myvideo."$filename_submenu1".list

done
echo main_list $main_list
cat cache.myvideo.category.titanlist | sort -u > _full/myvideo/myvideo.category.list


#WEBPATH Themen/Talente
#round Lifestyle#Themen/Lifestyle
#TITLE Lifestyle	
	
WEBLIST=`cat _liste`
echo weblist $WEBLIST
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
			echo ID $ID

#			PIC=`cat "cache.$filename.list" | tr ' ' '\n' | grep "$ID"| grep ".jp" | grep thumbs | cut -d "'" -f2 | tr ' ' '\n' | head -n1`
			PIC=`cat "cache.$filename.list" | tr ' ' '\n' | grep "$ID" | grep ".jp" | grep thumbs | cut -d "'" -f2 | tr ' ' '\n' | head -n1`
			echo PIC $PIC

ls cache.$filename.list
			TITLE=`cat cache.$filename.list | tr ' ' '\n' | grep "/watch/$ID" | head -n1 | cut -d"'" -f2 | cut -d"/" -f4 | tr '_' ' '`			
			echo TITLE $TITLE
	
#			URL=http://www.myvideo.de/watch/$ID/`echo $TITLE | tr ' ' '_'`

#echo $PIC | tr '/' '\n' | tail -n 4 | head -n1`"/"`echo $PIC | tr '/' '\n' | tail -n 3 | head -n1`"
			URL="http://www.myvideo.de/dynamic/get_player_video_xml.php?flash_playertype=SER&ID=$ID&_countlimit=4&autorun=yes;pageUrl=http://www.myvideo.de/watch/$ID/;playpath=flv:`echo $PIC | tr '/' '\n' | tail -n 4 | head -n1`/`echo $PIC | tr '/' '\n' | tail -n 3 | head -n1`/$ID"
			
			echo URL $URL
			if [ -z $PIC ];then
				PIC=`echo $ROUND1 | cut -d"'" -f4`
				if [ -z $PIC ];then
					LINE="$TITLE#$URL#http://atemio.dyndns.tv/mediathek/menu/comedy.jpg#myvideo_$piccount.jpg#MyVideo#12"
#					exit
				else
					LINE="$TITLE#$URL#$PIC#myvideo_$piccount.jpg#MyVideo#12"
				fi				
			else
				LINE="$TITLE#$URL#$PIC#myvideo_$piccount.jpg#MyVideo#12"
			fi
			echo LINE $LINE
#			exit
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

#				PIC=`cat "cache.$filename.$PAGE.list" | tr ' ' '\n' | grep "$ID"| grep ".jp" | cut -d "'" -f2 | tr ' ' '\n' | head -n1`
				PIC=`cat "cache.$filename.$PAGE.list" | tr ' ' '\n' | grep "$ID" | grep ".jp" | grep thumbs | cut -d "'" -f2 | tr ' ' '\n' | head -n1`
				echo PIC $PIC

ls cache.$filename.list			
#				if [ -z "$PIC" ];then
#					PIC=`echo $ROUND1 | cut -d"'" -f4`
#					echo PIC3 $PIC
#					exit
#				fi

				TITLE=`cat "cache.$filename.$PAGE.list" | tr ' ' '\n' | grep "/watch/$ID" | head -n1 | cut -d"'" -f2 | cut -d"/" -f4 | tr '_' ' '`
				echo TITLE $TITLE
		
				#http://www.myvideo.de/watch/5296613/Unsere_Hochzeit
#				URL=http://www.myvideo.de/watch/$ID/`echo $TITLE | tr ' ' '_'`
				URL="http://www.myvideo.de/dynamic/get_player_video_xml.php?flash_playertype=SER&ID=$ID&_countlimit=4&autorun=yes;pageUrl=http://www.myvideo.de/watch/$ID/;playpath=flv:`echo $PIC | tr '/' '\n' | tail -n 4 | head -n1`/`echo $PIC | tr '/' '\n' | tail -n 3 | head -n1`/$ID"				
				echo URL $URL

				if [ -z $PIC ];then
					PIC=`echo $ROUND1 | cut -d"'" -f4`
					if [ -z $PIC ];then
						LINE="$TITLE#$URL#http://atemio.dyndns.tv/mediathek/menu/comedy.jpg#myvideo_$piccount.jpg#MyVideo#12"
#						exit
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

exit
videoaz_list=`cat Videos_A-Z | tr ' ' '\n' | grep "/Videos_A-Z/" | grep -v Playlisten | grep -v Flight | cut -d "'" -f2 | sed 's!/Videos_A-Z/!!'`
kategorie_list="`cat Videos_in_Kategorien | tr ' ' '\n' | grep "href='http://feeds.myvideo.de/feeds/" | cut -d "'" -f2 | sed 's!http://feeds.myvideo.de/feeds/Kategorie-Videos-!!' | sed 's!-neu.rss!!' | sed 's!.rss!!'`

http://www.myvideo.de/Videos_A-Z?searchChannelID=17&searchChannel=Urlaub+%26+Reisen&searchOrder=5
http://www.myvideo.de/Videos_A-Z?searchChannelID=17&searchChannel=Urlaub-und-Reisen&searchOrder=5

Videos_A-Z?searchChannelID=1&searchChannel=Animation&searchOrder=5
Videos_A-Z?searchChannelID=2&searchChannel=Autos+%26+Verkehr&searchOrder=5

Themen/Sexy

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

#			PIC=`cat "cache.$filename.list" | tr ' ' '\n' | grep "$ID"| grep ".jp" | grep thumbs | cut -d "'" -f2 | tr ' ' '\n' | head -n1`
			PIC=`cat "cache.$filename.list" | tr ' ' '\n' | grep "$ID" | grep ".jp" | grep thumbs | cut -d "'" -f2 | tr ' ' '\n' | head -n1`
			echo PIC $PIC

ls cache.$filename.list
			TITLE=`cat cache.$filename.list | tr ' ' '\n' | grep "/watch/$ID" | head -n1 | cut -d"'" -f2 | cut -d"/" -f4 | tr '_' ' '`			
			echo TITLE $TITLE
	
#			URL=http://www.myvideo.de/watch/$ID/`echo $TITLE | tr ' ' '_'`

#echo $PIC | tr '/' '\n' | tail -n 4 | head -n1`"/"`echo $PIC | tr '/' '\n' | tail -n 3 | head -n1`"
			URL="http://www.myvideo.de/dynamic/get_player_video_xml.php?flash_playertype=SER&ID=$ID&_countlimit=4&autorun=yes;pageUrl=http://www.myvideo.de/watch/$ID/;playpath=flv:`echo $PIC | tr '/' '\n' | tail -n 4 | head -n1`/`echo $PIC | tr '/' '\n' | tail -n 3 | head -n1`/$ID"
			
			echo URL $URL
			LINE="$TITLE#$URL#$PIC#myvideo_$piccount.jpg#MyVideo#12"
			echo LINE $LINE
#			exit
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

#				PIC=`cat "cache.$filename.$PAGE.list" | tr ' ' '\n' | grep "$ID"| grep ".jp" | cut -d "'" -f2 | tr ' ' '\n' | head -n1`
				PIC=`cat "cache.$filename.$PAGE.list" | tr ' ' '\n' | grep "$ID" | grep ".jp" | grep thumbs | cut -d "'" -f2 | tr ' ' '\n' | head -n1`
				echo PIC $PIC

			
#				if [ -z "$PIC" ];then
#					PIC=`echo $ROUND1 | cut -d"'" -f4`
#					echo PIC3 $PIC
#					exit
#				fi

				TITLE=`cat "cache.$filename.$PAGE.list" | tr ' ' '\n' | grep "/watch/$ID" | head -n1 | cut -d"'" -f2 | cut -d"/" -f4 | tr '_' ' '`
				echo TITLE $TITLE
		
				#http://www.myvideo.de/watch/5296613/Unsere_Hochzeit
#				URL=http://www.myvideo.de/watch/$ID/`echo $TITLE | tr ' ' '_'`
				URL="http://www.myvideo.de/dynamic/get_player_video_xml.php?flash_playertype=SER&ID=$ID&_countlimit=4&autorun=yes;pageUrl=http://www.myvideo.de/watch/$ID/;playpath=flv:`echo $PIC | tr '/' '\n' | tail -n 4 | head -n1`/`echo $PIC | tr '/' '\n' | tail -n 3 | head -n1`/$ID"				
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