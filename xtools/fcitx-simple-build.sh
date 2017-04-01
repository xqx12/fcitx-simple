#!/bin/bash
#Filename: build.sh
#Last modified: 2017-02-28 02:00
# Author: Qixue Xiao <xiaoqixue_1@163.com>
#Description: 


cd fcitx
mkdir build
cd build
mkdir install-bin
cmake .. -DCMAKE_INSTALL_PREFIX=`pwd`/install-bin -DENABLE_TEST=ON -DENABLE_DEBUG=ON
#CC=wllvm CXX=wllvm++ cmake .. -DCMAKE_INSTALL_PREFIX=`pwd`/llvm-obj  -DENABLE_STATIC=ON

#export afl path
#CC=afl-gcc CXX=afl-g++ cmake .. -DCMAKE_INSTALL_PREFIX=`pwd`/afl-obj  -DENABLE_STATIC=ON

# patch -P1 build-c99.patch

###  use for libiconv link
#if [ ! -f src/lib/fcitx-utils/CMakeFiles/fcitx-utils.dir/link.txt ]
#then
    #echo "wrong dir, file not found for patch"
#else
    #sed -i 's/$/& -liconv/g' src/lib/fcitx-utils/CMakeFiles/fcitx-utils.dir/link.txt
    #sed -i 's/$/& -liconv/g' tools/cli/CMakeFiles/scel2org.dir/link.txt
#fi


make 
make install

## fcitx-simple cmake with afl
#CC=afl-gcc CXX=afl-g++ cmake .. -DCMAKE_INSTALL_PREFIX="/home/xqx/tests/fcitx-fuzz/fcitx/afl-build/afl-obj" \
#    -DLIB_INSTALL_DIR=/home/xqx/tests/fcitx-fuzz/fcitx/afl-build/afl-obj/lib


