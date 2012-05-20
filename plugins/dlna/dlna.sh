#! /bin/sh
#
### BEGIN INIT INFO
# Provides:             minidlna
# Required-Start:       $network
# Required-Stop:        $network
# Short-Description:    MiniDLNA Server
### END INIT INFO
 
NAME=minidlna
DESC="MiniDLNA"
DAEMON=/var/swap/titanplugins/dlna/minidlna
USER=root
GROUP=root
 
params="-f /var/etc/minidlna.conf -R"
 
case $1 in
  start)
    start-stop-daemon --verbose --start --exec /var/swap/titanplugins/dlna/minidlna -- ${params}
  ;;
  stop)
    start-stop-daemon --verbose --stop --exec /var/swap/titanplugins/dlna/minidlna
  ;;
  restart)
    $0 stop
    $0 start
  ;;
  *)
    echo "Usage: /var/swap/titanplugins/dlna/dlna.sh {start|restart|stop}"
  ;;
esac