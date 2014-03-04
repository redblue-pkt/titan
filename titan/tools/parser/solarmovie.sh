#!/bin/bash
#

buildtype=$1
wgetbin="wget -T2 -t2 --waitretry=2"

rm cache.*
rm _liste
rm -rf _full/solarmovie
mkdir -p _full/solarmovie/streams
rm cache.*
touch cache.solarmovie.titanlist

BEGINTIME=`date +%s`
DATENAME=`date +"%Y.%m.%d_%H.%M.%S"`
echo "[solarmovie.sh] START (buildtype: $buildtype): $DATENAME" > _full/solarmovie/build.log

if [ "$buildtype" = "full" ];then
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
watch-movies-of-2014.html
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

tv/watch-action-tv-shows.html
tv/watch-adventure-tv-shows.html
tv/watch-animation-tv-shows.html
tv/watch-comedy-tv-shows.html
tv/watch-drama-tv-shows.html
tv/watch-horror-tv-shows.html
tv/watch-romance-tv-shows.html
tv/watch-sci-fi-tv-shows.html
tv/watch-thriller-tv-shows.html
tv/watch-tv-shows-2014.html
tv/watch-tv-shows-2013.html
tv/watch-tv-shows-2012.html
tv/watch-tv-shows-2011.html
tv/watch-tv-shows-2010.html
tv/watch-tv-shows-2009.html
tv/watch-tv-shows-2008.html
tv/watch-tv-shows-2007.html
tv/watch-tv-shows-2006.html
tv/watch-tv-shows-2005.html
tv/watch-tv-shows-2004.html
tv/watch-tv-shows-2003.html
tv/watch-tv-shows-2002.html
tv/watch-tv-shows-2001.html
tv/watch-tv-shows-2000.html
tv/watch-tv-shows-1999.html
tv/watch-tv-shows-1998.html
tv/watch-tv-shows-1997.html
tv/watch-tv-shows-1996.html
tv/watch-tv-shows-1995.html
tv/watch-tv-shows-1994.html
tv/watch-tv-shows-1993.html
tv/watch-tv-shows-1992.html
tv/watch-tv-shows-1991.html
tv/watch-tv-shows-1990.html
tv/watch-tv-shows-1989.html
tv/watch-tv-shows-1988.html
tv/watch-tv-shows-1987.html
tv/watch-tv-shows-1986.html
tv/watch-tv-shows-1985.html
tv/watch-tv-shows-1984.html
tv/watch-tv-shows-1983.html
tv/watch-tv-shows-1982.html
tv/watch-tv-shows-1981.html
tv/watch-tv-shows-1980.html
tv/watch-tv-shows-1979.html
tv/watch-tv-shows-1978.html
tv/watch-tv-shows-1977.html
tv/watch-tv-shows-1976.html
tv/watch-tv-shows-1975.html
tv/watch-tv-shows-1974.html
tv/watch-tv-shows-1973.html
tv/watch-tv-shows-1972.html
tv/watch-tv-shows-1971.html
tv/watch-tv-shows-1970.html
tv/watch-tv-shows-1969.html
tv/watch-tv-shows-1968.html
tv/watch-tv-shows-1967.html
tv/watch-tv-shows-1966.html
tv/watch-tv-shows-1965.html
tv/watch-tv-shows-1964.html
tv/watch-tv-shows-1963.html
tv/watch-tv-shows-1962.html
tv/watch-tv-shows-1961.html
tv/watch-tv-shows-1960.html
tv/watch-tv-shows-1959.html
tv/watch-tv-shows-1958.html
tv/watch-tv-shows-1957.html
tv/watch-tv-shows-1956.html
tv/watch-tv-shows-1955.html
tv/watch-tv-shows-1954.html
tv/watch-tv-shows-1953.html
tv/watch-tv-shows-1952.html
tv/watch-tv-shows-1951.html
tv/watch-tv-shows-1950.html
tv/watch-tv-shows-1949.html
tv/watch-tv-shows-1948.html
tv/watch-tv-shows-1947.html
tv/watch-tv-shows-1946.html
tv/watch-tv-shows-1945.html
tv/watch-tv-shows-1944.html
tv/watch-tv-shows-1943.html
tv/watch-tv-shows-1942.html
tv/watch-tv-shows-1941.html
tv/watch-tv-shows-1940.html
tv/watch-tv-shows-1939.html
tv/watch-tv-shows-1938.html
tv/watch-tv-shows-1937.html
tv/watch-tv-shows-1936.html
tv/watch-tv-shows-1935.html
tv/watch-tv-shows-1934.html
tv/watch-tv-shows-1933.html
tv/watch-tv-shows-1932.html
tv/watch-tv-shows-1931.html
tv/watch-tv-shows-1930.html
"
else
watchlist="
watch-movies-of-2014.html
watch-movies-of-2013.html
tv/watch-tv-shows-2014.html
tv/watch-tv-shows-2013.html
"
fi

