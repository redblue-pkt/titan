#!/bin/sh
TPKDIR=$1
VERSION=$2

. "$HOME"/flashimg/config/make-config

cd "$HOME"/flashimg/source.titan/titan/tools

echo gettpkg extensions.h patch 1
cat ../extensions.h | sed s!/svn/tpk/sh4!$TPKDIR! > extensions.h.tmp
mv extensions.h.tmp ../extensions.h

echo gettpkg tpk.h patch 1
cat ../tpk.h | sed s!svn/tpk/sh4!$TPKDIR! > tpk.h.tmp
mv tpk.h.tmp ../tpk.h

echo gettpk tpk.h patch 2
cat ../tpk.h | sed s!97.74.32.10!$URLSECRET! > tpk.h.tmp
mv tpk.h.tmp ../tpk.h

echo add pluginversionsnummer to: $VERSION
cat ../struct.h | sed s/"^#define PLUGINVERSION 0"/"#define PLUGINVERSION $VERSION"/ > struct.h.tmp
mv struct.h.tmp ../struct.h