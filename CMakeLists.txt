cmake_minimum_required(VERSION 3.17.0 FATAL_ERROR)

### Config ###

set(OW_TARGET "openwinch")

## Board
# Can be emulator, raspberry, esp32
set(OW_BOARD "emulator")
set(OW_GPIO  "pigpio")
set(OW_GUI   "SH1106_I2C")

list(APPEND _OW_INCLUDE 
  ${CMAKE_CURRENT_LIST_DIR}/include
  ${CMAKE_CURRENT_LIST_DIR}/lib/slog/src
  ${CMAKE_CURRENT_LIST_DIR}/lib/lcdgfx/src
  ${CMAKE_CURRENT_LIST_DIR}/lib/cpp-httplib
)

set(_OW_LINK_OPT "")
list(APPEND _OW_LINK_DIR
  ${CMAKE_CURRENT_LIST_DIR}/lib/slog/src
  ${CMAKE_CURRENT_LIST_DIR}/lib/lcdgfx/bld
)
list(APPEND _OW_LINK
  Threads::Threads
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
  -g
  -O0

  -ffunction-sections
  -fdata-sections
  $<$<COMPILE_LANGUAGE:CXX>:${BUILD_FLAGS_FOR_CXX}> -ffreestanding
  $<$<COMPILE_LANGUAGE:CXX>:${BUILD_FLAGS_FOR_CXX}> -fpic
)

set(CMAKE_INSTALL_PREFIX "/usr/local")

#set(CMAKE_C_COMPILER "gcc")
#set(CMAKE_CXX_COMPILER "g++")

#set(CMAKE_C_COMPILER "clang-9")
#set(CMAKE_CXX_COMPILER "clang++-9")


### List files ###

file(GLOB_RECURSE OPENWINCH_SRC "src/*.cpp")
file(GLOB OPENWINCH_HEADERS "*.hpp")


### Detect version ###

# On systems without Git installed, there were errors since execute_process seemed to not throw an error without it?
find_package(Git QUIET)
if(Git_FOUND)
	# Gets the latest tag as a string like "v0.6.6"
	# Can silently fail if git isn't on the system
	execute_process(COMMAND ${GIT_EXECUTABLE} describe --tags --abbrev=0
		WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
		OUTPUT_VARIABLE _raw_version_string
		ERROR_VARIABLE _git_tag_error
	)
endif()

# execute_process can fail silenty, so check for an error
# if there was an error, just use the user agent as a version
if(_git_tag_error OR NOT Git_FOUND)
	message(WARNING "OpenWinch failed to find the latest Git tag, falling back to using user agent as the version.")
  set(OW_VERSION "0.2.0")
endif()

# Needed since git tags have "v" prefixing them.
# Also used if the fallback to user agent string is being used.
#string(REGEX MATCH "([0-9]+\\.?)+" OW_VERSION "${_raw_version_string}")

project(${OW_TARGET} VERSION ${OW_VERSION} LANGUAGES CXX)

### Detect Board ###

message(CHECK_START "Detecting board")
set(OW_DETECT_BOARD False)

if(OW_BOARD STREQUAL "raspberry")
  message(CHECK_PASS "Raspberry PI")
  list(APPEND CMAKE_MESSAGE_INDENT "  ")
  set(OW_DETECT_BOARD True)

  list(APPEND _OW_COMPILE_DEF "OW_BD_PI")

  message(CHECK_START "Detecting IO framework")
	if(OW_GPIO STREQUAL "pigpio")
    message(CHECK_PASS "Support PIGPIO.")

    list(APPEND _OW_COMPILE_DEF "OW_BG_PIGPIO")
    list(APPEND _OW_LINK "pigpio")
	else()
    message(CHECK_PASS "Support WIRINGPI.")

    list(APPEND _OW_COMPILE_DEF "OW_BG_WIRINGPI")
    list(APPEND _OW_LINK "wiringPi")
	endif()

  list(POP_BACK CMAKE_MESSAGE_INDENT)
endif()

if(OW_BOARD STREQUAL "emulator")
  message(CHECK_PASS "Emulator")
  set(OW_DETECT_BOARD True) 

  list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
  find_package(SDL2 REQUIRED)
  #find_package(SDL2_net REQUIRED)
  #find_package(SDL2_mixer REQUIRED)
  #find_package(SDL2_image REQUIRED)
  #find_package(SDL2_gfx REQUIRED)
  #find_package(SDL2_ttf REQUIRED)

  list(APPEND _OW_COMPILE_DEF "SDL_EMULATION" "OW_BD_EMU")
  list(APPEND _OW_INCLUDE 
    ${CMAKE_CURRENT_LIST_DIR}/lib/lcdgfx/tools/sdl
    ${SDL2_INCLUDE_DIRS}
  )
  list(APPEND _OW_LINK 
    "ssd1306_sdl"
    ${SDL2_LIBRARIES}
  )
	#LDLIBS += -L/mingw/lib)
endif()

if(OW_BOARD STREQUAL "esp32")
  message(CHECK_PASS "ESP32")
  set(OW_DETECT_BOARD True)

  list(APPEND _OW_COMPILE_DEF "OW_BD_ESP32" "OW_BG_ARDUINO")
  list(APPEND _OW_LINK "pigpio")
endif()

IF(NOT OW_DETECT_BOARD)
  message(CHECK_FAIL "not found")
endif()


### Build ###

find_package(Threads REQUIRED)
#find_package(stdc++ REQUIRED)
#find_package(rt REQUIRED)

include(CTest)
enable_testing()

add_executable(${OW_TARGET} ${OPENWINCH_SRC})

set_target_properties(${OW_TARGET} PROPERTIES
  C_STANDARD 99
  C_STANDARD_REQUIRED YES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

target_link_libraries(${OW_TARGET} PRIVATE ${_OW_LINK} )
target_link_directories(${OW_TARGET} PRIVATE ${_OW_LINK_DIR} )
target_link_options(${OW_TARGET} PRIVATE  ${_OW_LINK_OPT} )
target_include_directories(${OW_TARGET} PRIVATE ${_OW_INCLUDE} )
target_compile_options(${OW_TARGET} PRIVATE ${_OW_COMPILE_OPT} )
target_compile_definitions(${OW_TARGET} PRIVATE ${_OW_COMPILE_DEF} )


### Pack ###

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
