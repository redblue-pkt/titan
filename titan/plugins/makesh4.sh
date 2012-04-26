#*****************************************************#
#* this file is part of the tiTan / tiTanNIT Project *#
#* and allowed only for use with this.               *#
#*                                                   *#
#* copyright by NIT                                  *#
#*****************************************************#

STM=$1
MEDIAFW=$2

if [ -z "$STM" ]; then
	echo "error: use makesh4.sh <stm23|stm23|stm24> <1|2>"
	exit 1
fi
if [ -z "$MEDIAFW" ]; then
	echo "error: use makesh4.sh <stm23|stm23|stm24> <1|2>"
	exit 1
fi

cd "$HOME"/flashimg/source.titan/plugins
ln -s "$HOME"/flashimg/source.titan/titan "$HOME"/flashimg/source.titan/plugins/titan

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

	$HOME/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/devkit/sh4/bin/sh4-linux-gcc -D$eplayer -Os -fPIC -Wall \
	-I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/include/freetype2 \
	-I $eplayerinclude \
	-I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/cvs/driver/bpamem \
	-I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/cvs/apps/misc/tools/libmmeimage \
	-I "$HOME"/flashimg/source.titan \
	-c $2.c -o $2.o

	if [ ! -z $3 ]; then
		$HOME/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/devkit/sh4/bin/sh4-linux-gcc -shared -W1,-soname,$2.so -o $2.so $2.o $3.a
	else
		$HOME/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/devkit/sh4/bin/sh4-linux-gcc -shared -W1,-soname,$2.so -o $2.so $2.o
	fi

	$HOME/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/devkit/sh4/bin/sh4-linux-strip $2.so
	cd ..
	echo "[titan]--------------------------------------------------------"
	echo "[titan] Plugin $1 done"
	echo "[titan]--------------------------------------------------------"
}

rm -rf `find "$HOME"/flashimg/source.titan/plugins -type f -name "*.o"`
rm -rf `find "$HOME"/flashimg/source.titan/plugins -type f -name "*.so"`

echo "[titan]--------------------------------------------------------"
echo "[titan] Make networkbrowser"
echo "[titan]--------------------------------------------------------"
cd networkbrowser/netlib
cp Makefile."$STM".sh4 Makefile
make clean
make
cd "$HOME"/flashimg/source.titan/plugins
echo "[titan]--------------------------------------------------------"
echo "[titan] networkbrowser done"
echo "[titan]--------------------------------------------------------"

#dir, file, extralib
compile "networkbrowser" "networkbrowser" "netlib/netlib"
compile "hello" "hello" ""
compile "panel" "panel" ""
compile "mc" "mc" ""
compile "TopfieldVFD" "TopfieldVFD" ""
compile "mboxinfo" "mboxinfo" ""
compile "browser" "browser" ""
compile "keylock" "keylock" ""
compile "permtime" "permtime" ""
compile "imdb" "imdb" ""
compile "lcdpearl1" "lcdpearl1" ""
compile "callmonitor1" "callmonitor1" ""
compile "stopifnotused" "stopifnotused" ""
compile "tithek" "tithek" ""
compile "wins3" "wins3" ""
compile "rgui" "rgui" ""
compile "dvdplayer" "dvdplay" ""
compile "scriptexec" "scriptexec" ""
compile "optimize" "optimize" ""
compile "weather" "weather" ""

#rm -r networkbrowser/networkbrowser
