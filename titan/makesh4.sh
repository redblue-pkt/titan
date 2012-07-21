TYPE=$1
STM=$2
MEDIAFW=$3
SVNUSER=$4
SVNPASS=$5
SVNURL=$6
KERNELDIR=$7
ROOTDIR=$8
IPKDIR=$9
VERSION=${10}

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
sort -u "$HOME"/flashimg/source.titan/settings/channel > "$HOME"/flashimg/source.titan/settings/channel.sort
mv "$HOME"/flashimg/source.titan/settings/channel.sort "$HOME"/flashimg/source.titan/settings/channel
sed s/"^ *"// -i "$HOME"/flashimg/source.titan/settings/channel
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

"$HOME"/flashimg/source.titan/titan/tools/getipk.sh $IPKDIR $VERSION

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
echo make minidlna
make minidlna
cd "$HOME"/flashimg/source.titan/titan

if [ ! -e "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/sbin/minidlna ]; then
	echo "[titan]--------------------------------------------------------"
	echo "[titan] minidlna building error !!!"
	echo "[titan] check your src"
	echo "[titan]--------------------------------------------------------"
	exit 1
fi

echo "[titan]--------------------------------------------------------"
echo "[titan] minidlna done"
echo "[titan]--------------------------------------------------------"

if [ $STM != "stm23" ] ;then
	echo "[titan]--------------------------------------------------------"
	echo "[titan] curlftpfs"
	echo "[titan]--------------------------------------------------------"
	cd "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/cvs/cdk
	echo make curlftpfs-clean
	make curlftpfs-clean
	echo make curlftpfs
	make curlftpfs
	cd "$HOME"/flashimg/source.titan/titan
	
	if [ ! -e "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/bin/curlftpfs ]; then
		echo "[titan]--------------------------------------------------------"
		echo "[titan] curlftpfs building error !!!"
		echo "[titan] check your src"
		echo "[titan]--------------------------------------------------------"
		exit 1
	fi
	
	
	echo "[titan]--------------------------------------------------------"
	echo "[titan] curlftpfs done"
	echo "[titan]--------------------------------------------------------"
	
	echo "[titan]--------------------------------------------------------"
	echo "[titan] djmount"
	echo "[titan]--------------------------------------------------------"
	cd "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/cvs/cdk
	echo make djmount-clean
	make djmount-clean
	echo make djmount
	make djmount
	cd "$HOME"/flashimg/source.titan/titan
	
	if [ ! -e "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/bin/djmount ]; then
		echo "[titan]--------------------------------------------------------"
		echo "[titan] djmount building error !!!"
		echo "[titan] check your src"
		echo "[titan]--------------------------------------------------------"
		exit 1
	fi
	
	echo "[titan]--------------------------------------------------------"
	echo "[titan] djmount done"
	echo "[titan]--------------------------------------------------------"
	
	echo "[titan]--------------------------------------------------------"
	echo "[titan] sshfs"
	echo "[titan]--------------------------------------------------------"
	
	cd "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/cvs/cdk
	echo make sshfs-clean
	make sshfs-clean
	echo make sshfs
	make sshfs
	cd "$HOME"/flashimg/source.titan/titan
	
	if [ ! -e "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/bin/sshfs ]; then
		echo "[titan]--------------------------------------------------------"
		echo "[titan] sshfs building error !!!"
		echo "[titan] check your src"
		echo "[titan]--------------------------------------------------------"
		exit 1
	fi
	
	echo "[titan]--------------------------------------------------------"
	echo "[titan] sshfs done"
	echo "[titan]--------------------------------------------------------"
	
	echo "[titan]--------------------------------------------------------"
	echo "[titan] rarfs"
	echo "[titan]--------------------------------------------------------"
	
	cd "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/cvs/cdk
	echo make rarfs-clean
	make rarfs-clean
	echo make rarfs
	make rarfs
	cd "$HOME"/flashimg/source.titan/titan
	
	if [ ! -e "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/bin/rarfs ]; then
		echo "[titan]--------------------------------------------------------"
		echo "[titan] sshfs building error !!!"
		echo "[titan] check your src"
		echo "[titan]--------------------------------------------------------"
		exit 1
	fi
	
	echo "[titan]--------------------------------------------------------"
	echo "[titan] rarfs done"
	echo "[titan]--------------------------------------------------------"

	echo "[titan]--------------------------------------------------------"
	echo "[titan] gst"
	echo "[titan]--------------------------------------------------------"
	
	cd "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/cvs/cdk
	echo make gst_plugins_dvbmediasink-clean
	make gst_plugins_dvbmediasink-clean
	echo make gst_plugins_dvbmediasink
	make gst_plugins_dvbmediasink
	cd "$HOME"/flashimg/source.titan/titan
	
	if [ ! -e "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/gstreamer-0.10 ]; then
		echo "[titan]--------------------------------------------------------"
		echo "[titan] gst building error !!!"
		echo "[titan] check your src"
		echo "[titan]--------------------------------------------------------"
		exit 1
	fi
	
	echo "[titan]--------------------------------------------------------"
	echo "[titan] gst done"
	echo "[titan]--------------------------------------------------------"
	
	echo "[titan]--------------------------------------------------------"
	echo "[titan] directfb"
	echo "[titan]--------------------------------------------------------"
	
	cd "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/cvs/cdk
	echo make directfb-clean
	make directfb-clean
	echo make directfb
	make directfb
	cd "$HOME"/flashimg/source.titan/titan
	
	if [ ! -e "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/directfb-1.4-5 ]; then
		echo "[titan]--------------------------------------------------------"
		echo "[titan] directfb building error !!!"
		echo "[titan] check your src"
		echo "[titan]--------------------------------------------------------"
		exit 1
	fi
	
	echo "[titan]--------------------------------------------------------"
	echo "[titan] directfb done"
	echo "[titan]--------------------------------------------------------"
	
	echo "[titan]--------------------------------------------------------"
	echo "[titan] gmediarender"
	echo "[titan]--------------------------------------------------------"
	
	cd "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/cvs/cdk
	echo make gmediarender-clean
	make gmediarender-clean
	echo make gmediarender
	make gmediarender
	cd "$HOME"/flashimg/source.titan/titan
	
	if [ ! -e "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/bin/gmediarender ]; then
		echo "[titan]--------------------------------------------------------"
		echo "[titan] gmediarender building error !!!"
		echo "[titan] check your src"
		echo "[titan]--------------------------------------------------------"
		exit 1
	fi
	
	echo "[titan]--------------------------------------------------------"
	echo "[titan] gmediarender done"
	echo "[titan]--------------------------------------------------------"
fi

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
mkdir -p "$HOME"/flashimg/BUILD/titan/var/usr/lib
mkdir -p "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins
mkdir -p "$HOME"/flashimg/BUILD/titan/usr/local/bin
mkdir -p "$HOME"/flashimg/BUILD/titan/sbin
mkdir -p "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/netsurf
mkdir -p "$HOME"/flashimg/BUILD/titan/usr/sbin
mkdir -p "$HOME"/flashimg/BUILD/titan/usr/bin
mkdir -p "$HOME"/flashimg/BUILD/titan/usr/lib
mkdir -p "$HOME"/flashimg/BUILD/titan/usr/share
mkdir -p "$HOME"/flashimg/BUILD/titan/lib
mkdir -p "$HOME"/flashimg/BUILD/titan/modules

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

# minidlna
cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/sbin/minidlna "$HOME"/flashimg/BUILD/titan/usr/bin
cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libexif.so* "$HOME"/flashimg/BUILD/titan/lib
cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libvorbis.so* "$HOME"/flashimg/BUILD/titan/lib

if [ $MEDIAFW = 2 ];then
	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libavcodec.so* "$HOME"/flashimg/BUILD/titan/lib
	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libavformat.so* "$HOME"/flashimg/BUILD/titan/lib
	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libavutil.so* "$HOME"/flashimg/BUILD/titan/lib
elif [ $MEDIAFW = 1 ];then
	#directfb
	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/directfb-1.4-5 "$HOME"/flashimg/BUILD/titan/usr/lib
	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libdirectfb* "$HOME"/flashimg/BUILD/titan/usr/lib

	#gst
	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/gstreamer-0.10 "$HOME"/flashimg/BUILD/titan/usr/lib
	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libgst* "$HOME"/flashimg/BUILD/titan/usr/lib
	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libdirect* "$HOME"/flashimg/BUILD/titan/usr/lib
	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libfusion* "$HOME"/flashimg/BUILD/titan/usr/lib
	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libglib* "$HOME"/flashimg/BUILD/titan/usr/lib
	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libgmodule* "$HOME"/flashimg/BUILD/titan/usr/lib
	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libgobject* "$HOME"/flashimg/BUILD/titan/usr/lib
	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libgthread* "$HOME"/flashimg/BUILD/titan/usr/lib

	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libasound* "$HOME"/flashimg/BUILD/titan/usr/lib
	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libFLAC* "$HOME"/flashimg/BUILD/titan/usr/lib
	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libgio* "$HOME"/flashimg/BUILD/titan/usr/lib
	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libogg* "$HOME"/flashimg/BUILD/titan/usr/lib
	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libixml* "$HOME"/flashimg/BUILD/titan/usr/lib
	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libsoup* "$HOME"/flashimg/BUILD/titan/usr/lib
	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libthreadutil* "$HOME"/flashimg/BUILD/titan/usr/lib

	mkdir "$HOME"/flashimg/BUILD/titan/usr/lib-all
	find "$HOME"/flashimg/BUILD/titan/usr/lib -maxdepth 1 -type f \( ! -iname ".*" \) -exec cp {} "$HOME/flashimg/BUILD/titan/usr/lib-all/" \;	
	rm -rf `find "$HOME"/flashimg/BUILD/titan/usr/lib-all -type f -name "*.a"`
	rm -rf `find "$HOME"/flashimg/BUILD/titan/usr/lib-all -type f -name "*.la"`

	rm -rf "$HOME"/flashimg/BUILD/titan/usr/lib/lib*
	list=`ls -1 "$HOME"/flashimg/BUILD/titan/usr/lib-all`
	for ROUND in $list; do
		dest=`echo $ROUND | sed -e 's/.\{4\}$//'`
		echo cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/$ROUND "$HOME"/flashimg/BUILD/titan/usr/lib/$dest
#		cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/$ROUND "$HOME"/flashimg/BUILD/titan/usr/lib/$dest
	done
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libasound.so.2.0.0 "$HOME"/flashimg/BUILD/titan/usr/lib/libasound.so.2
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libdirect-1.4.so.5.0.6 "$HOME"/flashimg/BUILD/titan/usr/lib/libdirect-1.4.so.5
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libdirectfb-1.4.so.5.0.6 "$HOME"/flashimg/BUILD/titan/usr/lib/libdirectfb-1.4.so.5
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libFLAC.so.8.2.0 "$HOME"/flashimg/BUILD/titan/usr/lib/libFLAC.so.8
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libfusion-1.4.so.5.0.6 "$HOME"/flashimg/BUILD/titan/usr/lib/libfusion-1.4.so.5
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libgio-2.0.so.0.2800.3 "$HOME"/flashimg/BUILD/titan/usr/lib/libgio-2.0.so.0
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libglib-2.0.so.0.2800.3 "$HOME"/flashimg/BUILD/titan/usr/lib/libglib-2.0.so.0
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libgmodule-2.0.so.0.2800.3 "$HOME"/flashimg/BUILD/titan/usr/lib/libgmodule-2.0.so.0
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libgobject-2.0.so.0.2800.3 "$HOME"/flashimg/BUILD/titan/usr/lib/libgobject-2.0.so.0
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libgstapp-0.10.so.0.25.0 "$HOME"/flashimg/BUILD/titan/usr/lib/libgstapp-0.10.so.0
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libgstaudio-0.10.so.0.25.0 "$HOME"/flashimg/BUILD/titan/usr/lib/libgstaudio-0.10.so.0
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libgstbase-0.10.so.0.30.0 "$HOME"/flashimg/BUILD/titan/usr/lib/libgstbase-0.10.so.0
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libgstbasecamerabinsrc-0.10.so.0.0.0 "$HOME"/flashimg/BUILD/titan/usr/lib/libgstbasecamerabinsrc-0.10.so.0
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libgstbasevideo-0.10.so.0.0.0 "$HOME"/flashimg/BUILD/titan/usr/lib/libgstbasevideo-0.10.so.0
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libgstcdda-0.10.so.0.25.0 "$HOME"/flashimg/BUILD/titan/usr/lib/libgstcdda-0.10.so.0
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libgstcheck-0.10.so.0.30.0 "$HOME"/flashimg/BUILD/titan/usr/lib/libgstcheck-0.10.so.0
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libgstcodecparsers-0.10.so.0.0.0 "$HOME"/flashimg/BUILD/titan/usr/lib/libgstcodecparsers-0.10.so.0
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libgstcontroller-0.10.so.0.30.0 "$HOME"/flashimg/BUILD/titan/usr/lib/libgstcontroller-0.10.so.0
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libgstdataprotocol-0.10.so.0.30.0 "$HOME"/flashimg/BUILD/titan/usr/lib/libgstdataprotocol-0.10.so.0
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libgstfft-0.10.so.0.25.0 "$HOME"/flashimg/BUILD/titan/usr/lib/libgstfft-0.10.so.0
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libgstinterfaces-0.10.so.0.25.0 "$HOME"/flashimg/BUILD/titan/usr/lib/libgstinterfaces-0.10.so.0
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libgstnet-0.10.so.0.30.0 "$HOME"/flashimg/BUILD/titan/usr/lib/libgstnet-0.10.so.0
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libgstnetbuffer-0.10.so.0.25.0 "$HOME"/flashimg/BUILD/titan/usr/lib/libgstnetbuffer-0.10.so.0
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libgstpbutils-0.10.so.0.25.0 "$HOME"/flashimg/BUILD/titan/usr/lib/libgstpbutils-0.10.so.0
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libgstphotography-0.10.so.0.0.0 "$HOME"/flashimg/BUILD/titan/usr/lib/libgstphotography-0.10.so.0
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libgstreamer-0.10.so.0.30.0 "$HOME"/flashimg/BUILD/titan/usr/lib/libgstreamer-0.10.so.0
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libgstriff-0.10.so.0.25.0 "$HOME"/flashimg/BUILD/titan/usr/lib/libgstriff-0.10.so.0
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libgstrtp-0.10.so.0.25.0 "$HOME"/flashimg/BUILD/titan/usr/lib/libgstrtp-0.10.so.0
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libgstrtsp-0.10.so.0.25.0 "$HOME"/flashimg/BUILD/titan/usr/lib/libgstrtsp-0.10.so.0
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libgstsdp-0.10.so.0.25.0 "$HOME"/flashimg/BUILD/titan/usr/lib/libgstsdp-0.10.so.0
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libgstsignalprocessor-0.10.so.0.0.0 "$HOME"/flashimg/BUILD/titan/usr/lib/libgstsignalprocessor-0.10.so.0
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libgsttag-0.10.so.0.25.0 "$HOME"/flashimg/BUILD/titan/usr/lib/libgsttag-0.10.so.0
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libgstvideo-0.10.so.0.25.0 "$HOME"/flashimg/BUILD/titan/usr/lib/libgstvideo-0.10.so.0
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libgthread-2.0.so.0.2800.3 "$HOME"/flashimg/BUILD/titan/usr/lib/libgthread-2.0.so.0
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libixml.so.2.0.7 "$HOME"/flashimg/BUILD/titan/usr/lib/libixml.so.2
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libogg.so.0.7.1 "$HOME"/flashimg/BUILD/titan/usr/lib/libogg.so.0
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libsoup-2.4.so.1.3.0 "$HOME"/flashimg/BUILD/titan/usr/lib/libsoup-2.4.so.1
	cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/libthreadutil.so.6.0.2 "$HOME"/flashimg/BUILD/titan/usr/lib/libthreadutil.so.6

	rm -rf "$HOME"/flashimg/BUILD/titan/usr/lib-all
else
	#directfb
	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/directfb-1.4-5 "$HOME"/flashimg/BUILD/titan/usr/lib
	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libdirectfb* "$HOME"/flashimg/BUILD/titan/usr/lib
	
	#gst
	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/gstreamer-0.10 "$HOME"/flashimg/BUILD/titan/usr/lib
	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libgst* "$HOME"/flashimg/BUILD/titan/usr/lib
	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libdirect* "$HOME"/flashimg/BUILD/titan/usr/lib
	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libfusion* "$HOME"/flashimg/BUILD/titan/usr/lib
	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libglib* "$HOME"/flashimg/BUILD/titan/usr/lib
	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libgmodule* "$HOME"/flashimg/BUILD/titan/usr/lib
	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libgobject* "$HOME"/flashimg/BUILD/titan/usr/lib
	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libgthread* "$HOME"/flashimg/BUILD/titan/usr/lib

	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libasound* "$HOME"/flashimg/BUILD/titan/usr/lib
	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libFLAC* "$HOME"/flashimg/BUILD/titan/usr/lib
	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libgio* "$HOME"/flashimg/BUILD/titan/usr/lib
	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libogg* "$HOME"/flashimg/BUILD/titan/usr/lib
	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libixml* "$HOME"/flashimg/BUILD/titan/usr/lib
	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libsoup* "$HOME"/flashimg/BUILD/titan/usr/lib
	cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libthreadutil* "$HOME"/flashimg/BUILD/titan/usr/lib

	ln -s /var/swap/lib/gstreamer-0.10 "$HOME"/flashimg/BUILD/titan/var/usr/lib/gstreamer-0.10
	ln -s /var/swap/lib/directfb-1.4-5 "$HOME"/flashimg/BUILD/titan/var/usr/lib/directfb-1.4-5
fi

#gmediarender
cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/bin/gmediarender "$HOME"/flashimg/BUILD/titan/usr/bin
cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libupnp.so.6.3.1 "$HOME"/flashimg/BUILD/titan/usr/lib/libupnp.so.6
cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/share/gmediarender "$HOME"/flashimg/BUILD/titan/usr/share

#new net tools
cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/bin/djmount "$HOME"/flashimg/BUILD/titan/sbin
cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/bin/rarfs "$HOME"/flashimg/BUILD/titan/sbin
cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/bin/curlftpfs "$HOME"/flashimg/BUILD/titan/sbin
cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/bin/sshfs "$HOME"/flashimg/BUILD/titan/sbin
cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libcurl.so* "$HOME"/flashimg/BUILD/titan/lib
cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libfuse.so* "$HOME"/flashimg/BUILD/titan/lib

# ffmpeg
cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/bin/ffmpeg "$HOME"/flashimg/BUILD/titan/sbin
cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/bin/jpegtran "$HOME"/flashimg/BUILD/titan/sbin

# wlan
cp -a "$ROOTDIR"/lib/modules/8192cu.ko "$HOME"/flashimg/BUILD/titan/modules
cp -a "$ROOTDIR"/lib/modules/8712u.ko "$HOME"/flashimg/BUILD/titan/modules
cp -a "$ROOTDIR"/lib/modules/rt2870sta.ko "$HOME"/flashimg/BUILD/titan/modules
cp -a "$ROOTDIR"/lib/modules/rt3070sta.ko "$HOME"/flashimg/BUILD/titan/modules
cp -a "$ROOTDIR"/lib/modules/rt5370sta.ko "$HOME"/flashimg/BUILD/titan/modules

#ipkg
cp -a "$HOME"/flashimg/source.titan/ipkg "$HOME"/flashimg/BUILD/titan/var/usr/lib
sed "s/Version:.*/Version: $VERSION/" -i "$HOME"/flashimg/BUILD/titan/var/usr/lib/ipkg/status
sed "s/Version:.*/Version: $VERSION/" -i "$HOME"/flashimg/BUILD/titan/var/usr/lib/ipkg/info/*.control

#crypt
"$HOME"/flashimg/source.titan/titan/tools/ocrypt -c "$HOME"/flashimg/source.titan/titan/tools/trustlist "$HOME"/flashimg/BUILD/titan/var/etc/codepages/codepage.868

rm -rf `find "$HOME"/flashimg/BUILD/titan -type d -name "*.svn"`
rm -rf `find "$HOME"/flashimg/BUILD/titan -type f -name "*.h"`
rm -rf `find "$HOME"/flashimg/BUILD/titan -type f -name "*.c"`
rm -rf `find "$HOME"/flashimg/BUILD/titan -type f -name "*.o"`
rm -rf `find "$HOME"/flashimg/BUILD/titan -type f -name "*.po"`
rm -rf `find "$HOME"/flashimg/BUILD/titan -type f -name "*.a"`
rm -rf `find "$HOME"/flashimg/BUILD/titan -type f -name "*.la"`
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
