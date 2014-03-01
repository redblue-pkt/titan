#!/bin/sh

SVNUSER=$1
GROUP=$2
TYPE=update

if [ -z "$1" ]; then
	echo "usage: createpo.sh <svnuser> <group>"
	exit
fi

if [ -z "$2" ]; then
	echo "usage: createpo.sh <svnuser> <group>"
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

cat "$HOME"/ipk/source*/*/CONTROL/control | grep Section: | sort -u | sed 's!Section: !tmpstr = _("!g' | sed 's!Package:!\nPackage!g' | grep ^tmpstr | tr '\n' '#' | sed 's!#!");\n!g' >>"$HOME"/flashimg/source.titan/titan/tools/tmp/tpk_section.h
cat "$HOME"/ipk/source*/*/CONTROL/control | grep Showname: | sort -u | sed 's!Showname: !tmpstr = _("!g' | sed 's!Package:!\nPackage!g' | grep ^tmpstr | tr '\n' '#' | sed 's!#!");\n!g' >>"$HOME"/flashimg/source.titan/titan/tools/tmp/tpk_showname.h
cat "$HOME"/ipk/source*/*/CONTROL/control | grep Description: | sort -u | sed 's!Description: !tmpstr = _("!g' | sed 's!Package:!\nPackage!g' | grep ^tmpstr | tr '\n' '#' | sed 's!#!");\n!g' >>"$HOME"/flashimg/source.titan/titan/tools/tmp/tpk_description.h
cat "$HOME"/flashimg/source.titan/skins/tithek/tithekmainmenu/*.list | grep -v internettv | cut -d"#" -f1 | sort -u | sed -e 's/^/tmpstr = _("/' | tr '\n' '#' | sed 's!#!");\n!g' >>"$HOME"/flashimg/source.titan/titan/tools/tmp/tithek_mainmenu.h
cat /var/www/atemio/web/mediathek/*/*.category.list  | cut -d"#" -f1 | sort -u | sed -e 's/^/tmpstr = _("/' | grep -v link= | tr '\n' '#' | sed 's!#!");\n!g' >>"$HOME"/flashimg/source.titan/titan/tools/tmp/tithek_submenu.h

error=0

