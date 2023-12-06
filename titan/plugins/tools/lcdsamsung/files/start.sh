#!/bin/sh
PWD=`echo $0 | sed -e s/start.sh//g`
#PWD=`pwd`
#export LD_LIBRARY_PATH=$PWD/libs
cd $PWD
#./fbread $1 $2 &
fbread $1 100 &
exit
