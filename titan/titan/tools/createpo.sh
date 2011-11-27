#!/bin/sh

if [ -z "$1" ]; then
	echo "usage: createpo.sh <update|new>"
	exit
fi

rm -rf "$HOME"/flashimg/source.titan/titan/tools/tmp
mkdir -p "$HOME"/flashimg/source.titan/titan/tools/tmp

LIST=`find "$HOME"/flashimg/source.titan -type f -name "*.h"`
POLIST=`find "$HOME"/flashimg/source.titan/po -type f -name "*.po"`



for ROUND in $LIST; do
	cp -a $ROUND "$HOME"/flashimg/source.titan/titan/tools/tmp
done
cd "$HOME"/flashimg/source.titan/titan/tools/tmp
	
for ROUND in $POLIST; do
	echo "[create.po] update $ROUND"
	if [ "$1" == "update" ]; then
		xgettext -j -k_ *.h -o $ROUND
	else
		xgettext -k_ *.h -o "$ROUND"
	fi
done

cd "$HOME"/flashimg/source.titan
svn commit "autoupdate po files" "$HOME"/flashimg/source.titan/po