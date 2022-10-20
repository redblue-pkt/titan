#!/bin/sh

CONFIG=`echo "$0callmon.conf" | sed s/fritzbox_msg_new.sh//`
NC=`echo "$0nc" | sed s/fritzbox_msg_new.sh//`
if [ ! -x $NC ] ; then
   NC="nc"
fi

ICONV=`echo "$0iconv" | sed s/fritzbox_msg_new.sh//`
CURL=`echo "$0curl" | sed s/fritzbox_msg_new.sh//`
if [ ! -x $CURL ] ; then
   CURL="curl"
fi
LPATH=`echo "$0" | sed s/'\/fritzbox_msg_new.sh'//`
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$LPATH
export LD_LIBRARY_PATH

. $CONFIG
#. ./callmon.conf
#
#-------------------------------------------------------------------------------
#
# To enable the callmonitor on your fritzbox use your telephone and call
#    #96*5*         to enable
#    #96*4*         to disable
#
#
#SETUP---start------------------------------------------------------------------
#
##FRITZBOXIP=10.0.0.99   #IP of your fritzbox
TELDPORT=1012            #callmonitor port (default 1012)
#
#SETUP---end--------------------------------------------------------------------
#
#
#Options---start----------------------------------------------------------------
#
Version=v2.60                   #Zur Versionsanzeige im telnet
debug=0                             #wenn debug auf 1 steht wird am TV nichts angezeigt
##Ziel=4711                         #bitte Rufnummer eintragen die Überwacht werden soll 
if [ ! $Ziel ]; then
    Ziel=4711
fi
##Ziel_2=4711                   #2te Rufnummer die Überwacht werden soll
if [ ! $Ziel_2 ]; then
    Ziel_2=4711
fi
                                            #wenn keine zweite dann gleiche nochmal eintragen!
##Alle=1                  #wenn hier eine 1 steht werden alle Rufnummern überwacht
                      #unabhängig von Ziel und Ziel_2
##monRing=1               #eingehende Anrufe anzeigen
##muteRing=0              #Ton aus bei Anruf
##monCall=0               #ausgehende Anrufe anzeigen
monDisconnect=0       #Beenden eines Anrufes anzeigen. Hier ist noch ein Bug drin!
                      #Es werden immer alle Gespräche überwacht (alle Rufnummern
                      #werden überwacht- Ziel und Ziel_2 funktionieren hier nicht
                      #). Ich habe das deshalb immer auf 0.
ip=127.0.0.1            #ip Adresse der Dreambox
##anzeigetimeout=10       #Wie lange soll die Meldung angezeigt werden?
##anzeigewennaus=0        #Wenn 1 dann werden alle Meldungen gespeichert und angezeigt
                      #wenn die Dreambox eingeschaltet wird (kann ganz schön viel
                      #werden!). Wenn 0 werden die Meldungen nicht angezeigt wenn 
                      #die Box aus war als z.B. ein Anruf eingegangen ist.
                      #Empfehlen tue ich eine 0. Wer weis was passiert wenn man im
                      #Urlaub ist und etliche Meldungen gespeichert werden.
uebersichtsseite=0    #Wenn anzeige wenn aus und uebersichtsseite 1 ist, dann werden
                      #nach dem einschalten der Dream aus dem Standy alle verpassten
                      #Anrufe in einer Übersichtseite angezeigt mit Datum und Uhrzeit
                      #Hierzu muss OpenPanel installiert sein.
                      #Hier muss aber noch ein Start Script erstellt werden!                  
loginname=root        #Loginname für Dreambox (Beispiel:root)
passwort=kathrein       #passwort für Dreambox (Beispiel:dreambox)
vorwahl=030                     #Eigene Vorwahl eintragen
#
#Options---end------------------------------------------------------------------
#
#
#---- do not change anything below here ----
#
#additional paths to files we use (will be created automatically if not present)
#detecting Dreambox-Type (DM7020 or DM7000 or DM500 or DM600PVR)
 FRITZCACHE=/tmp/fritzbox.cache              #stores successfull reverse searches
#FRITZBUCH=/var/fritzbuch.tele		     #original
 FRITZBUCH=/mnt/swapextensions/fritzbuch.tele #TitanNit
 TMPFILE=/tmp/fritzbox.tmp                   #stores page from wget command
 TMPSTATUS=/tmp/fritzdream.status            #status der Dreambox (standby usw)
 TMPRING=/tmp/tmpring.xml                    #stores missed calls 
 TMPINCOMING=/tmp/ringin.tmp                     #for checking ring overview

NULL=/dev/null                              #'NULL'
SCRIPT=$0                                   #full path to this script
export SCRIPT

#-------------------------------------------------------------------------------
#definition of the reverse search online (GOYELLOW.DE)
#-------------------------------------------------------------------------------
SEARCHPRV="GoYellow"
SEARCHURL="http://www.goyellow.de/schnellsuche/?LIP=simpleSearch&MDN="
SEARCHURL="http://www.goyellow.de/inverssuche/?TEL="
SEARCHPDETAIL="Detailinformationen"
SEARCHERR="Keine.Einträge.gefunden"        #replace spaces with dots (regex)
#-------------------------------------------------------------------------------
#check if FritzBox! is up and callmonitor port is reachable. if not exit.
#-------------------------------------------------------------------------------
#cat /dev/null | ./nc $FRITZBOXIP $TELDPORT
cat /dev/null | $NC $FRITZBOXIP $TELDPORT
if [ $? != 0 ]; then
 echo "Fritzbox (" $FRITZBOXIP ") ist nicht auf Port" $TELDPORT "erreichbar!"
 echo "Skript wird beendet!"
 FRITZUP=down
 exit 1
