TARGET_EXEC ?= openwinch
BUILD_DIR ?= ./build

## Board
# Can be emulator, raspberry, esp32
BOARD ?= raspberry
GPIO  ?= pigpio
GUI   ?= SH1106_I2C


## Compilator & Linker
# C and C++ program compilers. Un-comment and specify for cross-compiling if needed. 
#CC 					?= gcc
#CC 						= clang-9

# CXX is the C++ compiler
#CXX    				?= g++
#CXX    					= clang++-9

# Un-comment the following line to compile C programs as C++ ones.
#CC						?= $(CXX)

CFLAGS 					+= --std=c99 -Og -ffunction-sections -fdata-sections

#CXXFLAGS is the flags for the C++ compiler
CXXFLAGS				+= -std=c++17 -ffreestanding -Og -ffunction-sections -fdata-sections -fpic

# CPPFLAGS is the flags for the preprocessor (they are common between C and C++ in gnu make)
# The C Preprocessor options (notice here "CPP" does not mean "C++"; man cpp for more info.). Actually $(INCLUDE) is included. 
CPPFLAGS				+= -Wall -Wextra -Wpedantic -Wconversion -MMD -MP -Wno-multichar
CPPFLAGS				+= -g
# clang 
#CPPFLAGS				+= -Wno-reserved-user-defined-literal

# LDFLAGS are the flags for linking
# The options used in linking as well as in any direct use of ld.
# LDFLAGS					=

# TARGET_ARCH allows to specify a target architecture
# TARGET_ARCH				=

# LDLIBS is the libraries to link
# define any directories containing header files other than /usr/include /usr/lib
LDLIBS = -lstdc++ -lm -lpthread -lrt
LDLIBS += -L lib/slog/src -lslog
LDLIBS += -L lib/lcdgfx/bld -llcdgfx

INCLUDES += -I./include -I./lib/slog/src -I./lib/lcdgfx/src -I./lib/cpp-httplib

SRCS := $(wildcard src/*.c) \
		$(wildcard src/*.cpp) \
		$(wildcard src/fonts/*.cpp) \
		$(wildcard src/pages/*.cpp) \
		$(wildcard src/emulator/*.cpp) \
		$(wildcard src/raspberry/*.cpp) \
		$(wildcard src/arduino/*.cpp)

OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)TARGET_ARCH

ifeq ($(BOARD), raspberry)
	#SRCS := $(wildcard src/raspberry/*.cpp)
	INCLUDES += -DOW_BD_PI

	ifeq ($(GPIO), pigpio)
		INCLUDES += -DOW_BG_PIGPIO
		LDLIBS += -lpigpio
	else
		INCLUDES += -DOW_BG_WIRINGPI
		LDLIBS += -lwiringPi
	endif
endif

ifeq ($(BOARD), emulator)
	#SRCS := $(wildcard src/emulator/*.cpp)
	INCLUDES += -I./lib/lcdgfx/tools/sdl -DSDL_EMULATION -DOW_BD_EMU
	LDLIBS += -L/mingw/lib -lssd1306_sdl $(shell sdl2-config --libs)
endif

ifeq ($(BOARD), esp32)
	#SRCS := $(wildcard src/arduino/*.cpp)
	INCLUDES += -DOW_BD_ESP32 -DOW_BG_ARDUINO
	LDLIBS += -lpigpio
endif


all:    $(BUILD_DIR)/$(TARGET_EXEC)
		@cp -r public $(BUILD_DIR)/public
		@cp src/raspberry/slog.cfg $(BUILD_DIR)/slog.cfg
		@echo  OpenWinch is compiled

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS) 
		$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS)

# assembly
$(BUILD_DIR)/%.s.o: %.s
		$(MKDIR_F)
		$(AS) $(ASFLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
		$(MKDIR_F)
		$(CC) $(CPPFLAGS) $(CFLAGS) $(INCLUDES) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
		$(MKDIR_F)
		$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
		$(RM) -R $(BUILD_DIR)

depend: $(SRCS)
		makedepend $(INCLUDES) $^

.PHONY: all clean depend

# Add dependency files, if they exist
-include $(DEPS)

MKDIR_P ?= mkdir -p
MKDIR_F ?= @mkdir -p $(@D)

# DO NOT DELETE THIS LINE -- make depend needs it
