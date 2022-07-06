#!/bin/sh
# first sh box parser for titannit mfg obi

SRC=$1
INPUT=$2
PAGE=$3
NEXT=$4
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`

URL=http://www.pornhub.com
NAME="PornHub"

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
	echo "Pornstars#$SRC $SRC pornstars '/pornstars?page=' 1#http://openaaf.dyndns.tv/mediathek/menu/pornstars.jpg#pornstars.jpg#$NAME#0" >>$TMP/$FILENAME.list
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
				suche = 1
				newpage = ""
				piccount = 0
			}
            #<div class="checkHomepage clearfix"><a class="sidebarIndent js-mxp" data-mxptype="Category" data-mxptext="Amateur" href="/video?c=3">
            /<div class=\"checkHomepage clearfix\">/ \
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

                    #data-mxptext="Pissing"
					i = index($0, "data-mxptext=\"") + 14
		            j = index(substr($0, i), "\"") - 1
					# 18. titel = Reife Frauen
		            title = substr($0, i, j)
	
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
							
						# in naechste zeile springen
						next
					}
				}
				# eindeutige zeile vor ersten treffer
				/<ul class=\"nf-videos videos search-video-thumbs\">/ \
				{
					# suche erlauben ab dieser zeile
					suche = 1
					# in naechste zeile springen
					next
				}
				/<ul class=\"videos row-5-thumbs search-video-thumbs\">/ \
				{
					# suche erlauben ab dieser zeile
					suche = 1
					# in naechste zeile springen
					next
				}
				/<ul class=\"dropdownHottestVideos videos\">/ \
				{
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
					if (suche == 1)
					{
						# extrahiere den newpage pfad
						i = index($0, "href=\"") + 6
			            j = index(substr($0, i), "\"") - 1
						# newpage = /view_video.php?viewkey=127170590
			            newpage = substr($0, i, j)
	
						# <img class="js-menuSwap" data-image="http://cdn1b.static.pornhub.phncdn.com/images/categories/118x88/28.jpg?cache=1488300184" width="118" height="88" alt="Reife Frauen">
						# extrahiere den titel title="Young-Devotion - Verdammt! Ist mir das jetzt wirklich passiert 17.02.14"
						i = index($0, "title=\"") + 7
			            j = index(substr($0, i), "\"") - 1
						# title = "Young-Devotion - Verdammt! Ist mir das jetzt wirklich passiert 17.02.14"
			            title = substr($0, i, j)
                        gsub(/&#.*;/," ",title)

						# in naechste zeile springen
						next
					}
				}
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
					if (pages != "0")# && pages >= NEXT + 1)
						print "Page (" NEXT + 1 "/" pages ")#" SRC " " SRC " " INPUT " \x27" PAGE "\x27 " NEXT + 1 "#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#" NAME "#0"
				}
		# 29. schreibe alles in die list datei
		' >$TMP/$FILENAME.list
	fi
	# 30. gebe titan den list namen mit pfad zurueck
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
				# <li class="page_next_set"><a class="greyButton" href="/video?c=95&amp;page=10">10</a></li>
				/class=\"page_next_set\">/ \
				{
						# extrahiere die max pages unter 10 pages
						i = index($0, "page=") + 5
			            j = index(substr($0, i), "\"") - 1
			            pages = substr($0, i, j)
			            print "pages" pages
			            # in naechste zeile springen
						next
				}
				# <li class="page_number"><a class="greyButton" href="/video/search?search=michaela&amp;page=5">5</a></li>
				/class=\"greyButton\">/ \
				{
						# extrahiere die max pages groesser 10 pages
						i = index($0, "page=") + 5
			            j = index(substr($0, i), "\"") - 1
			            pages = substr($0, i, j)
				print "pages2" pages

						# in naechste zeile springen
						next
				}
                #<a class="js-mxp" data-mxptype="Pornstar" data-mxptext="Leolulu" href="/pornstar/leolulu">
				/<a class=\"js-mxp\" data-mxptype=\"Pornstar\"/ \
				{
					if (suche == 1)
					{
						# extrahiere den newpage pfad
						i = index($0, "href=\"") + 6
			            j = index(substr($0, i), "\"") - 1
			            newpage = substr($0, i, j)

						i = index($0, "data-mxptext=\"") + 14
			            j = index(substr($0, i), "\"") - 1
			            title = substr($0, i, j)
						next
					}
				}
#				data-thumb_url="https://di.phncdn.com/pics/users/229/015/532/avatar1632735588/(m=eQJ6GCjadOf)(mh=ptGd9vVV0JdzLQFH)200x200.jpg"
				/data-thumb_url/ \
				{
					if (suche == 1)
					{
						# extrahiere den newpage pfad
						i = index($0, "data-thumb_url=\"") + 16
			            j = index(substr($0, i), "\"") - 1
			            pic = substr($0, i, j)
						next
					}
				}
                #<img src="https://di.phncdn.com/pics/users/433/074/032/avatar1551452811/(m=eQJ6GCjadOf)(mh=TT-sZ5VEN7ufxL9W)200x200.jpg" alt="Aidra Fox"/>
				/<img src=/ \
				{
					if (suche == 1 && pic == "")
					{
						# extrahiere den newpage pfad
						i = index($0, "<img src=\"") + 10
			            j = index(substr($0, i), "\"") - 1
			            pic = substr($0, i, j)
						next
					}
				}
				/<span class=\"videosNumber\">/ \
				{
					if (suche == 1 && newpage != "")
					{
						i = index($0, "<span class=\"videosNumber\">") + 28
			            j = index(substr($0, i), "</span>") - 1
						# <span class="videosNumber">1074 Videos			40M Aufrufe </span>
			            extra = substr($0, i, j)
						# in naechste zeile springen

						# trim left "  Hallo   tester   "
						gsub(/^[ \t]+/,"",extra)
						gsub(/[ \t]+$/,"",extra)
						gsub(/\t+/," / ",extra)
						gsub(/ +/," ",extra)

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
	genre) $INPUT;;
	pornstars) $INPUT;;
	hoster) $INPUT;;
esac
