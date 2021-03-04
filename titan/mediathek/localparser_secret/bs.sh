#!/bin/bash

# titannit box parser for burning series
SRC=$1
INPUT=$2
PARAM=$3
PARAM2=$4
URL="https://bs.to/"
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME="burningseries"

case $2 in
	init)	;;
	*) 	. /tmp/localhoster/hoster.sh
	   	mkdir $TMP > /dev/null 2>&1
		FILENAME="$PARSER $INPUT $PAGE $PARAM $PARAM2"
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
	echo "Burning Series ($TYPE)#$SRC $SRC mainmenu#http://openaaf.dyndns.tv/mediathek/menu/$PARSER.jpg#$PARSER.jpg#TiThek#0"
}

mainmenu()
{
        echo "Genres#$SRC $SRC genres#http://openaaf.dyndns.tv/mediathek/menu/serien.genre.jpg#serien.genre.jpg#$NAME#0" >/tmp/tithek/$PARSER.mainmenu.list
        echo "All Series#$SRC $SRC allseries#http://openaaf.dyndns.tv/mediathek/menu/serien.jpg#serien.jpg#$NAME#0" >>/tmp/tithek/$PARSER.mainmenu.list
        echo "Search#$SRC $SRC search#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#0" >>/tmp/tithek/$PARSER.mainmenu.list
	echo "/tmp/tithek/$PARSER.mainmenu.list"
}

genres()
{
	$curlbin -o - $URL"serie-genre" | awk -v SRC=$SRC -v NAME=$NAME '
BEGIN { in_genres = 0
      }

/<div class="genre">/ { in_genres = 1
                        next
                      }

/<span><strong>/ { i = index($0, "<span><strong>") + 14
                   j = index($0, "</strong></span>") - i
                   title = substr($0, i, j)
                   pic = tolower(title)
                   print title "#" SRC " " SRC " series " title "#http://openaaf.dyndns.tv/mediathek/menu/" pic ".jpg#" pic ".jpg#" NAME "#0"
                   next
                 }

' >/tmp/tithek/$PARSER.genres.list
	echo "/tmp/tithek/$PARSER.genres.list"
}

series()
{
	$curlbin -o - $URL"serie-genre" | awk -v PARAM=$PARAM -v SRC=$SRC -v NAME=$NAME '
/<span><strong>/ { i = index($0, "<span><strong>") + 14
                   j = index($0, "</strong></span>") - i
                   genre = substr($0, i, j)
                 }

/<li><a href=\"/ { if (genre == PARAM) {
                      i = index($0, "<li><a href=\"") + 13
                      j = index(substr($0, i), "\"") - 1
                      url = substr($0, i, j)
                      j = index(substr($0, i), "\">") - 1
                      k = i + j + 2
                      l = index(substr($0, k), "</a></li>") - 1
                      title = substr($0, k, l)
                      print title "#" SRC " " SRC " staffel " url "#http://openaaf.dyndns.tv/mediathek/menu/default.jpg#default.jpg#" NAME "#0"
                   }
                   next
                 }
' >/tmp/tithek/$PARSER.series.list
	cat /tmp/tithek/$PARSER.series.list | sed -e "s!&#039;!'!g" > /tmp/tithek/$PARSER.series.list.1
	echo "/tmp/tithek/$PARSER.series.list.1"
}

allseries()
{
	$curlbin -o - $URL"serie-alphabet" | awk -v PARAM=$PARAM -v SRC=$SRC -v NAME=$NAME '
/<span><strong>/ { i = index($0, "<span><strong>") + 14
                   j = index($0, "</strong></span>") - i
                   genre = substr($0, i, j)
                 }

/<li><a href=\"/ { if (genre == PARAM) {
                      i = index($0, "<li><a href=\"") + 13
                      j = index(substr($0, i), "\">") - 1
                      url = substr($0, i, j)
                      k = i + j + 2
                      l = index(substr($0, k), "</a></li>") - 1
                      title = substr($0, k, l)
                      print title "#" SRC " " SRC " staffel " url "#http://openaaf.dyndns.tv/mediathek/menu/default.jpg#default.jpg#" NAME "#0"
                   }
                   next
                 }
' >/tmp/tithek/$PARSER.allseries.list
	echo "/tmp/tithek/$PARSER.allseries.list"
}

staffel()
{
	$curlbin -o - $URL$PARAM | awk -v PARAM=$PARAM -v SRC=$SRC -v NAME=$NAME '
BEGIN { in_class_clearfix = 0
      }

/<ul class=\"clearfix\">/ { in_class_clearfix = 1
                          }

/<li><a href=\"/ { if (in_class_clearfix == 1) {
                      i = index($0, "<li><a href=\"") + 13
                      j = index(substr($0, i), "\">") - 1
                      url = substr($0, i, j)
                      k = i + j + 2
                      l = index(substr($0, k), "</a></li>") - 1
                      staffel = substr($0, k, l)
                      print "Staffel " staffel "#" SRC " " SRC " episode " url " " staffel "#http://openaaf.dyndns.tv/mediathek/menu/s" staffel ".jpg#s" staffel ".jpg#" NAME "#0"
                   }
                 }

/<\/ul>/ { in_class_clearfix = 0
         }
' >/tmp/tithek/$PARSER.staffel.list
	echo "/tmp/tithek/$PARSER.staffel.list"
}

