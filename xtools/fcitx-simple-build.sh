#!/bin/bash
#Filename: build.sh
#Last modified: 2017-02-28 02:00
# Author: Qixue Xiao <xiaoqixue_1@163.com>
#Description: 

ROOTDIR=/data/xqx/tests/fcitx-fuzz

export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$ROOTDIR/fcitx/build-4283/install-bin/lib/pkgconfig
export PATH=$PATH:$ROOTDIR/fcitx/build-4283/install-bin/bin
export CMAKE_PREFIX_PATH=$ROOTDIR/fcitx/build-4283/install-bin

export C_INCLUDE_PATH=$C_INCLUDE_PATH:$ROOTDIR/fcitx/build-4283/install-bin/include



cd fcitx-simple
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX="/home/xqx/tests/fcitx-fuzz/fcitx/afl-build/afl-obj" \
#    -DLIB_INSTALL_DIR=/home/xqx/tests/fcitx-fuzz/fcitx/afl-build/afl-obj/lib

make 
make install

## fcitx-simple cmake with afl
#CC=afl-gcc CXX=afl-g++ cmake .. -DCMAKE_INSTALL_PREFIX="/home/xqx/tests/fcitx-fuzz/fcitx/afl-build/afl-obj" \
#    -DLIB_INSTALL_DIR=/home/xqx/tests/fcitx-fuzz/fcitx/afl-build/afl-obj/lib