else
 FRITZUP=up
fi
#-------------------------------------------------------------------------------
#if script is called without any parameters show help text
#-------------------------------------------------------------------------------
if [ "$1" = "" ]; then
 echo
 echo "Usage: $SCRIPT START STOP CLEANUP SORTNAME SORTNUMBER TEST [phonenumber]"
 echo
 echo "START               Start Script and deamonize"
 echo "STOP                kill all processes we created"
 echo "CLEANUP             delete all temporary files incl. cache"
 echo "SORTNAME            sort cache file by name"
 echo "SORTNAME -r         sort cache file by name (reverse)"
 echo "SORTNUMBER          sort cache file by number"
 echo "SORTNUMBER -r       sort cache file by number (reverse)"
 echo "TEST [phonenumber]  For testing connectivity and trying the search"
 echo "loadPhoneBook"
 echo
fi
#-------------------------------------------------------------------------------
#reading function to interpret the data read from FritzBox! callmonitor port
#-------------------------------------------------------------------------------
mysearch () {

if [ "$1" = "Unbekannt" ]; then
   NAME="ohne Rufnummer"
   ADDRESSE=""
else
   #remove spaces from $1 and replace '+49' with ''0'
   NUMMER=`echo $1 | sed -e "s@\ @@g" -e "s@+49@0@"`
   #UH
   NUMMER=`echo $NUMMER | sed -e "s/^1/${vorwahl}1/g"`
   NUMMER=`echo $NUMMER | sed -e "s/^2/${vorwahl}2/g"`
   NUMMER=`echo $NUMMER | sed -e "s/^3/${vorwahl}3/g"`
   NUMMER=`echo $NUMMER | sed -e "s/^4/${vorwahl}4/g"`
   NUMMER=`echo $NUMMER | sed -e "s/^5/${vorwahl}5/g"`
   NUMMER=`echo $NUMMER | sed -e "s/^6/${vorwahl}6/g"`
   NUMMER=`echo $NUMMER | sed -e "s/^7/${vorwahl}7/g"`
   NUMMER=`echo $NUMMER | sed -e "s/^8/${vorwahl}8/g"`
   NUMMER=`echo $NUMMER | sed -e "s/^9/${vorwahl}9/g"`
   echo "$NUMMER"
   echo "Searching for $NUMMER in cache file..."
   if [ ! -f $FRITZBUCH ]; then  touch $FRITZBUCH; fi
    #try to get caller-id via $NUMBER from $FRITZBUCH
    NAME=`grep $NUMMER $FRITZBUCH | sed -e "s@$NUMMER\#@@" -e "s@;@\n@g"`
    ADDRESSE=`echo $NAME | sed -e 's/.*->//g'`
    NAME=`echo $NAME | sed -e 's/->.*//g'`
    echo -e
    echo -e "NAME nach Cache Suche: $NAME"
    echo -e "ADRESSE nach Cache Suche: $ADDRESSE"
    
   if [ "$NAME" = "" ]; then
    #try to get caller-id of $NUMMER via internet
    echo "Searching for $NUMMER via $SEARCHPRV"
    wget "$SEARCHURL$NUMMER"  -O $TMPFILE
    #extract caller-id from online result
    #UH20100112
#    NAME=`grep $SEARCHPDETAIL $TMPFILE | head -n 1 | sed -e "/</{; :loop; s/<[^<]*>//g; /</{; N; b loop; }; };"`
#    ADDRESSE=`grep "address" $TMPFILE| head -n 1`
#    ADDRESSE=`echo $ADDRESSE | sed 's/<[^>]*>/ /g'`

    NAME=`grep $SEARCHPDETAIL $TMPFILE | head -n 1 | sed -e 's/.*Detailinformationen zu //g' | sed -e 's/ in .*//g'`
    STRASSE=`grep '<span class="street street-address">' $TMPFILE | head -n 1 | sed -e 's/<span class="street street-address">//g' | sed -e 's/<\/span>.*//g'`
    POSTCODE=`grep '<span class="postcode postal-code">' $TMPFILE | head -n 1 | sed -e 's/.*<span class="postcode postal-code">//g' | sed -e 's/<\/span>.*//g'`
    ORT=`grep '<span class="city locality">' $TMPFILE | head -n 1 | sed -e 's/.*<span class="city locality">//g' | sed -e 's/<\/span>.*//g'`
    ADDRESSE="$STRASSE~n$POSTCODE $ORT"

    if [ "$NAME" = "" ]; then
     if grep $SEARCHERR $TMPFILE > $NULL; then
      NAME="Unbekannt"
      ADDRESSE=""
     fi;
    fi;
    if [ "$NAME" = "" ]; then
     NAME="Unbekannt"
     ADDRESSE=""
    else
     #dirty workarround to fix 'new-lines' when multiple caller-ids exist
     #to make sure only 1 line will be written to cache file
     #NAME=`printf %s***** $NAME | sed -e "s@\*\*\*\*\*@ @g;"
     #echo "Name gefunden in Goyellow: " $NAME 
     echo -e
     echo -e "$NUMMER#$NAME->$ADDRESSE" >> $FRITZCACHE
     export NAME
     export ADDRESSE
    fi;
   fi;
fi;
};


rdom () { local IFS=\> ; read -d \< E C ;}

