#!/usr/bin/env bash
rm -rf ./bin
mkdir ./bin
mkdir ./bin/sample
cp sample/test_1080p.264 ./bin/sample
cd ./bin
cmake ..
make

