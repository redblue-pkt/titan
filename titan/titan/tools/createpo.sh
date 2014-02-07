#!/bin/sh

SVNUSER=$1

if [ -z "$1" ]; then
	echo "usage: createpo.sh <svnuser> <update|new>"
	exit
fi

if [ -z "$2" ]; then
	echo "usage: createpo.sh <svnuser> <update|new>"
	exit
fi

rm -rf "$HOME"/flashimg/source.titan/titan/tools/tmp
mkdir -p "$HOME"/flashimg/source.titan/titan/tools/tmp

LIST=`find "$HOME"/flashimg/source.titan/titan "$HOME"/flashimg/source.titan/plugins -type f -name "*.h"`
LIST="$LIST $HOME/flashimg/source.titan/titan/titan.c"
POLIST=`find "$HOME"/flashimg/source.titan/po -type f -name "*_auto.po"`
SKINLIST=`find "$HOME"/flashimg/source.titan -type f -name "*kin.xml"`

for ROUND in $LIST; do
	cp -a $ROUND "$HOME"/flashimg/source.titan/titan/tools/tmp
done
cd "$HOME"/flashimg/source.titan/titan/tools/tmp

for ROUND in $SKINLIST; do
	echo "[create.skin] update $ROUND"
	NAME=`echo $ROUND | tr "/" "\n" | tail -n1`
	SECTION1=`echo $ROUND | tr "/" "\n" | tail -n3 | head -n1`
	SECTION2=`echo $ROUND | tr "/" "\n" | tail -n2 | head -n1`

	echo "[create.skin] $SECTION1_$SECTION2_$NAME.h"
	cp "$HOME"/flashimg/source.titan/titan/tools/dummy "$HOME"/flashimg/source.titan/titan/tools/tmp/"$SECTION1"_"$SECTION2"_"$NAME".h

	cat $ROUND | grep title= | sed 's/title=/\ntitle=/' | grep ^title= | cut -d '"' -f2 | sort -u | sed '/^ *$/d' | tr '\n' '#' | sed 's/#\+/\");\ntmpstr = _(\"\ /g'| sed 's/" /"/' >>"$HOME"/flashimg/source.titan/titan/tools/tmp/"$SECTION1"_"$SECTION2"_"$NAME".h
	cat $ROUND | grep text= | sed 's/text=/\ntext=/' | grep ^text= | cut -d '"' -f2 | sort -u | sed '/^ *$/d' | tr '\n' '#' | sed 's/#\+/\");\ntmpstr = _(\"\ /g'| sed 's/" /"/' >>"$HOME"/flashimg/source.titan/titan/tools/tmp/"$SECTION1"_"$SECTION2"_"$NAME".h
done

for ROUND in $POLIST; do
	echo "[create.po] update $ROUND"
	echo xgettext --omit-header -k_ *.* -o $ROUND
	if [ "$2" == "update" ]; then
		ROUND_UTF=`echo $ROUND | sed 's!titan.po_auto.po!titan.po_auto.po.utf!'`
		iconv -f ISO-8859-1 -t UTF-8 $ROUND > $ROUND_UTF
		xgettext --omit-header -j -k_ *.* -o $ROUND_UTF
		iconv -f UTF-8 -t ISO-8859-1 $ROUND_UTF > $ROUND
	else
		xgettext --omit-header -k_ *.* -o $ROUND
	fi
done

cd "$HOME"/flashimg/source.titan/po
if [ $SVNUSER = "aafsvn" ];then
	svn commit -m "[titan] autoupdate po files"
	svn commit "$HOME"/flashimg/source.titan/po
fi