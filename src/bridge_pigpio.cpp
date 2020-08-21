/**
 * @file bridge_pigpio.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */

#include "bridge_io.hpp"
#include "config.hpp"

#ifdef OW_BG_PIGPIO

#include <mutex>
#include <pigpio.h>

#ifdef OW_BG_DEBUG
#include <string>
#include <iostream>
#endif

std::once_flag init_flag;

void init_pigpio()
{
    std::call_once(init_flag, [](){
#ifdef OW_BG_DEBUG
      std::cout << "IO: PiGPIO ver " << std::to_string(gpioVersion()) << std::endl;
      std::cout << "IO: Hardware ver " << std::to_string(gpioHardwareRevision()) << std::endl;
#endif
      int ret = gpioInitialise();
      if (ret < 0) { throw ret; }
    });
}

void terminate_gpio() {
#ifdef OW_BG_DEBUG
      std::cout << "IO: PiGPIO free. " << std::endl;
#endif
  gpioTerminate();
}

OutputDevice::OutputDevice(uint8_t _pin): pin(_pin) {
  init_pigpio();

  int ret = gpioSetMode(this->pin, PI_OUTPUT);
  if (ret < 0) { throw ret; }

  // ret = gpioSetPullUpDown(this->pin, PI_PUD_UP);
  // if (ret < 0) { throw ret; }
}

void OutputDevice::on() {
  int ret = gpioWrite(this->pin, PI_ON);
  if (ret < 0) { throw ret; }
}

void OutputDevice::off() {
  int ret = gpioWrite(this->pin, PI_OFF);
  if (ret < 0) { throw ret; }
}




PWMOutputDevice::PWMOutputDevice(uint8_t _pin): pin(_pin) {
  init_pigpio();

//   if (this->pin == PWM_HARDWARE_PIN) {
//     gpioHardwarePWM(this->pin, PWM_OUTPUT);
//   } else {
//     gpioPWM(this->pin, PWM_LOW, PWM_HIGH);
//   }
  // pullUpDnControl(this->pin, PUD_UP);
}

void PWMOutputDevice::on() {
  this->value = 1;

  int ret = -1;
  if (this->pin == PWM_HARDWARE_PIN) {
    ret = gpioHardwarePWM(this->pin, 125000000, PWM_HIGH);
  } else {
    ret = gpioPWM(this->pin, PWM_HIGH);
  }
  if (ret < 0) { throw ret; }
}

void PWMOutputDevice::off() {
  this->value = 0;

  int ret = -1;
  if (this->pin == PWM_HARDWARE_PIN) {
    ret = gpioHardwarePWM(this->pin, 0, PWM_LOW);
  } else {
    ret = gpioPWM(this->pin, PWM_LOW);
  }
  if (ret < 0) { throw ret; }
}

void PWMOutputDevice::setValue(float _value) {
  // Assertion
  if (_value > 1) { _value = 1; }
  if (_value < 0) { _value = 0; }
  // Affectation
  this->value = _value;
  // Action
  uint16_t lvalue = PWM_HIGH * value;

  int ret = -1;
  if (this->pin == PWM_HARDWARE_PIN) {
    ret = gpioHardwarePWM(this->pin, 125000000, lvalue);
  } else {
    ret = gpioPWM(this->pin, lvalue);
  }
  if (ret < 0) { throw ret; }
}

float PWMOutputDevice::getValue() {
  return this->value;
}




InputDevice::InputDevice(uint8_t _pin, uint8_t pull, bool inverse): pin(_pin) {
  init_pigpio();

  // Set mode (if not good)
  int ret = gpioGetMode(this->pin);
  if (ret != PI_INPUT) {
    ret = gpioSetMode(this->pin, PI_INPUT);
    if (ret < 0) { throw ret; }
  }

  // Set Pull resistor
  ret = gpioSetPullUpDown(this->pin, pull);
  if (ret < 0) { throw ret; }

  // Set Interupt
  ret = gpioSetAlertFuncEx(this->pin, interruptEdge, this);
  if (ret < 0) { throw ret; }
}

InputDevice::~InputDevice() {
  int ret = gpioSetAlertFuncEx(this->pin, 0, this);
  if (ret < 0) { throw ret; }
}

uint8_t InputDevice::digitalRead() {
  int result = gpioRead(this->pin);
  if (result < 0) { throw result; }
  return result;
}

void InputDevice::interruptEdge(int gpio, int level, uint32_t tick, void *data)
{
#ifdef OW_BG_DEBUG
  std::cout << "IO : interrupt GPIO :" << std::to_string(gpio) << " Level :" << std::to_string(level) << std::endl;
#endif
  const InputDevice *instance = reinterpret_cast<InputDevice *>(data);

  if (instance != nullptr) {
    if (level == PI_HIGH && instance->releasedEvent != nullptr) {
      instance->releasedEvent(gpio, level, tick);
    } else

    if (level == PI_LOW && instance->pressedEvent != nullptr) {
      instance->pressedEvent(gpio, level, tick);
    }
  }
}

void InputDevice::when_pressed(const cb_t &callback) {
#ifdef OW_BG_DEBUG
  std::cout << "IO: Set pressed callback : " << std::endl;  // reinterpret_cast<unsigned char *>(callback) << std::endl;
#endif

  this->pressedEvent = callback;
}

void InputDevice::when_released(const cb_t &callback) {
#ifdef OW_BG_DEBUG
  std::cout << "IO: Set released callback : " << std::endl;  // reinterpret_cast<unsigned char *>(callback) << std::endl;
#endif

  this->releasedEvent = callback;
}

#endif
