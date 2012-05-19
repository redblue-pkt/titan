TYPE=$1
STM=$2
MEDIAFW=$3
SVNUSER=$4
SVNPASS=$5
SVNURL=$6
KERNELDIR=$7
ROOTDIR=$8
IPKDIR=$9

if [ -z "$TYPE" ]; then
	echo "error: use ./makesh4.sh <BOXTYPE> <stm22|stm23|stm24> <1|2> <svnuser> <svnpass> <svnurl> <kerneldir> <rootdir> <ipkdir>"
	exit 1
fi

if [ -z "$STM" ]; then
	echo "error: use ./makesh4.sh <BOXTYPE> <stm22|stm23|stm24> <1|2> <svnuser> <svnpass> <svnurl> <kerneldir> <rootdir> <ipkdir>"
	exit 1
fi

if [ -z "$MEDIAFW" ]; then
	echo "error: use ./makesh4.sh <BOXTYPE> <stm22|stm23|stm24> <1|2> <svnuser> <svnpass> <svnurl> <kerneldir> <rootdir> <ipkdir>"
	exit 1
fi

if [ -z "$SVNUSER" ]; then
	echo "error: use ./makesh4.sh <BOXTYPE> <stm22|stm23|stm24> <1|2> <svnuser> <svnpass> <svnurl> <kerneldir> <rootdir> <ipkdir>"
	exit 1
fi

if [ -z "$SVNPASS" ]; then
	echo "error: use ./makesh4.sh <BOXTYPE> <stm22|stm23|stm24> <1|2> <svnuser> <svnpass> <svnurl> <kerneldir> <rootdir> <ipkdir>"
	exit 1
fi

if [ -z "$SVNURL" ]; then
	echo "error: use ./makesh4.sh <BOXTYPE> <stm22|stm23|stm24> <1|2> <svnuser> <svnpass> <svnurl> <kerneldir> <rootdir> <ipkdir>"
	exit 1
fi

if [ -z "$KERNELDIR" ]; then
	echo "error: use ./makesh4.sh <BOXTYPE> <stm22|stm23|stm24> <1|2> <svnuser> <svnpass> <svnurl> <kerneldir> <rootdir> <ipkdir>"
	exit 1
fi

if [ -z "$ROOTDIR" ]; then
	echo "error: use ./makesh4.sh <BOXTYPE> <stm22|stm23|stm24> <1|2> <svnuser> <svnpass> <svnurl> <kerneldir> <rootdir> <ipkdir>"
	exit 1
fi

if [ -z "$IPKDIR" ]; then
	echo "error: use ./makesh4.sh <BOXTYPE> <stm22|stm23|stm24> <1|2> <svnuser> <svnpass> <svnurl> <kerneldir> <rootdir> <ipkdir>"
	exit 1
fi

echo "[titan]--------------------------------------------------------"
echo "[titan] get skin"
echo "[titan]--------------------------------------------------------"

rm -rf "$HOME"/flashimg/source.titan/skin
mkdir "$HOME"/flashimg/source.titan/skin	
svn co --username $SVNUSER --password $SVNPASS http://"$SVNURL"/svn/ipk/source.sh4/titanskins_default/var/usr/local/share/titan/skin/default "$HOME"/flashimg/source.titan/skin/default

echo "[titan]--------------------------------------------------------"
echo "[titan] get skin done"
echo "[titan]--------------------------------------------------------"

