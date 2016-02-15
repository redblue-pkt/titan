#!/bin/sh
# first sh box hoster for titannit mfg obi

INPUT=$1

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
	data="tpm=`$curlbin $1 | grep footerhash | cut -d"'" -f2``$curlbin $1 | grep superslots | cut -d"'" -f2`&id=`$curlbin $1 | grep data-id | cut -d'"' -f4`"
	streampage=`$curlbin $hosterurl/js/ecoss.js | grep "$.post('" | grep videos | cut -d"'" -f2`
	streamurl=$hosterurl`$curlbin -H "X-Requested-With: XMLHttpRequest" -X POST --data "$data" --referer $1 $hosterurl/$streampage | cut -d'"' -f6`
	if [ "$streamurl" != "$hosterurl" ];then
		echo $streamurl
	fi
}

case $hoster in
	ecostream) ecostream $1;;
esac
