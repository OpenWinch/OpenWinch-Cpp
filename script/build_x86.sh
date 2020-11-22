#!/usr/bin/env sh

BUILD_DIR='build/cmake/release_amd64'
OPTION='-DCMAKE_BUILD_TYPE=Release'

cd ..

echo "Make folder..."
rm -rf ${BUILD_DIR}
mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}

echo "Build stack..."
cmake ${OPTION} ../../..
cmake --build .

echo "Package..."
cpack

