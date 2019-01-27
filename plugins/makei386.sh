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


if [ -z "$STM" ]; then
	echo "error: use makesh4.sh <stm23|stm23|stm24> <1|2>"
	exit 1
fi

#if [ $GROUP == "dev" ]; then
#	devflag="-finstrument-functions -rdynamic -DBETA"
#else
#	devflag=""
#fi

cd "$HOME"/flashimg/source.titan/plugins
ln -s "$HOME"/flashimg/source.titan/titan "$HOME"/flashimg/source.titan/plugins/titan

#if [ $MEDIAFW = 1 ]; then
#	eplayer=EPLAYER3
#	eplayerinclude="$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/cvs/apps/misc/tools/libeplayer3/include
#	eplayerlib=eplayer3
#else
#	eplayer=EPLAYER4
#	eplayerinclude="$HOME/flashimg/BUILDGIT/checkout_$STM/tdt/tufsbox/cdkroot/usr/include/gstreamer-0.10
#			 -I$HOME/flashimg/BUILDGIT/checkout_$STM/tdt/tufsbox/cdkroot/usr/include/glib-2.0
#			 -I$HOME/flashimg/BUILDGIT/checkout_$STM/tdt/tufsbox/cdkroot/usr/include/libxml2
#			 -I$HOME/flashimg/BUILDGIT/checkout_$STM/tdt/tufsbox/cdkroot/usr/lib/glib-2.0/include"
#	eplayerlib=gstreamer-0.10
#fi

compile()
{
	cd $1
	echo "[titan]--------------------------------------------------------"
	echo "[titan] Make Plugin $1"
	echo "[titan]--------------------------------------------------------"

	gcc -DSIMULATE -Os $devflag -fPIC -Wall -Wno-unused-but-set-variable $4 -I/usr/include/freetype2 -I "$HOME"/flashimg/source.titan/libdreamdvd -I "$HOME" -c $2.c -o $2.o
	gcc  -shared  -fPIC -DPIC  $2.o    $devflag -DSH4 -D$eplayer -Wl,-O1 -Wl,--as-needed   -Wl,-soname -Wl,lib$2.so.0 -L/lib -L/usr/lib -L/usr/lib/i386-linux-gnu -o $2.so $4

	cd ..
	echo "[titan]--------------------------------------------------------"
	echo "[titan] Plugin $1 done"
	echo "[titan]--------------------------------------------------------"
}

rm -rf `find "$HOME"/flashimg/source.titan/plugins -type f -name "*.o"`
rm -rf `find "$HOME"/flashimg/source.titan/plugins -type f -name "*.so"`

cd "$HOME"/flashimg/source.titan/plugins

#dir, file, extralib
compile "browser" "browser" "" ""
compile "tithek" "tithek" "" "-l curl"
