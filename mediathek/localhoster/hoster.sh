#!/bin/sh
# first sh box hoster for titannit mfg obi

TYPE=$1
INPUT=$2
DEST=$3

INPUT=`echo $INPUT | sed 's!/Out/?s=!!g'`

ARCH=`cat /etc/.arch`
BOX=`cat /etc/model`
TMP=/tmp/localcache
CMD=/tmp/localhoster
#USERAGENT='Mozilla/5.0 (Windows NT 6.3; rv:36.0) Gecko/20100101 Firefox/36.0'
USERAGENT='Mozilla%2F5.0+%28Windows+NT+6.3%3B+rv%3A36.0%29+Gecko%2F20100101+Firefox%2F36.0'
debuglevel=`cat /mnt/config/titan.cfg | grep debuglevel | cut -d"=" -f2`
curlbin="curl -k -s -L --cookie /mnt/network/cookies --cookie-jar /mnt/network/cookies -A '$USERAGENT'"
curlbin2="curl -k -s --cookie /mnt/network/cookies --cookie-jar /mnt/network/cookies -A '$USERAGENT'"
youtubebin="$CMD/lib/youtube_dl/__main__.py --no-check-certificate --cookies /mnt/network/cookies --user-agent '$USERAGENT' --format mp4 --restrict-filenames --ignore-errors -g"
youtubebinbg="$CMD/lib/youtube_dl/__main__.py --no-check-certificate --cookies /mnt/network/cookies --user-agent '$USERAGENT' --format mp4 --restrict-filenames --ignore-errors --output"
export PYTHONHOME=/tmp/localhoster
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/tmp/localhoster/lib

if [ "$debuglevel" == "99" ]; then curlbin="$curlbin -v"; fi
if [ "$debuglevel" == "99" ]; then curlbin2="$curlbin2 -v"; fi
#if [ "$debuglevel" == "99" ]; then youtubebin="$youtubebin --verbose"; fi

wgetbin="wget -q -T2"

BIN="$CMD"/bin/python."$ARCH"

if [ ! -e "/tmp/localhoster/lib/python2.7/lib-dynload" ] && [ -e /tmp/localhoster/lib/python2.7/lib-dynload."$ARCH" ];then
#	ln -fs /tmp/localhoster/lib/python2.7/lib-dynload."$ARCH" /tmp/localhoster/lib/python2.7/lib-dynload
	mv -f /tmp/localhoster/lib/python2.7/lib-dynload."$ARCH" /tmp/localhoster/lib/python2.7/lib-dynload
	rm -rf /tmp/localhoster/lib/python2.7/lib-dynload.*
fi
if [ ! -e "/tmp/localhoster/lib/libpython2.7.so.1.0" ] && [ -e /tmp/localhoster/lib/libpython2.7.so.1.0."$ARCH" ];then
#	ln -fs /tmp/localhoster/lib/libpython2.7.so.1.0."$ARCH" /tmp/localhoster/lib/libpython2.7.so.1.0
	mv -f /tmp/localhoster/lib/libpython2.7.so.1.0."$ARCH" /tmp/localhoster/lib/libpython2.7.so.1.0
	rm -f /tmp/localhoster/lib/libpython2.7.so.1.0.*
fi
if [ ! -e "/tmp/localhoster/lib/libcrypto.so.1.0.0" ] && [ -e /tmp/localhoster/lib/libcrypto.so.1.0.0."$ARCH" ];then
#	ln -fs /tmp/localhoster/lib/libcrypto.so.1.0.0."$ARCH" /tmp/localhoster/lib/libcrypto.so.1.0.0
	mv -f /tmp/localhoster/lib/libcrypto.so.1.0.0."$ARCH" /tmp/localhoster/lib/libcrypto.so.1.0.0
	rm -f /tmp/localhoster/lib/libcrypto.so.1.0.0.*
fi
if [ ! -e "/tmp/localhoster/lib/libssl.so.1.0.0" ] && [ -e /tmp/localhoster/lib/libssl.so.1.0.0."$ARCH" ];then
#	ln -fs /tmp/localhoster/lib/libssl.so.1.0.0."$ARCH" /tmp/localhoster/lib/llibssl.so.1.0.0
	mv -f /tmp/localhoster/lib/libssl.so.1.0.0."$ARCH" /tmp/localhoster/lib/libssl.so.1.0.0
	rm -f /tmp/localhoster/lib/libssl.so.1.0.0.*
fi

if [ -e "$TMP/hoster.tar" ]; then rm -f $TMP/hoster.tar; fi
if [ -e "$TMP/parser.tar" ]; then rm -f $TMP/parser.tar; fi

