#!/bin/sh
# first sh box parser for titannit mfg obi

SRC=$1
INPUT=$2
PAGE=$3
NEXT=$4
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`

URL=http://www.pornhub.com
NAME="Pornhub"

case $2 in
	init)	;;
	*) 	. /tmp/localhoster/hoster.sh
	   	mkdir $TMP > /dev/null 2>&1
		FILENAME="$PARSER $INPUT $PAGE $NEXT"
	   	FILENAME=$(echo $FILENAME | tr '&' '.' | tr '/' '.' | tr '?' '.' | tr '=' '.' | sed -e 's/\&\+/./g' -e 's#\/\+#.#g' -e 's/\?\+/./g' -e 's/;\+/./g' -e 's/=\+/./g' -e 's/ \+/./g' -e 's/\.\+/./g')
		if [ -z "$FILENAME" ]; then FILENAME=none;fi
		PICNAME="$FILENAME"
		;;
esac

if [ `echo $SRC | grep ^"/mnt/parser" | wc -l` -gt 0 ];then
	TYPE="$SRC - Shell script"
elif [ `echo $SRC | grep ^"/var/swap" | wc -l` -gt 0 ];then
	TYPE="Swap - Shell script"
elif [ `echo $SRC | grep ^"/mnt/swapextensions" | wc -l` -gt 0 ];then
	TYPE="Mnt - Shell script"
else
	TYPE="Flash - Shell script"
fi

init()
{
	rm -rf $TMP > /dev/null 2>&1
	echo "$NAME ($TYPE)#$SRC $SRC mainmenu#http://openaaf.dyndns.tv/mediathek/menu/$PARSER.jpg#$PARSER.jpg#TiThek#1000"
}

mainmenu()
{
	echo "Genre#$SRC $SRC genre 'categories'#http://openaaf.dyndns.tv/mediathek/menu/Movies.genre.jpg#Movies.genre.jpg#$NAME#0" >$TMP/$FILENAME.list
#	echo "Pornstars#$SRC $SRC pornstars '/pornstars?page=' 1#http://openaaf.dyndns.tv/mediathek/menu/pornstars.jpg#pornstars.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Suchen#$SRC $SRC search '/video/search?search=%search%&page=' 1#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >>$TMP/$FILENAME.list
	echo "$TMP/$FILENAME.list"
}

# comment block1 start
<<"COMMENT"

<ul class="headerSubMenu">
 .
 <li class=" ">
 <a href="/video?c=28" onclick="ga('send', 'event', 'Header', 'click', 'Category 28');" alt="Reife Frauen">
 <img class="js-menuSwap" data-image="http://cdn1b.static.pornhub.phncdn.com/images/categories/118x88/28.jpg?cache=1488300184" width="118" height="88" alt="Reife Frauen">
 <span>Reife Frauen</span>
 </a>
 </li>
 .
 <li>
 <a class="categoryDefault" href="/categories" onclick="ga('send', 'event', 'Header', 'click', 'Categories');">
 <img class="js-menuSwap" data-image="http://cdn1b.static.pornhub.phncdn.com/www-static/images/categories_seeall.gif" alt="See All" width="118" height="88">
 <span>Alle anzeigen</span>
 </a>
 </li>
</ul>

TitanNit-ufs912:~# time /tmp/localparser/pornhub.sh /tmp/localparser/pornhub.sh genre 'categories'
/tmp/tithek/pornhub.genres.list
real    0m 1.07s
user    0m 0.39s
sys     0m 0.39s
TitanNit-ufs912:

TitanNit-ufs912:~# time /tmp/localparser/pornhub.sh /tmp/localparser/pornhub.sh genreold 'categories'
/tmp/localcache/pornhub.genreold.categories.list
real    0m 19.03s
user    0m 4.71s
sys     0m 12.14s
TitanNit-ufs912:~#

COMMENT
# comment block1 end

genre()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - $URL/$PAGE | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME \
		'
			# 1. BEGIN variable setzen
			BEGIN \
			{
				# 2. setzt suchvariable auf 0 vor dem start
				suche = 0
				newpage = ""
				piccount = 0
			}
			# 3. eindeutige zeile vor ersten treffer
			/<ul class=\"headerSubMenu\">/ \
			{
				# 4. suche erlauben ab dieser zeile
				suche = 1
				# 5. in naechste zeile springen
				next
			}
			# 6. eindeutige zeile nach letzen treffer
			/<\/ul>/ \
			{
				# 7. suche verbieten ab dieser zeile
				suche = 0
				# 8. in naechste zeile springen
	        	next
			}
			# 9. eindeutige zeile nach letzen treffer backup fals erste nicht klappt
			/categoryDefault/ \
			{
				# 10. suche verbieten ab dieser zeile
				suche = 0
				# 11. in naechste zeile springen
	            next
			}
			# 12. nextpage zeile
			# href=\"\/video?c=28
			/href=\"\/video?/ \
			{
				if (suche == 1)
				{
					# href="/video?c=28" onclick="ga.....>
					# 13. extrahiere den newpage pfad
					i = index($0, "href=\"") + 6
		            j = index(substr($0, i), "\"") - 1
					# 14. newpage = /video?c=28
		            newpage = substr($0, i, j)
					# 15. in naechste zeile springen
					next
				}
			}
			# 16. erste zeile mit treffer
			/<img class=\"/ \
			{
				if (suche == 1 && newpage != "")
				{
					# <img class="js-menuSwap" data-image="http://cdn1b.static.pornhub.phncdn.com/images/categories/118x88/28.jpg?cache=1488300184" width="118" height="88" alt="Reife Frauen">
					# 17. extrahiere den titel alt="Reife Frauen"
					i = index($0, "alt=\"") + 5
		            j = index(substr($0, i), "\"") - 1
					# 18. titel = Reife Frauen
		            title = substr($0, i, j)
	
					# <img class="js-menuSwap" data-image="http://cdn1b.static.pornhub.phncdn.com/images/categories/118x88/28.jpg?cache=1488300184" width="118" height="88" alt="Reife Frauen">	
					# 19. extrahiere den piclink data-image="http://cdn1b.static.pornhub.phncdn.com/images/categories/118x88/28.jpg?cache=1488300184"
					i = index($0, "data-image=\"") + 12
		            j = index(substr($0, i), "\"") - 1
					# 20. pic = http://cdn1b.static.pornhub.phncdn.com/images/categories/118x88/28.jpg?cache=1488300184
		            pic = substr($0, i, j)
					
					if ( pic == "" )
					{
						# 21. erstelle lokalen picname aus kleingeschriebenen titel
						# 22. titel = reife frauen
						picname = tolower(title)
	
						# 23. tausche leehrzeichen in punkte
						# 24. titel = reife.frauen
		            	gsub(" ", ".", picname)
						pic = "http://openaaf.dyndns.tv/mediathek/menu/" picname ".jpg"
					}
	
					if (title != "")
					{
						piccount += 1
						# 25. in naechste zeile springen
						# 26. \x27 = single quotes
						print title "#" SRC " " SRC " search \x27" newpage "&page=\x27 1#" pic "#" PICNAME "." piccount ".jpg#" NAME "#0"
					}
					
					# 27. reset variables
					newpage = ""
					title = ""
					picname = ""
					pic = ""
					# 28. in naechste zeile springen
					next
	         	}
			}
		# 29. schreibe alles in die list datei
		' >$TMP/$FILENAME.list
	fi
	# 30. gebe titan den list namen mit pfad zurueck
	echo "$TMP/$FILENAME.list"
}


genreold()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		piccount=0
		$curlbin $URL/$PAGE -o $TMP/cache.$FILENAME.1
		cat $TMP/cache.$FILENAME.1 | tr '\n' ' ' | sed 's/<a href=/\n\n\nfound=/g' | grep ^found= | grep "/video?c=" | tr '\t' ' ' | sed 's/ \+/ /g' | grep "img src" | grep ".jpg" >$TMP/cache.$FILENAME.2
		
		while read -u 3 ROUND; do
			PIC=`echo $ROUND | sed 's!<img src=!\npic=!g' | grep ^pic= | cut -d'"' -f2 | tail -n1`
			TITLE=`echo $ROUND | sed 's!data-mxptext=!\ntitle=!g' | grep ^title= | cut -d'"' -f2`
			NEWPAGE=`echo $ROUND | cut -d'"' -f2 | head -n1`
	
			if [ -z "$PIC" ]; then
#				PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
				PIC="http://openaaf.dyndns.tv/mediathek/menu/`echo $TITLE | tr 'A-Z' 'a-z'`.jpg"
			fi
	
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
	
			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
				if [ ! -e $TMP/$FILENAME.list ];then
					touch $TMP/$FILENAME.list
				fi
				piccount=`expr $piccount + 1`
	
				if [ `cat $TMP/$FILENAME.list | grep "#$NEWPAGE#" | wc -l` -eq 0 ];then
					LINE="$TITLE#$SRC $SRC search '$NEWPAGE&page=' 1#$PIC#$FILENAME.$FILENAME.$NEXT.$piccount.jpg#$NAME#0"
				fi
				echo "$LINE" >> $TMP/$FILENAME.list
			fi
	
		done 3<$TMP/cache.$FILENAME.2
		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi
	echo "$TMP/$FILENAME.list"
}

# comment block1 start
<<"COMMENT"

						<ul class="nf-videos videos search-video-thumbs">
														<li class="videoblock videoBox" id="33423702" _vkey="127170590" >
<div class="wrap">
.
.
 <div class="phimage">
 <div class="preloadLine"></div>
 <a href="/view_video.php?viewkey=127170590" title="Young-Devotion - Verdammt! Ist mir das jetzt wirklich passiert 17.02.14" class="img" data-related-url="/video/ajax_related_video?vkey=127170590" >
 <div class="img videoPreviewBg">
 <div class="marker-overlays">
 <var class="duration">6:06</var>
 <span class="hd-thumbnail">HD</span>
 </div>

 <img
 src="http://cdn1b.static.pornhub.phncdn.com/www-static/images/blank.gif"
 alt="Young-Devotion - Verdammt! Ist mir das jetzt wirklich passiert 17.02.14"
 data-mediumthumb="http://i0.cdn2b.image.pornhub.phncdn.com/videos/201410/14/33423702/original/(m=ecuK8daaaa)5.jpg"
 data-mediabook="http://cdn2b.video.pornhub.phncdn.com/videos/201410/14/33423702/180P_375K_33423702.webm?rs=150&ri=1000&s=1488442497&e=1488449697&h=46dd89a4b2712a5e40a7f065ebf157ad"
 class="js-preload js-videoThumb js-videoThumbFlip thumb js-videoPreview"
 width="150"
.
.
							</ul>

root@TitanNit-dm900:~# time /tmp/localparser/pornhub.sh /tmp/localparser/pornhub.sh searchold '/video?c=95&page=' 1
/tmp/localcache/pornhub.searchold.video.c.95.page.1.list

real    0m2.128s
user    0m0.625s
sys     0m1.452s

root@TitanNit-dm900:~# time /tmp/localparser/pornhub.sh /tmp/localparser/pornhub.sh search '/video?c=95&page=' 1
/tmp/localcache/pornhub.search.video.c.95.page.1.list

real    0m0.495s
user    0m0.090s
sys     0m0.114s

COMMENT
# comment block1 end

#https://de.pornhub.com/pornstar/lisa-ann&page=1
#https://de.pornhub.com/pornstar/lisa-ann?page=2


#https://de.pornhub.com/video/search?search=billion&page=1

search()
{
#echo $URL$PAGE$NEXT
#$curlbin -vo - $URL$PAGE$NEXT > /mnt/parser/7777
#$curlbin -vo - $URL$PAGE$NEXT > /tmp/localparser/6666
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - $URL$PAGE$NEXT | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT \
		'
			# BEGIN variable setzen
			BEGIN \
				{
					# setzt suchvariable auf 0 vor dem start
					suche = 1
					newpage = ""
					pages = "0"
					piccount = 0
				}
				#<li class="page_next"><a href="/video/search?search=billions&amp;page=2" class="orangeButton">Next</a></li>
				/class=\"page_next222222\">/ \
				{
#				print "88888888822" $0

					# da 2 pages sources geht keine variable ob schon gesetzt.
#					if (pages == "0" && $0 ~ /page=/)
					if ($0 ~ /page=/)
					{
#						pagesold = pages

						# extrahiere die max pages unter 10 pages
						i = index($0, "page=") + 5
			            j = index(substr($0, i), "\"") - 1
			            pages = substr($0, i, j)
#			            print "pages" pages

#						if (pagesold != 0 && pagesold > pages)
#							pages = pagesold

			            # in naechste zeile springen
						next
					}
				}
				# <li class="page_next_set"><a class="greyButton" href="/video?c=95&amp;page=10">10</a></li>
				/class=\"page_next_set\">/ \
				{
#				print "aaaaaaaaaaaa" $0

					# da 2 pages sources geht keine variable ob schon gesetzt.
#					if (pages == "0" && $0 ~ /page=/)
					if ($0 ~ /page=/)
					{
#						pagesold = pages
						# extrahiere die max pages unter 10 pages
#						i = index($0, "&amp;page=") + 10
						i = index($0, "page=") + 5
			            j = index(substr($0, i), "\"") - 1
			            pages = substr($0, i, j)
#						if (pagesold != 0 && pagesold > pages)
#							pages = pagesold

#			            print "pages1: " pages
			            # in naechste zeile springen
						next
					}
				}
				# <li class="page_number"><a class="greyButton" href="/video/search?search=michaela&amp;page=5">5</a></li>
				/class=\"page_number\">/ \
				{
#				print "bbbbbbbbbbb" $0

					# da 2 pages sources geht keine variable ob schon gesetzt.
#					if (pages == "0" && $0 ~ /page=/)
					if ($0 ~ /page=/)
					{
#						pagesold = pages
						# extrahiere die max pages groesser 10 pages
						i = index($0, "page=") + 5
			            j = index(substr($0, i), "\"") - 1
			            pages = substr($0, i, j)

#						if (pagesold != 0 && pagesold > pages)
#							pages = pagesold
							
#				print "pages2: " pages

						# in naechste zeile springen
						next
					}
				}
				# eindeutige zeile vor ersten treffer
				/<ul class=\"nf-videos videos search-video-thumbs\">/ \
				{
#				print "aaaaaa" $0

					# suche erlauben ab dieser zeile
					suche = 1
					# in naechste zeile springen
					next
				}
				/<ul class=\"videos row-5-thumbs search-video-thumbs\">/ \
				{
#				print "bbbbbb" $0

					# suche erlauben ab dieser zeile
					suche = 1
					# in naechste zeile springen
					next
				}
				/<ul class=\"dropdownHottestVideos videos\">/ \
				{
#				print "cccccc" $0

					# suche erlauben ab dieser zeile
					suche = 1
					# in naechste zeile springen
					next
				}
				# eindeutige zeile nach letzen treffer
				/<\/ul>/ \
				{
					# suche verbieten ab dieser zeile
			#		suche = 0
					# in naechste zeile springen
		        	next
				}
				# eindeutige zeile nach letzen treffer backup fals erste nicht klappt
				/<ul class=\"searchRelatedList sectionContent clearfix\">/ \
				{
					# suche verbieten ab dieser zeile
					suche = 0
					# in naechste zeile springen
		            next
				}
				# nextpage zeile
				# <a href="/view_video.php?viewkey=127170590" title="Young-Devotion - Verdammt! Ist mir das jetzt wirklich passiert 17.02.14" class="img" data-related-url="/video/ajax_related_video?vkey=127170590" >
				/<a href=\"\/view_video.php?/ \
				{
#print "ddddd" $0
#print "suche" suche

					if (suche == 1)
					{
						# extrahiere den newpage pfad
						i = index($0, "href=\"") + 6
			            j = index(substr($0, i), "\"") - 1
						# newpage = /view_video.php?viewkey=127170590
			            newpage = substr($0, i, j)
#print "newpage" newpage
	
						# <img class="js-menuSwap" data-image="http://cdn1b.static.pornhub.phncdn.com/images/categories/118x88/28.jpg?cache=1488300184" width="118" height="88" alt="Reife Frauen">
						# extrahiere den titel title="Young-Devotion - Verdammt! Ist mir das jetzt wirklich passiert 17.02.14"
						i = index($0, "title=\"") + 7
			            j = index(substr($0, i), "\"") - 1
						# title = "Young-Devotion - Verdammt! Ist mir das jetzt wirklich passiert 17.02.14"
			            title = substr($0, i, j)
#print "title" title

						# in naechste zeile springen
						next
					}
				}
				# bildlink treffer
#data-image
				# data-mediumthumb="http://i0.cdn2b.image.pornhub.phncdn.com/videos/201702/17/106465292/original/....
				/data-mediumthumb=/ \
				{
					if (suche == 1 && newpage != "")
					{
						# extrahiere den piclink data-image="http://i0.cdn2b.image.pornhub.phncdn.com/videos/201702/17/106465292/original/............
						i = index($0, "data-mediumthumb=\"") + 18
			            j = index(substr($0, i), "\"") - 1
						# pic = http://i0.cdn2b.image.pornhub.phncdn.com/videos/201702/17/106465292/original/........
			            pic = substr($0, i, j)

						if ( pic == "" )
						{
			            	pic = "http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
						}

						if (title != "")
						{
							piccount += 1
							# in naechste zeile springen
							# \x27 = single quotes
							print title "#" SRC " " SRC " hoster \x27" newpage "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#111"
						}
		
						# 27. reset variables
						newpage = ""
						title = ""
						picname = ""
						pic = ""
						# 28. in naechste zeile springen
						next
		         	}
				}
				# next page init
			END \
				{
#				print "pages3: " pages
#				print "NEXT + 1: " NEXT + 1

					if (pages != "0")# && pages >= NEXT + 1)
						print "Page (" NEXT + 1 "/" pages ")#" SRC " " SRC " " INPUT " \x27" PAGE "\x27 " NEXT + 1 "#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#" NAME "#0"
				}
		# 29. schreibe alles in die list datei
		' >$TMP/$FILENAME.list
	fi
	# 30. gebe titan den list namen mit pfad zurueck
	echo "$TMP/$FILENAME.list"
}

searchold()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		piccount=0

		$curlbin $URL/$PAGE$NEXT -o $TMP/cache.$FILENAME.1
		cat $TMP/cache.$FILENAME.1 | tr '\n' ' ' | sed 's/<a href=/\n\n\nfound=/g' | grep ^found= | grep "/view_video.php?viewkey=" | tr '\t' ' ' | sed 's/ \+/ /g' | grep "img src" >$TMP/cache.$FILENAME.2
	
#		<li class="page_next_set"><a class="greyButton" href="/video?c=95&amp;page=10">10</a></li>
		pages=`cat $TMP/cache.$FILENAME.1 | grep "&amp;page" | grep "page_next_set" | sed 's/&amp;page=/\nfound="/g' | grep ^found= | cut -d'"' -f2 | tail -n1`

#		<li class="page_number"><a class="greyButton" href="/video/search?search=michaela&amp;page=5">5</a></li>
		if [ -z "$pages" ];then
			pages=`cat $TMP/cache.$FILENAME.1 | grep "page_number" | sed 's/&amp;page=/\nfound="/g' | grep ^found= | cut -d'"' -f2 | tail -n1`
		fi

		while read -u 3 ROUND; do
			PIC=`echo $ROUND | sed 's!data-mediumthumb=!\npic=!g' | grep ^pic= | cut -d'"' -f2 | tail -n1`
#			if [ -z "$PIC" ];then
#				PIC=`echo $ROUND | sed 's!data-path=!\npic=!g' | grep ^pic= | cut -d'"' -f2 | tail -n1`
#			fi

			TITLE=`echo $ROUND | sed 's!title=!\ntitle=!g' | grep ^title= | cut -d'"' -f2`
			NEWPAGE=`echo $ROUND | cut -d'"' -f2`	
			TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/%28/(/g' -e 's/%29/)/g' -e 's/%3A/:/g' -e 's/%40/@/g' -e 's/%2B/&/g' -e 's/%C3/A/g' -e 's/%B1/&/g' -e 's/%5B//g' -e 's/%5D//g' -e 's!%2F!/!g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
	
			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ] && [ ! -z "$PIC" ];then
				if [ ! -e $TMP/$FILENAME.list ];then
					touch $TMP/$FILENAME.list
				fi
				piccount=`expr $piccount + 1`

				if [ `cat $TMP/$FILENAME.list | grep "#$NEWPAGE#" | wc -l` -eq 0 ];then
					LINE="$TITLE#$SRC $SRC hoster '$NEWPAGE'#$PIC#$FILENAME.$FILENAME.$NEXT.$piccount.jpg#$NAME#111"
				fi
				echo "$LINE" >> $TMP/$FILENAME.list
			fi
	
		done 3<$TMP/cache.$FILENAME.2

		if [ "$NEXT" -lt "$pages" ]; then
			NEXTPAGE=`expr $NEXT + 1`
			LINE="Page ($NEXTPAGE/$pages)#$SRC $SRC search '$PAGE' $NEXTPAGE#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#$NAME#0"
			echo "$LINE" >> $TMP/$FILENAME.list
		fi
		rm $TMP/cache.$FILENAME.* > /dev/null 2>&1
	fi
	echo "$TMP/$FILENAME.list"
}

# comment block1 start
<<"COMMENT"
	<div class="wrap">
		<div class="subscribe-to-pornstar-icon display-none">
			<button type="button" data-title="bei Pornostar anmelden" class="tooltipTrig" onclick="return false;" ><span></span></button>
		</div>
		<a class="js-mxp" data-mxptype="Pornstar" data-mxptext="Angela White" href="/pornstar/angela-white">
									<span class="pornstar_label">
				<span class="title-album">Rangordnung:
					<span class="rank_number">
						44					</span>

					<span class='icon rank-up'></span>				</span>
			</span>
			<img src="https://ci.phncdn.com/pics/pornstars/000/005/553/(m=lciyeNbOb_c)(mh=uncWnVAyet2L8iyD)thumb_21641.jpg" alt="Angela White"/>
		</a>
		<div class="thumbnail-info-wrapper">
			<a href="/pornstar/angela-white" class="title js-mxp" data-mxptype="Pornstar" data-mxptext="Angela White" >Angela White</a>
			<span class="videosNumber">156 Videos			8M Aufrufe </span>
		</div>
	</div>
COMMENT
# comment block1 end

pornstars()
{
#echo $URL$PAGE$NEXT
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - $URL$PAGE$NEXT | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT \
		'
			# BEGIN variable setzen
			BEGIN \
				{
					# setzt suchvariable auf 0 vor dem start
					suche = 0
					newpage = ""
					pages = "0"
					piccount = 0
				}
				# <li class="page_next_set"><a class="greyButton" href="/video?c=95&amp;page=10">10</a></li>
				/class=\"page_next_set\">/ \
				{
#				print "888888888" $0

					# da 2 pages sources geht keine variable ob schon gesetzt.
					#if (pages == "0")
					#{
						# extrahiere die max pages unter 10 pages
#						i = index($0, "&amp;page=") + 10
						i = index($0, "page=") + 5
			            j = index(substr($0, i), "\"") - 1
			            pages = substr($0, i, j)
#			            print "pages" pages
			            # in naechste zeile springen
						next
					#}
				}
				# <li class="page_number"><a class="greyButton" href="/video/search?search=michaela&amp;page=5">5</a></li>
				/class=\"greyButton\">/ \
				{
#				print "77777777777" $0

					# da 2 pages sources geht keine variable ob schon gesetzt.
					#if (pages == "0")
					#{
						# extrahiere die max pages groesser 10 pages
						i = index($0, "page=") + 5
			            j = index(substr($0, i), "\"") - 1
			            pages = substr($0, i, j)
#				print "pages" pages

						# in naechste zeile springen
						next
					#}
				}
				# eindeutige zeile vor ersten treffer
				/<ul class=\"videos row-5-thumbs popular-pornstar/ \
				{
#				print "000000000000"

					# suche erlauben ab dieser zeile
					suche = 1
					# in naechste zeile springen
					next
				}
				# eindeutige zeile nach letzen treffer
				/<\/ul>/ \
				{
					# suche verbieten ab dieser zeile
					suche = 0
					# in naechste zeile springen
		        	next
				}
				# eindeutige zeile nach letzen treffer backup fals erste nicht klappt
				/<div class="pagination3\">/ \
				{
					# suche verbieten ab dieser zeile
					suche = 0
					# in naechste zeile springen
		            next
				}
				# nextpage zeile
				# <a href="/view_video.php?viewkey=127170590" title="Young-Devotion - Verdammt! Ist mir das jetzt wirklich passiert 17.02.14" class="img" data-related-url="/video/ajax_related_video?vkey=127170590" >
				/<a href=\"\/pornstar\// \
				{
#				print "33333333333"
					if (suche == 1)
					{
#				print "44444444444"

						# extrahiere den newpage pfad
						i = index($0, "href=\"") + 6
			            j = index(substr($0, i), "\"") - 1
						# newpage = /view_video.php?viewkey=127170590
			            newpage = substr($0, i, j)
#				print "newpage: " newpage

						next
					}
				}
				# bildlink treffer
				# data-mediumthumb="http://i0.cdn2b.image.pornhub.phncdn.com/videos/201702/17/106465292/original/....
				/<img src=/ \
				{
#				print "11111111111"

#					if (suche == 1 && newpage != "")
					if (suche == 1)
					{
#				print "222222222222"

						# extrahiere den piclink data-image="http://i0.cdn2b.image.pornhub.phncdn.com/videos/201702/17/106465292/original/............
						i = index($0, "<img src=\"") + 10
			            j = index(substr($0, i), "\"") - 1
						# pic = http://i0.cdn2b.image.pornhub.phncdn.com/videos/201702/17/106465292/original/........
			            pic = substr($0, i, j)

						if ( pic == "" )
						{
			            	pic = "http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
						}
#				print "pic: " pic


						# <img class="js-menuSwap" data-image="http://cdn1b.static.pornhub.phncdn.com/images/categories/118x88/28.jpg?cache=1488300184" width="118" height="88" alt="Reife Frauen">
						# extrahiere den titel title="Young-Devotion - Verdammt! Ist mir das jetzt wirklich passiert 17.02.14"
						i = index($0, "alt=\"") + 5
			            j = index(substr($0, i), "\"") - 1
						# title = "Young-Devotion - Verdammt! Ist mir das jetzt wirklich passiert 17.02.14"
			            title = substr($0, i, j)
						# in naechste zeile springen
#				print "title: " title

#						if (title != "")
#						{
#							piccount += 1
#							# in naechste zeile springen
#							# \x27 = single quotes
#							print title "#" SRC " " SRC " play \x27" newpage "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#111"
#						}
#		
#						# 27. reset variables
#						newpage = ""
#						title = ""
#						picname = ""
#						pic = ""
#						# 28. in naechste zeile springen
#						next
		         	}
				}
				/<span class=\"videosNumber\">/ \
				{
#				print "55555555555"


					if (suche == 1 && newpage != "")
					{
#				print "66666666666"


						i = index($0, "<span class=\"videosNumber\">") + 28
			            j = index(substr($0, i), "</span>") - 1
						# <span class="videosNumber">1074 Videos			40M Aufrufe </span>
			            extra = substr($0, i, j)
						# in naechste zeile springen

						# trim left "  Hallo   tester   "
						gsub(/^[ \t]+/,"",extra)
						# trim right "  Hallo   tester   "
						gsub(/[ \t]+$/,"",extra)
						# trim middle "  Hallo   tester   "
						gsub(/\t+/," / ",extra)

#				print "extra: " extra

						if (title != "")
						{
							piccount += 1
							# in naechste zeile springen
							# \x27 = single quotes
#							print title " (" extra ")#" SRC " " SRC " play \x27" newpage "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#111"
							print title " (" extra ")#" SRC " " SRC " search \x27" newpage "?page=\x27 1#" pic "#" PICNAME "." piccount ".jpg#" NAME "#0"

						}
		
						# 27. reset variables
						newpage = ""
						title = ""
						picname = ""
						pic = ""
						# 28. in naechste zeile springen
						next
		         	}
				}				
				# next page init
			END \
				{
					print "Page (" NEXT + 1 "/" pages ")#" SRC " " SRC " " INPUT " \x27" PAGE "\x27 " NEXT + 1 "#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#" NAME "#0"
				}
		# 29. schreibe alles in die list datei
		' >$TMP/$FILENAME.list
	fi
	# 30. gebe titan den list namen mit pfad zurueck
	echo "$TMP/$FILENAME.list"
}

play()
{
	rm $TMP/cache.$PARSER.$INPUT.* > /dev/null 2>&1
	/tmp/localhoster/hoster.sh youtube_dl $URL$PAGE > $TMP/cache.$PARSER.$INPUT.1
	STREAMURL=`cat $TMP/cache.$PARSER.$INPUT.1`
	cat $STREAMURL
#	echo $URL$PAGE
}

hoster()
{
	echo $URL$PAGE
}

case $INPUT in
	init) $INPUT;;
	mainmenu) $INPUT;;
	play) $INPUT;;
	search) $INPUT;;
	searchold) $INPUT;;
	genre) $INPUT;;
	genreold) $INPUT;;
	pornstars) $INPUT;;
	hoster) $INPUT;;
esac
