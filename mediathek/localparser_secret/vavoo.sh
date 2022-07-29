#!/bin/sh
# first sh box parser for titannit mfg obi

SRC=$1
INPUT=$2
PAGE=$3
NEXT=$4
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
URL=https://www2.vjackson.info/live2/index
NAME="VaVoo"

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

    if [ ! -e $TMP/vavoo.3.signed.base64 ];then
        curl -v -L -s --user-agent "VAVOO/2.6" -X POST http://www.vavoo.tv/api/box/guest > $TMP/vavoo.1.guest
        token=$(cat $TMP/vavoo.1.guest | awk 'BEGIN {} /token/ { i = index($0, "token\":\"") + 8;j = index(substr($0, i), "\"") - 1;token = substr($0, i, j); print token; next}')
        echo $token > $TMP/vavoo.2.token
        curl -v -L -s --user-agent "VAVOO/2.6" --data "token=$token" -X POST https://www.vavoo.tv/api/box/ping2 > $TMP/vavoo.3.ping2
        signed=$(cat $TMP/vavoo.3.ping2 | awk 'BEGIN {} /signed/ { i = index($0, "signed\":\"") + 9;j = index(substr($0, i), "\"") - 1;signed = substr($0, i, j); print signed; next}')
        echo $signed > $TMP/vavoo.4.signed
        base64 -d $TMP/vavoo.4.signed > $TMP/vavoo.5.signed.base64
        cat $TMP/vavoo.5.signed.base64 | sed -nr 's/.*\\"validUntil\\":([^:]+),.*/\1/p' > $TMP/vavoo.6.signed.base64.timestamp
        timestamp=$(cat $TMP/vavoo.6.signed.base64.timestamp)
        cat $TMP/vavoo.5.signed.base64 | sed 's#false#true#g' | sed 's#\\"Daily usage duration reached\\"#null#g' | sed 's#\\"version\\":null#\\"version\\":\\"2.6\\"#g' | sed 's#\\"serivce\\":null#\\"serivce\\":\\"1.2.26\\"#g' | sed 's#\\"platform\\":null#\\"platform\\":\\"vavoo\\"#g' | sed "s#$timestamp#2609441001029#g" > $TMP/vavoo.7.signed.base64.timestamp.sed
    fi
    
	echo "Category#$SRC $SRC category#http://openaaf.dyndns.tv/mediathek/menu/category.jpg#category.jpg#$NAME#0" >$TMP/$FILENAME.list
	echo "$TMP/$FILENAME.list"
}

category()
{
    vavoo_auth=$(base64 $TMP/vavoo.7.signed.base64.timestamp.sed | tr -d '\n')
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - $URL | sed -e "s/\.ts$/\.ts?n=1\&b=5\&vavoo_auth=$vavoo_auth|User-Agent=VAVOO\/2.6/g" -e 's/^http:/#EXTVLCOPT:http-user-agent=VAVOO\/2.6\nhttp:/g' | awk -v TEST=$TEST -v TMP=$TMP -v FILENAME=$FILENAME -v SRC=$SRC -v URL=$URL -v PAGE=$PAGE -v NAME=$NAME -v PICNAME=$PICNAME \
		'
			BEGIN \
			{
				suche = 1
				newpage = ""
				piccount = 0
			}
            /#EXTINF/ \
			{
				if (suche == 1)
				{
					i = index($0, "group-title=\"") + 13
		            j = index(substr($0, i), "\"") - 1
		            title = substr($0, i, j)

					picname = tolower(title)

	            	gsub(" ", ".", picname)
					pic = "http://openaaf.dyndns.tv/mediathek/menu/" picname ".jpg"

					if (title != "")
					{
                        titletmp = titletmp title
						print title "#" SRC " " SRC " search \x27" PAGE "\x27 \x27" title "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#0"
                    }
					newpage = ""
					title = ""
					picname = ""
					pic = ""
					next
				}
			}
		' >$TMP/$FILENAME.list
	fi

    cat $TMP/$FILENAME.list | sort -u > $TMP/$FILENAME.sort.list
	echo "$TMP/$FILENAME.sort.list"
}

search()
{
    vavoo_auth=$(base64 $TMP/vavoo.7.signed.base64.timestamp.sed | tr -d '\n')
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - $URL | sed -e "s/\.ts$/\.ts?n=1\&b=5\&vavoo_auth=$vavoo_auth|User-Agent=VAVOO\/2.6/g" -e 's/^http:/#EXTVLCOPT:http-user-agent=VAVOO\/2.6\nhttp:/g' | awk -v NEXT="$NEXT" -v SRC=$SRC -v URL=$URL -v PAGE=$PAGE -v NAME=$NAME -v PICNAME=$PICNAME \
		'
			BEGIN \
			{
				found = 0
				newpage = ""
				piccount = 0

			}
            /#EXTINF/ \
			{
                if ($0 ~ NEXT)
                {
				    if (found == 0)
				    {
					    i = index($0, "tvg-name=\"") + 10
		                j = index(substr($0, i), "\"") - 1
		                title = substr($0, i, j)

					    i = index($0, "tvg-logo=\"") + 10
		                j = index(substr($0, i), "\"") - 1
		                pic = substr($0, i, j)

					    if ( pic == "" )
					    {
						    picname = tolower(title)
		                	gsub(" ", ".", picname)
						    pic = "http://openaaf.dyndns.tv/mediathek/menu/" picname ".jpg"
					    }
	    
                        found = 1
					    next
                    }
				}
            }
            /^http/ \
			{
                if (found == 1)
                {
	                newpage = substr($0, 1, length($0) - 1)
				    if (title != "" && title !~ "= = =")
				    {
					    piccount += 1
					    print title "#" newpage "#" pic "#" PICNAME "." piccount ".jpg#" NAME "#2"
#						print title " (" extra ")#" SRC " " SRC " play \x27" newpage "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#111"
#						print title "#" SRC " " SRC " hoster \x27" newpage "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#111"
				    }
				    
				    newpage = ""
				    title = ""
				    picname = ""
				    pic = ""
                    found = 0
				    next
				}
			}
		' >$TMP/$FILENAME.list
	fi
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
	category) $INPUT;;
	hoster) $INPUT;;
esac
