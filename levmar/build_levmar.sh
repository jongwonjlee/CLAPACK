#!/bin/bash

# build levmar
rm -rf build
mkdir -p build
cd build 
cmake ..
make

# run testsuites
# ./lmdemo
# ./expfit