#! /bin/sh
#
### BEGIN INIT INFO
# Provides:             minidlna
# Required-Start:       $network
# Required-Stop:        $network
# Short-Description:    MiniDLNA Server
### END INIT INFO

export PATH=$PATH:/var/swap/bin:/var/bin
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/var/swap/lib
	
NAME=minidlna
DESC="MiniDLNA"
DAEMON=minidlna
USER=root
GROUP=root
 
params="-f /var/swap/etc/minidlna.conf -R"
 
case $1 in
  start)
    start-stop-daemon --verbose --start --exec minidlna -- ${params}
  ;;
  stop)
    start-stop-daemon --verbose --stop --exec minidlna
  ;;
  restart)
    $0 stop
    $0 start
  ;;
  *)
    echo "Usage: /var/swap/titanplugins/dlna/dlna.sh {start|restart|stop}"
  ;;
esac