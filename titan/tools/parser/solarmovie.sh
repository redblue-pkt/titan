#!/bin/bash
#

rm cache.*
rm _liste
rm -rf _full/solarmovie
mkdir -p _full/solarmovie/streams
rm cache.*
touch cache.solarmovie.titanlist

watchlist="
watch-action-movies.html
watch-adventure-movies.html
watch-animation-movies.html
watch-comedy-movies.html
watch-drama-movies.html
watch-horror-movies.html
watch-romance-movies.html
watch-sci-fi-movies.html
watch-thriller-movies.html
watch-movies-of-2013.html
watch-movies-of-2012.html
watch-movies-of-2011.html
watch-movies-of-2010.html
watch-movies-of-2009.html
watch-movies-of-2008.html
watch-movies-of-2007.html
watch-movies-of-2006.html
watch-movies-of-2005.html
watch-movies-of-2004.html
watch-movies-of-2003.html
watch-movies-of-2002.html
watch-movies-of-2001.html
watch-movies-of-2000.html
watch-movies-of-1999.html
watch-movies-of-1998.html
watch-movies-of-1997.html
watch-movies-of-1996.html
watch-movies-of-1995.html
watch-movies-of-1994.html
watch-movies-of-1993.html
watch-movies-of-1992.html
watch-movies-of-1991.html
watch-movies-of-1990.html
watch-movies-of-1989.html
watch-movies-of-1988.html
watch-movies-of-1987.html
watch-movies-of-1986.html
watch-movies-of-1985.html
watch-movies-of-1984.html
watch-movies-of-1983.html
watch-movies-of-1982.html
watch-movies-of-1981.html
watch-movies-of-1980.html
watch-movies-of-1979.html
watch-movies-of-1978.html
watch-movies-of-1977.html
watch-movies-of-1976.html
watch-movies-of-1975.html
watch-movies-of-1974.html
watch-movies-of-1973.html
watch-movies-of-1972.html
watch-movies-of-1971.html
watch-movies-of-1970.html
watch-movies-of-1969.html
watch-movies-of-1968.html
watch-movies-of-1967.html
watch-movies-of-1966.html
watch-movies-of-1965.html
watch-movies-of-1964.html
watch-movies-of-1963.html
watch-movies-of-1962.html
watch-movies-of-1961.html
watch-movies-of-1960.html
watch-movies-of-1959.html
watch-movies-of-1958.html
watch-movies-of-1957.html
watch-movies-of-1956.html
watch-movies-of-1955.html
watch-movies-of-1954.html
watch-movies-of-1953.html
watch-movies-of-1952.html
watch-movies-of-1951.html
watch-movies-of-1950.html
watch-movies-of-1949.html
watch-movies-of-1948.html
watch-movies-of-1947.html
watch-movies-of-1946.html
watch-movies-of-1945.html
watch-movies-of-1944.html
watch-movies-of-1943.html
watch-movies-of-1942.html
watch-movies-of-1941.html
watch-movies-of-1940.html
watch-movies-of-1939.html
watch-movies-of-1938.html
watch-movies-of-1937.html
watch-movies-of-1936.html
watch-movies-of-1935.html
watch-movies-of-1934.html
watch-movies-of-1933.html
watch-movies-of-1932.html
watch-movies-of-1931.html
watch-movies-of-1930.html
"
#watchlist="tv/watch-action-tv-shows.html watch-movies-of-2013.html watch-action-movies.html"

