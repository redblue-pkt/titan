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

HELP=0

if [ "$HELP" = "1" ];then
	#svn build
	sudo apt-get install subversion
	sudo apt-get install advancecomp pngquant
	sudo apt-get install quilt
	sudo apt-get install wput
	#titan build
	sudo apt-get install libdirectfb-1.2-9 ??
	sudo apt-get install libpng-dev
	sudo apt-get install libjpeg-dev
	sudo apt-get install libfreetype6-dev
	sudo apt-get install libglib2.0-dev
	sudo apt-get install libtool
  
  
	sudo apt-get install fbgrab xloadimage
	sudo mkdir -p /dev/dvb/adapter0
	sudo touch /dev/dvb/adapter0/{frontend0,demux0,audio0,video0}

	gksudo gedit /etc/default/grub
	GRUB_CMDLINE_LINUX_DEFAULT="quiet vga=0x364"
	sudo update-grub 
fi

cd "$HOME"/flashimg/$SRCDIR/titan

gcc -DSIMULATE -DI386 -DCAMSUPP -Os -mhard-float -export-dynamic -Wall -Wno-unused-but-set-variable -Wno-implicit-function-declaration -Wno-unused-result -Wunused-variable -Wno-parentheses -Wno-maybe-uninitialized -I/usr/include/freetype2 -I "$HOME"/flashimg/$SRCDIR/libdreamdvd -I "$HOME" -c titan.c
/bin/bash libtool --tag=CC --mode=link gcc -DSIMULATE -DI386 -DNOFB -Os -export-dynamic -Wall -Wno-unused-but-set-variable -pipe -Os  -Wl,-rpath -Wl,/usr/lib -Wl,-rpath-link -Wl,/usr/lib -L/lib -L/usr/lib -L/usr/lib/i386-linux-gnu -o titan titan.o -lpthread -ldl -lpng -lfreetype -ljpeg -lz -lm
