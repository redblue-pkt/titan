STM=$1

BOX="build-gb800se"

export CC="$HOME"/openaaf/"$BOX"/tmp/sysroots/i686-linux/usr/mipsel/bin/mipsel-oe-linux-gcc
export LD="$HOME"/openaaf/"$BOX"/tmp/sysroots/i686-linux/usr/mipsel/bin/mipsel-oe-linux-ld
export CXX="$HOME"/openaaf/"$BOX"/tmp/sysroots/i686-linux/usr/mipsel/bin/mipsel-oe-linux-g++
export NM="$HOME"/openaaf/"$BOX"/tmp/sysroots/i686-linux/usr/mipsel/bin/mipsel-oe-linux-nm
export AR="$HOME"/openaaf/"$BOX"/tmp/sysroots/i686-linux/usr/mipsel/bin/mipsel-oe-linux-ar
export AS="$HOME"/openaaf/"$BOX"/tmp/sysroots/i686-linux/usr/mipsel/bin/mipsel-oe-linux-as
export RANLIB="$HOME"/openaaf/"$BOX"/tmp/sysroots/i686-linux/usr/mipsel/bin/mipsel-oe-linux-ranlib
export STRIP="$HOME"/openaaf/"$BOX"/tmp/sysroots/i686-linux/usr/mipsel/bin/mipsel-oe-linux-strip
export OBJCOPY="$HOME"/openaaf/"$BOX"/tmp/sysroots/i686-linux/usr/mipsel/bin/mipsel-oe-linux-objcopy
export OBJDUMP="$HOME"/openaaf/"$BOX"/tmp/sysroots/i686-linux/usr/mipsel/bin/mipsel-oe-linux-objdump


export CFLAGS="-O2"  
export CXXFLAGS="-O2"  
export LDFLAGS=""  
export PKG_CONFIG_PATH="$HOME"/openaaf/"$BOX"/tmp/sysroots/mipsel-oe-linux/usr/lib/pkgconfig
export LN_S="ln -s" 

cp libipkg.pc "$HOME"/openaaf/"$BOX"/tmp/sysroots/mipsel-oe-linux/usr/lib/pkgconfig

make clean
./configure \
--build=x86_64-unknown-linux-gnu \
--host=mipsel-oe-linux \
--prefix=/usr && \
make
#make install

cp .libs/* "$HOME"/openaaf/"$BOX"/tmp/sysroots/mipsel-oe-linux/usr/lib
