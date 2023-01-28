#!/bin/sh
# first sh box parser for titannit mfg obi

SRC=$1
INPUT=$2
PAGE=$3
NEXT=$4
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`

URL="http://dummy.url"
NAME="IpTV"

case $2 in
	init)	;;
	*) 	. /tmp/localhoster/hoster.sh
	   	mkdir $TMP > /dev/null 2>&1
		FILENAME="$PARSER $INPUT $PAGE $NEXT"
	   	FILENAME=$(echo $FILENAME | tr '&' '.' | tr '/' '.' | tr '?' '.' | tr '=' '.' | sed -e 's/\&\+/./g' -e 's#\/\+#.#g' -e 's/\?\+/./g' -e 's/;\+/./g' -e 's/=\+/./g' -e 's/ \+/./g' -e 's/\.\+/./g')
		if [ -z "$FILENAME" ]; then FILENAME=none;fi
		PICNAME="$FILENAME"
		if [ `cat /mnt/config/titan.cfg | grep tithek_iptv_url= | wc -l` -eq 1 ];then
#			URL=$(cat /mnt/config/titan.cfg | grep "tithek_iptv_url=" | sed -nr 's/.*tithek_iptv_url=([^=]+).*/\1/p') 
			URL=$(cat /mnt/config/titan.cfg | grep "tithek_iptv_url=" | sed 's/tithek_iptv_url=//g') 
		else
			URL="http://dummy.url"
		fi
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
    if [ "$URL" == "http://dummy.url" ];then
    	echo "Add Your IpTV Url on Tithek > Settings (Menu) > IpTV Url##http://openaaf.dyndns.tv/mediathek/menu/category.jpg#category.jpg#$NAME#0" >$TMP/$FILENAME.list
    else
    	echo "Category#$SRC $SRC category#http://openaaf.dyndns.tv/mediathek/menu/category.jpg#category.jpg#$NAME#0" >$TMP/$FILENAME.list
    fi
	echo "$TMP/$FILENAME.list"
}

category()
{
    rm "$TMP/$FILENAME.list"
  
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - "$URL" | awk -v TEST=$TEST -v TMP=$TMP -v FILENAME=$FILENAME -v SRC=$SRC -v URL=$URL -v PAGE=$PAGE -v NAME=$NAME -v PICNAME=$PICNAME \
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
					if ($0 ~ /group-title=/)
					{
						i = index($0, "group-title=\"") + 13
			            j = index(substr($0, i), "\"") - 1
			            title = substr($0, i, j)
					}
					else
					{
						i = index($0, ":-1,") + 4
						title = substr($0, i, length($0) - 1)
						found = 2
						next
					}
					
#	            	gsub("<0xe2><0xad><0x90><0xef><0xb8><0x8f>", "", title)
	            	gsub("⭐️", "", title)

					picname = tolower(title)

                    if ($0 ~ /tvg-logo=/)
                    {
					    i = index($0, "tvg-logo=\"") + 10
    	                j = index(substr($0, i), "\"") - 1
       	                pic = substr($0, i, j)
                    }

				    if ( pic == "" )
                    {
    	            	gsub(" ", ".", picname)
					    pic = "http://openaaf.dyndns.tv/mediathek/menu/" picname ".jpg"
                    }

                    if (pic ~ /.png/)
                        picext = "png"
                    else
                        picext = "jpg"

					if (++dup[title] == 1 && title != "")
					{
                        titletmp = titletmp title
						print title "#" SRC " " SRC " search \x27" PAGE "\x27 \x27" title "\x27#" pic "#" PICNAME "." picname ".jpg#" NAME "#0"
                    }
					newpage = ""
					title = ""
					picname = ""
					pic = ""
					next
				}
			}
            /^http/ \
			{
                if (found == 2)
                {
	                newpage = substr($0, 1, length($0) - 1)
				    if (title != "" && title !~ "= = =")
				    {
					    piccount += 1
					    print title "#" newpage "&tslivemode=1#" pic "#" PICNAME "." picname "." picext "#" NAME "#2"
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

    echo $TMP/$FILENAME.list
}

search()
{
    rm "$TMP/$FILENAME.list"
    NEXT=$(echo $NEXT | tr '+' ' ')
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - $URL | awk -v NEXT=$NEXT -v TMP=$TMP -v FILENAME=$FILENAME -v SRC=$SRC -v URL=$URL -v PAGE=$PAGE -v NAME=$NAME -v PICNAME=$PICNAME \
		'
			BEGIN \
			{
				found = 0
				newpage = ""
				piccount = 0

			}
            /#EXTINF/ \
			{
                IGNORECASE=1;
                if ($0 ~ NEXT)
                {
				    if (found == 0)
				    {
					    i = index($0, "tvg-name=\"") + 10
		                j = index(substr($0, i), "\"") - 1
		                title = substr($0, i, j)

					    picname = tolower(title)

                        gsub(/^[(]+/, "", picname)

	                	gsub(/ FHD/, " ", picname)
	                	gsub(/ HD\+/, " ", picname)
	                	gsub(/ HD/, " ", picname)
	                	gsub(/ RAW/, " ", picname)
	                	gsub(/ HEVC/, " ", picname)
                        gsub(/\(.*\)/, "", picname)
	                	gsub(/\+/, "", picname)
	                	gsub(/\//, " ", picname)
                        gsub(/+[)]+/, "", picname)

                        gsub(/^[ \t]+/, "", picname)
                        gsub(/[ \t]+$/, "", picname)
                        gsub(/\t+/, "", picname)

                        gsub(/^[ ]+/, "", picname)
                        gsub(/[ ]+$/, "", picname)
                        gsub(/ +/, "", picname)

                        if ($0 ~ /tvg-logo=/)
                        {
    					    i = index($0, "tvg-logo=\"") + 10
	    	                j = index(substr($0, i), "\"") - 1
	       	                pic = substr($0, i, j)
                        }

					    if ( pic == "" )
						    pic = "http://openaaf.dyndns.tv/mediathek/menu/" picname ".jpg"

                        if (pic ~ /.png/)
                            picext = "png"
                        else
                            picext = "jpg"

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
					    print title "#" newpage "&tslivemode=1#" pic "#" PICNAME "." picname "." picext "#" NAME "#2"
#					    print title "#" newpage "#" pic "#" PICNAME "." piccount ".jpg#" NAME "#2"
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
		' >"$TMP/$FILENAME.list"
	fi
	# 30. gebe titan den list namen mit pfad zurueck
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
