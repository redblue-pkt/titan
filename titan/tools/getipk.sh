#!/bin/sh
IPKDIR=$1
. "$HOME"/flashimg/config/make-config

cd "$HOME"/flashimg/source.titan/titan/tools

echo getipkg extensions.hpatch 1
cat ../extensions.h | sed s!/svn/ipk/sh4/titan!$IPKDIR! > extensions.h.tmp
mv extensions.h.tmp ../extensions.h

echo getipkg ipkg.h patch 1
cat ../ipkg.h | sed s!//97.74.32.10/svn/ipk/sh4/titan!http://$URLSECRET$IPKDIR/Packages.gz! > ipkg.h.tmp
mv ipkg.h.tmp ../ipkg.h

echo getipkg ipkg.h patch 2
cat ../ipkg.h | sed s!97.74.32.10!$URLSECRET! > ipkg.h.tmp
mv ipkg.h.tmp ../ipkg.h