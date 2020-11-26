
set(OW_TEST OpenWinchTest)

find_package(Threads REQUIRED)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

include(GoogleTest)
enable_testing()

file(GLOB_RECURSE TEST_SRC LIST_DIRECTORIES false *.h *.cpp)

add_executable(${OW_TEST} ${TEST_SRC})

target_link_libraries(${OW_TEST} PRIVATE ${_OW_LINK} ${OW_TARGET}_lib gtest gtest_main)
target_include_directories(${OW_TEST} PRIVATE ${_OW_INCLUDE})

gtest_add_tests(TARGET ${OW_TEST})

add_test(NAME testing COMMAND ${OW_TEST})

