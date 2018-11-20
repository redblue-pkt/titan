if [ -L /mnt ];then
	mv -f /mnt /mnt_old
	cp -a /etc/titan.restore/mnt /
fi

/usr/local/bin/titan /mnt/config/titan.cfg
