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
BUILDTYPE=${11}
BOX=${12}
GROUP=${13}
DISTRO=${14}
ARCH=${15}
#if [ "$GROUP" = "dev" ];then
	SRC="$HOME"/flashimg/BUILDGIT/checkout_"$STM"/builds/"$DISTRO"/release/"$BOX"
#else
#	SRC="$HOME"/flashimg/BUILDGIT/checkout_"$STM"/builds/"$DISTRO"/"$BOX"
#fi
SRCDIR=${16}
CPU=${17}
BOXNAME=${18}
TYPE1=${19}

DREAM="NOTDREAM"
if [ "$BOX" == "dm7020hd" ];then
	DREAM="DREAMBOX"
fi


#echo $*
if [ -e "$HOME"/flashimg/.ipk-build-error ]; then
	rm "$HOME"/flashimg/.ipk-build-error
	exit
fi

# work
#
#   mv -f "$HOME"/flashimg/$SRCDIR/titan/struct.mipsel.h "$HOME"/flashimg/$SRCDIR/titan/struct.h

#if [ "$GROUP" = "dev" ];then
#   mv -f "$HOME"/flashimg/$SRCDIR/titan/skin.mipsel.h "$HOME"/flashimg/$SRCDIR/titan/skin.h
#fi

if [ -z "$TYPE" ]; then
    echo "error: use ./makesh4.sh <BOXTYPE> <stm22|stm23|stm24> <1|2> <svnuser> <svnpass> <svnurl> <kerneldir> <rootdir> <ipkdir> <version> <buildtype>"
    touch "$HOME"/flashimg/.ipk-build-error
    exit 1
fi

if [ -z "$STM" ]; then
    echo "error: use ./makesh4.sh <BOXTYPE> <stm22|stm23|stm24> <1|2> <svnuser> <svnpass> <svnurl> <kerneldir> <rootdir> <ipkdir> <version> <buildtype>"
    touch "$HOME"/flashimg/.ipk-build-error
    exit 1
fi

if [ -z "$MEDIAFW" ]; then
    echo "error: use ./makesh4.sh <BOXTYPE> <stm22|stm23|stm24> <1|2> <svnuser> <svnpass> <svnurl> <kerneldir> <rootdir> <ipkdir> <version> <buildtype>"
    touch "$HOME"/flashimg/.ipk-build-error
    exit 1
fi

if [ -z "$SVNUSER" ]; then
    echo "error: use ./makesh4.sh <BOXTYPE> <stm22|stm23|stm24> <1|2> <svnuser> <svnpass> <svnurl> <kerneldir> <rootdir> <ipkdir> <version> <buildtype>"
    touch "$HOME"/flashimg/.ipk-build-error
    exit 1
fi

if [ -z "$SVNPASS" ]; then
    echo "error: use ./makesh4.sh <BOXTYPE> <stm22|stm23|stm24> <1|2> <svnuser> <svnpass> <svnurl> <kerneldir> <rootdir> <ipkdir> <version> <buildtype>"
    touch "$HOME"/flashimg/.ipk-build-error
    exit 1
fi

if [ -z "$SVNURL" ]; then
    echo "error: use ./makesh4.sh <BOXTYPE> <stm22|stm23|stm24> <1|2> <svnuser> <svnpass> <svnurl> <kerneldir> <rootdir> <ipkdir> <version> <buildtype>"
    touch "$HOME"/flashimg/.ipk-build-error
    exit 1
fi

if [ -z "$KERNELDIR" ]; then
    echo "error: use ./makesh4.sh <BOXTYPE> <stm22|stm23|stm24> <1|2> <svnuser> <svnpass> <svnurl> <kerneldir> <rootdir> <ipkdir> <version> <buildtype>"
    touch "$HOME"/flashimg/.ipk-build-error
    exit 1
fi

if [ -z "$ROOTDIR" ]; then
    echo "error: use ./makesh4.sh <BOXTYPE> <stm22|stm23|stm24> <1|2> <svnuser> <svnpass> <svnurl> <kerneldir> <rootdir> <ipkdir> <version> <buildtype>"
    touch "$HOME"/flashimg/.ipk-build-error
    exit 1
fi

if [ -z "$IPKDIR" ]; then
    echo "error: use ./makesh4.sh <BOXTYPE> <stm22|stm23|stm24> <1|2> <svnuser> <svnpass> <svnurl> <kerneldir> <rootdir> <ipkdir> <version> <buildtype>"
    touch "$HOME"/flashimg/.ipk-build-error
    exit 1
fi

if [ -z "$VERSION" ]; then
    echo "error: use ./makesh4.sh <BOXTYPE> <stm22|stm23|stm24> <1|2> <svnuser> <svnpass> <svnurl> <kerneldir> <rootdir> <ipkdir> <version> <buildtype>"
    touch "$HOME"/flashimg/.ipk-build-error
    exit 1
