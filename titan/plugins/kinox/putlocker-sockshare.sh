TYPE=$1
FILE=$2

# type
# putlocker
# sockshare

rm -rf cache.*
wget http://www.$TYPE.com/file/$FILE -O /tmp/kinox/cache_index.quell.html
HASH=`cat /tmp/kinox/cache_index.quell.html | grep hash | cut -d'"' -f 4`

post="POST /file/$FILE HTTP/1.1\r\nContent-Length: 51\r\nAccept-Encoding: gzip\r\nConnection: close\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nHost: www.$TYPE.com\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nhash=$HASH&confirm=Continue+as+Free+User"
echo -e $post | netcat www.$TYPE.com 80 > /tmp/kinox/cache_post.log

PHPSESSID=`cat cache_post.log | grep Set-Cookie | grep PHPSESSID= | cut -d "=" -f2 | cut -d";" -f1`
SERVERID=`cat cache_post.log | grep Set-Cookie | grep SERVERID= | cut -d "=" -f2 | cut -d";" -f1`
usender=`cat cache_post.log | grep Set-Cookie | grep usender= | cut -d "=" -f2 | cut -d";" -f1`

PHPSESSID=`strings cache_post.log | grep Set-Cookie | grep PHPSESSID= | cut -d "=" -f2 | cut -d";" -f1`
SERVERID=`strings cache_post.log | grep Set-Cookie | grep SERVERID= | cut -d "=" -f2 | cut -d";" -f1`
getstream="GET /file/$FILE HTTP/1.1\r\nHost: www.$TYPE.com\r\nUser-Agent: Mozilla/5.0 (X11; Linux i686) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.99 Safari/535.1\r\nCookie: SERVERID=$SERVERID; PHPSESSID=$PHPSESSID\r\nConnection: close\r\nAccept-Encoding: gzip\r\n\r\n"
echo -e $getstream | netcat www.$TYPE.com 80 > /tmp/kinox/cache_getstream.log

cat /tmp/kinox/cache_getstream.log | sed '1,14d' | zcat > /tmp/kinox/cache_index.html 
STREAMLINK=`cat /tmp/kinox/cache_index.html | grep playlist: | cut -d "'" -f2`

getstream1="GET $STREAMLINK HTTP/1.1\r\nAccept-Encoding: identity\r\nHost: www.$TYPE.com\r\nCookie: SERVERID=$SERVERID; PHPSESSID=$PHPSESSID\r\nConnection: close\r\nUser-Agent: Python-urllib/2.6\r\n\r\n"
echo -e $getstream1 | netcat www.$TYPE.com 80 > /tmp/kinox/cache_getstream1.log

STREAMLINK=`cat /tmp/kinox/cache_getstream1.log | grep xmlns:media= | tr " " "\n" | grep url= | cut -d'"' -f2`
rm -rf cache.*
echo $STREAMLINK
