TYPE=$1
FILE=$2
#echo /var/bin/putlocker-sockshare.sh sockshare 492B962B0DD5337E
rm -rf /tmp/kinox
mkdir /tmp/kinox

# type
# putlocker
# sockshare

rm -rf cache.*
#echo 00000000
#wget http://www.$TYPE/file/$FILE -O /tmp/kinox/cache_index.quell.html
wget http://www.$TYPE/$FILE -O /tmp/kinox/cache_index.quell.html
#echo 1111111111111
#echo  http://www.$TYPE/$FILE 
#HASH=`cat /tmp/kinox/cache_index.quell.html | grep hash | cut -d'"' -f 4`
ID=`cat /tmp/kinox/cache_index.quell.html | grep '<input type="hidden"' |  grep '"id"' | cut -d'"' -f6`
#echo ID $ID
FNAME=`cat /tmp/kinox/cache_index.quell.html | grep '<input type="hidden"' |  grep '"fname"' | cut -d'"' -f6`
OP=`cat /tmp/kinox/cache_index.quell.html | grep '<input type="hidden"' |  grep '"op"' | cut -d'"' -f6`
OP=download1

HASH="id=$ID&referer=&fname=$FNAME&method_free=Free&usr_login=&op=$OP"
COUNT=`echo $HASH | wc -c`
COUNT=`expr $COUNT - 1`

#echo COUNT $COUNT

post="POST /$ID HTTP/1.1\r\nContent-Length: $COUNT\r\nAccept-Encoding: gzip\r\nConnection: close\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nHost: www.$TYPE\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nid=$ID&referer=&fname=$FNAME&method_free=Free&usr_login=&op=$OP"
#post="POST /$ID HTTP/1.1\r\nContent-Length: 142\r\nAccept-Encoding: gzip\r\nConnection: close\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nHost: www.$TYPE\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nid=$ID&referer=&fname=$FNAME&method_free=Free&usr_login=&op=$OP"
#echo post $post
#post="POST /file/$FILE HTTP/1.1\r\nContent-Length: 51\r\nAccept-Encoding: gzip\r\nConnection: close\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nHost: www.$TYPE\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nhash=$HASH&confirm=Continue+as+Free+User"
echo -e $post | /var/usr/local/share/titan/plugins/tithek/netcat www.$TYPE 80 > /tmp/kinox/cache_post.log
#echo 1222222222
p=`cat /tmp/kinox/cache_post.log | grep "RegExp" | grep flv| sed "s/;',/\n/" | sed "s/;return p}('/\n/" | grep ";"`
k=`cat /tmp/kinox/cache_post.log | grep "RegExp" | grep flv| sed "s/;',/\n/" | sed "s/;return p}('/\n/" | tail -n1 | cut -d"'" -f2`
#echo FNAME $FNAME
#echo OP $OP
echo p $p
echo k $k

exit
PHPSESSID=`strings /tmp/kinox/cache_post.log | grep Set-Cookie | grep PHPSESSID= | cut -d "=" -f2 | cut -d";" -f1`
SERVERID=`strings /tmp/kinox/cache_post.log | grep Set-Cookie | grep SERVERID= | cut -d "=" -f2 | cut -d";" -f1`

getstream="GET /$FILE HTTP/1.1\r\nHost: www.$TYPE.com\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n"
#getstream="GET /file/$FILE HTTP/1.1\r\nHost: www.$TYPE\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nCookie: SERVERID=$SERVERID; PHPSESSID=$PHPSESSID\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n"
echo -e $getstream | /var/usr/local/share/titan/plugins/tithek/netcat www.$TYPE 80 > /tmp/kinox/cache_getstream.log

cat /tmp/kinox/cache_getstream.log | sed '1,14d' | zcat > /tmp/kinox/cache_index.html 
STREAMLINK=`cat /tmp/kinox/cache_index.html | grep playlist: | cut -d "'" -f2`

getstream1="GET $STREAMLINK HTTP/1.1\r\nAccept-Encoding: identity\r\nHost: www.$TYPE\r\nCookie: SERVERID=$SERVERID; PHPSESSID=$PHPSESSID\r\nConnection: close\r\nUser-Agent: Python-urllib/2.6\r\n\r\n"
echo -e $getstream1 | /var/usr/local/share/titan/plugins/tithek/netcat www.$TYPE 80 > /tmp/kinox/cache_getstream1.log

STREAMLINK=`cat /tmp/kinox/cache_getstream1.log | grep xmlns:media= | tr " " "\n" | grep url= | cut -d'"' -f2`
rm -rf cache.*
echo $STREAMLINK
