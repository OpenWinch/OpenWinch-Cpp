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
#ifndef OW_TA_DEBUG
#define OW_TA_DEBUG
#endif  // OW_TA_DEBUG

// Debug Bridge
#ifndef OW_BG_DEBUG
// #define OW_BG_DEBUG
#endif  // OW_BG_DEBUG

// Debug Logger
#ifndef OW_LOG_DEBUG
// #define OW_LOG_DEBUG
#endif  // OW_LOG_DEBUG

/***************/
/* CONFIG      */
/***************/

// Default mode
#ifndef OW_MODE
#define OW_MODE "ModeType.OneWay"
#endif  // OW_MODE

// Build Display type
// #define OW_GUI "DISABLE"           /* Disable GUI */
#define OW_GUI "SH1106_I2C"     /* OLED SH1106 on I2C */
// #define OW_GUI "EMULATOR"       /* Emulator with SDL */
// #define OW_GUI "CAPTURE"        /* Use Emulator to make capture */

/***************/
/* BUILD       */
/***************/

// Build Board
#if !defined(OW_BD_PI) && !defined(OW_BD_EMU) && !defined(OW_BD_ESP32)
#define OW_BD_EMU          /* Emulator (PC) board */
// #define OW_BD_PI        /* RaspberryPI board */
// #define OW_BD_ESP32     /* ESP32 board (Testing) */
#endif

// Build GPIO framework
#if !defined(OW_BG_PIGPIO) && !defined(OW_BG_WIRINGPI) && !defined(OW_BG_ARDUINO)
// #define OW_BG_PIGPIO    /* PiGpio framework */
// #define OW_BG_WIRINGPI  /* WiringPI framework (Deprecated) */
// #define OW_BG_ARDUINO   /* Arduino framework */
#endif

// Build Logger framework
#if !defined(OW_LOG_SLOG) && !defined(OW_LOG_SPDLOG)
#define OW_LOG_SLOG       /* slog framework */
// #define OW_LOG_SPDLOG   /* spdlog framework (Testing)*/
#endif

#endif  // CONFIG_HPP_
