#!/bin/bash
# box parser for titannit

SRC=$1
INPUT=$2
PARAM=$3
PAGE=$4
URL="http://kkiste.to/"
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME=`echo -n ${PARSER:0:2} | tr '[a-z]' '[A-Z]'`${PARSER:2}
curlbin="curl -k -s"
TMP=/tmp/parser

rm -rf $TMP > /dev/null 2>&1
mkdir $TMP > /dev/null 2>&1

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
	echo "$NAME ($TYPE)#$SRC $SRC mainmenu#http://atemio.dyndns.tv/mediathek/menu/$PARSER.jpg#$PARSER.jpg#TiThek#0"
}

mainmenu()
{
	echo "Kinofilme#$SRC $SRC videos aktuelle-kinofilme/?page= 1#http://atemio.dyndns.tv/mediathek/menu/all-newfirst.jpg#all-newfirst.jpg#$NAME#0" >/tmp/tithek/$PARSER.list
#	echo "Serien#$SRC $SRC videos serien/?page= 1#http://atemio.dyndns.tv/mediathek/menu/all-newfirst.jpg#all-newfirst.jpg#$NAME#0" >>/tmp/tithek/$PARSER.list
#	echo "Popular Tags#$SRC $SRC tags popular#http://atemio.dyndns.tv/mediathek/menu/category.jpg#category.jpg#$NAME#0" >>/tmp/tithek/$PARSER.list
#	echo "All Tags#$SRC $SRC tags nonpopular#http://atemio.dyndns.tv/mediathek/menu/category.jpg#category.jpg#$NAME#0" >>/tmp/tithek/$PARSER.list
#	echo "Long Videos#$SRC $SRC videos tag/0/pc?tag=long+videos#http://atemio.dyndns.tv/mediathek/menu/category.jpg#category.jpg#$NAME#0" >>/tmp/tithek/$PARSER.list
#	echo "Search#$SRC $SRC videos index/search/0/pc?query=#http://atemio.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >>/tmp/tithek/$PARSER.list
	echo "/tmp/tithek/$PARSER.list"
}

videos()
{
	$curlbin -o - $URL$PARAM$PAGE | awk -v SRC=$SRC -v INPUT=$INPUT -v PARAM=$PARAM -v PAGE=$PAGE -v PARSER=$PARSER -v NAME=$NAME '

BEGIN { title=""
        url=""
        pic=""
      }

/<a href="/ { if(match($0, /class="title">/) != 0) {
                 i = match($0, /<a href="/)
                 split(substr($0, i), a, "\"")
                 url = a[2]
                 i = match($0, / title="Jetzt /)
                 split(substr($0, i), a, "\"")
                 title = substr(a[2], 7, length(a[2]) - 21)
              }
            }

/<img src="/ { i = match($0, /<img src="/)
               split(substr($0, i), a, "\"")
               pic = a[2]
             }

/<div class="title"><strong>/ { n = split(pic, a, "/")
                                pic2 = a[n]
                                print title "#" SRC " " SRC " parts " url "#" pic "#" pic2 "#" NAME "#0"
                                title=""
                                url=""
                                pic=""
                                pic2=""
                              }

END   { print "Page " PAGE + 1 "#" SRC " " SRC " " INPUT " " PARAM " " PAGE + 1 "#http://atemio.dyndns.tv/mediathek/menu/next.jpg#next.jpg#" NAME "#0"
      }
' >/tmp/tithek/$PARSER.list
	echo "/tmp/tithek/$PARSER.list"
}

parts()
{
	$curlbin -o - $URL$PARAM >/tmp/tithek/kkiste.1
	cat /tmp/tithek/kkiste.1 | awk -v SRC=$SRC -v INPUT=$INPUT -v PARAM=$PARAM -v PAGE=$PAGE -v PARSER=$PARSER -v NAME=$NAME '
BEGIN { kz_parts=0
        count=1
      }

/<ul class="m">/ { kz_parts=1
                   next
                 }

/<a href="/ { if(kz_parts!=0) {
                 i = match($0, /<a href="/)
                 split(substr($0, i), a, "\"")
                 url = a[2]
                 i = match($0, /target="/)
                 split(substr($0, i), a, "\"")
                 title = a[3]
                 sub(/>/, "", title) 
                 sub(/<small>/, "", title) 
                 sub(/<\/small><\/a><\/li>/, "", title) 
                 split(title, a, "\[")
                 split(a[2], b, "\]")
                 split(b[1], a, " ")
                 part = a[2]
                 print title "#" SRC " " SRC " hoster " PARAM " " count "#http://atemio.dyndns.tv/mediathek/menu/" part ".jpg#" part ".jpg#" NAME "#111"
                 count=count+1
                 next
              }
            }

/<\/ul>/ { kz_parts=0
           next
         }
' >/tmp/tithek/$PARSER.list
	echo "/tmp/tithek/$PARSER.list"
}

hoster()
{
	i=`expr $PAGE \* 2`
	data=`$curlbin -L --cookie /mnt/network/cookies --cookie-jar /mnt/network/cookies "$URL$PARAM" | grep "data-det=" | cut -d'"' -f4`
	id=`$curlbin -L --cookie /mnt/network/cookies --cookie-jar /mnt/network/cookies --header "Content-Type: application/json" -H "X-Requested-With: XMLHttpRequest" -X POST --data "$data" --referer $URL$PARAM http://kkiste.to/xhr/link/ | cut -d'"' -f$i`
	echo "http://www.ecostream.tv/stream/$id.html"
}

tags()
{
	$curlbin -o - ${URL}index/main/0/pc | sed -e 's/{DATA_MARKERS}/data=pc.XX/g' -e 's/{/{\n/g' -e 's/,/,\n/g' -e 's/\[/\[\n/g' -e 's/\}/\n\}/g' -e 's/\]/\n\]/g' | awk -v PARAM=$PARAM -v SRC=$SRC -v NAME=$NAME '
BEGIN { table = ""
      }
/\"/  { m = 1
        while (m == 1) {
           s = $0
           i = gsub("[\"]", "x", s)
           m = i % 2
           if (m == 1) {
              getline l
              $0 = $0 l
           }
        }
      }
/^\{/ { next
      }
/\[$/ { split($0, a, "\"")
        table = a[2]
        next
      }
/^\}/ { next
      }
/^\]/ { table = ""
        next
      }
      { if (table == PARAM) {
           split($0, a, "\"")
           title = a[2]
           tag = title
           gsub(/ /, "%20", tag)
           print title "#" SRC " " SRC " videos index/tag/0/pc?tag=" tag "#http://atemio.dyndns.tv/mediathek/menu/" tag ".jpg#" tag ".jpg#" NAME "#0"
        }
      }
' >/tmp/tithek/$PARSER.list
	echo "/tmp/tithek/$PARSER.list"
}

case $INPUT in
	init) $INPUT;;
	mainmenu) $INPUT;;
	videos) $INPUT;;
	parts) $INPUT;;
	serien) $INPUT;;
	hoster) $INPUT;;
	tags) $INPUT;;
	page) $INPUT;;
esac

