#!/bin/sh

lead="#begin_$1"
tail="#end_$1"
file_orig="$3/skin.xml"
file_insert="$3/snippets/$1-$2.xml"

echo lead=$lead
echo tail=$tail
echo file_orig=$file_orig
echo file_insert=$file_insert

sed -i -e "/^$lead$/,/^$tail$/{ /^$lead$/{p; r $file_insert
        }; /$tail/p; d }" $file_orig

sync
