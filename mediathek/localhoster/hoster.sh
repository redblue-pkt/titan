#!/bin/sh
# first sh box hoster for titannit mfg obi

TYPE=$1
INPUT=$2
DEST=$3
USER=$DEST
PASS=$4

INPUT=`echo $INPUT | sed 's!/Out/?s=!!g'`

#FILENAME="`echo $SRC | tr '/' '\n' | tail -n1 | sed 's/.sh//'` $INPUT $PAGE $NEXT"
FILENAME="`echo $INPUT | sed -e 's/\-\+/./g' | sed -e 's/\+\+/./g' | sed -e 's/\&\+/./g' -e 's#\/\+#.#g' -e 's/\?\+/./g' -e 's/:\+/./g' -e 's/;\+/./g' -e 's/=\+/./g' -e 's/ \+/./g' -e 's/\.\+/./g'`"
PICNAME=`echo $FILENAME`

if [ -z "$FILENAME" ]; then
	FILENAME=none
fi


ARCH=`cat /etc/.arch`
BOX=`cat /etc/model`
TMP=/tmp/localcache
CMD=/tmp/localhoster
BIN="$CMD"/bin/python."$ARCH"
HLSBIN="$CMD"/bin/hlsdl."$ARCH"
CURLBIN="$CMD"/bin/curl."$ARCH"
if [ ! -e "$CURLBIN" ];then CURLBIN=curl; fi
#USERAGENT='Mozilla/5.0 (Windows NT 6.3; rv:36.0) Gecko/20100101 Firefox/36.0'
USERAGENT='Mozilla%2F5.0+%28Windows+NT+6.3%3B+rv%3A36.0%29+Gecko%2F20100101+Firefox%2F36.0'
debuglevel=`cat /mnt/config/titan.cfg | grep debuglevel | cut -d"=" -f2`
curlbin="$CURLBIN -k -s -L --cookie /mnt/network/cookies --cookie-jar /mnt/network/cookies -A $USERAGENT"
curlbin2="$CURLBIN -k -s --cookie /mnt/network/cookies --cookie-jar /mnt/network/cookies -A $USERAGENT"
youtubebin="$CMD/lib/youtube_dl/__main__.py --no-check-certificate --cookies /mnt/network/cookies --user-agent $USERAGENT --format mp4 --restrict-filenames --ignore-errors -g"
youtubebinbg="$CMD/lib/youtube_dl/__main__.py --no-check-certificate --cookies /mnt/network/cookies --user-agent $USERAGENT --format mp4 --restrict-filenames --ignore-errors --output"
hlsdlbg="$HLSBIN -u $USERAGENT -o"

export PYTHONHOME=/tmp/localhoster
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/tmp/localhoster/lib


if [ "$debuglevel" == "99" ]; then curlbin="$curlbin -v"; fi
if [ "$debuglevel" == "99" ]; then curlbin2="$curlbin2 -v"; fi
if [ "$debuglevel" == "99" ]; then youtubebin="$youtubebin --verbose"; fi

wgetbin="wget -q -T2"

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

vodlockerold()
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

vodlocker()
{
	rm -f $TMP/cache.hoster.$hoster.* > /dev/null 2>&1

	STREAMLIST="$TMP/$TYPE.$hoster.$FILENAME.streamlist"
	if [ -e "$STREAMLIST" ];then
		rm -f $STREAMLIST > /dev/null 2>&1
	fi

	$curlbin "$INPUT" -o $TMP/cache.hoster.$hoster.1
	cat $TMP/cache.hoster.$hoster.1 | sed 's/<source src=/\nfound=/g' | grep ^found= | cut -d"'" -f2 >$TMP/cache.hoster.$hoster.1.url1
	URL=`cat $TMP/cache.hoster.$hoster.1.url1`

	if [ ! -z "$URL" ];then
		echo "$URL" >> $STREAMLIST
	fi

	URL=$STREAMLIST

	echo "$URL"
}

aliezold() 
{ 
	URL=`$curlbin "$INPUT" | sed 's/source:/\nsource:/' | grep ^source: | cut -d"'" -f2` 
	REFERER=`echo "$INPUT" | sed -e 's/=/3D/g' -e 's/&/26/g'` 
	echo "$URL|Referer=$REFERER&User-Agent=$USERAGENT" 
} 

