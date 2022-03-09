#! /bin/sh
#

echo start gmediarender
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/var/swap/lib:/mnt/swapextensions/lib:/var/lib
export PATH=$PATH:/var/swap/bin:/mnt/swapextensions/bin:/var/bin

if [ -e /.gstreamer-0.10 ];then
	gmediarender -f $1 --gst-debug-disable
else
	gmediarender -f $1
fi
echo close gmediarender