#!/bin/bash
#

#cat movies-updates.html | grep coverPreview | sed 's!coverPreview!"!' | cut -d'"' -f3
#movies-genre-4-122
#cat movies-genre-4-Adventure.html | grep boxgrey | tr '><' '\n' | grep .html |wc -l

rm cache.*
rm _liste
rm -rf _full/movie4k
mkdir -p _full/movie4k/streams
rm cache.*
touch cache.movie4k.titanlist

#watchlist="
#movies-updates.html
#genres-movies.html
#"

watchlist="
movies-all.html
"

for ROUND in A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
	watchlist="$watchlist movies-all-$ROUND.html" 
done

echo watchlist $watchlist

testcount=0
piccount=0
count=0


#for ROUND1 in $watchlist; do
#	count=`expr $count + 1`
#
#	filename1="$ROUND1"
#	wget "http://www.movie4k.to/$ROUND1" -O cache."$count"."$filename1"
#	genrelist=`cat cache."$count"."$filename1" | grep tdmovies | grep .html | cut -d '"' -f6`
#	for ROUND2 in $genrelist; do
	for ROUND2 in $watchlist; do
#echo 111111111	
		count=`expr $count + 1`	
		filename2="$ROUND2"
		wget "http://www.movie4k.to/$ROUND2" -O cache."$count"."$filename2"
#cat cache.1.movies-all.html | grep boxgrey | grep movies-all-1-2.html  | tr '><' '\n' | grep .html | wc -l	
		if [ `cat cache."$count"."$filename2" | grep boxgrey | tr '><' '\n' | grep .html | grep -v './'  | cut -d'"' -f2 | wc -l` -gt 0 ];then
			pagelist="$ROUND2 `cat cache.$count.$filename2 | grep boxgrey | tr '><' '\n' | grep .html | grep -v './'  | cut -d'"' -f2`"	
		else
			pagelist="$ROUND2"
		fi

#		echo pagelist $pagelist
#			exit	

		for ROUND3 in $pagelist; do
#echo 222222222
			filename3="$ROUND3"
			wget "http://www.movie4k.to/$ROUND3" -O cache."$count"."$filename3"

#exit	
		#	id_list=`cat cache."$count"."$filename3" | grep coverPreview | sed 's!coverPreview!"!' | cut -d'"' -f3`
			id_list=`cat cache."$count"."$filename3" | grep '<a href="' | cut -d'"' -f2 | sort -u | grep .html`


					
			piccount=`expr $piccount + 1`
#			URL="http://atemio.dyndns.tv/mediathek/movie4k/streams/movie4k.$filename2.list"
#			TITLE=`echo $ROUND2 | sed 's/.html//' | tr '-' '\n' | tail -n1`
#			PIC="http://atemio.dyndns.tv/mediathek/menu/`echo "$TITLE" | tr 'A-Z' 'a-z'`.jpg"
#
#			echo round3 $ROUND3
#			if [ `echo $ROUND3 | grep "movies-all" | wc -l` -eq 1 ];then
#				ZUSATZ="Filme "
#			else
#				ZUSATZ="Serie "
#			fi						


#			LINE="$ZUSATZ$TITLE#$URL#$PIC#movie4k_$piccount.jpg#Movie4k#3"
#			if [ ! -z "$TITLE" ] && [ `cat cache.movie4k.category.titanlist | grep "^$TITLE" | wc -l` -eq 0 ];then
#				echo $LINE >> cache.movie4k.category.titanlist	
#			fi
#echo $LINE
#exit					
#			echo id_list $id_list
			for ROUND4 in $id_list; do
#echo 333333333
#				testcount=`expr $testcount + 1`
#				if [ $testcount = 2 ]; then
#					testcount=0
#					break
#				fi

				piccount=`expr $piccount + 1`
				STR=`echo $ROUND4 | sed "s/online-film.*//" | sed "s/watch-movie.*//"`
				if [ `echo $STR | grep "^-" | wc -l` -eq 1 ];then
					STR=\\$STR
				fi
