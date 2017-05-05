#!/bin/sh

sec=$1

if [ "$1" == "-h" ]; then
  echo "usage: $0 seconds program parameter" > /dev/stderr
  exit
fi

if [ -z "$sec" ]; then
  sec=10
fi

#remove ret code file
rm -f /tmp/.start_sec.ret

# start program in background
len="${#sec}"
len=$(($len + 1))
("${@:$len}"; echo $? > /tmp/.start_sec.ret) &
pid=$!

if [ -z "$pid" ]; then
	echo "[start_sec.sh] warning, pid is empty" > /dev/stderr
fi

# warte 
secprint=$sec
sec=$(($sec * 20))
a=0
while [ -d /proc/$pid ]; do
  #echo wait
  a=$(($a+1))
  if [ $a -gt $sec ]; then 
    echo "[start_sec.sh] warning, detect hanging progam after $secprint seconds (${@:2})" > /dev/stderr
    exit 100
  fi
  usleep 50000
done

if [ -e /tmp/.start_sec.ret ]; then
	exit `cat /tmp/.start_sec.ret`
fi
