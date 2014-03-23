#! /bin/sh
#
### BEGIN INIT INFO
# Provides:             xupnpd
# Required-Start:       $network
# Required-Stop:        $network
# Short-Description:    xupnpd Server
### END INIT INFO

export PATH=$PATH:/var/swap/bin:/mnt/swapextensions/bin:/var/bin
export XUPNPDROOTDIR=$2

model=`cat /etc/model`
NAME=xupnpd
DESC="xupnpd-$model"
DAEMON=xupnpd
USER=root
GROUP=root

 
case $1 in
  start)
	bouquet2m3u -o $2/playlists
    start-stop-daemon --verbose --start --exec xupnpd
  ;;
  stop)
  	killall -9 bouquet2m3u
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