aliez()
{
	rm -f $TMP/cache.hoster.$hoster.* > /dev/null 2>&1
	#http://emb.aliez.me/player/live.php?id=56180&w=700&h=480"
	REFERER=`echo "$INPUT" | sed -e 's/=/%3D/g' -e 's/&/%26/g'`
	EXTRA="|Referer=$REFERER&User-Agent=$USERAGENT"

	STREAMLIST="$TMP/$TYPE.$hoster.$FILENAME.streamlist"
	if [ -e "$STREAMLIST" ];then
		rm -f $STREAMLIST > /dev/null 2>&1
	fi

	$curlbin "$INPUT" -o $TMP/cache.hoster.$hoster.1
	cat $TMP/cache.hoster.$hoster.1 | sed 's/source:/\nfound=/' | grep ^found= | cut -d"'" -f2 >$TMP/cache.hoster.$hoster.url1
	URL=`cat $TMP/cache.hoster.$hoster.url1 | head -n1`
	if [ ! -z "$URL" ];then
		echo "$URL$EXTRA" >> $STREAMLIST
	fi
#	URL=""
	#file:	'http://a3.aliez.me:8080/hls/streama57449/index.m3u8?st=dgw2dOq8tyFkLLBLn2ycXA',

	cat $TMP/cache.hoster.$hoster.1 | sed 's/file:/\nfound=/' | grep ^found= | cut -d"'" -f2 >$TMP/cache.hoster.$hoster.url2
	URL=`cat $TMP/cache.hoster.$hoster.url2 | head -n1`
	if [ ! -z "$URL" ];then
		echo "$URL$EXTRA" >> $STREAMLIST
	fi
#URL=""
	#"file":		"rtmp%3A%2F%2Fa3.aliez.me%2Flive%2Fstreama57449%3Ftoken%3Dd11304fabb8e64327df8427e1c2fd5d9"
	cat $TMP/cache.hoster.$hoster.1 | sed 's/"file":/\nfound=/' | grep ^found= | cut -d'"' -f2 >$TMP/cache.hoster.$hoster.url3
	URL=`cat $TMP/cache.hoster.$hoster.url3 | head -n1`

	if [ "`echo $URL | grep rtmp | wc -l`" -eq 1 ];then
		#new swfobject.embedSWF("http://i.aliez.me/swf/playernew.swf?0", "mediaspace", "700", "480", "9.0.115.0", false, flashvars, params);
		cat $TMP/cache.hoster.$hoster.1 | sed 's/swfobject.embedSWF/\nfound=/' | grep ^found= | cut -d'"' -f2 >$TMP/cache.hoster.$hoster.url3.rtmp.swfurl
		SWFURL=`cat $TMP/cache.hoster.$hoster.url3.rtmp.swfurl | head -n1`
		URL=`echo "$URL" | sed -e 's/%3A/:/g' -e 's!%2F!/!g' -e 's!%3D!=!g' -e 's!%3F!?!g'`
		EXTRA=" pageUrl=$REFERER"
		if [ ! -z "$SWFURL" ];then
			EXTRA="$EXTRA swfUrl=$SWFURL swfVfy=1 timeout=15 live=1"
		fi
		if [ ! -z "$URL" ];then
			echo "$URL$EXTRA" >> $STREAMLIST
		fi
	fi
	URL=$STREAMLIST


	echo "$URL"
}

sport7()
{
	#http://sport7.tech/487b826914d11080dce4b502052b012d-live.html
	#var videoLink = 'http://g4.securestream.sport7.tech/stream/NzYyZDUwZWNkODc5YWM5YjViY2ZkOTVhZGNjOGM1ZTc=/BTSport3.m3u8';

	rm -f $TMP/cache.hoster.$hoster.* > /dev/null 2>&1
	REFERER=`echo "$INPUT" | sed -e 's/=/%3D/g' -e 's/&/%26/g'`
	EXTRA="|Referer=$REFERER&User-Agent=$USERAGENT"

	STREAMLIST="$TMP/$TYPE.$hoster.$FILENAME.streamlist"
	if [ -e "$STREAMLIST" ];then
		rm -f $STREAMLIST > /dev/null 2>&1
	fi

	$curlbin "$INPUT" -o $TMP/cache.hoster.$hoster.1
	cat $TMP/cache.hoster.$hoster.1 | sed 's/var videoLink/\nfound=/' | grep ^found= | cut -d"'" -f2 >$TMP/cache.hoster.$hoster.url1
	URL=`cat $TMP/cache.hoster.$hoster.url1 | head -n1`
	# enable httponly cookie
	sed 's/#HttpOnly_//g' -i /mnt/network/cookies

	if [ ! -z "$URL" ];then
		echo "$URL$EXTRA" > $STREAMLIST
		#echo "$URL$EXTRA"
		echo "$STREAMLIST"
	fi
	
	
#	URL=`$curlbin "$INPUT" | sed 's/var videoLink/\nfound=/' | grep ^found= | cut -d"'" -f2`
#	REFERER=`echo "$INPUT" | sed -e 's/=/3D/g' -e 's/&/26/g'`
#	sed 's/#HttpOnly_//g' -i /mnt/network/cookies
#	echo "$URL|Referer=$REFERER&X-f=96.91.79.87&User-Agent=$USERAGENT"
#	cat $STREAMLIST
}

