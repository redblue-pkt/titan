#!/bin/bash
# box parser for titannit

SRC=$1
INPUT=$2
PARAM=$3
#URL="https://beeg.com/api/v6/1525723877/"
URL="https://beeg.com/api/v6/1546225636701/"

PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME=`echo -n ${PARSER:0:1} | tr '[a-z]' '[A-Z]'`${PARSER:1}

case $2 in
	init)	;;
	*) 	. /tmp/localhoster/hoster.sh
	   	mkdir $TMP > /dev/null 2>&1
		FILENAME="$PARSER $INPUT $PAGE $PARAM"
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
	echo "$NAME ($TYPE)#$SRC $SRC mainmenu#http://atemio.dyndns.tv/mediathek/menu/$PARSER.jpg#$PARSER.jpg#TiThek#1000"
}

mainmenu()
{
#	echo "Sorted by date#$SRC $SRC videos index/main/0/pc#http://atemio.dyndns.tv/mediathek/menu/all-newfirst.jpg#all-newfirst.jpg#$NAME#0" >/tmp/tithek/$PARSER.list
	echo "Sorted by date#$SRC $SRC videos index/main/1/pc#http://atemio.dyndns.tv/mediathek/menu/all-newfirst.jpg#all-newfirst.jpg#$NAME#0" >/tmp/tithek/$PARSER.list
#        echo "Popular Tags#$SRC $SRC tags popular#http://atemio.dyndns.tv/mediathek/menu/category.jpg#category.jpg#$NAME#0" >>/tmp/tithek/$PARSER.list
        echo "All Tags#$SRC $SRC tags nonpopular#http://atemio.dyndns.tv/mediathek/menu/category.jpg#category.jpg#$NAME#0" >>/tmp/tithek/$PARSER.list
#        echo "Long Videos#$SRC $SRC videos index/tag/0/pc?tag=long+videos#http://atemio.dyndns.tv/mediathek/menu/category.jpg#category.jpg#$NAME#0" >>/tmp/tithek/$PARSER.list
#        echo "Search#$SRC $SRC videos index/search/0/pc?query=#http://atemio.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >>/tmp/tithek/$PARSER.list
	echo "/tmp/tithek/$PARSER.list"
}


