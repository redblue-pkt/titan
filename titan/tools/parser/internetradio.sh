#!/bin/bash
#

buildtype=$1
wgetbin="wget -T2 -t2 --waitretry=2"

rm cache.*
rm _liste
rm -rf _full/internetradio
mkdir -p _full/internetradio/streams
rm cache.*
touch cache.internetradio.titanlist

BEGINTIME=`date +%s`
DATENAME=`date +"%Y.%m.%d_%H.%M.%S"`
echo "[internetradio.sh] START (buildtype: $buildtype): $DATENAME" > _full/internetradio/build.log

if [ "$buildtype" == "full" ];then
	testcount=0
	piccount=0
	count=0
	icount=0
	
	count=`expr $count + 1`	
	$wgetbin "http://dir.xiph.org/yp.xml" -O cache.mainfile
	
	id_list="`cat cache.mainfile | tr '\n' ' ' | sed 's/ \+/ /g' | sed 's/<entry>/\n\n<entry>/g' | grep ^"<entry>" | tr ' ' '~'`"
	id_list_count="`cat cache.mainfile | tr '\n' ' ' | sed 's/ \+/ /g' | sed 's/<entry>/\n\n<entry>/g' | grep ^"<entry>" | tr ' ' '~' | wc -l`"
	
	count1=0
	for ROUND1 in $id_list; do
		piccount=`expr $piccount + 1`
		count1=`expr $count1 + 1`
		echo "add($count1/$id_list_count)"
	
#	if [ $count1 == 10 ]; then
#		break
#	fi
		URL=`echo $ROUND1 | sed 's/<listen_url>/\nurl=</g' | grep ^"url=" | tr '~' ' ' | cut -d '<' -f2`
		GENRE=`echo $ROUND1 | sed 's/<genre>/\ngenre=</g' | grep ^"genre=" | tr '~' ' ' | cut -d '<' -f2`
		GENRE=`echo $GENRE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' # ' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
	#	GENRE=`echo $(php -r "echo rawurldecode('$GENRE');")`
	
		TITLE=`echo $ROUND1 | sed 's/<server_name>/\ntitle=</g' | grep ^"title=" | tr '~' ' ' | cut -d '<' -f2`
		TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' # ' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g' -e 's/\.\+/./g'`

	#	TITLE=`echo $(php -r "echo rawurldecode('$TITLE');")`
				
		PIC=http://atemio.dyndns.tv/mediathek/menu/`echo $TITLE | tr 'A-Z' 'a-z' | tr ' ' '_'`.jpg
		
		if [ -z "$PIC" ]; then
			PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
		fi
	
		if [ -z "$GENRE" ]; then
			GENRE="unknown"
		fi

		filename=`echo $GENRE | tr 'A-Z' 'a-z'`
		for ROUND in 50 60 70 80 90; do
			filename=`echo $filename | sed -e "s/$ROUND/19$ROUND /" -e "s/19$ROUND/19$ROUND /" -e "s/best$ROUND/19$ROUND /" -e "s/all$ROUND/19$ROUND /" -e "s/hits$ROUND/19$ROUND /"`
		done

		for ROUND in 2000 2001 2002 2003 2004 2005 2006 2007 2008 2009 2010 2011 2012 2013 2014; do
			filename=`echo $filename | sed -e "s/$ROUND/$ROUND /" -e "s/best$ROUND/$ROUND /"`
		done
