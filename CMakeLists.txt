cmake_minimum_required(VERSION 3.17.0 FATAL_ERROR)

### Config ###
set(OW_NAME "OpenWinch")
set(OW_TARGET "openwinch")

set(CMAKE_PROJECT_DESCRIPTION "Winch main app.")
set(CMAKE_PROJECT_HOMEPAGE_URL "http://")

## Board
# Can be emulator, raspberry, esp32
set(OW_BOARD "emulator")
set(OW_GPIO  "pigpio")
set(OW_GUI   "SH1106_I2C")
set(SDL_EMULATION False)

list(APPEND _OW_INCLUDE 
  ${CMAKE_CURRENT_LIST_DIR}/include
  ${CMAKE_CURRENT_LIST_DIR}/lib/slog/src
  ${CMAKE_CURRENT_LIST_DIR}/lib/lcdgfx/src
  ${CMAKE_CURRENT_LIST_DIR}/lib/cpp-httplib
  ${CMAKE_CURRENT_LIST_DIR}/lib/pigpio
)

set(_OW_LINK_OPT "")
set(_OW_LINK_DIR "")

# list(APPEND _OW_LINK_DIR
#   ${CMAKE_CURRENT_LIST_DIR}/lib/lcdgfx/bld
# )

list(APPEND _OW_LINK
  Threads::Threads
  httplib::httplib
  slog
  lcdgfx
)

set(_OW_COMPILE_DEF "")

list(APPEND _OW_COMPILE_OPT
  -Wall
  -Wextra
  -Wpedantic
  -Wconversion
  -MMD
  -MP
  -Wno-multichar
  -ffunction-sections
  -fdata-sections
  $<$<COMPILE_LANGUAGE:CXX>:${BUILD_FLAGS_FOR_CXX}> -ffreestanding
  $<$<COMPILE_LANGUAGE:CXX>:${BUILD_FLAGS_FOR_CXX}> -fpic
)

list(APPEND OW_PROPERTIES
  C_STANDARD 99
  C_STANDARD_REQUIRED YES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

set(CMAKE_INSTALL_PREFIX "/opt")
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/modules/)

### LIST files ###

file(GLOB_RECURSE OPENWINCH_SRC "src/*.cpp")
#file(GLOB OPENWINCH_HEADERS "*.hpp")

### Detect version ###
include("${CMAKE_CURRENT_LIST_DIR}/cmake/checker-git.cmake")

project(${OW_TARGET} VERSION ${OW_VERSION} LANGUAGES CXX)

### Detect Board ###
include("${CMAKE_CURRENT_LIST_DIR}/cmake/checker-board.cmake")

### External libs ###
add_subdirectory(lib/cpp-httplib)
add_subdirectory(lib/lcdgfx)
add_subdirectory(lib/slog)

### Internal libs ###
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
#find_package(stdc++ REQUIRED)
#find_package(RT)

### Testing ###
include(CTest)
enable_testing()

### Build ###
add_executable(${OW_TARGET} ${OPENWINCH_SRC})

set_target_properties( ${OW_TARGET} PROPERTIES ${OW_PROPERTIES} )

target_link_libraries( ${OW_TARGET} PRIVATE ${_OW_LINK} )
target_link_directories( ${OW_TARGET} PRIVATE ${_OW_LINK_DIR} )
target_link_options( ${OW_TARGET} PRIVATE  ${_OW_LINK_OPT} )
target_include_directories( ${OW_TARGET} PRIVATE ${_OW_INCLUDE} )
target_compile_options( ${OW_TARGET} PRIVATE ${_OW_COMPILE_OPT} )
target_compile_definitions( ${OW_TARGET} PRIVATE ${_OW_COMPILE_DEF} )


install(TARGETS ${OW_TARGET}
    COMPONENT bin
    RUNTIME DESTINATION "/opt/openwinch/bin"
    LIBRARY DESTINATION "/opt/openwinch/lib"
    DESTINATION "/home/"
)


### Pack ###
include("${CMAKE_CURRENT_LIST_DIR}/cmake/CPack.cmake")
