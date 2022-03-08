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
	devflag="-finstrument-functions -rdynamic -DBETA"
else
	devflag=""
fi

cd "$HOME"/flashimg/$SRCDIR/plugins
ln -s "$HOME"/flashimg/$SRCDIR/titan "$HOME"/flashimg/$SRCDIR/plugins/titan

if [ $MEDIAFW = 1 ]; then
    eplayer=EPLAYER3
    eplayerinclude="$HOME"/flashimg/BUILDGIT/checkout_"$STM"/apps/tools/libeplayer3/include
	linking="-lm -lpthread -ldl -lpng -lfreetype -leplayer3 -ldreamdvd -ljpeg -lmmeimage -lmme_host -lz -lssl -lcrypto"
fi
if [ $MEDIAFW = 2 ]; then
    eplayer=EPLAYER4
    eplayerinclude="$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cdkroot/usr/include/gstreamer-1.0
             -I$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cdkroot/usr/include/glib-2.0
             -I$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cdkroot/usr/include/libxml2
             -I$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cdkroot/usr/lib/glib-2.0/include
             -I$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cdkroot/usr/lib/gstreamer-1.0/include"
	linking="-lm -lglib-2.0 -lgobject-2.0 -lgio-2.0 -lpthread -ldl -lz -lpng -lfreetype -lgstreamer-1.0 -ldreamdvd -ljpeg -lmmeimage -lmme_host -lz -lssl -lcrypto"
fi
if [ $MEDIAFW = 4 ]; then
    eplayer="EPLAYER3 -DEPLAYER4"
    eplayerinclude="$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cdkroot/usr/include/gstreamer-1.0
             -I$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cdkroot/usr/include/glib-2.0
             -I$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cdkroot/usr/include/libxml2
             -I$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cdkroot/usr/lib/glib-2.0/include
             -I$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cdkroot/usr/lib/gstreamer-1.0/include
             -I$HOME/flashimg/BUILDGIT/checkout_"$STM"/apps/tools/libeplayer3/include"
    eplayerlib=gstreamer-1.0
	linking="-lm -lglib-2.0 -lgobject-2.0 -lgio-2.0 -lpthread -ldl -lz -lpng -lfreetype -lgstreamer-1.0 -leplayer3 -ldreamdvd -ljpeg -lmmeimage -lmme_host -lz -lssl -lcrypto"
fi

if [ "$GROUP" = "dev" ] && [ "$TYPE" != "ufs910" ] && [ "$TYPE" != "ufs922" ]; then
    devflag="-finstrument-functions -rdynamic -DBETA -DEXTEPLAYER3"
elif [ "$GROUP" = "dev" ]; then
    devflag="-DBETA -DEXTEPLAYER3"
else
    devflag="-DEXTEPLAYER3"
fi


#if [ "$GROUP" = "dev" ];then
	eplayerinclude="$eplayerinclude
    -I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tufsbox/cdkroot/usr/include
    -I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tufsbox/cdkroot/usr/include/freetype2
    -I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tufsbox/cdkroot/usr/include/openssl
    -I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/apps/titan/libdreamdvd
    -I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/driver/bpamem
	-I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/apps/tools/libmme_image
	-I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/build_tmp/linux-sh4-2.6.32.71_stm24_0217/include
    -I "$HOME"/flashimg/$SRCDIR"
#else
#	eplayerinclude="$eplayerinclude
#    -I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tufsbox/cdkroot/usr/include
#    -I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tufsbox/cdkroot/usr/include/freetype2
#    -I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tufsbox/cdkroot/usr/include/openssl
#    -I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/apps/titan/libdreamdvd
#    -I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/driver/bpamem
#	-I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/apps/tools/libmme_image
#	-I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/cdk/linux-sh4-2.6.32.61_stm24_0217/include
#    -I "$HOME"/flashimg/$SRCDIR"
#fi

