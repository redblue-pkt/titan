#!/bin/sh
# first sh box parser for titannit mfg obi

SRC=$1
INPUT=$2
PAGE=$3
NEXT=$4
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
URL=https://www2.vjackson.info
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

getkey()
{
    if [ ! -e $TMP/vavoo.7.signed.base64.timestamp.sed ];then
        $curlbin -X POST http://www.vavoo.tv/api/box/guest > $TMP/vavoo.1.guest
        token=$(cat $TMP/vavoo.1.guest | awk 'BEGIN {} /token/ { i = index($0, "token\":\"") + 8;j = index(substr($0, i), "\"") - 1;token = substr($0, i, j); print token; next}')
        echo $token > $TMP/vavoo.2.token
        $curlbin --data "token=$token" -X POST https://www.vavoo.tv/api/box/ping2 > $TMP/vavoo.3.ping2
        signed=$(cat $TMP/vavoo.3.ping2 | awk 'BEGIN {} /signed/ { i = index($0, "signed\":\"") + 9;j = index(substr($0, i), "\"") - 1;signed = substr($0, i, j); print signed; next}')
        echo $signed > $TMP/vavoo.4.signed
        base64 -d $TMP/vavoo.4.signed > $TMP/vavoo.5.signed.base64
#        cat $TMP/vavoo.5.signed.base64 | sed -nr 's/.*\\"validUntil\\":([^:]+),.*/\1/p' > $TMP/vavoo.6.signed.base64.timestamp
#        timestamp=$(cat $TMP/vavoo.6.signed.base64.timestamp)
#        cat $TMP/vavoo.5.signed.base64 | sed 's#false#true#g' | sed 's#\\"Daily usage duration reached\\"#null#g' | sed 's#\\"version\\":null#\\"version\\":\\"2.6\\"#g' | sed 's#\\"serivce\\":null#\\"serivce\\":\\"1.2.26\\"#g' | sed 's#\\"platform\\":null#\\"platform\\":\\"vavoo\\"#g' | sed "s#160#260#g" | sed "s#3.#4.#g" | sed "s#5.#6.#g" | sed "s#7.#8.#g" | sed "s#9.#0.#g" > $TMP/vavoo.7.signed.base64.timestamp.sed
        cat $TMP/vavoo.5.signed.base64 | sed 's#false#true#g' | sed 's#\\"Daily usage duration reached\\"#null#g' | sed 's#\\"version\\":null#\\"version\\":\\"2.6\\"#g' | sed 's#\\"serivce\\":null#\\"serivce\\":\\"1.2.26\\"#g' | sed 's#\\"platform\\":null#\\"platform\\":\\"vavoo\\"#g' | sed "s#:16#:26#g" > $TMP/vavoo.7.signed.base64.timestamp.sed
    fi
}

mainmenu()
{
	echo "Category#$SRC $SRC category '/live2/index'#http://openaaf.dyndns.tv/mediathek/menu/category.jpg#category.jpg#$NAME#0" >$TMP/$FILENAME.list
	echo "Suchen#$SRC $SRC search '/live2/index' '%search%' 1#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >>$TMP/$FILENAME.list
	echo "$TMP/$FILENAME.list"
}

