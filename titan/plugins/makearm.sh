#*****************************************************#
#* this file is part of the tiTan / tiTanNIT Project *#
#* and allowed only for use with this.               *#
#*                                                   *#
#* copyright by NIT                                  *#
#*****************************************************#

STM=$1
MEDIAFW=$2
GROUP=$3
BOX=$4
DISTRO=$5
ARCH=$6
SRCDIR=$7

echo $*
echo STM=$1
echo MEDIAFW=$2
echo GROUP=$3
echo BOX=$4
echo DISTRO=$5
echo ARCH=$6
echo SRCDIR=$7

if [ -z "$STM" ]; then
	echo "error: use makesh4.sh <stm23|stm23|stm24> <1|2>"
	exit 1
fi
if [ -z "$MEDIAFW" ]; then
	echo "error: use makesh4.sh <stm23|stm23|stm24> <1|2>"
	exit 1
fi

if [ $GROUP == "dev" ]; then
	devflag="-fexceptions -rdynamic"
else
	devflag=""
fi

cd "$HOME"/flashimg/$SRCDIR/plugins
ln -s "$HOME"/flashimg/$SRCDIR/titan "$HOME"/flashimg/$SRCDIR/plugins/titan

#if [ "$GROUP" = "dev" ];then
	SRC="$HOME"/flashimg/BUILDGIT/checkout_"$STM"/builds/"$DISTRO"/release/"$BOX"
#else
#	SRC="$HOME"/flashimg/BUILDGIT/checkout_"$STM"/builds/"$DISTRO"/"$BOX"
#fi

#if [ $MEDIAFW = 1 ]; then
#	eplayer=EPLAYER3
#	eplayerinclude="$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/cvs/apps/misc/tools/libeplayer3/include
#	eplayerlib=eplayer3
#else
	eplayer=EPLAYER4
	eplayerinclude="$SRC/tmp/sysroots/$BOX/usr/include/$eplayerlib
	        -I$SRC/tmp/sysroots/$BOX/usr/lib/$eplayerlib/include
			-I$SRC/tmp/sysroots/$BOX/usr/include/glib-2.0
			-I$SRC/tmp/sysroots/$BOX/usr/include/libxml2
			-I $SRC/tmp/sysroots/$BOX/usr/include
			-I$SRC/tmp/sysroots/$BOX/usr/lib/glib-2.0/include"
	eplayerlib=$eplayerlib
#fi

#if [ "$DISTRO" = "atemio" ];then
	GCCPATH=arm-oe-linux-gnueabi
#else
#	GCCPATH=mips32el-oe-linux
#fi

if [ "$GROUP" = "dev" ] && [ "$TYPE" != "ufs910" ] && [ "$TYPE" != "ufs922" ]; then
	devflag="-finstrument-functions -rdynamic -DBETA"
else
	devflag=""
fi

