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
SRCDIR=${16}
CPU=${17}
BOXNAME=${18}
TYPE1=${19}
CREATE=1

rm "$HOME"/flashimg/.ipk-build-error

# work
#mv -f "$HOME"/flashimg/$SRCDIR/titan/struct.mipsel.h "$HOME"/flashimg/$SRCDIR/titan/struct.h

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
#svn co --username $SVNUSER --password $SVNPASS http://"$SVNURL"/svn/ipk/source/skins_default/_path_/usr/local/share/titan/skin/default "$HOME"/flashimg/$SRCDIR/skin/default
cp -a "$HOME"/flashimg/$SRCDIR/skins/default "$HOME"/flashimg/$SRCDIR/skin/default

echo sh4 remove oled stuff
rm -rf "$HOME"/flashimg/$SRCDIR/skin/default/oled
rm -f "$HOME"/flashimg/$SRCDIR/skin/default/oledskin.xml

echo "[titan]--------------------------------------------------------"
echo "[titan] get skin done"
echo "[titan]--------------------------------------------------------"

echo "[titan]--------------------------------------------------------"
echo "[titan] get settings"
echo "[titan]--------------------------------------------------------"
rm -rf "$HOME"/flashimg/$SRCDIR/settings.svn
if [ "$TYPE" = "ufs910" ] || [ "$TYPE" = "ufs922" ] || [ "$TYPE" = "ufc960" ];then
    svn co --username $SVNUSER --password $SVNPASS http://"$SVNURL"/svn/ipk/source/settings_default_sat_1_0/mnt/settings "$HOME"/flashimg/$SRCDIR/settings.svn
elif [ "$TYPE1" = "homecastpro-sat" ];then
    svn co --username $SVNUSER --password $SVNPASS http://"$SVNURL"/svn/ipk/source/settings_orf_1xsat_1_0/mnt/settings "$HOME"/flashimg/$SRCDIR/settings.svn
elif [ "$TYPE1" = "homecastpro-cable" ];then
    svn co --username $SVNUSER --password $SVNPASS http://"$SVNURL"/svn/ipk/source/settings_kabelplus_cable_1_0/mnt/settings "$HOME"/flashimg/$SRCDIR/settings.svn
else
    svn co --username $SVNUSER --password $SVNPASS http://"$SVNURL"/svn/ipk/source/settings_default_all_2_0/mnt/settings "$HOME"/flashimg/$SRCDIR/settings.svn
#    cp -a "$HOME"/flashimg/$SRCDIR/settings "$HOME"/flashimg/$SRCDIR/settings.svn
fi
sort -u "$HOME"/flashimg/$SRCDIR/settings.svn/channel > "$HOME"/flashimg/$SRCDIR/settings.svn/channel.sort
mv -f "$HOME"/flashimg/$SRCDIR/settings.svn/channel.sort "$HOME"/flashimg/$SRCDIR/settings.svn/channel
sed s/"^ *"// -i "$HOME"/flashimg/$SRCDIR/settings.svn/channel

echo "[titan]--------------------------------------------------------"
echo "[titan] get settings done"
echo "[titan]--------------------------------------------------------"

#cp -a "$HOME"/flashimg/$SRCDIR/skins/[^.]* "$HOME"/flashimg/$SRCDIR/plugins

echo "[titan]--------------------------------------------------------"
echo "[titan] cleanup"
echo "[titan]--------------------------------------------------------"

cd "$HOME"/flashimg/$SRCDIR/titan
rm -rf titan
rm -rf titan.o
rm -rf `find "$HOME"/flashimg/$SRCDIR/plugins -type f -name "*.o"`
rm -rf `find "$HOME"/flashimg/$SRCDIR/plugins -type f -name "*.so"`

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

echo "$HOME"/flashimg/$SRCDIR/titan/tools/gettpk.sh $IPKDIR $VERSION $SRCDIR
"$HOME"/flashimg/$SRCDIR/titan/tools/gettpk.sh $IPKDIR $VERSION $SRCDIR

