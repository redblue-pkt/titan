link=`cat $1 | grep "RegExp" | grep flv| sed "s/;',/\n/" | sed "s/;return p}('/\n/" | tail -n1 | cut -d"'" -f2`
echo $link
