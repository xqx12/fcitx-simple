#!/bin/bash
#Filename: ../../patch-iconv.sh
#Last modified: 2017-03-31 02:31
# Author: Qixue Xiao <xiaoqixue_1@163.com>
#Description: 

if [ ! -f src/lib/fcitx-utils/CMakeFiles/fcitx-utils.dir/link.txt ]
then
    echo "wrong dir, file not found"
    exit 1
fi
sed -i 's/$/& -liconv/g' src/lib/fcitx-utils/CMakeFiles/fcitx-utils.dir/link.txt
sed -i 's/$/& -liconv/g' tools/cli/CMakeFiles/scel2org.dir/link.txt

