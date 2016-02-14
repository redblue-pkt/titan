ecostream($link)
{
	debuglevel=`cat /mnt/config/titan.cfg | grep debuglevel | cut -d"=" -f2`
	curlbin='curl -k -s -L --cookie /mnt/network/cookies --cookie-jar /mnt/network/cookies'
	if [ "$debuglevel" == "99" ]; then curlbin="$curlbin -v"; fi
	hosterurl=http://www.ecostream.tv
	
	data="tpm=`$curlbin $link | grep footerhash | cut -d"'" -f2``$curlbin $link | grep superslots | cut -d"'" -f2`&id=`$curlbin $link | grep data-id | cut -d'"' -f4`"
	streampage=`$curlbin $hosterurl/js/ecoss.js | grep "$.post('" | grep videos | cut -d"'" -f2`
	streamurl=$hosterurl`$curlbin -H "X-Requested-With: XMLHttpRequest" -X POST --data "$data" --referer $src $hosterurl/$streampage | cut -d'"' -f6`
	if [ "$streamurl" != "$hosterurl" ];then
		echo $streamurl
	else
		echo NULL
	fi
}