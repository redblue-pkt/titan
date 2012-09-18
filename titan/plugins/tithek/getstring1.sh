link=`cat $1 | grep "RegExp" | grep flv| sed "s/;',/\n/" | sed "s/;return p}('/\n/" | grep ";"`
echo $link
