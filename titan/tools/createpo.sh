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
rm -f "$HOME"/flashimg/source.titan/titan/tools/error/error.log

for ROUND in $POLIST; do
	echo "[createpo.sh] ############################ start ###############################"
	echo "[createpo.sh] update $ROUND"
	echo xgettext --omit-header -k_ *.* -o $ROUND
	if [ "$TYPE" == "update" ]; then
	old=0
		if [ $old = 1 ];then
			ROUND_CLEAN=`echo $ROUND | sed 's!titan.po_auto.po!titan.po_auto.clean.po!'`
			ROUND_UTF=`echo $ROUND | sed 's!titan.po_auto.po!titan.po_auto.utf.po!'`
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
			if [ ! -e "$ROUND_UTF" ] || [ `cat "$ROUND_UTF" | wc -l` -eq 0 ]; then error="1"; break;fi
	
			echo "[createpo.sh] xgettext --omit-header -j -k_ *.* -o $ROUND_UTF"
	#		rm -f "$HOME"/flashimg/source.titan/titan/tools/error/error.log
			cmd="xgettext --omit-header -j -k_ *.* -o $ROUND_UTF"
			echo "--------------------------------------" >> "$HOME"/flashimg/source.titan/titan/tools/error/error.log
			echo "[createpo.sh] $cmd" > "$HOME"/flashimg/source.titan/titan/tools/error/error.log
			$cmd >> "$HOME"/flashimg/source.titan/titan/tools/error/error.log 2>&1
			log=`cat "$HOME"/flashimg/source.titan/titan/tools/error/error.log`
			echo 2: $log
			if [ ! -e "$ROUND_UTF" ] || [ `cat "$ROUND_UTF" | wc -l` -eq 0 ]; then error="2"; break;fi
			if [ `echo $log | grep "fatal error" | wc -l` -gt 0 ]; then error="3"; break;fi
	
			echo "[createpo.sh] xgettext --omit-header -k_ *.* -o $ROUND_NEW"
	#		rm -f "$HOME"/flashimg/source.titan/titan/tools/error/error.log
			cmd="xgettext --omit-header -k_ *.* -o $ROUND_NEW"
			echo "--------------------------------------" >> "$HOME"/flashimg/source.titan/titan/tools/error/error.log
			echo "[createpo.sh] $cmd" >> "$HOME"/flashimg/source.titan/titan/tools/error/error.log
			$cmd >> "$HOME"/flashimg/source.titan/titan/tools/error/error.log 2>&1
			log=`cat "$HOME"/flashimg/source.titan/titan/tools/error/error.log`
			echo 3: $log
			if [ ! -e "$ROUND_NEW" ] || [ `cat "$ROUND_NEW" | wc -l` -eq 0 ]; then error="3"; break;fi
			if [ `echo $log | grep "fatal error" | wc -l` -gt 0 ]; then error="4";break;fi
	
			echo "[createpo.sh] msgmerge $ROUND_UTF $ROUND_NEW > $ROUND_NEW_MERGE"
			msgmerge $ROUND_UTF $ROUND_NEW > $ROUND_NEW_MERGE
			if [ ! -e "$ROUND_NEW_MERGE" ] || [ `cat "$ROUND_NEW_MERGE" | wc -l` -eq 0 ]; then error="5"; break;fi
	
			iconv -f ISO-8859-1 -t UTF-8 $ROUND_EDIT > $ROUND_EDIT_UTF
			if [ ! -e "$ROUND_EDIT_UTF" ] || [ `cat "$ROUND_EDIT_UTF" | wc -l` -eq 0 ]; then error="6"; break;fi
	
			echo "[createpo.sh] msgmerge $ROUND_NEW_MERGE $ROUND_EDIT_UTF > $ROUND_MERGE_UTF $ret"
			msgmerge $ROUND_NEW_MERGE $ROUND_EDIT_UTF > $ROUND_MERGE_UTF
			if [ ! -e "$ROUND_MERGE_UTF" ] || [ `cat "$ROUND_EDIT_UTF" | wc -l` -eq 0 ]; then error="7"; break;fi
	
			iconv -f UTF-8 -t ISO-8859-1 $ROUND_MERGE_UTF > $ROUND_MERGE
			if [ ! -e "$ROUND_MERGE" ] || [ `cat "$ROUND_MERGE" | wc -l` -eq 0 ]; then error="8"; break;fi
	
			SEARCH=`cat $ROUND_MERGE | grep -n "Content-Transfer-Encoding: 8bit" | cut -d":" -f1`
	
			echo "[createpo.sh] SEARCH $SEARCH"
			CUT=`expr $SEARCH + 1`
			echo "[createpo.sh] CUT $CUT"
	
			cat $ROUND_MERGE | sed "1,"$CUT"d" > $OUTFILE_PO
			if [ ! -e "$OUTFILE_PO" ] || [ `cat "$OUTFILE_PO" | wc -l` -eq 0 ]; then error="9"; break;fi
	
			echo "[createpo.sh] msgfmt -v $OUTFILE_PO -o $OUTFILE_MO"
	#		rm -f "$HOME"/flashimg/source.titan/titan/tools/error/error.log
			cmd="msgfmt -v $OUTFILE_PO -o $OUTFILE_MO"
			echo "--------------------------------------" >> "$HOME"/flashimg/source.titan/titan/tools/error/error.log
			echo "[createpo.sh] $cmd" >> "$HOME"/flashimg/source.titan/titan/tools/error/error.log
			$cmd >> "$HOME"/flashimg/source.titan/titan/tools/error/error.log 2>&1
			log=`cat "$HOME"/flashimg/source.titan/titan/tools/error/error.log`
			echo 9: $log
			if [ ! -e "$OUTFILE_MO" ] || [ `cat "$OUTFILE_MO" | wc -l` -eq 0 ]; then error="10"; break;fi
			if [ `echo $log | grep "fatal error" | wc -l` -gt 0 ]; then error="11"; break;fi
			
			iconv -f UTF-8 -t ISO-8859-1 $ROUND_NEW_MERGE > $ROUND
			if [ ! -e "$ROUND" ] || [ `cat "$ROUND" | wc -l` -eq 0 ]; then error="12"; break;fi
	
			if [ ! -e $OUTFILE_MO ];then
				cp -a $OUTFILE_PO $OUTFILE_ERROR
				cp -a $OUTFILE_PO $OUTFILE_ERROR_AUTO
				error="13"
			fi
		else
			OUTFILE_MO=`echo $ROUND | sed 's!titan.po_auto.po!titan.mo!'`
			OUTFILE_PO=`echo $ROUND | sed 's!titan.po_auto.po!titan.outfile.po!'`
