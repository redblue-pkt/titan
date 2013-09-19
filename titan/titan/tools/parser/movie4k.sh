#!/bin/bash
#

buildtype=$1
wgetbin="wget -T2 -t2 --waitretry=2"

rm cache.*
rm _liste
rm -rf _full/movie4k
mkdir -p _full/movie4k/streams
rm cache.*
touch cache.movie4k.titanlist

BEGINTIME=`date +%s`
DATENAME=`date +"%Y.%m.%d_%H.%M.%S"`
echo "[movie4k.sh] START (buildtype: $buildtype): $DATENAME" > _full/movie4k/build.log

if [ "$buildtype" = "full" ];then
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
		$wgetbin "http://www.movie4k.to/$ROUND2" -O cache."$count"."$filename2"
		if [ `cat cache."$count"."$filename2" | grep boxgrey | tr '><' '\n' | grep .html | grep -v './'  | cut -d'"' -f2 | wc -l` -gt 0 ];then
			pagelist="$ROUND2 `cat cache.$count.$filename2 | grep boxgrey | tr '><' '\n' | grep .html | grep -v './'  | cut -d'"' -f2`"	
		else
			pagelist="$ROUND2"
		fi
	
		for ROUND3 in $pagelist; do
			filename3="$ROUND3"
			$wgetbin "http://www.movie4k.to/$ROUND3" -O cache."$count"."$filename3"
	
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
				TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' # ' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`

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
				
				if [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
					LINE="$TITLE$LANGTXT#$URL#$PIC#movie4k_$piccount.jpg#Movie4k#$lang"			
					if [ `cat cache.movie4k.titanlist | grep "^$TITLE$LANGTXT" | wc -l` -eq 0 ];then
						echo $LINE >> cache.movie4k.titanlist
					fi
					if [ `cat cache.movie4k.movies.titanlist | grep "^$TITLE$LANGTXT" | wc -l` -eq 0 ];then
						echo $LINE >> cache.movie4k.movies.titanlist
					fi
				fi
			done
		done
	done
fi

# kinofilme de/eng

watchlist="
index.php?lang=de
index.php
"

for ROUND2 in $watchlist; do
	piccount=`expr $piccount + 1`
	count=`expr $count + 1`	
	filename2="$ROUND2"
	$wgetbin "http://www.movie4k.to/$ROUND2" -O cache."$count"."$filename2"
	if [ "$ROUND2" = "index.php?lang=de" ];then
		TYPE="kino.ger"
		TYPENAME="Kino Filme (de)"
	elif [ "$ROUND2" = "index.php" ];then
		TYPE="kino.eng"
		TYPENAME="Kino Filme (en)"
	fi

	pagelist=`cat cache."$count"."$filename2" | tr '\n' ' ' | sed 's/ \+/ /g' | sed 's/<div style="float:left"> <a href=/\n\nlink=/g' | grep ^link=  | cut -d'"' -f2`

	for ROUND3 in $pagelist; do
		piccount=`expr $piccount + 1`
		filename3="$ROUND3"
#		PIC=`cat cache."$count"."$filename2" | grep $ROUND3 | grep "img src" | tail -n1 | cut -d '"' -f6`
#		PIC=`cat cache."$count"."$filename2" | grep $ROUND3 | grep "img src" | head -n1 | cut -d '"' -f6`
		PIC=`cat cache."$count"."$filename2" | grep $ROUND3 | sed 's/<img src=/\npic=/g' | grep ^pic= | grep .jpg | cut -d'"' -f2 | head -n1`

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
		TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' # ' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`

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

# letzte updates de/eng

watchlist="
index.php?lang=de
index.php
"

for ROUND2 in $watchlist; do
	piccount=`expr $piccount + 1`
	count=`expr $count + 1`	
	filename2="$ROUND2"
	$wgetbin "http://www.movie4k.to/$ROUND2" -O cache."$count"."$filename2"
	if [ "$ROUND2" = "index.php?lang=de" ];then
		TYPE="last.updates.ger"
		TYPENAME="Letzte Updates (de)"
	elif [ "$ROUND2" = "index.php" ];then
		TYPE="last.updates.eng"
		TYPENAME="Letzte Updates (en)"
	fi

#	pagelist=`cat cache."$count"."$filename2" | grep "float:left" | cut -d '"' -f4`
# newest uploads
	pagelist=`cat cache."$count"."$filename2" | grep '<td valign="top" height="100%">' | cut -d '"' -f6`
# hauptseite rechte liste, ka was da gelistet ist
#	pagelist=`cat cache."$count"."$filename2" | grep '.html"><img src=' | cut -d '"' -f4| grep .html`

	for ROUND3 in $pagelist; do
		piccount=`expr $piccount + 1`
		filename3="$ROUND3"
