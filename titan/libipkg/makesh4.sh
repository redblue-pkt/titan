STM=$1
export CC="$HOME"/flashimg/BUILDGIT/checkout_"$STM"/tdt/tufsbox/devkit/sh4/bin/sh4-linux-gcc

make clean
make
