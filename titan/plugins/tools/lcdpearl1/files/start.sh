#!/bin/sh
PWD=`echo $0 | sed -e s/start.sh//g`
#PWD=`pwd`
export LD_LIBRARY_PATH=$PWD/libs
cd $PWD
#./lcd4linux -f $PWD/lcd4linux.conf $1
#./lcd4linux -f /var/etc/lcd4linux.conf $1
./lcd4linux -f lcd4linux.conf $1

exit
