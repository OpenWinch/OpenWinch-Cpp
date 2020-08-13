/**
 * @file hardware_pi.hpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */

#ifndef HARDWAREPI_HPP_
#define HARDWAREPI_HPP_

#include "hardware.hpp"

class Button {

};

class OutputDevice {
 public:
  void on();
  void off();
};

class PWMOutputDevice : virtual public OutputDevice {
 public:
  void setValue(float);
  float getValue();
};

class Raspberrypi : virtual public Board {
 public:
  Raspberrypi(Winch*);

 protected:
  OutputDevice power_cmd;
  OutputDevice reverse_cmd;
  OutputDevice speed_cmd;
  PWMOutputDevice throttle_cmd;
  Button key_enter_btn;
  Button key_left_btn;
  Button key_right_btn;

};

#endif  // HARDWAREPI_HPP_