echo "[titan]--------------------------------------------------------"
echo "[titan] Tpkdir done"
echo "[titan]--------------------------------------------------------"

if [ $BUILDTYPE == 10 ] || [ $BUILDTYPE == 11 ]; then
    #echo "[titan]--------------------------------------------------------"
    #echo "[titan] libipkg"
    #echo "[titan]--------------------------------------------------------"
    #cd "$HOME"/flashimg/$SRCDIR/libipkg
    #./makesh4.sh $STM
    #cd "$HOME"/flashimg/$SRCDIR/titan
    #if [ ! -e "$HOME"/flashimg/$SRCDIR/libipkg/.libs/libipkg.so.0.0.0 ]; then
    #   echo "[titan]--------------------------------------------------------"
    #   echo "[titan] ipkg building error !!!"
    #   echo "[titan] check your src"
    #   echo "[titan]--------------------------------------------------------"
    #   touch "$HOME"/flashimg/.ipk-build-error
    #   exit 1
    #fi
    #echo "[titan]--------------------------------------------------------"
    #echo "[titan] libipkg done"
    #echo "[titan]--------------------------------------------------------"
    
    ###
    echo "[titan]--------------------------------------------------------"
    echo "[titan] libdreamdvd"
    echo "[titan]--------------------------------------------------------"
    cd "$HOME"/flashimg/$SRCDIR/libdreamdvd
    ./makesh4.sh $STM
    cd "$HOME"/flashimg/$SRCDIR/titan
    if [ ! -e "$HOME"/flashimg/$SRCDIR/libdreamdvd/.libs/libdreamdvd.so.0.0.0 ]; then
        echo "[titan]--------------------------------------------------------"
        echo "[titan] libdreamdvd building error !!!"
        echo "[titan] check your src"
        echo "[titan]--------------------------------------------------------"
        touch "$HOME"/flashimg/.ipk-build-error
        exit 1
    fi
    cp "$HOME"/flashimg/$SRCDIR/libdreamdvd/.libs/libdreamdvd.so.0.0.0 "$ROOTDIR"/lib
    
    echo "[titan]--------------------------------------------------------"
    echo "[titan] libdreamdvd done"
    echo "[titan]--------------------------------------------------------"
    ###
fi

echo "[titan]--------------------------------------------------------"
echo "[titan] update git cdkroot"
echo "[titan]--------------------------------------------------------"

#cp "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/cvs/apps/misc/tools/libdreamdvd/.libs/* $HOME/flashimg/BUILDGIT/checkout_"$STM"/tufsbox/cdkroot/usr/lib

echo "[titan]--------------------------------------------------------"
echo "[titan] update git cdkroot done"
echo "[titan]--------------------------------------------------------"


echo "[titan]--------------------------------------------------------"
echo "[titan] Make titan"
echo "[titan]--------------------------------------------------------"

# set eplayer as default
# MEDIAFW=1

if [ $MEDIAFW = 1 ]; then
    eplayer=EPLAYER3
    eplayerinclude="$HOME"/flashimg/BUILDGIT/checkout_"$STM"/apps/tools/libeplayer3/include
	linking="-lm -lpthread -ldl -lpng -lfreetype -leplayer3 -ldreamdvd -ljpeg -lmmeimage -lmme_host -lz -lssl -lcrypto -lcurl"
fi
if [ $MEDIAFW = 2 ]; then
    eplayer=EPLAYER4
    eplayerinclude="$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cdkroot/usr/include/gstreamer-1.0
             -I$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cdkroot/usr/include/glib-2.0
             -I$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cdkroot/usr/include/libxml2
             -I$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cdkroot/usr/lib/glib-2.0/include
             -I$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cdkroot/usr/lib/gstreamer-1.0/include"
	linking="-lm -lglib-2.0 -lgobject-2.0 -lgio-2.0 -lpthread -ldl -lz -lpng -lfreetype -lgstreamer-1.0 -ldreamdvd -ljpeg -lmmeimage -lmme_host -lz"