sportstream365()
{
	#http://sport7.tech/487b826914d11080dce4b502052b012d-live.html
	#var videoLink = 'http://g4.securestream.sport7.tech/stream/NzYyZDUwZWNkODc5YWM5YjViY2ZkOTVhZGNjOGM1ZTc=/BTSport3.m3u8';

	rm -f $TMP/cache.hoster.$hoster.* > /dev/null 2>&1
#	REFERER=`echo "$INPUT" | sed -e 's/=/%3D/g' -e 's/&/%26/g'`
	REFERER=`echo "$INPUT" | sed -e 's/=/%3D/g' -e 's/&/%26/g'`
	EXTRA="|Referer=$REFERER&User-Agent=$USERAGENT"

	STREAMLIST="$TMP/$TYPE.$hoster.$FILENAME.streamlist"
	if [ -e "$STREAMLIST" ];then
		rm -f $STREAMLIST > /dev/null 2>&1
	fi

	$curlbin "$INPUT" -o $TMP/cache.hoster.$hoster.1

	#<input type="hidden" name="game" value="146875046">
	id=$(cat $TMP/cache.hoster.$hoster.1 | sed -nr 's/.*name="game" value="([^"]+)".*/\1/p')
	ts=$(date +%s)
	URL="http://sportstream365.com/LiveFeed/GetGame?id=$id&partner=24"

	$curlbin "$URL" --referer "$REFERER" -H "X-Requested-With: ShockwaveFlash/27.0.0.187" -o $TMP/cache.hoster.$hoster.2


	#"VI":"1032572"
	id=$(cat $TMP/cache.hoster.$hoster.2 | sed -nr 's/.*"VI" :"([^"]+)".*/\1/p')
	if [ -z "$id" ];then
		id=$(cat $TMP/cache.hoster.$hoster.2 | sed -nr 's/.*"VI":"([^"]+)".*/\1/p')
	fi
	#https://github.com/XvBMC/repository.xvbmc/blob/master/Dependencies/script.module.liveresolver/lib/liveresolver/resolvers/sportstream365.py
	URL=http://93.189.57.254/edge0/xrecord/$id/prog_index.m3u8
	# enable httponly cookie
	sed 's/#HttpOnly_//g' -i /mnt/network/cookies

	if [ ! -z "$URL" ];then
		echo "$URL$EXTRA" > $STREAMLIST
		#echo "$URL$EXTRA"
		echo "$STREAMLIST"
	fi
}

sportsonline()
{
	#http://sportsonline.pw/livetv/pt-sporttv1.html
	rm -f $TMP/cache.hoster.$hoster.* > /dev/null 2>&1
	REFERER=`echo "$INPUT" | sed -e 's/=/%3D/g' -e 's/&/%26/g'`
	EXTRA="|Referer=$REFERER&User-Agent=$USERAGENT"

	STREAMLIST="$TMP/$TYPE.$hoster.$FILENAME.streamlist"
	if [ -e "$STREAMLIST" ];then
		rm -f $STREAMLIST > /dev/null 2>&1
	fi

	$curlbin "$INPUT" -o $TMP/cache.hoster.$hoster.1

	#<iframe src="http://widestream.io/embedClappr.php?live=13593" width="100%" height="100%" scrolling="no" frameborder="0" allowfullscreen="true"></iframe>
	TMPURL=$(cat $TMP/cache.hoster.$hoster.1 | sed -nr 's/.*iframe src="([^"]+)".*/\1/p')

	$curlbin "$TMPURL" --referer "$REFERER" -H "X-Requested-With: ShockwaveFlash/27.0.0.187" -o $TMP/cache.hoster.$hoster.2

	#source: "http://ultra.widestream.io:8081/wideedge/1022qvk/playlist.m3u8?wmsAuthSign=c2VydmVyX3RpbWU9MTIvOS8yMDE3IDEyOjMyOjQzIEFNJmhhc2hfdmFsdWU9ZW9WeThRL0JmaVA2dnFUQm15Ukkvdz09JnZhbGlkbWludXRlcz0yMA==",
	TMPURL=$(cat $TMP/cache.hoster.$hoster.2 | sed -nr 's/.*source: "([^"]+)".*/\1/p')

	if [ ! -z "$TMPURL" ];then
		echo "$TMPURL$EXTRA" > $STREAMLIST
		#echo "$URL$EXTRA"
		echo "$STREAMLIST"
	fi
}

