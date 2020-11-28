
set(OW_TEST OpenWinchTest)

include(GoogleTest)
include(CodeCoverage)

append_coverage_compiler_flags(--coverage)
option(INSTALL_GTEST OFF)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O0")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")

find_package(Threads REQUIRED)
#find_package(GTest REQUIRED) # CONFIG

add_subdirectory("${PROJECT_SOURCE_DIR}/lib/googletest" "lib/googletest")
#include_directories(${GTEST_INCLUDE_DIRS})

file(GLOB_RECURSE TEST_SRC LIST_DIRECTORIES false *.h *.hpp *.cpp)
set(TEST_SRC ${TEST_SRC} PARENT_SCOPE)

## LIBRARY ###
add_library(${OW_TEST}_lib STATIC ${OPENWINCH_SRC})

set_target_properties( ${OW_TEST}_lib PROPERTIES ${OW_PROPERTIES} )
target_link_libraries( ${OW_TEST}_lib PRIVATE ${_OW_LINK} )
target_link_directories( ${OW_TEST}_lib PRIVATE ${_OW_LINK_DIR} )
target_link_options( ${OW_TEST}_lib PRIVATE  ${_OW_LINK_OPT} )
target_include_directories( ${OW_TEST}_lib PRIVATE ${_OW_INCLUDE} )
target_compile_options( ${OW_TEST}_lib PRIVATE ${_OW_COMPILE_OPT} )
target_compile_definitions( ${OW_TEST}_lib PRIVATE ${_OW_COMPILE_DEF} )

## TESTS ###
add_executable(${OW_TEST} ${TEST_SRC})
target_link_libraries(${OW_TEST} PRIVATE ${OW_TEST}_lib gtest gmock gmock_main)
target_include_directories(${OW_TEST} PRIVATE ${_OW_INCLUDE})

add_test(NAME AllTestsIn${OW_TEST} COMMAND ${OW_TEST})

if(OW_BOARD STREQUAL "emulator")
  gtest_discover_tests(${OW_TEST}
                      WORKING_DIRECTORY ${PROJECT_DIR}
                      PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
  )
endif()
set_target_properties(${TESTNAME} PROPERTIES FOLDER test)

set(COVERAGE_EXCLUDES "test/*" "lib/*" "googlemock/*" "googletest/*")
set(BASE_DIRECTORY "${PROJECT_SOURCE_DIR}")
setup_target_for_coverage_gcovr_xml(
    NAME ctest_coverage_gcov_xml
    EXECUTABLE ctest -j ${PROCESSOR_COUNT}
    DEPENDENCIES ${OW_TEST} ${OW_TARGET}_lib
    BASE_DIRECTORY ${BASE_DIRECTORY}
    EXCLUDE ${COVERAGE_EXCLUDES}   
)

setup_target_for_coverage_gcovr_html(
    NAME ctest_coverage_gcov_html
    BASE_DIRECTORY ${BASE_DIRECTORY}
    EXCLUDE ${COVERAGE_EXCLUDES}
    EXECUTABLE ctest -j ${PROCESSOR_COUNT}
    DEPENDENCIES ${OW_TEST} ${OW_TARGET}_lib
)

setup_target_for_coverage_lcov(
    NAME       ctest_coverage_lcov
    BASE_DIRECTORY ${BASE_DIRECTORY}
    EXCLUDE ${COVERAGE_EXCLUDES}
    EXECUTABLE ctest -j ${PROCESSOR_COUNT}
    DEPENDENCIES ${OW_TEST} ${OW_TARGET}_lib
    LCOV_ARGS  --no-external 
)
