#!/bin/bash
#

rm cache.*
rm _liste
rm -rf _full/movie4k
mkdir -p _full/movie4k/streams
rm cache.*
touch cache.movie4k.titanlist

# filme a-z
watchlist="
movies-all.html
"

for ROUND in A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
	watchlist="$watchlist movies-all-$ROUND.html" 
done

testcount=0
piccount=0
count=0

for ROUND2 in $watchlist; do
	count=`expr $count + 1`	
	filename2="$ROUND2"
	wget "http://www.movie4k.to/$ROUND2" -O cache."$count"."$filename2"
	if [ `cat cache."$count"."$filename2" | grep boxgrey | tr '><' '\n' | grep .html | grep -v './'  | cut -d'"' -f2 | wc -l` -gt 0 ];then
		pagelist="$ROUND2 `cat cache.$count.$filename2 | grep boxgrey | tr '><' '\n' | grep .html | grep -v './'  | cut -d'"' -f2`"	
	else
		pagelist="$ROUND2"
	fi

	for ROUND3 in $pagelist; do
		filename3="$ROUND3"
		wget "http://www.movie4k.to/$ROUND3" -O cache."$count"."$filename3"

		id_list=`cat cache."$count"."$filename3" | grep '<a href="' | cut -d'"' -f2 | sort -u | grep .html`

		piccount=`expr $piccount + 1`
		for ROUND4 in $id_list; do
			piccount=`expr $piccount + 1`
			STR=`echo $ROUND4 | sed "s/online-film.*//" | sed "s/watch-movie.*//"`
			if [ `echo $STR | grep "^-" | wc -l` -eq 1 ];then
				STR=\\$STR
			fi

			PIC=`cat cache."$count"."$filename3" | grep "$STR" | grep coverPreview | cut -d "'" -f4 | head -n1`
			ID=`echo $ROUND4 | sed "s/.*online-film-//" | sed "s/.*watch-movie-//" | sed "s/.html.*//"`
			ZEILE=`cat cache."$count"."$filename3" | grep -n $ID | head -n1 | cut -d ":" -f1`
			ALLES=`cat cache."$count"."$filename3" | wc -l`
			CUT=`expr $ALLES - $ZEILE`
			TMPLANG=`cat cache."$count"."$filename3" | tail -n $CUT | grep small.png | head -n1`

			if [ -z "$PIC" ]; then
				lostcount=`expr $lostcount + 1`
				PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
			fi
			TITLE=`echo $ROUND4 | sed "s!-online-film-!;!" | sed "s!-watch-movie-!;!" | tr ";" "\n" | head -n1 | tr '-' ' ' | sed 's/^ //'`
			URL=http://www.movie4k.to/"$ROUND4"

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
			
			if [ ! -z "$TITLE" ] && [ ! -z "$URL" ] && [ `cat cache.movie4k.titanlist | grep "^$TITLE$LANGTXT" | wc -l` -eq 0 ];then
				LINE="$TITLE$LANGTXT#$URL#$PIC#movie4k_$piccount.jpg#Movie4k#$lang"			
				echo $LINE >> cache.movie4k.titanlist
				echo $LINE >> cache.movie4k.movies.titanlist
			fi
		done
	done
done

# kinofilme de/eng

watchlist="
index.php?lang=de
index.php
"

for ROUND2 in $watchlist; do
	piccount=`expr $piccount + 1`
	count=`expr $count + 1`	
	filename2="$ROUND2"
	wget "http://www.movie4k.to/$ROUND2" -O cache."$count"."$filename2"
	if [ "$ROUND2" = "index.php?lang=de" ];then
		TYPE="newest.ger"
		TYPENAME="Kino Filme (de)"
	elif [ "$ROUND2" = "index.php" ];then
		TYPE="newest.eng"
		TYPENAME="Kino Filme (en)"
	fi

	pagelist=`cat cache."$count"."$filename2" | grep "float:left" | cut -d '"' -f4`
	for ROUND3 in $pagelist; do
		piccount=`expr $piccount + 1`
		filename3="$ROUND3"
		PIC=`cat cache."$count"."$filename2" | grep $ROUND3 | grep "img src" | tail -n1 | cut -d '"' -f6`
		ID=`echo $ROUND3 | sed "s/.*online-film-//" | sed "s/.*watch-movie-//" | sed "s/.html.*//"`
		ZEILE=`cat cache."$count"."$filename2" | grep -n $ID | head -n1 | cut -d ":" -f1`
		ALLES=`cat cache."$count"."$filename2" | wc -l`
		CUT=`expr $ALLES - $ZEILE`
		TMPLANG=`cat cache."$count"."$filename2" | tail -n $CUT | grep small.png | head -n1`

		if [ -z "$PIC" ]; then
			lostcount=`expr $lostcount + 1`
			PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
		fi
		TITLE=`echo $ROUND3 | sed "s!-online-film-!;!" | sed "s!-watch-movie-!;!" | tr ";" "\n" | head -n1 | tr '-' ' ' | sed 's/^ //'`
		URL=http://www.movie4k.to/"$ROUND3"

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

