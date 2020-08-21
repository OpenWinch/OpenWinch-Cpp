/**
 * @file config.hpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */

#ifndef CONFIG_HPP_
#define CONFIG_HPP_

/***************/
/* HARD CONFIG */
/***************/

// Build for board type
#define OW_BOARD "openwinch.hardwarePi.RaspberryPi"
// #define OW_BOARD openwinch.hardwarePi.RaspberryPi

// Default mode
#define OW_MODE "ModeType.OneWay"

// Build Display type
#define OW_GUI "SH1106_I2C"     /* OLED SH1106 on I2C */
// #define OW_GUI "EMULATOR"       /* Emulator with SDL */
// #define OW_GUI "CAPTURE"        /* Use Emulator to make capture */


/***************/
/* GPIO CONFIG */
/***************/

// Debug Bridge
// #define OW_BG_DEBUG

// Define Framework to use
#define OW_BG_PIGPIO    /* PiGpio Framework */
// #define OW_BG_WIRINGPI  /* WiringPI Framework (Deprecated) */

#endif  // CONFIG_HPP_
