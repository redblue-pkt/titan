if [ -L /mnt ];then
	mv -f /mnt /mnt_old
	cp -a /etc/titan.restore/mnt /
fi

date
echo npdate
ntpdate -b ptbtime1.ptb.de &
sleep 5
echo "####################################"
date
echo "####################################"

sleep 5
/usr/local/bin/titan /mnt/config/titan.cfg