fi
if [ $MEDIAFW = 4 ]; then
    eplayer="EPLAYER3 -DEPLAYER4"
    eplayerinclude="$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cdkroot/usr/include/gstreamer-1.0
             -I$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cdkroot/usr/include/glib-2.0
             -I$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cdkroot/usr/include/libxml2
             -I$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cdkroot/usr/lib/glib-2.0/include
             -I$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cdkroot/usr/lib/gstreamer-1.0/include
			 -I$HOME/flashimg/BUILDGIT/checkout_"$STM"/apps/tools/libeplayer3/include"
	linking="-lm -lglib-2.0 -lgobject-2.0 -lgio-2.0 -lpthread -ldl -lz -lpng -lfreetype -lgstreamer-1.0 -leplayer3 -ldreamdvd -ljpeg -lmmeimage -lmme_host -lz"
fi

#if [ "$GROUP" = "dev" ] && ([ "$TYPE" = "spark" ] || [ "$TYPE" = "spark7162" ]); then
if [ "$TYPE" = "spark_aus" ] || [ "$TYPE" = "spark7162_aus" ]; then
    devflag="-finstrument-functions -rdynamic -DBETA -DDDTBUILD -DEPLAYER3"
elif [ "$GROUP" = "dev" ] && [ "$TYPE" != "ufs910" ] && [ "$TYPE" != "ufs922" ]; then
    devflag="-finstrument-functions -rdynamic -DBETA -DDDTBUILD -DEXTEPLAYER3"
elif [ "$GROUP" = "dev" ]; then
    devflag="-DBETA -DDDTBUILD -DEXTEPLAYER3"
else
    devflag="-DDDTBUILD -DEXTEPLAYER3"
fi

 #ffmpeg=3.4.2
ffmpeg=3.2
if [ "$GROUP" = "dev" ] && [ "$TYPE" = "ufs912" ] && [ "$ffmpeg" = "3.4.2" ];then
	sed 's!typedef int bool;!//typedef int bool;!' -i "$HOME"/flashimg/$SRCDIR/titan/bcm.h
	sed 's!enum { false, true };!//enum { false, true };!' -i "$HOME"/flashimg/$SRCDIR/titan/bcm.h
fi

#if [ "$GROUP" = "dev" ];then
	"$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tufsbox/cross/bin/sh4-linux-gcc -DSH4 -D$eplayer -DDVDPLAYER -Os $devflag -export-dynamic -Wall -Wno-unused-but-set-variable \
   -I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tufsbox/cdkroot/usr/include \
    -I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tufsbox/cdkroot/usr/include/freetype2 \
    -I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tufsbox/cdkroot/usr/include/openssl \
    -I $eplayerinclude \
    -I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/cdk/source/titan/titan/libdreamdvd \
    -I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/driver/bpamem \
	-I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/apps/tools/libmme_image  \
	-I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/cdk/build_tmp/linux-sh4-2.6.32.71_stm24_0217/include  \
    -I "$HOME"/flashimg/$SRCDIR \
    -c titan.c
#else
#	"$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tufsbox/cross/bin/sh4-linux-gcc -DSH4 -D$eplayer -DDVDPLAYER -Os $devflag -export-dynamic -Wall -Wno-unused-but-set-variable \
#    -I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tufsbox/cdkroot/usr/include \
#    -I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tufsbox/cdkroot/usr/include/freetype2 \
#    -I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tufsbox/cdkroot/usr/include/openssl \
#    -I $eplayerinclude \
#    -I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/apps/titan/libdreamdvd \
#    -I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/driver/bpamem \
#	-I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/apps/tools/libmme_image  \
#	-I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/cdk/linux-sh4-2.6.32.61_stm24_0217/include  \
#    -I "$HOME"/flashimg/$SRCDIR \
#    -c titan.c
#fi