#			OUTFILE_PO=$ROUND

			echo "[createpo.sh] iconv -f UTF-8 -t ISO-8859-1 $ROUND > $OUTFILE_PO"
			iconv -f UTF-8 -t ISO-8859-1 $ROUND > $OUTFILE_PO
			if [ ! -e "$OUTFILE_PO" ] || [ `cat "$OUTFILE_PO" | wc -l` -eq 0 ]; then error="1"; break;fi

			echo "[createpo.sh] msgfmt -v $OUTFILE_PO -o $OUTFILE_MO"
	#		rm -f "$HOME"/flashimg/source.titan/titan/tools/error/error.log
			cmd="msgfmt -v $OUTFILE_PO -o $OUTFILE_MO"
			echo "--------------------------------------" >> "$HOME"/flashimg/source.titan/titan/tools/error/error.log
			echo "[createpo.sh] $cmd" >> "$HOME"/flashimg/source.titan/titan/tools/error/error.log
			$cmd >> "$HOME"/flashimg/source.titan/titan/tools/error/error.log 2>&1
			log=`cat "$HOME"/flashimg/source.titan/titan/tools/error/error.log`
			echo 1: $log
			if [ ! -e "$OUTFILE_MO" ] || [ `cat "$OUTFILE_MO" | wc -l` -eq 0 ]; then error="2"; break;fi
			if [ `echo $log | grep "fatal error" | wc -l` -gt 0 ]; then error="3"; break;fi
			
#			iconv -f UTF-8 -t ISO-8859-1 $ROUND_NEW_MERGE > $ROUND
#			if [ ! -e "$ROUND" ] || [ `cat "$ROUND" | wc -l` -eq 0 ]; then error="4"; break;fi

		fi
	else
		xgettext --omit-header -k_ *.* -o $ROUND
	fi
	echo "[createpo.sh] ############################# end ##################################"
done

if [ $error != 0 ];then
	echo "[createpo.sh] found error($error)"
fi

echo "[createpo.sh] check user $SVNUSER"
echo "[createpo.sh] check group $GROUP"
	
cd "$HOME"/flashimg/source.titan/po
if [ "$SVNUSER" = "aafsvn" ] && [ "$GROUP" = "dev" ] && [ "$error" = "0" ];then
	echo "[createpo.sh] svn commit -m [titan] autoupdate po files"
	svn commit -m "[titan] autoupdate po files"
	svn commit "$HOME"/flashimg/source.titan/po
elif [ "$SVNUSER" = "aafsvn" ] && [ "$GROUP" = "dev" ];then
	echo "[createpo.sh] svn commit -m [titan] ERROR autoupdate po files"
	
	cp -a "$HOME"/flashimg/source.titan/titan/tools/error/error.log "$HOME"/flashimg/source.titan/titan/tools/error/create_po_error_code
	
	LINE=`cat "$HOME"/flashimg/source.titan/titan/tools/error/error.log | grep -n "fatal error" | cut -d: -f1`
	if [ ! -z "$LINE" ];then
		LINE=`expr $LINE - 1`
		FILE=`cat "$HOME"/flashimg/source.titan/titan/tools/error/error.log | sed -ne ""$LINE"p" | cut -d: -f1`
		LINE=`cat "$HOME"/flashimg/source.titan/titan/tools/error/error.log | sed -ne ""$LINE"p" | cut -d: -f2`
		LINE=`expr $LINE - 1`
		ERROR=`cat "$FILE" | sed -ne ""$LINE"p"`
		echo "$ERROR" >> "$HOME"/flashimg/source.titan/titan/tools/error/create_po_error_code
	fi

	cd "$HOME"/flashimg/source.titan/titan/tools/error
	svn commit -m "[titan] ERROR autoupdate po files"
	svn commit "$HOME"/flashimg/source.titan/titan/tools/error/create_po_error_code
else
	echo "[createpo.sh] skip: svn commit"
fi