#echo 444444444
#				echo ROUND4 $ROUND4
#				ls cache."$count"."$filename3"
#				echo STR $STR

#echo 444444444aaaaaa

				PIC=`cat cache."$count"."$filename3" | grep "$STR" | grep coverPreview | cut -d "'" -f4 | head -n1`
#echo 444444444bbbbbbb

				ID=`echo $ROUND4 | sed "s/.*online-film-//" | sed "s/.*watch-movie-//" | sed "s/.html.*//"`
				ZEILE=`cat cache."$count"."$filename3" | grep -n $ID | head -n1 | cut -d ":" -f1`
				ALLES=`cat cache."$count"."$filename3" | wc -l`
				CUT=`expr $ALLES - $ZEILE`
#echo 5555555555
				TMPLANG=`cat cache."$count"."$filename3" | tail -n $CUT | grep small.png | head -n1`
#				echo PIC $PIC
#				echo ID $ID
#				echo ZEILE $ZEILE
#				echo ALLES $ALLES
#				echo CUT $CUT
#				echo TMPLANG $TMPLANG
#echo 5555555555aaaaaaaaaa

				if [ -z "$PIC" ]; then
					lostcount=`expr $lostcount + 1`
					PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
#					exit
				fi
#echo 666666666				
				TITLE=`echo $ROUND4 | sed "s!-online-film-!;!" | sed "s!-watch-movie-!;!" | tr ";" "\n" | head -n1 | tr '-' ' ' | sed 's/^ //'`
				URL=http://www.movie4k.to/"$ROUND4"
#				echo URL $URL
#				echo PIC $PIC
#				echo TITLE $TITLE
				lang=35
				if [ `echo $TMPLANG | grep "us_ger_small.png" | wc -l` -eq 1 ];then
					lang=34
					LANGTXT=" (de)"
				elif [ `echo $TMPLANG | grep "us_flag_small.png" | wc -l` -eq 1 ];then
					lang=35
					LANGTXT=" (en)"
				else
					LANGTXT=" (??)"
				fi
#				echo lang $lang
#				echo title $TITLE
#				ls cache.movie4k."$filename2".titanlist
				
#				if [ ! -z "$TITLE" ] && [ ! -z "$URL" ] && [ `cat cache.movie4k.$filename2.titanlist | grep "^$TITLE$LANGTXT" | wc -l` -eq 0 ];then
				if [ ! -z "$TITLE" ] && [ ! -z "$URL" ] && [ `cat cache.movie4k.titanlist | grep "^$TITLE$LANGTXT" | wc -l` -eq 0 ];then
					LINE="$TITLE$LANGTXT#$URL#$PIC#movie4k_$piccount.jpg#Movie4k#$lang"
#					echo $LINE >> cache.movie4k."$filename2".titanlist	
			
#					if [ `cat cache.movie4k.titanlist | grep "^$TITLE$LANGTXT" | wc -l` -eq 0 ];then
						echo $LINE >> cache.movie4k.titanlist
#					fi
				fi
#echo 77777777777
			done
#			cat cache.movie4k."$filename2".titanlist | sort -u > _full/movie4k/streams/movie4k.$filename2.list
		done
	done
#done

#rm -f /home/obiwan/test/_full/movie4k/streams/*
#cp backupcache.movie4k.titanlist cache.movie4k.titanlist

watchlist="
genres-movies.html
"

for ROUND1 in $watchlist; do
	count=`expr $count + 1`

	filename1="$ROUND1"
	wget "http://www.movie4k.to/$ROUND1" -O cache."$count"."$filename1"
	genrelist=`cat cache."$count"."$filename1" | grep tdmovies | grep .html | cut -d '"' -f6`
	for ROUND2 in $genrelist; do
		count=`expr $count + 1`	
		filename2="$ROUND2"
		wget "http://www.movie4k.to/$ROUND2" -O cache."$count"."$filename2"
		if [ `cat cache."$count"."$filename2" | grep boxgrey | tr '><' '\n' | grep .html | grep -v './'  | cut -d'"' -f2 | wc -l` -gt 0 ];then
			pagelist="$ROUND2 `cat cache.$count.$filename2 | grep boxgrey | tr '><' '\n' | grep .html | grep -v './'  | cut -d'"' -f2`"	
		else
			pagelist="$ROUND2"
		fi
		ck=0
