#!/bin/bash
#Filename: build.sh
#Last modified: 2017-03-21 21:37
# Author: Qixue Xiao <xiaoqixue_1@163.com>
#Description: 

mkdir gbuild
cd gbuild
cmake .. -DCMAKE_INSTALL_PREFIX=`fcitx4-config --prefix` -DLIB_INSTALL_DIR=`fcitx4-config --libdir` -DENABLE_DEBUG=ON -DENABLE_TEST=ON
make