assia()
{
	#http://assia.tv/live/gol-tv/?lang=ru
	rm -f $TMP/cache.hoster.$hoster.* > /dev/null 2>&1
	REFERER=`echo "$INPUT" | sed -e 's/=/%3D/g' -e 's/&/%26/g'`
	EXTRA="|Referer=$REFERER&User-Agent=$USERAGENT"

	STREAMLIST="$TMP/$TYPE.$hoster.$FILENAME.streamlist"
	if [ -e "$STREAMLIST" ];then
		rm -f $STREAMLIST > /dev/null 2>&1
	fi

	$curlbin "$INPUT" -o $TMP/cache.hoster.$hoster.1

	#this.videoplayer2600 = new Uppod({m:"video",uid:"videoplayer2600",comment:"",file:"http://video.assia.tv/hls/50.m3u8?md5=UD9zcmEXXgjdB92hTu9_nw&expires=1512784272",st:"uppodvideo",onReady: function(uppod){setTimeout(uppod.Pause, 60000);}});
	TMPURL=$(cat $TMP/cache.hoster.$hoster.1 | sed -nr 's/.*file:"([^"]+)".*/\1/p' | tail -n1)

	if [ ! -z "$TMPURL" ];then
		echo "$TMPURL$EXTRA" > $STREAMLIST
		#echo "$URL$EXTRA"
		#http://video.assia.tv/hls/50.m3u8?md5=olw9K2ANtWtCPiL6LreprA&expires=1512783904|User-Agent=Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.90 Safari/537.36

		echo "$STREAMLIST"
	fi
}