/bin/sh "$HOME"/flashimg/BUILDGIT/checkout_$STM/cdk/source/titan/titan/libtool --tag=CC   --mode=link "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tufsbox/cross/bin/sh4-linux-gcc -DSH4 -D$eplayer -DDVDPLAYER -Os -export-dynamic -Wall -Wno-unused-but-set-variable -pipe -Os  -Wl,-rpath -Wl,/usr/lib -Wl,-rpath-link -Wl,"$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tufsbox/cdkroot/usr/lib -L"$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tufsbox/cdkroot/usr/lib -L"$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tufsbox/cdkroot/lib -o titan titan.o $linking
cp "$HOME"/flashimg/$SRCDIR/titan/.libs/titan "$HOME"/flashimg/$SRCDIR/titan
"$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tufsbox/cross/bin/sh4-linux-strip titan

################

echo "[titan]--------------------------------------------------------"
echo "[titan] titan done"
echo "[titan]--------------------------------------------------------"

if [ "$HOME" == "/home/aaf-svn" ];then
    echo "[titan]--------------------------------------------------------"
    echo "[titan] manuel build stop po building !!!"
    echo "[titan]--------------------------------------------------------"
    touch "$HOME"/flashimg/.ipk-build-error
    exit 1
fi

if [ "$SRCDIR" = "source.titan.building" ]; then exit 1; fi

rm -rf "$HOME"/flashimg/BUILD/titan
rm -rf "$HOME"/flashimg/BUILD/titan-smal

if [ ! -e ./titan ]; then
    echo "[titan]--------------------------------------------------------"
    echo "[titan] building titan bin error !!!"
    echo "[titan] check your src"
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
    echo "[titan] building tpk bin error !!!"
    echo "[titan] check your src"
    echo "[titan]--------------------------------------------------------"
    touch "$HOME"/flashimg/.ipk-build-error
    exit 1
fi

echo "[titan]--------------------------------------------------------"
echo "[titan] TPK done"
echo "[titan]--------------------------------------------------------"

echo "[titan]--------------------------------------------------------"
echo "[titan] Make Plugins"
echo "[titan]--------------------------------------------------------"
"$HOME"/flashimg/$SRCDIR/plugins/makesh4.sh "$STM" "$MEDIAFW" "$GROUP" "$BOX" "$DISTRO" "$ARCH" "$SRCDIR"
echo "[titan]--------------------------------------------------------"
echo "[titan] Plugins done"
echo "[titan]--------------------------------------------------------"

echo "[titan]--------------------------------------------------------"
echo "[titan] Update Language Po files"
echo "[titan]--------------------------------------------------------"
"$HOME"/flashimg/$SRCDIR/titan/tools/createpo.sh "$SVNUSER" "$GROUP" $SRCDIR $TYPE $CREATE
echo "[titan]--------------------------------------------------------"
echo "[titan] Update Language Po files done"
echo "[titan]--------------------------------------------------------"

#echo "[titan]--------------------------------------------------------"
#echo "[titan] Make Language Po files"
#echo "[titan]--------------------------------------------------------"
#"$HOME"/flashimg/$SRCDIR/po/de/LC_MESSAGES/make.sh "$STM"
#"$HOME"/flashimg/$SRCDIR/po/fr/LC_MESSAGES/make.sh "$STM"
#"$HOME"/flashimg/$SRCDIR/po/gr/LC_MESSAGES/make.sh "$STM"
#"$HOME"/flashimg/$SRCDIR/po/it/LC_MESSAGES/make.sh "$STM"
#"$HOME"/flashimg/$SRCDIR/po/nl/LC_MESSAGES/make.sh "$STM"
#"$HOME"/flashimg/$SRCDIR/po/pl/LC_MESSAGES/make.sh "$STM"
#"$HOME"/flashimg/$SRCDIR/po/ru/LC_MESSAGES/make.sh "$STM"
#"$HOME"/flashimg/$SRCDIR/po/vn/LC_MESSAGES/make.sh "$STM"
#"$HOME"/flashimg/$SRCDIR/po/en/LC_MESSAGES/make.sh "$STM"

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

