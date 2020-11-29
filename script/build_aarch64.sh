#!/usr/bin/env sh

BUILD_DIR='build/cmake/release_aarch64'
TOOLCHAIN='../../../cmake/platforms/arm-aarch64.toolchain.cmake'
OPTION='-DCMAKE_BUILD_TYPE=Debug'

cd ..

echo "Make folder..."
#rm -rf ${BUILD_DIR}
mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}

echo "Build stack..."
cmake ${OPTION} -DCMAKE_TOOLCHAIN_FILE=${TOOLCHAIN} ../../..
cmake --build .

echo "Package..."
cpack