cricfree()
{
	#http://cricfree.sc/update/bt2.php
	rm -f $TMP/cache.hoster.$hoster.* > /dev/null 2>&1
	REFERER=`echo "$INPUT" | sed -e 's/=/%3D/g' -e 's/&/%26/g'`
	EXTRA="|Referer=$REFERER&User-Agent=$USERAGENT"

	STREAMLIST="$TMP/$TYPE.$hoster.$FILENAME.streamlist"
	if [ -e "$STREAMLIST" ];then
		rm -f $STREAMLIST > /dev/null 2>&1
	fi

	$curlbin "$INPUT" -o $TMP/cache.hoster.$hoster.1

	#<iframe frameborder="0" marginheight="0" allowfullscreen="true" marginwidth="0" height="555" src="http://cricfree.sc/update/bt1.php" id="iframe" name="iframe_a" scrolling="no" width="620">Your Browser Do not Support Iframe</iframe>
	TMPURL1=$(cat $TMP/cache.hoster.$hoster.1 | grep "<iframe" | grep cricfree | sed -nr 's/.*src="([^"]+)".*/\1/p')

#	$curlbin "$INPUT" -o $TMP/cache.hoster.$hoster.1
	$curlbin "$TMPURL1" -o $TMP/cache.hoster.$hoster.2

	#<script type='text/javascript'>id='bt2i'; width='620'; height='490';</script><script type='text/javascript' src='http://hi.notkodi.science/streamgame.js'></script>
	TMPURL2=$(cat $TMP/cache.hoster.$hoster.2 | grep "id=" | sed -nr "s/.*src='([^']+)'.*/\1/p")
	id=$(cat $TMP/cache.hoster.$hoster.2 | grep "id=" | sed -nr "s/.*id='([^']+)'.*/\1/p")

	$curlbin "$TMPURL2" --referer "$REFERER" -H "X-Requested-With: ShockwaveFlash/27.0.0.187" -o $TMP/cache.hoster.$hoster.3

	#document.write('<iframe allowfullscreen width="'+width+'" height="'+height+'" scrolling="no" frameborder="0" marginwidth="0" marginheight="0" scrolling="no" allowtransparency="true" src="http://veremos.date/all.php?id='+id+'&p='+p+'&c='+c+'&stretching='
	TMPURL3=$(cat $TMP/cache.hoster.$hoster.3 | grep document.write | sed -nr 's/.*src="([^"]+)".*/\1/p')

	#var stretching = 'uniform';
	stretching=$(cat $TMP/cache.hoster.$hoster.3 | sed -nr "s/.*var stretching = '([^']+)'.*/\1/p")

	#var c = '0';
	c=$(cat $TMP/cache.hoster.$hoster.3 | sed -nr "s/.*var c = '([^']+)'.*/\1/p")

	#var p = '0';
	p=$(cat $TMP/cache.hoster.$hoster.3 | sed -nr "s/.*var p = '([^']+)'.*/\1/p")

	#http://veremos.date/all.php?id=bt2i&p=0&c=0&stretching=uniform
	TMPURL3=$(echo $TMPURL3 | sed -e "s/'+id+'/$id/" -e "s/'+p+'/$p/" -e "s/'+c+'/$c/" -e "s/'+stretching+'/$stretching/")

	#$curlbin http://veremos.date/all.php?id=bt2i&p=0&c=0&stretching=uniform" --referer "http://cricfree.sc/update/bt2.php"
	$curlbin "$TMPURL3" --referer "$REFERER" -H "X-Requested-With: ShockwaveFlash/27.0.0.187" -o $TMP/cache.hoster.$hoster.4

	#<iframe allowfullscreen="" width="100%" height="100%" scrolling="no" frameborder="0" marginwidth="0" marginheight="0" allowtransparency="true" src="http://soretin.stream/embeds/1all.php?id=256512&st=vkWS1ifC7TSE10IQKmgUEQ&e=1512815257&amp;p=0&amp;c=0&amp;stretching="></iframe>
	TMPURL4=$(cat $TMP/cache.hoster.$hoster.4 | grep "<iframe" | sed -nr 's/.*src="([^"]+)".*/\1/p')

	#$curlbin "http://soretin.stream/embeds/1all.php?id=256512&st=jOQy59wu1omnfUnNywQLDA&e=1512812987&p=0&c=0&stretching=" --referer "http://veremos.date/all.php?id=bt2i&p=0&c=0&stretching=uniform"
	$curlbin "$TMPURL4" --referer "$TMPURL3" -H "X-Requested-With: ShockwaveFlash/27.0.0.187" -o $TMP/cache.hoster.$hoster.5

	#return(["h","t","t","p",":","\/","\/","8","0",".","8","2",".","6","5",".","1","6","2","\/","c","a","v","s","c","a","m","p","e","o","n","\/","2","5","6","5","1","2",".","m","3","u","8","?","w","m","s","A","u","t","h","S","i","g","n","2","=","N","W","E","x","N","T","Y","3","Y","W","Y","3","N","j","h","l","Z","g","=","=","&","t","o","k","e","n","="].join("") + yUAerrrlsnutgriaaSbea.join("") + document.getElementById("aieBskfcrugtntihSa").innerHTML);
	TMPURL=$(cat $TMP/cache.hoster.$hoster.5 | grep 'return(\["' | sed -e 's/","//g' -e 's/\\//g'| cut -d'"' -f2)

	#return(["h","t","t","p",":","\/","\/","8","0",".","8","2",".","6","5",".","1","6","2","\/","c","a","v","s","c","a","m","p","e","o","n","\/","2","5","6","5","1","2",".","m","3","u","8","?","w","m","s","A","u","t","h","S","i","g","n","2","=","N","W","E","x","N","T","Y","3","Y","W","Y","3","N","j","h","l","Z","g","=","=","&","t","o","k","e","n","="].join("") + uaagraneeblAStrsyrrUi.join("") + document.getElementById("rBkacSifaguntesith").innerHTML);
	#uaagraneeblAStrsyrrUi
	searchtoken1=$(cat $TMP/cache.hoster.$hoster.5 | grep 'return(\["'| sed -e 's/","//g' -e 's/\\//g' | awk '{ print $3 }' | cut -d"." -f1)
	#var uaagraneeblAStrsyrrUi = ["3Rp","UJ5","ZoY","c2V","J","jgy","X3Z","9MT","XNo","ydm","ZEp","VyX","0OC","VlP","MDI","bWU","UxM","hbH"];
	TOKEN1=$(cat $TMP/cache.hoster.$hoster.5 | grep "var $searchtoken1" | sed -e 's/","//g' -e 's/\\//g' | cut -d'"' -f2)

	#return(["h","t","t","p",":","\/","\/","8","0",".","8","2",".","6","5",".","1","6","2","\/","c","a","v","s","c","a","m","p","e","o","n","\/","2","5","6","5","1","2",".","m","3","u","8","?","w","m","s","A","u","t","h","S","i","g","n","2","=","N","W","E","x","N","T","Y","3","Y","W","Y","3","N","j","h","l","Z","g","=","=","&","t","o","k","e","n","="].join("") + uaagraneeblAStrsyrrUi.join("") + document.getElementById("rBkacSifaguntesith").innerHTML);
	#rBkacSifaguntesith
	searchtoken2=$(cat $TMP/cache.hoster.$hoster.5 | sed -nr 's/.*document.getElementById\("([^"]+)".*/\1/p')
	#<span style='display:none' id=uieBiSefkngtshatrc>VZbFOMVlSL3F2lZhaE01Ta9xAWMvwQGwPZnMRY9nZ2ppVXWG</span><span style='display:none' id=suteaentirhgBfkSci>bnMWZL3XFlvAhMVpwPZ0pZ9TFSWnVGwaVMGR9Q22aOYZlEx1</span><span style='display:none' id=tSiBkaefnshuticegr>LWVMAbFZ1VaPhO3MnpF2R29ZlvSEGalwTYp9VMZQX0xZnWwG</span><span style='display:none' id=iecaSftBresutghkin>vVxZVQpVMn0ZS1Pp22nO3hwa9ZFTYRaWGFlMl9wZGEbAXLWM</span><span style='display:none' id=SnstuicrBgfitehake>Th2F1QFSWAZ9MWlPv92Vpw0LOZGwRMXbaV3ZElnMnaYpZVxG</span><span style='display:none' id=stBeiuSheiakfcrngt>bYTxap13WP2ZLlhnMF2GVXlwvWVOASaMEnQw9RVZ90GMZpZF</span><span style='display:none' id=ihenkcraesSiugBftt>wL2xZv9nMXwnZGGaAVVWWQFlZS90EMp3TlaVFP2ORbZpY1Mh</span><span style='display:none' id=hftrceSekuiiastngB>1ZXap0LlnO3VlGGZabWRnFZMMEw2xMvhZwpPVFY9VAWT92SQ</span><span style='display:none' id=inBehfgtcSautirkse>x2a2Zp3OF09XLWQZVbMMwFZnGGRpYnvPha19VEMlATVZWlwS</span><span style='display:none' id=rBkacSifaguntesith>MGZVL2wvanFha3ROWlVFQlE9PSZ2YWxpZG1pbnV0ZXM9MTAw</span><span style='display:none' id=etBirkuaiStfehgcsn>ZA1ZQGZOa39nYpVnWpS2llLwFxvawFVZb0MMPREGhXM2TVW9</span><span style='display:none' id=rshfekaguceSitnBit>pRW2TQMZaVlZ29OWFnXaFMLnV1VxZ3Y0ZGpAb9hPMSwvGEwl</span><span style='display:none' id=irBcttfehsganeSiuk>MTpXZWZb3MvnFZ2wlLFGA9nQpa2SYR9E01VxZGhwWMVOVPla</span><span style='display:none' id=skieftSBngitcuarhe>xAG1QET0WG9lnhZFPpYOa2M9lRZXnaFwVSpWLMbwVZ3MVZv2</span><span style='display:none' id=iSnkrBusfceghttiea>32EbYXWFl1Z2VZMVSGv9pZPMZLaFnaTpAh9OxGlQWMwRVw0n</span><span style='display:none' id=utrtiSfieeasBhnkcg>wZGZWxF2bPpahW9EnMV1TvYw92XlVaMpSLGMQl3OVRnA0FZZ</span><span style='display:none' id=SiisktnfeaghceuBrt>29W1LbRnVlMTZwF2WMEpw9AvGVZ0VYSnZXP3OMhxGZlQpFaa</span><span style='display:none' id=scugBteriiSehafktn>lFTMVV9W3ZhZaA1FMn9YpEvVnaxQL2XSZMpGWbwO2R0lZwPG</span><span style='display:none' id=itefaihSreuBgkcnts>Y0Eb2hQannvFZV2ZGZAaLVZlSMGlFp3V9xXPwMMTR9wO1WWp</span><span style='display:none' id=aecnhiigtuekfSrsBt>naYZSMMPbQnvE9hTVW3Ga2pwZ0GMRZwXpWFZ2llLAxOVVF19</span><span style='display:none' id=citeehgiustarnBSkf>XG29ZphlbYMLSaZ9V3PwWlQpGM12a0ZZvEFwVxWAFOMVnnRT</span><span style='display:none' id=rtcenehgusaikBSfit>3hpv2ZnVXV9lWlYxV0M9wMWPSMAnGa2ZQFROpab1ZwZETGLF</span><span style='display:none' id=csSnhfBitteguakeir>ZnXhv2GpMZP0xaAMQ1YVV9lZROWwaFnWT3M9ZlwF2pEVGLbS</span><span style='display:none' id=stigfkinrehBSaceut>p2wl0RQG3OFZhFMnaTxXV9pvZA2LMbVESWlZaVP1YZwMnG9W</span><html>
	#MGZVL2wvanFha3ROWlVFQlE9PSZ2YWxpZG1pbnV0ZXM9MTAw
	TOKEN2=$(cat $TMP/cache.hoster.$hoster.5 | sed "s/id=$searchtoken2/\nfoundid=/" | grep ^"foundid=" | cut -d'>' -f2 | cut -d'<' -f1)

	if [ ! -z "$TMPURL" ];then
#		echo "$TMPURL$token1$token2$EXTRA" > $STREAMLIST
		#http://89.248.172.95/cavscampeon/256512.m3u8?wmsAuthSign2=NWExNTY3YWY3NjhlZg==&token=c2VydmVyX3RpbWU9MTUxMjgxMjM4NyZoYXNoX3ZhbHVlPVNrcndRU0Z2Mlp2QlUvbjg4N0lLQWc9PSZ2YWxpZG1pbnV0ZXM9MTAw|Referer=http%3A%2F%2Fsoretin.stream%2Fembeds%2F1all.php%3Fid%3D256512%26st%3DjOQy59wu1omnfUnNywQLDA%26e%3D1512812987%26p%3D0%26c%3D0%26stretching%3D&User-Agent=Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.100 Safari/537.36&X-Requested-With=ShockwaveFlash/23.0.0.162

		echo "$TMPURL$TOKEN1$TOKEN2" > $STREAMLIST

		#echo "$URL$EXTRA"
		echo "$STREAMLIST"
	fi
}

