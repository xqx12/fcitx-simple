#!/bin/bash
#Filename: build.sh
#Last modified: 2017-02-28 02:00
# Author: Qixue Xiao <xiaoqixue_1@163.com>
#Description: 

sudo apt-get install libxkbfile-dev \
    libgtk2.0-dev  \
    libgirepository1.0-dev  \
    libopencc-dev   \
    libenchant-dev   \
    libxml2-dev
    
sudo apt-get install qt-sdk bison
sudo apt-get install cmake extra-cmake-modules
sudo apt-get install libicu-dev
sudo apt-get install libqt4-dev

# install extra-cmake-modules
#git clone git://anongit.kde.org/extra-cmake-modules
#cd extra-cmake-modules
#mkdir build && cd build    
#cmake ..
#make && sudo make install
#cd ../..


if [ ! -f libxkbcommon-0.5.0.tar.xz ]
then
  wget http://xkbcommon.org/download/libxkbcommon-0.5.0.tar.xz
fi

if [ ! -d libxkcommon-0.5.0 ]
then
    tar xvf libxkbcommon-0.5.0.tar.xz
    cd libxkbcommon-0.5.0
    ./configure --disable-x11
    make
    sudo make install
    cd ..
fi

cd fcitx
mkdir build
cd build
mkdir install-bin
cmake .. -DCMAKE_INSTALL_PREFIX=`pwd`/install-bin -DENABLE_TEST=ON -DENABLE_DEBUG=ON
#CC=wllvm CXX=wllvm++ cmake .. -DCMAKE_INSTALL_PREFIX=`pwd`/llvm-obj  -DENABLE_STATIC=ON

# patch -P1 build-c99.patch
make 
make install

## fcitx-simple cmake with afl
#CC=afl-gcc CXX=afl-g++ cmake .. -DCMAKE_INSTALL_PREFIX="/home/xqx/tests/fcitx-fuzz/fcitx/afl-build/afl-obj" \
#    -DLIB_INSTALL_DIR=/home/xqx/tests/fcitx-fuzz/fcitx/afl-build/afl-obj/lib


