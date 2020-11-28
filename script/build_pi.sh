#!/usr/bin/env sh

BUILD_DIR='build/cmake/release_pi'
TOOLCHAIN='../../../cmake/platforms/arm-gnueabihf.toolchain.cmake'
#OPTION='-DCMAKE_BUILD_TYPE=Release'

cd ..

echo "Make folder..."
rm -rf ${BUILD_DIR}
mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}

echo "Build stack..."
cmake ${OPTION} -DCMAKE_TOOLCHAIN_FILE=${TOOLCHAIN} ../../..
cmake --build .

echo "Package..."
cpack