broadcast()
{
	#http://assia.tv/live/gol-tv/?lang=ru
	rm -f $TMP/cache.hoster.$hoster.* > /dev/null 2>&1
	REFERER=`echo "$INPUT" | sed -e 's/=/%3D/g' -e 's/&/%26/g'`
	EXTRA="|Referer=$REFERER&User-Agent=$USERAGENT"

	STREAMLIST="$TMP/$TYPE.$hoster.$FILENAME.streamlist"
	if [ -e "$STREAMLIST" ];then
		rm -f $STREAMLIST > /dev/null 2>&1
	fi

	#$curlbin --referer "http://bro.adca.st/stream.php?id=17733&p=1&c=0&stretching=uniform&old=0" "http://bro.adca.st/token2.php"
	$curlbin "http://bro.adca.st/token2.php" --referer "$INPUT" -o $TMP/cache.hoster.$hoster.1

	#{"rumba":"ruYg748QTLGfAn2iZo3bpw&expires=1512880893"}
	TOKEN=$(cat $TMP/cache.hoster.$hoster.1 | sed -nr 's/.*"rumba":"([^"]+)".*/\1/p' | tail -n1)

	#$curlbin --referer "http://bonstreams.net" "http://bro.adca.st/stream.php?id=17733&p=1&c=0&stretching=uniform&old=0"
	$curlbin "$INPUT" --referer "http://bonstreams.net" -o $TMP/cache.hoster.$hoster.2

    #trap = "aHR0cDovL3RpZXIyLnBva2VyY29hbGl0aW9uLnB3L28xMC8xNzczMy5tM3U4P3NmPU5UazVOR0UzWXpSaE16bGpOUT09JnRva2VuPQ=
	TRAP=$(cat $TMP/cache.hoster.$hoster.2 | sed -nr 's/.*trap = "([^"]+)".*/\1/p' | tail -n1)

	echo "$TRAP" > $TMP/cache.hoster.$hoster.base64
	TMPURL=$(base64 -d $TMP/cache.hoster.$hoster.base64)

	# enable httponly cookie
	sed 's/#HttpOnly_//g' -i /mnt/network/cookies

	if [ ! -z "$TMPURL" ];then
		#hls://http://tier2.pokercoalition.pw/o10/17733.m3u8?sf=NTk5NGE3YzRhMzljNQ==&token=jvVfe9gic8uQ3QqCXmZemw&expires=1512879969|Referer=http%3A%2F%2Fbro.adca.st%2Fstream.php%3Fid%3D17733%26p%3D1%26c%3D0%26stretching%3Duniform%26old%3D0&User-Agent=Mozilla%2F5.0%20(Windows%20NT%2010.0%3B%20Win64%3B%20x64)%20AppleWebKit%2F537.36%20(KHTML,%20like%20Gecko)%20Chrome%2F61.0.3163.100%20Safari%2F537.36
		echo "$TMPURL$TOKEN$EXTRA" > $STREAMLIST
		echo "$STREAMLIST"
	fi
}

