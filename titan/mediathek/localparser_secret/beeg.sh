#!/bin/bash
# box parser for titannit

SRC=$1
INPUT=$2
PAGE=$3
NEXT=$4
PAGE2=$5
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`

if [ -z "$FILENAME" ]; then
	FILENAME=none
fi
NAME=`echo -n ${PARSER:0:1} | tr '[a-z]' '[A-Z]'`${PARSER:1}
URL="https://beeg.com/api/v6/1546225636701/"

case $2 in
	init)	;;
	*) 	. /tmp/localhoster/hoster.sh
	   	mkdir $TMP > /dev/null 2>&1
		FILENAME="$PARSER $INPUT $PAGE $NEXT $PAGE2"
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

#	echo "Kino#$SRC $SRC search 'filme/' 1 '/?order=veroeffentlichung'#http://openaaf.dyndns.tv/mediathek/menu/kino.ger.jpg#kino.ger.jpg#$NAME#0" >$TMP/$FILENAME.list


#	echo "Sorted by date#$SRC $SRC videos index/main/0/pc#http://openaaf.dyndns.tv/mediathek/menu/all-newfirst.jpg#all-newfirst.jpg#$NAME#0" >/tmp/tithek/$PARSER.list
	echo "Sorted by date#$SRC $SRC videos 'index/main/' 0 '/pc'#http://openaaf.dyndns.tv/mediathek/menu/all-newfirst.jpg#all-newfirst.jpg#$NAME#0" >/tmp/tithek/$PARSER.list
#        echo "Popular Tags#$SRC $SRC tags popular#http://openaaf.dyndns.tv/mediathek/menu/category.jpg#category.jpg#$NAME#0" >>/tmp/tithek/$PARSER.list
        echo "All Tags#$SRC $SRC tags nonpopular#http://openaaf.dyndns.tv/mediathek/menu/category.jpg#category.jpg#$NAME#0" >>/tmp/tithek/$PARSER.list
        echo "Pornstar#$SRC $SRC pornstar nonpopular#http://openaaf.dyndns.tv/mediathek/menu/category.jpg#category.jpg#$NAME#0" >>/tmp/tithek/$PARSER.list

#        echo "Long Videos#$SRC $SRC videos index/tag/0/pc?tag=long+videos#http://openaaf.dyndns.tv/mediathek/menu/category.jpg#category.jpg#$NAME#0" >>/tmp/tithek/$PARSER.list
#        echo "Search#$SRC $SRC videos index/search/0/pc?query=#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >>/tmp/tithek/$PARSER.list
	echo "/tmp/tithek/$PARSER.list"
}


#{"quality":null,"title":"Pink panty stuffing and cum","set_id":106551,"full":1,"ps_name":"Jessica Starling","nt_name":null,"svid":1786133462,"duration":425,"datetime":"2020-10-25 19:00:06","thumbs":[{"pid":1219962,"offset":4,"image":"106551-0004.jpg","start":null,"end":null,"crops":{"16x9":{"left":null,"top":null,"width":null,"height":null},"4x3":{"left":null,"top":null,"width":null,"height":null},"3x2":{"left":null,"top":null,"width":null,"height":null},"1x1":{"left":null,"top":null,"width":null,"height":null}}},{"pid":1219963,"offset":46,"image":"106551-0046.jpg","start":null,"end":null,"crops":{"16x9":{"left":null,"top":null,"width":null,"height":null},"4x3":{"left":null,"top":null,"width":null,"height":null},"3x2":{"left":null,"top":null,"width":null,"height":null},"1x1":{"left":null,"top":null,"width":null,"height":null}}},{"pid":1219964,"offset":48,"image":"106551-0048.jpg","start":null,"end":null,"crops":{"16x9":{"left":null,"top":null,"width":null,"height":null},"4x3":{"left":null,"top":null,"width":null,"height":null},"3x2":{"left":null,"top":null,"width":null,"height":null},"1x1":{"left":null,"top":null,"width":null,"height":null}}},{"pid":1219965,"offset":49,"image":"106551-0049.jpg","start":null,"end":null,"crops":{"16x9":{"left":null,"top":null,"width":null,"height":null},"4x3":{"left":null,"top":null,"width":null,"height":null},"3x2":{"left":null,"top":null,"width":null,"height":null},"1x1":{"left":null,"top":null,"width":null,"height":null}}},{"pid":1219966,"offset":52,"image":"106551-0052.jpg","start":null,"end":null,"crops":{"16x9":{"left":null,"top":null,"width":null,"height":null},"4x3":{"left":null,"top":null,"width":null,"height":null},"3x2":{"left":null,"top":null,"width":null,"height":null},"1x1":{"left":null,"top":null,"width":null,"height":null}}},{"pid":1219967,"offset":54,"image":"106551-0054.jpg","start":null,"end":null,"crops":{"16x9":{"left":null,"top":null,"width":null,"height":null},"4x3":{"left":null,"top":null,"width":null,"height":null},"3x2":{"left":null,"top":null,"width":null,"height":null},"1x1":{"left":null,"top":null,"width":null,"height":null}}},{"pid":1219968,"offset":56,"image":"106551-0056.jpg","start":null,"end":null,"crops":{"16x9":{"left":null,"top":null,"width":null,"height":null},"4x3":{"left":null,"top":null,"width":null,"height":null},"3x2":{"left":null,"top":null,"width":null,"height":null},"1x1":{"left":null,"top":null,"width":null,"height":null}}},{"pid":1219969,"offset":57,"image":"106551-0057.jpg","start":null,"end":null,"crops":{"16x9":{"left":null,"top":null,"width":null,"height":null},"4x3":{"left":null,"top":null,"width":null,"height":null},"3x2":{"left":null,"top":null,"width":null,"height":null},"1x1":{"left":null,"top":null,"width":null,"height":null}}},{"pid":1219970,"offset":58,"image":"106551-0058.jpg","start":null,"end":null,"crops":{"16x9":{"left":null,"top":null,"width":null,"height":null},"4x3":{"left":null,"top":null,"width":null,"height":null},"3x2":{"left":null,"top":null,"width":null,"height":null},"1x1":{"left":null,"top":null,"width":null,"height":null}}},{"pid":1219971,"offset":62,"image":"106551-0062.jpg","start":null,"end":null,"crops":{"16x9":{"left":null,"top":null,"width":null,"height":null},"4x3":{"left":null,"top":null,"width":null,"height":null},"3x2":{"left":null,"top":null,"width":null,"height":null},"1x1":{"left":null,"top":null,"width":null,"height":null}}},{"pid":1219972,"offset":72,"image":"106551-0072.jpg","start":null,"end":null,"crops":{"16x9":{"left":null,"top":null,"width":null,"height":null},"4x3":{"left":null,"top":null,"width":null,"height":null},"3x2":{"left":null,"top":null,"width":null,"height":null},"1x1":{"left":null,"top":null,"width":null,"height":null}}},{"pid":1219973,"offset":75,"image":"106551-0075.jpg","start":null,"end":null,"crops":{"16x9":{"left":null,"top":null,"width":null,"height":null},"4x3":{"left":null,"top":null,"width":null,"height":null},"3x2":{"left":null,"top":null,"width":null,"height":null},"1x1":{"left":null,"top":null,"width":null,"height":null}}},{"pid":1219974,"offset":81,"image":"106551-0081.jpg","start":null,"end":null,"crops":{"16x9":{"left":null,"top":null,"width":null,"height":null},"4x3":{"left":null,"top":null,"width":null,"height":null},"3x2":{"left":null,"top":null,"width":null,"height":null},"1x1":{"left":null,"top":null,"width":null,"height":null}}},{"pid":1219975,"offset":84,"image":"106551-0084.jpg","start":null,"end":null,"crops":{"16x9":{"left":null,"top":null,"width":null,"height":null},"4x3":{"left":null,"top":null,"width":null,"height":null},"3x2":{"left":null,"top":null,"width":null,"height":null},"1x1":{"left":null,"top":null,"width":null,"height":null}}},{"pid":1219976,"offset":86,"image":"106551-0086.jpg","start":null,"end":null,"crops":{"16x9":{"left":null,"top":null,"width":null,"height":null},"4x3":{"left":null,"top":null,"width":null,"height":null},"3x2":{"left":null,"top":null,"width":null,"height":null},"1x1":{"left":null,"top":null,"width":null,"height":null}}},{"pid":1219977,"offset":89,"image":"106551-0089.jpg","start":null,"end":null,"crops":{"16x9":{"left":null,"top":null,"width":null,"height":null},"4x3":{"left":null,"top":null,"width":null,"height":null},"3x2":{"left":null,"top":null,"width":null,"height":null},"1x1":{"left":null,"top":null,"width":null,"height":null}}},{"pid":1219978,"offset":90,"image":"106551-0090.jpg","start":null,"end":null,"crops":{"16x9":{"left":null,"top":null,"width":null,"height":null},"4x3":{"left":null,"top":null,"width":null,"height":null},"3x2":{"left":null,"top":null,"width":null,"height":null},"1x1":{"left":null,"top":null,"width":null,"height":null}}}]},


#{"pages":218,"videos":[{"quality":null,"title":"An

videos()
{

#	rm "$TMP/$FILENAME.list"

	if [ ! -e "$TMP/$FILENAME.list" ]; then

#{"quality":null,"title":"Tipping the pizza guy","set_id":106427,"full":1,"ps_name":"Jenny Jinx","nt_name":null,"svid":1122173165,"duration":563,"datetime":"2020-10-25 15:00:05","thumbs":

#$curlbin $URL$PARAM > /tmp/1234

#$curlbin "$URL$PAGE$NEXT$PAGE2" > $TMP/$FILENAME.curl.list
#	$curlbin -o - $URL$PARAM | sed -e 's/{/{\n/g' -e 's/,/,\n/g' -e 's/\[/\[\n/g' -e 's/\}/\n\}/g' -e 's/\]/\n\]/g' | awk -v SRC=$SRC -v PARSER=$PARSER -v NAME=$NAME '
#	$curlbin -o - $URL$PARAM | sed -e 's/{/{\n/g' -e 's/\[/\[\n/g' -e 's/\}/\n\}/g' -e 's/\]/\n\]/g' | awk -v SRC=$SRC -v PARSER=$PARSER -v NAME=$NAME '
	$curlbin -o - "$URL$PAGE$NEXT$PAGE2" | sed -e 's/{\"quality\"/{\n\"quality\"/g' | awk -v SRC=$SRC -v PARSER=$PARSER -v NAME=$NAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT -v PAGE2=$PAGE2 '

BEGIN {     title = ""
            id = ""
            svid = ""
            setid = ""
            duration = ""
            pic = ""
            image = ""
            pages = "1"
      }
/"pages":/ { 
					# extrahiere den newpage pfad
					i = index($0, "\"pages\":") + 8
		            j = index(substr($0, i), ",") - 1
		            pages = substr($0, i, j)
#print "pages: " pages
      }
/"quality":/ { 
					# extrahiere den newpage pfad
					i = index($0, "\"title\":\"") + 9
		            j = index(substr($0, i), "\"") - 1
		            title = substr($0, i, j)
#print "00000: " $0
#print "title: " title
					# extrahiere den newpage pfad
					i = index($0, "\"svid\":") + 7
		            j = index(substr($0, i), ",") - 1
		            svid = substr($0, i, j)
#print "svid: " svid

					# extrahiere den newpage pfad
					i = index($0, "\"set_id\":") + 9
		            j = index(substr($0, i), ",") - 1
		            setid = substr($0, i, j)
#print "setid: " setid

					# extrahiere den newpage pfad
					i = index($0, "\"duration\":") + 12
		            j = index(substr($0, i), ",") - 1
		            duration = substr($0, i, j)

					# extrahiere den newpage pfad
					i = index($0, "\"image\":\"") + 9
		            j = index(substr($0, i), "\"") - 1
		            image = substr($0, i, j)

pic = image

#print "pic: " pic


					if (title != "")
					{
        #           title = toupper(tag)
        #           gsub(/ /, "%20", tag)
        #           print title "#" SRC " " SRC " play video/" id "#https://img.beeg.com/236x177/" pic ".jpg#" PARSER "_" pic ".jpg#" NAME "#111"
					if (image != "")
                        print title "#" SRC " " SRC " hoster " svid "#https://img.beeg.com/236x177/" image "#" PARSER "_" image "#" NAME "#111"
                    else
                        print title "#" SRC " " SRC " hoster " svid "#https://img.beeg.com/236x177/" pic ".jpg#" PARSER "_" pic ".jpg#" NAME "#111"

                    }
            title = ""
            id = ""
            svid = ""
            setid = ""
            duration = ""
            pic = ""
            image = ""

      }
			END \
			{
#				if (curpage != pages)
					print "Page (" NEXT + 1 "/" pages ")#" SRC " " SRC " " INPUT " \x27" PAGE "\x27 " NEXT + 1 " \x27" PAGE2 "\x27#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#" NAME "#0"
			}
' >$TMP/$FILENAME.list
fi
	echo "$TMP/$FILENAME.list"
}

play()
{
	/tmp/localhoster/hoster.sh youtube_dl https://beeg.com/$PAGE > $TMP/cache.$PARSER.$INPUT.1
	STREAMURL=`cat $TMP/cache.$PARSER.$INPUT.1`
	cat $STREAMURL | head -n1
}

hoster()
{
	echo https://beeg.com/$PAGE
}

tags()
{

if [ ! -e $TMP/$FILENAME.list ]; then
#{"tag":"pump","videos":11}
#	$curlbin -o - ${URL}index/main/0/pc | sed -e 's/{DATA_MARKERS}/data=pc.XX/g' -e 's/{/{\n/g' -e 's/,/,\n/g' -e 's/\[/\[\n/g' -e 's/\}/\n\}/g' -e 's/\]/\n\]/g' | awk -v PARAM=$PARAM -v 
	$curlbin -o - ${URL}index/main/0/pc | sed -e 's/{DATA_MARKERS}/data=pc.XX/g' -e 's/{/{\n/g' -e 's/\}/\n\}/g' -e 's/\]/\n\]/g' | awk -v PARAM=$PARAM -v SRC=$SRC -v NAME=$NAME '
BEGIN { table = ""
      }
/"tag":/ { split($0, a, "\"")
           tag = a[4]
#print "00000: " $0
#print "22222: " tag
           title = toupper(tag)
           gsub(/ /, "%20", tag)
           print title "#" SRC " " SRC " videos \x27index/tag/\x27 0 \x27/pc?tag=" tag "\x27#http://openaaf.dyndns.tv/mediathek/menu/" tag ".jpg#" tag ".jpg#" NAME "#0"
      }
' >$TMP/$FILENAME.list
fi
	echo $TMP/$FILENAME.list
}

#https://api.beeg.com/api/v6/1595400363153/index/people/0/pc?search_mode=code&people=Kiittenymph

pornstar()
{

if [ ! -e $TMP/$FILENAME.list ]; then

#{"tag":"pump","videos":11}
#	$curlbin -o - ${URL}index/main/0/pc | sed -e 's/{DATA_MARKERS}/data=pc.XX/g' -e 's/{/{\n/g' -e 's/,/,\n/g' -e 's/\[/\[\n/g' -e 's/\}/\n\}/g' -e 's/\]/\n\]/g' | awk -v PARAM=$PARAM -v 
	$curlbin -o - ${URL}index/main/0/pc | sed -e 's/{DATA_MARKERS}/data=pc.XX/g' -e 's/{/{\n/g' -e 's/\}/\n\}/g' -e 's/\]/\n\]/g' | awk -v PARAM=$PARAM -v SRC=$SRC -v NAME=$NAME '
BEGIN { table = ""
      }
/"ps_name":/ { split($0, a, "\"")

                    # extrahiere den newpage pfad
					i = index($0, "\"ps_name\":\"") + 11
		            j = index(substr($0, i), "\",") - 1
		            ps_name = substr($0, i, j)

 #          ps_name = a[4]
#print "00000: " $0
#print "22222: " ps_name
#           title = toupper(ps_name)
           title = ps_name

#           gsub(/ /, "%20", ps_name)
           print title "#" SRC " " SRC " videos \x27index/people/\x27 0 \x27/pc?search_mode=code&people=" ps_name "\x27#http://openaaf.dyndns.tv/mediathek/menu/" ps_name ".jpg#" ps_name ".jpg#" NAME "#0"
      }
' >$TMP/$FILENAME.list
fi
	echo "$TMP/$FILENAME.list"

}

#{"quality":null,"title":"Super pochaco takes advantage of you","set_id":106661,"full":1,"ps_name":"Hidori Rose","nt_name":null,"svid":1711098099,"duration":907,"datetime":"2020-10-26 0

#{"tag":"white","videos":33058},{"tag":"whore","videos":100},{"tag":"widow","videos":1},{"tag":"wife","videos":618},{"tag":"wife share","videos":2},{"tag":"wild","videos":256},

case $INPUT in
#	init) $INPUT;;
	mainmenu) $INPUT;;
	videos) $INPUT;;
	hoster) $INPUT;;
	play) $INPUT;;
	tags) $INPUT;;
	page) $INPUT;;
	pornstar) $INPUT;;
esac

