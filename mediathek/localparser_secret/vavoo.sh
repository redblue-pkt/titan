#!/bin/sh
# first sh box parser for titannit mfg obi

SRC=$1
INPUT=$2
PAGE=$3
NEXT=$4
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME="VaVoo"

case $2 in
	init)	;;
	*) 	. /tmp/localhoster/hoster.sh
	   	mkdir $TMP > /dev/null 2>&1
		FILENAME="$PARSER $INPUT $PAGE $NEXT"
	   	FILENAME=$(echo $FILENAME | tr '&' '.' | tr '/' '.' | tr '?' '.' | tr '=' '.' | sed -e 's/\&\+/./g' -e 's#\/\+#.#g' -e 's/\?\+/./g' -e 's/;\+/./g' -e 's/=\+/./g' -e 's/ \+/./g' -e 's/\.\+/./g')
		if [ -z "$FILENAME" ]; then FILENAME=none;fi
		PICNAME="$FILENAME"
		URL=`cat /mnt/config/titan.cfg | grep tithek_vavoo_url | grep -v "#" | cut -d "=" -f2`
		if [ -z "$URL" ];then URL=https://www2.vavoo.to; fi
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
	echo "$NAME ($TYPE)#$SRC $SRC mainmenu#http://openaaf.dyndns.tv/mediathek/menu/$PARSER.jpg#$PARSER.jpg#TiThek#66"
}

getkey()
{
#    if [ ! -e $TMP/vavoo.7.signed.base64.timestamp.sed ];then
#        $curlbin -X POST http://www.vavoo.tv/api/box/guest > $TMP/vavoo.1.guest
#        token=$(cat $TMP/vavoo.1.guest | awk 'BEGIN {} /token/ { i = index($0, "token\":\"") + 8;j = index(substr($0, i), "\"") - 1;token = substr($0, i, j); print token; next}')
#        echo $token > $TMP/vavoo.2.token
#        $curlbin --data "token=$token" -X POST https://www.vavoo.tv/api/box/ping2 > $TMP/vavoo.3.ping2
#        signed=$(cat $TMP/vavoo.3.ping2 | awk 'BEGIN {} /signed/ { i = index($0, "signed\":\"") + 9;j = index(substr($0, i), "\"") - 1;signed = substr($0, i, j); print signed; next}')
#        echo $signed > $TMP/vavoo.4.signed
#        base64 -d $TMP/vavoo.4.signed > $TMP/vavoo.5.signed.base64
#        cat $TMP/vavoo.5.signed.base64 | sed 's#false#true#g' | sed 's#\\"Daily usage duration reached\\"#null#g' | sed 's#\\"version\\":null#\\"version\\":\\"2.6\\"#g' | sed 's#\\"serivce\\":null#\\"serivce\\":\\"1.2.26\\"#g' | sed 's#\\"verified\\":true#\\"verified\\":\\"false\\"#g' | sed 's#\\"platform\\":null#\\"platform\\":\\"vavoo\\"#g' | sed "s#:16#:26#g" > $TMP/vavoo.7.signed.base64.timestamp.sed

        $curlbin3 --location --request POST 'https://www.vavoo.tv/api/box/ping2' --header 'Content-Type: application/json' --data "{\"vec\": \"$($curlbin3 https://raw.githubusercontent.com/michaz1988/michaz1988.github.io/master/data.json | sed -nr 's/.*\"([^\"]+)\".*/\1/p' | $BUSYBOXBIN shuf -n 1)\"}" | sed 's#^.*"signed":"##' | sed "s#\"}}##g" | sed 's/".*//' > $TMP/vavoo.4.signed
        cp $TMP/vavoo.4.signed /tmp/vavoo.authkey
        base64 -d $TMP/vavoo.4.signed > $TMP/vavoo.5.signed.base64
        cat $TMP/vavoo.5.signed.base64 > $TMP/vavoo.7.signed.base64.timestamp.sed
#    fi

}

