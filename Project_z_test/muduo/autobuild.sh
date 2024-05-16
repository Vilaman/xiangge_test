#!/bin/bash

set -e

# 如果没有build目录，创建该目录
if [ ! -d `pwd`/build ]; then
    mkdir `pwd`/build
fi

rm -rf `pwd`/build/*

cd `pwd`/build &&
    cmake .. &&
    make

# 回到项目根目录
cd ..
'pwd'

if [ ! -d 'pwd'/mymuduo ]; then 
    mkdir mymuduo
fi

for header in `ls *.h`
do
    cp $header mymuduo
done