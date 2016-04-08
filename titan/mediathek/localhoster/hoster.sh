#!/bin/sh
# first sh box hoster for titannit mfg obi

TYPE=$1
INPUT=$2

ARCH=`cat /etc/.arch`
debuglevel=`cat /mnt/config/titan.cfg | grep debuglevel | cut -d"=" -f2`
curlbin='curl -k -s -L --cookie /mnt/network/cookies --cookie-jar /mnt/network/cookies'
if [ "$debuglevel" == "99" ]; then curlbin="$curlbin -v"; fi
wgetbin="wget -q -T2"
TMP=/tmp/localcache

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
	if [ "$streamurl" != "$hosterurl" ];then
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

if [ "$TYPE" == "get" ];then
	case $hoster in
		ecostream) ecostream $INPUT;;
		giga) giga $INPUT;;
	esac
fi