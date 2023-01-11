#!/bin/bash
# box parser for titannit

SRC=$1
INPUT=$2
PAGE=$3
NEXT=$4
PAGE2=$5
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`

NAME=`echo -n ${PARSER:0:1} | tr '[a-z]' '[A-Z]'`${PARSER:1}
#NAME="XVIDEOS"
URL="https://xvideos.com"

#https://api-search.xvideos.best/id/35443907/?format=ajax
#https://api-search.xvideos.best/id/35443907/?format=json

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

if [ `echo $SRC | grep ^"/mnt/parser" |wc -l` -gt 0 ];then
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
	echo "$NAME ($TYPE)#$SRC $SRC mainmenu#http://openaaf.dyndns.tv/mediathek/menu/$PARSER.jpg#$PARSER.jpg#TiThek#1000"
}

mainmenu()
{
   	echo "Sorted by date#$SRC $SRC search '/new/' 0#http://openaaf.dyndns.tv/mediathek/menu/all-newfirst.jpg#all-newfirst.jpg#$NAME#0" >$TMP/$FILENAME.list
    echo "All Tags#$SRC $SRC tags '/tags/'#http://openaaf.dyndns.tv/mediathek/menu/category.jpg#category.jpg#$NAME#0" >>$TMP/$FILENAME.list
	echo "Suchen#$SRC $SRC search '/?k=%search%&p=' 0#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >>$TMP/$FILENAME.list
	echo "$TMP/$FILENAME.list"
}

search()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then

		if [ "$PAGE" == "/new/" ] && [ $NEXT == "0" ];then
    		TMPURL="$URL"
		else
		    TMPURL="$URL$PAGE$NEXT$PAGE2"
		fi

		$curlbin -o - "$TMPURL" | sed -e 's/{\"quality\"/{\n\"quality\"/g' | awk -v SRC=$SRC  -v PICNAME=$PICNAME -v PARSER=$PARSER -v NAME=$NAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT -v PAGE2=$PAGE2 \
		'
			BEGIN \
			{
				title = ""
				pic = ""
				image = ""
				pages = "1"
			}
			/<div id=\"video_/ \
			{
				# extrahiere den newpage pfad
				i = index($0, "data-src=\"") + 10
				j = index(substr($0, i), "\"") - 1
				pic = substr($0, i, j)
			#print "pic: " pic

				# extrahiere den newpage pfad
				i = index($0, "title=\"") + 7
				j = index(substr($0, i), "\"") - 1
				title = substr($0, i, j)
			#print "title: " title

				# extrahiere den newpage pfad
				i = index($0, "<p class=\"title\"><a href=\"") + 26
				j = index(substr($0, i), "\"") - 1
				url = substr($0, i, j)
			#print "url: " url

				if (title != "")
				{
		            piccount += 1
		            print title "#" SRC " " SRC " hoster \x27" url "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#111"
		        }

				title = ""
				pic = ""
			}
			/<div class="pagination/ \
			{
				# extrahiere den newpage pfad
				i = index($0, "class=\"last-page\">") + 18
				j = index(substr($0, i), "<") - 1
				pages = substr($0, i, j)
			#print "pages: " pages
			}
			END \
			{
#				if (curpage != pages)
					print "Page (" NEXT + 1 "/" pages ")#" SRC " " SRC " " INPUT " \x27" PAGE "\x27 " NEXT + 1"#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#" NAME "#0"
			}
		' >$TMP/$FILENAME.list
	fi
	echo "$TMP/$FILENAME.list"
}

tags()
{
	if [ ! -e $TMP/$FILENAME.list ]; then
		$curlbin -o - ${URL}$PAGE | grep -v "<strong>All tags</strong>" | sed -e 's!</a>!\n</a>!g' -e 's/{/{\n/g' -e 's/\}/\n\}/g' -e 's/\]/\n\]/g' | awk -v PARAM=$PARAM -v SRC=$SRC -v NAME=$NAME \
		'
			BEGIN \
			{
				table = ""
			}
			/<a href=\"\/tags\// \
			{
				split($0, a, "\"")
				tag = a[2]
				#print "00000: " $0
				#print "22222: " tag
				if (tag != "" && tag != "tags-list")
				{
				   title = toupper(tag)
				   gsub(/\/TAGS\//, "", title)
				   pic = tolower(title)
				   print title "#" SRC " " SRC " search \x27" tag "/\x27 0#http://openaaf.dyndns.tv/mediathek/menu/" pic ".jpg#" pic ".jpg#" NAME "#0"
				}
			}
		' >$TMP/$FILENAME.list
	fi
	echo $TMP/$FILENAME.list
}

hoster()
{
	echo $URL$PAGE
}

case $INPUT in
	init) $INPUT;;
	mainmenu) $INPUT;;
	hoster) $INPUT;;
	tags) $INPUT;;
	search) $INPUT;;
esac

