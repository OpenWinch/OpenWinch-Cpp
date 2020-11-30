/**
 * @file bridge_pigpio.hpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */

#ifndef BRIDGE_IO_HPP_
#define BRIDGE_IO_HPP_

#include <functional>
#include <cinttypes>

#define PWM_HIGH 255
#define PWM_LOW 0
// BCM_GPIPO 18
#define PWM_HARDWARE_PIN 1
#define PI_PUD_UP 2

typedef std::function<void(int, int, uint32_t)> cb_t;

class Device {
 public:
  static void init_gpio();
  static void terminate_gpio();
 private:
  Device();
  ~Device()= default;
  Device(const Device&)= delete;
  Device& operator=(const Device&)= delete;
};

class InputDevice {
 public:
  explicit InputDevice(uint8_t _pin, uint8_t _pull = PI_PUD_UP, bool _inverse = false);
  // InputDevice(const InputDevice&) = 0;
  virtual ~InputDevice();
  uint8_t digitalRead();
  virtual void when_pressed(const cb_t &);
  virtual void when_released(const cb_t &);
 protected:
  uint8_t pin;
  uint8_t pull;
  bool inverse;
  cb_t pressedEvent;
  cb_t releasedEvent;
  static void interruptEdge(int, int, uint32_t, void *);
};

class OutputDevice {
 public:
  explicit OutputDevice(uint8_t _pin, uint8_t _pull = PI_PUD_UP, bool _inverse = false);
  // OutputDevice(const OutputDevice&) = 0;
  virtual ~OutputDevice() = default;
  void on();
  void off();
 protected:
  uint8_t pin;
  uint8_t pull;
  bool inverse;
};

class PWMOutputDevice {
 public:
  explicit PWMOutputDevice(uint8_t _pin, uint8_t _pull = PI_PUD_UP, bool _inverse = false);
  // PWMOutputDevice(const PWMOutputDevice&) = 0;
  virtual ~PWMOutputDevice() = default;
  void on();
  void off();
  void setValue(float);
  float getValue();
 protected:
  uint8_t pin;
  uint8_t pull;
  bool inverse;
  float value;
};

#endif  // BRIDGE_IO_HPP_
