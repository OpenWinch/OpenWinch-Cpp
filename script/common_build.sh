#!/usr/bin/env bash

if [[ -z "$BUILD_DIR" ]]; then
  echo "Cannot running directly !!!"
  exit
fi

cd ..

echo -e "\n\nDebug entry"
echo "Clean     : ${CLEAN}"
echo "Dir       : ${BUILD_DIR}"
echo "Toolchain : ${TOOLCHAIN}"
echo "Option    : ${OPTION}"

if [[ ! -z "$CLEAN" ]]; then
  echo -e "\nClean workspace..."
  rm -rf ${BUILD_DIR}
fi

echo -e "\nMake folder..."
mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}" || exit

echo -e "\nBuild stack..."
cmake ${OPTION} ../../..
cmake --build .

if [[ -z "$TOOLCHAIN" ]]; then
  echo -e "\n\nTesting..."
  ctest

  echo -e "\n\nCoverage..."
  make ctest_coverage_lcov
  make ctest_coverage_gcov_xml
  make ctest_coverage_gcov_html
fi

echo -e "\n\nPackage..."
cpack

echo -e "Ended..."