piccount=0
count=0
for ROUND0 in $watchlist; do
	count=`expr $count + 1`
	echo file."$count"."$ROUND0"
	wget "http://www.solarmovie.so/$ROUND0" -O file."$count"."$ROUND0"
	if [ `cat file."$count"."$ROUND0" | grep "DOCTYPE html PUBLIC" | wc -l` -eq 0 ]; then
		pages=`cat file."$count"."$ROUND0" | zcat | grep ".html?page=" | grep "</a></li>" | grep -v 'class="next">' | sed 's!page=!\npage="!' | grep ^"page=" | cut -d'"' -f2`
		input=`cat file."$count"."$ROUND0" | zcat | tr '\n' ' '| sed 's/<div class="typicalGrey coverGroup">\+/\n/g'| sed 's!</a>\+!\n!g'| sed 's/ \+/ /g'| grep ^' <a class="coverImage"' | sed 's! <a class="coverImage" !!' |tr ' ' '~'`
	else
		pages=`cat file."$count"."$ROUND0" | grep ".html?page=" | grep "</a></li>" | grep -v 'class="next">' | sed 's!page=!\npage="!' | grep ^"page=" | cut -d'"' -f2`
		input=`cat file."$count"."$ROUND0" | tr '\n' ' '| sed 's/<div class="typicalGrey coverGroup">\+/\n/g'| sed 's!</a>\+!\n!g'| sed 's/ \+/ /g'| grep ^' <a class="coverImage"' | sed 's! <a class="coverImage" !!' |tr ' ' '~'`
	fi
	
	piccount=`expr $piccount + 1`
	URL="http://atemio.dyndns.tv/mediathek/solarmovie/streams/solarmovie.$ROUND0.list"
	PIC="http://atemio.dyndns.tv/mediathek/menu/$ROUND0.jpg"
	TITLE=`echo $ROUND0 | sed 's/.html//' | sed 's/watch-//' | tr '-' ' '`

	LINE="$TITLE#$URL#$PIC#solarmovie_$piccount.jpg#Solarmovie#0"
	if [ ! -z "$TITLE" ]; then
		echo $LINE >> cache.solarmovie.category.titanlist	
	fi

	pcount=0
	while [ "$pcount" -lt "$pages" ]
	do
		echo "pcount $pcount"
		pcount=`expr $pcount + 1`
		wget "http://www.solarmovie.so/$ROUND0.html?page=$pcount" -O file."$count".page"$pcount"."$ROUND0"
		if [ `cat file."$count".page"$pcount"."$ROUND0" | grep "DOCTYPE html PUBLIC" | wc -l` -eq 0 ]; then
			input=`cat file."$count".page"$pcount"."$ROUND0" | zcat | tr '\n' ' '| sed 's/<div class="typicalGrey coverGroup">\+/\n/g'| sed 's!</a>\+!\n!g'| sed 's/ \+/ /g'| grep ^' <a class="coverImage"' | sed 's! <a class="coverImage" !!' | tr ' ' '~'`
		else
			input=`cat file."$count".page"$pcount"."$ROUND0"  | tr '\n' ' '| sed 's/<div class="typicalGrey coverGroup">\+/\n/g'| sed 's!</a>\+!\n!g'| sed 's/ \+/ /g'| grep ^' <a class="coverImage"' | sed 's! <a class="coverImage" !!' | tr ' ' '~'`
		fi

		if [ ! -z "$input" ]; then
			for ROUND1 in $input; do
				piccount=`expr $piccount + 1`
				TITLE=`echo $ROUND1 | cut -d '"' -f2 | tr '~' ' '`
				URL=http://www.solarmovie.so/`echo $ROUND1 | cut -d '"' -f4 | tr '~' ' '`
				PIC=`echo $ROUND1 | cut -d '"' -f8 | tr '~' ' '`
				LINE="$TITLE#$URL#$PIC#solarmovie_$piccount.jpg#Solarmovie#28"
	
				if [ ! -z "$TITLE" ] && [ `cat cache.solarmovie.titanlist | grep ^"$TITLE#" | wc -l` -gt 0 ];then
					echo $LINE >> cache.solarmovie.$ROUND0.titanlist	
					echo $LINE >> cache.solarmovie.titanlist
				fi
			done
		fi
	done
	
	if [ `cat cache.solarmovie.$ROUND0.titanlist | wc -l` -gt 0 ];then
		cat cache.solarmovie.$ROUND0.titanlist | sort -um > _full/solarmovie/streams/solarmovie.$ROUND0.list
	fi	
done

cat cache.solarmovie.titanlist | sort -u > _full/solarmovie/streams/solarmovie.all-sorted.list
cat cache.solarmovie.category.titanlist | sort -u > _full/solarmovie/solarmovie.category.list

for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
	filename=`echo "$ROUND" | tr 'A-Z' 'a-z'`
	if [ `cat cache.solarmovie.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
		cat cache.solarmovie.titanlist | grep ^"$ROUND" > cache.solarmovie.titanlist."$ROUND"
		cat cache.solarmovie.titanlist."$ROUND" | sort -um > _full/solarmovie/streams/solarmovie.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
		echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/solarmovie/streams/solarmovie."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#Solarmovie#0 >> _full/solarmovie/solarmovie.a-z.list
	elif [ `cat cache.solarmovie.titanlist | grep ^"$filename" | wc -l` -gt 0 ];then
		cat cache.solarmovie.titanlist | grep ^"$ROUND" > cache.solarmovie.titanlist."$ROUND"
		cat cache.solarmovie.titanlist."$ROUND" | sort -um > _full/solarmovie/streams/solarmovie.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
		echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/solarmovie/streams/solarmovie."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#Solarmovie#0 >> _full/solarmovie/solarmovie.a-z.list
	fi
done
rm cache.*
#mkdir /var/www/atemio/web/mediathek/solarmovie
#cp -a _full/solarmovie/* /var/www/atemio/web/mediathek/solarmovie
#cp -a mainmenu.list /var/www/atemio/web/mediathek
#cp -a mainmenu-solarmovie.list /var/www/atemio/web/mediathek
