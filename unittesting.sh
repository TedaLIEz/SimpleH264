#!/usr/bin/env bash
cd test
rm -rf build
mkdir build
cd build
cmake ..
make
chmod u+x SimpleH264_Test
./SimpleH264_Test