#!/usr/bin/env bash

set -euo pipefail
set -a

BUILD_DIR='build/cmake/release_eabi'
TOOLCHAIN='../../../cmake/platforms/arm-gnueabi.toolchain.cmake'
TYPE='Debug'

OPTION="-DCMAKE_BUILD_TYPE=$TYPE -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN"

## Debug
#OPTION="-LAH ${OPTION} -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON"

## Clean
#CLEAN='1'

set +a

bash common_build.sh
