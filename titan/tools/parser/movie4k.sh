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
#movies-updates.html

watchlist="
movies-genre-1-Action.html
movies-genre-58-Adult.html
movies-genre-4-Adventure.html
movies-genre-5-Animation.html
movies-genre-6-Biography.html
movies-genre-27-Bollywood.html
movies-genre-3-Comedy.html
movies-genre-7-Crime.html
movies-genre-8-Documentary.html
movies-genre-2-Drama.html
movies-genre-9-Family.html
movies-genre-10-Fantasy.html
movies-genre-13-History.html
movies-genre-14-Horror.html
movies-genre-15-Music.html
movies-genre-56-Musical.html
movies-genre-17-Mystery.html
movies-genre-26-Other.html
movies-genre-59-Reality-TV.html
movies-genre-20-Romance.html
movies-genre-21-Sci-Fi.html
movies-genre-55-Short.html
movies-genre-22-Sport.html
movies-genre-23-Thriller.html
movies-genre-24-War.html
movies-genre-25-Western.html
"

#watchlist="tv/watch-tv-shows-1930.html"
#http://img.movie4k.to/thumbs/cover-393425-10-000-BC-2008-movie4k-film.jpg
#10-000-BC-2008-watch-movie-3484560.html

piccount=0
count=0
for ROUND1 in $watchlist; do
	count=`expr $count + 1`

	filename1=`echo $ROUND1`
	wget "http://www.movie4k.to/$ROUND1" -O cache."$count"."$filename1"
#	id_list=`cat cache."$count"."$filename1" | grep coverPreview | sed 's!coverPreview!"!' | cut -d'"' -f3`
	id_list=`cat cache."$count"."$filename1" | grep '<a href="' | cut -d'"' -f2 | sort -u | grep .html`

	piccount=`expr $piccount + 1`
	URL="http://atemio.dyndns.tv/mediathek/movie4k/streams/movie4k.$filename1.list"
	PIC="http://atemio.dyndns.tv/mediathek/menu/$filename1.jpg"
	TITLE=`echo $ROUND1 | sed 's/.html//' | tr '-' '\n' | tail -n1`

	LINE="$TITLE#$URL#$PIC#movie4k_$piccount.jpg#Movie4k#3"
	if [ ! -z "$TITLE" ]; then
		echo $LINE >> cache.movie4k.category.titanlist	
	fi
			
	echo id_list $id_list
	for ROUND2 in $id_list; do
		piccount=`expr $piccount + 1`
#		count=`expr $count + 1`
		wget "http://www.movie4k.to/$ROUND2" -O cache."$count"."$filename1"."$ROUND2"
#		PIC=`cat cache."$count"."$filename1"."$ROUND2" | grep "$ROUND2" | grep "img src" | cut -d'"' -f4`
		PIC=`cat cache."$count"."$filename1"."$ROUND2" | grep '<meta property="og:image" content="' | cut -d'"' -f4`
		
		TITLE=`echo $ROUND2 | sed "s!-online-film-!;!" | sed "s!-watch-movie-!;!" | tr ";" "\n" | head -n1 | tr '-' ' '`
		URL=http://www.movie4k.to/"$ROUND2"
		echo URL $URL
		echo PIC $PIC
		echo TITLE $TITLE
		lang=0
		if [ `cat cache."$count"."$filename1"."$ROUND2" | grep "us_ger_small.png" | wc -l` -eq 1 ];then
			lang=1
			TITLE="$TITLE (de)"
		elif [ `cat cache."$count"."$filename1"."$ROUND2" | grep "us_flag_small.png" | wc -l` -eq 1 ];then
			lang=2
			TITLE="$TITLE (en)"
		else
			TITLE="$TITLE (??)"
		fi
		echo lang $lang

		LINE="$TITLE#$URL#$PIC#movie4k_$piccount.jpg#Movie4k#34"
		if [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
			echo $LINE >> cache.movie4k."$filename1".titanlist	
		fi

		if [ ! -z "$TITLE" ] && [ ! -z "$URL" ] && [ `cat cache.movie4k.titanlist | grep ^"$TITLE" | wc -l` -eq 0 ];then
			echo $LINE >> cache.movie4k.titanlist
		fi
	done
	cat cache.movie4k."$filename1".titanlist | sort -u > _full/movie4k/streams/movie4k.$filename1.list
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

#cp -a _full/movie4k /var/www/atemio/web/mediathek
#cp -a mainmenu.list /var/www/atemio/web/mediathek
#cp -a mainmenu-movie4k.list /var/www/atemio/web/mediathek

rm file.*
rm cache.*
