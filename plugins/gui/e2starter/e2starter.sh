#! /bin/sh
#

echo start e2starter
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/var/swap/lib:/mnt/swapextensions/lib:/var/lib
export PATH=$PATH:/var/swap/bin:/mnt/swapextensions/bin:/var/bin

e2.sh

echo close e2starter