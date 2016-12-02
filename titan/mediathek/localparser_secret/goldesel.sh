#!/bin/sh
# first sh box parser for titannit mfg obi

. /tmp/localhoster/hoster.sh

SRC=$1
INPUT=$2
PAGE=$3

FILENAME=`echo $PAGE | tr '/' '.'`
FILENAME=`echo $FILENAME | tr '&' '.'`

if [ -z "$PAGE" ]; then
	FILENAME=none
fi

URL=http://goldesel.to
PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME=Goldesel

mkdir $TMP > /dev/null 2>&1

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
	echo "$NAME ($TYPE)#$SRC $SRC mainmenu#http://atemio.dyndns.tv/mediathek/menu/$PARSER.jpg#$PARSER.jpg#TiThek#0"
}

mainmenu()
{
	echo "Category#$SRC $SRC category#http://atemio.dyndns.tv/mediathek/menu/category.jpg#category.jpg#$NAME#0" > $TMP/$PARSER.$INPUT.list
	echo "$TMP/$PARSER.$INPUT.list"
}

category()
{
    echo "Kinofilme#$SRC $SRC new '/'#http://atemio.dyndns.tv/mediathek/menu/all-newfirst.jpg#all-newfirst.jpg#$NAME#0" > $TMP/$PARSER.$INPUT.list
    echo "Filme#$SRC $SRC page 'filme'#http://atemio.dyndns.tv/mediathek/menu/Movies.jpg#Movies.jpg#$NAME#0" >> $TMP/$PARSER.$INPUT.list
	echo "$TMP/$PARSER.$INPUT.list"
}

#
#  <form id="challenge-form" action="/cdn-cgi/l/chk_jschl" method="get">
#    <input type="hidden" name="jschl_vc" value="528818834f8ad23629ea5d31c3a5390c"/>
#    <input type="hidden" name="pass" value="1479475035.975-uJ99NUZ87d"/>
#    <input type="hidden" id="jschl-answer" name="jschl_answer"/>
#  </form>
#  
#http://goldesel.to/cdn-cgi/l/chk_jschl?jschl_vc=f29b5aecef651bc1df9cad068c58848c&pass=1479474954.758-6i1RP0vPFz&jschl_answer=2914582
#

new()
{
	if [ -e "$TMP/$PARSER.$INPUT.list" ] ; then
		rm $TMP/$PARSER.$INPUT.list
	fi

#	$curlbin $URL/$PAGE -o $TMP/cache.$PARSER.$INPUT.0

#	DATA=`cat $TMP/cache.$PARSER.$INPUT.0 | sed 's/ \+/ /g' | sed 's/<form id=/\nfound=/g' | grep ^found=`

#	actionpath=`cat $TMP/cache.$PARSER.$INPUT.0 | tr '\n' ' ' | sed 's/ \+/ /g' | sed 's/<form id=/\nfound=/g' | grep ^found= | sed 's/action=/\naction=/g' | grep ^action= | cut -d'"' -f2`
#	jschlvc=`cat $TMP/cache.$PARSER.$INPUT.0 | tr '\n' ' ' | sed 's/ \+/ /g' | sed 's/<form id=/\nfound=/g' | grep ^found= | sed 's/"jschl_vc"/\njschl_vc=/g' | grep ^jschl_vc= | cut -d'"' -f2`
#	pass=`cat $TMP/cache.$PARSER.$INPUT.0 | tr '\n' ' ' | sed 's/ \+/ /g' | sed 's/<form id=/\nfound=/g' | grep ^found= | sed 's/"pass"/\npass=/g' | grep ^pass= | cut -d'"' -f2`
#	jschlanswer=2914582
	#`cat $TMP/cache.$PARSER.$INPUT.0 | tr '\n' ' ' | sed 's/ \+/ /g' | sed 's/<form id=/\nfound=/g' | grep ^found= | sed 's/"jschl-answer"/\njschl-answer=/g' | grep ^jschl-answer= | cut -d'"' -f2`

#echo actionpath $actionpath
#echo jschlvc $jschlvc
#echo pass $pass
#echo jschlanswer $jschlanswer

#echo 222 "$URL"/"$actionpath"?jschl_vc="$jschlvc"&pass="$pass"&jschl_answer="$jschlanswer"

#	$curlbin "$URL/$actionpath?jschl_vc=$jschlvc&pass=$pass&jschl_answer=$jschlanswer" -o $TMP/cache.$PARSER.$INPUT.00
#echo $TMP/cache.$PARSER.$INPUT.00
#exit

#	$curlbin $URL/$PAGE -o $TMP/cache.$PARSER.$INPUT.1

	/tmp/localhoster/hoster.sh get $URL/$PAGE > $TMP/cache.$PARSER.$INPUT.1

	piccount=0

	cat $TMP/cache.$PARSER.$INPUT.1 | sed 's/<div class/\n<div class/g' | sed 's/<a href="/\n<a href="/g' | grep ^'<a href="film' | grep title= | sed 's/ /~/g' >$TMP/cache.$PARSER.$INPUT.2

	while read -u 3 ROUND; do
		TITLE=`echo $ROUND | sed 's/title=/\ntitle=/' | grep ^"title=" | cut -d '"' -f2 | tr '~' ' ' | sed 's/#/%/'`
		TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' # ' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
		PIC=$URL/`echo $ROUND | sed s'!<img~src=!\nsrc=!' | grep ^"src=" | cut -d '"' -f2 | tr '~' ' '`
		NEWPAGE=`echo $ROUND | sed 's/<a~href=/\nhref=/' | grep ^"href=" | cut -d '"' -f2`
	
		if [ -z "$PIC" ] || [ "$PIC" = "$URL/" ]; then  
			PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
			TMPPIC="default.jpg"
		else
			TMPPIC=goldesel_`echo $PIC | cut -d '/' -f6`
		fi

		if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ];then
			if [ `cat $TMP/$PARSER.$INPUT.list | grep ^"$NEWPAGE" | wc -l` -eq 0 ];then
				if [ ! -e $TMP/$PARSER.$INPUT.list ];then
					touch $TMP/$PARSER.$INPUT.list
				fi
				LINE="$TITLE#$SRC $SRC hosterlist $NEWPAGE#$PIC#$TMPPIC#$NAME#0"
				echo "$LINE" >> $TMP/$PARSER.$INPUT.list
        $curlbin $PIC -o /tmp/tithek/$TMPPIC -A 'Mozilla/5.0 (Windows NT 6.3; rv:36.0) Gecko/20100101 Firefox/36.0'
			fi
		fi
	done 3<$TMP/cache.$PARSER.$INPUT.2