episode()
{
	$curlbin -o - $URL$PARAM | awk -v PARAM=$PARAM -v PARAM2=$PARAM2 -v SRC=$SRC -v NAME=$NAME '
BEGIN { in_table_row = 0
      }

/<tr>/ { in_table_row = 1
         next
       }

/<td><a href=\"/ { if (in_table_row == 1) {
                      i = index($0, "<td><a href=\"") + 13
                      j = index(substr($0, i), "\"") - 1
                      url = substr($0, i, j)
                      i = index($0, "title=\"") + 7
                      j = index(substr($0, i), "\"") - 1
                      title = substr($0, i, j)
                      k = i + j + 2
                      l = index(substr($0, k), "</a></td>") - 1
                      episode = substr($0, k, l)
                      print title "#" SRC " " SRC " hosterlist " url "#http://openaaf.dyndns.tv/mediathek/menu/s" PARAM2 "e" episode ".jpg#s" PARAM2 "e" episode ".jpg#" NAME "#0"
                   }
                   next
                 }

/<\/tr>/ { in_table_row = 0
           next
         }
' >/tmp/tithek/$PARSER.episode.list
	echo "/tmp/tithek/$PARSER.episode.list"
}

hosterlist()
{
	$curlbin -o - $URL$PARAM | awk -v PARAM=$PARAM -v PARAM2=$PARAM2 -v SRC=$SRC -v NAME=$NAME '
BEGIN { in_hosterlist = 0
      }

/<ul class=\"hoster-tabs top\">/ { in_hosterlist = 1
                                   next
                                 }

/href=\"/ { if (in_hosterlist == 1) {
               i = index($0, "href=\"") + 6
               j = index(substr($0, i), "\">") - 1
               url = substr($0, i, j)
            }
            next
          }

/<\/span>&nbsp;/ { if (in_hosterlist == 1) {
                 i = index($0, "</span>&nbsp;") + 13
                 title = substr($0, i)
                 pic = tolower(title)
                 print title "#" SRC " " SRC " hoster " url "#http://openaaf.dyndns.tv/mediathek/menu/" pic ".jpg#" pic ".jpg#" NAME "#111"
              }
              next
            }

/<\/ul>/ { in_hosterlist = 0
          next
        }

' >/tmp/tithek/$PARSER.hosterlist.list
	echo "/tmp/tithek/$PARSER.hosterlist.list"
}


###################
# in_hoster
###################
#<div id="video_actions">
#                <div>
#            <a href="http://vivo.sx/dcd747a9af" target="_blank"><span class="icon link_go"></span> Link zum Originalvideo</a>
#                    </div>
#            </div>
#    <br style="clear:both;"/>

###################
# in_hoster_iframe
###################
#<iframe scrolling='no' frameborder='no' width='630' height='390' allowfullscreen='ture' webkitallowfullscreen='true' mozallowfullscreen='true' src='https://openload.co/embed/r3Qxa6zWdwM/Die.Simpsons.S01E01.Es.weihnachtet.schwer.German.DVDRiP.XviD.iNTERNAL-UTOPiA.mkv'></iframe>
#        <div id="video_actions">
#                <div>
#            <a href="javascript:alert(&#039;Nicht möglich&#039;);" target="_blank"><span class="icon link_go"></span> Link zum Originalvideo</a>
#                    </div>
#            </div>
#    <br style="clear:both;"/>


#function followlocation()
#{
#	system("curlbin -s -D - $1 | grep Location: | tail -n1")
#}

hoster()
{
	STREAM=`$curlbin -o - $URL$PARAM | awk '
/class=\"hoster-player\"/ { i = index($0, "<a href=\"") + 9
                            j = index(substr($0, i), "\"") - 1
                            url = substr($0, i, j)
                            print url
                            next
                          }
'`

	STREAM=`echo $STREAM | sed 's!https://bs.to/out/!watch/!'`
	HEADER=`$BIN /tmp/localhoster/bs.py $STREAM`

	$curlbin -H "$HEADER" -o - $URL/api/$STREAM >/tmp/tithek/$PARSER.hoster.1
	cat /tmp/tithek/$PARSER.hoster.1 | grep fullurl | sed 's!fullurl!\nfullurl!' | grep ^fullurl | cut -d'"' -f3 | tr -d '\\'
}

hosterold()
{
	STREAM=`$curlbin -o - $URL$PARAM | awk '
/class=\"hoster-player\"/ { i = index($0, "<a href=\"") + 9
                            j = index(substr($0, i), "\"") - 1
                            url = substr($0, i, j)
                            print url
                            next
                          }
'`
	$curlbin -o - $STREAM >/tmp/tithek/$PARSER.hoster.1
	cat /tmp/tithek/$PARSER.hoster.1 | $curlbin -o - $STREAM | awk '
/\"og:url\" content=/ { i = index($0, "\"og:url\" content=") + 18
                        j = index(substr($0, i), "\"") - 1
                        url = substr($0, i, j)
                        print url
                        next
                      }
/name=\"id\" value=\"/ { i = index($0, "value=\"") + 7
                         j = index(substr($0, i), "\"") - 1
                         url = "http://vidto.me/" substr($0, i, j) ".html"
                         print url
                         next
                       }
' >/tmp/tithek/$PARSER.hoster.list 
	cat /tmp/tithek/$PARSER.hoster.list
}

case $INPUT in
#	init) $INPUT;;
	mainmenu) $INPUT;;
	genres) $INPUT;;
	series) $INPUT;;
	allseries) $INPUT;;
	staffel) $INPUT;;
	episode) $INPUT;;
	hosterlist) $INPUT;;
	hoster) $INPUT;;
esac

