/**
 * @file bridge_.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */

#include "openwinch.hpp"
#include "bridge_io.hpp"

#ifdef OW_BD_PI
#ifdef OW_BG_WIRINGPI

#ifdef __cplusplus
extern "C" {
#endif
#include <wiringPi.h>
#include <softPwm.h>
#ifdef __cplusplus
}
#endif

std::once_flag init_flag;

void Device::init_pigpio() {
    std::call_once(init_flag, [](){
#ifdef OW_BG_DEBUG

#endif  // OW_BG_DEBUG
    });
}

void Device::terminate_gpio() {
#ifdef OW_BG_DEBUG

#endif  // OW_BG_DEBUG
}


OutputDevice::OutputDevice(uint8_t _pin): pin(_pin) {
  Device::init_pigpio();

  pinMode(this->pin, OUTPUT);
  // pullUpDnControl(this->pin, PUD_UP);
}

void OutputDevice::on() {
  digitalWrite(this->pin, HIGH);
}

void OutputDevice::off() {
  digitalWrite(this->pin, LOW);
}



PWMOutputDevice::PWMOutputDevice(uint8_t _pin): pin(_pin) {
  Device::init_pigpio();

  if (this->pin == PWM_HARDWARE_PIN) {
    pinMode(this->pin, PWM_OUTPUT);
  } else {
    softPwmCreate(this->pin, PWM_LOW, PWM_HIGH);
  }
  // pullUpDnControl(this->pin, PUD_UP);
}

void PWMOutputDevice::on() {
  this->value = 1;

  if (this->pin == PWM_HARDWARE_PIN) {
    pwmWrite(this->pin, PWM_HIGH);
  } else {
    softPwmWrite(this->pin, PWM_HIGH);
  }
}

void PWMOutputDevice::off() {
  this->value = 0;
  if (this->pin == PWM_HARDWARE_PIN) {
    pwmWrite(this->pin, PWM_LOW);
  } else {
    softPwmWrite(this->pin, PWM_LOW);
  }
}

void PWMOutputDevice::setValue(float _value) {
  // Assertion
  if (_value > 1) { _value = 1; }
  if (_value < 0) { _value = 0; }
  // Affectation
  this->value = _value;
  // Action
  uint16_t lvalue = PWM_HIGH * value;
  pwmWrite(this->pin, lvalue);
}

float PWMOutputDevice::getValue() {
  return this->value;
}



InputDevice::InputDevice(uint8_t _pin): pin(_pin) {
  Device::init_pigpio();

  pinMode(this->pin, INPUT);
  pullUpDnControl(this->pin, PUD_UP);

//   if (wiringPiISR(this->pin, INT_EDGE_FALLING, &__challSensorU) < 0) {
//     error_exit("Unable to setup ISR U: ");
//   }
}

void InputDevice::interruptEdge(InputDevice* instance) {

}

void InputDevice::when_pressed(void*) {

}

#endif  // OW_BG_WIRINGPI
#endif  // OW_BD_PI