for ROUND in $POLIST; do
	echo "[createpo.sh] ############################ start ###############################"
	echo "[createpo.sh] update $ROUND"
	echo xgettext --omit-header -k_ *.* -o $ROUND
	if [ "$TYPE" == "update" ]; then
		ROUND_CLEAN=`echo $ROUND | sed 's!titan.po_auto.po!titan.po_auto.clean.po!'`
		ROUND_UTF=`echo $ROUND | sed 's!titan.po_auto.po!titan.po_auto.utf.po!'`
		OUTFILE_MO=`echo $ROUND | sed 's!titan.po_auto.po!titan.mo!'`
		OUTFILE_PO=`echo $ROUND | sed 's!titan.po_auto.po!titan.outfile.po!'`
		OUTFILE_ERROR=`echo $ROUND | sed 's!titan.po_auto.po!titan.outfile.po.error!'`
		OUTFILE_ERROR_AUTO=`echo $ROUND | sed 's!titan.po_auto.po!titan.outfile.po.auto.error!'`
		ROUND_EDIT=`echo $ROUND | sed 's!titan.po_auto.po!titan.po!'`
		ROUND_EDIT_UTF=`echo $ROUND | sed 's!titan.po_auto.po!titan.utf.po!'`
		ROUND_MERGE_UTF=`echo $ROUND | sed 's!titan.po_auto.po!titan.merge.utf.po!'`
		ROUND_MERGE=`echo $ROUND | sed 's!titan.po_auto.po!titan.merge.po!'`
		ROUND_NEW=`echo $ROUND | sed 's!titan.po_auto.po!titan.new.po!'`
		ROUND_NEW_MERGE=`echo $ROUND | sed 's!titan.po_auto.po!titan.new.merge.po!'`

		cat $ROUND | sed '/#.*/d' > $ROUND_CLEAN
		iconv -f ISO-8859-1 -t UTF-8 $ROUND_CLEAN > $ROUND_UTF
		ret=$?
		if [ $ret -eq 0 ] || [ ! -e "$ROUND_UTF" ]; then error="$error $ROUND_UTF";fi

		xgettext --omit-header -j -k_ *.* -o $ROUND_UTF
		ret=$?
		if [ $ret -eq 0 ] || [ ! -e "$ROUND_UTF" ]; then error="$error $ROUND_UTF";fi

		echo "[createpo.sh] xgettext --omit-header -k_ *.* -o $ROUND_NEW"
		xgettext --omit-header -k_ *.* -o $ROUND_NEW
		ret=$?
		if [ $ret -eq 0 ] || [ ! -e "$ROUND_NEW" ]; then error="$error $ROUND_NEW";fi

		echo "[createpo.sh] msgmerge $ROUND_UTF $ROUND_NEW > $ROUND_NEW_MERGE"
		msgmerge $ROUND_UTF $ROUND_NEW > $ROUND_NEW_MERGE
		ret=$?
		if [ $ret -eq 0 ] || [ ! -e "$ROUND_NEW_MERGE" ]; then error="$error $ROUND_NEW_MERGE";fi

		iconv -f ISO-8859-1 -t UTF-8 $ROUND_EDIT > $ROUND_EDIT_UTF
		ret=$?
		if [ $ret -eq 0 ] || [ ! -e "$ROUND_EDIT_UTF" ]; then error="$error $ROUND_EDIT_UTF";fi

		echo "[createpo.sh] msgmerge $ROUND_NEW_MERGE $ROUND_EDIT_UTF > $ROUND_MERGE_UTF"
		msgmerge $ROUND_NEW_MERGE $ROUND_EDIT_UTF > $ROUND_MERGE_UTF
		ret=$?
		if [ $ret -eq 0 ] || [ ! -e "$ROUND_MERGE_UTF" ]; then error="$error $ROUND_MERGE_UTF";fi

		iconv -f UTF-8 -t ISO-8859-1 $ROUND_MERGE_UTF > $ROUND_MERGE
		ret=$?
		if [ $ret -eq 0 ] || [ ! -e "$ROUND_MERGE" ]; then error="$error $ROUND_MERGE";fi

		SEARCH=`cat $ROUND_MERGE | grep -n "Content-Transfer-Encoding: 8bit" | cut -d":" -f1`

		echo "[createpo.sh] SEARCH $SEARCH"
		CUT=`expr $SEARCH + 1`
		echo "[createpo.sh] CUT $CUT"

		cat $ROUND_MERGE | sed "1,"$CUT"d" > $OUTFILE_PO
		ret=$?
		if [ $ret -eq 0 ] || [ ! -e "$OUTFILE_PO" ]; then error="$error $OUTFILE_PO";fi

		echo "[createpo.sh] msgfmt -v $OUTFILE_PO -o $OUTFILE_MO"
		msgfmt -v $OUTFILE_PO -o $OUTFILE_MO		
		ret=$?
		if [ $ret -eq 0 ] || [ ! -e "$OUTFILE_MO" ]; then error="$error $OUTFILE_MO";fi
		
		iconv -f UTF-8 -t ISO-8859-1 $ROUND_NEW_MERGE > $ROUND
		ret=$?
		if [ $ret -eq 0 ] || [ ! -e "$ROUND" ]; then error="$error $ROUND";fi

		if [ ! -e $OUTFILE_MO ];then
			cp -a $OUTFILE_PO $OUTFILE_ERROR
			cp -a $OUTFILE_PO $OUTFILE_ERROR_AUTO
			error="$error $OUTFILE_MO"
		fi

	else
		xgettext --omit-header -k_ *.* -o $ROUND
	fi
	echo "[createpo.sh] ############################# end ##################################"
done

echo "[createpo.sh] check user $SVNUSER"
echo "[createpo.sh] check group $GROUP"

cd "$HOME"/flashimg/source.titan/po
if [ "$SVNUSER" = "aafsvn" ] && [ "$GROUP" = "dev" ] && [ "$error" = "0" ];then
	echo "[createpo.sh] svn commit -m [titan] autoupdate po files"
	svn commit -m "[titan] autoupdate po files"
	svn commit "$HOME"/flashimg/source.titan/po
else
	echo "[createpo.sh] error: $error"
fi