cmake_minimum_required(VERSION 3.17.0 FATAL_ERROR)

### Config ###
set(OW_NAME "OpenWinch")
set(OW_TARGET "openwinch")
set(OW_VERSION "0.0.0" CACHE INTERNAL "")

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

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_CPPCHECK "/usr/bin/cppcheck;--std=c++11")
#set(CMAKE_CXX_CPPLINT "cpplint")

#set(CMAKE_INSTALL_PREFIX "/opt")
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/modules/)

### Detect version ###
include("${CMAKE_CURRENT_LIST_DIR}/cmake/checker-git.cmake")

project(${OW_TARGET} VERSION ${OW_VERSION} LANGUAGES CXX)

### Detect Board ###
include("${CMAKE_CURRENT_LIST_DIR}/cmake/checker-board.cmake")

### External libs ###
add_subdirectory(lib/cpp-httplib)
add_subdirectory(lib/lcdgfx)
add_subdirectory(lib/slog)

### Build ###
add_subdirectory(src)

### Testing ###
add_subdirectory(test)

### Pack ###
include("${CMAKE_CURRENT_LIST_DIR}/cmake/CPack.cmake")
