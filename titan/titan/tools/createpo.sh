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
	echo "[createpo.sh] ############################ start ###############################"
	echo "[createpo.sh] update $ROUND"
	echo xgettext --omit-header -k_ *.* -o $ROUND
	if [ "$2" == "update" ]; then
		ROUND_CLEAN=`echo $ROUND | sed 's!titan.po_auto.po!titan.po_auto.clean.po!'`
		ROUND_UTF=`echo $ROUND | sed 's!titan.po_auto.po!titan.po_auto.utf.po!'`
		OUTFILE_MO=`echo $ROUND | sed 's!titan.po_auto.po!titan.mo!'`
		OUTFILE_PO=`echo $ROUND | sed 's!titan.po_auto.po!titan.outfile.po!'`
		ROUND_EDIT=`echo $ROUND | sed 's!titan.po_auto.po!titan.po!'`
		ROUND_EDIT_UTF=`echo $ROUND | sed 's!titan.po_auto.po!titan.utf.po!'`
		ROUND_MERGE_UTF=`echo $ROUND | sed 's!titan.po_auto.po!titan.merge.utf.po!'`
		ROUND_MERGE=`echo $ROUND | sed 's!titan.po_auto.po!titan.merge.po!'`
		ROUND_NEW=`echo $ROUND | sed 's!titan.po_auto.po!titan.new.po!'`
		ROUND_NEW_MERGE=`echo $ROUND | sed 's!titan.po_auto.po!titan.new.merge.po!'`

		cat $ROUND | sed '/#.*/d' > $ROUND_CLEAN
		iconv -f ISO-8859-1 -t UTF-8 $ROUND_CLEAN > $ROUND_UTF
		xgettext --omit-header -j -k_ *.* -o $ROUND_UTF

		echo "[createpo.sh] xgettext --omit-header -k_ *.* -o $ROUND_NEW"
		xgettext --omit-header -k_ *.* -o $ROUND_NEW
		echo "[createpo.sh] msgmerge $ROUND_UTF $ROUND_NEW > $ROUND_NEW_MERGE"
		msgmerge $ROUND_UTF $ROUND_NEW > $ROUND_NEW_MERGE

		iconv -f ISO-8859-1 -t UTF-8 $ROUND_EDIT > $ROUND_EDIT_UTF
		echo "[createpo.sh] msgmerge $ROUND_NEW_MERGE $ROUND_EDIT_UTF > $ROUND_MERGE_UTF"
		msgmerge $ROUND_NEW_MERGE $ROUND_EDIT_UTF > $ROUND_MERGE_UTF
		iconv -f UTF-8 -t ISO-8859-1 $ROUND_MERGE_UTF > $ROUND_MERGE
		
		SEARCH=`cat $ROUND_MERGE | grep -n "Content-Transfer-Encoding: 8bit" | cut -d":" -f1`

		echo "[createpo.sh] SEARCH $SEARCH"
		CUT=`expr $SEARCH + 1`
		echo "[createpo.sh] CUT $CUT"

		cat $ROUND_MERGE | sed "1,"$CUT"d" > $OUTFILE_PO

		echo "[createpo.sh] msgfmt -v $OUTFILE_PO -o $OUTFILE_MO"
		msgfmt -v $OUTFILE_PO -o $OUTFILE_MO		

		iconv -f UTF-8 -t ISO-8859-1 $ROUND_NEW_MERGE > $ROUND
	else
		xgettext --omit-header -k_ *.* -o $ROUND
	fi
	echo "[createpo.sh] ############################# end ##################################"
done

echo "[createpo.sh] check user"
		
cd "$HOME"/flashimg/source.titan/po
if [ $SVNUSER = "aafsvn" ];then
	echo "[createpo.sh] svn commit -m [titan] autoupdate po files"
	svn commit -m "[titan] autoupdate po files"
	svn commit "$HOME"/flashimg/source.titan/po
fi