loadXML(){
	
	echo "Lese ID: $1"
	$CURL --data "sid=$SID&pbid=$1" http://$FRITZBOXIP:49000/phonebook.lua  > /tmp/curlhelp7.tmp
	        
	 while rdom; do
	    if [[ $E = realName ]]; then
		name=$C
	    fi
	    if [[ $E = number* ]]; then
		type=`echo "$E" | cut -d'"' -f2 | sed s/" "//g | sed s/"home"/"privat"/g | sed s/"work"/"geschaeftl."/g | sed s/"mobile"/"mobil"/g `
		nummer=$C
		tel=`echo $nummer | sed s/"+49"/"0"/g| sed s/" "//g`
		echo "$tel#$name->$type"  >> $FRITZBUCH
	    fi
	done < /tmp/curlhelp7.tmp 
};

loadXMLsh4(){
	local IFS=\> 	
	echo "Lese ID: $1"	
	$CURL --data "sid=$SID&pbid=$1" http://$FRITZBOXIP:49000/phonebook.lua  > /tmp/curlhelp7.tmp
	grep "realName\|number " /tmp/curlhelp7.tmp > /tmp/curlhelp7a.tmp
	 
	while read line E; do
		C=`echo $E | cut -d'/' -f2`
		
		if [[ $C = realName ]]; then
			name=`echo $E | cut -d'<' -f1`
		fi
		if [[ $C = number ]]; then			
			type=`echo $line| cut -d'"' -f2 | sed s/" "//g | sed s/"home"/"privat"/g | sed s/"work"/"geschaeftl."/g | sed s/"mobile"/"mobil"/g `
			tel=`echo $E | cut -d'<' -f1 | sed s/"+49"/"0"/g| sed s/" "//g`
			echo "$tel#$name->$type"  >> $FRITZBUCH
		fi
		    
	done < /tmp/curlhelp7a.tmp
};

loadFritzBook70 () {
    echo "Version 70"
                
	Challenge=`wget -O - "http://$FRITZBOXIP/login_sid.lua" 2>/dev/null | grep Challenge | cut -d"<" -f6 | cut -d">" -f2`
	Passwd="$FritzPass"
	# login aufbauen und hashen
	echo "login aufbauen und hashen start"
	CPSTR="$Challenge-$Passwd"
	MD5=`echo -n $CPSTR | $ICONV -f ISO8859-1 -t UTF-16LE > /tmp/helpx; md5sum /tmp/helpx | awk '{print substr($0,1,32)}'`
	RESPONSE="$Challenge-$MD5"

	POSTDATA="username=$FritzUser&response=$RESPONSE"

	SID=`wget -O - --post-data="$POSTDATA" "http://$FRITZBOXIP/login_sid.lua" 2>/dev/null  | grep SID | cut -d"<" -f4 | cut -d">" -f2`

	echo "login aufbauen und hashen ende SID:$SID"
              
        BOOKID=-1	#Liste mit möglichen IDs, klappt ab v7.20 nicht mehr?
        BOOKID=240	#Google
        BOOKID=0	#Standard
        
        $CURL --data "sid=$SID&pbid=$BOOKID" http://$FRITZBOXIP:49000/phonebook.lua | grep "id:"  > /tmp/curlhelp1.tmp
        
        #langsamere Routine nur bei sh4-Boxen verwenden
        sh4=`cat /proc/cpuinfo | grep "sh4" | wc -c`
        
        while read line 
        	do
        		BOOKID=`echo $line | cut -d':' -f2 | sed s/" \, name"//g`
        		NAME=`echo $line | cut -d':' -f3`
        		echo "Lese $NAME BookID: $BOOKID"
        		if [ $sh4 -eq 0 ]; then
        			loadXML $BOOKID
        		else
        			loadXMLsh4 $BOOKID
        		fi
        	done < /tmp/curlhelp1.tmp     

	
};

loadFritzBook () {
 if [ $usePhoneBook = "1" ]; then
        #if [ -f $FRITZBUCH ];   then  rm $FRITZBUCH; fi
        if [ ! -f $FRITZBUCH ]; then
            echo "FritzBook new -> $FRITZBUCH"
            
            loadFritzBook70
            
            if [ -f /tmp/curlhelp1.tmp ];   then  rm /tmp/curlhelp1.tmp; fi
            if [ -f /tmp/curlhelp7.tmp ];   then  rm /tmp/curlhelp7.tmp; fi
            if [ -f /tmp/curlhelp7a.tmp ];   then  rm /tmp/curlhelp7a.tmp; fi
        fi
    fi  

};
#-------------------------------------------------------------------------------
#Script has been initiated with parameters
#(START,RING,CALL,DISCONNECT,CONNECT,STOP,SORTNAME,SORTNUMBER,TEST,CLEANUP)

timeout=14
while [ -e /tmp/callrun ]; do
    if [ $timeout != "0" ]; then
        timeout=$(($timeout-1))
        sleep 1
    else
        break
    fi
done

touch /tmp/callrun

timeout=14
while [ -e /tmp/xmessage ]; do
    if [ $timeout != "0" ]; then
        timeout=$(($timeout-1))
        sleep 1
    else
        break
    fi