#{"quality":null,"title":"Pink panty stuffing and cum","set_id":106551,"full":1,"ps_name":"Jessica Starling","nt_name":null,"svid":1786133462,"duration":425,"datetime":"2020-10-25 19:00:06","thumbs":[{"pid":1219962,"offset":4,"image":"106551-0004.jpg","start":null,"end":null,"crops":{"16x9":{"left":null,"top":null,"width":null,"height":null},"4x3":{"left":null,"top":null,"width":null,"height":null},"3x2":{"left":null,"top":null,"width":null,"height":null},"1x1":{"left":null,"top":null,"width":null,"height":null}}},{"pid":1219963,"offset":46,"image":"106551-0046.jpg","start":null,"end":null,"crops":{"16x9":{"left":null,"top":null,"width":null,"height":null},"4x3":{"left":null,"top":null,"width":null,"height":null},"3x2":{"left":null,"top":null,"width":null,"height":null},"1x1":{"left":null,"top":null,"width":null,"height":null}}},{"pid":1219964,"offset":48,"image":"106551-0048.jpg","start":null,"end":null,"crops":{"16x9":{"left":null,"top":null,"width":null,"height":null},"4x3":{"left":null,"top":null,"width":null,"height":null},"3x2":{"left":null,"top":null,"width":null,"height":null},"1x1":{"left":null,"top":null,"width":null,"height":null}}},{"pid":1219965,"offset":49,"image":"106551-0049.jpg","start":null,"end":null,"crops":{"16x9":{"left":null,"top":null,"width":null,"height":null},"4x3":{"left":null,"top":null,"width":null,"height":null},"3x2":{"left":null,"top":null,"width":null,"height":null},"1x1":{"left":null,"top":null,"width":null,"height":null}}},{"pid":1219966,"offset":52,"image":"106551-0052.jpg","start":null,"end":null,"crops":{"16x9":{"left":null,"top":null,"width":null,"height":null},"4x3":{"left":null,"top":null,"width":null,"height":null},"3x2":{"left":null,"top":null,"width":null,"height":null},"1x1":{"left":null,"top":null,"width":null,"height":null}}},{"pid":1219967,"offset":54,"image":"106551-0054.jpg","start":null,"end":null,"crops":{"16x9":{"left":null,"top":null,"width":null,"height":null},"4x3":{"left":null,"top":null,"width":null,"height":null},"3x2":{"left":null,"top":null,"width":null,"height":null},"1x1":{"left":null,"top":null,"width":null,"height":null}}},{"pid":1219968,"offset":56,"image":"106551-0056.jpg","start":null,"end":null,"crops":{"16x9":{"left":null,"top":null,"width":null,"height":null},"4x3":{"left":null,"top":null,"width":null,"height":null},"3x2":{"left":null,"top":null,"width":null,"height":null},"1x1":{"left":null,"top":null,"width":null,"height":null}}},{"pid":1219969,"offset":57,"image":"106551-0057.jpg","start":null,"end":null,"crops":{"16x9":{"left":null,"top":null,"width":null,"height":null},"4x3":{"left":null,"top":null,"width":null,"height":null},"3x2":{"left":null,"top":null,"width":null,"height":null},"1x1":{"left":null,"top":null,"width":null,"height":null}}},{"pid":1219970,"offset":58,"image":"106551-0058.jpg","start":null,"end":null,"crops":{"16x9":{"left":null,"top":null,"width":null,"height":null},"4x3":{"left":null,"top":null,"width":null,"height":null},"3x2":{"left":null,"top":null,"width":null,"height":null},"1x1":{"left":null,"top":null,"width":null,"height":null}}},{"pid":1219971,"offset":62,"image":"106551-0062.jpg","start":null,"end":null,"crops":{"16x9":{"left":null,"top":null,"width":null,"height":null},"4x3":{"left":null,"top":null,"width":null,"height":null},"3x2":{"left":null,"top":null,"width":null,"height":null},"1x1":{"left":null,"top":null,"width":null,"height":null}}},{"pid":1219972,"offset":72,"image":"106551-0072.jpg","start":null,"end":null,"crops":{"16x9":{"left":null,"top":null,"width":null,"height":null},"4x3":{"left":null,"top":null,"width":null,"height":null},"3x2":{"left":null,"top":null,"width":null,"height":null},"1x1":{"left":null,"top":null,"width":null,"height":null}}},{"pid":1219973,"offset":75,"image":"106551-0075.jpg","start":null,"end":null,"crops":{"16x9":{"left":null,"top":null,"width":null,"height":null},"4x3":{"left":null,"top":null,"width":null,"height":null},"3x2":{"left":null,"top":null,"width":null,"height":null},"1x1":{"left":null,"top":null,"width":null,"height":null}}},{"pid":1219974,"offset":81,"image":"106551-0081.jpg","start":null,"end":null,"crops":{"16x9":{"left":null,"top":null,"width":null,"height":null},"4x3":{"left":null,"top":null,"width":null,"height":null},"3x2":{"left":null,"top":null,"width":null,"height":null},"1x1":{"left":null,"top":null,"width":null,"height":null}}},{"pid":1219975,"offset":84,"image":"106551-0084.jpg","start":null,"end":null,"crops":{"16x9":{"left":null,"top":null,"width":null,"height":null},"4x3":{"left":null,"top":null,"width":null,"height":null},"3x2":{"left":null,"top":null,"width":null,"height":null},"1x1":{"left":null,"top":null,"width":null,"height":null}}},{"pid":1219976,"offset":86,"image":"106551-0086.jpg","start":null,"end":null,"crops":{"16x9":{"left":null,"top":null,"width":null,"height":null},"4x3":{"left":null,"top":null,"width":null,"height":null},"3x2":{"left":null,"top":null,"width":null,"height":null},"1x1":{"left":null,"top":null,"width":null,"height":null}}},{"pid":1219977,"offset":89,"image":"106551-0089.jpg","start":null,"end":null,"crops":{"16x9":{"left":null,"top":null,"width":null,"height":null},"4x3":{"left":null,"top":null,"width":null,"height":null},"3x2":{"left":null,"top":null,"width":null,"height":null},"1x1":{"left":null,"top":null,"width":null,"height":null}}},{"pid":1219978,"offset":90,"image":"106551-0090.jpg","start":null,"end":null,"crops":{"16x9":{"left":null,"top":null,"width":null,"height":null},"4x3":{"left":null,"top":null,"width":null,"height":null},"3x2":{"left":null,"top":null,"width":null,"height":null},"1x1":{"left":null,"top":null,"width":null,"height":null}}}]},


