/**
 * @file bridge_pigpio.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */

#include "openwinch.hpp"
#include "bridge_io.hpp"

#ifdef OW_BD_PI
#ifdef OW_BG_PIGPIO

#ifdef __cplusplus
extern "C" {
#endif
#include <pigpio.h>
#ifdef __cplusplus
}
#endif

#include <mutex>

#ifdef OW_BG_DEBUG
#include <string>
#include <iostream>
#endif  // OW_BG_DEBUG

std::once_flag init_flag;

void Device::init_gpio() {
    std::call_once(init_flag, [](){
#ifdef OW_BG_DEBUG
      std::cout << "IO: PiGPIO ver " << std::to_string(gpioVersion()) << std::endl;
      std::cout << "IO: Hardware ver " << std::to_string(gpioHardwareRevision()) << std::endl;
#endif  // OW_BG_DEBUG
      // gpioCfgBufferSize (PI_DEFAULT_BUFFER_MILLIS);
      // gpioCfgClock (PI_DEFAULT_CLK_MICROS, PI_DEFAULT_CLK_PERIPHERAL, 0);
      // gpioCfgInterfaces (PI_DEFAULT_IF_FLAGS);
      // gpioCfgDMAchannels (PI_DEFAULT_DMA_PRIMARY_CHANNEL, PI_DEFAULT_DMA_SECONDARY_CHANNEL);
      // gpioCfgSocketPort (PI_DEFAULT_SOCKET_PORT);
      // gpioCfgMemAlloc (PI_DEFAULT_MEM_ALLOC_MODE);
      // gpioCfgSetInternals (PI_DEFAULT_CFG_INTERNALS);
      int ret = gpioInitialise();
      if (ret < 0) { throw ret; }
    });
}

void Device::terminate_gpio() {
#ifdef OW_BG_DEBUG
  std::cout << "IO: PiGPIO free. " << std::endl;
#endif  // OW_BG_DEBUG
  gpioTerminate();
}

/**
 * @brief Construct a new Output Device:: Output Device object
 * 
 * @param _pin GPIO pin.
 * @param _pull Pull resistor up or down.
 * @param _inverse Inverse state.
 */
OutputDevice::OutputDevice(uint8_t _pin, uint8_t _pull, bool _inverse) :
    pin(_pin),
    pull(_pull),
    inverse(_inverse) {
  Device::init_gpio();

  // Set mode (if not good)
  int ret = gpioGetMode(this->pin);
  if (ret != PI_OUTPUT) {
    ret = gpioSetMode(this->pin, PI_OUTPUT);
    if (ret < 0) { throw ret; }
  }

  // Set Pull resistor
  ret = gpioSetPullUpDown(this->pin, this->pull);
  if (ret < 0) { throw ret; }
}

/**
 * @brief Change state of GPIO to ON/HIGH
 */
void OutputDevice::on() {
  int ret = gpioWrite(this->pin, PI_ON);
  if (ret < 0) { throw ret; }
}

/**
 * @brief Change state of GPIO to OFF/LOW
 */
void OutputDevice::off() {
  int ret = gpioWrite(this->pin, PI_OFF);
  if (ret < 0) { throw ret; }
}



/**
 * @brief Construct a new PWMOutputDevice::PWMOutputDevice object
 * 
 * @param _pin GPIO pin.
 * @param _pull Pull resistor up or down.
 * @param _inverse Inverse state.
 */
PWMOutputDevice::PWMOutputDevice(uint8_t _pin, uint8_t _pull, bool _inverse) :
    pin(_pin),
    pull(_pull),
    inverse(_inverse) {
  Device::init_gpio();

//   if (this->pin == PWM_HARDWARE_PIN) {
//     gpioHardwarePWM(this->pin, PWM_OUTPUT);
//   } else {
//     gpioPWM(this->pin, PWM_LOW, PWM_HIGH);
//   }
  // pullUpDnControl(this->pin, PUD_UP);
}

/**
 * @brief Change state of GPIO to ON/HIGH
 */
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

/**
 * @brief Change state of GPIO to OFF/LOW
 */
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

/**
 * @brief Change value of GPIO 
 * 
 * @param _value 
 */
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



/**
 * @brief Construct a new Input Device object
 * 
 * @param _pin GPIO pin.
 * @param _pull Pull resistor up or down.
 * @param _inverse Inverse state.
 */
InputDevice::InputDevice(uint8_t _pin, uint8_t _pull, bool _inverse) :
    pin(_pin),
    pull(_pull),
    inverse(_inverse) {

  Device::init_gpio();

  // Set mode (if not good)
  int ret = gpioGetMode(this->pin);
  if (ret != PI_INPUT) {
    ret = gpioSetMode(this->pin, PI_INPUT);
    if (ret < 0) { throw ret; }
  }

  // Set Pull resistor
  ret = gpioSetPullUpDown(this->pin, this->pull);
  if (ret < 0) { throw ret; }

  // Set Interupt
  ret = gpioSetAlertFuncEx(this->pin, interruptEdge, this);
  if (ret < 0) { throw ret; }
}

/**
 * @brief Destroy the Input Device object
 */
InputDevice::~InputDevice() {
  int ret = gpioSetAlertFuncEx(this->pin, 0, this);
  if (ret < 0) { throw ret; }
}

/**
 * @brief Read state of GPIO
 * 
 * @return uint8_t State PI_ON or PI_OFF
 */
uint8_t InputDevice::digitalRead() {
  int result = gpioRead(this->pin);
  if (result < 0) { throw result; }
  return (uint8_t)result;
}

void InputDevice::interruptEdge(int gpio, int level, uint32_t tick, void *data)
{
#ifdef OW_BG_DEBUG
  std::cout << "IO : interrupt GPIO :" << std::to_string(gpio) << " Level :" << std::to_string(level) << std::endl;
#endif  // OW_BG_DEBUG
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
#endif  // OW_BG_DEBUG

  this->pressedEvent = callback;
}

void InputDevice::when_released(const cb_t &callback) {
#ifdef OW_BG_DEBUG
  std::cout << "IO: Set released callback : " << std::endl;  // reinterpret_cast<unsigned char *>(callback) << std::endl;
#endif  // OW_BG_DEBUG

  this->releasedEvent = callback;
}

#endif  // OW_BG_PIGPIO
#endif  // OW_BD_PI
