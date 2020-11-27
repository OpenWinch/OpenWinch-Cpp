
set(OW_TEST OpenWinchTest)

#include(CodeCoverage)
#append_coverage_compiler_flags(--coverage)

#include(GoogleTest)

find_package(Threads REQUIRED)
#find_package(GTest REQUIRED) # CONFIG


add_subdirectory("${PROJECT_SOURCE_DIR}/lib/googletest" "lib/googletest")

#include_directories(${GTEST_INCLUDE_DIRS})


file(GLOB_RECURSE TEST_SRC LIST_DIRECTORIES false *.h *.hpp *.cpp)

add_executable(${OW_TEST} ${TEST_SRC})
target_link_libraries(${OW_TEST} PRIVATE ${_OW_LINK} ${OW_TARGET}_lib gtest gmock gtest_main)
target_include_directories(${OW_TEST} PRIVATE ${_OW_INCLUDE})

add_test(NAME AllTestsIn${OW_TEST} COMMAND ${OW_TEST})

#gtest_add_tests(TARGET ${OW_TEST})
gtest_discover_tests(${OW_TEST}
                    WORKING_DIRECTORY ${PROJECT_DIR}
                    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
)

set_target_properties(${TESTNAME} PROPERTIES FOLDER test)

# setup_target_for_coverage_lcov(
#     NAME       coverage
#     EXECUTABLE make test
#     LCOV_ARGS  --no-external
#     EXCLUDE    "${PROJECT_SOURCE_DIR}/tests/*"
# )