all()
{
	rm -f $TMP/cache.hoster.$hoster.* > /dev/null 2>&1
	REFERER=`echo "$INPUT" | sed -e 's/=/%3D/g' -e 's/&/%26/g'`
	EXTRA="|Referer=$REFERER&User-Agent=$USERAGENT"

	STREAMLIST="$TMP/$TYPE.$hoster.$FILENAME.streamlist"
	if [ -e "$STREAMLIST" ];then
		rm -f $STREAMLIST > /dev/null 2>&1
	fi

	rm $TMP/cache.$PARSER.$INPUT.$FROM.1 > /dev/null 2>&1

	$curlbin -o $TMP/cache.$PARSER.$INPUT.$FROM.1 ${1}
	URL=`zcat $TMP/cache.$PARSER.$INPUT.$FROM.1 | grep iframe | sed -nr 's/.*src="([^"]+)".*/\1/p'`
	if [ -z "$URL" ];then
		URL=`cat $TMP/cache.$PARSER.$INPUT.$FROM.1 | grep iframe | sed -nr 's/.*src="([^"]+)".*/\1/p'`
	fi
	if [ -z "$URL" ];then
		URL=`cat $TMP/cache.$PARSER.$INPUT.$FROM.1 | grep "text/javascript" | grep -v jQuery | sed -nr 's/.*src="([^"]+)".*/\1/p'`
	fi

#	echo $URL
	$curlbin "$INPUT" -o $TMP/cache.hoster.$hoster.2
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
	mkdir $TMP > /dev/null 2>&1

	echo "$BIN $youtubebin $INPUT --username $USER --password $PASS" > /tmp/.last_hoster_youtube_dl.log

	$BIN $youtubebin "$INPUT" --username "$USER" --password "$PASS" > $TMP/$TYPE.$hoster.$FILENAME.streamlist
#	cat $TMP/$TYPE.$hoster.$FILENAME.streamlist
	echo $TMP/$TYPE.$hoster.$FILENAME.streamlist
}

