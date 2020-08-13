/*
  hardware_config.h - OpenWinch Project

  Copyright (C) 2020  Mickael Gaillard
*/

#ifndef HARDWARE_CONFIG_HPP_
#define HARDWARE_CONFIG_HPP_

// I2C
#define I2C_SDA 2
#define I2C_SLC 3

// UART
#define UART_TX 14
#define UART_RX 15

// Reverse/Init(hold)
#define IN_KEY_ENTER 4

// Move Left
#define IN_KEY_LEFT 22
#define IN_KEY_RIGHT 23

// Reverse
#define OUT_REVERSE 17

// Speed Mode
#define OUT_SPD 24
#define OUT_PWR 25

// Throttle
#define OUT_THROTTLE 18

#define LCD_WIDTH 128
#define LCD_HEIGHT 64
#define LCD_ADDR 0x3c

#define LCD_FPS 10

#endif  // HARDWARE_CONFIG_HPP_