echo "[titan]--------------------------------------------------------"
echo "[titan] get settings"
echo "[titan]--------------------------------------------------------"
rm -rf "$HOME"/flashimg/source.titan/settings.svn
svn co --username $SVNUSER --password $SVNPASS http://"$SVNURL"/svn/ipk/source.sh4/titansettings_default_sat_1_0/var/etc/titan "$HOME"/flashimg/source.titan/settings.svn
mv -f "$HOME"/flashimg/source.titan/settings.svn/* "$HOME"/flashimg/source.titan/settings 
echo "[titan]--------------------------------------------------------"
echo "[titan] get settings done"
echo "[titan]--------------------------------------------------------"

cp -a "$HOME"/flashimg/source.titan/skins/[^.]* "$HOME"/flashimg/source.titan/plugins

echo "[titan]--------------------------------------------------------"
echo "[titan] cleanup"
echo "[titan]--------------------------------------------------------"

cd "$HOME"/flashimg/source.titan/titan
rm -rf titan
rm -rf titan.o
rm -rf `find "$HOME"/flashimg/source.titan/plugins -type f -name "*.o"`
rm -rf `find "$HOME"/flashimg/source.titan/plugins -type f -name "*.so"`

echo "[titan]--------------------------------------------------------"
echo "[titan] cleanup done"
echo "[titan]--------------------------------------------------------"

#echo "[titan]--------------------------------------------------------"
#echo "[titan] Make Securety"
#echo "[titan]--------------------------------------------------------"

"$HOME"/flashimg/source.titan/titan/tools/gettitancode.sh $KERNELDIR $ROOTDIR $TYPE

#echo "[titan]--------------------------------------------------------"
#echo "[titan] Security done"
#echo "[titan]--------------------------------------------------------"

echo "[titan]--------------------------------------------------------"
echo "[titan] Make Ipkdir"
echo "[titan]--------------------------------------------------------"

"$HOME"/flashimg/source.titan/titan/tools/getipk.sh $IPKDIR

echo "[titan]--------------------------------------------------------"
echo "[titan] Ipkdir done"
echo "[titan]--------------------------------------------------------"


echo "[titan]--------------------------------------------------------"
echo "[titan] netsurf"
echo "[titan]--------------------------------------------------------"

cd "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/cvs/cdk
echo make curl
make curl
		
cd "$HOME"/flashimg/source.titan/netsurf
./makesh4.sh $STM
cd "$HOME"/flashimg/source.titan/titan
if [ ! -e "$HOME"/flashimg/source.titan/netsurf/netsurf-2.8/nsfb ]; then
	echo "[titan]--------------------------------------------------------"
	echo "[titan] netsurf building error !!!"
	echo "[titan] check your src"
	echo "[titan]--------------------------------------------------------"
	exit 1
fi

echo "[titan]--------------------------------------------------------"
echo "[titan] netsurf done"
echo "[titan]--------------------------------------------------------"

echo "[titan]--------------------------------------------------------"
echo "[titan] minidlna"
echo "[titan]--------------------------------------------------------"

cd "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/cvs/cdk
echo make minidlna-clean
make minidlna-clean
rm .deps/minidlna.do_*
echo make minidlna
make minidlna
cd "$HOME"/flashimg/source.titan/titan

if [ ! -e "$HOME"/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/sbin/minidlna ]; then
	echo "[titan]--------------------------------------------------------"
	echo "[titan] minidlna building error !!!"
	echo "[titan] check your src"
	echo "[titan]--------------------------------------------------------"
	exit 1
fi

echo "[titan]--------------------------------------------------------"
echo "[titan] minidlna done"
echo "[titan]--------------------------------------------------------"

echo "[titan]--------------------------------------------------------"
echo "[titan] libipkg"
echo "[titan]--------------------------------------------------------"
cd "$HOME"/flashimg/source.titan/libipkg
./makesh4.sh $STM
cd "$HOME"/flashimg/source.titan/titan
if [ ! -e "$HOME"/flashimg/source.titan/libipkg/.libs/libipkg.so.0.0.0 ]; then
	echo "[titan]--------------------------------------------------------"
	echo "[titan] ipkg building error !!!"
	echo "[titan] check your src"
	echo "[titan]--------------------------------------------------------"
	exit 1
fi
echo "[titan]--------------------------------------------------------"
echo "[titan] libipkg done"
echo "[titan]--------------------------------------------------------"

###
echo "[titan]--------------------------------------------------------"
echo "[titan] libdreamdvd"
echo "[titan]--------------------------------------------------------"
cd "$HOME"/flashimg/source.titan/libdreamdvd
./makesh4.sh $STM
cd "$HOME"/flashimg/source.titan/titan
if [ ! -e "$HOME"/flashimg/source.titan/libdreamdvd/.libs/libdreamdvd.so.0.0.0 ]; then
	echo "[titan]--------------------------------------------------------"
	echo "[titan] libdreamdvd building error !!!"
	echo "[titan] check your src"
	echo "[titan]--------------------------------------------------------"
	exit 1
fi
cp "$HOME"/flashimg/source.titan/libdreamdvd/.libs/libdreamdvd.so.0.0.0 "$ROOTDIR"/lib

echo "[titan]--------------------------------------------------------"
echo "[titan] libdreamdvd done"
echo "[titan]--------------------------------------------------------"
###

echo "[titan]--------------------------------------------------------"
echo "[titan] update git cdkroot"
echo "[titan]--------------------------------------------------------"

#cp "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/cvs/apps/misc/tools/libdreamdvd/.libs/* $HOME/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib

echo "[titan]--------------------------------------------------------"
echo "[titan] update git cdkroot done"
echo "[titan]--------------------------------------------------------"


echo "[titan]--------------------------------------------------------"
echo "[titan] Make titan"
echo "[titan]--------------------------------------------------------"

if [ $MEDIAFW = 1 ]; then
	eplayer=EPLAYER3
	eplayerinclude="$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/cvs/apps/misc/tools/libeplayer3/include
	eplayerlib=eplayer3
else
	eplayer=EPLAYER4
	eplayerinclude="$HOME/flashimg/BUILDGIT/checkout_$STM/tdt/tufsbox/cdkroot/usr/include/gstreamer-0.10
			 -I$HOME/flashimg/BUILDGIT/checkout_$STM/tdt/tufsbox/cdkroot/usr/include/glib-2.0
			 -I$HOME/flashimg/BUILDGIT/checkout_$STM/tdt/tufsbox/cdkroot/usr/include/libxml2
			 -I$HOME/flashimg/BUILDGIT/checkout_$STM/tdt/tufsbox/cdkroot/usr/lib/glib-2.0/include"
	eplayerlib=gstreamer-0.10
fi

"$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/devkit/sh4/bin/sh4-linux-gcc -DSH4 -DCAMSUPP -D$eplayer -DDVDPLAYER -Os -export-dynamic -Wall \
	-I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/include/freetype2 \
	-I $eplayerinclude \
	-I "$HOME"/flashimg/source.titan/libdreamdvd \
	-I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/cvs/driver/bpamem \
	-I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/cvs/apps/misc/tools/libmmeimage \
	-I "$HOME"/flashimg/source.titan \
	-c titan.c
	
"$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/devkit/sh4/bin/sh4-linux-gcc -Os -export-dynamic -lpthread -ldl -lpng -lfreetype -l$eplayerlib -ldreamdvd -ljpeg -lmmeimage -lmme_host -lipkg -Wall \
	titan.o \
	-o titan
	
"$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/devkit/sh4/bin/sh4-linux-strip titan

echo "[titan]--------------------------------------------------------"
echo "[titan] titan done"
echo "[titan]--------------------------------------------------------"

rm -rf "$HOME"/flashimg/BUILD/titan
rm -rf "$HOME"/flashimg/BUILD/titan-smal

if [ ! -e ./titan ]; then
	echo "[titan]--------------------------------------------------------"
	echo "[titan] building error !!!"
	echo "[titan] check your src"
	echo "[titan]--------------------------------------------------------"
	exit 1
fi

echo "[titan]--------------------------------------------------------"
echo "[titan] Make Plugins"
echo "[titan]--------------------------------------------------------"
"$HOME"/flashimg/source.titan/plugins/makesh4.sh "$STM" "$MEDIAFW"
echo "[titan]--------------------------------------------------------"
echo "[titan] Plugins done"
echo "[titan]--------------------------------------------------------"

echo "[titan]--------------------------------------------------------"
echo "[titan] Update Language Po files"
echo "[titan]--------------------------------------------------------"
"$HOME"/flashimg/source.titan/titan/tools/createpo.sh "$SVNUSER" update
echo "[titan]--------------------------------------------------------"
echo "[titan] Update Language Po files done"
echo "[titan]--------------------------------------------------------"

echo "[titan]--------------------------------------------------------"
echo "[titan] Make Language Po files"
echo "[titan]--------------------------------------------------------"
"$HOME"/flashimg/source.titan/po/de/LC_MESSAGES/make.sh "$STM"
"$HOME"/flashimg/source.titan/po/ru/LC_MESSAGES/make.sh "$STM"
"$HOME"/flashimg/source.titan/po/pl/LC_MESSAGES/make.sh "$STM"
"$HOME"/flashimg/source.titan/po/gr/LC_MESSAGES/make.sh "$STM"
"$HOME"/flashimg/source.titan/po/fr/LC_MESSAGES/make.sh "$STM"
echo "[titan]--------------------------------------------------------"
echo "[titan] Language Po files done"
echo "[titan]--------------------------------------------------------"

mkdir -p "$HOME"/flashimg/BUILD/titan/var/etc/titan
mkdir -p "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins
mkdir -p "$HOME"/flashimg/BUILD/titan/usr/local/bin
mkdir -p "$HOME"/flashimg/BUILD/titan/sbin
mkdir -p "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/netsurf
mkdir -p "$HOME"/flashimg/BUILD/titan/usr/sbin
mkdir -p "$HOME"/flashimg/BUILD/titan/usr/bin
mkdir -p "$HOME"/flashimg/BUILD/titan/lib

PLIST=`ls -1 "$HOME"/flashimg/source.titan/plugins`

for ROUND in $PLIST;do
	if [ "$ROUND" == "makesh4.sh" ]; then
		skiped=1
	else
		echo "[titan] copy plugins/$ROUND" 
		cp -a "$HOME"/flashimg/source.titan/plugins/"$ROUND" "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins
	fi
done

cp -a "$HOME"/flashimg/source.titan/settings/* "$HOME"/flashimg/BUILD/titan/var/etc/titan
cp -a "$HOME"/flashimg/source.titan/skin "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan
cp -a "$HOME"/flashimg/source.titan/po "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan
cp titan "$HOME"/flashimg/BUILD/titan/usr/local/bin
chmod 755 "$HOME"/flashimg/BUILD/titan/usr/local/bin/titan
cp -a "$HOME"/flashimg/source.titan/picons "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan
cp -a "$HOME"/flashimg/source.titan/web "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan
cp -a "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/rc."$TYPE".html "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/rc.html
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/rc.*.html
cp -a "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/img/rc."$TYPE".png "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/img/rc.png
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/img/rc.*.png
cp -a "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/img/rc_virtual."$TYPE".png "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/img/rc_virtual.png
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/img/rc_virtual.*.png
cp -a "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/rc_virtual."$TYPE".html "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/rc_virtual.html
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/rc_virtual.*.html
cp -a "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/index."$TYPE".html "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/index.html
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/index.*.html

cp -a "$HOME"/flashimg/BUILD/titan/var/etc/titan/rcconfig."$TYPE" "$HOME"/flashimg/BUILD/titan/var/etc/titan/rcconfig
rm -rf "$HOME"/flashimg/BUILD/titan/var/etc/titan/rcconfig.*

cp -a "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgMusic."$TYPE".mvi "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgMusic.mvi
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgMusic.*.mvi
cp -a "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgBrowser."$TYPE".mvi "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgBrowser.mvi
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgBrowser.*.mvi
cp -a "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgPicture."$TYPE".mvi "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgPicture.mvi
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgPicture.*.mvi
cp -a "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgVideo."$TYPE".mvi "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgVideo.mvi
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgVideo.*.mvi
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgDVD.*
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgRadio.*
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgRSS.*
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgSettings.*
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgVLC.*
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgWeather.*

cp -a "$HOME"/flashimg/source.titan/netsurf/netsurf-2.8/nsfb "$HOME"/flashimg/BUILD/titan/usr/bin
cp -a "$HOME"/flashimg/source.titan/netsurf/netsurf-2.8/framebuffer/res/config/Choices "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/netsurf
cp -a "$HOME"/flashimg/source.titan/netsurf/netsurf-2.8/framebuffer/res/config/Aliases "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/netsurf
cp -a "$HOME"/flashimg/source.titan/netsurf/netsurf-2.8/framebuffer/res/config/*.css "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/netsurf
cp -a "$HOME"/flashimg/source.titan/netsurf/netsurf-2.8/framebuffer/res/config/messages "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/netsurf

cp -a "$HOME"/flashimg/source.titan/libipkg/.libs/libipkg.so.0.0.0 "$HOME"/flashimg/BUILD/titan/lib/libipkg.so.0
cp -a "$HOME"/flashimg/source.titan/libipkg/.libs/ipkg-cl "$HOME"/flashimg/BUILD/titan/usr/bin/ipkg

cp -a "$HOME"/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/sbin/minidlna "$HOME"/flashimg/BUILD/titan/usr/bin
cp -a "$HOME"/BUILDGIT/checkout_"$STM"/tdt/cvs/cdk/minidlna-1.0.24/minidlna.conf "$HOME"/flashimg/BUILD/titan/var/etc

rm -rf `find "$HOME"/flashimg/BUILD/titan -type d -name "*.svn"`
rm -rf `find "$HOME"/flashimg/BUILD/titan -type f -name "*.h"`
rm -rf `find "$HOME"/flashimg/BUILD/titan -type f -name "*.c"`
rm -rf `find "$HOME"/flashimg/BUILD/titan -type f -name "*.o"`
rm -rf `find "$HOME"/flashimg/BUILD/titan -type f -name "*.po"`
rm -rf `find "$HOME"/flashimg/BUILD/titan -type f -name "make.sh"`
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/networkbrowser/netlib
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/tithek/menu

cp -a "$HOME"/flashimg/BUILD/titan "$HOME"/flashimg/BUILD/titan-smal
rm -rf "$HOME"/flashimg/BUILD/titan-smal/var/usr/local/share/titan/picons
rm -rf "$HOME"/flashimg/BUILD/titan-smal/var/etc/titan/titan.cfg

echo "[titan]--------------------------------------------------------"
echo "[titan] building to: "$HOME"/flashimg/BUILD/titan"
echo "[titan] copy this folder to duckbox: /var/usr/local/share/titan"
echo "[titan]--------------------------------------------------------"
