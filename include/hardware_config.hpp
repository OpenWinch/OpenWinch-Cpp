/*
  hardware_config.h - OpenWinch Project

  Copyright (C) 2020  Mickael Gaillard
*/

#ifndef HARDWARE_CONFIG_HPP_
#define HARDWARE_CONFIG_HPP_

// I2C (not use by GPIO, but direct acces)
//#define I2C_SDA 2
//#define I2C_SLC 3

// UART (not use by GPIO, but direct acces)
//#define UART_TX 14
//#define UART_RX 15

// Throttle
#define OUT_THROTTLE 18

// Enter
#define IN_KEY_ENTER 7

// Move Left
#define IN_KEY_LEFT 25
#define IN_KEY_RIGHT 8

// Reverse
#define OUT_REVERSE 9

// Speed Mode
#define OUT_SPD 11
#define OUT_PWR 10

// Tacho Hall
#define IN_HS_W 23
#define IN_HS_V 4
#define IN_HS_U 24

// LCD
#define LCD_ADDR 0x3c


#endif  // HARDWARE_CONFIG_HPP_