done
case $1 in
  START|start)
  echo
  echo "------------------------------------"
  echo "Fritzbox Callmonitor" $Version "gestartet"
  echo "---Folgende Optionen sind gesetzt---"
  echo "Debugmodus: " $debug
  echo "Ueberwachte Rufnummern: " $Ziel " und " $Ziel_2
  echo "Alle Rufnummern ueberwachen: " $Alle
  echo "Eingehende Anrufe ueberwachen: " $monRing
  echo "Ausgehende Anrufe ueberwachen: " $monCall
  echo "Verbindungsende ueberwachen: " $monDisconnect
  echo "IP Adresse der TV-Box: " $ip
  echo "Anzeigetimeout: " $anzeigetimeout
  echo "Anzeigewennaus: " $anzeigewennaus
  #echo "Übersichtsseite: " $uebersichtsseite
  echo "Fritzbox Telefonbuch: " $usePhoneBook
  echo "------------------------------------"
  echo "**************************************"
  echo "loadFritzBook start"
  loadFritzBook
  echo "loadFritzBook ende"
  if [ $debug = 0 ]; then
     #wget -T 1 -q -O /dev/null "http://$ip/cgi-bin/xmessage?caption=Skript%20gestartet&body=FritzBox!%20Callmonitor%20gestartet&timeout=3"
     $CURL "http://$ip/cgi-bin/xmessage?caption=Skript%20gestartet&body=FritzBox!%20Callmonitor%20gestartet&timeout=3" -o /dev/null
     (touch /tmp/xmessage; sleep 4; rm  /tmp/xmessage) &
  fi
  #very tricky line to get data from FritzBox! using netcat and divide it using
  #awk. Only make changes to this line if you completely understand it!
  #Make sure the next line end with a trailing '&' or your dreambox will hang!
    #tail -f < /dev/null|./nc $FRITZBOXIP $TELDPORT|awk -F";" '{if ($4==""){$4="Unbekannt"} system (ENVIRON ["SCRIPT"] " "$2" "$1" "$3" "$4" "$5" "$6" "$7 )}' &
    echo "tail start"
    tail -f < /dev/null|$NC $FRITZBOXIP $TELDPORT|awk -F";" '{if ($4==""){$4="Unbekannt"} system (ENVIRON ["SCRIPT"] " "$2" "$1" "$3" "$4" "$5" "$6" "$7 )}' &
    echo "Ende"
     ;;
  RING)
    mysearch $5
    echo $2" "$3" Ankommend: von "$5" auf "$6"und hier noch wert 7 "$7 
    #title of messagebox 
    c=">>>> Anruf ankommend <<<<"
    #prepare text
    #UH20100110
        fuer=$6
    if [ "$6" = "4711" ]
       then fuer="Name1"
#       elif [ "$6" = "MSN2" ]
#          then fuer="Name2"
    fi

#    b=" Anruf von "$5"~n~n "$NAME"~n "$ADDRESSE"~n für "$6
    b=" Anruf von "$5"~n~n "$NAME"~n "$ADDRESSE"~n~n~n für "$fuer

    b1=`echo $b               | sed -e '{s/4711/Name1/g;s/MSN2/Name2/g;s/MSN3/Name3/g;s/SIP0/VOIP0/g;s/SIP1/VOIP1/g;s/SIP2/VOIP2/g;}'` 
#    b1=`echo $b               | sed -e '{s/4711/Name1/g;s/MSN2/Name2/g;s/MSN3/Name3/g;s/SIP0/VOIP0/g;s/SIP1/VOIP1/g;s/SIP2/VOIP2/g;}'` 
    #Wert in Logb zwischenspeichern für TMPRING
    logb=`echo $b1            | sed -e '{s/~n//g;}'`  

    b1=`echo $b1              | sed -e '{s/ /%20/g;s/Ä/%C3%84/g;s/Ö/%C3%96/g;s/Ü/%C3%9C/g;s/ä/%C3%A4/g;s/ö/%C3%B6/g;s/ü/%C3%BC/g;s/á/ss/g;s/&/%26/g;s/~n/%0a/g;s/#/%20/g;}'`
    c1=`echo $c               | sed -e '{s/ /%20/g;s/Ä/%C3%84/g;s/Ö/%C3%96/g;s/Ü/%C3%9C/g;s/ä/%C3%A4/g;s/ö/%C3%B6/g;s/ü/%C3%BC/g;s/á/ss/g;s/&/%26/g;s/~n/%0a/g;s/#/%20/g;}'`
    #Eingehende Anrufe anzeigen?
    if [ $monRing = 1 ]; then
      #Welche Rufnummern sollen überwacht werden?
      if [ $6 = $Ziel -o $6 = $Ziel_2 -o $Alle = 1 ] ; then
        #wget -O- http://$loginname:$passwort@$ip/xml/boxstatus > $TMPSTATUS
        #wget -O- http://$ip/query?boxstatus > $TMPSTATUS
        $CURL http://$ip/query?boxstatus > $TMPSTATUS
        #STANDBY=`grep "standby" $TMPSTATUS| head -n 1 | sed -e '{s/<standby>//g;s/<\/standby>//g}'`
        STANDBY=`grep "standby" $TMPSTATUS| wc -l`
        #STANDBY=0
        echo "Standbystatus: $STANDBY"
          if [ $STANDBY = 0 ]; then
            if [ $debug = 0 ]; then 
              if [ $muteRing = 1 ]; then #wenn Option gesetzt ist dann wird der Ton abgeschaltet 
                #wget -O- http://$loginname:$passwort@127.0.0.1/cgi-bin/audio >> $TMPSTATUS #tonstatus wird ermittelt und in eine Datei geschrieben
                #mute=`grep "mute" $TMPSTATUS| head -n 1 | sed -e '{s/mute://g;s/<br>//g;s/ //g}'`
                #if [ $mute = 0 ]; then
                  #wget -q -O /dev/null "http://@$ip/query?sendrc&rcmute"  >> $NULL #Befehl für Tonsperre
                    $CURL http://$ip/query?mutestatus > $TMPSTATUS
                                mute=`grep "muteon" $TMPSTATUS| wc -l`
                                if [ $mute = 0 ]; then
                        $CURL "http://@$ip/query?sendrc&rcmute"  >> $NULL #Befehl für Tonsperre
                    fi
                #fi
              fi
              #wget -q -O /dev/null "http://$ip/cgi-bin/xmessage?caption=$c1&body=$b1&timeout=$anzeigetimeout"  >> $NULL
                $CURL "http://$ip/cgi-bin/xmessage?caption=$c1&body=$b1&timeout=$anzeigetimeout"  >> $NULL
                (touch /tmp/xmessage; sleep $anzeigetimeout; rm  /tmp/xmessage) &
            fi  
          else
            echo "Dreambox ist im Standby"
            if [ $anzeigewennaus = 1 ]; then #wenn die Box im Standby ist und die Option gesetzt ist dann wird nach dem einschalten der Box die Meldung angezeigt
              echo "Meldung wird wenn debug 0 ist trotzdem angezeigt da -anzeigewennaus- 1 ist"
              #wenn die Leitung überwacht wird dann den Anruf erst einmal in TMPRING schreiben und TMPINCOMING schreiben
              if [ $uebersichtsseite = 1 ]; then
                echo "Anruf wird für Übersichtsseite gespeichert"
              if [ -f $TMPRING ];     then
                  echo "TMPRING gibt es schon neu "
                  echo "<shell name=\"von $5 $NAME\" shortcut=\"\" help=\"$2 $3\"><sh></sh></shell>
