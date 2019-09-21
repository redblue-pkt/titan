#!/bin/sh
# first sh box parser for titannit mfg obi

case $2 in
	init) echo skip load hoster.sh;;
	*) . /tmp/localhoster/hoster.sh;;
esac

#SRC=$1
#INPUT=$2
#PAGE=$3
#NEXT=$4
#
#FILENAME=`echo $PAGE | tr '/' '.'`
#FILENAME=`echo $FILENAME | tr '&' '.'`
#
#if [ -z "$PAGE" ]; then
#	FILENAME=none
#fi

SRC=$1
INPUT=$2
PAGE=$3
NEXT=$4
PAGE2=$5
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`

FILENAME="$PARSER $INPUT $PAGE $NEXT $PAGE2"
FILENAME=`echo $FILENAME | tr '&' '.' | tr '/' '.' | tr '?' '.'  | tr '=' '.' | sed 's/ \+/./g' | sed 's/\.\+/./g'`

if [ -z "$FILENAME" ]; then
	FILENAME=none
fi

# curl https://kinox.su/779-meine-teuflisch-gute-freundin-2018.

URL=https://kinox.su/

PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME=KinoXsu

mkdir $TMP > /dev/null 2>&1

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
	echo "$NAME ($TYPE)#$SRC $SRC mainmenu#http://atemio.dyndns.tv/mediathek/menu/$PARSER.jpg#$PARSER.jpg#TiThek#0"
}

mainmenu()
{
	echo "Suchen#$SRC $SRC search 'do=search&subaction=search&story=%search%' 1#http://atemio.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" > $TMP/$FILENAME.list
	echo "$TMP/$FILENAME.list"
}

search()
{
	#rm "$TMP/$FILENAME.list"
	#data="do=search&subaction=search&story=terminator"
	#$curlbin -X POST --data "$data" --referer "$URL" "$URL/$PAGE" -o "$TMP/cache.$FILENAME.1"

	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - -X POST --data "$PAGE" $URL$NEXT | awk -v URL=$URL -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT \
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
				#<div class="zizizi2">
				# HD 720
				#</div>
				/<div class=\"zizizi2\">/ \
				{
					searchtype = 1
					next
				}
				/^ / \
				{
					if (searchtype == 1)
					{
						type = $0
#						print "type" type
						searchtype=0
					}
					next
				}
				#<span class="plovkaz"><a href="https://kinox.su/1003-zombieland-2009.html">Zombieland (2009)</a></span><span class="zojchh"></span>    
				/<span class=\"plovkaz\">/ \
				{
#					print "88888888822" $0

					i = index($0, "a href=\"") + 8
					j = index(substr($0, i), "\"") - 1
			            	newpage = substr($0, i, j)
#			            	print "newpage" newpage

					i = index($0, ".html\">") + 7
				        j = index(substr($0, i), "<") - 1
			            	title = substr($0, i, j)
#			            	print "title" title	
					next
				}
				#<span class="kvittaz"><img src="/uploads/posts/2018-02/1519582125_750dbb4faa45.jpg" alt="Zombieland (2009) online stream KinoX"></span>
				/<span class=\"kvittaz\">/ \
				{
#					print "88888888822" $0

					i = index($0, "img src=\"") + 9
					j = index(substr($0, i), "\"") - 1
			            	pic = substr($0, i, j)
#			            	print "pic" pic

					if ( pic == "" )
					{
			            		pic = "http://atemio.dyndns.tv/mediathek/menu/default.jpg"
					}

					if (title != "")
					{
						piccount += 1
						# in naechste zeile springen
						# \x27 = single quotes
						gsub(/^ /, "", type)
						print title " (" type ")#" SRC " " SRC " play \x27" newpage "\x27#" URL pic "#" PICNAME "." piccount ".jpg#" NAME "#111"
					}
	
					# 27. reset variables
					newpage = ""
					title = ""
					picname = ""
					pic = ""
					# 28. in naechste zeile springen
					next

				}
			END
				{
#				print "pages3: " pages
#				print "NEXT + 1: " NEXT + 1

					if (pages != "0")# && pages >= NEXT + 1)
						print "Page (" NEXT + 1 "/" pages ")#" SRC " " SRC " " INPUT " \x27" PAGE "\x27 " NEXT + 1 "#http://atemio.dyndns.tv/mediathek/menu/next.jpg#next.jpg#" NAME "#0"
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
	#/mnt/parser/kinoxsu.sh /mnt/parser/kinoxsu.sh hosterlist 'https://kinox.su/262-terminator-genisys-2015.html'

	rm "$TMP/$FILENAME.list"
	$curlbin "$PAGE" -o "$TMP/cache.$FILENAME.1"

	#<iframe src="https://verystream.com/e/CiamZWPYHZt/" width="640" height="360" allowfullscreen="true" webkitallowfullscreen="true" mozallowfullscreen="true" scrolling="no" frameborder="0"></iframe>

	URL=`cat "$TMP/cache.$FILENAME.1" | grep iframe | sed -nr 's/.*src="([^"]+)".*/\1/p'`

	/tmp/localhoster/hoster.sh youtube_dl $URL > $TMP/cache.$PARSER.$INPUT.1
	STREAMURL=`cat $TMP/cache.$PARSER.$INPUT.1`
	cat $STREAMURL
}


case $INPUT in
	init) $INPUT;;
	mainmenu) $INPUT;;
	search) $INPUT;;
	play) $INPUT;;
esac
