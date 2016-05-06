STM=$1
export CC="$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/devkit/sh4/bin/sh4-linux-gcc
export LD="$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/devkit/sh4/bin/sh4-linux-ld 
export CXX="$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/devkit/sh4/bin/sh4-linux-g++ 

export NM="$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/devkit/sh4/bin/sh4-linux-nm  
export AR="$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/devkit/sh4/bin/sh4-linux-ar 
export AS="$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/devkit/sh4/bin/sh4-linux-as 
export RANLIB="$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/devkit/sh4/bin/sh4-linux-ranlib 
export STRIP="$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/devkit/sh4/bin/sh4-linux-strip 
export OBJCOPY="$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/devkit/sh4/bin/sh4-linux-objcopy 
export OBJDUMP="$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/devkit/sh4/bin/sh4-linux-objdump 
export CFLAGS="-O2"  
export CXXFLAGS="-O2"  
export LDFLAGS=""  
export PKG_CONFIG_PATH="$HOME/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/pkgconfig"  
export LN_S="ln -s" 

cp libipkg.pc $HOME/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib/pkgconfig
#CC=sh4-linux-gcc CXX=sh4-linux-g++ LD=sh4-linux-ld NM=sh4-linux-nm AR=sh4-linux-ar AS=sh4-linux-as RANLIB=sh4-linux-ranlib STRIP=sh4-linux-strip OBJCOPY=sh4-linux-objcopy OBJDUMP=sh4-linux-objdump LN_S="ln -s" CFLAGS="-O2" CXXFLAGS="-O2" LDFLAGS="" PKG_CONFIG_PATH="/home/obiwan/flashimg/BUILDGIT/checkout_stm24/tdt/tufsbox/cdkroot/usr/lib/pkgconfig" \
make clean
./autogen.sh

./configure \
--build=x86_64-unknown-linux-gnu \
--host=sh4-linux \
--prefix=/usr && \
make
#make install

cp .libs/* $HOME/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/cdkroot/usr/lib
