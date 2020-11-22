cmake_minimum_required(VERSION 3.2.0 FATAL_ERROR)

project(ssd1306_sdl C)


file(GLOB SDL_SRCS
  "*.c"
)

file(GLOB_RECURSE SDL_HEADERS "*.h" "*.hpp" )

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

add_library(ssd1306_sdl STATIC ${SDL_SRCS} ${SDL_HEADERS})
target_compile_definitions(ssd1306_sdl PRIVATE "SDL_EMULATION" )
set_target_properties(ssd1306_sdl PROPERTIES ${OW_PROPERTIES})
target_include_directories(ssd1306_sdl PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/.
)
target_compile_options(ssd1306_sdl PRIVATE ${_OW_COMPILE_OPT})