youtube_dlbg()
{
#	echo "$BIN $youtubebinbg $DEST $INPUT" > /tmp/.last_hoster_youtube_dlbg.log
#	URL=`$BIN $CMD/lib/youtube_dl/__main__.py --no-check-certificate --cookies /mnt/network/cookies --user-agent "$USERAGENT" --format mp4 --restrict-filenames --ignore-errors --output "$1" "$2"`
#	echo "$URL" >> /tmp/.last_hoster_youtube_dlbg.log
#	echo $URL
	mkdir $TMP > /dev/null 2>&1

	echo "$BIN $youtubebinbg $DEST $INPUT" > /tmp/.last_hoster_$TYPE.log
	$BIN $youtubebinbg "$DEST" "$INPUT" >> /tmp/.last_hoster_$TYPE.log
#	echo $TMP/$TYPE.$hoster.$FILENAME.streamlist
}

hlsdl()
{
	mkdir $TMP > /dev/null 2>&1

	echo "$HLSBIN $hlsdlbg $DEST $INPUT" > /tmp/.last_hoster_$TYPE.log

	REFERER=$(echo "$INPUT" | sed -nr 's/.*Referer=([^=]+)&.*/\1/p')
	if [ -z "$REFERER" ];then
		REFERER=$(echo "$INPUT" | sed -nr 's/.*Referer=([^=]+).*/\1/p')
	fi

	if [ ! -z "$REFERER" ];then
		REFERER="Referer: $REFERER"
	fi

	TMPUSERAGENT=$(echo "$INPUT" | sed -nr 's/.*User-Agent=([^=]+)&.*/\1/p')
	if [ -z "$TMPUSERAGENT" ];then
		TMPUSERAGENT=$(echo "$INPUT" | sed -nr 's/.*User-Agent=([^=]+).*/\1/p')
	fi
	if [ ! -z "$TMPUSERAGENT" ];then
		USERAGENT=$TMPUSERAGENT
	fi

	URL=$(echo "$INPUT" | tr '|' '\n' | head -n1)

	echo $HLSBIN "$URL" -v -f -u "$USERAGENT" -h "$REFERER" -o "$DEST" >> /tmp/.last_hoster_$TYPE.log
	$HLSBIN "$URL" -v -f -u "$USERAGENT" -h "$REFERER" -o "$DEST" >> /tmp/.last_hoster_$TYPE.log
#	$HLSBIN "$URL" -v -u "$USERA" -h "$REFERER" -o "$DEST" >> /tmp/.last_hoster_$TYPE.log

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
		sport7) sport7 $INPUT;;
		sportstream365) sportstream365 $INPUT;;
		sportsonline) sportsonline $INPUT;;
		assia) assia $INPUT;;
		cricfree) cricfree $INPUT;;
		adca) broadcast $INPUT;;
#		*) all $INPUT;;
	esac
fi

if [ "$TYPE" == "youtube_dl" ];then
	echo  "$INPUT" > /tmp/.last_hoster_$TYPE_$hoster.log
	case $hoster in
		*) youtube_dl $INPUT $USER $PASS;;
	esac
fi

if [ "$TYPE" == "youtube_dlbg" ];then
	echo  "$INPUT" > /tmp/.last_hoster_$TYPE_$hoster.log
	case $hoster in
		*) youtube_dlbg $DEST $INPUT;;
	esac
fi

if [ "$TYPE" == "hlsdl" ];then
	echo  "$INPUT" > /tmp/.last_hoster_$TYPE_$hoster.log
	case $hoster in
		*) hlsdl $INPUT;;
	esac
fi