<line name=\"$ADDRESSE\" />
<line name=\"für $6\" />
<separator />" >>$TMPRING
               else
                  echo "TMPRING gibt es noch nicht"         
                  echo "<menu name=\"Verpasste Anrufe\">
<shell name=\"von $5 $NAME\" shortcut=\"\" help=\"$2 $3\"><sh></sh></shell>     
<line name=\"$ADDRESSE\" />
<line name=\"für $6\" />
<separator />" >$TMPRING
                fi
                  echo "nach der Abfrage ob es TMPring schon gibt"
                  echo "1" > $TMPINCOMING
              else
                if [ $debug = 0 ]; then     
                  echo "Meldung wird nach dem einschalten der Box angezeigt"
                  #wget -q -O /dev/null "http://$ip/cgi-bin/xmessage?caption=$c1&body=$b1&timeout=$anzeigetimeout"  >> $NULL
                  $CURL "http://$ip/cgi-bin/xmessage?caption=$c1&body=$b1&timeout=$anzeigetimeout"  >> $NULL
                  (touch /tmp/xmessage; sleep $anzeigetimeout; rm  /tmp/xmessage) &
                fi
              fi  
            fi
          fi
      fi
    fi   
    
    if [ -f $TMPSTATUS ];     then  rm $TMPSTATUS; fi
    if [ -f $TMPFILE ];     then  rm $TMPFILE; fi
    ;;
 
  CALL)
 
 		echo "******************* call"
    #Aufruf Kamera?#
    ext=0
    if [ `echo $7 | grep "callcam1" | wc -l` -eq 1 ]; then
      rm /tmp/callcam*
      touch /tmp/callcam1
      $CURL "http://$ip/query?infobarplugin&IP-Kamera"
      $CURL "http://$ip/query?sendrc&rcok"
      ext=1
		fi
    if [ `echo $7 | grep "callcam2" | wc -l` -eq 1 ]; then
    	rm /tmp/callcam*
      touch /tmp/callcam2
      $CURL "http://$ip/query?infobarplugin&IP-Kamera" 
      CURL "http://$ip/query?sendrc&rcok"
      ext=1
		fi
    
    if [ $ext -eq 0 ]; then
    	mysearch $7
    	echo $2" "$3" Ausgehend: von "$6" auf "$7
     	 #title of messagebox
     	 c="<<<< Anruf abgehend >>>>"
     	 #prepare text
     	 #UH20100110
     	 if [ "$6" = "4711" ]
     	    then von=`echo $6               | sed -e '{s/4711/Name1/g;}'`
#    	     elif [ "$6" = "4712" ]
#    	        then von=`echo $6               | sed -e '{s/4712/MSN2/g;}'`
     	 fi

#      	b=" Anruf von "$6"~n~n für "$7"~n~n "$NAME"~n "$ADDRESSE
      	b=" Anruf von "$von"~n~n~n für "$7"~n~n "$NAME"~n "$ADDRESSE

      	b1=`echo $b               | sed -e '{s/MSN1/Name1/g;s/MSN2/Name2/g;s/MSN3/Name3/g;s/SIP0/VOIP0/g;s/SIP1/VOIP1/g;s/SIP2/VOIP2/g;}'` 
