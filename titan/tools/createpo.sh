#!/bin/sh

if [ -z "$1" ]; then
	echo "usage: createpo.sh <update|new>"
	exit
fi

rm -rf "$HOME"/flashimg/source.titan/titan/tools/tmp
mkdir -p "$HOME"/flashimg/source.titan/titan/tools/tmp

LIST=`find "$HOME"/flashimg/source.titan -type f -name "*.h"`
POLIST=`find "$HOME"/flashimg/source.titan/po -type f -name "*_auto.po"`
SKINLIST=`find "$HOME"/flashimg/source.titan -type f -name "*kin.xml"`


for ROUND in $LIST; do
	cp -a $ROUND "$HOME"/flashimg/source.titan/titan/tools/tmp
done
cd "$HOME"/flashimg/source.titan/titan/tools/tmp

for ROUND in $SKINLIST; do
	echo "[create.skin] update $ROUND"
	cp "$HOME"/flashimg/source.titan/titan/tools/dummy "$HOME"/flashimg/source.titan/titan/tools/tmp/"$SECTION1"_"$SECTION2"_"$NAME".h
	NAME=`echo $ROUND | tr "/" "\n" | tail -n1`
	SECTION1=`echo $ROUND | tr "/" "\n" | tail -n3 | head -n1`
	SECTION2=`echo $ROUND | tr "/" "\n" | tail -n2 | head -n1`
	echo SECTION1 $SECTION1
	echo SECTION2 $SECTION2
	echo NAME $NAME

	cat $ROUND | grep title= | sed 's/title=/\ntitle=/' | grep ^title= | cut -d '"' -f2 | sort -u | sed '/^ *$/d' | tr '\n' '#' | sed 's/#\+/\");\ntmpstr = _(\"\ /g'| sed 's/" /"/' >>"$HOME"/flashimg/source.titan/titan/tools/tmp/"$SECTION1"_"$SECTION2"_"$NAME".h
	cat $ROUND | grep text= | sed 's/text=/\ntext=/' | grep ^text= | cut -d '"' -f2 | sort -u | sed '/^ *$/d' | tr '\n' '#' | sed 's/#\+/\");\ntmpstr = _(\"\ /g'| sed 's/" /"/' >>"$HOME"/flashimg/source.titan/titan/tools/tmp/"$SECTION1"_"$SECTION2"_"$NAME".h
done

for ROUND in $POLIST; do
	echo "[create.po] update $ROUND"
	if [ "$1" == "update" ]; then
		xgettext --omit-header -j -k_ *.h -o $ROUND
	else
		xgettext --omit-header -k_ *.h -o $ROUND
	fi
done

cd "$HOME"/flashimg/source.titan/po
svn commit -m "[titan] autoupdate po files"
svn commit "$HOME"/flashimg/source.titan/po