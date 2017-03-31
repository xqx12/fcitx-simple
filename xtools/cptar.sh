#!/bin/bash
#Filename: /tmp/1.sh
#Last modified: 2017-03-29 21:12
# Author: Qixue Xiao <xiaoqixue_1@163.com>
#Description: 
ROOTDIR=/home/xqx/projects/fcitx-test/
cp $ROOTDIR/llvm-fcitx/src/im/pinyin/data/pinyin.tar.gz fcitx/src/im/pinyin/data/pinyin.tar.gz

cp $ROOTDIR/llvm-fcitx/src/im/table/data/table.tar.gz fcitx/src/im/table/data/table.tar.gz

cp $ROOTDIR/llvm-fcitx/src/module/spell/dict/en_dict-20121020.tar.gz fcitx/src/module/spell/dict/en_dict-20121020.tar.gz

cp $ROOTDIR/llvm-fcitx/src/module/pinyin-enhance/data/py_table-20121124.tar.gz fcitx/src/module/pinyin-enhance/data/py_table-20121124.tar.gz

cp $ROOTDIR/llvm-fcitx/src/module/pinyin-enhance/data/py_stroke-20121124.tar.gz fcitx/src/module/pinyin-enhance/data/py_stroke-20121124.tar.gz
