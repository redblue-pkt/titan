#!/bin/bash
#

RANGE=$1
portlist="21 445"

rm /tmp/netbrowser.portscan.log > /dev/null 2>&1

for PORT in $portlist; do
	portscan 5 $RANGE $PORT 0 255 >> /tmp/netbrowser.portscan.log
done

arp -a > /tmp/netbrowser.arp.log
arp -a > /tmp/netbrowser.arp.log

cat /tmp/netbrowser.portscan.log | cut -d" " -f1 | sort -u >/tmp/netbrowser.portscan.ip.log

while read -u 3 ROUND; do
#	echo ROUND $ROUND
	while read -u 3 ROUND2; do
#		echo ROUND2 $ROUND2

		IP=$(echo $ROUND2 | awk '{ print $2 }'  | cut -d"(" -f2 | cut -d")" -f1)
#		echo IP $IP

		DNS=$(echo $ROUND2 | awk '{ print $1 }')
#		echo DNS $DNS

		if [ "$IP" == "$ROUND" ];then
			echo "$DNS|$IP"
		fi
	done 3</tmp/netbrowser.arp.log
	
done 3</tmp/netbrowser.portscan.ip.log
