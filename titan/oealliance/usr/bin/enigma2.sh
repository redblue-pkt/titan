if [ -L /mnt ];then
	mv -f /mnt /mnt_old
	cp -a /etc/titan.restore/mnt /
fi

ntpdate -b ptbtime1.ptb.de &

/usr/local/bin/titan /mnt/config/titan.cfg
