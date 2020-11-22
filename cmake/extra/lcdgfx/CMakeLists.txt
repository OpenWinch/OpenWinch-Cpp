cmake_minimum_required(VERSION 3.2.0 FATAL_ERROR)

project(lcdgfx C CXX)

file(GLOB LCDGLX_SRCS 
  "src/canvas/fonts/*.c"
  "src/canvas/*.cpp"
  "src/lcd_hal/*.cpp"
  "src/lcd_hal/linux/*.cpp"
  "src/v2/*.c"
  "src/v2/lcd/*.cpp"
  "src/v2/lcd/sh1106/*.cpp"
  "src/v2/lcd/ssd1306/*.cpp"
  "src/v2/nano_engine/*.cpp"
)
file(GLOB_RECURSE LCDGLX_HEADERS "src/*.h" "src/*.hpp" )

list(APPEND _OW_COMPILE_OPT
  -Wall
  -Wextra
  -Wpedantic
  -Wconversion
  -MD
  -Wno-multichar
  -ffunction-sections
  -fdata-sections
  $<$<COMPILE_LANGUAGE:CXX>:${BUILD_FLAGS_FOR_CXX}> -ffreestanding
  $<$<COMPILE_LANGUAGE:CXX>:${BUILD_FLAGS_FOR_CXX}> -fpic
)

list(APPEND OW_PROPERTIES
  C_STANDARD 99
  C_STANDARD_REQUIRED YES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

add_library(lcdgfx STATIC ${LCDGLX_SRCS} ${LCDGLX_HEADERS})
set_target_properties(lcdgfx PROPERTIES ${OW_PROPERTIES})
target_include_directories(lcdgfx PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src
    ${CMAKE_CURRENT_LIST_DIR}/src/v2
    ${CMAKE_CURRENT_LIST_DIR}/tools/sdl
)
target_compile_options(lcdgfx PRIVATE ${_OW_COMPILE_OPT})

#debug
if(SDL_EMULATION)
    target_compile_definitions(lcdgfx PRIVATE "SDL_EMULATION" )
    add_subdirectory(tools/sdl)
endif()















# idf_component_register(
#     SRCS
#         ./src/canvas/fonts/fonts.c
#         ./src/canvas/canvas.cpp
#         ./src/canvas/font.cpp
#         ./src/lcd_hal/esp/esp32_spi.cpp
#         ./src/lcd_hal/esp/esp8266_spi.cpp
#         ./src/lcd_hal/esp/esp32_i2c.cpp
#         ./src/lcd_hal/esp/esp8266_i2c.cpp
#         ./src/lcd_hal/esp/platform.cpp
#         ./src/lcd_hal/interface.c
#         ./src/v2/lcd/ili9341/lcd_ili9341.cpp
#         ./src/v2/lcd/lcd_common.cpp
#         ./src/v2//nano_engine/core.cpp
#         ./src/v2/ssd1306_utf8.c
#     INCLUDE_DIRS
#         ./src
# )

# # This is Makefile for ESP32 IDF

# COMPONENT_ADD_INCLUDEDIRS := ./src \

# COMPONENT_SRCDIRS := ./src \
#                      ./src/lcd_hal \
#                      ./src/lcd_hal/esp \
#                      ./src/v2 \
#                      ./src/v2/lcd \
#                      ./src/v2/lcd/ili9341 \
#                      ./src/canvas \
#                      ./src/canvas/fonts \
#                      ./src/v2/nano_engine \
