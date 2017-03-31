#!/bin/bash
#Filename: build.sh
#Last modified: 2017-03-21 21:37
# Author: Qixue Xiao <xiaoqixue_1@163.com>
#Description: 

mkdir gbuild
cd gbuild
cmake .. -DCMAKE_INSTALL_PREFIX=`fcitx4-config --prefix` -DLIB_INSTALL_DIR=`fcitx4-config --libdir` -DENABLE_DEBUG=ON -DENABLE_TEST=ON


make


#/home/xqx/projects/afl-test/afl-fuzz/afl-gcc -Xlinker -zmuldefs  -g -O0 -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -fvisibility=hidden     CMakeFiles/fcitx-testbed.dir/main.c.o  -o fcitx-testbed-static -rdynamic ../src/libfcitx-simple-static.a libfcitx-core.a libfcitx-config.a -Wl,--whole-archive libfcitx-utils.a ../src/libfcitx-simple-common-static.a -lfcitx-utils -lpthread -Wl,-rpath,/home/xqx/tests/fcitx-fuzz/fcitx-simple/afl-build/src -ldl -liconv
