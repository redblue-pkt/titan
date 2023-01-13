#! /bin/sh
#

echo start gmediarender
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/var/swap/lib:/mnt/swapextensions/lib:/var/lib
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/var/swap/usr/lib:/mnt/swapextensions/usr/lib:/var/usr/lib
export PATH=$PATH:/var/swap/bin:/mnt/swapextensions/bin:/var/bin
export PATH=$PATH:/var/swap/usr/bin:/mnt/swapextensions/usr/bin:/var/usr/bin

if [ -e /etc/.oebuild ] || [ -e /etc/.ovbuild ];then
	check=`opkg list_installed titan-gmediarender | wc -l`
	echo "check: $check"
	if [ "$check" == 0 ];then
		opkg install titan-gmediarender --force-reinstall
	fi
	check=`opkg list_installed libupnp | wc -l`
	echo "check: $check"
	if [ "$check" == 0 ];then
		opkg install libupnp --force-reinstall
	fi
fi

if [ -e /.gstreamer-0.10 ];then
	gmediarender -f $1 --gst-debug-disable
else
	gmediarender -f $1
fi
echo close gmediarender
