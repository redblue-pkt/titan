#!/bin/bash

SVNUSER=$1
GROUP=$2
TYPE=update
SRCDIR=$3
BOXNAME=$4
CREATE=$5

if [ -z "$1" ]; then
	echo "[createpo.sh] usage: createpo.sh <svnuser> <update|new>"
	exit
fi

if [ -z "$2" ]; then
	echo "[createpo.sh] usage: createpo.sh <svnuser> <update|new>"
	exit
fi

rm -rf "$HOME"/flashimg/$SRCDIR/titan/tools/tmp
mkdir -p "$HOME"/flashimg/$SRCDIR/titan/tools/tmp

LIST=`find "$HOME"/flashimg/$SRCDIR/titan "$HOME"/flashimg/$SRCDIR/plugins -type f -name "*.h"`
LIST="$LIST "`find "$HOME"/flashimg/$SRCDIR/titan "$HOME"/flashimg/$SRCDIR/plugins -type f -name "*.c"`
POLIST=`find "$HOME"/flashimg/$SRCDIR/po -type f -name "*_auto.po"`
SKINLIST=`find "$HOME"/flashimg/$SRCDIR -type f -name "*kin.xml"`
#SKINLIST="$SKINLIST "`find "$HOME"/ipk/source* -type f -name "*kin.xml"`
HLIST=`find "$HOME"/flashimg/$SRCDIR/web -type f -name "*.html"`

##aus /plugins alle *.h und *.c > tmp
for ROUND in $LIST; do
	cp -a $ROUND "$HOME"/flashimg/$SRCDIR/titan/tools/tmp
done

for ROUND in $HLIST; do
	FILENAME=`echo $ROUND | sed 's/.html//g' | tr '/' '\n' | tail -n1`
	echo "[createpo] webif update $FILENAME from $ROUND"
	cat $ROUND | sed 's/_(/\ntmpstr = _(/g' | grep ^"tmpstr = _(" | sed 's/").*/");/g' >> "$HOME"/flashimg/$SRCDIR/titan/tools/tmp/webif_$FILENAME.h
done

cd "$HOME"/flashimg/$SRCDIR/titan/tools/tmp

for ROUND in $SKINLIST; do
	##echo "[createpo] skin update $ROUND"
	NAME=`echo $ROUND | tr "/" "\n" | tail -n1`
	SECTION1=`echo $ROUND | tr "/" "\n" | tail -n3 | head -n1`
	SECTION2=`echo $ROUND | tr "/" "\n" | tail -n2 | head -n1`

	echo "[createpo] skin update" "$SECTION1"_"$SECTION2"_"$NAME".h from "$ROUND"
	cp "$HOME"/flashimg/$SRCDIR/titan/tools/dummy "$HOME"/flashimg/$SRCDIR/titan/tools/tmp/"$SECTION1"_"$SECTION2"_"$NAME"

	## dummy = 'tmpstr = _("'
	cat $ROUND | sed 's/\x0D$//' | grep text= | sed 's/text=/\ntext=/' | grep ^text= | grep -v ^text=\' | cut -d '"' -f2 | sort -u | sed '/^ *$/d' | tr '\n' '#' | sed 's/#\+/\");\ntmptext = _(\"\ /g'| sed 's/" /"/' >>"$HOME"/flashimg/$SRCDIR/titan/tools/tmp/"$SECTION1"_"$SECTION2"_"$NAME"
	cat $ROUND | sed 's/\x0D$//' | grep title= | sed 's/title=/\ntitle=/' | grep ^title= | cut -d '"' -f2 | sort -u | sed '/^ *$/d' | tr '\n' '#' | sed 's/#\+/\");\ntmptitle = _(\"\ /g'| sed 's/" /"/' >>"$HOME"/flashimg/$SRCDIR/titan/tools/tmp/"$SECTION1"_"$SECTION2"_"$NAME"
	## Reihenfolge getauscht wegen einigen Files ohne 'title='
	## Letzte Zeile = 'tmpstr = _("' und das führt zu 'msgid = ""' !!
	## weg damit:
	cat "$HOME"/flashimg/$SRCDIR/titan/tools/tmp/"$SECTION1"_"$SECTION2"_"$NAME"  | grep ';' > "$HOME"/flashimg/$SRCDIR/titan/tools/tmp/"$SECTION1"_"$SECTION2"_"$NAME".h
	rm "$HOME"/flashimg/$SRCDIR/titan/tools/tmp/"$SECTION1"_"$SECTION2"_"$NAME" 
