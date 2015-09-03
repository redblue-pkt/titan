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

compile()
{
	cd $1
	echo "[titan]--------------------------------------------------------"
	echo "[titan] Make Plugin $1"
	echo "[titan]--------------------------------------------------------"

	$HOME/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/devkit/sh4/bin/sh4-linux-gcc -D$eplayer -Os $devflag -fPIC -Wall -Wno-unused-but-set-variable $4 \
	-I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/include/freetype2 \
	-I $eplayerinclude \
	-I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/cvs/driver/bpamem \
	-I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/cvs/apps/misc/tools/libmmeimage \
	-I "$HOME"/flashimg/$SRCDIR \
	-c $2.c -o $2.o

	if [ ! -z $3 ]; then
		$HOME/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/devkit/sh4/bin/sh4-linux-gcc -Os $devflag -shared -Wl,-soname,$2.so -o $2.so $2.o $3.a
	else
		$HOME/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/devkit/sh4/bin/sh4-linux-gcc -Os $devflag -shared -Wl,-soname,$2.so -o $2.so $2.o
	fi

	$HOME/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/devkit/sh4/bin/sh4-linux-strip $2.so
	cd ..
	echo "[titan]--------------------------------------------------------"
	echo "[titan] Plugin $1 done"
	echo "[titan]--------------------------------------------------------"
}

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
compile "callmonitor1" "callmonitor1" "" ""
compile "stopifnotused" "stopifnotused" "" ""
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
compile "hbbtv_sh4" "hbbtv" "" ""
compile "instar" "instar" "" ""
compile "tmdb" "tmdb" "" ""
compile "gmediarender" "gmediarender" "" ""
compile "imdb" "imdb" "" ""
compile "filemanager" "filemanager" "" ""
compile "catcatch" "catcatch" "" ""
compile "readerconfig" "readerconfig" "" ""
compile "tiwakeup" "tiwakeup" "" ""
compile "autotimer" "autotimer" "" ""
compile "usbreset" "usbreset" "" ""
compile "tsSchnitt" "tsSchnitt" "" ""
compile "xupnpd" "xupnpd" "" ""
compile "wm2014" "wm2014" "" ""

cd "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/cvs/cdk
echo make libcurl
make libcurl

cd "$HOME"/flashimg/$SRCDIR/plugins
compile "facebook" "facebook" "" "-l curl"
compile "tithek" "tithek" "" "-l curl"

#rm -r networkbrowser/networkbrowser
