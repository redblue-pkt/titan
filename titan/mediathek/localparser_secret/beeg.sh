#!/bin/bash
# box parser for titannit

SRC=$1
INPUT=$2
PARAM=$3
URL="https://beeg.com/api/v6/1525723877/"
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
	echo "Sorted by date#$SRC $SRC videos index/main/0/pc#http://atemio.dyndns.tv/mediathek/menu/all-newfirst.jpg#all-newfirst.jpg#$NAME#0" >/tmp/tithek/$PARSER.list
        echo "Popular Tags#$SRC $SRC tags popular#http://atemio.dyndns.tv/mediathek/menu/category.jpg#category.jpg#$NAME#0" >>/tmp/tithek/$PARSER.list
        echo "All Tags#$SRC $SRC tags nonpopular#http://atemio.dyndns.tv/mediathek/menu/category.jpg#category.jpg#$NAME#0" >>/tmp/tithek/$PARSER.list
        echo "Long Videos#$SRC $SRC videos index/tag/0/pc?tag=long+videos#http://atemio.dyndns.tv/mediathek/menu/category.jpg#category.jpg#$NAME#0" >>/tmp/tithek/$PARSER.list
        echo "Search#$SRC $SRC videos index/search/0/pc?query=#http://atemio.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >>/tmp/tithek/$PARSER.list
	echo "/tmp/tithek/$PARSER.list"
}

videos()
{
	$curlbin -o - $URL$PARAM | sed -e 's/{/{\n/g' -e 's/,/,\n/g' -e 's/\[/\[\n/g' -e 's/\}/\n\}/g' -e 's/\]/\n\]/g' | awk -v SRC=$SRC -v PARSER=$PARSER -v NAME=$NAME '
BEGIN { table = ""
        title = ""
        id = ""
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
/^\}/ { if (table == "videos") {
           print title "#" SRC " " SRC " play video/" id "#https://img.beeg.com/236x177/" id ".jpg#" PARSER "_" id ".jpg#" NAME "#111"
#           print title "#" SRC " " SRC " hosterlist " id "#https://img.beeg.com/236x177/" id ".jpg#" PARSER "_" id ".jpg#" NAME "#0"

           title = ""
           id = ""
        }
        next
      }
/^\]/ { table = ""
        next
      }
      { if (table == "videos") {
           split($0, a, "\"")
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
           }
        }
      }
' >/tmp/tithek/$PARSER.list
	echo "/tmp/tithek/$PARSER.list"
}

hosterlist()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		/tmp/localhoster/hoster.sh youtube_dl http://beeg.com/$PARAM > $TMP/cache.$FILENAME.1
		piccount=0

		while read -u 3 ROUND; do
			TITLE=`echo $ROUND | cut -d"/" -f6`
			PIC="http://atemio.dyndns.tv/mediathek/menu/`echo $TITLE | tr '/' '.'`.jpg"
			NEWPAGE="$ROUND"

			if [ -z "$PIC" ] || [ "$PIC" = ".jpg" ]; then
				PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
			fi

			if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
				if [ ! -e $TMP/$FILENAME.list ];then
					touch $TMP/$FILENAME.list
				fi
				piccount=`expr $piccount + 1`
				LINE="$TITLE#$NEWPAGE#$PIC#$TITLE.$piccount.jpg#$NAME#2"
				echo "$LINE" >> $TMP/$FILENAME.list
			fi

		done 3<$TMP/cache.$FILENAME.1
		rm $TMP/cache.* > /dev/null 2>&1
	fi
	echo "$TMP/$FILENAME.list"
}

#unused
play()
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
#	init) $INPUT;;
	mainmenu) $INPUT;;
	videos) $INPUT;;
	hosterlist) $INPUT;;
	play) $INPUT;;
	tags) $INPUT;;
	page) $INPUT;;
esac

