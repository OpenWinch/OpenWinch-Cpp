/**
 * @file bridge_emulator.cpp
 * @author Mickael GAILLARD (mick.gaillard@gmail.com)
 * @brief OpenWinch Project
 * 
 * @copyright Copyright © 2020-2021
 */

#include "openwinch.hpp"
#include "bridge_io.hpp"

#ifdef OW_BD_EMU

OutputDevice::OutputDevice(uint8_t _pin, uint8_t _pull, bool _inverse) { }
void OutputDevice::on() { }
void OutputDevice::off() { }


PWMOutputDevice::PWMOutputDevice(uint8_t _pin, uint8_t _pull, bool _inverse) { }
void PWMOutputDevice::on() { }
void PWMOutputDevice::off() { }
void PWMOutputDevice::setValue(float _value) { }
float PWMOutputDevice::getValue() {
  return 0;
}


InputDevice::InputDevice(uint8_t _pin, uint8_t _pull, bool _inverse) { }
InputDevice::~InputDevice() { }
uint8_t InputDevice::digitalRead() {
  return 0;
}
void InputDevice::interruptEdge(int gpio, int level, uint32_t tick, void *data) { }
void InputDevice::when_pressed(const cb_t &callback) { this->pressedEvent = callback; }
void InputDevice::when_released(const cb_t &callback) { this->releasedEvent = callback; }

#endif  // OW_BD_EMU
