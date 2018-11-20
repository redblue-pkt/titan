if [ -L /mnt ];then
	mv -f /mnt /mnt_old; fi
	cp -a /etc/titan.restore/mnt /
fi

titan /mnt/config/titan.cfg