done

cat "$HOME"/ipk/source*/*/CONTROL/control | sed 's/\x0D$//' | grep Section: | sort -u | sed 's!Section: !tmpstr = _("!g' | sed 's!Package:!\nPackage!g' | grep ^tmpstr | tr '\n' '#' | sed 's!#!");\n!g' >>"$HOME"/flashimg/$SRCDIR/titan/tools/tmp/tpk_section.h
cat "$HOME"/ipk/source*/*/CONTROL/control | sed 's/\x0D$//' | grep Showname: | sort -u | sed 's!Showname: !tmpstr = _("!g' | sed 's!Package:!\nPackage!g' | grep ^tmpstr | tr '\n' '#' | sed 's!#!");\n!g' >>"$HOME"/flashimg/$SRCDIR/titan/tools/tmp/tpk_showname.h
cat "$HOME"/ipk/source*/*/CONTROL/control | sed 's/\x0D$//' | grep Description: | sort -u | sed 's!Description: !tmpstr = _("!g' | sed 's!Package:!\nPackage!g' | grep ^tmpstr | tr '\n' '#' | sed 's!#!");\n!g' >>"$HOME"/flashimg/$SRCDIR/titan/tools/tmp/tpk_description.h
cat "$HOME"/flashimg/$SRCDIR/mediathek/tithekmainmenu/*.list | sed 's/\x0D$//' | grep -v internettv | cut -d"#" -f1 | sort -u | sed -e '/^\s*$/d' | sed -e 's/^/tmpstr = _("/' | tr '\n' '#' | sed 's!#!");\n!g' >>"$HOME"/flashimg/$SRCDIR/titan/tools/tmp/tithek_mainmenu.h
#cat /var/www/atemio/web/mediathek/*/*.category.list  | cut -d"#" -f1 | sort -u | sed -e 's/^/tmpstr = _("/' | grep -v link= | grep -v title= | tr '\0' '#' | tr '\n' '#' | sed 's!#!");\n!g' >>"$HOME"/flashimg/$SRCDIR/titan/tools/tmp/tithek_submenu.h
ls "$HOME"/flashimg/$SRCDIR/help/de/ | sed 's/.txt/");/g' | sed 's/^/tmpstr = _("/g' >> "$HOME"/flashimg/$SRCDIR/titan/tools/tmp/webif_help.h

file --mime-encoding "$HOME"/flashimg/$SRCDIR/po/*/*/*.po >> "$HOME"/flashimg/$SRCDIR/error/coding.log 2>&1
##schreibt den mime-type ('text/plain; charset=us-ascii') in die coding.log

error=0

for ROUND in $POLIST; do
	echo "[createpo.sh] ############################ start ###############################"
	##echo "[createpo.sh] update $ROUND"
