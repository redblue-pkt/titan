#!/bin/sh
# first sh box parser for titannit mfg obi

SRC=$1
INPUT=$2
PAGE=$3
NEXT=$4

PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`

URL=http://openloadmovie.co
#PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME=OpenloadMovie

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
	echo "$NAME ($TYPE)#$SRC $SRC mainmenu#http://openaaf.dyndns.tv/mediathek/menu/$PARSER.jpg#$PARSER.jpg#TiThek#0"
}

mainmenu()
{
	echo "Movies#$SRC $SRC search 'movies/page/' 1#http://openaaf.dyndns.tv/mediathek/menu/Movies.jpg#movies.jpg#$NAME#0" > $TMP/$FILENAME.list
	echo "$TMP/$FILENAME.list"
} 

search()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - $URL/$PAGE/$NEXT/ | tr '\n' ' ' | sed 's/<div class="poster"/\n<div class="poster"/g' | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT \
		'
			# BEGIN variable setzen
			BEGIN \
			{
				# setzt suchvariable auf 0 vor dem start
				piccount = 0
				pages = "1"
			}
			/<div class=\"pagination\">/ \
			{
				i = index($0, "<div class=\"pagination\"><span>") + 30
	            j = index(substr($0, i), "</span>") - 1
				pagetext = substr($0, i, j)

#				i = index($pagetext, "Page ") + 5
#	            j = index(substr($pagetext, i), " of ") - 1
#				curpage = substr($pagetext, i, j)

#				i = index($pagetext, " of ") + 4
#	            j = index(substr($pagetext, i), "") - 1
#				pages = substr($pagetext, i, j)

				split(pagetext, a, " ")
				curpage = a[2]
				pages = a[4]

				next
			}
			/<div class=\"poster\"/ \
			{
				i = index($0, "<a href=\"") + 9
	            j = index(substr($0, i), "\"") - 1
				newpage = substr($0, i, j)
				gsub("http://openloadmovie.co/", "", newpage)
				gsub("https://openloadmovie.co/", "", newpage)
				gsub("http://openloadmovie.me/", "", newpage)
				gsub("https://openloadmovie.me/", "", newpage)

				i = index($0, "<img src=\"") + 10
	            j = index(substr($0, i), "\"") - 1
				pic = substr($0, i, j)

				i = index($0, "alt=\"") + 5
	            j = index(substr($0, i), "\"") - 1
				title = substr($0, i, j)
				gsub(" Openload Movies", "", title)


				piccount += 1
				if ( pic == "" )
				{
	            	pic = "http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
				}
				print title "#" SRC " " SRC " hoster \x27" newpage "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#111"
				next
			}
			END
			{
				if (curpage != pages)
					print "Page (" NEXT + 1 "/" pages ")#" SRC " " SRC " " INPUT " \x27" PAGE "\x27 " NEXT + 1 "#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#" NAME "#0"
			}
		# schreibe alles in die list datei
		' >$TMP/$FILENAME.list
	fi
	# gebe titan den list namen mit pfad zurueck
	echo "$TMP/$FILENAME.list"
}

hoster()
{
	#<iframe class="metaframe rptss" src="https://openload.co/embed/5EhZ1cmxtvE/Rangoon_%282017%29_DVDRip.mp4" frameborder="0" allowfullscreen></iframe></div> </div><div class="control"><nav class="player"><ul class="options"><li><a><i class="icon-menu"></i> <b>Options</b></a><ul class="idTabs"><li><a class="options" href="#option-1">Openload </a></li> </ul></li></ul></nav><span class="qualityx">DVD</span><nav class="controles"><ul class="list"><li><a class="lightSwitcher" href="javascript:void(0);"><i class="icon-wb_sunny"></i></a></li></ul></nav></div></div><script type="text/javascript">$(document).ready(function(){$("#oscuridad").css("height", $(document).height()).hide();$(".lightSwitcher").click(function(){$("#oscuridad").toggle();if ($("#oscuridad").is(":hidden"))
	#<iframe class="metaframe rptss" src="data:image/gif;base64,R0lGODdhAQABAPAAAP///wAAACwAAAAAAQABAEACAkQBADs=" data-lazy-src="https://openload.co/embed/-eH6UStG_Ok/Kong%3A_Skull_Island_%282017%29.mp4" frameborder="0" allowfullscreen></iframe>
	if [ ! -e "$TMP/$FILENAME.list" ]; then

#<tbody>
#<tr id="mov5lQZpS522393">
#<td><a class="link_a" href="https://openloadmovie.org/links/mov5lqzps522393/" target="_blank">Download</a></td>
#<td><img src="https://plus.google.com/_/favicon?domain=openload.co"> openload.co</td>
#<td>WEB</td>
#<td>English</td>
#<td>3 days </td>
#</tr>
#<tbody>

#		$curlbin -o - $URL/$PAGE/ | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT \
		$curlbin -o - $PAGE/ | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT \
		'
			# BEGIN variable setzen
			BEGIN \
			{
				# setzt suchvariable auf 0 vor dem start
				piccount = 0
				pages = "1"
			}
			/<iframe class=/ \
			{
				i = index($0, "data-lazy-src=\"") + 15
	            		j = index(substr($0, i), "\"") - 1
				newpage = substr($0, i, j)

				if(newpage ~ /\(data\) \{alert\(/)
				{
					i = index($0, "src=\"") + 5
		            		j = index(substr($0, i), "\"") - 1
					newpage = substr($0, i, j)
				}
				print newpage
				next
			}
		# schreibe alles in die list datei
		' >$TMP/$FILENAME.list
	fi
	# gebe titan den list namen mit pfad zurueck
	cat "$TMP/$FILENAME.list"
}

case $INPUT in
#	init) $INPUT;;
	mainmenu) $INPUT;;
	search) $INPUT;;
	hoster) $INPUT;;
esac
