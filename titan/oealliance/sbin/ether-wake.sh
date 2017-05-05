#! /bin/sh

ping -c2 -w3 -W3 "$1"

while read ip _ _ mac _ dev; do
	if [ "$ip" == "$1" -a "$mac" != "00:00:00:00:00:00" ]; then
		echo "wakeup $mac on $dev"
		ether-wake -i "$dev" "$mac"
		exit $?
	fi
done < /proc/net/arp

exit 1