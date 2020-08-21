/**
 * @file config.hpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */

#ifndef CONFIG_HPP_
#define CONFIG_HPP_

// Build for board type
#define OW_BOARD "openwinch.hardwarePi.RaspberryPi"
// #define OW_BOARD openwinch.hardwarePi.RaspberryPi

// Default mode
#define OW_MODE "ModeType.OneWay"

// Build Display type
#define OW_GUI "SH1106_I2C"
// #define OW_GUI EMULATOR

// Define Framework of GPIO
//#define OW_BG_DEBUG

// PiGpio Framework
#define OW_BG_PIGPIO

// WiringPI Framework
// #define OW_BG_WIRINGPI

#endif  // CONFIG_HPP_