compile()
{
	cd $1
	echo "[titan]--------------------------------------------------------"
	echo "[titan] Make Plugin $1"
	echo "[titan]--------------------------------------------------------"

#	$SRC/tmp/sysroots/$ARCH/usr/bin/$GCCPATH/arm-oe-linux-gnueabi-gcc -DMIPSEL -D$eplayer -Os -mhard-float $devflag -fPIC -Wall -Wno-unused-but-set-variable $4 \
#	-I $SRC/tmp/sysroots/$BOX/usr/include/freetype2 \
#	-I $eplayerinclude \
#	-I $SRC/tmp/sysroots/$BOX/usr/include/dreamdvd \
#	-I "$HOME"/flashimg/$SRCDIR/libdreamdvd \
#	-I $SRC/tmp/sysroots/$BOX/usr/include \
#	-I "$HOME" \
#	-c $2.c -o $2.o
#
#	if [ ! -z $3 ]; then
#		$SRC/tmp/sysroots/$ARCH/usr/bin/$GCCPATH/arm-oe-linux-gnueabi-gcc -shared -Wl,-soname,$2.so -o $2.so $2.o $3.a
#	else
#		$SRC/tmp/sysroots/$ARCH/usr/bin/$GCCPATH/arm-oe-linux-gnueabi-gcc -shared -Wl,-soname,$2.so -o $2.so $2.o
#	fi

#/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/sysroots/inihdp/usr/bin/crossscripts/arm-oe-linux-gnueabi-libtool  --tag=CC   --mode=compile /home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihde/tmp/sysroots/x86_64-linux/usr/bin/arm-oe-linux-gnueabi/arm-oe-linux-gnueabi-gcc  -mhard-float --sysroot=/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/sysroots/inihdp -DPACKAGE_NAME=\"tuxbox-apps-titan-plugins\" -DPACKAGE_TARNAME=\"tuxbox-titan-plugins\" -DPACKAGE_VERSION=\"0.0.1\" -DPACKAGE_STRING=\"tuxbox-apps-titan-plugins\ 0.0.1\" -DPACKAGE_BUGREPORT=\"\" -DPACKAGE_URL=\"\" -DPACKAGE=\"tuxbox-titan-plugins\" -DVERSION=\"0.0.1\" -DSTDC_HEADERS=1 -DHAVE_SYS_TYPES_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_MEMORY_H=1 -DHAVE_STRINGS_H=1 -DHAVE_INTTYPES_H=1 -DHAVE_STDINT_H=1 -DHAVE_UNISTD_H=1 -DHAVE_DLFCN_H=1 -DLT_OBJDIR=\".libs/\" -I.  -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE   -I/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/sysroots/inihdp/usr/include 	-I/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/sysroots/inihdp/usr/include/$eplayerlib 	-I/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/sysroots/inihdp/usr/include/glib-2.0 	-I/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/sysroots/inihdp/usr/include/libxml2 	-I/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/sysroots/inihdp/usr/lib/glib-2.0/include 	-I/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/sysroots/inihdp/usr/include/freetype2 	-I/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/sysroots/inihdp/usr/include/dreamdvd 	-I/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/sysroots/inihdp/usr/include/libdreamdvd 	-I/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/work/inihdp-oe-linux/titan-plugins/2.0+svnr32455-r1/titan/libdreamdvd 	-I/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/work/inihdp-oe-linux/titan-plugins/2.0+svnr32455-r1/titan/titan  -DMIPSEL -DCAMSUPP -DEPLAYER4 -MT autotimer.lo -MD -MP -MF .deps/libmc.Tpo -c -o libmc.lo mc.c
#/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihde/tmp/sysroots/x86_64-linux/usr/bin/arm-oe-linux-gnueabi/arm-oe-linux-gnueabi-gcc  -mhard-float --sysroot=/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/sysroots/inihdp -shared  -fPIC -DPIC  .libs/libmc.o    -mhard-float --sysroot=/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/sysroots/inihdp -DMIPSEL -DCAMSUPP -DEPLAYER4 -Wl,-O1 -Wl,--as-needed   -Wl,-soname -Wl,libmc.so.0 -o .libs/libmc.so.0.0.0
	
	LIB=""
	mkdir .deps
	if [ ! -z $3 ]; then
		LIB=$3.a
	fi
	
	eplayerlib=gstreamer-1.0

EXTEPLAYER3=1

if [ "$GROUP" = "dev" ]; then
#    devflag="-finstrument-functions -rdynamic -DBETA"
    devflag="-DBETA -DEXTEPLAYER3"
	eplayer=EPLAYER3
	eplayerlib=eplayer3
	eplayerinclude="$HOME"/flashimg/source.titan/libeplayer3/include
else
	if [ "$EXTEPLAYER3" = "1" ];then
	    devflag="-DEXTEPLAYER3"
		eplayer=EPLAYER3
		eplayerlib=eplayer3
		eplayerinclude="$HOME"/flashimg/source.titan/libeplayer3/include
	else
		eplayer=EPLAYER4
		eplayerlib=gstreamer-1.0
		devflag=""
	fi
fi


if [ $GROUP == "dev" ] || [ "$EXTEPLAYER3" == "1" ]; then
	$SRC/tmp/sysroots/$BOX/usr/bin/crossscripts/arm-oe-linux-gnueabi-libtool  --tag=CC   --mode=compile $SRC/tmp/sysroots/$ARCH/usr/bin/arm-oe-linux-gnueabi/arm-oe-linux-gnueabi-gcc  -mhard-float $devflag -mhard-float --sysroot=$SRC/tmp/sysroots/$BOX -DPACKAGE_NAME=\"tuxbox-apps-titan-plugins\" -DPACKAGE_TARNAME=\"tuxbox-titan-plugins\" -DPACKAGE_VERSION=\"0.0.1\" -DPACKAGE_STRING=\"tuxbox-apps-titan-plugins\ 0.0.1\" -DPACKAGE_BUGREPORT=\"\" -DPACKAGE_URL=\"\" -DPACKAGE=\"tuxbox-titan-plugins\" -DVERSION=\"0.0.1\" -DSTDC_HEADERS=1 -DHAVE_SYS_TYPES_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_MEMORY_H=1 -DHAVE_STRINGS_H=1 -DHAVE_INTTYPES_H=1 -DHAVE_STDINT_H=1 -DHAVE_UNISTD_H=1 -DHAVE_DLFCN_H=1 -DLT_OBJDIR=\".libs/\" -I.  -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE   -I$SRC/tmp/sysroots/$BOX/usr/include                                                       										-I$eplayerinclude       -I$SRC/tmp/sysroots/$BOX/usr/include/glib-2.0 	-I$SRC/tmp/sysroots/$BOX/usr/include/libxml2 	-I$SRC/tmp/sysroots/$BOX/usr/lib/glib-2.0/include 	-I$SRC/tmp/sysroots/$BOX/usr/include/freetype2 	-I$SRC/sysroots/$BOX/usr/include/dreamdvd 	-I$SRC/tmp/sysroots/$BOX/usr/include/libdreamdvd 	-I"$HOME"/flashimg/$SRCDIR/libdreamdvd 	-I"$HOME"/flashimg/$SRCDIR/titan  -DMIPSEL -DCAMSUPP -D$eplayer -MT lib$2.lo -MD -MP -MF .deps/lib$2.Tpo -c -o lib$2.lo $2.c
#	$SRC/tmp/sysroots/$BOX/usr/bin/crossscripts/arm-oe-linux-gnueabi-libtool  --tag=CC   --mode=compile $SRC/tmp/sysroots/$ARCH/usr/bin/arm-oe-linux-gnueabi/arm-oe-linux-gnueabi-gcc  -mhard-float $devflag -mhard-float --sysroot=$SRC/tmp/sysroots/$BOX -DPACKAGE_NAME=\"tuxbox-apps-titan-plugins\" -DPACKAGE_TARNAME=\"tuxbox-titan-plugins\" -DPACKAGE_VERSION=\"0.0.1\" -DPACKAGE_STRING=\"tuxbox-apps-titan-plugins\ 0.0.1\" -DPACKAGE_BUGREPORT=\"\" -DPACKAGE_URL=\"\" -DPACKAGE=\"tuxbox-titan-plugins\" -DVERSION=\"0.0.1\" -DSTDC_HEADERS=1 -DHAVE_SYS_TYPES_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_MEMORY_H=1 -DHAVE_STRINGS_H=1 -DHAVE_INTTYPES_H=1 -DHAVE_STDINT_H=1 -DHAVE_UNISTD_H=1 -DHAVE_DLFCN_H=1 -DLT_OBJDIR=\".libs/\" -I.  -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE   -I$SRC/tmp/sysroots/$BOX/usr/include 	-I$SRC/tmp/sysroots/$BOX/usr/include/$eplayerlib 	-I$SRC/tmp/sysroots/$BOX/usr/lib/$eplayerlib/include     				-I$SRC/tmp/sysroots/$BOX/usr/include/glib-2.0 	-I$SRC/tmp/sysroots/$BOX/usr/include/libxml2 	-I$SRC/tmp/sysroots/$BOX/usr/lib/glib-2.0/include 	-I$SRC/tmp/sysroots/$BOX/usr/include/freetype2 	-I$SRC/sysroots/$BOX/usr/include/dreamdvd 	-I$SRC/tmp/sysroots/$BOX/usr/include/libdreamdvd 	-I"$HOME"/flashimg/$SRCDIR/libdreamdvd 	-I"$HOME"/flashimg/$SRCDIR/titan  -DMIPSEL -DCAMSUPP -DEPLAYER4 -MT lib$2.lo -MD -MP -MF .deps/lib$2.Tpo -c -o lib$2.lo $2.c

	$SRC/tmp/sysroots/$ARCH/usr/bin/arm-oe-linux-gnueabi/arm-oe-linux-gnueabi-gcc  -mhard-float --sysroot=$SRC/tmp/sysroots/$BOX -shared  -fPIC -DPIC  .libs/lib$2.o    -mhard-float $devflag --sysroot=$SRC/tmp/sysroots/$BOX -DMIPSEL -DCAMSUPP -D$eplayer -Wl,-O1 -Wl,--as-needed   -Wl,-soname -Wl,lib$2.so.0 $LIB -o .libs/lib$2.so $4
#	$SRC/tmp/sysroots/$ARCH/usr/bin/arm-oe-linux-gnueabi/arm-oe-linux-gnueabi-gcc  -mhard-float --sysroot=$SRC/tmp/sysroots/$BOX -shared  -fPIC -DPIC  .libs/lib$2.o    -mhard-float $devflag --sysroot=$SRC/tmp/sysroots/$BOX -DMIPSEL -DCAMSUPP -DEPLAYER4 -Wl,-O1 -Wl,--as-needed   -Wl,-soname -Wl,lib$2.so.0 $LIB -o .libs/lib$2.so $4
else
	$SRC/tmp/sysroots/$BOX/usr/bin/crossscripts/arm-oe-linux-gnueabi-libtool  --tag=CC   --mode=compile $SRC/tmp/sysroots/$ARCH/usr/bin/arm-oe-linux-gnueabi/arm-oe-linux-gnueabi-gcc  -mhard-float $devflag -mhard-float --sysroot=$SRC/tmp/sysroots/$BOX -DPACKAGE_NAME=\"tuxbox-apps-titan-plugins\" -DPACKAGE_TARNAME=\"tuxbox-titan-plugins\" -DPACKAGE_VERSION=\"0.0.1\" -DPACKAGE_STRING=\"tuxbox-apps-titan-plugins\ 0.0.1\" -DPACKAGE_BUGREPORT=\"\" -DPACKAGE_URL=\"\" -DPACKAGE=\"tuxbox-titan-plugins\" -DVERSION=\"0.0.1\" -DSTDC_HEADERS=1 -DHAVE_SYS_TYPES_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_MEMORY_H=1 -DHAVE_STRINGS_H=1 -DHAVE_INTTYPES_H=1 -DHAVE_STDINT_H=1 -DHAVE_UNISTD_H=1 -DHAVE_DLFCN_H=1 -DLT_OBJDIR=\".libs/\" -I.  -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE   -I$SRC/tmp/sysroots/$BOX/usr/include 	-I$SRC/tmp/sysroots/$BOX/usr/include/$eplayerlib 	-I$SRC/tmp/sysroots/$BOX/usr/lib/$eplayerlib/include     -I$SRC/tmp/sysroots/$BOX/usr/include/glib-2.0 	-I$SRC/tmp/sysroots/$BOX/usr/include/libxml2 	-I$SRC/tmp/sysroots/$BOX/usr/lib/glib-2.0/include 	-I$SRC/tmp/sysroots/$BOX/usr/include/freetype2 	-I$SRC/sysroots/$BOX/usr/include/dreamdvd 	-I$SRC/tmp/sysroots/$BOX/usr/include/libdreamdvd 	-I"$HOME"/flashimg/$SRCDIR/libdreamdvd 	-I"$HOME"/flashimg/$SRCDIR/titan  -DMIPSEL -DCAMSUPP -DEPLAYER4 -MT lib$2.lo -MD -MP -MF .deps/lib$2.Tpo -c -o lib$2.lo $2.c
	$SRC/tmp/sysroots/$ARCH/usr/bin/arm-oe-linux-gnueabi/arm-oe-linux-gnueabi-gcc  -mhard-float --sysroot=$SRC/tmp/sysroots/$BOX -shared  -fPIC -DPIC  .libs/lib$2.o    -mhard-float $devflag --sysroot=$SRC/tmp/sysroots/$BOX -DMIPSEL -DCAMSUPP -DEPLAYER4 -Wl,-O1 -Wl,--as-needed   -Wl,-soname -Wl,lib$2.so.0 $LIB -o .libs/lib$2.so $4
fi

	cp -a .libs/lib$2.so lib$2.so

	$SRC/tmp/sysroots/$ARCH/usr/bin/$GCCPATH/arm-oe-linux-gnueabi-strip lib$2.so
	cd ..
	echo "[titan]--------------------------------------------------------"
	echo "[titan] Plugin $1 done"
	echo "[titan]--------------------------------------------------------"
}

