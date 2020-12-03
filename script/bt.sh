#!/usr/bin/env bash
BUILD_DIR=

set -euo pipefail
set -a

BUILD_DIR='build/cmake/dev'
TYPE='Debug'

OPTION="-DCMAKE_BUILD_TYPE=$TYPE"

## Debug
#OPTION="-LAH ${OPTION} -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON"

## Clean
#CLEAN='1'

set +a

cd ..

echo -e "\n\nDebug entry"
#echo "Clean     : ${CLEAN}"
echo "Dir       : ${BUILD_DIR}"
#echo "Toolchain : ${TOOLCHAIN}"
echo "Option    : ${OPTION}"

#rm -rf ${BUILD_DIR}

mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}" || exit

cmake ${OPTION} ../../..
cmake --build . || exit

ctest -V
