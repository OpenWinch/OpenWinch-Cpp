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

//#define OW_BG_DEBUG

class Logger;
class InputDevice;
class OutputDevice;
class PWMOutputDevice;
class Tachometer;

class Raspberrypi : virtual public Board {
 public:
  explicit Raspberrypi(Winch*);
  void initialize() override;
  void emergency() override;
  void setThrottleValue(uint8_t) override;
  uint8_t getThrottleValue() override;
  void setReverse(bool) override;
  void setSpeedMode(SpeedMode) override;
  uint8_t getBattery() override;

 protected:
  Logger *logger = nullptr;
  Tachometer *tacho = nullptr;
  OutputDevice *power_cmd = nullptr;
  OutputDevice *reverse_cmd = nullptr;
  OutputDevice *speed_cmd = nullptr;
  PWMOutputDevice *throttle_cmd = nullptr;
  InputDevice *key_enter_btn = nullptr;
  InputDevice *key_left_btn = nullptr;
  InputDevice *key_right_btn = nullptr;
  uint32_t last_tick_btn;

  bool debounced(uint32_t);
  void pressedEnter(int, int, uint32_t);
  void pressedLeft(int, int, uint32_t);
  void pressedRight(int, int, uint32_t);

};

#endif  // HARDWARE_PI_HPP_
