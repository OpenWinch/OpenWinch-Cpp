/**
 * @file hardware_pi.hpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */

#ifndef HARDWARE_PI_HPP_
#define HARDWARE_PI_HPP_

#include "hardware.hpp"

class Logger;

class InputDevice {
 public:
  explicit InputDevice(uint8_t);
  void when_pressed(void*);
 protected:
  uint8_t pin;
  static void interruptEdge(InputDevice*);
};

class OutputDevice {
 public:
  explicit OutputDevice(uint8_t);
  void on();
  void off();
 protected:
  uint8_t pin;
};

#define PWM_HIGH 1024
#define PWM_LOW 0
// BCM_GPIPO 18
#define PWM_HARDWARE_PIN 1

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

class Raspberrypi : virtual public Board {
 public:
  explicit Raspberrypi(Winch*);

 protected:
  Logger *logger = nullptr;
  OutputDevice *power_cmd;
  OutputDevice *reverse_cmd;
  OutputDevice *speed_cmd;
  PWMOutputDevice *throttle_cmd;
  InputDevice *key_enter_btn;
  InputDevice *key_left_btn;
  InputDevice *key_right_btn;

};

#endif  // HARDWARE_PI_HPP_
