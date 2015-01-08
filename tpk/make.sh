SRCDIR=$1
gcc -D TPKCREATE -D WORKDIR=\"$HOME/ipk/BUILD\" "$HOME"/flashimg/$SRCDIR/tpk/tpk.c -Wall -I "$HOME"/flashimg/$SRCDIR/titan -o "$HOME"/flashimg/$SRCDIR/tpk/tpk
