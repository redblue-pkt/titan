#! /bin/sh
#
echo start gmediarender
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/var/swap/lib
gmediarender -f $1 --gst-debug-disable
echo close gmediarender