#!/usr/bin/env sh

BUILD_DIR='build/cmake/release_amd64'
#OPTION='-DCMAKE_BUILD_TYPE=Release'
OPTION='-DCMAKE_BUILD_TYPE=Debug'
OPTION+='-DCMAKE_VERBOSE_MAKEFILE:BOOL=ON'

cd ..

echo "Make folder..."
rm -rf ${BUILD_DIR}
mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}

echo "Build stack..."
cmake -LAH ${OPTION} ../../..
cmake --build .

echo "Package..."
cpack