rm -rf `find "$HOME"/flashimg/$SRCDIR/plugins -type f -name "*.o"`
rm -rf `find "$HOME"/flashimg/$SRCDIR/plugins -type f -name "*.so"`
rm -rf `find "$HOME"/flashimg/$SRCDIR/plugins -type f -name "*.lo"`
rm -rf `find "$HOME"/flashimg/$SRCDIR/plugins -type f -name "*.libs"`
rm -rf `find "$HOME"/flashimg/$SRCDIR/plugins -type f -name "*.deps"`

echo "[titan]--------------------------------------------------------"
echo "[titan] Make networkbrowser"
echo "[titan]--------------------------------------------------------"

cd networkbrowser/netlib
cp Makefile."$STM"."$ARCH".arm Makefile
sed "s!CC =.*!CC = $SRC/tmp/sysroots/$ARCH/usr/bin/arm-oe-linux-gnueabi/arm-oe-linux-gnueabi-gcc!" -i Makefile
sed "s!AR =.*!AR = $SRC/tmp/sysroots/$ARCH/usr/bin/arm-oe-linux-gnueabi/arm-oe-linux-gnueabi-ar!" -i Makefile
sed "s!-I .*!-I $SRC/tmp/sysroots/$BOX/usr/include!" -i Makefile