#	rm $TMP/cache.* > /dev/null 2>&1

	echo "$TMP/$PARSER.$INPUT.list"
}

page()
{
	if [ -e "$TMP/$PARSER.$INPUT.list" ] ; then
		rm $TMP/$PARSER.$INPUT.list
	fi
#	$curlbin $URL/$PAGE -o $TMP/cache.$PARSER.$INPUT.1
	/tmp/localhoster/hoster.sh get $URL/$PAGE > $TMP/cache.$PARSER.$INPUT.1

	cat $TMP/cache.$PARSER.$INPUT.1 | sed 's/<p><ul class="rls_table/\n<p><ul class=rls_table/g' | sed 's/<\/ul><\/u>/<\/ul><\/u>\n/g' | grep ^'<p><ul class=rls_table' | sed 's/<a href="/\n<a href="/g' | sed 's/<li step=/\n<li step=/g' | grep ^'<a href=' | grep 'tba">VID' | sed 's/ /~/g' >$TMP/cache.$PARSER.$INPUT.2

	while read -u 3 ROUND; do
		TITLE=`echo $ROUND |  sed -e 's/<div~class="tle">/\n<div~class="tle""/' -e 's/<b/"b/g' | grep ^'<div~class="tle""' | cut -d '"' -f4 | tr '~' ' ' | sed 's/#/%/'`
		TITLE=`echo $TITLE | sed -e 's/&#038;/&/g' -e 's/&amp;/und/g' -e 's/&quot;/"/g' -e 's/&lt;/\</g' -e 's/&#034;/\"/g' -e 's/&#039;/\"/g' # ' -e 's/#034;/\"/g' -e 's/#039;/\"/g' -e 's/&szlig;/Ãx/g' -e 's/&ndash;/-/g' -e 's/&Auml;/Ã/g' -e 's/&Uuml;/ÃS/g' -e 's/&Ouml;/Ã/g' -e 's/&auml;/Ã¤/g' -e 's/&uuml;/Ã¼/g' -e 's/&ouml;/Ã¶/g' -e 's/&eacute;/Ã©/g' -e 's/&egrave;/Ã¨/g' -e 's/%F6/Ã¶/g' -e 's/%FC/Ã¼/g' -e 's/%E4/Ã¤/g' -e 's/%26/&/g' -e 's/%C4/Ã/g' -e 's/%D6/Ã/g' -e 's/%DC/ÃS/g' -e 's/|/ /g' -e 's/(/ /g' -e 's/)/ /g' -e 's/+/ /g' -e 's/\//-/g' -e 's/,/ /g' -e 's/;/ /g' -e 's/:/ /g' -e 's/\.\+/./g'`
		PIC=$URL/`echo $ROUND | sed s'/data-original=/\ndata-original=/' | grep ^'data-original=' | cut -d '"' -f2 | sed 's/\/mini\//\/poster\//g'`
		NEWPAGE=`echo $ROUND | cut -d '"' -f2`
	
		if [ "$PIC" == "$URL/" ] ; then
			PIC="http://atemio.dyndns.tv/mediathek/menu/default.jpg"
			TMPPIC="default.jpg"
		else
			TMPPIC=goldesel_`echo $PIC | cut -d '/' -f6`
		fi

		if [ ! -z "$TITLE" ] && [ ! -z "$NEWPAGE" ] ; then
			LINE="$TITLE#$SRC $SRC hosterlist $NEWPAGE#$PIC#$TMPPIC#$NAME#0"
			echo "$LINE" >> $TMP/$PARSER.$INPUT.list
      $curlbin $PIC -o /tmp/tithek/$TMPPIC -A 'Mozilla/5.0 (Windows NT 6.3; rv:36.0) Gecko/20100101 Firefox/36.0'
		fi
	done 3<$TMP/cache.$PARSER.$INPUT.2
	rm $TMP/cache.$PARSER.$INPUT.* > /dev/null 2>&1
	echo "$TMP/$PARSER.$INPUT.list"
}