if [ "$ARCH" == "sh4" ] && [ "$BOX" != "ufs912" ]; then
	if [ -e "$CMD/lib/python2.7/lib-dynload.mipsel" ]; then rm -rf $CMD/lib/python2.7/lib-dynload.mipsel; fi
	if [ -e "$CMD/lib/python2.7/lib-dynload.arm" ]; then rm -rf $CMD/lib/python2.7/lib-dynload.arm; fi

	if [ -e "$CMD/lib/libpython2.7.so.1.0.mipsel" ]; then rm -rf $CMD/lib/libpython2.7.so.1.0.mipsel; fi
	if [ -e "$CMD/lib/libpython2.7.so.1.0.arm" ]; then rm -rf $CMD/lib/libpython2.7.so.1.0.arm; fi

	if [ -e "$CMD/lib/libssl.so.1.0.0.mipsel" ]; then rm -rf $CMD/lib/libssl.so.1.0.0.mipsel; fi
	if [ -e "$CMD/lib/libssl.so.1.0.0.arm" ]; then rm -rf $CMD/lib/libssl.so.1.0.0.arm; fi

	if [ -e "$CMD/lib/libcrypto.so.1.0.0.mipsel" ]; then rm -rf $CMD/lib/libcrypto.so.1.0.0.mipsel; fi
	if [ -e "$CMD/lib/libcrypto.so.1.0.0.arm" ]; then rm -rf $CMD/lib/libcrypto.so.1.0.0.arm; fi
fi

hostercheck=`echo $INPUT | tr 'A-Z' 'a-z' | sed 's!://!\n!' | cut -d'/' -f1 | tail -n1 | tr '.' '\n' | wc -l`
hosterline=`expr $hostercheck - 1`
hoster=`echo $INPUT | tr 'A-Z' 'a-z' | sed 's!://!\n!' | cut -d'/' -f1 | tail -n1 | cut -d"." -f$hosterline`
#echo $hoster

debuglevel=`cat /mnt/config/titan.cfg | grep debuglevel | cut -d"=" -f2`
if [ "$debuglevel" == "99" ]; then curlbin="$curlbin -v"; fi

ecostream()
{
	hosterurl=http://www.ecostream.tv
	data="tpm=`$curlbin $INPUT | grep footerhash | cut -d"'" -f2``$curlbin $INPUT | grep superslots | cut -d"'" -f2`&id=`$curlbin $INPUT | grep data-id | cut -d'"' -f4`"
	streampage=`$curlbin $hosterurl/js/ecoss.js | grep "$.post('" | grep videos | cut -d"'" -f2`
	streamurl=$hosterurl`$curlbin -H "X-Requested-With: XMLHttpRequest" -X POST --data "$data" --referer $INPUT $hosterurl/$streampage | cut -d'"' -f6`
	if [ "`echo $streamurl | grep DOCTYPE | wc -l`" -eq 0 ] && [ "$streamurl" != "$hosterurl" ];then
		echo $streamurl
	fi
}

giga()
{
	streampage=`$curlbin $INPUT | grep "<iframe src=" | cut -d'"' -f2 | head -n1`
	streamurl=`$curlbin $streampage | grep '{file:' | cut -d'"' -f2 | grep 1080p`
	if [ -z "$streamurl" ];then
		streamurl=`$curlbin $streampage | grep '{file:' | cut -d'"' -f2 | grep 720p`
	fi
	if [ -z "$streamurl" ];then
		streamurl=`$curlbin $streampage | grep '{file:' | cut -d'"' -f2 | grep 480p`
	fi
	if [ -z "$streamurl" ];then
		streamurl=`$curlbin $streampage | grep '{file:' | cut -d'"' -f2 | grep 360p`
	fi
	if [ -z "$streamurl" ];then
		streamurl=`$curlbin $streampage | grep '{file:' | cut -d'"' -f2 | grep 240p`
	fi
	if [ -z "$streamurl" ];then
		streamurl=`$curlbin $streampage | grep '{file:' | cut -d'"' -f2 | grep 180p`
	fi
	echo $streamurl
}

vidup()
{
	$BIN $CMD/vidup.py $INPUT
}

nosvideo()
{
	$BIN $CMD/nosvideo.py $INPUT
}

allmyvideos()
{
	$BIN $CMD/allmyvideos.py $INPUT
}

flashx()
{
	$BIN $CMD/flashx.py $INPUT
}

openload()
{
	$BIN $CMD/openload.py $INPUT
}

briskfile()
{
	$BIN $CMD/briskfile.py $INPUT
}

vodlocker()
{
	$BIN $CMD/vodlocker.py $INPUT
}

videoweed()
{
	$BIN $CMD/videoweed.py $INPUT
}

vodzi()
{
	$BIN $CMD/vodzi.py $INPUT
}

youwatch()
{
	$BIN $CMD/youwatch.py $INPUT
}

thevideo()
{
	$BIN $CMD/thevideo.py $INPUT
}

movshare()
{
	$BIN $CMD/movshare.py $INPUT
}