videos()
{
#{"quality":null,"title":"Tipping the pizza guy","set_id":106427,"full":1,"ps_name":"Jenny Jinx","nt_name":null,"svid":1122173165,"duration":563,"datetime":"2020-10-25 15:00:05","thumbs":

#$curlbin $URL$PARAM > /tmp/1234

#$curlbin $URL$PARAM > /tmp/a123
#	$curlbin -o - $URL$PARAM | sed -e 's/{/{\n/g' -e 's/,/,\n/g' -e 's/\[/\[\n/g' -e 's/\}/\n\}/g' -e 's/\]/\n\]/g' | awk -v SRC=$SRC -v PARSER=$PARSER -v NAME=$NAME '
#	$curlbin -o - $URL$PARAM | sed -e 's/{/{\n/g' -e 's/\[/\[\n/g' -e 's/\}/\n\}/g' -e 's/\]/\n\]/g' | awk -v SRC=$SRC -v PARSER=$PARSER -v NAME=$NAME '
	$curlbin -o - $URL$PARAM | sed -e 's/{\"quality\"/{\n\"quality\"/g' | awk -v SRC=$SRC -v PARSER=$PARSER -v NAME=$NAME '

BEGIN {     title = ""
            id = ""
            svid = ""
            setid = ""
            duration = ""
            pic = ""
            image = ""
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


if (length(duration) == 1)
{
#    print "duration: 000+" duration
    pic = pid "-000"duration
}
else if (length(duration) == 2)
{
#    print "duration: 00+" duration
    pic = pid "-00"duration
}
else if (length(duration) == 3)
{
#    print "duration: 0+" duration
    pic = pid "-0"duration
}

else
{
#    print "duration: " duration
    pic = pid "-"duration
}

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
' >/tmp/tithek/$PARSER.list
	echo "/tmp/tithek/$PARSER.list"
}

play()
{
	/tmp/localhoster/hoster.sh youtube_dl https://beeg.com/$PARAM > $TMP/cache.$PARSER.$INPUT.1
	STREAMURL=`cat $TMP/cache.$PARSER.$INPUT.1`
	cat $STREAMURL | head -n1
}

hoster()
{
	echo https://beeg.com/$PARAM
}

#unused
play1()
{
	$curlbin -o - $URL$PARAM | sed -e 's/{/{\n/g' -e 's/,/,\n/g' -e 's/\[/\[\n/g' -e 's/\}/\n\}/g' -e 's/\]/\n\]/g' | awk -v PARSER=$PARSER -v NAME=$NAME '
BEGIN { table = ""
        title = ""
        id = ""
        v720p = ""
        v480p = ""
        v240p = ""
        h2d = "0123456789ABCDEF"
        for (i = 0; i <= 255; i++) {
           ord[sprintf("%c", i)] = i
        }
        crc = "uN1ps2eNDSyHUs08RmofF1F8opmhedPKNDWK9w"
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
/^\}/ { if (v720p == "null") {
           if (v480p == "null") {
              video = decrypt_url(v240p)
           }
           else {
              video = decrypt_url(v480p)
           }
        }
        else {
           video = decrypt_url(v720p)
        }
        if (video != "null") {
#           print title "#http:" video "#https://img.beeg.com/236x177/" id ".jpg#" PARSER "_" id ".jpg#" NAME "#2"
           print "http:" video
           }
        title = ""
        id = ""
        v720p = ""
        v480p = ""
        v240p = ""
        next
      }
/^\]/ { table = ""
        next
      }
      { split($0, a, "\"")
        if (a[3] ==":") {
           value = a[4]
        }
        else {
           value = substr(a[3], 2, length(a[3]) - 2)
        }
        if (a[2] == "title") {
           title = value
        }
        else {
           if (a[2] == "id") {
              id = value
           }
           else {
              if (a[2] == "240p") {
                 v240p = value
              }
              else {
                 if (a[2] == "480p") {
                    v480p = value
                 }
                 else {
                    if (a[2] == "720p") {
                       v720p = value
                    }
                 }
              }
           }
        }
      }

function decrypt_url(url)
      { print "URL1: " url >>"/mnt/parser/beeg.log"
        sub(/\{DATA_MARKERS\}/, "", url)
        key_von = match(url, /\/key=/) + 5
        print "URL2: " url >>"/mnt/parser/beeg.log"
        key_bis = match(url, /\%2Cend/)
        key_len = key_bis - key_von
        key = ""
        for (i = key_von; i < key_bis; i++) {
           s = substr(url, i , 1)
           if (s == "%") {
              n = (index(h2d, toupper(substr(url, i + 1, 1))) - 1) * 16
              n = n + (index(h2d, toupper(substr(url, i + 2, 1))) - 1)
              if (n != 194) {
                 s = sprintf("%c", n)
              }
              else {
                 s = ""
              }
              i = i + 2
           }
           key = key s
        }
        print "KEY1: " key >>"/mnt/parser/beeg.log"
        o = ""
        for (i = 1; i <= length(key); i++) {
           o = o sprintf("%c", ord[substr(key, i, 1)] - (ord[substr(crc, ((i - 1) % length(crc)) + 1, 1)] % 21))
        }
        print "KEY2: " o >>"/mnt/parser/beeg.log"
        r = ""
        for (i = length(o) - 2; i > -2; i = i - 3) {
           if (i > 0) {
              r = r substr(o, i, 3)
           }
           else {
              r = r substr(o, 1, 2 + i)
           }
        }
        print "KEY3: " r >>"/mnt/parser/beeg.log"
        return substr(url, 1, key_von - 1) r substr(url, key_bis)
      }
' >/tmp/tithek/$PARSER.list
#	echo "/tmp/tithek/$PARSER.list"
	cat "/tmp/tithek/$PARSER.list"
}

tags()
{

if [ ! -e "/tmp/tithek/$PARSER.list" ]; then
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
           print title "#" SRC " " SRC " videos index/tag/0/pc?tag=" tag "#http://atemio.dyndns.tv/mediathek/menu/" tag ".jpg#" tag ".jpg#" NAME "#0"
      }
' >/tmp/tithek/$PARSER.list
	echo "/tmp/tithek/$PARSER.list"
fi

}

case $INPUT in
	init) $INPUT;;
	mainmenu) $INPUT;;
	videos) $INPUT;;
	hoster) $INPUT;;
	play) $INPUT;;
	tags) $INPUT;;
	page) $INPUT;;
esac