#     	 b1=`echo $b               | sed -e '{s/MSN1/Name1/g;s/MSN2/Name2/g;s/MSN3/Name3/g;s/SIP0/VOIP0/g;s/SIP1/VOIP1/g;s/SIP2/VOIP2/g;}'` 

      	b1=`echo $b1              | sed -e '{s/ /%20/g;s/Ä/%C3%84/g;s/Ö/%C3%96/g;s/Ü/%C3%9C/g;s/ä/%C3%A4/g;s/ö/%C3%B6/g;s/ü/%C3%BC/g;s/á/ss/g;s/&/%26/g;s/~n/%0a/g;s/#/%20/g;}'`
      	c1=`echo $c               | sed -e '{s/ /%20/g;s/Ä/%C3%84/g;s/Ö/%C3%96/g;s/Ü/%C3%9C/g;s/ä/%C3%A4/g;s/ö/%C3%B6/g;s/ü/%C3%BC/g;s/á/ss/g;s/&/%26/g;s/~n/%0a/g;s/#/%20/g;}'`
      	#tempfile schreiben für Übersicht
      fi
      #Ausgehende Anrufe anzeigen?
      if [ $monCall = 1 ] && [ $ext -eq 0 ] ; then
        #Welche Rufnummern sollen überwacht werden?
        if [ $6 = $Ziel -o $6 = $Ziel_2 -o $Alle = 1 ] ; then
          echo "Diese Rufnummer wird überwacht"
          #wget -O- http://$loginname:$passwort@$ip/xml/boxstatus > $TMPSTATUS
          #wget -O- http://$ip/query?boxstatus > $TMPSTATUS
          $CURL http://$ip/query?boxstatus > $TMPSTATUS
          #STANDBY=`grep "standby" $TMPSTATUS| head -n 1 | sed -e '{s/<standby>//g;s/<\/standby>//g}'`
          STANDBY=`grep "standby" $TMPSTATUS| wc -l`
          STANDBY=0
          echo "Standbystatus: $STANDBY"
          if [ $STANDBY = 0 ]; then
            echo "Wenn debug noch 0 ist wird die Meldung angezeigt"
            if [ $debug = 0 ]; then
              #wget -q -O /dev/null "http://$ip/cgi-bin/xmessage?caption=$c1&body=$b1&timeout=$anzeigetimeout"  >> $NULL
              $CURL "http://$ip/cgi-bin/xmessage?caption=$c1&body=$b1&timeout=$anzeigetimeout"  >> $NULL
              (touch /tmp/xmessage; sleep $anzeigetimeout; rm  /tmp/xmessage) &
            fi
          else
            echo "Dreambox ist im Standby"
            if [ $anzeigewennaus = 1 ]; then
              echo "Meldung wird wenn debug 0 ist trotzdem angezeigt da -anzeigewennaus- 1 ist"
              if [ $debug = 0 ]; then     
                echo "Meldung wird nach dem einschalten der Box angezeigt"
                #wget -q -O /dev/null "http://$ip/cgi-bin/xmessage?caption=$c1&body=$b1&timeout=$anzeigetimeout"  >> $NULL
                $CURL "http://$ip/cgi-bin/xmessage?caption=$c1&body=$b1&timeout=$anzeigetimeout"  >> $NULL
                (touch /tmp/xmessage; sleep $anzeigetimeout; rm  /tmp/xmessage) &
              fi
            fi  
          fi  
        fi
      fi
      if [ -f $TMPSTATUS ];     then  rm $TMPSTATUS; fi
      if [ -f $TMPFILE ];     then  rm $TMPFILE; fi
      ;;
  
  DISCONNECT)
  #wget -O- http://$loginname:$passwort@$ip/xml/boxstatus > $TMPSTATUS
  #wget -O- http://$ip/query?boxstatus > $TMPSTATUS
  $CURL http://$ip/query?boxstatus > $TMPSTATUS
  #STANDBY=`grep "standby" $TMPSTATUS| head -n 1 | sed -e '{s/<standby>//g;s/<\/standby>//g}'`
  STANDBY=`grep "standby" $TMPSTATUS| wc -l`
  if [ $STANDBY = 0 ]; then
    if [ $muteRing = 1 ]; then #wenn Option gesetzt ist dann wird der Ton abgeschaltet 
        $CURL http://$ip/query?mutestatus > $TMPSTATUS
        mute=`grep "muteon" $TMPSTATUS| wc -l`
        if [ $mute = 1 ]; then
            $CURL "http://@$ip/query?sendrc&rcmute"  >> $NULL   
        fi
      #wget -O- http://$loginname:$passwort@127.0.0.1/cgi-bin/audio >> $TMPSTATUS #tonstatus wird ermittelt und in eine Datei geschrieben
      #$CURL http://$loginname:$passwort@127.0.0.1/cgi-bin/audio >> $TMPSTATUS #tonstatus wird ermittelt und in eine Datei geschrieben
      #'mute=`grep "mute" $TMPSTATUS| head -n 1 | sed -e '{s/mute://g;s/<br>//g;s/ //g}'`
      #if [ $mute = 1 ]; then
        #wget -q -O /dev/null "http://$loginname:$passwort@$ip/cgi-bin/rc?113"  >> $NULL #Befehl für Tonsperre
        #$CURL "http://$loginname:$passwort@$ip/cgi-bin/rc?113"  >> $NULL #Befehl für Tonsperre
      #fi
    fi
  fi 
     h=$(($5/3600))                        #get hours from   $5
     m=$(($5%3600/60))                     #get minutes from $5
     s=$((($5%3600)%60))                   #get seconds from $5
     if [ $h -lt 10 ]; then h="0"$h; fi    #add leading 0 if needed
     if [ $m -lt 10 ]; then m="0"$m; fi    #add leading 0 if needed
     if [ $s -lt 10 ]; then s="0"$s; fi    #add leading 0 if needed
     #title of messagebox
     c="-- Anrufauswertung --"
     #prepare text
     b="~n Aufgelegt nach "$5" Sekunden Dauer (hh:mm:ss): "$h":"$m":"$s"~n"
     
     #Beenden eines Anrufes anzeigen?
     if [ $monDisconnect = 1 ]; then  
          if [ $debug = 0 ]; then
          #generate the messagebox
          b1=`echo $b               | sed -e '{s/ /%20/g;s/Ä/%C3%84/g;s/Ö/%C3%96/g;s/Ü/%C3%9C/g;s/ä/%C3%A4/g;s/ö/%C3%B6/g;s/ü/%C3%BC/g;s/á/ss/g;s/&/%26/g;s/~n/%0a/g;s/#/%20/g;}'`
          c1=`echo $c               | sed -e '{s/ /%20/g;s/Ä/%C3%84/g;s/Ö/%C3%96/g;s/Ü/%C3%9C/g;s/ä/%C3%A4/g;s/ö/%C3%B6/g;s/ü/%C3%BC/g;s/á/ss/g;s/&/%26/g;s/~n/%0a/g;s/#/%20/g;}'`
          echo "Disconnect"
          if [ $STANDBY = 0 ]; then
           #wget -q -O /dev/null "http://$ip/cgi-bin/xmessage?caption=$c1&body=$b1&timeout=$anzeigetimeout"          >> $NULL
           $CURL -q -O /dev/null "http://$ip/cgi-bin/xmessage?caption=$c1&body=$b1&timeout=$anzeigetimeout"          >> $NULL
           (touch /tmp/xmessage; sleep $anzeigetimeout; rm  /tmp/xmessage) &
          fi
         fi
     fi 
     if [ -f $TMPSTATUS ];     then  rm $TMPSTATUS; fi
     if [ -f $TMPINCOMING ];     then  rm $TMPINCOMING; fi
     ;;
  CONNECT)
    echo "connect ist jetzt aktiv - verbunden"
    #kontrollieren ob dieses ein eingehender Anruf war
    if [ -f $TMPINCOMING ];     then 
    eingehend=`grep "1" $TMPINCOMING| head -n 1`
    else 
    eingehend="0"
    fi
    #wenn das ein eingehender anruf war dann letzten Zeilen löschen da der Anruf angenommen wurde
    if [ $eingehend = 1 ]; then 
      if [ -f $TMPRING ];     then  sed -i '$D' $TMPRING; fi
      if [ -f $TMPRING ];     then  sed -i '$D' $TMPRING; fi
      if [ -f $TMPRING ];     then  sed -i '$D' $TMPRING; fi
      if [ -f $TMPRING ];     then  sed -i '$D' $TMPRING; fi
       
      if [ -f $TMPINCOMING ];     then  rm $TMPINCOMING; fi
    fi
     ;;
  SORTNAME|sortname)
    #convert $FRITZCACHE to unix format
    unix2dos -u  $FRITZCACHE
    #convert $FRITZCACHE to upper case
    awk -F '#' '{print toupper($2)"#"$1"#"$2}' $FRITZCACHE|sort $2>$FRITZCACHE"_sort"
    #sort $FRITZCACHE
    awk -F '#' '{print $2"#"$3}' $FRITZCACHE"_sort" > $FRITZCACHE
     ;;
  SORTNUMBER|sortnumber)
    #sort FRITZCACHE by telephone number
    sort $FRITZCACHE $2 > $FRITZCACHE"_sort"
    mv $FRITZCACHE"_sort" $FRITZCACHE
    ;;
  CLEANUP|cleanup)                       #cleanup (removes all files)
     echo "deleting $TMPFILE (if exists)"
     if [ -f $TMPFILE ];     then  rm $TMPFILE; fi
     echo "deleting $FRITZCACHE (if exists)"
     if [ -f $FRITZCACHE ];   then  rm $FRITZCACHE; fi
     echo "deleting $FRITZBUCH (if exists)"
     if [ -f $FRITZBUCH ];   then  rm $FRITZBUCH; fi
     ;;
  STOP1|stop1)
     #kill all processes we spawned
     #hopefully only a single tail task is running so we kill it :)
     pid=`ps | grep "tail.-f"| grep -v grep | awk '{print $1}'`
     if [ "X${pid}" != "X" ] ; then
        kill -9 ${pid}
     fi
     pid=`ps |grep "Unbekannt" | grep -v grep | awk '{print $1}'`
     if [ "X${pid}" != "X" ] ; then
        kill -9 ${pid}
     fi
     pid=`ps |grep ".*nc.*$TELDPORT" | grep -v grep | awk '{print $1}'`
     if [ "X${pid}" != "X" ] ; then
        kill -9 ${pid}
     fi
     pid=`ps |grep $SCRIPT | grep -v grep | awk '{print $1}'`
     if [ "X${pid}" != "X" ] ; then
        kill -9 ${pid}
     fi
     ;;
    STOP|stop)
     #STOP1 (alt STOP) bewirkt hier nichts!
     #tail zu killen reicht, um die anderen auch zu entfernen :)
     pid=`ps | grep "tail*"| grep -v grep | awk '{print $1}'`
     if [ "X${pid}" != "X" ] ; then
        kill -9 ${pid}
     fi
     #fals noch ein 2. läuft :-)
     pid=`ps | grep "tail*"| grep -v grep | awk '{print $1}'`
     if [ "X${pid}" != "X" ] ; then
        kill -9 ${pid}
     fi  
     ;;
  SHOW|show)
  sleep 10     
  if [ -f $TMPRING ];     then
    eintragvorhanden=`grep "von" $TMPRING| head -n 1`
    echo "Es ist ein Eintrag in TMPRING vorhanden"
    if [ "$eintragvorhanden" = "" ]; then
      echo "kein Eintrag in TMPRING"
      rm $TMPRING
    else
      echo "</menu>" >>$TMPRING
      ebox exit; exit=`opexec --width=580 --height=400 --xmlpath="$TMPRING"`
      rm $TMPRING
      #sed -i '$D' $TMPRING #falls die Datei nicht gelöscht werden soll muss man die letzte Zeile in der Datei löschen
    fi 
  else
    echo "Die Datei TMPRING existiert nicht"  
  fi
  ;;
  
  TEST|test)
        loadFritzBook
      echo "Testing the script: "$0
      mysearch "$2"
      echo "Searching for phone number: "$2" resulted in: "$NAME
      #wget -O- http://$loginname:$passwort@$ip/xml/boxstatus > $TMPSTATUS
      #wget -O- http://$ip/query?boxstatus > $TMPSTATUS
      $CURL http://$ip/query?boxstatus > $TMPSTATUS
      #STANDBY=`grep "standby" $TMPSTATUS| head -n 1 | sed -e '{s/<standby>//g;s/<\/standby>//g}'`
      STANDBY=`grep "standby" $TMPSTATUS| wc -l`
      STANDBY=0
      echo "Standbystatus: $STANDBY"
      p="$NAME    "
      p20=`echo $p               | sed -e '{s/ /%20/g;s/Ä/%C3%84/g;s/Ö/%C3%96/g;s/Ü/%C3%9C/g;s/ä/%C3%A4/g;s/ö/%C3%B6/g;s/ü/%C3%BC/g;s/á/ss/g;s/&/%26/g;s/~n/%0a/g;s/#/%20/g;}'`
      t20=`echo $t               | sed -e '{s/ /%20/g;s/Ä/%C3%84/g;s/Ö/%C3%96/g;s/Ü/%C3%9C/g;s/ä/%C3%A4/g;s/ö/%C3%B6/g;s/ü/%C3%BC/g;s/á/ss/g;s/&/%26/g;s/~n/%0a/g;s/#/%20/g;}'`
      ADDRESSE20=`echo $ADDRESSE | sed -e '{s/ /%20/g;s/Ä/%C3%84/g;s/Ö/%C3%96/g;s/Ü/%C3%9C/g;s/ä/%C3%A4/g;s/ö/%C3%B6/g;s/ü/%C3%BC/g;s/á/ss/g;s/&/%26/g;s/~n/%0a/g;s/#/%20/g;}'`
      if [ $STANDBY = 0 ]; then
        if [ $debug = 0 ]; then
          if [ $muteRing = 1 ]; then #wenn Option gesetzt ist dann wird der Ton abgeschaltet
            #wget -O- http://$loginname:$passwort@127.0.0.1/cgi-bin/audio >> $TMPSTATUS #tonstatus wird ermittelt und in eine Datei geschrieben
            #mute=`grep "mute" $TMPSTATUS| head -n 1 | sed -e '{s/mute://g;s/<br>//g;s/ //g}'`
            #if [ $mute = 0 ]; then 
                #wget -q -O /dev/null "http://$ip/query?sendrc&rcmute"  >> $NULL #Befehl für Tonsperre
                $CURL "http://$ip/query?sendrc&rcmute"  >> $NULL #Befehl für Tonsperre
            #fi    
          fi
          #wget -q -O /dev/null "http://$ip/cgi-bin/xmessage?caption=Test%20des%20Scripts&body=Ergebnis:%0a$p20%0a${ADDRESSE20}&timeout=$anzeigetimeout"          >> $NULL
          $CURL "http://$ip/cgi-bin/xmessage?caption=Test%20des%20Scripts&body=Ergebnis:%0a$p20%0a${ADDRESSE20}&timeout=$anzeigetimeout"          >> $NULL
          (touch /tmp/xmessage; sleep $anzeigetimeout; rm  /tmp/xmessage) &
        fi
      else
        if [ $anzeigewennaus = 1 ]; then
          if [ $debug = 0 ]; then
           #wget -q -O /dev/null "http://$ip/cgi-bin/xmessage?caption=Test%20des%20Scripts&body=Ergebnis:%0a$p20%0a${ADDRESSE20}&timeout=$anzeigetimeout"          >> $NULL
           $CURL "http://$ip/cgi-bin/xmessage?caption=Test%20des%20Scripts&body=Ergebnis:%0a$p20%0a${ADDRESSE20}&timeout=$anzeigetimeout"          >> $NULL
          (touch /tmp/xmessage; sleep $anzeigetimeout; rm  /tmp/xmessage) & 
          fi  
        else
          echo "Keine Anzeige da die Box aus ist und -anzeigewennaus- 0 ist"
        fi
      fi
      if [ -f $TMPFILE ];     then  rm $TMPFILE; fi
      if [ -f $TMPSTATUS ];     then  rm $TMPSTATUS; fi
  ;; 
  loadPhoneBook|loadphonebook)
    if [ -f $FRITZBUCH ]; then
        rm $FRITZBUCH
    fi
    loadFritzBook
    if [ -f $FRITZBUCH ]; then
        cat $FRITZBUCH
        $CURL "http://$ip/cgi-bin/xmessage?caption=FritzBook%20geladen&body=FritzBox!%20Telefonbuch%20geladen&timeout=5" -o /dev/null
        (touch /tmp/xmessage; sleep 5; rm  /tmp/xmessage) &
      else
        $CURL "http://$ip/cgi-bin/xmessage?caption=Fehler%20beim%20laden&body=FritzBox!%20Telefonbuch%20konnte%20nicht%20geladen%20werden&timeout=5" -o /dev/null
        (touch /tmp/xmessage; sleep 5; rm  /tmp/xmessage) &
    fi
  ;;
esac
rm /tmp/callrun