streamcloud()
{
	$BIN $CMD/streamcloud.py $INPUT
}

vidzi()
{
	$BIN $CMD/vidzi.py $INPUT
}

vivo()
{
	$BIN $CMD/vivo.py $INPUT
}

vidto()
{
	$BIN $CMD/vidto.py $INPUT
}

goldesel()
{
	$BIN $CMD/goldesel.py $INPUT
}

cloudflare()
{
	$BIN $CMD/cloudflare.py $INPUT
}

nowvideo()
{
	$BIN $CMD/nowvideo.py $INPUT
}

divxstage()
{
	$BIN $CMD/divxstage.py $INPUT
}

novamov()
{
	$BIN $CMD/novamov.py $INPUT
}

xvidstage()
{
	$BIN $CMD/xvidstage.py $INPUT
}

waaw()
{
	$BIN $CMD/waaw.py $INPUT
}

streamango()
{
	$BIN $CMD/streamango.py $INPUT
}

vidlox()
{
	$BIN $CMD/vidlox.py $INPUT
}

aliez()
{
	#http://emb.aliez.me/player/live.php?id=56180&w=700&h=480"
	URL=`$curlbin "$INPUT" | sed 's/source:/\nsource:/' | grep ^source: | cut -d"'" -f2`
	REFERER=`echo "$INPUT" | sed -e 's/=/3D/g' -e 's/&/26/g'`
	echo "$URL|Referer=$REFERER&User-Agent=$USERAGENT"
}

directstream()
{
	echo "$INPUT"
#	rm -f /tmp/_last_hoster_* > /dev/null 2>&1
	echo  "$INPUT" > /tmp/.last_hoster_$hoster.log
}

youtube_dl()
{
#	echo "$BIN $youtubebin $INPUT" > /tmp/.last_hoster_youtube_dl.log
#	$BIN $CMD/lib/youtube_dl/__main__.py --no-check-certificate --cookies /mnt/network/cookies --user-agent "$USERAGENT" --format mp4 --restrict-filenames --ignore-errors -g "$INPUT" > /tmp/youtube_dl.streamlink.log 2>&1
#	cat /tmp/youtube_dl.streamlink.log | tail -n1
	$BIN $youtubebin "$INPUT"
}

youtube_dlbg()
{
#	echo "$BIN $youtubebinbg $DEST $INPUT" > /tmp/.last_hoster_youtube_dlbg.log
#	URL=`$BIN $CMD/lib/youtube_dl/__main__.py --no-check-certificate --cookies /mnt/network/cookies --user-agent "$USERAGENT" --format mp4 --restrict-filenames --ignore-errors --output "$1" "$2"`
#	echo "$URL" >> /tmp/.last_hoster_youtube_dlbg.log
#	echo $URL
	$BIN $youtubebinbg $DEST $INPUT
}

if [ "$TYPE" == "get" ];then
	echo  "$INPUT" > /tmp/.last_hoster_$TYPE_$hoster.log
	case $hoster in
		ecostream) ecostream $INPUT;;
		giga) giga $INPUT;;
		nosvideo) nosvideo $INPUT;;
		allmyvideos) allmyvideos $INPUT;;
		flashx) flashx $INPUT;;
		openload|oload) openload $INPUT;;
		briskfile) briskfile $INPUT;;
		videoweed|bitvid) videoweed $INPUT;;
		vodlocker) vodlocker $INPUT;;
		youwatch|chouhaa|ay8ou8ohth) youwatch $INPUT;;
		thevideo) thevideo $INPUT;;
		movshare|wholecloud|vidgg) movshare $INPUT;;
		vidto) vidto $INPUT;;
		vidup) vidup $INPUT;;
		vidzi) vidzi $INPUT;;
		vivo) vivo $INPUT;;
		goldesel|movie4k) cloudflare $INPUT;;
		streamcloud1) streamcloud $INPUT;;	
		nowvideo) nowvideo $INPUT;;
		divxstage|cloudtime) divxstage $INPUT;;
		novamov|auroravid) novamov $INPUT;;
		xvidstage) xvidstage $INPUT;;
		waaw|netu|hqq) waaw $INPUT;;
		streamango|streamcherry) streamango $INPUT;;
                vidlox) vidlox $INPUT;;
		redirector|googlevideo|vodcloud|google) directstream "$INPUT";;
		aliez) aliez $INPUT;;
	esac
fi

if [ "$TYPE" == "youtube_dl" ];then
	echo  "$INPUT" > /tmp/.last_hoster_$TYPE_$hoster.log
	case $hoster in
		*) youtube_dl $INPUT;;
	esac
fi

if [ "$TYPE" == "youtube_dlbg" ];then
	echo  "$INPUT" > /tmp/.last_hoster_$TYPE_$hoster.log
	case $hoster in
		*) youtube_dlbg $DEST $INPUT;;
	esac
fi

