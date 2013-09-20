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
			#-e 's/.*00s.*/2000/' -e 's/.*100.*/100/' 
			
			ROUND2=`echo $ROUND2 | sed -e 's/.*alter.*/Alternative/' -e 's/.*adult.*/Alternative-(Adult)/' -e 's/.*brit.*/Alternative-(Britpop)/' -e 's/.*classic.*/Alternative-(Classic)/' -e 's/.*coll.*/Alternative-(College)/' -e 's/.*dancepunk.*/Alternative-(Dancepunk)/' -e 's/.*dream.*/Alternative-(Dream-Pop)/' -e 's/.*emo.*/Alternative-(Emo)/' -e 's/.*goth.*/Alternative-(Goth)/' -e 's/.*grunge.*/Alternative-(Grunge)/' -e 's/.*hardcore.*/Alternative-(Hardcore)/' -e 's/.*indie.*/Alternative-(Indie-Pop)/' -e 's/.*industrial.*/Alternative-(Industrial)/' -e 's/.*lofi.*/Alternative-(LoFi)/' -e 's/.*modern.*/Alternative-(Modern-Rock)/' -e 's/.*wave.*/Alternative-(New-Wave)/' -e 's/.*noise.*/Alternative-(Noise-Pop)/' -e 's/.*post.*/Alternative-(Post-Punk)/' -e 's/.*power.*/Alternative-(Power-Pop)/' -e 's/.*punk.*/Alternative-(Punk)/' -e 's/.*ska.*/Alternative-(Ska)/' -e 's/.*xtreme.*/Alternative-(Xtreme)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*blues.*/Blues/' -e 's/.*acoustic.*/Blues-(Acoustic)/' -e 's/.*cajun.*/Blues-(Cajun-and-Zydeco)/' -e 's/.*chicago.*/Blues-(Chicago)/' -e 's/.*contemporary.*/Blues-(Contemporary)/' -e 's/.*country.*/Blues-(Country)/' -e 's/.*delta.*/Blues-(Delta)/' -e 's/.*electric.*/Blues-(Electric)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*classical.*/Classical/' -e 's/.*baroque.*/Classical-(Baroque)/' -e 's/.*chamber.*/Classical-(Chamber)/' -e 's/.*choral.*/Classical-(Choral)/' -e 's/.*period.*/Classical-(Period)/' -e 's/.*early.*/Classical-(Early)/' -e 's/.*impressionist.*/Classical-(Impressionist)/' -e 's/.*modern.*/Classical-(Modern)/' -e 's/.*opera.*/Classical-(Opera)/' -e 's/.*piano.*/Classical-(Piano)/' -e 's/.*romantic.*/Classical-(Romantic)/' -e 's/.*symphony.*/Classical-(Symphony)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*country.*/Country/' -e 's/.*country.*/Country-(Alt)/' -e 's/.*americana.*/Country-(Americana)/' -e 's/.*bluegrass.*/Country-(Bluegrass)/' -e 's/.*classic.*/Country-(Classic)/' -e 's/.*contemporary.*/Country-(Contemporary)/' -e 's/.*honky.*/Country-(Honky-Tonk)/' -e 's/.*hot.*/Country-(Hot-Country-Hits)/' -e 's/.*western.*/Country-(Western)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*listening.*/Easy-Listening/' -e 's/.*exotica.*/Easy-Listening-(Exotica)/' -e 's/.*light.*/Easy-Listening-(Light-Rock)/' -e 's/.*lounge.*/Easy-Listening-(Lounge)/' -e 's/.*orchestral.*/Easy-Listening-(Orchestral-Pop)/' -e 's/.*polka.*/Easy-Listening-(Polka)/' -e 's/.*space.*/Easy-Listening-(Space-Age-Pop)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*electronic.*/Electronic/' -e 's/.*acid.*/Electronic-(Acid-House)/' -e 's/.*ambient.*/Electronic-(Ambient)/'  -e 's/.*breakbeat.*/Electronic-(Big-Beat)/' -e 's/.*dance.*/Electronic-(Dance)/' -e 's/.*demo.*/Electronic-(Demo)/' -e 's/.*disco.*/Electronic-(Disco)/' -e 's/.*downtempo.*/Electronic-(Downtempo)/' -e 's/.*drum.*/Electronic-(Drum-and-Bass)/' -e 's/.*electro.*/Electronic-(Electro)/' -e 's/.*garage.*/Electronic-(Garage)/' -e 's/.*hard.*/Electronic-(Hard-House)/' -e 's/.*house.*/Electronic-(House)/' -e 's/.*idm.*/Electronic-(IDM)/' -e 's/.*jungle.*/Electronic-(Jungle)/' -e 's/.*jungle.*/Electronic-(Jungle)/' -e 's/.*progressive.*/Electronic-(Progressive)/' -e 's/.*techno.*/Electronic-(Techno)/' -e 's/.*trance.*/Electronic-(Trance)/' -e 's/.*tribal.*/Electronic-(Tribal)/' -e 's/.*trip.*/Electronic-(Trip-Hop)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*folk.*/Folk/' -e 's/.*alterna.*/Folk-(Alternative)/' -e 's/.*contemp.*/Folk-(Contemporary)/' -e 's/.*rock.*/Folk-(Rock)/' -e 's/.*acoustic.*/Folk-(New-Acoustic)/' -e 's/.*time.*/Folk-(Old-Time)/' -e 's/.*traditional.*/Folk-(Traditional)/' -e 's/.*world.*/Folk-(World)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*inspirational.*/Inspirational/' -e 's/.*christian.*/Inspirational-(Christian)/' -e 's/.*contemp.*/Inspirational-(Contemporary)/' -e 's/.*gospel.*/Inspirational-(Gospel)/' -e 's/.*praise.*/Inspirational-(Praise-and-Worship)/' -e 's/.*sermons.*/Inspirational-(Sermons-and-Services)/' -e 's/.*southern.*/Inspirational-(Southern-Gospel)/' -e 's/.*traditional.*/Inspirational-(Traditional-Gospel)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*international.*/International/' -e 's/.*european.*/International-(European)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*jazz.*/Jazz/' -e 's/.*avant.*/Jazz-(Avant)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*latin.*/Latin/' -e 's/.*bachata.*/Latin-(Bachata)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*metal.*/Metal/' -e 's/.*black.*/Metal-(Black)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*misc.*/Misc/' -e 's/.*age.*/Misc-(New-Age)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*Pop.*/Pop/' -e 's/.*oldies.*/Pop-(Oldies)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*public.*/Public-Radio/' -e 's/.*sports.*/Public-Radio-(Sports)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*Rap.*/Rap/' -e 's/.*Hip.*/Rap-(Hip-Hop)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*reggae.*/Reggae/' -e 's/.*dancehall.*/Reggae-(Ragga)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*rock.*/Rock/' -e 's/.*glam.*/Rock-(Glam)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*soundtracks.*/Soundtracks/' -e 's/.*original.*/Soundtracks-(Original)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*talk.*/Talk/' -e 's/.*political.*/Talk-(Political)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*themes.*/Themes/' -e 's/.*best.*/Themes-(Best)/' -e 's/.*party.*/Themes-(Party)/' -e 's/.*chill.*/Themes-(Chill)/'`

			if [ "$back" != "$ROUND2" ];then
				echo "genre: $back > $ROUND2"
			fi

			if [ "$ROUND2" = "Alternative" ] || [ "$ROUND2" = "Alternative-(Adult)" ] || [ "$ROUND2" = "Alternative-(Britpop)" ] || [ "$ROUND2" = "Alternative-(Classic)" ] || [ "$ROUND2" = "Alternative-(College)" ] || [ "$ROUND2" = "Alternative-(Dancepunk)" ] || [ "$ROUND2" = "Alternative-(Dream-Pop)" ] || [ "$ROUND2" = "Alternative-(Emo)" ] || [ "$ROUND2" = "Alternative-(Goth)" ] || [ "$ROUND2" = "Alternative-(Grunge)" ] || [ "$ROUND2" = "Alternative-(Hardcore)" ] || [ "$ROUND2" = "Alternative-(Indie-Pop)" ] || [ "$ROUND2" = "Alternative-(Industrial)" ] || [ "$ROUND2" = "Alternative-(LoFi)" ] || [ "$ROUND2" = "Alternative-(Modern-Rock)" ] || [ "$ROUND2" = "Alternative-(New-Wave)" ] || [ "$ROUND2" = "Alternative-(Noise-Pop)" ] || [ "$ROUND2" = "Alternative-(Post-Punk)" ] || [ "$ROUND2" = "Alternative-(Power-Pop)" ] || [ "$ROUND2" = "Alternative-(Punk)" ] || [ "$ROUND2" = "Alternative-(Ska)" ] || [ "$ROUND2" = "Alternative-(Xtreme)" ];then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Blues" ] || [ "$ROUND2" = "Blues-(Acoustic)" ] || [ "$ROUND2" = "Blues-(Cajun-and-Zydeco)" ] || [ "$ROUND2" = "Blues-(Chicago)" ] || [ "$ROUND2" = "Blues-(Contemporary)" ] || [ "$ROUND2" = "Blues-(Country)" ] || [ "$ROUND2" = "Blues-(Delta)" ] || [ "$ROUND2" = "Blues-(Electric)" ];then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Classical" ] || [ "$ROUND2" = "Classical-(Baroque)" ] || [ "$ROUND2" = "Classical-(Chamber)" ] || [ "$ROUND2" = "Classical-(Choral)" ] || [ "$ROUND2" = "Classical-(Period)" ] || [ "$ROUND2" = "Classical-(Early)" ] || [ "$ROUND2" = "Classical-(Impressionist)" ] || [ "$ROUND2" = "Classical-(Modern)" ] || [ "$ROUND2" = "Classical-(Opera)" ] || [ "$ROUND2" = "Classical-(Piano)" ] || [ "$ROUND2" = "Classical-(Romantic)" ] || [ "$ROUND2" = "Classical-(Symphony)" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Country" ] || [ "$ROUND2" = "Country-(Alt)" ] || [ "$ROUND2" = "Country-(Americana)" ] || [ "$ROUND2" = "Country-(Bluegrass)" ] || [ "$ROUND2" = "Country-(Classic)" ] || [ "$ROUND2" = "Country-(Contemporary)" ] || [ "$ROUND2" = "Country-(Honky-Tonk)" ] || [ "$ROUND2" = "Country-(Hot-Country-Hits)" ] || [ "$ROUND2" = "Country-(Western)" ];then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Easy-Listening" ] || [ "$ROUND2" = "Easy-Listening-(Exotica)" ] || [ "$ROUND2" = "Easy-Listening-(Light-Rock)" ] || [ "$ROUND2" = "Easy-Listening-(Lounge)" ] || [ "$ROUND2" = "Easy-Listening-(Orchestral-Pop)" ] || [ "$ROUND2" = "Easy-Listening-(Polka)" ] || [ "$ROUND2" = "Easy-Listening-(Space-Age-Pop)" ];then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Electronic" ] || [ "$ROUND2" = "Electronic-(Acid-House)" ] || [ "$ROUND2" = "Electronic-(Ambient)" ] || [ "$ROUND2" = "Electronic-(Big-Beat)" ] || [ "$ROUND2" = "Electronic-(Dance)" ] || [ "$ROUND2" = "Electronic-(Demo)" ] || [ "$ROUND2" = "Electronic-(Disco)" ] || [ "$ROUND2" = "Electronic-(Downtempo)" ] || [ "$ROUND2" = "Electronic-(Drum-and-Bass)" ] || [ "$ROUND2" = "Electronic-(Electro)" ] || [ "$ROUND2" = "Electronic-(Garage)" ] || [ "$ROUND2" = "Electronic-(Hard-House)" ] || [ "$ROUND2" = "Electronic-(House)" ] || [ "$ROUND2" = "Electronic-(IDM)" ] || [ "$ROUND2" = "Electronic-(Jungle)" ] || [ "$ROUND2" = "Electronic-(Progressive)" ] || [ "$ROUND2" = "Electronic-(Techno)" ] || [ "$ROUND2" = "Electronic-(Trance)" ] || [ "$ROUND2" = "Electronic-(Tribal)" ] || [ "$ROUND2" = "Electronic-(Trip-Hop)" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Folk" ] || [ "$ROUND2" = "Folk-(Alternative)" ] || [ "$ROUND2" = "Folk-(Contemporary)" ] || [ "$ROUND2" = "Folk-(Rock)" ] || [ "$ROUND2" = "Folk-(New-Acoustic)" ] || [ "$ROUND2" = "Folk-(Old-Time)" ] || [ "$ROUND2" = "Folk-(Alternative)" ] || [ "$ROUND2" = "Folk-(Traditional)" ] || [ "$ROUND2" = "Folk-(World)" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Inspirational" ] || [ "$ROUND2" = "Inspirational-(Christian)" ] || [ "$ROUND2" = "Inspirational-(Contemporary)" ] || [ "$ROUND2" = "Inspirational-(Gospel)" ] || [ "$ROUND2" = "Inspirational-(Praise-and-Worship)" ] || [ "$ROUND2" = "Inspirational-(Sermons-and-Services)" ] || [ "$ROUND2" = "Inspirational-(Southern-Gospel)" ] || [ "$ROUND2" = "Inspirational-(Traditional-Gospel)" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "International" ] || [ "$ROUND2" = "International-(European)" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Jazz" ] || [ "$ROUND2" = "Jazz-(Avant)" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Latin" ] || [ "$ROUND2" = "Latin-(Bachata)" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Metal" ] || [ "$ROUND2" = "Metal-(Black)" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Misc" ] || [ "$ROUND2" = "Misc-(New-Age)" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Pop" ] || [ "$ROUND2" = "Pop-(Oldies)" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Public-Radio" ] || [ "$ROUND2" = "Public-Radio-(Sports)" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Rap" ] || [ "$ROUND2" = "Rap-(Hip-Hop)" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Reggae" ] || [ "$ROUND2" = "Reggae-(Ragga)" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Rock" ] || [ "$ROUND2" = "Rock-(Glam)" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Soundtracks" ] || [ "$ROUND2" = "Soundtracks-(Glam)" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Talk" ] || [ "$ROUND2" = "Talk-(Political)" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Themes" ] || [ "$ROUND2" = "Themes-(Best)" ] || [ "$ROUND2" = "Themes-(Party)" ] || [ "$ROUND2" = "Themes-(Chill)" ]; then
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
