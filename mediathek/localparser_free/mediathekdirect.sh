#!/bin/sh
# first sh box parser for titannit mfg obi

. /tmp/localhoster/hoster.sh

SRC=$1
INPUT=$2
PAGE=$3
NEXT=$4

PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`

FILENAME="`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'` $INPUT $PAGE $NEXT"
FILENAME="`echo $FILENAME | sed -e 's/\&\+/./g' -e 's#\/\+#.#g' -e 's/\?\+/./g' -e 's/;\+/./g' -e 's/=\+/./g' -e 's/ \+/./g' -e 's/\.\+/./g'`"
PICNAME=`echo $FILENAME`

if [ -z "$FILENAME" ]; then
	FILENAME=none
fi

URL=http://www.mediathekdirekt.de
#PARSER=`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'`
NAME=MediathekDirekt

#rm -rf $TMP > /dev/null 2>&1
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
	rm -f /mnt/network/cookies > /dev/null 2>&1
	rm -rf $TMP > /dev/null 2>&1
	echo "$NAME ($TYPE)#$SRC $SRC videos 'good.json'#http://atemio.dyndns.tv/mediathek/menu/$PARSER.jpg#$PARSER.jpg#TiThek#0"
}

videos()
{
	if [ ! -e "$TMP/$FILENAME.list" ]; then
		$curlbin -o - $URL/$PAGE | tr '[' '\n' | awk -v SRC=$SRC -v NAME=$NAME -v PICNAME=$PICNAME \
		'
			# BEGIN variable setzen
			BEGIN
			{
				# setzt suchvariable auf 0 vor dem start
			}
			# eindeutige zeile vor ersten treffer
			/^\"/ \
			{
				# print "111111" $0
				gsub(/\\"/, "\x27", $0, $0)

				# print "222222" $0

				split($0, a, "\"")
				# print "a1" a[1]
				# print "a2" a[2]
				# print "a3" a[3]
				# print "a4" a[4]
				# print "a5" a[5]
				# print "a6" a[6]
				# print "a7" a[7]
				# print "a8" a[8]
				# print "a9" a[9]
				# print "a10" a[10]
				# print "a11" a[11]
				# print "a12" a[12]
				# print "a13" a[13]
				# print "a14" a[14]
				# print "a15" a[15]
				
            	pic = "http://atemio.dyndns.tv/mediathek/menu/default.jpg"

				sender = a[2]
				title = a[4]
				date = a[8]
				duration = a[10]
				stream = a[14]
				print sender " - " title " (" date ") (" duration ")#" stream "#" pic "#" PICNAME ".jpg#" NAME "#2"

				next
			}
		# schreibe alles in die list datei
		' >$TMP/$FILENAME.list
	fi
	# gebe titan den list namen mit pfad zurueck
	echo "$TMP/$FILENAME.list"
}

case $INPUT in
	init) $INPUT;;
	videos) $INPUT;;
esac
