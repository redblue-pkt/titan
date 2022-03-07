#! /bin/sh
#

echo start kodistarter
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/var/swap/lib:/mnt/swapextensions/lib:/var/lib
export PATH=$PATH:/var/swap/bin:/mnt/swapextensions/bin:/var/bin

kodi.sh

echo close kodistarter