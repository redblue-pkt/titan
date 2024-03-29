#!/bin/sh
# first sh box parser for titannit mfg obi

SRC=$1
INPUT=$2
PAGE=$3
NEXT=$4
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`

URL="http://dummy.url"
NAME="LoadM3U"

case $2 in
	init)	;;
	*) 	. /tmp/localhoster/hoster.sh
	   	mkdir $TMP > /dev/null 2>&1
		FILENAME="$PARSER $INPUT $PAGE $NEXT"
	   	FILENAME=$(echo $FILENAME | tr '&' '.' | tr '/' '.' | tr '?' '.' | tr '=' '.' | sed -e 's/\&\+/./g' -e 's#\/\+#.#g' -e 's/\?\+/./g' -e 's/;\+/./g' -e 's/=\+/./g' -e 's/ \+/./g' -e 's/\.\+/./g')
		if [ -z "$FILENAME" ]; then FILENAME=none;fi
		PICNAME="$FILENAME"
		if [ `cat /mnt/config/titan.cfg | grep tithek_loadm3u_url= | wc -l` -eq 1 ];then
			URL=$(cat /mnt/config/titan.cfg | grep "tithek_loadm3u_url=" | sed 's/tithek_loadm3u_url=//g')
            if [ `cat /mnt/config/searchhistory | grep ^"$URL" | wc -l` -eq 0 ];then
    #echo " " >> /mnt/config/titan.cfg
                echo "$URL" >> /mnt/config/searchhistory
                sed -e '/^\s*$/' -i /mnt/config/searchhistory
            fi
		else
            if [ -e "/mnt/swapextensions/usr/local/share/titan/plugins/network/xupnpd/etc/playlists" ];then
                URL="/mnt/swapextensions/usr/local/share/titan/plugins/network/xupnpd/etc/playlists"
            else
    			URL="http://dummy.url"
            fi
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
    	echo "Add Your LoadM3U (IpTV) Url or local dir from m3u file on Tithek > Settings (Menu) > LoadM3U (IpTV) Url##http://openaaf.dyndns.tv/mediathek/menu/category.jpg#category.jpg#$NAME#0" >$TMP/$FILENAME.list
    else
    	echo "Category#$SRC $SRC category#http://openaaf.dyndns.tv/mediathek/menu/category.jpg#category.jpg#$NAME#0" >$TMP/$FILENAME.list
     	echo "Suchen#$SRC $SRC search '$URL' '%search%' 1#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >>$TMP/$FILENAME.list
     	echo "Auswahl#$SRC $SRC search '%search%' '%search%' 1#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >>$TMP/$FILENAME.list
    fi

	echo "$TMP/$FILENAME.list"
}

category()
{
#needed without write URL to titan.cfg
#    if [ "$PAGE" == "$NEXT" ];then
#        URL=$PAGE
#    fi
    rm "$TMP/$FILENAME.list"
    if [ ! -e "$TMP/$FILENAME.list" ]; then
        if [ $(echo "$URL" | grep ^/ | wc -l) -eq 1 ];then
            if [ $(echo "$URL" | awk '{ split($0, a, ".");print a[length(a)]}') == "m3u" ];then
		        TITLE=`echo $URL | sed 's/.m3u//' | awk '{ split($0, a, "/");print a[length(a)]}'`
		        filename=`echo $TITLE`
		        echo "$TITLE#$SRC $SRC search '$URL' '$TITLE' 1#http://openaaf.dyndns.tv/mediathek/menu/$filename.jpg#$filename.jpg#$NAME#0" >> $TMP/$FILENAME.list
            else
                ls -1 "$URL" > $TMP/cache.$FILENAME.1

                while read -u 3 ROUND; do
		            TITLE=`echo $ROUND | sed 's/.m3u//'`
		            filename=`echo $TITLE`
		            echo "$TITLE#$SRC $SRC search '$URL/$ROUND' '$TITLE' 1#http://openaaf.dyndns.tv/mediathek/menu/$filename.jpg#$filename.jpg#$NAME#0" >> $TMP/$FILENAME.list
        		done 3<$TMP/cache.$FILENAME.1
            fi
        else
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
                            #EXTINF:-1,DE: ARTE
#						    i = index($0, ":-1,") + 4
#						    title = substr($0, i, length($0) - 1)
                            split($0, a, ",")
                            tag = a[length(a)]
                            title = tag

#                            if (title ~ /:/)
#                            {
#                                split(title, b, ":")
#                                tag2 = b[length(b)]
#                                tag3 = b[1]
#                                gsub(/^ /, "", tag3)
#
#                                title = tag2 " (" tag3 ")"
#                            }

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
	fi

    echo $TMP/$FILENAME.list
}

#sh4 error
#/tmp/localparser/vavoo.sh: line 474: shuf: not found
#awk: bad regex '+[)]+': Invalid preceding regular expression

search()
{
#    if [ $(echo "$URL" | grep ^/ | wc -l) -eq 1 ] && [ -d "$URL" ];then
#        exit
#    fi
    if [ "$PAGE" == "$NEXT" ];then
#echo 11
        if [ `cat /mnt/config/titan.cfg | grep "tithek_loadm3u_url=" | wc -l` -eq 0 ];then
#echo " " >> /mnt/config/titan.cfg
            echo -en "\ntithek_loadm3u_url=$PAGE" >> /mnt/config/titan.cfg
            sed -e '/^\s*$/' -i /mnt/config/titan.cfg
        else
            sed "s#tithek_loadm3u_url=.*#tithek_loadm3u_url=$PAGE#g" -i /mnt/config/titan.cfg
        fi
#echo 22
        $SRC $SRC category $PAGE $NEXT 1
        exit
    fi
    echo 3 > /proc/sys/vm/drop_caches

    rm "$TMP/$FILENAME.list"
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
            echo "cp /mnt/settings/bouquets.tithek.autoupdate."$NAME"."$NEXT".tv /tmp/settings/bouquets.tithek.autoupdate."$NAME"."$NEXT".tv.org" >> $TMP/$FILENAME.cmd.list
        else
            echo "cp /mnt/settings/channel /tmp/settings/channel.tmp" > $TMP/$FILENAME.cmd.list
        fi
    fi

	if [ ! -e "$TMP/$FILENAME.list" ]; then
        if [ $(echo "$PAGE" | grep ^/ | wc -l) -eq 1 ];then
#        if [ "$PAGE" == "$URL" ];then
		    CMD="cat $PAGE"
        else
            CMD="$curlbin -o - $URL"
        fi
        $CMD | awk -v ADD2CHANNEL=$ADD2CHANNEL -v NEXT="$NEXT" -v TMP=$TMP -v FILENAME=$FILENAME -v SRC=$SRC -v URL=$URL -v PAGE=$PAGE -v NAME=$NAME -v PICNAME=$PICNAME \
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
            /#EXTINF/ \
		    {
                IGNORECASE=1;
#                if ($0 ~ NEXT || PAGE == NEXT)
                if ($0 ~ NEXT || PAGE ~ /^[/]/)
                {
			        if (found == 0)
			        {
					    if ($0 ~ /tvg-name=/)
					    {
					        i = index($0, "tvg-name=\"") + 10
		                    j = index(substr($0, i), "\"") - 1
       	                    title = substr($0, i, j)

#                            if (title ~ /:/)
#                            {
#                                split(title, b, ":")
#                                tag2 = b[length(b)]
#                                tag3 = b[1]
#                                gsub(/^ /, "", tag3)
#
#                                title = tag2 " (" tag3 ")"
#                            }
                        }
                        else
                        {
                            #EXTINF:-1,DE: ARTE
                            split($0, a, ",")
                            tag = a[length(a)]
                            title = tag
#                            if (title ~ /:/)
#                            {
#                                split(title, b, ":")
#                                tag2 = b[length(b)]
#                                tag3 = b[1]
#                                gsub(/^ /, "", tag3)
#
#                                title = tag2 " (" tag3 ")"
#                            }
                        }
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
                if (found == 2)
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
                if (found == 1)
                {
#	                newpage = substr($0, 1, length($0) - 1)
	                newpage = substr($0, 1, length($0))
				    if (++dup[title] == 1 && title != "" && title !~ "= = =")
				    {
					    piccount += 1

#					    i = index($0, "/live/") + 6
#		                j = index(substr($0, i), ".ts") - 1
#		                id = substr($0, i, j)

                        #EXTINF:-1,DE: ARTE
                        split(substr($0, 1, length($0)), a, "/")
                        id = a[length(a)]

                        if(ADD2CHANNEL != 0)
                        {
                            epgurl = "http://epgurl.dummy.to/" id

                            if(ADD2CHANNEL == 2)
                            {
#                               newpage2 = newpage
#                                j = index(substr($0, 1), "?n=1&b=5&vavoo_auth=") - 1
#                                newpage2 = substr($0, 1, j)
                                #real    1m3.487s
#                                cmd2 = cmd2 " -e \"s;#http.*/" id ".ts.*VAVOO.*#;#" newpage2 "?n=1\\&b=5\\&vavoo_auth=" vavoo_auth "|User-Agent=VAVOO/2.6\\&tslivemode=1#;g\" \x5c\n"
                                newpage3 = newpage
                                gsub("(&)", "\\\\\\&", newpage3) 
#                                cmd2 = cmd2 " -e \"s;#http.*/" id ".ts.*VAVOO.*#;#" newpage3 "\\&tslivemode=1#;g\" \x5c\n"
                                cmd2 = cmd2 " -e \"s;#http.*/" id "\\&tslivemode=1.*#;#" newpage3 "\\&tslivemode=1#;g\" \x5c\n"
                            }
                            else if(ADD2CHANNEL == 3)
                            {
#                                newpage2 = newpage
#                                j = index(substr($0, 1), "?n=1&b=5&vavoo_auth=") - 1
#                                newpage2 = substr($0, 1, j)
                                #real    1m27.071s
#                                cmd3 = cmd3 "sed \"s;#http.*/" id ".ts.*VAVOO/2.6&tslivemode=1;#" newpage2 "?n=1\\&b=5\\&vavoo_auth=" vavoo_auth "|User-Agent=VAVOO/2.6\\&tslivemode=1;g\" -i /tmp/settings/channel.tmp\n"
                                cmd3 = cmd3 "sed \"s;#http.*/" id ".ts.*VAVOO/#;#" newpage3 "\\&tslivemode=1;g\" -i /tmp/settings/channel.tmp\n"
                                cmd3 = cmd3 "sed \"s;#http.*/" id "\\&tslivemode=1#;#" newpage3 "\\&tslivemode=1;g\" -i /tmp/settings/channel.tmp\n"
                            }
                            else
                            {
                                #real    0m6.421s
                                #user    0m4.576s
                                #sys     0m1.168s
                            }

                            cmd = cmd "echo \"" title "#" id "#0#0#0#0#0#0#0#0#0#0#" newpage "&tslivemode=1#" epgurl "\" >> /tmp/settings/channel.tmp\n"
                            cmd = cmd "echo \"" id "#0#0#0#20001#0#0#0#0#0#0#2\" >> /tmp/settings/transponder.tmp\n"
                            cmd = cmd "echo \"LoadM3U (IpTV)#0#20001#3\" >> /tmp/settings/satellites.tmp\n"
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
	    ' >"$TMP/$FILENAME.list"

	fi
	# 30. gebe titan den list namen mit pfad zurueck
#	echo "$TMP/$FILENAME.list"

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
            MSG="Update all Services with LoadM3U Streamurl and Service Bouquets"
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

######
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
    save $NEXT
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
    cp /tmp/settings/channel /mnt/settings/channel
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

######

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
