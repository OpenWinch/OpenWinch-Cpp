/**
 * @file hardwarepi.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020
 */

#include "hardware_pi.hpp"
#include "hardware_config.hpp"
#include "logger.hpp"

#include <wiringPi.h>
#include <softPwm.h>

Raspberrypi::Raspberrypi(Winch *winch) : Board(winch) {
  this->logger = &Logger::get();
  this->logger->debug("IO : Initialize Raspberry...");

  // Power
  this->power_cmd = new OutputDevice(OUT_PWR);
  this->power_cmd->off();

  // Reverse
  this->reverse_cmd = new OutputDevice(OUT_REVERSE);

  // Speed mode (Lo, Medium, Hi)
  this->speed_cmd = new OutputDevice(OUT_SPD);

  // Throlle
  this->throttle_cmd = new PWMOutputDevice(OUT_THROTTLE);

  // Move
  this->key_enter_btn = new InputDevice(IN_KEY_ENTER);
  this->key_left_btn = new InputDevice(IN_KEY_LEFT);
  this->key_right_btn = new InputDevice(IN_KEY_RIGHT);

  // Register event
//   this->key_enter_btn.when_pressed = this->pressedEnter;
//   this->key_left_btn = this->pressedLeft;
//   this->key_right_btn = this->pressedRight;
}



OutputDevice::OutputDevice(uint8_t _pin): pin(_pin) {
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