category()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
        getkey
        vavoo_auth=$(base64 $TMP/vavoo.7.signed.base64.timestamp.sed | tr -d '\n')

		$curlbin -o - $URL$PAGE | sed -e "s/\.ts$/\.ts?n=1\&b=5\&vavoo_auth=$vavoo_auth|User-Agent=VAVOO\/2.6/g" -e 's/^http:/#EXTVLCOPT:http-user-agent=VAVOO\/2.6\nhttp:/g' | awk -v TEST=$TEST -v TMP=$TMP -v FILENAME=$FILENAME -v SRC=$SRC -v URL=$URL -v PAGE=$PAGE -v NAME=$NAME -v PICNAME=$PICNAME \
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

                    if(title == "Germany")
                        pic = URL "/live2/logo/3597546190.jpeg"

                    if(title == "France")
                        pic = URL "/live2/logo/1277148681.jpeg"

                    if(title == "Albania")
                        pic = URL "/live2/logo/2745766782.jpeg"

                    # fix dublicate lines
					if (++dup[title] == 1 && title != "")
					{
                        titletmp = titletmp title
#						print title "#" SRC " " SRC " search \x27" PAGE "\x27 \x27" title "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#0"
						print title "#" SRC " " SRC " search \x27" PAGE "\x27 \x27" title "\x27#" pic "#" PICNAME "." picname ".jpg#" NAME "#0"
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


write()
{
    search 1
}

writecmd()
{
    NEXT=$(echo $NEXT | tr '+' ' ')

#    remove $NEXT
    search 2
    save $NEXT
    killall -9 titan
}

save()
{
    if [ ! -z "$1" ];then 
        NEXT=$1
    fi

    cat /mnt/settings/channel.tmp | sort -u > /mnt/settings/channel
    cp -a /mnt/settings/bouguets.tithek.autoupdate."$NAME"."$NEXT".tv.tmp /mnt/settings/bouguets.tithek.autoupdate."$NAME"."$NEXT".tv

    cat /mnt/settings/bouquets.cfg.tmp | awk '!seen[$0]++' > /mnt/settings/bouquets.cfg
    cat /mnt/settings/transponder.tmp | awk '!seen[$0]++' > /mnt/settings/transponder
    sed s/"^ *"// -i /mnt/settings/channel
    remove $NEXT
}

remove()
{
    if [ ! -z "$1" ];then 
        NEXT=$1
    fi

    rm /mnt/settings/bouquets.cfg.* > /dev/null 2>&1
    rm /mnt/settings/transponder.* > /dev/null 2>&1
    rm /mnt/settings/channel.* > /dev/null 2>&1
    rm /mnt/settings/bouguets.tithek.autoupdate."$NAME"."$NEXT".tv.* > /dev/null 2>&1
}

search()
{
    NEXT=$(echo $NEXT | tr '+' ' ')

    ADD2CHANNEL=0
    if [ ! -z "$1" ];then 
        ADD2CHANNEL=$1
        remove $NEXT
        rm /mnt/settings/bouguets.tithek.autoupdate."$NAME"."$NEXT".tv.* > /dev/null 2>&1
    fi

	if [ ! -e "$TMP/$FILENAME.list" ] || [ "$ADD2CHANNEL" != "0" ]; then
        getkey
        vavoo_auth=$(base64 $TMP/vavoo.7.signed.base64.timestamp.sed | tr -d '\n')

		$curlbin -o - $URL$PAGE | sed -e "s/\.ts$/\.ts?n=1\&b=5\&vavoo_auth=$vavoo_auth|User-Agent=VAVOO\/2.6/g" -e 's/^http:/#EXTVLCOPT:http-user-agent=VAVOO\/2.6\nhttp:/g' | awk -v ADD2CHANNEL="$ADD2CHANNEL" -v NEXT="$NEXT" -v SRC=$SRC -v URL=$URL -v PAGE=$PAGE -v NAME=$NAME -v PICNAME=$PICNAME \
		'
			BEGIN \
			{
				found = 0
				newpage = ""
				piccount = 0
                
                if(ADD2CHANNEL != 0)
                {
                    cmd = "cp -a /mnt/settings/channel /mnt/settings/channel.tmp"
                    system(cmd)

                    cmd = "cp -a /mnt/settings/transponder /mnt/settings/transponder.tmp"
                    system(cmd)

                    cmd = "cp -a /mnt/settings/bouquets.cfg /mnt/settings/bouquets.cfg.tmp"
                    system(cmd)
                }
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
                            gsub("https://vjackson.info", URL, pic)

                        }

					    if ( pic == "" )
						    pic = "http://openaaf.dyndns.tv/mediathek/menu/" picname ".jpg"

                        if (pic ~ /.png/)
                            picext = "png"
                        else
                            picext = "jpg"  

# show 1 sender
#                       gsub(/(.*)/, "", title)

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
				    if (++dup[title] == 1 && title != "" && title !~ "= = =")
				    {
					    piccount += 1

					    i = index($0, "/live/") + 6
		                j = index(substr($0, i), ".ts") - 1
		                id = substr($0, i, j)

                        if(ADD2CHANNEL != 0)
                        {
                            epgurl = "http://epgurl.dummy.to/" id
                            cmd = "echo \"" title "#" id "#0#0#0#0#0#0#0#0#0#0#" newpage "#" epgurl "\" >> /mnt/settings/channel.tmp"
                            system(cmd)

                            cmd = "echo \"" id "#0#0#0#192#0#0#0#0#0#0#2\" >> /mnt/settings/transponder.tmp"
                            system(cmd)

                            cmd = "echo \"0#" id "\" >> /mnt/settings/bouguets.tithek.autoupdate." NAME "." NEXT ".tv.tmp"
                            system(cmd)

                            if(++dup[cmd] == 1)
                                cmd = "echo \"" NAME "-" NEXT "#0#/mnt/settings/bouguets.tithek.autoupdate." NAME "." NEXT ".tv\" >> /mnt/settings/bouquets.cfg.tmp"
                            system(cmd)
                        }

					    print title "#" newpage "&tslivemode=1#" pic "#" PICNAME "." picname "." picext "#" NAME "#2"
#					    print title "#" newpage "#" pic "#" PICNAME "." piccount "." picext "#" NAME "#2"
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
			END \
				{
                    if(ADD2CHANNEL != 0)
                    {
#                        cmd = "cat /mnt/settings/channel.tmp | sort -u > /mnt/settings/channel"
#                        system(cmd)
#
##                        cmd = "cat /mnt/settings/transponder.tmp | sort -u > /mnt/settings/transponder"
##                        system(cmd)
#
##                        cmd = "cat /mnt/settings/bouguets.tithek.autoupdate." NAME "." NEXT ".tv.tmp | sort -u > /mnt/settings/bouguets.tithek.autoupdate." NAME "." NEXT ".tv"
#                        cmd = "cp -a /mnt/settings/bouguets.tithek.autoupdate." NAME "." NEXT ".tv.tmp /mnt/settings/bouguets.tithek.autoupdate." NAME "." NEXT ".tv"
#                        system(cmd)
#
##                       cmd = "cat /mnt/settings/bouquets.cfg.tmp | sort -u > /mnt/settings/bouquets.cfg"
##                       system(cmd)
                    }
				}
		' >$TMP/$FILENAME.list
	fi

    if [ "$ADD2CHANNEL" != "0" ];then
        error=0
        if [ ! -e /mnt/settings/bouquets.cfg.tmp ];then error=1; fi
        if [ ! -e /mnt/settings/transponder.tmp ];then error=1; fi
        if [ ! -e /mnt/settings/channel.tmp ];then error=1; fi
        if [ ! -e /mnt/settings/bouguets.tithek.autoupdate."$NAME"."$NEXT".tv.tmp ];then error=1; fi
        if [ "$error" == "1" ];then
            echo "errormsg: add2channel "$NAME"-"$NEXT" has been creating error as .tmp"
        else
            echo "add2channel "$NAME"-"$NEXT" has been completed as .tmp"
        fi
    else
        cat $TMP/$FILENAME.list | sort -u > $TMP/$FILENAME.sort.list
	    echo "$TMP/$FILENAME.sort.list"
    fi
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
	write) $INPUT;;
	writecmd) $INPUT;;
	remove) $INPUT;;
	save) $INPUT;;
esac