#		filename=`echo $filename | sed -e 's/00s/2000 /' -e 's/100/100 /' -e 's/^classi.*/classical/' -e 's/^adu.*/adult/' -e 's/^adven.*/adventist/' -e 's/^afr.*/africa/' -e 's/^alt.*/alternative/' -e 's/^ama.*/amateur/' -e 's/^ambi.*/ambiance/' -e 's/^amo.*/amor/' -e 's/^ameri.*/america/' -e 's/^anim.*/animation/' -e 's/^ann.*/annees/' -e 's/^arab.*/arabic/' -e 's/^bach.*/bach/' -e 's/^bala.*/balladen/' -e 's/^ball.*/balladen/' -e 's/^band.*/band/' -e 's/^bas.*/bass/' -e 's/^bienven.*/bienvenue/' -e 's/^colo.*/colombia/' -e 's/^comp.*/computer/' -e 's/^dance.*/dance/' -e 's/^latin.*/latin/' -e 's/^liv.*/live/' -e 's/^love.*/love/' -e 's/^musi.*/musik/' -e 's/^no.*/nostalgia/' -e 's/^old.*/oldies/' -e 's/^pop.*/pop/' -e 's/^pro.*/progressive/' -e 's/^radio.*/radio/' -e 's/.*reg.*/reggae/' -e 's/^rel.*/relaxing/' -e 's/.*rock.*/rock/' -e 's/^soul.*/soul/' -e 's/.*sound.*/soundtracks/' -e 's/^sym.*/sym/' -e 's/.*talk.*/talk/' -e 's/^vari.*/various/' -e 's/^easy.*/easylistning/' -e 's/^elect.*/electronic/' -e 's/^folk.*/folk/' -e 's/^neofolk.*/folk/' -e 's/^inspi.*/inspirational/' -e 's/^intern.*/international/' -e 's/^jazz.*/jazz/' -e 's/^smoothjazz.*/jazz/' -e 's/^metal.*/metal/' -e 's/^deathmetal.*/metal/' -e 's/^powermetal.*/metal/' -e 's/^misc.*/misc/' -e 's/^new.*/newage/' -e 's/.*pop.*/pop/' -e 's/.*public.*/publicradio/' -e 's/.*urban.*/urban/' -e 's/.*rnb.*/rnb/' -e 's/.*rap.*/rap/' -e 's/.*holi.*/holiday/' -e 's/.*them.*/themes/' -e 's/.*deca.*/decades/'`

		filename=`echo $filename | sed -e 's/top 40/top40 /' -e 's/top 40/top40 /' -e 's/#/ /g'`
		filename=`echo $filename | tr -d '"' | tr '-' ' ' | tr '_' ' ' | tr '.' ' ' | sed 's/\ \+/ /g'`

		if [ ! -z "$TITLE" ] && [ ! -z "$URL" ] && [ -e cache.internetradio.titanlist ] && [ `cat cache.internetradio.titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
			LINE="$TITLE#$URL#$PIC#internetradio_$piccount.jpg#Internet Radio#2"
			echo $LINE >> cache.internetradio.titanlist
		elif [ ! -z "$TITLE" ] && [ ! -z "$URL" ]; then
			LINE="$TITLE#$URL#$PIC#internetradio_$piccount.jpg#Internet Radio#2"
			echo $LINE >> cache.internetradio.titanlist
		fi
	
		for ROUND2 in $filename; do
#			echo "$filename ($ROUND2)"
			back=$ROUND2
			ROUND2=`echo $ROUND2 | sed -e 's/.*00s.*/2000/' -e 's/.*100.*/100/' -e 's/.*alt.*/alternative/' -e 's/.*blue.*/blues/' -e 's/.*bala.*/balladen/' -e 's/.*ball.*/balladen/' -e 's/.*bas.*/bass/' -e 's/.*classi.*/classical/' -e 's/.*colo.*/colombia/' -e 's/.*count.*/country/' -e 's/.*dance.*/dance/' -e 's/.*latin.*/latin/' -e 's/.*liv.*/live/' -e 's/.*love.*/love/' -e 's/.*oldi.*/oldies/' -e 's/.*pop.*/pop/' -e 's/.*reg.*/reggae/' -e 's/.*rela.*/relaxing/' -e 's/.*rock.*/rock/' -e 's/.*soul.*/soul/' -e 's/.*sound.*/soundtracks/' -e 's/.*talk.*/talk/' -e 's/.*easy.*/easylistning/' -e 's/.*elect.*/electronic/' -e 's/.*folk.*/folk/' -e 's/.*inspi.*/inspirational/' -e 's/.*intern.*/international/' -e 's/.*jazz.*/jazz/' -e 's/.*metal.*/metal/' -e 's/.*misc.*/misc/' -e 's/.*new.*/newage/' -e 's/.*pop.*/pop/' -e 's/.*public.*/publicradio/' -e 's/.*urban.*/urban/' -e 's/.*rnb.*/rnb/' -e 's/.*rap.*/rap/' -e 's/.*holi.*/holiday/' -e 's/.*them.*/themes/' -e 's/.*deca.*/decades/'`
			if [ "$back" != "$ROUND2" ];then
				echo "genre: $back > $ROUND2"
			fi

			if [ "$ROUND2" = "alternative" ] || [ "$ROUND2" = "blues" ] || [ "$ROUND2" = "balladen" ] || [ "$ROUND2" = "bass" ] || [ "$ROUND2" = "classical" ] || [ "$ROUND2" = "colombia" ] || [ "$ROUND2" = "country" ] || [ "$ROUND2" = "dance" ] || [ "$ROUND2" = "easylistning" ] || [ "$ROUND2" = "electronic" ] || [ "$ROUND2" = "folk" ] || [ "$ROUND2" = "inspirational" ] || [ "$ROUND2" = "international" ] || [ "$ROUND2" = "jazz" ] || [ "$ROUND2" = "metal" ] || [ "$ROUND2" = "misc" ] || [ "$ROUND2" = "newage" ] || [ "$ROUND2" = "pop" ] || [ "$ROUND2" = "latin" ] || [ "$ROUND2" = "live" ] || [ "$ROUND2" = "love" ] || [ "$ROUND2" = "misc" ] || [ "$ROUND2" = "oldies" ] || [ "$ROUND2" = "publicradio" ] || [ "$ROUND2" = "urban" ] || [ "$ROUND2" = "rnb" ] || [ "$ROUND2" = "rap" ] || [ "$ROUND2" = "reggae" ] || [ "$ROUND2" = "rock" ] || [ "$ROUND2" = "relax" ] || [ "$ROUND2" = "holiday" ] || [ "$ROUND2" = "soundtracks" ] || [ "$ROUND2" = "talk" ] || [ "$ROUND2" = "themes" ] || [ "$ROUND2" = "decades" ] || [ "$ROUND2" = "metal" ] || [ "$ROUND2" = "jazz" ]; then
				GENRE="$ROUND2"
			else
				GENRE="unknown"
				i=1950
				until [ "$i" -gt 2015 ]
				do
				#echo $i
					if [ "$ROUND2" = "$i" ]; then
						GENRE=$ROUND2
						echo "genre (year): $back > $ROUND2"
						break
					fi
				i=$[$i+1]
				done
			fi

			check=`echo "$TITLE" | tr 'A-Z' 'a-z' | cut -d" " -f1`
			if [ "$check" == "$GENRE" ]; then
				ROUND2="unknown-samename"
			fi

			if [ ! -z "$TITLE" ] && [ ! -z "$URL" ] && [ -e cache.internetradio.genre."$GENRE".titanlist ] &&  [ `cat cache.internetradio.genre."$GENRE".titanlist | grep "#$URL#" | wc -l` -eq 0 ];then
				LINE="$TITLE#$URL#$PIC#internetradio_$piccount.jpg#Internet Radio#2"
				echo $LINE >> cache.internetradio.genre."$GENRE".titanlist
	
				piccount=`expr $piccount + 1`
				URL1="http://atemio.dyndns.tv/mediathek/internetradio/streams/internetradio.genre.$GENRE.list"
				PIC1="http://atemio.dyndns.tv/mediathek/menu/$GENRE.jpg"
				LINE1="$GENRE#$URL1#$PIC1#internetradio_$piccount.jpg#Internet Radio#0"
				if [ ! -z "$URL1" ] && [ `cat cache.internetradio.category.titanlist | grep "#$URL1#" | wc -l` -eq 0 ];then
					echo $LINE1 >> cache.internetradio.category.titanlist
				fi
				cat cache.internetradio.genre."$GENRE".titanlist | sort -u > _full/internetradio/streams/internetradio.genre."$GENRE".list
			elif [ ! -z "$TITLE" ] && [ ! -z "$URL" ] && [ ! -e cache.internetradio.genre."$GENRE".titanlist ];then
				LINE="$TITLE#$URL#$PIC#internetradio_$piccount.jpg#Internet Radio#2"
				echo $LINE >> cache.internetradio.genre."$GENRE".titanlist
	
				piccount=`expr $piccount + 1`
				URL2="http://atemio.dyndns.tv/mediathek/internetradio/streams/internetradio.genre.$GENRE.list"
				PIC2="http://atemio.dyndns.tv/mediathek/menu/$GENRE.jpg"
				LINE2="$GENRE#$URL2#$PIC2#internetradio_$piccount.jpg#Internet Radio#0"
				if [ ! -z "$URL2" ] && [ `cat cache.internetradio.category.titanlist | grep "#$URL2#" | wc -l` -eq 0 ];then
					echo $LINE2 >> cache.internetradio.category.titanlist
				fi
				cat cache.internetradio.genre."$GENRE".titanlist | sort -u > _full/internetradio/streams/internetradio.genre."$GENRE".list		
			fi
		done
				
	#		echo ROUND1 $ROUND1
	#		echo URL $URL
	#		echo PIC $PIC
	#		echo TITLE $TITLE
	#		echo GENRE $GENRE
	
	done
	
	if [ "$buildtype" = "full" ];then
		cat cache.internetradio.category.titanlist | sort -u > _full/internetradio/internetradio.category.list	
		cat cache.internetradio.titanlist | sort -u > _full/internetradio/streams/internetradio.all-sorted.list
		
		for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
			filename=`echo "$ROUND" | tr 'A-Z' 'a-z'`
			if [ `cat cache.internetradio.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
				cat cache.internetradio.titanlist | grep ^"$ROUND" >> cache.internetradio.titanlist."$ROUND"
				cat cache.internetradio.titanlist."$ROUND" | sort -um > _full/internetradio/streams/internetradio.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
				echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/internetradio/streams/internetradio."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#Internet Radio#3 >> _full/internetradio/internetradio.a-z.list
			elif [ `cat cache.internetradio.titanlist | grep ^"$filename" | wc -l` -gt 0 ];then
				cat cache.internetradio.titanlist | grep ^"$filename" >> cache.internetradio.titanlist."$ROUND"
				cat cache.internetradio.titanlist."$ROUND" | sort -um > _full/internetradio/streams/internetradio.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
				echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/internetradio/streams/internetradio."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#Internet Radio#3 >> _full/internetradio/internetradio.a-z.list
		
			fi
		done
	fi
fi

DONETIME=`date +%s`
TIME=`expr $DONETIME - $BEGINTIME`
echo "[internetradio.sh] build time: ($TIME s) done" >> _full/internetradio/build.log	
echo "[internetradio.sh] internetradio: "`ls -1 _full/internetradio` >> _full/internetradio/build.log
echo "[internetradio.sh] internetradio/streams: "`ls -1 _full/internetradio/streams` >> _full/internetradio/build.log

if [ "$buildtype" != "full" ];then
	cp -a _full/internetradio/* /var/www/atemio/web/mediathek/internetradio
fi

rm cache.*

exit