make clean
make
cd "$HOME"/flashimg/$SRCDIR/plugins
echo "[titan]--------------------------------------------------------"
echo "[titan] networkbrowser done"
echo "[titan]--------------------------------------------------------"

#dir, file, extralib
compile "networkbrowser" "networkbrowser" "netlib/netlib" ""
compile "hello" "hello" "" ""
compile "panel" "panel" "" ""
compile "mc" "mc" "" ""
compile "TopfieldVFD" "TopfieldVFD" "" ""
compile "mboxinfo" "mboxinfo" "" ""
compile "browser" "browser" "" ""
compile "keylock" "keylock" "" ""
compile "permtime" "permtime" "" ""
compile "zapback" "zapback" "" ""
compile "imdbapi" "imdbapi" "" ""
compile "lcdpearl1" "lcdpearl1" "" ""
compile "lcdsamsung" "lcdsamsung" "" ""
compile "wlandisplay" "wlandisplay" "" ""
compile "callmonitor1" "callmonitor1" "" ""
compile "stopifnotused" "stopifnotused" "" ""
compile "tithek" "tithek" "" "-l curl"
compile "wins3" "wins3" "" ""
compile "rgui" "rgui" "" ""
compile "dvdplayer" "dvdplay" "" ""
compile "scriptexec" "scriptexec" "" ""
compile "optimize" "optimize" "" ""
compile "weather" "weather" "" ""
compile "tinews" "tinews" "" ""
compile "stock" "stock" "" ""
compile "streaminfo" "streaminfo" "" ""
compile "tmc" "tmc" "" ""
compile "dlna" "dlna" "" ""
compile "hbbtv_mipsel" "hbbtv" "" ""
compile "instar" "instar" "" ""
compile "tmdb" "tmdb" "" ""
compile "gmediarender" "gmediarender" "" ""
compile "imdb" "imdb" "" ""
compile "filemanager" "filemanager" "" ""
compile "catcatch" "catcatch" "" ""
compile "facebook" "facebook" "" "-l curl"
compile "readerconfig" "readerconfig" "" ""
compile "tiwakeup" "tiwakeup" "" ""
compile "autotimer" "autotimer" "" ""
compile "usbreset" "usbreset" "" ""
compile "tsSchnitt" "tsSchnitt" "" ""
compile "xupnpd" "xupnpd" "" ""
compile "wm2018" "wm2018" "" ""
compile "kravencfg" "kravencfg" "" ""
compile "multiimage" "MultiImage" "" ""
compile "sispmctl" "sispmctl" "" ""
compile "kodistarter" "kodistarter" "" ""
compile "e2starter" "e2starter" "" ""

#rm -r networkbrowser/networkbrowser
