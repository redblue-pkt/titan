#!/bin/sh
IPKDIR=$1

cd "$HOME"/flashimg/source.titan/titan/tools

cat ../../plugins/panel/panel_feed.h | sed s!/svn/ipk/sh4/titan!$IPKDIR! > panel_feed.h.tmp
mv panel_feed.h.tmp ../../plugins/panel/panel_feed.h

cat ../global.h | sed s!97.74.32.10/svn/ipk/sh4/titan!$IPKDIR! > global.h.tmp
mv global.h.tmp ../global.h