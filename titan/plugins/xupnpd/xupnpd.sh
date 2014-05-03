#! /bin/sh
#
### BEGIN INIT INFO
# Provides:             xupnpd
# Required-Start:       $network
# Required-Stop:        $network
# Short-Description:    xupnpd Server
### END INIT INFO

export PATH=$PATH:/var/swap/bin:/mnt/swapextensions/bin:/var/bin

model=`cat /etc/model`
NAME=xupnpd
DESC="xupnpd-$model"
DAEMON=xupnpd
USER=root
GROUP=root
IP=`ifconfig | grep inet | grep Bcast | awk '{print $2}' | cut -d":" -f2`
 
case $1 in
  start)
	bouquet2m3u http://$IP:22222/ /mnt/swapextensions/etc/xupnpd/playlists
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