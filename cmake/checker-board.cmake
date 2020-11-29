
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
    list(POP_BACK CMAKE_MESSAGE_INDENT)

    list(APPEND _OW_COMPILE_DEF "OW_BG_PIGPIO")
    list(APPEND _OW_LINK "pigpio")

    add_subdirectory(lib/pigpio)
    list(APPEND _OW_LINK pigpio)
	else()
    message(CHECK_PASS "Support WIRINGPI.")
    list(POP_BACK CMAKE_MESSAGE_INDENT)

    list(APPEND _OW_COMPILE_DEF "OW_BG_WIRINGPI")
    list(APPEND _OW_LINK "wiringPi")
	endif()
endif()

if(OW_BOARD STREQUAL "emulator")
  message(CHECK_PASS "Emulator")
  set(OW_DETECT_BOARD True) 

  list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules/SDL2/)
  find_package(SDL2 REQUIRED)
  #find_package(SDL2_net REQUIRED)
  #find_package(SDL2_mixer REQUIRED)
  #find_package(SDL2_image REQUIRED)
  #find_package(SDL2_gfx REQUIRED)
  #find_package(SDL2_ttf REQUIRED)

  list(APPEND _OW_COMPILE_DEF "SDL_EMULATION" "OW_BD_EMU")
  list(APPEND _OW_INCLUDE 
    ${PROJECT_SOURCE_DIR}/lib/lcdgfx/tools/sdl
    ${SDL2_INCLUDE_DIRS}
  )
  #"ssd1306_sdl"
  list(APPEND _OW_LINK 
    "ssd1306_sdl"
    ${SDL2_LIBRARIES}
  )
  #LDLIBS += -L/mingw/lib)
  set(SDL_EMULATION True)
endif()

if(OW_BOARD STREQUAL "esp32")
  message(CHECK_PASS "ESP32")
  set(OW_DETECT_BOARD True)

  list(APPEND _OW_COMPILE_DEF "OW_BD_ESP32" "OW_BG_ARDUINO")
  list(APPEND _OW_LINK "pigpio")
endif()

if(NOT OW_DETECT_BOARD)
  message(CHECK_FAIL "not found")
endif()
