
#! /bin/sh
#
### BEGIN INIT INFO
# Provides:             xupnpd
# Required-Start:       $network
# Required-Stop:        $network
# Short-Description:    xupnpd Server
### END INIT INFO

if [ -e /etc/.ddtbuild ];then
    export PATH=$PATH:/var/swap/bin:/mnt/swapextensions/bin:/var/bin

    if [ -e /mnt/swapextensions/bin/xupnpd ];then
	    INSTDIR=/mnt/swapextensions
    else
	    INSTDIR=/var/swap
    fi	
    export XUPNPDROOTDIR=$INSTDIR/etc/xupnpd
else
    INSTDIR=`echo $0 | sed -e s/xupnpd.sh//g`

    export XUPNPDROOTDIR=$INSTDIR/etc
    export PATH=$PATH:/var/swap/bin:/mnt/swapextensions/bin:/var/bin:$INSTDIR
fi

model=`cat /etc/model`
host=`hostname`
NAME=xupnpd
#DESC="xupnpd-$model"
DESC="xupnpd-$host"
DAEMON=xupnpd
USER=root
GROUP=root
IP=`ifconfig | grep inet | grep Bcast | awk '{print $2}' | cut -d":" -f2`
PORT=`cat /mnt/config/titan.cfg | grep stream_port | cut -d"=" -f2` 
 
case $1 in
  start)
    if [ -e /etc/.ddtbuild ];then
    	bouquet2m3u $IP $PORT $INSTDIR/etc/xupnpd/playlists
    else
    	bouquet2m3u $IP $PORT $INSTDIR/etc/playlists
    fi
    start-stop-daemon --verbose --start --exec xupnpd
  ;;
  stop)
    start-stop-daemon --verbose --stop --exec xupnpd
  ;;
  restart)
    $0 stop
    $0 start
  ;;
  *)
    echo "Usage: xupnpd.sh {start|restart|stop}"
  ;;
esac
