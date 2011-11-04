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

	$HOME/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/devkit/sh4/bin/sh4-linux-gcc -D$eplayer-Os -fPIC -Wall \
	-I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/include/freetype2 \
	-I $eplayerinclude \
	-I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/cvs/driver/bpamem \
	-I "$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/cvs/apps/misc/tools/libmmeimage \
	-c $1.c -o $1.o

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

compile "networkbrowser" "networkbrowser" "netlib/netlib"
compile "hello" "hello" ""
compile "aafpanel" "aafpanel" ""
compile "mc" "mc" ""
compile "TopfieldVFD" "TopfieldVFD" ""
compile "mboxinfo" "mboxinfo" ""
#compile "callmonitor1" "callmonitor1" ""

#rm -r networkbrowser/networkbrowser