id_list=""
		for ROUND3 in $pagelist; do
#				ck=`expr $ck + 1`
#				if [ $ck = 10 ]; then
#					ck=0
#					break
#				fi
			filename3="$ROUND3"
			wget "http://www.movie4k.to/$ROUND3" -O cache."$count"."$filename3"

			id_list="$id_list "`cat cache."$count"."$filename3" | grep coverPreview | sed 's!coverPreview!"!' | cut -d'"' -f3 | tr ' ' '\n' | grep -v ").append" | sort -u`
		done


		TITLE=`echo $ROUND2 | sed 's/.html//' | tr '-' '\n' | tail -n1`

echo $id_list > _full/movie4k/streams/_$TITLE
ck=0
		for ROUND4 in $id_list; do
#				ck=`expr $ck + 1`
#				if [ $ck = 2 ]; then
#					ck=0
#					break
#				fi
			filename4="$ROUND4"
			LINE=`cat cache.movie4k.titanlist | grep $ROUND4`
			if [ ! -z "$LINE" ];then
				echo $LINE >> cache.movie4k.`echo "$TITLE" | tr 'A-Z' 'a-z'`.titanlist	
			fi
		done

piccount=`expr $piccount + 1`

		URL="http://atemio.dyndns.tv/mediathek/movie4k/streams/movie4k.`echo "$TITLE" | tr 'A-Z' 'a-z'`.list"
		PIC="http://atemio.dyndns.tv/mediathek/menu/`echo "$TITLE" | tr 'A-Z' 'a-z'`.jpg"

		if [ `echo $ROUND2 | grep "movies" | wc -l` -eq 1 ];then
			ZUSATZ="Filme "
		else
			ZUSATZ="Serie "
		fi
		LINE="$ZUSATZ$TITLE#$URL#$PIC#movie4k_$piccount.jpg#Movie4k#3"
		if [ ! -z "$TITLE" ];then
			echo $LINE >> cache.movie4k.category.titanlist	
		fi
		
		cat cache.movie4k.`echo "$TITLE" | tr 'A-Z' 'a-z'`.titanlist  | sort -u > _full/movie4k/streams/movie4k.`echo "$TITLE" | tr 'A-Z' 'a-z'`.list
	done
done

cat cache.movie4k.titanlist | sort -u > _full/movie4k/streams/movie4k.all-sorted.list
#cat cache.movie4k.category.titanlist | sort -u > _full/movie4k/movie4k.category.list
cat cache.movie4k.category.titanlist > _full/movie4k/movie4k.category.list

for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
	filename=`echo "$ROUND" | tr 'A-Z' 'a-z'`
	if [ `cat cache.movie4k.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
		cat cache.movie4k.titanlist | grep ^"$ROUND" > cache.movie4k.titanlist."$ROUND"
		cat cache.movie4k.titanlist."$ROUND" | sort -um > _full/movie4k/streams/movie4k.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
		echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/movie4k/streams/movie4k."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#Movie4k#3 >> _full/movie4k/movie4k.a-z.list
	elif [ `cat cache.movie4k.titanlist | grep ^"$filename" | wc -l` -gt 0 ];then
		cat cache.movie4k.titanlist | grep ^"$ROUND" > cache.movie4k.titanlist."$ROUND"
		cat cache.movie4k.titanlist."$ROUND" | sort -um > _full/movie4k/streams/movie4k.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
		echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/movie4k/streams/movie4k."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#Movie4k#3 >> _full/movie4k/movie4k.a-z.list
	fi
done

#echo lostcount $lostcount

#cp -a _full/movie4k/* /var/www/atemio/web/mediathek/movie4k
#cp -a mainmenu.list /var/www/atemio/web/mediathek
#cp -a mainmenu-movie4k.list /var/www/atemio/web/mediathek

rm file.*
rm cache.*
