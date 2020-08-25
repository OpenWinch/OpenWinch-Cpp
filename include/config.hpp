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
/* DEBUG       */
/***************/

// Debug Tachometer
#define OW_TA_DEBUG

// Debug Bridge
//#define OW_BG_DEBUG

// Debug Logger
#define OW_LOG_DEBUG

/***************/
/* CONFIG      */
/***************/

// Build for board type
//#define OW_BOARD "openwinch.hardwarePi.RaspberryPi"
#define OW_BOARD "openwinch.hardware.Emulator"

// Default mode
#define OW_MODE "ModeType.OneWay"

// Build Display type
#define OW_GUI "DISABLE"           /* Disable GUI */
// #define OW_GUI "SH1106_I2C"     /* OLED SH1106 on I2C */
// #define OW_GUI "EMULATOR"       /* Emulator with SDL */
// #define OW_GUI "CAPTURE"        /* Use Emulator to make capture */

/***************/
/* BUILD       */
/***************/

// Build Board
//#define OW_BD_PI        /* RaspberryPI board */
#define OW_BD_EMU         /* Emulator (PC) board */
//#define OW_BD_ESP32     /* ESP32 board (Testing) */

// Build GPIO framework
#define OW_BG_PIGPIO       /* PiGpio framework */
// #define OW_BG_WIRINGPI  /* WiringPI framework (Deprecated) */
//#define OW_BG_ARDUINO

// Build Logger framework
#define OW_LOG_SLOG       /* slog framework */
//#define OW_LOG_SPDLOG   /* spdlog framework (Testing)*/

#endif  // CONFIG_HPP_
