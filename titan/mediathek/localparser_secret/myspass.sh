#!/bin/sh
# first sh box parser for titannit mfg obi

SRC=$1
INPUT=$2
PAGE=$3
NEXT=$4
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`

URL=http://www.myspass.de
#URL=http://m.myspass.de
NAME="MySpass"

case $2 in
	init)	;;
	*) 	. /tmp/localhoster/hoster.sh
	   	mkdir $TMP > /dev/null 2>&1
		FILENAME="$PARSER $INPUT $PAGE $NEXT $PAGE2"
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
	echo "Von A-Z#$SRC $SRC season 'sendungen-a-bis-z/' 1#http://openaaf.dyndns.tv/mediathek/menu/shows.jpg#a-z.jpg#$NAME#0" >$TMP/$FILENAME.list
	echo "Suchen#$SRC $SRC search 'frontend/json/search-videos.php?QUERY_STRING=%search%' 1#http://openaaf.dyndns.tv/mediathek/menu/search.jpg#search.jpg#$NAME#112" >>$TMP/$FILENAME.list
	echo "$TMP/$FILENAME.list"
}

season()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - $URL/$PAGE | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT \
		'
			# BEGIN variable setzen
			BEGIN \
			{
				# setzt suchvariable auf 0 vor dem start
				piccount = 0
				pages = "1"
			}
			/<div id=\"content\" class=\"container\"/ \
			{
                found = 1
            }
			/<a href=/ \
			{
                if(found == 1)
                {
    				i = index($0, "<a href=\"") + 9
    	       		j = index(substr($0, i), "\"") - 1
	    			url = substr($0, i, j)
                }
            }
			/data-src=/ \
			{
                if(found == 1)
                {
				    i = index($0, "data-src=\"") + 10
	                j = index(substr($0, i), "\"") - 1
				    pic = substr($0, i, j)
                }
            }
			/ alt=\"/ \
			{
                if(found == 1)
                {
	    			i = index($0, "alt=\"") + 5
               		j = index(substr($0, i), "\"") - 1
	    			title = substr($0, i, j)
                }
            }
			/data-srcset=/ \
			{
                if(found == 1)
                {
	    			i = index($0, "alt=\"") + 5
               		j = index(substr($0, i), "\"") - 1
	    			title = substr($0, i, j)
                    if(title != "")
                    {
				        piccount += 1
				        if ( pic == "" )
				        {
	                    			pic = "http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
				        }
				        else
					        pic = "http:" pic

				        print title "#" SRC " " SRC " year \x27" url "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#0"
                        title = ""
                        url = ""
                        pic = ""
                    }
                }
			}
			/<img class=/ \
			{
                if(found == 1)
                {
    				i = index($0, " src=\"") + 6
	           		j = index(substr($0, i), "\"") - 1
	    			pic = substr($0, i, j)

                    if(title != "")
                    {
				        piccount += 1
				        if ( pic == "" )
				        {
	                    			pic = "http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
				        }
				        else
					        pic = "http:" pic

				        print title "#" SRC " " SRC " year \x27" url "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#0"
                        title = ""
                        url = ""
                        pic = ""
                    }
                }
            }
			/<footer class="footer-info row">/ \
			{
                found = 0
                title = ""
                url = ""
                pic = ""
            }
			END \
			{
#				if (curpage != pages)
#					print "Page (" NEXT + 1 "/" pages ")#" SRC " " SRC " " INPUT " \x27" PAGE "\x27 " NEXT + 1 "#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#" NAME "#0"
			}
		# schreibe alles in die list datei
		' >$TMP/$FILENAME.list
	fi
	# gebe titan den list namen mit pfad zurueck
	echo "$TMP/$FILENAME.list"
}

year()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - $URL/$PAGE | tr -d '\n' | tr -d '\r' | sed 's/<option/\n<option/g' | sed 's!</option>!</option>\n!g' | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT \
		'
			# BEGIN variable setzen
			BEGIN \
			{
				# setzt suchvariable auf 0 vor dem start
				piccount = 0
				pages = "1"
                found = 1
			}
			/<div id=\"content\" class=\"container\"/ \
			{
                found = 1
            }
			/data-remote-endpoint=/ \
            {
    				i = index($0, "data-remote-endpoint=\"") + 22
    	       		j = index(substr($0, i), "\"") - 1
	    			url1 = substr($0, i, j)
            }
			/<option data-remote-args=/ \
			{
                if(found == 1)
                {
    				i = index($0, "<option data-remote-args=\"") + 26
    	       		j = index(substr($0, i), "\"") - 1
	    			url2 = substr($0, i, j)

                    i = index($0, ">") + 1
    	       		j = index(substr($0, i), "<") - 1
	    			title = substr($0, i, j)

                    gsub(/^[ \t]+/,"",title)
                    gsub(/[ \t]+$/,"",title)
                    gsub(/\t+/," / ",title)

    				i = index($0, "&category=") + 10
    	       		j = index(substr($0, i), "\"") - 1
	    			category = substr($0, i, j)

    				print "Staffel " title " (" category ")#" SRC " " SRC " episodes \x27" url1 url2 "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#0"

                }
            }
			/<footer class="footer-info row">/ \
			{
                found = 0
                title = ""
                url = ""
                pic = ""
            }
			END \
			{
#				if (curpage != pages)
#					print "Page (" NEXT + 1 "/" pages ")#" SRC " " SRC " " INPUT " \x27" PAGE "\x27 " NEXT + 1 "#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#" NAME "#0"
			}
		# schreibe alles in die list datei
		' >$TMP/$FILENAME.list
	fi
	# gebe titan den list namen mit pfad zurueck
	echo "$TMP/$FILENAME.list"
}

episodes()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - $URL/$PAGE | sed 's/\\n/\n/g' | tr -d '\\' | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT -v URL=$URL \
		'
			# BEGIN variable setzen
			BEGIN \
			{
				# setzt suchvariable auf 0 vor dem start
				piccount = 0
				pages = "1"
                found = 1
			}
			/<div id=\"content\" class=\"container\"/ \
			{
                found = 1
            }
			/<td><a href=/ \
			{
                if(found == 1)
                {
    				i = index($0, "<a href=\"") + 9
    	       		j = index(substr($0, i), "\"") - 1
	    			url = substr($0, i, j)

	    			i = index($0, "/\">") + 3
               		j = index(substr($0, i), "<") - 1
	    			title = substr($0, i, j)

                    if(title != "")
                    {
				        piccount += 1
				        if ( pic == "" )
				        {
	                    	pic = "http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
				        }
				        else
					        pic = "http:" pic

                        cmd="echo " url " | tr \"/\" \"\\n\" | tail -n2 | head -n1"; cmd|getline id;close(cmd)
                        desc = "curl --connect-timeout 5 https://www.myspass.de/includes/apps/video/getvideometadataxml.php?id=" id " | sed -nr \"s/.*<description><.*\\\[(.*)\\\]\\\].*/\\1/p\""
                        pic = "echo https:// | tr -d \"\\n\" && curl --connect-timeout 5 https://www.myspass.de/includes/apps/video/getvideometadataxml.php?id=" id " | sed -nr \"s;.*<imagePreview><.*\\\[\/\/(.*)\\\]\\\].*;\\1;p\" | tr -d \"\\n\""

				        print "episodes: " title "#" SRC " " SRC " play \x27" url "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#111#" desc

                        desc = "" 
                        title = ""
                        url = ""
                        pic = ""
                    }
                }
            }
			END \
			{
#				if (curpage != pages)
#					print "Page (" NEXT + 1 "/" pages ")#" SRC " " SRC " " INPUT " \x27" PAGE "\x27 " NEXT + 1 "#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#" NAME "#0"
			}
		# schreibe alles in die list datei
		' >$TMP/$FILENAME.list
	fi
	# gebe titan den list namen mit pfad zurueck
	echo "$TMP/$FILENAME.list"
}

search()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - $URL/$PAGE | sed 's/{"uniqueID":/\n{"uniqueID":/g' | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT \
		'
			# BEGIN variable setzen
			BEGIN \
			{
				# setzt suchvariable auf 0 vor dem start
				piccount = 0
				pages = "1"
			}
			/"uniqueID"/ \
			{
#				print "1111111" $0
				i = index($0, "\"title\":\"") + 9
	            		j = index(substr($0, i), "\",\"") - 1
				title = substr($0, i, j)
#				print "title: " title

				i = index($0, "\"broadcast\":\"") + 13
	            		j = index(substr($0, i), "\",\"") - 1
				extra = substr($0, i, j)
#				print "extra: " extra

				i = index($0, "\"url\":\"") + 7
	            		j = index(substr($0, i), "\"") - 1
				newpage = substr($0, i, j)
				gsub(/\\/, "", newpage)

#				print "newpage: " newpage

				i = index($0, "\"teaser\":\"") + 10
	            		j = index(substr($0, i), "\"") - 1
				pic = substr($0, i, j)
				gsub(/\\/, "", pic)
#				print "pic: " pic

				piccount += 1
				if ( pic == "" )
				{
	            			pic = "http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
				}
				print title " (" extra ")#" SRC " " SRC " play \x27" newpage "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#111"
				next
			}
			END \
			{
				if (curpage != pages)
					print "Page (" NEXT + 1 "/" pages ")#" SRC " " SRC " " INPUT " \x27" PAGE "\x27 " NEXT + 1 "#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#" NAME "#0"
			}
		# schreibe alles in die list datei
		' >$TMP/$FILENAME.list
	fi
	# gebe titan den list namen mit pfad zurueck
	echo "$TMP/$FILENAME.list"
}

play()
{
	rm $TMP/$FILENAME.list
	if [ ! -e "$TMP/$FILENAME.list" ]; then
        ID=$(echo $PAGE | tr '/' '\n' | tail -n2 | head -n1)
        ROUND=$ID
		$curlbin https://www.myspass.de/includes/apps/video/getvideometadataxml.php?id=$ID > "$TMP/cache.$FILENAME.$ROUND.1"
		

		TITLE="`cat $TMP/cache.$FILENAME.$ROUND.1 | sed 's/<title><!\[CDATA\[/\nfound=\]/g' | grep ^found= | cut -d"]" -f2`"
		NEWPAGE="`cat $TMP/cache.$FILENAME.$ROUND.1 | sed 's/<url_flv><!\[CDATA\[/\nfound=\]/g' | grep ^found= | cut -d"]" -f2`"

        ID_DUMMY=$(echo $NEWPAGE | sed -nr 's/.*myspass.*\/.*\/(.*+)\/.*/\1/p')
        EXTRAID_DUMMY=$(echo $NEWPAGE | sed -nr 's/.*myspass.*\/(.*+)\/.*\/.*/\1/p')
        SEASONID_DUMMY=$(echo $NEWPAGE | sed -nr 's/.*myspass.*\/(.*+)\/.*\/.*\/.*/\1/p')
        EXTRAID=$(expr $EXTRAID_DUMMY / $ROUND)
        SEASONID=$(cat $TMP/cache.$FILENAME.$ROUND.1 | sed -nr 's/.*season_id.*\[CDATA\[(.*+)].*].*/\1/p')


        NEWPAGE=$(echo $NEWPAGE | sed "s!/$ID_DUMMY\_!/$ROUND\_!")
        NEWPAGE=$(echo $NEWPAGE | sed "s!/$ID_DUMMY/!/$ROUND/!")
        NEWPAGE=$(echo $NEWPAGE | sed "s!/$EXTRAID_DUMMY/!/$EXTRAID/!")
        NEWPAGE=$(echo $NEWPAGE | sed "s!/$SEASONID_DUMMY/!/$SEASONID/!")

		PIC="http:`cat $TMP/cache.$FILENAME.$ROUND.1 | sed 's/<imagePreview><!\[CDATA\[/\nfound=\]/g' | grep ^found= | cut -d"]" -f2`"

		if [ -z "$PIC" ] || "$PIC" = "http:" ] ; then
			PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
		fi

		echo "$NEWPAGE" > $TMP/$FILENAME.list
    fi
	echo "$TMP/$FILENAME.list"
}


seasonapi()
{
	rm "$TMP/$FILENAME.list"

	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - $URL/$PAGE | sed 's/{"format_id":/\n{"format_id":/g' | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT \
		'
			# BEGIN variable setzen
			BEGIN \
			{
				# setzt suchvariable auf 0 vor dem start
				piccount = 0
				pages = "1"
			}
			/"format_id"/ \
			{
				i = index($0, "\"format_id\":\"") + 13
	            		j = index(substr($0, i), "\",\"") - 1
				id = substr($0, i, j)

				i = index($0, "\"format\":\"") + 10
	            		j = index(substr($0, i), "\",\"") - 1
				title = substr($0, i, j)

				i = index($0, "\"number_of_seasons\":\"") + 21
	            		j = index(substr($0, i), "\",\"") - 1
				extra = substr($0, i, j)

				i = index($0, "\"node_id\":\"") + 11
	            		j = index(substr($0, i), "\"") - 1
				newpage = substr($0, i, j)
				gsub(/\\/, "", newpage)

				i = index($0, "\"original_image\":\"") + 18
	            		j = index(substr($0, i), "\"") - 1
				pic = substr($0, i, j)
				gsub(/\\/, "", pic)

				piccount += 1
				if ( pic == "" )
				{
	            			pic = "http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
				}
				else
					pic = "http:" pic

				print title " (" extra " Staffeln)#" SRC " " SRC " year \x27api/index.php?command=seasonslist&id=" id "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#0"

				next
			}
			END \
			{
				if (curpage != pages)
					print "Page (" NEXT + 1 "/" pages ")#" SRC " " SRC " " INPUT " \x27" PAGE "\x27 " NEXT + 1 "#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#" NAME "#0"
			}
		# schreibe alles in die list datei
		' >$TMP/$FILENAME.list
	fi
	# gebe titan den list namen mit pfad zurueck
	echo "$TMP/$FILENAME.list"
}

yearapi()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - $URL/$PAGE | sed 's/{"season_id":/\n{"season_id":/g' | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT \
		'
			# BEGIN variable setzen
			BEGIN \
			{
				# setzt suchvariable auf 0 vor dem start
				piccount = 0
				pages = "1"
			}
			/"season_id"/ \
			{
				i = index($0, "\"season_id\":\"") + 13
	            		j = index(substr($0, i), "\",\"") - 1
				id = substr($0, i, j)

				i = index($0, "\"season_number\":\"") + 17
	            		j = index(substr($0, i), "\",\"") - 1
				title = substr($0, i, j)

				i = index($0, "\"season_name\":\"") + 15
	            		j = index(substr($0, i), "\",\"") - 1
				extra = substr($0, i, j)

				i = index($0, "\"node_id\":\"") + 11
	            		j = index(substr($0, i), "\"") - 1
				newpage = substr($0, i, j)
				gsub(/\\/, "", newpage)

				i = index($0, "\"original_image\":\"") + 18
	            		j = index(substr($0, i), "\"") - 1
				pic = substr($0, i, j)
				gsub(/\\/, "", pic)

				piccount += 1
				if ( pic == "" )
				{
	            			pic = "http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
				}
				else
					pic = "http:" pic

				print "Staffel " title " (" extra ")#" SRC " " SRC " episodes \x27api/index.php?command=seasonepisodes&id=" id "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#0"

				next
			}
			END \
			{
				if (curpage != pages)
					print "Page (" NEXT + 1 "/" pages ")#" SRC " " SRC " " INPUT " \x27" PAGE "\x27 " NEXT + 1 "#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#" NAME "#0"
			}
		# schreibe alles in die list datei
		' >$TMP/$FILENAME.list
	fi
	# gebe titan den list namen mit pfad zurueck
	echo "$TMP/$FILENAME.list"
}

episodesapi()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - $URL/$PAGE | sed 's/{"format":/\n{"format":/g' | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT \
		'
			# BEGIN variable setzen
			BEGIN \
			{
				# setzt suchvariable auf 0 vor dem start
				piccount = 0
				pages = "1"
			}
			/"format"/ \
			{
				i = index($0, "\"episode_id\":\"") + 14
	            		j = index(substr($0, i), "\",\"") - 1
				id = substr($0, i, j)

				i = index($0, "\"title\":\"") + 9
	            		j = index(substr($0, i), "\",\"") - 1
				title = substr($0, i, j)
				gsub(" - Teil 1", "", title)

				i = index($0, "\"season_name\":\"") + 15
	            		j = index(substr($0, i), "\",\"") - 1
				extra = substr($0, i, j)

				i = index($0, "\"season_number\":\"") + 17
	            		j = index(substr($0, i), "\",\"") - 1
				season = substr($0, i, j)

				i = index($0, "\"episode_nr\":\"") + 14
	            		j = index(substr($0, i), "\",\"") - 1
				episode = substr($0, i, j)

				i = index($0, "\"myspass_url\":\"") + 15
	            		j = index(substr($0, i), "\"") - 1
				newpage = substr($0, i, j)
				gsub(/\\/, "", newpage)

				i = index($0, "\"original_image\":\"") + 18
	            		j = index(substr($0, i), "\"") - 1
				pic = substr($0, i, j)
				gsub(/\\/, "", pic)

				piccount += 1
				if ( pic == "" )
				{
	            			pic = "http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
				}
				print "S" season "E" episode " - " title "#" SRC " " SRC " parts \x27" newpage "\x27#http:" pic "#" PICNAME "." piccount ".jpg#" NAME "#0"

				next
			}
			END \
			{
				if (curpage != pages)
					print "Page (" NEXT + 1 "/" pages ")#" SRC " " SRC " " INPUT " \x27" PAGE "\x27 " NEXT + 1 "#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#" NAME "#0"
			}
		# schreibe alles in die list datei
		' >$TMP/$FILENAME.list
	fi
	# gebe titan den list namen mit pfad zurueck
	echo "$TMP/$FILENAME.list"
}

searchapi()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - $URL/$PAGE | sed 's/{"uniqueID":/\n{"uniqueID":/g' | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME -v INPUT=$INPUT -v PAGE=$PAGE -v NEXT=$NEXT \
		'
			# BEGIN variable setzen
			BEGIN \
			{
				# setzt suchvariable auf 0 vor dem start
				piccount = 0
				pages = "1"
			}
			/"uniqueID"/ \
			{
#				print "1111111" $0
				i = index($0, "\"title\":\"") + 9
	            		j = index(substr($0, i), "\",\"") - 1
				title = substr($0, i, j)
#				print "title: " title

				i = index($0, "\"broadcast\":\"") + 13
	            		j = index(substr($0, i), "\",\"") - 1
				extra = substr($0, i, j)
#				print "extra: " extra

				i = index($0, "\"url\":\"") + 7
	            		j = index(substr($0, i), "\"") - 1
				newpage = substr($0, i, j)
				gsub(/\\/, "", newpage)

#				print "newpage: " newpage

				i = index($0, "\"teaser\":\"") + 10
	            		j = index(substr($0, i), "\"") - 1
				pic = substr($0, i, j)
				gsub(/\\/, "", pic)
#				print "pic: " pic

				piccount += 1
				if ( pic == "" )
				{
	            			pic = "http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
				}
				print title " (" extra ")#" SRC " " SRC " hoster \x27" newpage "\x27#" pic "#" PICNAME "." piccount ".jpg#" NAME "#111"
				next
			}
			END \
			{
				if (curpage != pages)
					print "Page (" NEXT + 1 "/" pages ")#" SRC " " SRC " " INPUT " \x27" PAGE "\x27 " NEXT + 1 "#http://openaaf.dyndns.tv/mediathek/menu/next.jpg#next.jpg#" NAME "#0"
			}
		# schreibe alles in die list datei
		' >$TMP/$FILENAME.list
	fi
	# gebe titan den list namen mit pfad zurueck
	echo "$TMP/$FILENAME.list"
}

hosterapi()
{
	echo $URL$PAGE
}

playapi()
{
	echo $URL$PAGE
}

partsapi()
{
	rm $TMP/$FILENAME.list
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		FOUND=`echo $PAGE | sed 's/Teil/\n/g' | grep ^http | sed 's!http://www.myspass.de/myspass!!g' | sed 's!http://www.myspass.de!!g'`
#		IDLIST=`$curlbin "$PAGE" | grep "$FOUND" | sed 's/<a href=/\nfound=/g' | grep ^found= | grep title= | grep -v myspassTeaserTextDesc | cut -d '"' -f2 | sort -u | tr '/' ' ' | awk '{ print $NF }'`
		IDLIST=`$curlbin "$PAGE" | grep "$FOUND"| sed 's/<a href=/\nfound=/g' | grep ^found= | tr -d ' ' | cut -d "/" -f6 | sort -u`

		for ROUND in $IDLIST; do

			$curlbin http://www.myspass.de/includes/apps/video/getvideometadataxml.php?id=$ROUND > "$TMP/cache.$FILENAME.$ROUND.1"
			
			#	<title><![CDATA[23. September 2006 - Teil 1]]></title>
			#	<url_flv><![CDATA[http://x3583brainc11021.s.o.l.lb.core-cdn.net/secdl/9b7b49482bf7ba65145bc28c6c3ac7b5/58ea7331/11021brainpool/ondemand/3583brainpool/163840/myspass2009/82/578/9231/12073/12073_61.mp4]]></url_flv>
			#	<imagePreview><![CDATA[//www.myspass.de/myspass/media/images/videos/73/12073_640x360.jpg]]></imagePreview>

			TITLE="`cat $TMP/cache.$FILENAME.$ROUND.1 | sed 's/<title><!\[CDATA\[/\nfound=\]/g' | grep ^found= | cut -d"]" -f2`"
			NEWPAGE="`cat $TMP/cache.$FILENAME.$ROUND.1 | sed 's/<url_flv><!\[CDATA\[/\nfound=\]/g' | grep ^found= | cut -d"]" -f2`"

#<id><![CDATA[29870]]></id>
#<format_id><![CDATA[104]]></format_id>
#<season_id><![CDATA[836]]></season_id>

#https://cldf-od.r53.cdn.tv1.eu/secdl/aa257d4ea8f9de7da13100c6e915a3c0/5e0d413e/11021brainpool/ondemand/3583brainpool/163840/myspass2009/104/24971320/355453000/892216900/892216900_61.mp4

#https://cldf-od.r53.cdn.tv1.eu/secdl/aa257d4ea8f9de7da13100c6e915a3c0/5e0d413e/11021brainpool/ondemand/3583brainpool/163840/myspass2009/104/836/11905/29870/29870_61.mp4

#https://cldf-od.r53.cdn.tv1.eu/secdl/5e6d2242be7cca293dc96d629127fa69/5e0d428f/11021brainpool/ondemand/3583brainpool/163840/myspass2009/104/836/11900/29870/29870_61.mp4

#sed -nr 's/.*myspass.*\/.*\/([.*]+)\/.*/\1/p'
#sed -nr 's/.*src="([^"]+)".*/\1/p'
#echo https://cldf-od.r53.cdn.tv1.eu/secdl/aa257d4ea8f9de7da13100c6e915a3c0/5e0d413e/11021brainpool/ondemand/3583brainpool/163840/myspass2009/104/24971320/355453000/892216900/892216900_61.mp4 | sed -nr 's/.*myspass.*\/.*\/(.*+)\/.*/\1/p'
#echo https://cldf-od.r53.cdn.tv1.eu/secdl/aa257d4ea8f9de7da13100c6e915a3c0/5e0d413e/11021brainpool/ondemand/3583brainpool/163840/myspass2009/104/24971320/355453000/892216900/892216900_61.mp4 | sed -nr 's/.*myspass.*\/.*\/(.*+)\/.*/\1/p'
#echo https://cldf-od.r53.cdn.tv1.eu/secdl/aa257d4ea8f9de7da13100c6e915a3c0/5e0d413e/11021brainpool/ondemand/3583brainpool/163840/myspass2009/104/24971320/355453000/892216900/892216900_61.mp4 | sed -nr 's/.*myspass.*\/(.*+)\/.*/\1/p'

ID_DUMMY=$(echo $NEWPAGE | sed -nr 's/.*myspass.*\/.*\/(.*+)\/.*/\1/p')
EXTRAID_DUMMY=$(echo $NEWPAGE | sed -nr 's/.*myspass.*\/(.*+)\/.*\/.*/\1/p')
SEASONID_DUMMY=$(echo $NEWPAGE | sed -nr 's/.*myspass.*\/(.*+)\/.*\/.*\/.*/\1/p')
EXTRAID=$(expr $EXTRAID_DUMMY / $ROUND)
SEASONID=$(cat $TMP/cache.$FILENAME.$ROUND.1 | sed -nr 's/.*season_id.*\[CDATA\[(.*+)].*].*/\1/p')

#echo ID_DUMMY $ID_DUMMY
#echo EXTRAID_DUMMY $EXTRAID_DUMMY
#echo SEASONID_DUMMY $SEASONID_DUMMY
#echo EXTRAID $EXTRAID
#echo SEASONID $SEASONID
#echo NEWPAGE $NEWPAGE

NEWPAGE=$(echo $NEWPAGE | sed "s!/$ID_DUMMY\_!/$ROUND\_!")
#echo NEWPAGE1 $NEWPAGE
NEWPAGE=$(echo $NEWPAGE | sed "s!/$ID_DUMMY/!/$ROUND/!")
#echo NEWPAGE2 $NEWPAGE
NEWPAGE=$(echo $NEWPAGE | sed "s!/$EXTRAID_DUMMY/!/$EXTRAID/!")
#echo NEWPAGE3 $NEWPAGE
NEWPAGE=$(echo $NEWPAGE | sed "s!/$SEASONID_DUMMY/!/$SEASONID/!")
#echo NEWPAGE4 $NEWPAGE

			PIC="http:`cat $TMP/cache.$FILENAME.$ROUND.1 | sed 's/<imagePreview><!\[CDATA\[/\nfound=\]/g' | grep ^found= | cut -d"]" -f2`"

			if [ -z "$PIC" ] || "$PIC" = "http:" ] ; then
				PIC="http://openaaf.dyndns.tv/mediathek/menu/default.jpg"
			fi

			echo "$TITLE#$NEWPAGE#$PIC#$PICNAME$piccount.jpg#$NAME#2" >> $TMP/$FILENAME.list

		done
	fi

	echo "$TMP/$FILENAME.list"
}

case $INPUT in
	init) $INPUT;;
	mainmenu) $INPUT;;
	search) $INPUT;;
	season) $INPUT;;
	year) $INPUT;;
	episodes) $INPUT;;
	play) $INPUT;;
	parts) $INPUT;;
	alpha) $INPUT;;
	play) $INPUT;;
esac