piccount=0
count=0
for ROUND0 in $watchlist; do
	count=`expr $count + 1`

	filename0=`echo $ROUND0 | tr '/' '-'`
	$wgetbin "http://www.solarmovie.so/$ROUND0" -O cache."$count"."$filename0"
	if [ `cat cache."$count"."$filename0" | grep "<h4>Nothing was found</h4>" | wc -l` -eq 0 ];then
		if [ `cat cache."$count"."$filename0" | grep "DOCTYPE html PUBLIC" | wc -l` -eq 0 ]; then
			pages=`cat cache."$count"."$filename0" | zcat | grep ".html?page=" | grep "</a></li>" | grep -v 'class="next">' | sed 's!page=!\npage="!' | grep ^"page=" | cut -d'"' -f2`
			input=`cat cache."$count"."$filename0" | zcat | tr '\n' ' '| sed 's/<div class="typicalGrey coverGroup">\+/\n/g'| sed 's!</a>\+!\n!g'| sed 's/ \+/ /g'| grep ^' <a class="coverImage"' | sed 's! <a class="coverImage" !!' |tr ' ' '~'`
		else
			pages=`cat cache."$count"."$filename0" | grep ".html?page=" | grep "</a></li>" | grep -v 'class="next">' | sed 's!page=!\npage="!' | grep ^"page=" | cut -d'"' -f2`
			input=`cat cache."$count"."$filename0" | tr '\n' ' '| sed 's/<div class="typicalGrey coverGroup">\+/\n/g'| sed 's!</a>\+!\n!g'| sed 's/ \+/ /g'| grep ^' <a class="coverImage"' | sed 's! <a class="coverImage" !!' |tr ' ' '~'`
		fi
		piccount=`expr $piccount + 1`
		URL="http://atemio.dyndns.tv/mediathek/solarmovie/streams/solarmovie.$filename0.list"
		PIC="http://atemio.dyndns.tv/mediathek/menu/$filename0.jpg"
		TITLE=`echo $ROUND0 | sed 's/.html//' | sed 's/watch-//' | tr '-' ' ' | tr '/' ' '`
		TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' # ' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`

		pcount=0
		echo pages $pages
		if [ ! -z $pages ]; then

			if [ "$pcount" -lt "$pages" ]; then
				LINE="$TITLE#$URL#$PIC#solarmovie_$piccount.jpg#Solarmovie#3"
				if [ ! -z "$TITLE" ]; then
					echo $LINE >> cache.solarmovie.category.titanlist	
				fi
			fi
		
			while [ "$pcount" -lt "$pages" ]
			do
				echo "pcount $pcount"
				pcount=`expr $pcount + 1`
		#		$wgetbin "http://www.solarmovie.so/$ROUND0.html?page=$pcount" -O cache."$count".page"$pcount"."$filename0"
				$wgetbin "http://www.solarmovie.so/$ROUND0?page=$pcount" -O cache."$count".page"$pcount"."$filename0"

				if [ `cat cache."$count".page"$pcount"."$filename0" | grep "<h4>Nothing was found</h4>" | wc -l` -eq 0 ];then
					if [ `cat cache."$count".page"$pcount"."$filename0" | grep "DOCTYPE html PUBLIC" | wc -l` -eq 0 ]; then
						input=`cat cache."$count".page"$pcount"."$filename0" | zcat | tr '\n' ' '| sed 's/<div class="typicalGrey coverGroup">\+/\n/g'| sed 's!</a>\+!\n!g'| sed 's/ \+/ /g'| grep ^' <a class="coverImage"' | sed 's! <a class="coverImage" !!' | tr ' ' '~'`
					else
						input=`cat cache."$count".page"$pcount"."$filename0"  | tr '\n' ' '| sed 's/<div class="typicalGrey coverGroup">\+/\n/g'| sed 's!</a>\+!\n!g'| sed 's/ \+/ /g'| grep ^' <a class="coverImage"' | sed 's! <a class="coverImage" !!' | tr ' ' '~'`
					fi
			
					if [ ! -z "$input" ]; then
						for ROUND1 in $input; do
							piccount=`expr $piccount + 1`
							TITLE=`echo $ROUND1 | cut -d '"' -f2 | tr '~' ' ' | sed 's/#/%/'`
							TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' # ' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
							URL=http://www.solarmovie.so`echo $ROUND1 | sed s'!href=!\nhref=!' | grep ^"href=" | cut -d '"' -f2 | tr '~' ' '`
							PIC=`echo $ROUND1 | sed s'!<img~src=!\n<img~src=!' | grep ^"<img~src=" | cut -d '"' -f2 | tr '~' ' '`
							LINE="$TITLE#$URL#$PIC#solarmovie_$piccount.jpg#Solarmovie#28"
			
			#				if [ ! -z "$TITLE" ] && [ `cat cache.solarmovie."$filename0".titanlist | grep ^"$TITLE#" | wc -l` -gt 0 ];then
							if [ ! -z "$TITLE" ] && [ ! -z "$URL" ];then
								echo $LINE >> cache.solarmovie."$filename0".titanlist	
							fi
			
							if [ ! -z "$TITLE" ] && [ ! -z "$URL" ] && [ `cat cache.solarmovie.titanlist | grep ^"$TITLE" | wc -l` -eq 0 ];then
								echo $LINE >> cache.solarmovie.titanlist
							fi
						done
					fi
				fi
			done
			
			if [ `cat cache.solarmovie.$filename0.titanlist | wc -l` -gt 0 ];then
				cat cache.solarmovie.$filename0.titanlist | sort -u > _full/solarmovie/streams/solarmovie.$filename0.list
			fi	
		fi
	fi
done

if [ "$buildtype" = "full" ];then
	cat cache.solarmovie.titanlist | sort -u > _full/solarmovie/streams/solarmovie.all-sorted.list
	#cat cache.solarmovie.category.titanlist | sort -u > _full/solarmovie/solarmovie.category.list
	cat cache.solarmovie.category.titanlist > _full/solarmovie/solarmovie.category.list
	
	for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
		filename=`echo "$ROUND" | tr 'A-Z' 'a-z'`
		if [ `cat cache.solarmovie.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
			cat cache.solarmovie.titanlist | grep ^"$ROUND" > cache.solarmovie.titanlist."$ROUND"
			cat cache.solarmovie.titanlist."$ROUND" | sort -u > _full/solarmovie/streams/solarmovie.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
			echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/solarmovie/streams/solarmovie."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#Solarmovie#3 >> _full/solarmovie/solarmovie.a-z.list
		elif [ `cat cache.solarmovie.titanlist | grep ^"$filename" | wc -l` -gt 0 ];then
			cat cache.solarmovie.titanlist | grep ^"$filename" > cache.solarmovie.titanlist."$ROUND"
			cat cache.solarmovie.titanlist."$ROUND" | sort -u > _full/solarmovie/streams/solarmovie.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
			echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/solarmovie/streams/solarmovie."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#Solarmovie#3 >> _full/solarmovie/solarmovie.a-z.list
		fi
	done
fi

DONETIME=`date +%s`
TIME=`expr $DONETIME - $BEGINTIME`
echo "[solarmovie.sh] build time: ($TIME s) done" >> _full/solarmovie/build.log	
echo "[solarmovie.sh] solarmovie: "`ls -1 _full/solarmovie` >> _full/solarmovie/build.log
echo "[solarmovie.sh] solarmovie/streams: "`ls -1 _full/solarmovie/streams` >> _full/solarmovie/build.log

if [ "$buildtype" != "full" ];then
	cp -a _full/solarmovie/* /var/www/atemio/web/mediathek/solarmovie
fi

rm cache.*

exit