#		PIC=`cat cache."$count"."$filename2" | grep $ROUND3 | grep "img src" | tail -n1 | cut -d '"' -f6`
		PIC=`cat cache."$count"."$filename2" | grep $ROUND3 | grep "img src" | head -n1 | cut -d '"' -f6`

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
		TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' # ' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`

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
if [ "$buildtype" = "full" ];then

	watchlist="
	tvshows-all.html
	"
	
	for ROUND in A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
		watchlist="$watchlist tvshows-all-$ROUND.html" 
	done
	
	testcount=0
	piccount=0
	count=0
	
	for ROUND3 in $watchlist; do
		count=`expr $count + 1`
		filename3="$ROUND3"
		$wgetbin "http://www.movie4k.to/$ROUND3" -O cache."$count"."$filename3"
	
		id_list=`cat cache."$count"."$filename3" | grep tvshows-season- | cut -d '"' -f6 | sort -u | grep .html`
		echo id_list $id_list
	
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
	
			TITLE=`echo $ROUND4 | sed "s!tvshows-season-!!" | sed "s!.html!!" | head -n1 | tr '-' ' ' | sed 's/^ //'`
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' # ' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`

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
			
			if [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
				LINE="$TITLE$LANGTXT#$URL#$PIC#movie4k_$piccount.jpg#Movie4k#$lang"			
				if [ `cat cache.movie4k.titanlist | grep "^$TITLE$LANGTXT" | wc -l` -eq 0 ];then
					echo $LINE >> cache.movie4k.titanlist
				fi
				if [ `cat cache.movie4k.series.titanlist | grep "^$TITLE$LANGTXT" | wc -l` -eq 0 ];then
					echo $LINE >> cache.movie4k.series.titanlist
				fi
			fi
		done
	done
fi

# filme update
watchlist="
movies-updates.html
"

for ROUND3 in $watchlist; do
	filename3="$ROUND3"
	$wgetbin "http://www.movie4k.to/$ROUND3" -O cache."$count"."$filename3"

	id_list=`cat cache."$count"."$filename3" | grep "\.html"  | cut -d'"' -f2 | grep "\.html" | sort -um`

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
		TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' # ' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`

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

		# show allways
		lang=34
		
		if [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
			LINE="$TITLE$LANGTXT#$URL#$PIC#movie4k_$piccount.jpg#Movie4k#$lang"			
			if [ `cat cache.movie4k.titanlist | grep "^$TITLE$LANGTXT" | wc -l` -eq 0 ];then
				echo $LINE >> cache.movie4k.titanlist
			fi
			if [ `cat cache.movie4k.movies.update.titanlist | grep "^$TITLE$LANGTXT" | wc -l` -eq 0 ];then
				echo $LINE >> cache.movie4k.movies.update.titanlist
			fi
		fi
	done
done

if [ "$buildtype" = "full" ];then
	# add Film dummy in category
	piccount=`expr $piccount + 1`
	URL="http://atemio.dyndns.tv/mediathek/movie4k/streams/movie4k.movies.list"
	PIC="http://atemio.dyndns.tv/mediathek/menu/Movies.jpg"
	LINE="Filme (alle)#$URL#$PIC#movie4k_$piccount.jpg#Movie4k#3"
	echo $LINE >> cache.movie4k.category.titanlist
	
	piccount=`expr $piccount + 1`
	URL="http://atemio.dyndns.tv/mediathek/movie4k/streams/movie4k.movies.update.list"
	PIC="http://atemio.dyndns.tv/mediathek/menu/Movies.update.jpg"
	LINE="Filme (letzten updates)#$URL#$PIC#movie4k_$piccount.jpg#Movie4k#3"
	echo $LINE >> cache.movie4k.category.titanlist
	
	piccount=`expr $piccount + 1`
	URL="http://atemio.dyndns.tv/mediathek/movie4k/streams/movie4k.movies.genre.list"
	PIC="http://atemio.dyndns.tv/mediathek/menu/Movies.genre.jpg"
	LINE="Filme (genre)#$URL#$PIC#movie4k_$piccount.jpg#Movie4k#3"
	echo $LINE >> cache.movie4k.category.titanlist
	
	# filme in genres sortieren
	watchlist="
	genres-movies.html
	"
	for ROUND1 in $watchlist; do
		count=`expr $count + 1`
	
		filename1="$ROUND1"
		$wgetbin "http://www.movie4k.to/$ROUND1" -O cache."$count"."$filename1"
	
		genrelist=`cat cache."$count"."$filename1" | grep tdmovies | grep .html | cut -d '"' -f6`
		for ROUND2 in $genrelist; do
			count=`expr $count + 1`	
			filename2="$ROUND2"
			$wgetbin "http://www.movie4k.to/$ROUND2" -O cache."$count"."$filename2"
			if [ `cat cache."$count"."$filename2" | grep boxgrey | tr '><' '\n' | grep .html | grep -v './'  | cut -d'"' -f2 | wc -l` -gt 0 ];then
				pagelist="$ROUND2 `cat cache.$count.$filename2 | grep boxgrey | tr '><' '\n' | grep .html | grep -v './'  | cut -d'"' -f2`"	
			else
				pagelist="$ROUND2"
			fi
			ck=0
			id_list=""
			for ROUND3 in $pagelist; do
				count=`expr $count + 1`	
				filename3="$ROUND3"
				$wgetbin "http://www.movie4k.to/$ROUND3" -O cache."$count"."$filename3"
				ck=`expr $ck + 1`
				echo !!!!!!!!!!!!!!!!!!!ck $ck
				if [ "$ck" = "10" ];then
					echo ----------------------------------------------------------------
					break
				fi
	
	#			id_list="$id_list "`cat cache."$count"."$filename3" | grep coverPreview | sed 's!coverPreview!"!' | cut -d'"' -f3 | tr ' ' '\n' | grep -v ").append" | sort -u`
				id_list="$id_list "`cat cache."$count"."$filename3" | grep coverPreview | sed 's!coverPreview!"!' | grep "TR id=" | cut -d'"' -f3`
			done
	
			TITLE=`echo $ROUND2 | sed 's/.html//' | tr '-' '\n' | tail -n1`
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' # ' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
	
			echo $id_list > _full/movie4k/streams/_$TITLE
			ck=0
			for ROUND4 in $id_list; do
				filename4="$ROUND4"
	# check if more as 1 results
	#			LINE=`cat cache.movie4k.movies.titanlist | grep $ROUND4".html#"`
				LINE=`cat cache.movie4k.movies.titanlist | grep $ROUND4".html#" | head -n1`
				if [ ! -z "$LINE" ];then
					echo "$LINE\n" >> cache.movie4k.category.movie.`echo "$TITLE" | tr 'A-Z' 'a-z'`.titanlist	
				fi
			done
	
			piccount=`expr $piccount + 1`
	
			URL="http://atemio.dyndns.tv/mediathek/movie4k/streams/movie4k.category.movie.`echo "$TITLE" | tr 'A-Z' 'a-z'`.list"
			PIC="http://atemio.dyndns.tv/mediathek/menu/`echo "$TITLE" | tr 'A-Z' 'a-z'`.jpg"
	
			LINE="$TITLE#$URL#$PIC#movie4k_$piccount.jpg#Movie4k#3"
			if [ ! -z "$TITLE" ];then
				echo $LINE >> cache.movie4k.movies.genre.titanlist
			fi
	# unsortet for check on website		
	#		cat cache.movie4k.category.movie.`echo "$TITLE" | tr 'A-Z' 'a-z'`.titanlist  | sort -u > _full/movie4k/streams/movie4k.category.movie.`echo "$TITLE" | tr 'A-Z' 'a-z'`.list
			cat cache.movie4k.category.movie.`echo "$TITLE" | tr 'A-Z' 'a-z'`.titanlist > _full/movie4k/streams/movie4k.category.movie.`echo "$TITLE" | tr 'A-Z' 'a-z'`.list
		done
	done
	
	# add Series dummys
	piccount=`expr $piccount + 1`
	URL="http://atemio.dyndns.tv/mediathek/movie4k/streams/movie4k.series.list"
	PIC="http://atemio.dyndns.tv/mediathek/menu/serien.jpg"
	LINE="Serien (alle)#$URL#$PIC#movie4k_$piccount.jpg#Movie4k#3"
	echo $LINE >> cache.movie4k.category.titanlist
	
	piccount=`expr $piccount + 1`
	URL="http://atemio.dyndns.tv/mediathek/movie4k/streams/movie4k.series.genre.list"
	PIC="http://atemio.dyndns.tv/mediathek/menu/serien.genre.jpg"
	LINE="Serien (genre)#$URL#$PIC#movie4k_$piccount.jpg#Movie4k#3"
	echo $LINE >> cache.movie4k.category.titanlist
	
	# serien in genres sortieren
	watchlist="
	genres-tvshows.html
	"
	
	for ROUND1 in $watchlist; do
		count=`expr $count + 1`
	
		filename1="$ROUND1"
		$wgetbin "http://www.movie4k.to/$ROUND1" -O cache."$count"."$filename1"
		genrelist=`cat cache."$count"."$filename1" | grep tvshows-genre | grep .html | cut -d'"' -f6 | sort -um`
		for ROUND2 in $genrelist; do
			count=`expr $count + 1`	
			filename2="$ROUND2"
			$wgetbin "http://www.movie4k.to/$ROUND2" -O cache."$count"."$filename2"
	
			id_list=`cat cache."$count"."$filename2" | grep tvshows-season | grep .html | cut -d'"' -f6 | sort -um`
			TITLE=`echo $ROUND2 | sed 's/.html//' | tr '-' '\n' | tail -n1`
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' # ' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`

			for ROUND4 in $id_list; do
				filename4="$ROUND4"
	# check if more as 1 results
				LINE=`cat cache.movie4k.series.titanlist | grep "$ROUND4#" | head -n1`
	#			LINE=`cat cache.movie4k.series.titanlist | grep "$ROUND4#"`
	
				if [ ! -z "$LINE" ];then
					echo $LINE >> cache.movie4k.category.series.`echo "$TITLE" | tr 'A-Z' 'a-z'`.titanlist	
				fi
			done
	
			piccount=`expr $piccount + 1`
	
			URL="http://atemio.dyndns.tv/mediathek/movie4k/streams/movie4k.category.series.`echo "$TITLE" | tr 'A-Z' 'a-z'`.list"
			PIC="http://atemio.dyndns.tv/mediathek/menu/`echo "$TITLE" | tr 'A-Z' 'a-z'`.jpg"
	
			LINE="$TITLE#$URL#$PIC#movie4k_$piccount.jpg#Movie4k#3"
			if [ ! -z "$TITLE" ];then
				echo $LINE >> cache.movie4k.series.genre.titanlist
			fi
	# unsortet for check on website
	#		cat cache.movie4k.category.series.`echo "$TITLE" | tr 'A-Z' 'a-z'`.titanlist  | sort -u > _full/movie4k/streams/movie4k.category.series.`echo "$TITLE" | tr 'A-Z' 'a-z'`.list
			cat cache.movie4k.category.series.`echo "$TITLE" | tr 'A-Z' 'a-z'`.titanlist > _full/movie4k/streams/movie4k.category.series.`echo "$TITLE" | tr 'A-Z' 'a-z'`.list
		done
	done
	
	cat cache.movie4k.series.genre.titanlist | sort -u > _full/movie4k/streams/movie4k.series.genre.list
	cat cache.movie4k.movies.genre.titanlist | sort -u > _full/movie4k/streams/movie4k.movies.genre.list

	cat cache.movie4k.series.titanlist | sort -u > _full/movie4k/streams/movie4k.series.list
	cat cache.movie4k.movies.titanlist | sort -u > _full/movie4k/streams/movie4k.movies.list
	
	cat cache.movie4k.titanlist | sort -u > _full/movie4k/streams/movie4k.all-sorted.list
	#cat cache.movie4k.category.titanlist | sort -um > _full/movie4k/movie4k.category.list
	cat cache.movie4k.category.titanlist > _full/movie4k/movie4k.category.list
	
	for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
		filename=`echo "$ROUND" | tr 'A-Z' 'a-z'`
		if [ `cat cache.movie4k.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
			cat cache.movie4k.titanlist | grep ^"$ROUND" > cache.movie4k.titanlist."$ROUND"
			cat cache.movie4k.titanlist."$ROUND" | sort -u > _full/movie4k/streams/movie4k.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
			echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/movie4k/streams/movie4k."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#Movie4k#3 >> _full/movie4k/movie4k.a-z.list
		elif [ `cat cache.movie4k.titanlist | grep ^"$filename" | wc -l` -gt 0 ];then
			cat cache.movie4k.titanlist | grep ^"$filename" > cache.movie4k.titanlist."$ROUND"
			cat cache.movie4k.titanlist."$ROUND" | sort -u > _full/movie4k/streams/movie4k.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
			echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/movie4k/streams/movie4k."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#Movie4k#3 >> _full/movie4k/movie4k.a-z.list
		fi
	done
fi
#cat cache.movie4k.series.update.titanlist > _full/movie4k/streams/movie4k.series.update.list
cat cache.movie4k.movies.update.titanlist > _full/movie4k/streams/movie4k.movies.update.list

DONETIME=`date +%s`
TIME=`expr $DONETIME - $BEGINTIME`
echo "[movie4k.sh] build time: ($TIME s) done" >> _full/movie4k/build.log	
echo "[movie4k.sh] movie4k: "`ls -1 _full/movie4k` >> _full/movie4k/build.log
echo "[movie4k.sh] movie4k/streams: "`ls -1 _full/movie4k/streams` >> _full/movie4k/build.log

if [ "$buildtype" != "full" ];then
	cp -a _full/movie4k/* /var/www/atemio/web/mediathek/movie4k
fi

rm cache.*

exit