mainmenu()
{
	echo "Category#$SRC $SRC category '/live2/index'#http://openaaf.dyndns.tv/mediathek/menu/category.jpg#category.jpg#$NAME#0" >$TMP/$FILENAME.list
	echo "Suchen#$SRC $SRC search '/live2/index' '%search%' 1#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >>$TMP/$FILENAME.list
	echo "$TMP/$FILENAME.list"
}

category()
{
#rm $TMP/$FILENAME.list
	if [ ! -e "$TMP/$FILENAME.list" ]; then
        getkey
        vavoo_auth=$(base64 $TMP/vavoo.7.signed.base64.timestamp.sed | tr -d '\n')

		$curlbin -o - $URL$PAGE | sed 's!},{!}\n{!g' | awk -v TEST=$TEST -v TMP=$TMP -v FILENAME=$FILENAME -v SRC=$SRC -v URL=$URL -v PAGE=$PAGE -v NAME=$NAME -v PICNAME=$PICNAME \
		'
			BEGIN \
			{
				suche = 1
				newpage = ""
				piccount = 0
			}
            /\{\"group\":/ \
			{
				if (suche == 1)
				{
				    i = index($0, "\{\"group\":\"") + 10
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

#sh4 error
#/tmp/localparser/vavoo.sh: line 474: shuf: not found
#awk: bad regex '+[)]+': Invalid preceding regular expression

search()
{
#rm $TMP/$FILENAME.list
    echo 3 > /proc/sys/vm/drop_caches

    NEXT=$(echo $NEXT | tr '+' ' ')

#    ADD2CHANNEL=2

    ADD2CHANNEL=0
    if [ ! -z "$1" ];then 
        ADD2CHANNEL=$1
        remove "$NEXT"
        rm /mnt/settings/bouquets.tithek.autoupdate."$NAME"."$NEXT".tv.* > /dev/null 2>&1

        if [ ! -e /tmp/settings ];then mkdir /tmp/settings; fi
        if [ -e /mnt/settings/bouquets.tithek.autoupdate."$NAME"."$NEXT".tv ];then
            echo "LIST=$(cat /mnt/settings/bouquets.tithek.autoupdate."$NAME"."$NEXT".tv | sed 's/0#/#/g' | tr '\n' '#' | sed 's/##/#\\|#/g' | sed "s/^#/'/" | sed -e "s/#$/'/g")" >$TMP/$FILENAME.cmd.list
            echo "cat /mnt/settings/channel | grep -v \$LIST >> /tmp/settings/channel.tmp" >> $TMP/$FILENAME.cmd.list
            echo "cp '/mnt/settings/bouquets.tithek.autoupdate."$NAME"."$NEXT".tv' '/tmp/settings/bouquets.tithek.autoupdate."$NAME"."$NEXT".tv.org'" >> $TMP/$FILENAME.cmd.list
        else
            echo "cp /mnt/settings/channel /tmp/settings/channel.tmp" > $TMP/$FILENAME.cmd.list
        fi
    fi

	if [ ! -e "$TMP/$FILENAME.list" ] || [ "$ADD2CHANNEL" != "0" ]; then
        getkey
        vavoo_auth=$(base64 $TMP/vavoo.7.signed.base64.timestamp.sed | tr -d '\n')

		$curlbin -o - $URL$PAGE | sed 's!},{!}\n{!g' | awk -v vavoo_auth="$vavoo_auth" -v ADD2CHANNEL="$ADD2CHANNEL" -v NEXT="$NEXT" -v SRC=$SRC -v URL=$URL -v PAGE=$PAGE -v NAME=$NAME -v PICNAME=$PICNAME \
		'
			BEGIN \
			{
				found = 0
				newpage = ""
				piccount = 0
                
                if(ADD2CHANNEL != 0)
                {
                    cmd = cmd "cp -a /mnt/settings/channel /tmp/settings/channel.org\n"
#                    cmd = cmd "cp -a /tmp/settings/channel.org /tmp/settings/channel.tmp\n"
                    cmd = cmd "cp -a /mnt/settings/transponder /tmp/settings/transponder.org\n"
                    cmd = cmd "cp -a /tmp/settings/transponder.org /tmp/settings/transponder.tmp\n"
                    cmd = cmd "cp -a /mnt/settings/satellites /tmp/settings/satellites.org\n"
                    cmd = cmd "cp -a /tmp/settings/satellites.org /tmp/settings/satellites.tmp\n"
                    cmd = cmd "cp -a /mnt/settings/bouquets.cfg /tmp/settings/bouquets.cfg.org\n"
                    cmd = cmd "cp -a /tmp/settings/bouquets.cfg.org /tmp/settings/bouquets.cfg.tmp\n"
                    cmd2 = "sed "
                }
			}
            /\{\"group\":/ \
			{
                IGNORECASE=1;
                if ($0 ~ NEXT)
                {
				    if (found == 0)
				    {
			            i = index($0, "\"name\":\"") + 8
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

#sh4 error
#                        gsub(/+[)]+/, "", picname)
                        gsub(/\+[)]\+/, "", picname)

                        gsub(/^[ \t]+/, "", picname)
                        gsub(/[ \t]+$/, "", picname)
                        gsub(/\t+/, "", picname)

                        gsub(/^[ ]+/, "", picname)
                        gsub(/[ ]+$/, "", picname)
                        gsub(/ +/, "", picname)

			            i = index($0, "\"logo\":\"") + 8
    	                j = index(substr($0, i), "\"") - 1
       	                pic = substr($0, i, j)
                        gsub("https://vjackson.info", URL, pic)

					    if ( pic == "" )
						    pic = "http://openaaf.dyndns.tv/mediathek/menu/" picname ".jpg"

                        if (pic ~ /.png/)
                            picext = "png"
                        else
                            picext = "jpg"  

			            i = index($0, "\"url\":\"") + 7
                        j = index(substr($0, i), "\"") - 1
                        newpage = substr($0, i, j)
                        newpage = newpage "?n=1&b=5&vavoo_auth=" vavoo_auth "|User-Agent=VAVOO/2.6"

				        if (++dup[title] == 1 && title != "" && title !~ "= = =")
				        {
					        piccount += 1

					        i = index(newpage, "/live2/play/") + 12
		                    j = index(substr(newpage, i), ".ts") - 1
		                    id = substr(newpage, i, j)

                            if(ADD2CHANNEL != 0)
                            {
                                epgurl = "http://epgurl.dummy.to/" id

                                if(ADD2CHANNEL == 2)
                                {
                                    #real    1m3.487s
                                    newpage3 = newpage
                                    gsub("(&)", "\\\\\\&", newpage3) 
                                    cmd2 = cmd2 " -e \"s;#http.*/" id ".ts.*VAVOO.*#;#" newpage3 "\\&tslivemode=1#;g\" \x5c\n"
                                }
                                else
                                {
                                    #real    0m6.421s
                                    #user    0m4.576s
                                    #sys     0m1.168s
                                }

                                cmd = cmd "echo \"" title "#" id "#0#0#0#0#0#0#0#0#0#0#" newpage "&tslivemode=1#" epgurl "\" >> /tmp/settings/channel.tmp\n"
                                cmd = cmd "echo \"" id "#0#0#0#20000#0#0#0#0#0#0#2\" >> /tmp/settings/transponder.tmp\n"
                                cmd = cmd "echo \"VaVoo (IpTV)#0#20000#3\" >> /tmp/settings/satellites.tmp\n"
                                cmd = cmd "echo \"0#" id "\" >> \"/tmp/settings/bouquets.tithek.autoupdate." NAME "." NEXT ".tv.tmp\"\n"
                                if(++dup[cmd4] == 1)
                                    cmd4 = cmd4 "echo \"" NAME "-" NEXT "#0#/mnt/settings/bouquets.tithek.autoupdate." NAME "." NEXT ".tv\" >> /tmp/settings/bouquets.cfg.tmp\n"
                            }
                            else
                            {
        					    print title "#" newpage "&tslivemode=1#" pic "#" PICNAME "." picname "." picext "#" NAME "#2"
    #   					    print title "#" newpage "#" pic "#" PICNAME "." piccount "." picext "#" NAME "#2"
    #	    					print title " (" extra ")#" SRC " " SRC " play \x27" newpage "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#111"
    #	    					print title "#" SRC " " SRC " hoster \x27" newpage "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#111"
                            }
				        }
				        
				        newpage = ""
				        title = ""
				        picname = ""
				        pic = ""
                        found = 0
				        next
                    }
				}
			}
			END \
				{
                    if(ADD2CHANNEL != 0)
                    {
                        cmd5 = cmd5 "cat /tmp/settings/channel.tmp | sort -u > /tmp/settings/channel\n"
#                        cmd5 = cmd5 "cat /tmp/settings/transponder.tmp | sort -u > /tmp/settings/transponder\n"
                        cmd5 = cmd5 "cp -a \"/tmp/settings/bouquets.tithek.autoupdate." NAME "." NEXT ".tv.tmp\" \"/tmp/settings/bouquets.tithek.autoupdate." NAME "." NEXT ".tv\"\n"
                        cmd5 = cmd5 "cat /tmp/settings/bouquets.cfg.tmp | awk \x27!seen[$0]++\x27 > /tmp/settings/bouquets.cfg\n"
                        cmd5 = cmd5 "cat /tmp/settings/transponder.tmp | awk \x27!seen[$0]++\x27 > /tmp/settings/transponder\n"
                        cmd5 = cmd5 "cat /tmp/settings/satellites.tmp | awk \x27!seen[$0]++\x27 > /tmp/settings/satellites\n"

                        cmd5 = cmd5 "sed s/\"^ *\"// -i /mnt/settings/channel\n"

                        print cmd
 
                        if(ADD2CHANNEL == 2)
                        {
                            cmd2 = cmd2 " -i /tmp/settings/channel.tmp\n"
                            print cmd2
                        }
                        else if(ADD2CHANNEL == 3)
                            print cmd3

                        print cmd4
                        print cmd5
                    }
				}
		' >$TMP/$FILENAME.list
	fi 

    if [ "$ADD2CHANNEL" != "0" ];then
        if [ -e $TMP/$FILENAME.list ];then 
            cat $TMP/$FILENAME.list >> $TMP/$FILENAME.cmd.list
#            cp $TMP/$FILENAME.list $TMP/$FILENAME.cmd.list
#            chmod 755 $TMP/$FILENAME.list
#            $TMP/$FILENAME.list
            chmod 755 $TMP/$FILENAME.cmd.list
            $TMP/$FILENAME.cmd.list
        fi
        error=0
        if [ ! -e /tmp/settings/bouquets.cfg ];then error=1; fi
        if [ ! -e /tmp/settings/transponder ];then error=1; fi
        if [ ! -e /tmp/settings/channel ];then error=1; fi
        if [ ! -e /tmp/settings/bouquets.tithek.autoupdate."$NAME"."$NEXT".tv ];then error=1; fi
        if [ "$ADD2CHANNEL" == "1" ];then
            MSG="Create Service Bouquets"
        elif [ "$ADD2CHANNEL" == "2" ];then
            MSG="Update all Services with VaVoo Streamurl and Service Bouquets"
        fi
        if [ "$error" == "1" ];then
            echo "errormsg: $MSG "$NAME"-"$NEXT" has been creating error in /tmp/settings"
        else
            echo "$MSG "$NAME"-"$NEXT" has been completed in /tmp/settings"
        fi
    else
        cat $TMP/$FILENAME.list | sort -u > $TMP/$FILENAME.sort.list
	    echo "$TMP/$FILENAME.sort.list"
    fi
}

create_service_bouquets()
{
    search 1
}

#backslash hex
#awk -F, '{print "\x5c" }'
#a single quote hex
#awk -F, '{print "\047" }'
#a single quote octa
#awk -F, '{print "\x27" }'

update_service_bouquets()
{
    NEXT=$(echo $NEXT | tr '+' ' ')

    remove "$NEXT"
    search 1
    save "$NEXT"
}

update_all_channels()
{
    NEXT=$(echo $NEXT | tr '+' ' ')

    remove "$NEXT"
    search 2
    save "$NEXT"
}

writecmd()
{
#rm -rf /mnt/settings
#cp -a /etc/titan.restore/mnt/settings /mnt

    NEXT=$(echo $NEXT | tr '+' ' ')

    remove "$NEXT"
#only create 0m8.507s
#    search 2
#create and update sat channels with streamid (fast) sed -e .. -i 1m2.206s
    search 2
#create and update sat channels with streamid (slow) sed -- -i 1m28.566s
#    search 4
#create and update sat channels with streamid (very slow) echo sed -i 3m24.186s
#    search 5

    save "$NEXT"
    killall -9 titan
}

save()
{
    if [ ! -z "$1" ];then 
        NEXT="$1"
    fi

    cp /tmp/settings/bouquets.cfg /mnt/settings/bouquets.cfg
    cp /tmp/settings/bouquets.tithek.autoupdate."$NAME"."$NEXT".tv /mnt/settings/bouquets.tithek.autoupdate."$NAME"."$NEXT".tv 

    freespace=`getfreespace /mnt/settings`
    filesize=`getfreespace /tmp/settings/channel`
    if [ "$freespace" -gt "$filesize" ]; then
        cp /tmp/settings/channel /mnt/settings/channel
    else
        vavoo_auth=$(cat /tmp/vavoo.authkey | tr -d '\n')
        cat /tmp/settings/channel | sed "s/vavoo_auth=$vavoo_auth|User-Agent=/vavoo_auth=12345|User-Agent=/g" > /tmp/settings/channel.wokey
        cp /tmp/settings/channel.wokey /mnt/settings/channel
    fi
    cp /tmp/settings/transponder /mnt/settings/transponder
    cp /tmp/settings/satellites /mnt/settings/satellites

#    cat /tmp/settings/channel.tmp | sort -u > /mnt/settings/channel
#    cp -a /tmp/settings/bouquets.tithek.autoupdate."$NAME"."$NEXT".tv.tmp /mnt/settings/bouquets.tithek.autoupdate."$NAME"."$NEXT".tv
#    cat /tmp/settings/bouquets.cfg.tmp | awk '!seen[$0]++' > /mnt/settings/bouquets.cfg
#    cat /tmp/settings/transponder.tmp | awk '!seen[$0]++' > /mnt/settings/transponder
#    sed s/"^ *"// -i /mnt/settings/channel
#    remove $NEXT
}

remove()
{
    if [ ! -z "$1" ];then 
        NEXT="$1"
    fi

    if [ -e /tmp/settings ];then
        rm -rf /tmp/settings
    else
        rm /mnt/settings/bouquets.cfg.* > /dev/null 2>&1
        rm /mnt/settings/transponder.* > /dev/null 2>&1
        rm /mnt/settings/channel.* > /dev/null 2>&1
        rm /mnt/settings/satellites.* > /dev/null 2>&1
        rm /mnt/settings/bouquets.tithek.autoupdate."$NAME"."$NEXT".tv.* > /dev/null 2>&1
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
	create_service_bouquets) $INPUT;;
    update_service_bouquets) $INPUT;;
    update_all_channels) $INPUT;;
	writecmd) $INPUT;;
	remove) $INPUT;;
	save) $INPUT;;
esac