compile()
{
	cd $1
	echo "[titan]--------------------------------------------------------"
	echo "[titan] Make Plugin $1"
	echo "[titan]--------------------------------------------------------"

	LIB=""
	mkdir .deps
	if [ ! -z $3 ]; then
		LIB=$3.a
	fi

	if [ $uselibtool == y ];then
		rm -rf .deps
		rm -rf .libs
		mkdir .deps
		STM=stm24
		cp $HOME/flashimg/BUILDGIT/checkout_$STM/apps/titan/plugins/libtool $HOME/flashimg/source.titan/plugins/libtool
		sed s#^AR=.*#AR=$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cross/bin/sh4-linux-ar# -i $HOME/flashimg/source.titan/plugins/libtool
		sed s#^CC=.*#CC=$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cross/bin/sh4-linux-gcc# -i $HOME/flashimg/source.titan/plugins/libtool
		sed s#^RANLIB=.*#RANLIB=$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cross/bin/sh4-linux-ranlib# -i $HOME/flashimg/source.titan/plugins/libtool
		sed s#^NM=.*#NM=$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cross/bin/sh4-linux-nm# -i $HOME/flashimg/source.titan/plugins/libtool
		sed s#^STRIP=.*#STRIP=$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cross/bin/sh4-linux-strip# -i $HOME/flashimg/source.titan/plugins/libtool
		sed s#^OBJDUMP=.*#OBJDUMP=$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cross/bin/sh4-linux-objdump# -i $HOME/flashimg/source.titan/plugins/libtool
	
		/bin/sh $HOME/flashimg/source.titan/plugins/libtool  --tag=CC   --mode=compile $HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cross/bin/sh4-linux-gcc -DPACKAGE_NAME=\"tuxbox-apps-titan-plugins\" -DPACKAGE_TARNAME=\"tuxbox-titan-plugins\" -DPACKAGE_VERSION=\"0.0.1\" -DPACKAGE_STRING=\"tuxbox-apps-titan-plugins\ 0.0.1\" -DPACKAGE_BUGREPORT=\"\" -DPACKAGE_URL=\"\" -DPACKAGE=\"tuxbox-titan-plugins\" -DVERSION=\"0.0.1\" -DSTDC_HEADERS=1 -DHAVE_SYS_TYPES_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_MEMORY_H=1 -DHAVE_STRINGS_H=1 -DHAVE_INTTYPES_H=1 -DHAVE_STDINT_H=1 -DHAVE_UNISTD_H=1 -DHAVE_DLFCN_H=1 -DLT_OBJDIR=\".libs/\" -I.  -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -pipe -Os -I$eplayerinclude -MT $2.lo -MD -MP -MF .deps/$2.Tpo -c -o $2.lo $2.c
		mv -f .deps/$2.Tpo .deps/$2.Plo
	
		#/bin/sh $HOME/flashimg/source.titan/plugins/libtool  --tag=CC   --mode=link $HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cross/bin/sh4-linux-gcc  -I$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cdkroot/usr/include -I$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cdkroot/usr/include/freetype2 -I$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cdkroot/usr/include/gstreamer-1.0 -I$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cdkroot/usr/include/glib-2.0 -I$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cdkroot/usr/include/libxml2 -I$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cdkroot/usr/lib/glib-2.0/include -I$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cdkroot/usr/lib/gstreamer-1.0/include -I$HOME/flashimg/BUILDGIT/checkout_$STM/driver/bpamem -I$HOME/flashimg/BUILDGIT/checkout_$STM/apps/titan/libdreamdvd -I$HOME/flashimg/BUILDGIT/checkout_$STM/apps/tools/libeplayer3/include -I$HOME/flashimg/BUILDGIT/checkout_$STM/apps/tools/libmmeimage -I$HOME/flashimg/BUILDGIT/checkout_$STM/apps/titan/titan  -Wl,-rpath -Wl,/usr/lib -Wl,-rpath-link -Wl,$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cdkroot/usr/lib -L$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cdkroot/usr/lib -L$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cdkroot/lib -o lib$2.la -rpath /lib $2.lo
		$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cross/bin/sh4-linux-gcc  -shared  -fPIC -DPIC  .libs/$2.o    $devflag -DSH4 -D$eplayer -Wl,-O1 -Wl,--as-needed   -Wl,-soname -Wl,lib$2.so.0 -L$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cdkroot/usr/lib -L$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cdkroot/lib $LIB -o .libs/lib$2.so $4
	
		cp .libs/lib$2.so lib$2.so
	else
		$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cross/bin/sh4-linux-gcc -D$eplayer -Os $devflag -fPIC -Wall -Wno-unused-but-set-variable $4 -I$eplayerinclude -c $2.c -o $2.o
		$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cross/bin/sh4-linux-gcc  -shared  -fPIC -DPIC  $2.o    $devflag -DSH4 -D$eplayer -Wl,-O1 -Wl,--as-needed   -Wl,-soname -Wl,lib$2.so.0 -L$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cdkroot/usr/lib -L$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cdkroot/lib $LIB -o lib$2.so $4
	fi

	$HOME/flashimg/BUILDGIT/checkout_$STM/tufsbox/cross/bin/sh4-linux-strip lib$2.so

	cd ..
	echo "[titan]--------------------------------------------------------"
	echo "[titan] Plugin $1 done"
	echo "[titan]--------------------------------------------------------"
}

