#! /bin/sh
#

#rm -rf /var/usr/lib/gstreamer-0.10
#ln -s /var/swap/lib/gstreamer-0.10 /var/usr/lib
#rm -rf /var/usr/lib/directfb-1.4-5
#ln -s /var/swap/lib/directfb-1.4-5 /var/usr/lib

echo start gmediarender
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/var/swap/lib
gmediarender -f $1 --gst-debug-disable
echo close gmediarender