#	echo xgettext --omit-header -k_ *.* -o $ROUND
	if [ "$TYPE" == "update" ]; then
		ROUND_CLEAN=`echo $ROUND | sed 's!titan.po_auto.po!titan.po_auto.clean.po!'`
		#ROUND_UTF=`echo $ROUND | sed 's!titan.po_auto.po!titan.po_auto.utf.po!'`
		OUTFILE_MO=`echo $ROUND | sed 's!titan.po_auto.po!titan.mo!'`
		OUTFILE_PO=`echo $ROUND | sed 's!titan.po_auto.po!titan.outfile.po!'`
		ROUND_EDIT=`echo $ROUND | sed 's!titan.po_auto.po!titan.po!'`
		#ROUND_EDIT_UTF=`echo $ROUND | sed 's!titan.po_auto.po!titan.utf.po!'`
		#ROUND_MERGE_UTF=`echo $ROUND | sed 's!titan.po_auto.po!titan.merge.utf.po!'`
		#ROUND_MERGE=`echo $ROUND | sed 's!titan.po_auto.po!titan.merge.po!'`
		ROUND_NEW=`echo $ROUND | sed 's!titan.po_auto.po!titan.new.po!'`
		ROUND_NEW_MERGE=`echo $ROUND | sed 's!titan.po_auto.po!titan.new.merge.po!'`
		

		##Aus der titan.po_auto.po alle Kommentare löschen und in titan.po_auto.clean.po speichern
		echo "[createpo.sh] update $ROUND "`stat -c=%y "$ROUND" `
		cat $ROUND | sed '/#.*/d' > $ROUND_CLEAN			
	
		if [ ! -e "$ROUND_CLEAN" ] || [ `cat "$ROUND_CLEAN" | wc -l` -eq 0 ]; then error="1"; break;fi

		##cmd="xgettext --omit-header -j -k_ *.* -o $ROUND_UTF"
		##Der Parameter --no-wrap verhindert ein ändern der Zeilenlänge. Eine Fehlerursache weniger
		cmd="xgettext --omit-header --no-wrap -j -k_ *.* -o $ROUND_CLEAN"
		echo "[createpo.sh] $cmd" >> "$HOME"/flashimg/$SRCDIR/error/po.log		
		$cmd >> "$HOME"/flashimg/$SRCDIR/error/po.log 2>&1
		if [ ! -e "$ROUND_CLEAN" ] || [ `cat "$ROUND_CLEAN" | wc -l` -eq 0 ]; then error="3"; break;fi
		log=`cat "$HOME"/flashimg/$SRCDIR/error/po.log`
		if [ `echo $log | grep "fatal error" | wc -l` -gt 0 ]; then error="4"; break;fi
		##Nun haben wir schon ALLE neuen msgid's mit drin! > *.* sammelt alle neuen Einträge, -j sorgt für das Zusammenfügen 
		##enthält aber noch alte (nicht mehr verwendete) msgid's
		
		##-s sortierte Ausgabe erstellen. Bringt die msgid "" an den Anfang!
		cmd="xgettext --omit-header -s -k_ *.* -o $ROUND_NEW"
		echo "[createpo.sh] $cmd" >> "$HOME"/flashimg/$SRCDIR/error/po.log
		$cmd >> "$HOME"/flashimg/$SRCDIR/error/po.log 2>&1
		if [ ! -e "$ROUND_NEW" ] || [ `cat "$ROUND_NEW" | wc -l` -eq 0 ]; then error="5"; break;fi
		log=`cat "$HOME"/flashimg/$SRCDIR/error/po.log`
		if [ `echo $log | grep "fatal error" | wc -l` -gt 0 ]; then error="6";break;fi

		echo "[createpo.sh] msgmerge $ROUND_CLEAN $ROUND_NEW > $ROUND_NEW_MERGE"
		##-q Fortschrittsanzeige unterdrücken
		msgmerge -q $ROUND_CLEAN $ROUND_NEW > $ROUND_NEW_MERGE	
		if [ ! -e "$ROUND_NEW_MERGE" ] || [ `cat "$ROUND_NEW_MERGE" | wc -l` -eq 0 ]; then error="7"; break;fi

		## hier reicht eigentlich nun ROUND_NEW_MERGE, aber wozu "Content-Type:" rauslöschen?
		##cat $ROUND_CLEAN > $OUTFILE_PO
		## mit dem Eintrag Content-Type gibt msgfmt zwar keine Warnungen aus, aber titan arbeitet damit falsch!??
		cat $ROUND_NEW_MERGE | sed 's/"Content-Type:.*//g' > $OUTFILE_PO
		if [ ! -e "$OUTFILE_PO" ] || [ `cat "$OUTFILE_PO" | wc -l` -eq 0 ]; then error="11"; break;fi

		cmd="msgfmt -v $OUTFILE_PO -o $OUTFILE_MO"
		echo "[createpo.sh] $cmd" >> "$HOME"/flashimg/$SRCDIR/error/po.log
		$cmd >> "$HOME"/flashimg/$SRCDIR/error/po.log 2>&1
		if [ ! -e "$OUTFILE_MO" ] || [ `cat "$OUTFILE_MO" | wc -l` -eq 0 ]; then error="12"; break;fi
		log=`cat "$HOME"/flashimg/$SRCDIR/error/po.log`
		if [ `echo $log | grep "fatal error" | wc -l` -gt 0 ]; then error="13"; break;fi
			
		cat $ROUND_NEW_MERGE > $ROUND
		if [ ! -e "$ROUND" ] || [ `cat "$ROUND" | wc -l` -eq 0 ]; then error="14"; break;fi

		if [ ! -e $OUTFILE_MO ];then
			## wohl nie aufgetreten: $OUTFILE_ERROR/$OUTFILE_ERROR_AUTO sind nicht definiert
			cp -a $OUTFILE_PO $OUTFILE_ERROR
			cp -a $OUTFILE_PO $OUTFILE_ERROR_AUTO
			error="15"
		fi
	else
		xgettext --omit-header -k_ *.* -o $ROUND
	fi
	
	##Aus der neuen titan.po_auto.po alle Kommentare löschen und in titan.po speichern
	cat $ROUND | sed '/#.*/d' > $ROUND_EDIT
	if [ ! -e "$ROUND_EDIT" ] || [ `cat "$ROUND_EDIT" | wc -l` -eq 0 ]; then error="16"; break;fi
	
	echo "[createpo.sh] no error found ($error)"
	echo "[createpo.sh] ############################# end ##################################"