PLIST=`ls -1 "$HOME"/flashimg/$SRCDIR/plugins`

for ROUND in $PLIST;do
    if [ "$ROUND" == "makesh4.sh" ]; then
        skiped=1
    else
        echo "[titan] copy plugins/$ROUND" 
        cp -a "$HOME"/flashimg/$SRCDIR/plugins/"$ROUND" "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins
    fi
done

cp -a "$HOME"/flashimg/$SRCDIR/help "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan
cp -a "$HOME"/flashimg/$SRCDIR/var.settings/* "$HOME"/flashimg/BUILD/titan/var/etc/titan
cat "$HOME"/flashimg/BUILD/titan/var/etc/titan/titan-merge.sh4.cfg "$HOME"/flashimg/BUILD/titan/var/etc/titan/titan-merge.all.cfg | sort -u > "$HOME"/flashimg/BUILD/titan/var/etc/titan/titan-merge.cfg 
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
#cp -a "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/index."$TYPE".html "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/index.html
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/index.*.html
cp -a "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/rc_virtual."$TYPE".html "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/rc_virtual.html
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/rc_virtual.*.html
echo use system_extensions.ddtbuild.html > system_extensions.html
cp -a "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/system_extensions.ddtbuild.html "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/system_extensions.html
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/web/system_extensions.*.html

cp -a "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/mc/skin/bgMusic."$TYPE".mvi "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/mc/skin/bgMusic.mvi
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/mc/skin/bgMusic.*.mvi
cp -a "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/mc/skin/bgBrowser."$TYPE".mvi "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/mc/skin/bgBrowser.mvi
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/mc/skin/bgBrowser.*.mvi
cp -a "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/mc/skin/bgPicture."$TYPE".mvi "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/mc/skin/bgPicture.mvi
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/mc/skin/bgPicture.*.mvi
cp -a "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/mc/skin/bgVideo."$TYPE".mvi "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/mc/skin/bgVideo.mvi
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/mc/skin/bgVideo.*.mvi
cp -a "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/mc/skin/bgSettings."$TYPE".mvi "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/mc/skin/bgSettings.mvi
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/mc/skin/bgSettings.*.mvi
cp -a "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/mc/skin/bgWeather."$TYPE".mvi "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/mc/skin/bgWeather.mvi
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/mc/skin/bgWeather.*.mvi
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/mc/skin/bgDVD.*
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/mc/skin/bgRadio.*
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/mc/skin/bgRSS.*
#rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/mc/skin/bgSettings.*
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/mc/skin/bgVLC.*
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/mc/skin/bgWeather.*

rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/mc/skin/black.mvi

#if [ "$TYPE" == "ufs910" ] || [ "$TYPE" == "ufs922" ];then
#   rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/mc/skin/bgMusic.*
#   rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/mc/skin/bgBrowser.*
#   rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/mc/skin/bgPicture.*
#   rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/mc/skin/bgSettings.*
#   rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/mc/skin/bgWeather.*
#
#   cd "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/mc/skin/
#   ln -s bgVideo.mvi "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/mc/skin/bgMusic.mvi
#   ln -s bgVideo.mvi "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/mc/skin/bgBrowser.mvi
#   ln -s bgVideo.mvi "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/mc/skin/bgPicture.mvi
#   ln -s bgVideo.mvi "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/mc/skin/bgSettings.mvi
#   ln -s bgVideo.mvi "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/mc/skin/bgWeather.mvi
#   cd "$HOME"/flashimg/$SRCDIR/titan
#fi

cp -a "$HOME"/flashimg/$SRCDIR/netsurf/netsurf-2.8/nsfb "$HOME"/flashimg/BUILD/titan/usr/bin
cp -a "$HOME"/flashimg/$SRCDIR/netsurf/netsurf-2.8/framebuffer/res/config/Choices "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/netsurf
cp -a "$HOME"/flashimg/$SRCDIR/netsurf/netsurf-2.8/framebuffer/res/config/Aliases "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/netsurf
cp -a "$HOME"/flashimg/$SRCDIR/netsurf/netsurf-2.8/framebuffer/res/config/*.css "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/netsurf
cp -a "$HOME"/flashimg/$SRCDIR/netsurf/netsurf-2.8/framebuffer/res/config/messages "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/netsurf

rm -f "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/netsurf/Choices
ln -s /mnt/config/choices "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/netsurf/Choices

#cp -a "$HOME"/flashimg/$SRCDIR/libipkg/.libs/libipkg.so.0.0.0 "$HOME"/flashimg/BUILD/titan/lib/libipkg.so.0
#cp -a "$HOME"/flashimg/$SRCDIR/libipkg/.libs/ipkg-cl "$HOME"/flashimg/BUILD/titan/usr/bin/ipkg

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
#       cp "$HOME"/flashimg/BUILD/titan/usr/lib-all/$ROUND "$HOME"/flashimg/BUILD/titan/usr/lib/$dest
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
#cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/bin/gmediarender "$HOME"/flashimg/BUILD/titan/usr/bin
cp -a "$HOME"/flashimg/$SRCDIR/gmediarender/src/gmediarender "$HOME"/flashimg/BUILD/titan/usr/bin
cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/libupnp.so.6.3.1 "$HOME"/flashimg/BUILD/titan/usr/lib/libupnp.so.6
cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/share/gmediarender "$HOME"/flashimg/BUILD/titan/usr/share

#new net tools
cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/bin/djmount "$HOME"/flashimg/BUILD/titan/sbin
# rarfs last version not working eplayer has playback error
#cp -a "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/bin/rarfs "$HOME"/flashimg/BUILD/titan/sbin
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
#cp -a "$HOME"/flashimg/$SRCDIR/ipkg "$HOME"/flashimg/BUILD/titan/var/usr/lib
#sed "s/Version:.*/Version: $VERSION/" -i "$HOME"/flashimg/BUILD/titan/var/usr/lib/ipkg/status
#sed "s/Version:.*/Version: $VERSION/" -i "$HOME"/flashimg/BUILD/titan/var/usr/lib/ipkg/info/*.control

#crypt
"$HOME"/flashimg/$SRCDIR/titan/tools/ocrypt -c "$HOME"/flashimg/$SRCDIR/titan/tools/trustlist "$HOME"/flashimg/BUILD/titan/var/etc/codepages/codepage.868

rm -rf `find "$HOME"/flashimg/BUILD/titan -type d -name "CONTROL"`
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
rm -rf `find "$HOME"/flashimg/BUILD/titan -type f -name "make*"`
rm -rf `find "$HOME"/flashimg/BUILD/titan -type f -name "Makefile*"`

rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/network/networkbrowser/netlib
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/tithek/menu
rm -rf "$HOME"/flashimg/BUILD/titan/var/usr/local/share/titan/plugins/player/tithek/tithekmainmenu

cp -a "$HOME"/flashimg/BUILD/titan/var/etc/titan.mnt/titan."$TYPE".cfg "$HOME"/flashimg/BUILD/titan/var/etc/titan.mnt/titan.cfg
cp -a "$HOME"/flashimg/BUILD/titan/var/etc/titan.mnt/titan."$TYPE1".cfg "$HOME"/flashimg/BUILD/titan/var/etc/titan.mnt/titan.cfg
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