# show allways
		lang=34
		
		if [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
			LINE="$TITLE$LANGTXT#$URL#$PIC#movie4k_$piccount.jpg#Movie4k#$lang"			
			if [ `cat cache.movie4k.$TYPE.titanlist | grep "^$TITLE$LANGTXT" | wc -l` -eq 0 ];then
				echo $LINE >> cache.movie4k.$TYPE.titanlist
			fi
#			if [ `cat cache.movie4k.titanlist | grep "^$TITLE$LANGTXT" | wc -l` -eq 0 ];then
#				echo $LINE >> cache.movie4k.titanlist
#			fi
		fi
	done

	URL="http://atemio.dyndns.tv/mediathek/movie4k/streams/movie4k.$TYPE.list"
	PIC="http://atemio.dyndns.tv/mediathek/menu/$TYPE.jpg"
	
	LINE="$TYPENAME#$URL#$PIC#movie4k_$piccount.jpg#Movie4k#3"
	if [ ! -z "$TYPENAME" ];then
		echo $LINE >> cache.movie4k.category.titanlist	
	fi
			
	cat cache.movie4k.$TYPE.titanlist > _full/movie4k/streams/movie4k.$TYPE.list	
done

#serien a-z

watchlist="
tvshows-all.html
"

for ROUND in A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
	watchlist="$watchlist tvshows-all-$ROUND.html" 
done

testcount=0
piccount=0
count=0

for ROUND2 in $watchlist; do
	count=`expr $count + 1`	
	filename2="$ROUND2"
	wget "http://www.movie4k.to/$ROUND2" -O cache."$count"."$filename2"
	if [ `cat cache."$count"."$filename2" | grep boxgrey | tr '><' '\n' | grep .html | cut -d'"' -f2 | wc -l` -gt 0 ];then
		pagelist="$ROUND2 `cat cache.$count.$filename2 | grep boxgrey | tr '><' '\n' | grep .html | cut -d'"' -f2 | sed "s!http://www.movie4k.to//!!"`"	
	else
		pagelist="$ROUND2"
	fi

	for ROUND3 in $pagelist; do
		count=`expr $count + 1`
		filename3="$ROUND3"
#		echo ROUND3 $ROUND3
#		sleep 1
		wget "http://www.movie4k.to/$ROUND3" -O cache."$count"."$filename3"

		id_list=`cat cache."$count"."$filename3" | grep tvshows-season- | cut -d '"' -f6 | sort -u | grep .html`
#		echo id_list $id_list
		
		piccount=`expr $piccount + 1`
		for ROUND4 in $id_list; do
			piccount=`expr $piccount + 1`
			STR=`echo $ROUND4 | sed "s/tvshows-season.*//" | sed "s/watch-movie.*//"`
			if [ `echo $STR | grep "^-" | wc -l` -eq 1 ];then
				STR=\\$STR
			fi

			PIC=`cat cache."$count"."$filename3" | grep "$STR" | grep coverPreview | cut -d "'" -f4 | head -n1`
			ID=`echo $ROUND4 | sed "s/.*tvshows-season-//" | sed "s/.*watch-movie-//" | sed "s/.html.*//"`
			ZEILE=`cat cache."$count"."$filename3" | grep -n $ID | head -n1 | cut -d ":" -f1`
			ALLES=`cat cache."$count"."$filename3" | wc -l`
			CUT=`expr $ALLES - $ZEILE`
			TMPLANG=`cat cache."$count"."$filename3" | tail -n $CUT | grep small.png | head -n1`

			if [ -z "$PIC" ]; then
				lostcount=`expr $lostcount + 1`
				PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
			fi
#			echo round4 $ROUND4
#			exit
			TITLE=`echo $ROUND4 | sed "s!tvshows-season-!!" | sed "s!.html!!" | head -n1 | tr '-' ' ' | sed 's/^ //'`
			URL=http://www.movie4k.to/"$ROUND4"

			lang=37
			if [ `echo $TMPLANG | grep "us_ger_small.png" | wc -l` -eq 1 ];then
				lang=36
				LANGTXT=" (de)"
			elif [ `echo $TMPLANG | grep "us_flag_small.png" | wc -l` -eq 1 ];then
				lang=37
				LANGTXT=" (en)"
			else
				LANGTXT=" (??)"
			fi
			
			if [ ! -z "$TITLE" ] && [ ! -z "$URL" ] && [ `cat cache.movie4k.titanlist | grep "^$TITLE$LANGTXT" | wc -l` -eq 0 ];then
				LINE="$TITLE$LANGTXT#$URL#$PIC#movie4k_$piccount.jpg#Movie4k#$lang"			
				echo $LINE >> cache.movie4k.titanlist
				echo $LINE >> cache.movie4k.series.titanlist
			fi
		done
	done
done

# add Film dummy in category
piccount=`expr $piccount + 1`
URL="http://atemio.dyndns.tv/mediathek/movie4k/streams/movie4k.movies.list"
PIC="http://atemio.dyndns.tv/mediathek/menu/movies.jpg"
LINE="Filme (alle)#$URL#$PIC#movie4k_$piccount.jpg#Movie4k#3"
echo $LINE >> cache.movie4k.category.titanlist

# filme in genres sortieren
watchlist="
genres-movies.html
"
for ROUND1 in $watchlist; do
	count=`expr $count + 1`

	filename1="$ROUND1"
	wget "http://www.movie4k.to/$ROUND1" -O cache."$count"."$filename1"

	genrelist=movies-updates.html `cat cache."$count"."$filename1" | grep tdmovies | grep .html | cut -d '"' -f6`
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
			filename3="$ROUND3"
			wget "http://www.movie4k.to/$ROUND3" -O cache."$count"."$filename3"

			id_list="$id_list "`cat cache."$count"."$filename3" | grep coverPreview | sed 's!coverPreview!"!' | cut -d'"' -f3 | tr ' ' '\n' | grep -v ").append" | sort -u`
		done


		TITLE=`echo $ROUND2 | sed 's/.html//' | tr '-' '\n' | tail -n1`

		echo $id_list > _full/movie4k/streams/_$TITLE
		ck=0
		for ROUND4 in $id_list; do
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

# add Series dummys
piccount=`expr $piccount + 1`
URL="http://atemio.dyndns.tv/mediathek/movie4k/streams/movie4k.series.list"
PIC="http://atemio.dyndns.tv/mediathek/menu/series.jpg"
LINE="Filme (alle)#$URL#$PIC#movie4k_$piccount.jpg#Movie4k#3"
echo $LINE >> cache.movie4k.category.titanlist

# serien in genres sortieren
watchlist="
genres-tvshows.html
"

for ROUND1 in $watchlist; do
	count=`expr $count + 1`

	filename1="$ROUND1"
	wget "http://www.movie4k.to/$ROUND1" -O cache."$count"."$filename1"

	genrelist=`cat cache."$count"."$filename1" | grep tvshows-genre | grep .html | cut -d '"' -f6`
	for ROUND2 in $genrelist; do
		count=`expr $count + 1`	
		filename2="$ROUND2"
		wget "http://www.movie4k.to/$ROUND2" -O cache."$count"."$filename2"
		
		pagelist="$ROUND2"

		ck=0
		id_list=""
		id_list="$id_list "`cat cache."$count"."$filename2" | grep tvshows-season | cut -d'"' -f6 | sort -u`

		TITLE=`echo $ROUND2 | sed 's/.html//' | tr '-' '\n' | tail -n1`

		for ROUND4 in $id_list; do
			filename4="$ROUND4"
			LINE=`cat cache.movie4k.titanlist | grep $ROUND4`
			if [ ! -z "$LINE" ];then
				echo $LINE >> cache.movie4k.`echo "$TITLE" | tr 'A-Z' 'a-z'`.series.titanlist	
			fi
		done

		piccount=`expr $piccount + 1`

		URL="http://atemio.dyndns.tv/mediathek/movie4k/streams/movie4k.`echo "$TITLE" | tr 'A-Z' 'a-z'`.series.list"
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
		
		cat cache.movie4k.`echo "$TITLE" | tr 'A-Z' 'a-z'`.series.titanlist  | sort -u > _full/movie4k/streams/movie4k.`echo "$TITLE" | tr 'A-Z' 'a-z'`.series.list
	done
done

cat cache.movie4k.series.titanlist | sort -u > _full/movie4k/streams/movie4k.series.list
cat cache.movie4k.movies.titanlist | sort -u > _full/movie4k/streams/movie4k.movies.list

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