done

if [ $error != 0 ];then
	echo "[createpo.sh] found error($error)"
	echo "[createpo.sh] ###################### error log start ##########################"
	cat "$HOME"/flashimg/$SRCDIR/error/po.log
	echo "[createpo.sh] ####################### error log end ###########################"	
fi

file --mime-encoding "$HOME"/flashimg/$SRCDIR/po/*/*/*.po >> "$HOME"/flashimg/$SRCDIR/error/coding.log 2>&1

echo "[createpo.sh] check user $SVNUSER"
echo "[createpo.sh] check group $GROUP"

cd "$HOME"/flashimg/$SRCDIR/po
if [ "$SVNUSER" = "aafsvn" ] && [ "$GROUP" = "update" ] && [ "$BOXNAME" = "atemio7600" ] && [ "$error" = "0" ] && [ "$CREATE" == "1" ];then
	echo "[createpo.sh] svn commit -m [titan] autoupdate po files ($BOXNAME)"
	svn commit -m "[titan] autoupdate po files ($BOXNAME)"
	svn commit "$HOME"/flashimg/$SRCDIR/po
elif [ "$SVNUSER" = "aafsvn" ] && [ "$GROUP" = "dev" ] && [ "$CREATE" == "1" ];then
	echo "[createpo.sh] svn commit -m [titan] ERROR autoupdate po files"
	
	cp -a "$HOME"/flashimg/$SRCDIR/error/po.log "$HOME"/flashimg/$SRCDIR/error/create_po_error_code
	
	LINE=`cat "$HOME"/flashimg/$SRCDIR/error/po.log | grep -n "fatal error" | cut -d: -f1`
	if [ ! -z "$LINE" ];then
		LINE=`expr $LINE - 1`
		FILE=`cat "$HOME"/flashimg/$SRCDIR/error/po.log | sed -ne ""$LINE"p" | cut -d: -f1`
		LINE=`cat "$HOME"/flashimg/$SRCDIR/error/po.log | sed -ne ""$LINE"p" | cut -d: -f2`
		LINE1=`expr $LINE - 1`
		LINE2=`expr $LINE + 1`
		echo "[createpo.sh] ############################################" >> "$HOME"/flashimg/$SRCDIR/error/create_po_error_code
		echo "[createpo.sh] ###### error should be in the middle #######" >> "$HOME"/flashimg/$SRCDIR/error/create_po_error_code
		echo "[createpo.sh] ############################################" >> "$HOME"/flashimg/$SRCDIR/error/create_po_error_code
		cat -n "$FILE" | sed -ne ""$LINE1","$LINE2"p" >> "$HOME"/flashimg/$SRCDIR/error/create_po_error_code
		echo "[createpo.sh] ############################################" >> "$HOME"/flashimg/$SRCDIR/error/create_po_error_code
	fi

	echo "[createpo.sh] ################## coding #################" >> "$HOME"/flashimg/$SRCDIR/error/create_po_error_code
	cat "$HOME"/flashimg/$SRCDIR/error/coding.log >> "$HOME"/flashimg/$SRCDIR/error/create_po_error_code

	cd "$HOME"/flashimg/$SRCDIR/error

	CHECK=`whoami`
	if [ "$CHECK" != "atemio" ]; then
		echo "[createpo.sh] exit, building only with user atemio"
		exit
	fi

	svn commit -m "[titan] ERROR autoupdate po files"
	svn commit "$HOME"/flashimg/$SRCDIR/error/create_po_error_code
else
	echo "[createpo.sh] skip: svn commit"
fi