hosterlist()
{
	if [ -e "$TMP/$PARSER.$INPUT.list" ] ; then
		rm $TMP/$PARSER.$INPUT.list
	fi
	$curlbin $URL/$PAGE -o $TMP/cache.$PARSER.$INPUT.1 -A 'Mozilla/5.0 (Windows NT 6.3; rv:36.0) Gecko/20100101 Firefox/36.0'
#	/tmp/localhoster/hoster.sh get $URL/$PAGE > $TMP/cache.$PARSER.$INPUT.1

	cat $TMP/cache.$PARSER.$INPUT.1 | sed 's!<h2>Stream-Links</h2>!\nfound=!g' | sed 's!<h2>Sample-Links</h2>!\nerror=!g' | grep ^found= | sed 's/<li data=/\ndata=/g' | grep ^data= >$TMP/cache.$PARSER.$INPUT.2

	while read -u 3 ROUND; do
		NEWPAGE="data=`echo $ROUND | cut -d '"' -f2 | sed 's/;/%3B/g'`"
		TITLE=`echo $ROUND | cut -d ';' -f5 | cut -d '.' -f1`
		if [ ! -z "$TITLE" ] && [ "$TITLE" != " " ] && [ ! -z "$NEWPAGE" ];then
			PIC=`echo $TITLE | tr [A-Z] [a-z]`
			LINE="$TITLE#$SRC $SRC hoster $NEWPAGE '--referer $URL/$PAGE'#http://atemio.dyndns.tv/mediathek/menu/$PIC.jpg#$PIC.jpg#$NAME#111"
			echo "$LINE" >> $TMP/$PARSER.$INPUT.list
		fi
	done 3<$TMP/cache.$PARSER.$INPUT.2
	rm $TMP/cache.$PARSER.$INPUT.* > /dev/null 2>&1
	echo "$TMP/$PARSER.$INPUT.list"
}

hoster()
{
	rm $TMP/cache.$PARSER.$INPUT.* > /dev/null 2>&1
	$curlbin $URL/res/links -X POST --data "$PAGE" -o $TMP/cache.$PARSER.$INPUT.1 -A 'Mozilla/5.0 (Windows NT 6.3; rv:36.0) Gecko/20100101 Firefox/36.0'
	STREAMURL=`cat $TMP/cache.$PARSER.$INPUT.1 | sed 's!url="http://dref.me/?!\nstreamurl="!' | grep ^streamurl= | cut -d'"' -f2 | sed 's!%3A!:!g' | sed 's!%2F!/!g'`
#	rm $TMP/cache.$PARSER.$INPUT.* > /dev/null 2>&1
	echo $STREAMURL
}

case $INPUT in
	init) $INPUT;;
	mainmenu) $INPUT;;
	category) $INPUT;;
	new) $INPUT;;
	page) $INPUT;;
	hosterlist) $INPUT;;
	hoster) $INPUT;;
esac
