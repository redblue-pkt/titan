#!/bin/bash
# titannit box parser for burning series

SRC=$1
INPUT=$2
PARAM=$3
PARAM2=$4
URL="http://bs.to/"
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME="burningseries"
wgetbin="wget -q -T2"
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
	echo "Burning Series($TYPE)#$SRC $SRC mainmenu#http://atemio.dyndns.tv/mediathek/menu/$PARSER.jpg#$PARSER.jpg#TiThek#0"
}

mainmenu()
{
        echo "Genres#$SRC $SRC genres#http://atemio.dyndns.tv/mediathek/menu/serien.genre.jpg#serien.genre.jpg#$NAME#0" >/tmp/tithek/$PARSER.mainmenu.list
        echo "All Series#$SRC $SRC allseries#http://atemio.dyndns.tv/mediathek/menu/serien.jpg#serien.jpg#$NAME#0" >>/tmp/tithek/$PARSER.mainmenu.list
        echo "Search#$SRC $SRC search#http://atemio.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#0" >>/tmp/tithek/$PARSER.mainmenu.list
	echo "/tmp/tithek/$PARSER.mainmenu.list"
}

genres()
{
	$wgetbin $URL"andere-serien" -O - | awk -v SRC=$SRC -v NAME=$NAME '
BEGIN { in_genres = 0
      }

/<div class="genre">/ { in_genres = 1
                        next
                      }

/<span><strong>/ { i = index($0, "<span><strong>") + 14
                   j = index($0, "</strong></span>") - i
                   title = substr($0, i, j) 
                   pic = tolower(title) 
                   print title "#" SRC " " SRC " series " title "#http://atemio.dyndns.tv/mediathek/menu/" pic ".jpg#" pic ".jpg#" NAME "#0"
                   next
                 }
                  
' >/tmp/tithek/$PARSER.genres.list
	echo "/tmp/tithek/$PARSER.genres.list"
}

series()
{
	$wgetbin $URL"andere-serien" -O - | awk -v PARAM=$PARAM -v SRC=$SRC -v NAME=$NAME '
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
                      print title "#" SRC " " SRC " staffel " url "#http://atemio.dyndns.tv/mediathek/menu/default.jpg#default.jpg#" NAME "#0"
                   }
                   next
                 }
' >/tmp/tithek/$PARSER.series.list
	echo "/tmp/tithek/$PARSER.series.list"
}

staffel()
{
	$wgetbin $URL$PARAM -O - | awk -v PARAM=$PARAM -v SRC=$SRC -v NAME=$NAME '
/<li class=\" current\">/ { sub(/<li class=\" current\">/, "<li class=\" \">", $0)
                          }

/<li class=\" \"><a href="/ { i = index($0, "<li class=\" \"><a href=\"") + 23
                              j = index(substr($0, i), "\">") - 1
                              url = substr($0, i, j)
                              k = i + j + 2
                              l = index(substr($0, k), "</a></li>") - 1
                              staffel = substr($0, k, l)
                              print "Staffel " staffel "#" SRC " " SRC " episode " url " " staffel "#http://atemio.dyndns.tv/mediathek/menu/s" staffel ".jpg#s" staffel ".jpg#" NAME "#0"
                            }
' >/tmp/tithek/$PARSER.staffel.list
	echo "/tmp/tithek/$PARSER.staffel.list"
}

episode()
{
	$wgetbin $URL$PARAM -O - | awk -v PARAM=$PARAM -v PARAM2=$PARAM2 -v SRC=$SRC -v NAME=$NAME '
BEGIN { in_table_row = 0
        episode = ""
        url = ""
        title = ""
      }
/<tr>/ { in_table_row = 1
         next
       }
/<td><a href=\"/ { if (in_table_row == 1) {
                      i = index($0, "<td><a href=\"") + 13
                      j = index(substr($0, i), "\">") - 1
                      url = substr($0, i, j)
                   }
                   next
                 }

/<td>/ { if (in_table_row == 1) {
            i = index($0, "<td>") + 4
            j = index(substr($0, i), "</td>") - 1
            episode = substr($0, i, j)
         }
         next
       }

/<strong>/ { if (in_table_row == 1) {
                i = index($0, "<strong>") + 8
                j = index(substr($0, i), "</strong>") - 1
                title = substr($0, i, j)
                do {
                   i = match(title, /&#[0-2][0-9][0-9];/)
                   if (i != 0) {
                      j = substr(title, i + 2, 3) * 1
                      title = substr(title, 1, i - 1) sprintf("%c", j) substr(title, i + 6)
                   }
                } while(i != 0)
             }
             next
           }

/<\/tr>/ { if (in_table_row == 1) {
             if (episode != "") {
                print title "#" SRC " " SRC " hoster " url "#http://atemio.dyndns.tv/mediathek/menu/s" PARAM2 "e" episode ".jpg#s" PARAM2 "e" episode ".jpg#" NAME "#0"
                title = ""
                url = ""
                episode = ""
             }
             in_table_row = 0
          }
          next
        }

/<li class=\" current\">/ { sub(/<li class=\" current\">/, "<li class=\" \">", $0)
                          }
' >/tmp/tithek/$PARSER.episode.list
	echo "/tmp/tithek/$PARSER.episode.list"
}

hoster()
{
	$wgetbin $URL$PARAM -O - | awk -v PARAM=$PARAM -v PARAM2=$PARAM2 -v SRC=$SRC -v NAME=$NAME '
BEGIN { in_hoster = 0
        url = ""
        title = ""
      }

/<h3>Hoster dieser Episode<\/h3>/ { in_hoster = 1
                                    next
                                  }

/href=\"/ { if (in_hoster == 1) {
               i = index($0, "href=\"") + 6
               j = index(substr($0, i), "\">") - 1
               url = substr($0, i, j)
            }
            next
          }

/<\/span> / { if (in_hoster == 1) {
                 i = index($0, "</span> ") + 8
                 j = index(substr($0, i), "</a>") - 1
                 title = substr($0, i, j)
                 print title "#" SRC " " SRC " video " url "#http://atemio.dyndns.tv/mediathek/menu/default.jpg#default.jpg#" NAME "#0"
              }
              next
            }

/<\/ul>/ { in_hoster = 0
          next
        }

' >/tmp/tithek/$PARSER.hoster.list
	echo "/tmp/tithek/$PARSER.hoster.list"
}

video()
{
	$wgetbin $URL$PARAM -O - | awk -v PARAM=$PARAM -v PARAM2=$PARAM2 -v SRC=$SRC -v NAME=$NAME '
BEGIN { in_hoster = 0
        url = ""
        title = ""
      }

/<div id=\"video_actions\">/ { in_video = 1
                               next
                             }

/<a href=\"/ { if (in_video == 1) {
                  i = index($0, "<a href=\"") + 9
                  j = index(substr($0, i), "\"") - 1
                  url = substr($0, i, j)
                  print "Originalvideo#" url "#http://atemio.dyndns.tv/mediathek/menu/default.jpg#default.jpg#" NAME "#14"
               }
               next
             }

/<\/div>/ { in_video = 0
            next
          }

' >/tmp/tithek/$PARSER.video.list
	echo "/tmp/tithek/$PARSER.video.list"
}

case $INPUT in
	init) $INPUT;;
	mainmenu) $INPUT;;
	genres) $INPUT;;
	series) $INPUT;;
	staffel) $INPUT;;
	episode) $INPUT;;
	hoster) $INPUT;;
	video) $INPUT;;
esac