fi

if [ -z "$BUILDTYPE" ]; then
    echo "error: use ./makesh4.sh <BOXTYPE> <stm22|stm23|stm24> <1|2> <svnuser> <svnpass> <svnurl> <kerneldir> <rootdir> <ipkdir> <version> <buildtype>"
    echo BUILDTYPE 0 = build all
    echo BUILDTYPE 1 = build libdreamdvd and titan
    echo BUILDTYPE 2 = build titan only
    touch "$HOME"/flashimg/.ipk-build-error
    exit 1
fi


if [ $# -lt 10 ]; then
    echo "error: use ./makesh4.sh <BOXTYPE> <stm22|stm23|stm24> <1|2> <svnuser> <svnpass> <svnurl> <kerneldir> <rootdir> <ipkdir> <version> <buildtype>"
    echo BUILDTYPE 0 = build all
    echo BUILDTYPE 1 = build libdreamdvd and titan
    echo BUILDTYPE 2 = build titan only
    touch "$HOME"/flashimg/.ipk-build-error
    exit 1
fi

echo "[titan]--------------------------------------------------------"
echo "[titan] get skin"
echo "[titan]--------------------------------------------------------"

rm -rf "$HOME"/flashimg/$SRCDIR/skin
mkdir "$HOME"/flashimg/$SRCDIR/skin

# atemio6200 ??
#if [ "$TYPE" = "atemio5200" ];then
#	CREATE=0
#    svn co --username $SVNUSER --password $SVNPASS http://"$SVNURL"/svn/ipk/source/skinsdefault_defnominitv/_path_/usr/local/share/titan/skin/defnominitv "$HOME"/flashimg/$SRCDIR/skin/default
#else
	CREATE=1
    svn co --username $SVNUSER --password $SVNPASS http://"$SVNURL"/svn/ipk/source/skinsdefault_default/_path_/usr/local/share/titan/skin/default "$HOME"/flashimg/$SRCDIR/skin/default
	if [ "$TYPE" = "atemio5200" ];then
		cp -a "$HOME"/flashimg/$SRCDIR/skin/default/skinconfig "$HOME"/flashimg/$SRCDIR/skin/default/skinconfig.minitv
		mv -f "$HOME"/flashimg/$SRCDIR/skin/default/skinconfig.nominitv "$HOME"/flashimg/$SRCDIR/skin/default/skinconfig
	fi
#fi


echo "[titan]--------------------------------------------------------"
echo "[titan] get skin done"
echo "[titan]--------------------------------------------------------"

echo "[titan]--------------------------------------------------------"
echo "[titan] get settings"
echo "[titan]--------------------------------------------------------"
rm -rf "$HOME"/flashimg/$SRCDIR/settings.svn
if [ "$TYPE" = "ufs910" ] || [ "$TYPE" = "ufs922" ];then
    svn co --username $SVNUSER --password $SVNPASS http://"$SVNURL"/svn/ipk/source/settings_default_sat_1_0/mnt/settings "$HOME"/flashimg/$SRCDIR/settings.svn
else
    svn co --username $SVNUSER --password $SVNPASS http://"$SVNURL"/svn/ipk/source/settings_default_all_2_0/mnt/settings "$HOME"/flashimg/$SRCDIR/settings.svn
fi
sort -u "$HOME"/flashimg/$SRCDIR/settings.svn/channel > "$HOME"/flashimg/$SRCDIR/settings.svn/channel.sort
mv -f "$HOME"/flashimg/$SRCDIR/settings.svn/channel.sort "$HOME"/flashimg/$SRCDIR/settings.svn/channel
sed s/"^ *"// -i "$HOME"/flashimg/$SRCDIR/settings.svn/channel

echo "[titan]--------------------------------------------------------"
echo "[titan] get settings done"
echo "[titan]--------------------------------------------------------"

cp -a "$HOME"/flashimg/$SRCDIR/skins/[^.]* "$HOME"/flashimg/$SRCDIR/plugins

echo "[titan]--------------------------------------------------------"
echo "[titan] cleanup"
echo "[titan]--------------------------------------------------------"

cd "$HOME"/flashimg/$SRCDIR/titan
rm -rf titan
rm -rf titan.o
#rm -rf `find "$HOME"/flashimg/$SRCDIR/plugins -type f -name "*.o"`
#rm -rf `find "$HOME"/flashimg/$SRCDIR/plugins -type f -name "*.so"`

echo "[titan]--------------------------------------------------------"
echo "[titan] cleanup done"
echo "[titan]--------------------------------------------------------"

#echo "[titan]--------------------------------------------------------"
#echo "[titan] Make Securety"
#echo "[titan]--------------------------------------------------------"

"$HOME"/flashimg/$SRCDIR/titan/tools/gettitancode.sh $KERNELDIR $ROOTDIR $TYPE $SRCDIR $CPU $STM $BOXNAME

#echo "[titan]--------------------------------------------------------"
#echo "[titan] Security done"
#echo "[titan]--------------------------------------------------------"

echo "[titan]--------------------------------------------------------"
echo "[titan] Make Tpkdir"
echo "[titan]--------------------------------------------------------"

"$HOME"/flashimg/$SRCDIR/titan/tools/gettpk.sh $IPKDIR $VERSION $SRCDIR

echo "[titan]--------------------------------------------------------"
echo "[titan] Tpkdir done"
echo "[titan]--------------------------------------------------------"

echo "[titan]--------------------------------------------------------"
echo "[titan] update git cdkroot"
echo "[titan]--------------------------------------------------------"

echo "[titan]--------------------------------------------------------"
echo "[titan] Make titan"
echo "[titan]--------------------------------------------------------"


eplayer=EPLAYER4
eplayerlib=gstreamer-1.0
eplayerinclude="$SRC/tmp/sysroots/$BOX/usr/include/$eplayerlib
        -I$SRC/tmp/sysroots/$BOX/usr/lib/$eplayerlib/include
        -I$SRC/tmp/sysroots/$BOX/usr/include/glib-2.0
        -I$SRC/tmp/sysroots/$BOX/usr/include/libxml2
        -I$SRC/tmp/sysroots/$BOX/usr/lib/glib-2.0/include"

if [ "$GROUP" = "dev" ]; then
#    devflag="-finstrument-functions -rdynamic -DBETA"
    devflag="-DBETA"
else
    devflag=""
fi

#if [ "$DISTRO" = "atemio" ];then
    GCCPATH=mipsel-oe-linux
#else
#   GCCPATH=mips32el-oe-linux
#fi

cd "$HOME"/flashimg/$SRCDIR/titan
pwd

#if [ "$DISTRO" != "titannit" ];then
#   echo "$SRC/tmp/sysroots/$ARCH/usr/bin/$GCCPATH/mipsel-oe-linux-gcc -DMIPSEL -D$DREAM -DCAMSUPP -D$eplayer -Os -mhard-float -export-dynamic -Wall -Wno-unused-but-set-variable -Wno-implicit-function-declaration \
#       -I $SRC/tmp/sysroots/$BOX/usr/include/freetype2 \
#       -I $SRC/tmp/sysroots/$BOX/usr/include \
#       -I $eplayerinclude \
#       -I $SRC/tmp/sysroots/$BOX/usr/include/dreamdvd \
#       -I "$HOME"/flashimg/$SRCDIR/libdreamdvd \
#       -I "$HOME" \
#       -c titan.c
#       "
#
#   $SRC/tmp/sysroots/$ARCH/usr/bin/$GCCPATH/mipsel-oe-linux-gcc -DMIPSEL -D$DREAM -DCAMSUPP -D$eplayer -Os $devflag -mhard-float -export-dynamic -Wall -Wno-unused-but-set-variable -Wno-implicit-function-declaration \
#       -I $SRC/tmp/sysroots/$BOX/usr/include/freetype2 \
#       -I $SRC/tmp/sysroots/$BOX/usr/include \
#       -I $eplayerinclude \
#       -I $SRC/tmp/sysroots/$BOX/usr/include/dreamdvd \
#       -I "$HOME"/flashimg/$SRCDIR/libdreamdvd \
#       -I "$HOME" \
#       -c titan.c
#
#   echo "$SRC/tmp/sysroots/$ARCH/usr/bin/$GCCPATH/mipsel-oe-linux-gcc -Os -mhard-float -export-dynamic -lm -lglib-2.0 -lgobject-2.0 -lpthread -ldl -lz -lpng -lfreetype -l$eplayerlib -ldreamdvd -ljpeg -Wall \
#       titan.o \
#       -o titan
#       "
#
#   $SRC/tmp/sysroots/$ARCH/usr/bin/$GCCPATH/mipsel-oe-linux-gcc -Os -mhard-float $devflag -export-dynamic -lm -lglib-2.0 -lgobject-2.0 -lpthread -ldl -lz -lpng -lfreetype -l$eplayerlib -ldreamdvd -ljpeg -Wall \
#       titan.o \
#       -o titan


#/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihde/tmp/sysroots/x86_64-linux/usr/bin/mipsel-oe-linux/mipsel-oe-linux-gcc  -mel -mabi=32 -mhard-float -march=mips32 --sysroot=/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/sysroots/inihdp -DPACKAGE_NAME=\"tuxbox-apps-titan\" -DPACKAGE_TARNAME=\"tuxbox-titan\" -DPACKAGE_VERSION=\"0.0.1\" -DPACKAGE_STRING=\"tuxbox-apps-titan\ 0.0.1\" -DPACKAGE_BUGREPORT=\"\" -DPACKAGE_URL=\"\" -DPACKAGE=\"tuxbox-titan\" -DVERSION=\"0.0.1\" -DSTDC_HEADERS=1 -DHAVE_SYS_TYPES_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_MEMORY_H=1 -DHAVE_STRINGS_H=1 -DHAVE_INTTYPES_H=1 -DHAVE_STDINT_H=1 -DHAVE_UNISTD_H=1 -DHAVE_DLFCN_H=1 -DLT_OBJDIR=\".libs/\" -I.    -DMIPSEL -D$DREAM -DCAMSUPP -DEPLAYER4 -Os -mhard-float -export-dynamic -Wall -Wno-unused-but-set-variable -Wno-implicit-function-declaration -I/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/sysroots/inihdp/usr/include   -I/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/sysroots/inihdp/usr/include/$eplayerlib     -I/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/sysroots/inihdp/usr/include/glib-2.0   -I/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/sysroots/inihdp/usr/include/libxml2    -I/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/sysroots/inihdp/usr/lib/glib-2.0/include   -I/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/sysroots/inihdp/usr/include/freetype2  -I/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/sysroots/inihdp/usr/include/dreamdvd   -I/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/sysroots/inihdp/usr/include/libdreamdvd    -I/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/work/inihdp-oe-linux/titan-bin/2.0+svnr32459-r1/titan/libdreamdvd  -I/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/work/inihdp-oe-linux/titan-bin/2.0+svnr32459-r1/titan/titan -MT titan-titan.o -DMIPSEL -D$DREAM -DCAMSUPP -DEPLAYER4 -MD -MP -MF .deps/titan-titan.Tpo -c -o titan-titan.o `test -f 'titan.c' || echo './'`titan.c
#/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/sysroots/inihdp/usr/bin/crossscripts/mipsel-oe-linux-libtool  --tag=CC   --mode=link /home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihde/tmp/sysroots/x86_64-linux/usr/bin/mipsel-oe-linux/mipsel-oe-linux-gcc  -mel -mabi=32 -mhard-float -march=mips32 --sysroot=/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/sysroots/inihdp -DMIPSEL -D$DREAM -DCAMSUPP -DEPLAYER4 -Os -mhard-float -export-dynamic -Wall -Wno-unused-but-set-variable -Wno-implicit-function-declaration -I/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/sysroots/inihdp/usr/include  -I/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/sysroots/inihdp/usr/include/$eplayerlib     -I/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/sysroots/inihdp/usr/include/glib-2.0   -I/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/sysroots/inihdp/usr/include/libxml2    -I/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/sysroots/inihdp/usr/lib/glib-2.0/include   -I/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/sysroots/inihdp/usr/include/freetype2  -I/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/sysroots/inihdp/usr/include/dreamdvd   -I/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/sysroots/inihdp/usr/include/libdreamdvd    -I/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/work/inihdp-oe-linux/titan-bin/2.0+svnr32459-r1/titan/libdreamdvd  -I/home/atemio/flashimg/BUILDGIT/checkout_mips360/builds/titannit/inihdp/tmp/work/inihdp-oe-linux/titan-bin/2.0+svnr32459-r1/titan/titan -DMIPSEL -D$DREAM -DCAMSUPP -DEPLAYER4 -Wl,-O1  -Wl,--as-needed -o titan titan-titan.o -DMIPSEL -D$DREAM -DCAMSUPP -lm -lglib-2.0 -lgobject-2.0 -lpthread -ldl -lz -lpng -lfreetype -l$eplayerlib -ldreamdvd -ljpeg

    mkdir .deps
    $SRC/tmp/sysroots/$ARCH/usr/bin/mipsel-oe-linux/mipsel-oe-linux-gcc -mel -mabi=32 -mhard-float -march=mips32 --sysroot=$SRC/tmp/sysroots/$BOX -DPACKAGE_NAME=\"tuxbox-apps-titan\" -DPACKAGE_TARNAME=\"tuxbox-titan\" -DPACKAGE_VERSION=\"0.0.1\" -DPACKAGE_STRING=\"tuxbox-apps-titan\ 0.0.1\" -DPACKAGE_BUGREPORT=\"\" -DPACKAGE_URL=\"\" -DPACKAGE=\"tuxbox-titan\" -DVERSION=\"0.0.1\" -DSTDC_HEADERS=1 -DHAVE_SYS_TYPES_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_MEMORY_H=1 -DHAVE_STRINGS_H=1 -DHAVE_INTTYPES_H=1 -DHAVE_STDINT_H=1 -DHAVE_UNISTD_H=1 -DHAVE_DLFCN_H=1 -DLT_OBJDIR=\".libs/\" -I.    -DDVDPLAYER -DMIPSEL -D$DREAM -DCAMSUPP -DEPLAYER4 -Os -mhard-float $devflag -export-dynamic -Wall -Wno-unused-but-set-variable -Wno-implicit-function-declaration -I$SRC/tmp/sysroots/$BOX/usr/include     -I$SRC/tmp/sysroots/$BOX/usr/include/$eplayerlib     -I$SRC/tmp/sysroots/$BOX/usr/lib/$eplayerlib/include     -I$SRC/tmp/sysroots/$BOX/usr/include/glib-2.0   -I$SRC/tmp/sysroots/$BOX/usr/include/libxml2    -I$SRC/tmp/sysroots/$BOX/usr/lib/glib-2.0/include   -I$SRC/tmp/sysroots/$BOX/usr/include/freetype2  -I$SRC/tmp/sysroots/$BOX/usr/include/dreamdvd   -I$SRC/tmp/sysroots/$BOX/usr/include/libdreamdvd    -I$SRC/tmp/sysroots/$BOX/usr/include -I"$HOME"/flashimg/source.titan/libdreamdvd     -I"$HOME"/flashimg/source.titan/titan -MT titan-titan.o -DMIPSEL -D$DREAM -DCAMSUPP -DEPLAYER4 -MD -MP -MF .deps/titan-titan.Tpo -c -o titan-titan.o `test -f 'titan.c' || echo './'`titan.c
    $SRC/tmp/sysroots/$BOX/usr/bin/crossscripts/mipsel-oe-linux-libtool  --tag=CC   --mode=link $SRC/tmp/sysroots/$ARCH/usr/bin/mipsel-oe-linux/mipsel-oe-linux-gcc  -mel -mabi=32 -mhard-float -march=mips32 --sysroot=$SRC/tmp/sysroots/$BOX -DMIPSEL -D$DREAM -DCAMSUPP -DEPLAYER4 -Os -mhard-float $devflag -export-dynamic -Wall -Wno-unused-but-set-variable -Wno-implicit-function-declaration -I$SRC/tmp/sysroots/$BOX/usr/include   -I$SRC/tmp/sysroots/$BOX/usr/include/$eplayerlib     -I$SRC/tmp/sysroots/$BOX/usr/lib/$eplayerlib/include     -I$SRC/tmp/sysroots/$BOX/usr/include/glib-2.0   -I$SRC/tmp/sysroots/$BOX/usr/include/libxml2    -I$SRC/tmp/sysroots/$BOX/usr/lib/glib-2.0/include   -I$SRC/tmp/sysroots/$BOX/usr/include/freetype2  -I$SRC/tmp/sysroots/$BOX/usr/include/openssl  -I$SRC/tmp/sysroots/$BOX/usr/include/dreamdvd   -I$SRC/tmp/sysroots/$BOX/usr/include/libdreamdvd    -I"$HOME"/flashimg/source.titan/libdreamdvd     -I"$HOME"/flashimg/source.titan/titan -DMIPSEL -D$DREAM -DCAMSUPP -DEPLAYER4 -Wl,-O1  -Wl,--as-needed -o titan titan-titan.o -DDVDPLAYER -DMIPSEL -D$DREAM -DCAMSUPP -lm -lglib-2.0 -lgobject-2.0 -lpthread -ldl -lz -lpng -lfreetype -l$eplayerlib -ldreamdvd -ljpeg -lssl -lcrypto
    $SRC/tmp/sysroots/$ARCH/usr/bin/$GCCPATH/mipsel-oe-linux-strip titan

#branch 3.3
#$SRC/tmp/sysroots/$ARCH/usr/bin/mipsel-oe-linux/mipsel-oe-linux-gcc  -mel -mabi=32 -mhard-float -march=mips32 --sysroot=$SRC/tmp/sysroots/$BOX -DPACKAGE_NAME=\"tuxbox-apps-titan\" -DPACKAGE_TARNAME=\"tuxbox-titan\" -DPACKAGE_VERSION=\"0.0.1\" -DPACKAGE_STRING=\"tuxbox-apps-titan\ 0.0.1\" -DPACKAGE_BUGREPORT=\"\" -DPACKAGE_URL=\"\" -DPACKAGE=\"tuxbox-titan\" -DVERSION=\"0.0.1\" -DSTDC_HEADERS=1 -DHAVE_SYS_TYPES_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_MEMORY_H=1 -DHAVE_STRINGS_H=1 -DHAVE_INTTYPES_H=1 -DHAVE_STDINT_H=1 -DHAVE_UNISTD_H=1 -DHAVE_DLFCN_H=1 -DLT_OBJDIR=\".libs/\" -I.    -DMIPSEL -D$DREAM -DCAMSUPP -DEPLAYER4 -Os -mhard-float $devflag -export-dynamic -Wall -Wno-unused-but-set-variable -Wno-implicit-function-declaration -I$SRC/tmp/sysroots/$BOX/usr/include 	-I$SRC/tmp/sysroots/$BOX/usr/lib/gstreamer-1.0/include 	-I$SRC/tmp/sysroots/$BOX/usr/include/gstreamer-1.0 	-I$SRC/tmp/sysroots/$BOX/usr/include/glib-2.0 	-I$SRC/tmp/sysroots/$BOX/usr/include/libxml2 	-I$SRC/tmp/sysroots/$BOX/usr/lib/glib-2.0/include 	-I$SRC/tmp/sysroots/$BOX/usr/include/freetype2 	-I$SRC/tmp/sysroots/$BOX/usr/include/dreamdvd 	-I$SRC/tmp/sysroots/$BOX/usr/include/libdreamdvd 	-I"$HOME"/flashimg/source.titan/libdreamdvd     -I"$HOME"/flashimg/source.titan/titan      -MT titan-titan.o -MD -MP -MF .deps/titan-titan.Tpo -c -o titan-titan.o `test -f 'titan.c' || echo './'`titan.c
#mv -f .deps/titan-titan.Tpo .deps/titan-titan.Po
#$SRC/tmp/sysroots/$BOX/usr/bin/crossscripts/mipsel-oe-linux-libtool  --tag=CC   --mode=link $SRC/tmp/sysroots/$ARCH/usr/bin/mipsel-oe-linux/mipsel-oe-linux-gcc  -mel -mabi=32 -mhard-float -march=mips32 --sysroot=$SRC/tmp/sysroots/$BOX -DMIPSEL -D$DREAM -DCAMSUPP -DEPLAYER4 -Os -mhard-float $devflag -export-dynamic -Wall -Wno-unused-but-set-variable -Wno-implicit-function-declaration -I$SRC/tmp/sysroots/$BOX/usr/include 	-I$SRC/tmp/sysroots/$BOX/usr/lib/gstreamer-1.0/include 	-I$SRC/tmp/sysroots/$BOX/usr/include/gstreamer-1.0 	-I$SRC/tmp/sysroots/$BOX/usr/include/glib-2.0 	-I$SRC/tmp/sysroots/$BOX/usr/include/libxml2 	-I$SRC/tmp/sysroots/$BOX/usr/lib/glib-2.0/include 	-I$SRC/tmp/sysroots/$BOX/usr/include/freetype2 	-I$SRC/tmp/sysroots/$BOX/usr/include/dreamdvd 	-I$SRC/tmp/sysroots/$BOX/usr/include/libdreamdvd 	-I"$HOME"/flashimg/source.titan/libdreamdvd     -I"$HOME"/flashimg/source.titan/titan       -Wl,-O1  -Wl,--as-needed -o titan titan-titan.o -lm -lglib-2.0 -lgobject-2.0 -lpthread -ldl -lz -lpng -lfreetype -lgstreamer-1.0 -ldreamdvd -ljpeg
#$SRC/tmp/sysroots/$ARCH/usr/bin/$GCCPATH/mipsel-oe-linux-strip titan

    echo "[titan]--------------------------------------------------------"
    echo "[titan] titan done"
    echo "[titan]--------------------------------------------------------"

    if [ "$SRCDIR" = "source.titan.building" ]; then exit 1; fi

    rm -rf "$HOME"/flashimg/BUILD/titan
    rm -rf "$HOME"/flashimg/BUILD/titan-smal

    if [ ! -e ./titan ]; then
        echo "[titan]--------------------------------------------------------"
        echo "[titan] building error !!!"
        echo "[titan] check your src"
        echo "[titan]--------------------------------------------------------"
        touch "$HOME"/flashimg/.ipk-build-error
        exit 1
    fi
#fi

if [ "$HOME" == "/home/aaf-svn" ];then
    echo "[titan]--------------------------------------------------------"
    echo "[titan] manuel build stop po building !!!"
    echo "[titan]--------------------------------------------------------"
    touch "$HOME"/flashimg/.ipk-build-error
    exit 1
fi

echo "[titan]--------------------------------------------------------"
echo "[titan] Make TPK Binary"
echo "[titan]--------------------------------------------------------"

rm -rf "$HOME"/flashimg/$SRCDIR/tpk/tpk
"$HOME"/flashimg/$SRCDIR/tpk/make.sh $SRCDIR

if [ ! -e "$HOME"/flashimg/$SRCDIR/tpk/tpk ]; then
    echo "[titan]--------------------------------------------------------"
    echo "[titan] building error !!!"
    echo "[titan] check your src"
    echo "[titan]--------------------------------------------------------"
    touch "$HOME"/flashimg/.ipk-build-error
    exit 1
fi

echo "[titan]--------------------------------------------------------"
echo "[titan] TPK done"
echo "[titan]--------------------------------------------------------"

#if [ "$DISTRO" != "titannit" ];then

    echo "[titan]--------------------------------------------------------"
    echo "[titan] Make Plugins"
    echo "[titan]--------------------------------------------------------"
    "$HOME"/flashimg/$SRCDIR/plugins/makemipsel.sh "$STM" "$MEDIAFW" "$GROUP" "$BOX" "$DISTRO" "$ARCH" "$SRCDIR"
    echo "[titan]--------------------------------------------------------"
    echo "[titan] Plugins done"
    echo "[titan]--------------------------------------------------------"
#fi

echo "[titan]--------------------------------------------------------"
echo "[titan] Update Language Po files"
echo "[titan]--------------------------------------------------------"
"$HOME"/flashimg/$SRCDIR/titan/tools/createpo.sh "$SVNUSER" "$GROUP" $SRCDIR $TYPE $CREATE
echo "[titan]--------------------------------------------------------"
echo "[titan] Update Language Po files done"
echo "[titan]--------------------------------------------------------"

echo "[titan]--------------------------------------------------------"
echo "[titan] Make Language Po files"
echo "[titan]--------------------------------------------------------"
#"$HOME"/flashimg/$SRCDIR/po/de/LC_MESSAGES/make.sh "$STM"
#"$HOME"/flashimg/$SRCDIR/po/fr/LC_MESSAGES/make.sh "$STM"
#"$HOME"/flashimg/$SRCDIR/po/gr/LC_MESSAGES/make.sh "$STM"
#"$HOME"/flashimg/$SRCDIR/po/it/LC_MESSAGES/make.sh "$STM"
#"$HOME"/flashimg/$SRCDIR/po/nl/LC_MESSAGES/make.sh "$STM"
#"$HOME"/flashimg/$SRCDIR/po/pl/LC_MESSAGES/make.sh "$STM"
#"$HOME"/flashimg/$SRCDIR/po/ru/LC_MESSAGES/make.sh "$STM"
#"$HOME"/flashimg/$SRCDIR/po/vn/LC_MESSAGES/make.sh "$STM"

echo "[titan]--------------------------------------------------------"
echo "[titan] Language Po files done"
echo "[titan]--------------------------------------------------------"

mkdir -p "$HOME"/flashimg/BUILD/titan/var/etc/titan
mkdir -p "$HOME"/flashimg/BUILD/titan/var/etc/codepages
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

#if [ "$DISTRO" != "titannit" ];then
    PLIST=`ls -1 "$HOME"/flashimg/$SRCDIR/plugins`

    for ROUND in $PLIST;do
        if [ "$ROUND" == "makesh4.sh" ]; then
            skiped=1
        else
            echo "[titan] copy plugins/$ROUND"
            cp -a "$HOME"/flashimg/$SRCDIR/plugins/"$ROUND" "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins
        fi
    done
#else
#   cp -a $SRC/tmp/rootfs/$BOX/var/usr/local/share/titan/plugins/* "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins
#fi

cp -a "$HOME"/flashimg/$SRCDIR/help "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan
cp -a "$HOME"/flashimg/$SRCDIR/var.settings/* "$HOME"/flashimg/BUILD/titan/var/etc/titan
cat "$HOME"/flashimg/BUILD/titan/var/etc/titan/titan-merge.mipsel.cfg "$HOME"/flashimg/BUILD/titan/var/etc/titan/titan-merge.all.cfg | sort -u > "$HOME"/flashimg/BUILD/titan/var/etc/titan/titan-merge.cfg
sed 's/&/\\&/g' -i "$HOME"/flashimg/BUILD/titan/var/etc/titan/titan-merge.cfg
rm -rf "$HOME"/flashimg/BUILD/titan/var/etc/titan/titan-merge.*.cfg

mkdir "$HOME"/flashimg/BUILD/titan/var/etc/titan.mnt
cp -a "$HOME"/flashimg/$SRCDIR/mnt.settings/* "$HOME"/flashimg/BUILD/titan/var/etc/titan.mnt
cp -a "$HOME"/flashimg/$SRCDIR/skin "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan
cp -a "$HOME"/flashimg/$SRCDIR/po "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan
cp titan "$HOME"/flashimg/BUILD/titan/usr/local/bin
chmod 755 "$HOME"/flashimg/BUILD/titan/usr/local/bin/titan
cp -a "$HOME"/flashimg/$SRCDIR/picons "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan
cp -a "$HOME"/flashimg/$SRCDIR/web "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan
cp -a "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/rc."$TYPE".html "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/rc.html
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/rc.*.html
cp -a "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/img/rc."$TYPE".png "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/img/rc.png
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/img/rc.*.png
cp -a "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/img/rc_virtual."$TYPE".png "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/img/rc_virtual.png
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/img/rc_virtual.*.png
cp -a "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/rc_virtual."$TYPE".html "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/rc_virtual.html
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/rc_virtual.*.html
# ??????????????
#cp -a "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/index."$TYPE".html "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/index.html
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/index.*.html

cp -a "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgMusic."$TYPE".mvi "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgMusic.mvi
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgMusic.*.mvi
cp -a "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgBrowser."$TYPE".mvi "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgBrowser.mvi
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgBrowser.*.mvi
cp -a "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgPicture."$TYPE".mvi "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgPicture.mvi
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgPicture.*.mvi
cp -a "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgVideo."$TYPE".mvi "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgVideo.mvi
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgVideo.*.mvi
cp -a "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgSettings."$TYPE".mvi "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgSettings.mvi
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgSettings.*.mvi
cp -a "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgWeather."$TYPE".mvi "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgWeather.mvi
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgWeather.*.mvi
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgDVD.*
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgRadio.*
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgRSS.*
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgVLC.*
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/mc/skin/bgWeather.*

tar -zxvf "$HOME"/flashimg/$SRCDIR/netsurf-3.3/netsurf-git-settings.tar.gz -C "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/netsurf
cp -a "$HOME"/flashimg/$SRCDIR/netsurf/netsurf-2.8/framebuffer/res/config/Choices "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/netsurf

rm -f "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/netsurf/Choices
ln -s /mnt/config/choices "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/netsurf/Choices

rm -rf "$HOME"/flashimg/BUILD/titan/usr/lib-all

#crypt
"$HOME"/flashimg/$SRCDIR/titan/tools/ocrypt -c "$HOME"/flashimg/$SRCDIR/titan/tools/trustlist "$HOME"/flashimg/BUILD/titan/var/etc/codepages/codepage.868

rm -rf `find "$HOME"/flashimg/BUILD/titan -type d -name "*.svn"`
rm -rf `find "$HOME"/flashimg/BUILD/titan -type d -name "*.deps"`
rm -rf `find "$HOME"/flashimg/BUILD/titan -type d -name "*.libs"`
rm -rf `find "$HOME"/flashimg/BUILD/titan -type f -name "*.h"`
rm -rf `find "$HOME"/flashimg/BUILD/titan -type f -name "*.c"`
rm -rf `find "$HOME"/flashimg/BUILD/titan -type f -name "*.o"`
rm -rf `find "$HOME"/flashimg/BUILD/titan -type f -name "*.po"`
rm -rf `find "$HOME"/flashimg/BUILD/titan -type f -name "*.a"`
rm -rf `find "$HOME"/flashimg/BUILD/titan -type f -name "*.la"`
rm -rf `find "$HOME"/flashimg/BUILD/titan -type f -name "*.lo"`
rm -rf `find "$HOME"/flashimg/BUILD/titan -type f -name "*.Tpo"`
rm -rf `find "$HOME"/flashimg/BUILD/titan -type f -name "make.sh"`
rm -rf `find "$HOME"/flashimg/BUILD/titan -type f -name "Makefile.am"`
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/networkbrowser/netlib
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/tithek/menu
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/tithek/tithekmainmenu

cp -a "$HOME"/flashimg/BUILD/titan/var/etc/titan.mnt/titan."$TYPE".cfg "$HOME"/flashimg/BUILD/titan/var/etc/titan.mnt/titan.cfg
rm -rf "$HOME"/flashimg/BUILD/titan/var/etc/titan.mnt/titan.*.cfg

cp -a "$HOME"/flashimg/BUILD/titan/var/etc/titan.mnt/rcconfig."$TYPE" "$HOME"/flashimg/BUILD/titan/var/etc/titan.mnt/rcconfig
rm -rf "$HOME"/flashimg/BUILD/titan/var/etc/titan.mnt/rcconfig.*

rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/skin/default/skin/*_grid.png
# mc grid filelist pngs
#rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/skin/default/skin/*_grid_*.png

cp -a "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/skin/default/skinconfig* "$HOME"/flashimg/BUILD/titan/var/etc/titan.mnt/

cp -a "$HOME"/flashimg/BUILD/titan "$HOME"/flashimg/BUILD/titan-smal
rm -rf "$HOME"/flashimg/BUILD/titan-smal/var/usr/local/share/titan/picons
rm -rf "$HOME"/flashimg/BUILD/titan-smal/var/etc/titan/titan.cfg

echo "[titan]--------------------------------------------------------"
echo "[titan] building to: "$HOME"/flashimg/BUILD/titan"
echo "[titan] copy this folder to duckbox: /var/usr/local/share/titan"
echo "[titan]--------------------------------------------------------"
