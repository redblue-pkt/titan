#!/bin/bash
#

buildtype=$1
wgetbin="wget -T2 -t2 --waitretry=2"

rm cache.*
rm _liste*
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
			ROUND2=`echo $ROUND2 | sed -e 's/.*easy.*/Easy-Listening/' -e 's/.*listening.*/Easy-Listening/' -e 's/.*exotica.*/Easy-Listening-(Exotica)/' -e 's/.*light.*/Easy-Listening-(Light-Rock)/' -e 's/.*lounge.*/Easy-Listening-(Lounge)/' -e 's/.*orchestral.*/Easy-Listening-(Orchestral-Pop)/' -e 's/.*polka.*/Easy-Listening-(Polka)/' -e 's/.*space.*/Easy-Listening-(Space-Age-Pop)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*elet.*/Electronic/' -e 's/.*elek.*/Electronic/' -e 's/.*electronic.*/Electronic/' -e 's/.*acid.*/Electronic-(Acid-House)/' -e 's/.*ambient.*/Electronic-(Ambient)/'  -e 's/.*breakbeat.*/Electronic-(Big-Beat)/' -e 's/.*dance.*/Electronic-(Dance)/' -e 's/.*demo.*/Electronic-(Demo)/' -e 's/.*disco.*/Electronic-(Disco)/' -e 's/.*downtempo.*/Electronic-(Downtempo)/' -e 's/.*drum.*/Electronic-(Drum-and-Bass)/' -e 's/.*electro.*/Electronic-(Electro)/' -e 's/.*garage.*/Electronic-(Garage)/' -e 's/.*hard.*/Electronic-(Hard-House)/' -e 's/.*house.*/Electronic-(House)/' -e 's/.*idm.*/Electronic-(IDM)/' -e 's/.*jungle.*/Electronic-(Jungle)/' -e 's/.*jungle.*/Electronic-(Jungle)/' -e 's/.*progressive.*/Electronic-(Progressive)/' -e 's/.*techno.*/Electronic-(Techno)/' -e 's/.*trance.*/Electronic-(Trance)/' -e 's/.*tribal.*/Electronic-(Tribal)/' -e 's/.*trip.*/Electronic-(Trip-Hop)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*folk.*/Folk/' -e 's/.*alterna.*/Folk-(Alternative)/' -e 's/.*contemp.*/Folk-(Contemporary)/' -e 's/.*rock.*/Folk-(Rock)/' -e 's/.*acoustic.*/Folk-(New-Acoustic)/' -e 's/.*time.*/Folk-(Old-Time)/' -e 's/.*traditional.*/Folk-(Traditional)/' -e 's/.*world.*/Folk-(World)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*inspirational.*/Inspirational/' -e 's/.*christian.*/Inspirational-(Christian)/' -e 's/.*contemp.*/Inspirational-(Contemporary)/' -e 's/.*gospel.*/Inspirational-(Gospel)/' -e 's/.*praise.*/Inspirational-(Praise-and-Worship)/' -e 's/.*sermons.*/Inspirational-(Sermons-and-Services)/' -e 's/.*southern.*/Inspirational-(Southern-Gospel)/' -e 's/.*traditional.*/Inspirational-(Traditional-Gospel)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*international.*/International/' -e 's/.*portugal.*/International-(Portugal)/' -e 's/.*romanian.*/International-(Romanian)/' -e 's/.*nederland.*/International-(Nederlands)/' -e 's/.*english.*/International-(English)/' -e 's/.*ameri.*/International-(America)/' -e 's/.*ital.*/International-(Italia)/' -e 's/.*korea.*/International-(Korea)/' -e 's/.*spanish.*/International-(Spanish)/' -e 's/.*afric.*/International-(African)/' -e 's/.*afrikaans.*/International-(Afrikaans)/' -e 's/.*arabic.*/International-(Arabic)/' -e 's/.*asian.*/International-(Asian)/' -e 's/.*bollywood.*/International-(Bollywood)/' -e 's/.*brazi.*/International-(Brazilian)/' -e 's/.*carib.*/International-(Caribbean)/' -e 's/.*celtic.*/International-(Celtic)/' -e 's/.*chinese.*/International-(Chinese)/' -e 's/.*euro.*/International-(European)/' -e 's/.*filipino.*/International-(Filipino)/' -e 's/.*french.*/International-(French)/' -e 's/.*german.*/International-(German)/' -e 's/.*greek.*/International-(Greek)/' -e 's/.*hawaiian.*/International-(Hawaiian-and-Pacific)/' -e 's/.*hebrew.*/International-(Hebrew)/' -e 's/.*hindi.*/International-(Hindi)/' -e 's/.*indian.*/International-(Indian)/' -e 's/.*islamic.*/International-(Islamic)/' -e 's/.*japanese.*/International-(Japanese)/' -e 's/.*klezmer.*/International-(Klezmer)/' -e 's/.*korean.*/International-(Korean)/' -e 's/.*medite.*/International-(Mediterranean)/' -e 's/.*middle.*/International-(Middle-Eastern)/' -e 's/.*north.*/International-(North-American)/' -e 's/.*russian.*/International-(Russian)/' -e 's/.*soca.*/International-(Soca)/' -e 's/.*south.*/International-(South-American)/' -e 's/.*tamil.*/International-(Tamil)/' -e 's/.*turkish.*/International-(Turkish)/' -e 's/.*worldbeat.*/International-(Worldbeat)/' -e 's/.*zouk.*/International-(Zouk)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*jazz.*/Jazz/' -e 's/.*acid.*/Jazz-(Acid)/' -e 's/.*avant.*/Jazz-(Avant-Garde)/' -e 's/.*band.*/Jazz-(Big-Band)/' -e 's/.*bop.*/Jazz-(Bop)/' -e 's/.*classic.*/Jazz-(Classic)/' -e 's/.*cool.*/Jazz-(Cool)/' -e 's/.*fusion.*/Jazz-(Fusion)/' -e 's/.*hard.*/Jazz-(Hard-Bop)/' -e 's/.*latin.*/Jazz-(Latin)/' -e 's/.*smooth.*/Jazz-(Smooth)/' -e 's/.*swing.*/Jazz-(Swing)/' -e 's/.*vocal.*/Jazz-(Vocal)/' -e 's/.*world.*/Jazz-(World-Fusion)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*latin.*/Latin/' -e 's/.*bachata.*/Latin-(Bachata)/' -e 's/.*banda.*/Latin-(Banda)/' -e 's/.*bossa.*/Latin-(Bossa-Nova)/' -e 's/.*cumbia.*/Latin-(Cumbia)/' -e 's/.*maria.*/Latin-(Mariachi)/' -e 's/.*meren.*/Latin-(Merengue)/' -e 's/.*ranch.*/Latin-(Ranchera)/' -e 's/.*reggaet.*/Latin-(Reggaeton)/' -e 's/.*regional.*/Latin-(Regional-Mexican)/' -e 's/.*salsa.*/Latin-(Salsa)/' -e 's/.*tango.*/Latin-(Tango)/' -e 's/.*tejano.*/Latin-(Tejano)/' -e 's/.*tropic.*/Latin-(Tropicalia)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*metal.*/Metal/' -e 's/.*black.*/Metal-(Black)/' -e 's/.*extreme.*/Metal-(Extreme)/' -e 's/.*grindc.*/Metal-(Grindcore)/' -e 's/.*hair.*/Metal-(Hair)/' -e 's/.*heavy.*/Metal-(Heavy)/' -e 's/.*power.*/Metal-(Power)/' -e 's/.*progres.*/Metal-(Progressive)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*misc.*/Misc/' -e 's/.*age.*/Misc-(New-Age)/' -e 's/.*environm.*/Misc-(Environmental)/' -e 's/.*ethnic.*/Misc-(Ethnic-Fusion)/' -e 's/.*healing.*/Misc-(Healing)/' -e 's/.*meditation.*/Misc-(Meditation)/' -e 's/.*spiritual.*/Misc-(Spiritual)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*pop.*/Pop/' -e 's/.*contemporary.*/Pop-(Adult-Contemporary)/' -e 's/.*barbershop.*/Pop-(Barbershop)/' -e 's/.*bubblegum.*/Pop-(Bubblegum)/' -e 's/.*idols.*/Pop-(Idols)/' -e 's/.*soft.*/Pop-(Soft)/' -e 's/.*teen.*/Pop-(Teen)/' -e 's/.*top40.*/Pop-(Top-40)/' -e 's/.*world.*/Pop-(World)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*public.*/Public-Radio/' -e 's/.*news.*/Public-Radio-(News)/' -e 's/.*sport.*/Public-Radio-(Sports)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*rap.*/Rap/' -e 's/.*dirty.*/Rap-(Dirty-South)/' -e 's/.*east.*/Rap-(East-Coast)/' -e 's/.*freestyle.*/Rap-(Freestyle)/' -e 's/.*gangsta.*/Rap-(Gangsta)/' -e 's/.*hip.*/Rap-(Hip-Hop)/' -e 's/.*mixtapes.*/Rap-(Mixtapes)/' -e 's/.*school.*/Rap-(Old-School)/' -e 's/.*turnta.*/Rap-(Turntablism)/' -e 's/.*underground .*/Rap-(Underground-Hip-Hop)/' -e 's/.*west .*/Rap-(West-Coast Rap)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*reggae.*/Reggae/' -e 's/.*contempo.*/Reggae-(Contemporary)/' -e 's/.*dancehall.*/Reggae-(Dancehall)/' -e 's/.*dub.*/Reggae-(Dub)/' -e 's/.*dancehall.*/Reggae-(Ragga)/' -e 's/.*roots.*/Reggae-(Reggae-Roots)/' -e 's/.*steady.*/Reggae-(Rock-Steady)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*rock.*/Rock/' -e 's/.*album.*/Rock-(Adult-Album-Alternative)/' -e 's/.*invasion.*/Rock-(British-Invasion)/' -e 's/.*garage.*/Rock-(Garage)/' -e 's/.*glam.*/Rock-(Glam)/' -e 's/.*bands.*/Rock-(Jam-Bands)/' -e 's/.*piano.*/Rock-(Piano)/' -e 's/.*psychedelic.*/Rock-(Psychedelic)/' -e 's/.*singer.*/Rock-(Singer and Songwriter)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*sound.*/Soundtracks/' -e 's/.*anime.*/Soundtracks-(Anime)/' -e 's/.*kids.*/Soundtracks-(Kids)/' -e 's/.*original.*/Soundtracks-(Original)/' -e 's/.*showtunes.*/Soundtracks-(Showtunes)/' -e 's/.*video.*/Soundtracks-(Video-Game-Music)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*talk.*/Talk/' -e 's/.*comedy.*/Talk-(Comedy)/' -e 's/.*community.*/Talk-(Community)/' -e 's/.*educational.*/Talk-(Educational)/' -e 's/.*government.*/Talk-(Government)/' -e 's/.*other.*/Talk-(Other)/' -e 's/.*political.*/Talk-(Political)/' -e 's/.*scanner.*/Talk-(Scanner)/' -e 's/.*spoken.*/Talk-(Spoken-Word)/' -e 's/.*technology.*/Talk-(Technology)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*themes.*/Themes/' -e 's/.*best.*/Themes-(Best)/' -e 's/.*chill.*/Themes-(Chill)/' -e 's/.*eclectic.*/Themes-(Eclectic)/' -e 's/.*female.*/Themes-(Female)/' -e 's/.*heartache.*/Themes-(Heartache)/' -e 's/.*instrumental.*/Themes-(Instrumental)/' -e 's/.*lgbt.*/Themes-(LGBT)/' -e 's/.*romance.*/Themes-(Love-and-Romance)/' -e 's/.*party.*/Themes-(Party-Mix)/' -e 's/.*patriotic.*/Themes-(Patriotic)/' -e 's/.*rainy.*/Themes-(Rainy-Day-Mix)/' -e 's/.*reality.*/Themes-(Reality)/' -e 's/.*sexy.*/Themes-(Sexy)/' -e 's/.*shuffle.*/Themes-(Shuffle)/' -e 's/.*travel.*/Themes-(Travel-Mix)/' -e 's/.*tribute.*/Themes-(Tribute)/' -e 's/.*trippy.*/Themes-(Trippy)/' -e 's/.*work.*/Themes-(Work-Mix)/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*mixed.*/Mixed/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*volksmu.*/Volksmusik/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*baladas.*/Balladen/' -e 's/.*balladen.*/Balladen/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*chart.*/Charts/'`

			ROUND2=`echo $ROUND2 | sed -e 's/.*groo.*/Groove/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*hit.*/Hits/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*holiday.*/Holiday/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*live.*/Live/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*love.*/Lovesongs/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*musical.*/Musical/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*oldi.*/oldi/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*oldskool.*/Oldskool/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*soul.*/Soul/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*top100.*/Top100/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*underground.*/Underground/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*urban.*/Urban/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*rbn.*/Rbn/'`
			ROUND2=`echo $ROUND2 | sed -e 's/.*professional.*/Professional/'`

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
			elif [ "$ROUND2" = "International" ] || [ "$ROUND2" = "International-(America)" ] || [ "$ROUND2" = "International-(Italia)" ] || [ "$ROUND2" = "International-(Korea)" ] || [ "$ROUND2" = "International-(Spanish)" ] || [ "$ROUND2" = "International-(African)" ] || [ "$ROUND2" = "International-(Afrikaans)" ] || [ "$ROUND2" = "International-(Arabic)" ] || [ "$ROUND2" = "International-(Asian)" ] || [ "$ROUND2" = "International-(Bollywood)" ] || [ "$ROUND2" = "International-(Brazilian)" ] || [ "$ROUND2" = "International-(Caribbean)" ] || [ "$ROUND2" = "International-(Celtic)" ] || [ "$ROUND2" = "International-(Chinese)" ] || [ "$ROUND2" = "International-(European)" ] || [ "$ROUND2" = "International-(Filipino)" ] || [ "$ROUND2" = "International-(French)" ] || [ "$ROUND2" = "International-(German)" ] || [ "$ROUND2" = "International-(Greek)" ] || [ "$ROUND2" = "International-(Hawaiian-and-Pacific)" ] || [ "$ROUND2" = "International-(Hebrew)" ] || [ "$ROUND2" = "International-(Hindi)" ] || [ "$ROUND2" = "International-(Indian)" ] || [ "$ROUND2" = "International-(Islamic)" ] || [ "$ROUND2" = "International-(Japanese)" ] || [ "$ROUND2" = "International-(Klezmer)" ] || [ "$ROUND2" = "International-(Korean)" ] || [ "$ROUND2" = "International-(Mediterranean)" ] || [ "$ROUND2" = "International-(Middle-Eastern)" ] || [ "$ROUND2" = "International-(North-American)" ] || [ "$ROUND2" = "International-(Russian)" ] || [ "$ROUND2" = "International-(Soca)" ] || [ "$ROUND2" = "International-(South-American)" ] || [ "$ROUND2" = "International-(Tamil)" ] || [ "$ROUND2" = "International-(Turkish)" ] || [ "$ROUND2" = "International-(Worldbeat)" ] || [ "$ROUND2" = "International-(Zouk)" ]; then
				GENRE="$ROUND2"				
			elif [ "$ROUND2" = "Jazz" ] || [ "$ROUND2" = "Jazz-(Acid)" ] || [ "$ROUND2" = "Jazz-(Avant-Garde)" ] || [ "$ROUND2" = "Jazz-(Big-Band)" ] || [ "$ROUND2" = "Jazz-(Bop)" ] || [ "$ROUND2" = "Jazz-(Classic)" ] || [ "$ROUND2" = "Jazz-(Cool)" ] || [ "$ROUND2" = "Jazz-(Fusion)" ] || [ "$ROUND2" = "Jazz-(Hard-Bop)" ] || [ "$ROUND2" = "Jazz-(Latin)" ] || [ "$ROUND2" = "Jazz-(Smooth)" ] || [ "$ROUND2" = "Jazz-(Swing)" ] || [ "$ROUND2" = "Jazz-(Vocal)" ] || [ "$ROUND2" = "Jazz-(World-Fusion)" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Latin" ] || [ "$ROUND2" = "Latin-(Bachata)" ] || [ "$ROUND2" = "Latin-(Banda)" ] || [ "$ROUND2" = "Latin-(Bossa-Nova)" ] || [ "$ROUND2" = "Latin-(Cumbia)" ] || [ "$ROUND2" = "Latin-(Mariachi)" ] || [ "$ROUND2" = "Latin-(Merengue)" ] || [ "$ROUND2" = "Latin-(Ranchera)" ] || [ "$ROUND2" = "Latin-(Reggaeton)" ] || [ "$ROUND2" = "Latin-(Regional-Mexican)" ] || [ "$ROUND2" = "Latin-(Salsa)" ] || [ "$ROUND2" = "Latin-(Tango)" ] || [ "$ROUND2" = "Latin-(Tejano)" ] || [ "$ROUND2" = "Latin-(Tropicalia)" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Metal" ] || [ "$ROUND2" = "Metal-(Black)" ] || [ "$ROUND2" = "Metal-(Extreme)" ] || [ "$ROUND2" = "Metal-(Grindcore)" ] || [ "$ROUND2" = "Metal-(Hair)" ] || [ "$ROUND2" = "Metal-(Heavy)" ] || [ "$ROUND2" = "Metal-(Power)" ] || [ "$ROUND2" = "Metal-(Progressive)" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Misc" ] || [ "$ROUND2" = "Misc-(New-Age)" ] || [ "$ROUND2" = "Misc-(Environmental)" ] || [ "$ROUND2" = "Misc-(Ethnic-Fusion)" ] || [ "$ROUND2" = "Misc-(Healing)" ] || [ "$ROUND2" = "Misc-(Meditation)" ] || [ "$ROUND2" = "Misc-(Spiritual)" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Pop" ] || [ "$ROUND2" = "Pop-(Adult-Contemporary)" ] || [ "$ROUND2" = "Pop-(Barbershop)" ] || [ "$ROUND2" = "Pop-(Bubblegum)" ] || [ "$ROUND2" = "Pop-(Idols)" ] || [ "$ROUND2" = "Pop-(Soft)" ] || [ "$ROUND2" = "Pop-(Teen)" ] || [ "$ROUND2" = "Pop-(Top-40)" ] || [ "$ROUND2" = "Pop-(World)" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Public-Radio" ] || [ "$ROUND2" = "Public-Radio-(News)" ] || [ "$ROUND2" = "Public-Radio-(Sports)" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Rap" ] || [ "$ROUND2" = "Rap-(Dirty-South)" ] || [ "$ROUND2" = "Rap-(East-Coast)" ] || [ "$ROUND2" = "Rap-(Freestyle)" ] || [ "$ROUND2" = "Rap-(Gangsta)" ] || [ "$ROUND2" = "Rap-(Hip-Hop)" ] || [ "$ROUND2" = "Rap-(Mixtapes)" ] || [ "$ROUND2" = "Rap-(Old-School)" ] || [ "$ROUND2" = "Rap-(Turntablism)" ] || [ "$ROUND2" = "Rap-(Underground-Hip-Hop)" ] || [ "$ROUND2" = "Rap-(West-Coast Rap)" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Reggae" ] || [ "$ROUND2" = "Reggae-(Contemporary)" ] || [ "$ROUND2" = "Reggae-(Dancehall)" ] || [ "$ROUND2" = "Reggae-(Dub)" ] || [ "$ROUND2" = "Reggae-(Ragga)" ] || [ "$ROUND2" = "Reggae-(Reggae-Roots)" ] || [ "$ROUND2" = "Reggae-(Rock-Steady)" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Rock" ] || [ "$ROUND2" = "Rock-(Adult-Album-Alternative)" ] || [ "$ROUND2" = "Rock-(British-Invasion)" ] || [ "$ROUND2" = "Rock-(Garage)" ] || [ "$ROUND2" = "Rock-(Glam)" ] || [ "$ROUND2" = "Rock-(Jam-Bands)" ] || [ "$ROUND2" = "Rock-(Piano)" ] || [ "$ROUND2" = "Rock-(Psychedelic)" ] || [ "$ROUND2" = "Rock-(Singer and Songwriter)" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Soundtracks" ] || [ "$ROUND2" = "Soundtracks-(Anime)" ] || [ "$ROUND2" = "Soundtracks-(Kids)" ] || [ "$ROUND2" = "Soundtracks-(Original)" ] || [ "$ROUND2" = "Soundtracks-(Showtunes)" ] || [ "$ROUND2" = "Soundtracks-(Video-Game-Music)" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Talk" ] || [ "$ROUND2" = "Talk-(Comedy)" ] || [ "$ROUND2" = "Talk-(Community)" ] || [ "$ROUND2" = "Talk-(Educational)" ] || [ "$ROUND2" = "Talk-(Government)" ] || [ "$ROUND2" = "Talk-(Other)" ] || [ "$ROUND2" = "Talk-(Political)" ] || [ "$ROUND2" = "Talk-(Scanner)" ] || [ "$ROUND2" = "Talk-(Spoken-Word)" ] || [ "$ROUND2" = "Talk-(Technology)" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Themes" ] || [ "$ROUND2" = "Themes-(Best)" ] || [ "$ROUND2" = "Themes-(Chill)" ] || [ "$ROUND2" = "Themes-(Eclectic)" ] || [ "$ROUND2" = "Themes-(Female)" ] || [ "$ROUND2" = "Themes-(Heartache)" ] || [ "$ROUND2" = "Themes-(Instrumental)" ] || [ "$ROUND2" = "Themes-(LGBT)" ] || [ "$ROUND2" = "Themes-(Love-and-Romance)" ] || [ "$ROUND2" = "Themes-(Party-Mix)" ] || [ "$ROUND2" = "Themes-(Patriotic)" ] || [ "$ROUND2" = "Themes-(Rainy-Day-Mix)" ] || [ "$ROUND2" = "Themes-(Reality)" ] || [ "$ROUND2" = "Themes-(Sexy)" ] || [ "$ROUND2" = "Themes-(Shuffle)" ] || [ "$ROUND2" = "Themes-(Travel-Mix)" ] || [ "$ROUND2" = "Themes-(Tribute)" ] || [ "$ROUND2" = "Themes-(Trippy)" ] || [ "$ROUND2" = "Themes-(Work-Mix)" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Mixed" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "volksmusik" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Balladen" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Charts" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Groove" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Hits" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Holiday" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Live" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Lovesongs" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Musical" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Oldies" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Oldskool" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Soul" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Top100" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Underground" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Urban" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Rbn" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "Professional" ]; then
				GENRE="$ROUND2"
			elif [ "$ROUND2" = "International" ]; then
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
				echo $back >> _liste_unknown_samename
				ROUND2="unknown-samename"
			fi

			if [ "$GENRE" = "unknown" ]; then
				echo $back >> _liste_unknown
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
	
	cat _liste_unknown | sort -u > _liste_sorted
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
