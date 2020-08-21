/**
 * @file bridge_pigpio.hpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */

#ifndef BRIDGE_PIGPIO_HPP_
#define BRIDGE_PIGPIO_HPP_

#include <functional>
#include <cinttypes>

#define PWM_HIGH 255
#define PWM_LOW 0
// BCM_GPIPO 18
#define PWM_HARDWARE_PIN 1
#define PI_PUD_UP 2

typedef std::function<void(int, int, uint32_t)> cb_t;

class InputDevice {
 public:
  explicit InputDevice(uint8_t _pin, uint8_t pull = PI_PUD_UP, bool inverse = false);
  ~InputDevice();
  uint8_t digitalRead();
  void when_pressed(const cb_t &);
  void when_released(const cb_t &);
 protected:
  uint8_t pin;
  cb_t pressedEvent;
  cb_t releasedEvent;
  static void interruptEdge(int, int, uint32_t, void *);
};

class OutputDevice {
 public:
  explicit OutputDevice(uint8_t);
  void on();
  void off();
 protected:
  uint8_t pin;
};

class PWMOutputDevice {
 public:
  explicit PWMOutputDevice(uint8_t);
  void on();
  void off();
  void setValue(float);
  float getValue();
 protected:
  uint8_t pin;
  float value;
};

#endif  // BRIDGE_PIGPIO_HPP_