uselibtool=n

rm -rf `find "$HOME"/flashimg/$SRCDIR/plugins -type f -name "*.o"`
rm -rf `find "$HOME"/flashimg/$SRCDIR/plugins -type f -name "*.so"`

echo "[titan]--------------------------------------------------------"
echo "[titan] Make networkbrowser"
echo "[titan]--------------------------------------------------------"
cd networkbrowser/netlib
cp Makefile."$STM".sh4 Makefile
make clean
make
cd "$HOME"/flashimg/$SRCDIR/plugins
echo "[titan]--------------------------------------------------------"
echo "[titan] networkbrowser done"
echo "[titan]--------------------------------------------------------"

#dir, file, extralib
compile "browser_netsurf" "browser" "" ""

compile "games_catcatch" "catcatch" "" ""
compile "games_wins3" "wins3" "" ""
compile "games_wm2018" "wm2018" "" ""

compile "infos_imdb" "imdb" "" "-l curl"
compile "infos_imdbapi" "imdbapi" "" ""
compile "infos_streaminfo" "streaminfo" "" ""
compile "infos_tinews" "tinews" "" ""
compile "infos_tmdb" "tmdb" "" ""

compile "network_dlna" "dlna" "" ""
compile "network_facebook" "facebook" "" "-l curl"
compile "network_networkbrowser" "networkbrowser" "netlib/netlib" ""
compile "network_xupnpd" "xupnpd" "" ""

compile "player_dvdplayer" "dvdplay" "" ""
compile "player_gmediarender" "gmediarender" "" ""
compile "player_mc" "mc" "" "-l curl"
compile "player_tithek" "tithek" "" "-l curl"
compile "player_tmc" "tmc" "" ""

compile "tools_autotimer" "autotimer" "" ""
compile "tools_callmonitor1" "callmonitor1" "" ""
compile "tools_filemanager" "filemanager" "" ""
compile "tools_instar" "instar" "" ""
compile "tools_keylock" "keylock" "" ""
compile "tools_kravencfg" "kravencfg" "" ""
compile "tools_lcdpearl1" "lcdpearl1" "" ""
compile "tools_lcdsamsung" "lcdsamsung" "" ""
compile "tools_optimize" "optimize" "" ""

compile "tools_permtime" "permtime" "" ""
compile "tools_rgui" "rgui" "" ""
compile "tools_scriptexec" "scriptexec" "" ""
compile "tools_stock" "stock" "" ""
compile "tools_stopifnotused" "stopifnotused" "" ""
compile "tools_tsSchnitt" "tsSchnitt" "" ""
compile "tools_usbreset" "usbreset" "" ""
compile "tools_weather" "weather" "" ""
compile "tools_wlandisplay" "wlandisplay" "" ""
compile "tools_zapback" "zapback" "" ""

compile "hello" "hello" "" ""
compile "panel" "panel" "" ""
compile "TopfieldVFD" "TopfieldVFD" "" ""
compile "mboxinfo" "mboxinfo" "" ""
compile "hbbtv_sh4" "hbbtv" "" ""
compile "readerconfig" "readerconfig" "" ""
compile "tiwakeup" "tiwakeup" "" ""

