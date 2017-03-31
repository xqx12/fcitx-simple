#!/bin/bash
#Filename: ../../../mk-testbd-static.sh
#Last modified: 2017-03-31 02:53
# Author: Qixue Xiao <xiaoqixue_1@163.com>
#Description: 

/home/xqx/projects/afl-test/afl-fuzz/afl-gcc   -g -O0 -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -fvisibility=hidden     \
    CMakeFiles/fcitx-testbed.dir/main.c.o  -o fcitx-testbed-static -rdynamic ../src/libfcitx-simple-static.a libfcitx-core.a \
    libfcitx-config.a libfcitx-utils.a ../src/libfcitx-simple-common-static.a -lfcitx-utils -lpthread \
    -Wl,-rpath,/home/xqx/tests/fcitx-fuzz/fcitx-simple/afl-build/src -ldl -liconv

## --whole-archive , to link all in a archive
## -Xlinker -zmuldefs, to igonre repeat functions.
/home/xqx/projects/afl-test/afl-fuzz/afl-gcc -Xlinker -zmuldefs  -g -O0 -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -fvisibility=hidden     CMakeFiles/fcitx-testbed.dir/main.c.o  -o fcitx-testbed-static -rdynamic ../src/libfcitx-simple-static.a libfcitx-core.a libfcitx-config.a -Wl,--whole-archive libfcitx-utils.a ../src/libfcitx-simple-common-static.a -lfcitx-utils -lpthread -Wl,-rpath,/home/xqx/tests/fcitx-fuzz/fcitx-simple/afl-build/src -ldl -liconv
