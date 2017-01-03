#!/bin/sh
# first sh box hoster for titannit mfg obi

TYPE=$1
INPUT=$2

INPUT=`echo $INPUT | sed 's!/Out/?s=!!g'`

ARCH=`cat /etc/.arch`
BOX=`cat /etc/model`
debuglevel=`cat /mnt/config/titan.cfg | grep debuglevel | cut -d"=" -f2`
curlbin="curl -k -s -L --cookie /mnt/network/cookies --cookie-jar /mnt/network/cookies -A 'Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Maxthon/4.4.7.3000 Chrome/30.0.1599.101 Safari/537.36'"
curlbin2='curl -k -s --cookie /mnt/network/cookies --cookie-jar /mnt/network/cookies'
if [ "$debuglevel" == "99" ]; then curlbin="$curlbin -v"; fi
if [ "$debuglevel" == "99" ]; then curlbin2="$curlbin2 -v"; fi
wgetbin="wget -q -T2"
TMP=/tmp/localcache
CMD=/tmp/localhoster

BIN="$CMD"/bin/python."$ARCH"
ln -fs /tmp/localhoster/lib/python2.7/lib-dynload."$ARCH" /tmp/localhoster/lib/python2.7/lib-dynload
ln -fs /tmp/localhoster/lib/libpython2.7.so.1.0."$ARCH" /tmp/localhoster/lib/libpython2.7.so.1.0
export PYTHONHOME=/tmp/localhoster
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/tmp/localhoster/lib

if [ -e "$TMP/hoster.tar" ]; then rm -f $TMP/hoster.tar; fi
if [ -e "$TMP/parser.tar" ]; then rm -f $TMP/parser.tar; fi

if [ "$ARCH" == "sh4" ] && [ "$BOX" != "ufs912" ]; then
	if [ -e "$CMD/lib/python2.7/lib-dynload.mipsel" ]; then rm -rf $CMD/lib/python2.7/lib-dynload.mipsel; fi
	if [ -e "$CMD/lib/libpython2.7.so.1.0.mipsel" ]; then rm -rf $CMD/lib/libpython2.7.so.1.0.mipsel; fi
fi

hoster=`echo $INPUT | tr 'A-Z' 'a-z' | sed 's!http://!!' | sed 's!https://!!' | cut -d'/' -f1 | sed 's/www.//' | tr '.' '\n' | head -n1`

debuglevel=`cat /mnt/config/titan.cfg | grep debuglevel | cut -d"=" -f2`
curlbin='curl -k -s -L --cookie /mnt/network/cookies --cookie-jar /mnt/network/cookies'
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


if [ "$TYPE" == "get" ];then
	case $hoster in
		ecostream) ecostream $INPUT;;
		giga) giga $INPUT;;
		vidup) vidup $INPUT;;
		nosvideo) nosvideo $INPUT;;
		allmyvideos) allmyvideos $INPUT;;
		flashx) flashx $INPUT;;
		openload) openload $INPUT;;
		briskfile) briskfile $INPUT;;
		bitvid) videoweed $INPUT;;
		videoweed) videoweed $INPUT;;
		vodlocker) vodlocker $INPUT;;
		vodzi) vodzi $INPUT;;
		youwatch) youwatch $INPUT;;
		chouhaa) youwatch $INPUT;;
		thevideo) thevideo $INPUT;;
		movshare) movshare $INPUT;;
		wholecloud) movshare $INPUT;;
		vidgg) movshare $INPUT;;
		vidzi) vidzi $INPUT;;
		vivo) vivo $INPUT;;
		vidto) vidto $INPUT;;
		goldesel) cloudflare $INPUT;;
		movie4k) cloudflare $INPUT;;
		streamcloud1) streamcloud $INPUT;;	
		nowvideo) nowvideo $INPUT;;
		divxstage) divxstage $INPUT;;
		cloudtime) divxstage $INPUT;;
	esac
fi