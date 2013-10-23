#! /bin/sh
#
### BEGIN INIT INFO
# Provides:             minidlna
# Required-Start:       $network
# Required-Stop:        $network
# Short-Description:    MiniDLNA Server
### END INIT INFO

export PATH=$PATH:/var/swap/bin:/mnt/swapextensions/bin:/var/bin
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/var/swap/lib:/mnt/swapextensions/lib:/var/lib

model=`cat /etc/model`
NAME=minidlna
DESC="MiniDLNA-$model"
DAEMON=minidlna
USER=root
GROUP=root
 
params="-f $2 -R"
 
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
    echo "Usage: dlna.sh {start|restart|stop}"
  ;;
esac